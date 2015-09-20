/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012-2015 Funkenstein Software Consulting, all rights reserved.
See license.txt for more information
===========================================================================*/

/*
 * Test application using the shell support infrastructure and the NLFS
 * filesystem to implement a basic shell.
 */

//---------------------------------------------------------------------------

#include "kerneltypes.h"
#include "kernel.h"
#include "thread.h"
#include "drvUART.h"
#include "memutil.h"
#include "shell_support.h"
#include "nlfs.h"
#include "nlfs_file.h"
#include "nlfs_eeprom.h"

#include <avr/io.h>
#include <avr/sleep.h>

//---------------------------------------------------------------------------
// Need to have this to handle pure-virtual functions
extern "C" void __cxa_pure_virtual() {}

//---------------------------------------------------------------------------
// Global objects
static Thread AppThread;			//!< Main "application" thread
static Thread IdleThread;			//!< Idle thread - runs when app can't

static ATMegaUART clUART;			//!< UART device driver object

static NLFS_EEPROM clNLFS;
static NLFS_Host_t clHost;

//---------------------------------------------------------------------------
#define STACK_SIZE_APP		(384)	//!< Size of the main app's stack
#define STACK_SIZE_IDLE		(128)	//!< Size of the idle thread stack

//---------------------------------------------------------------------------
#define UART_SIZE_RX		(8)	//!< UART RX Buffer size
#define UART_SIZE_TX		(32)	//!< UART TX Buffer size

//---------------------------------------------------------------------------
static uint8_t aucAppStack[STACK_SIZE_APP];
static uint8_t aucIdleStack[STACK_SIZE_IDLE];

//---------------------------------------------------------------------------
static uint8_t aucTxBuffer[UART_SIZE_TX];
static uint8_t aucRxBuffer[UART_SIZE_RX];

//---------------------------------------------------------------------------
static void AppEntry(void);
static void IdleEntry(void);

static void PrintString(const char *szStr_);
static void PrintString(const char *szStr_, int iLen_);

static char cmd_dir( CommandLine_t *pstCommand_ );
static char cmd_cat( CommandLine_t *pstCommand_ );
//---------------------------------------------------------------------------
// Dummy command list, our shell only support the "dir" command
static ShellCommand_t astCommands[] =
{
    { "dir",    cmd_dir },
    { "cat",    cmd_cat },
    { 0 , 0 }
};

//---------------------------------------------------------------------------
// Dir command implementation.  Since there's no cd command, assume we're
// dir'ing from the root directory "/".
static char cmd_dir( CommandLine_t *pstCommand_ )
{
    uint16_t u16Root;

    uint8_t u8Count = 0;
    char acVal[16] = {0};

    NLFS_File_Stat_t stStats;

    // Get the "/" file node    
    u16Root = FS_ROOT_BLOCK;

    PrintString("Directory Listing For: ");
    clNLFS.GetStat(u16Root, &stStats);
    PrintString((const char*)stStats.acFileName);
    PrintString("\n");

    u16Root = clNLFS.GetFirstChild(u16Root);

    // Iterate through all child nodes in the FS.
    while (u16Root && INVALID_NODE != u16Root)
    {        
        if (clNLFS.GetStat( u16Root, &stStats))
        {
            // Print the filename and size for each file.
            PrintString("    ");
            PrintString((const char*)stStats.acFileName);
            PrintString("    ");
            MemUtil::DecimalToString( stStats.u32FileSize, acVal );
            PrintString((const char*)acVal);
            PrintString(" Bytes");
            PrintString("\n");
        }
        u8Count++;
        u16Root = clNLFS.GetNextPeer(u16Root);
    }

    // Display total number of files found
    PrintString(" Found ");
    MemUtil::DecimalToString( u8Count, acVal);
    PrintString((const char*)acVal);
    PrintString(" Files\n");

    return 0;
}

//---------------------------------------------------------------------------
// Print the contents of a file (as ascii) to the terminal
static char cmd_cat( CommandLine_t *pstCommand_ )
{    
    char acBuf[16];
    int iBytesRead;
    NLFS_File clFile;

    if (!pstCommand_->u8NumOptions)
    {
        PrintString("File Not Found\n");
        return -1;
    }

    if (INVALID_NODE == clFile.Open(&clNLFS, pstCommand_->astOptions[0].pstStart->pcToken, NLFS_FILE_READ))
    {
        PrintString("File Not Found\n");
        return -1;
    }

    while ((iBytesRead = clFile.Read((void*)acBuf, 16)) > 0)
    {
        PrintString(acBuf, iBytesRead);
    }

    clFile.Close();

    return 0;
}

//---------------------------------------------------------------------------
// Prepare an NLFS filesystem
static void NLFS_Prepare(void)
{
    NLFS_File clFile;

    clHost.u32Data = 0; //Format at EEPROM address 0

    clNLFS.Format(&clHost, 2048, 8, 16);

    clFile.Open( &clNLFS, "/a.txt", NLFS_FILE_CREATE | NLFS_FILE_WRITE );
    clFile.Write((void*)("Hello World!\n"), 13);
    clFile.Close();

    clFile.Open( &clNLFS, "/b.txt", NLFS_FILE_CREATE | NLFS_FILE_WRITE);
    clFile.Write((void*)("Hello!\n"), 7);
    clFile.Close();

    clFile.Open( &clNLFS, "/c.txt", NLFS_FILE_CREATE | NLFS_FILE_WRITE);
    clFile.Write((void*)("World!\n"), 7);
    clFile.Close();

    clFile.Open( &clNLFS, "/d.txt", NLFS_FILE_CREATE | NLFS_FILE_WRITE);
    clFile.Write((void*)("Mark3 Rulez!\n"), 13);
    clFile.Close();

    clFile.Open( &clNLFS, "/e.txt", NLFS_FILE_CREATE | NLFS_FILE_WRITE);
    clFile.Write((void*)("FunkSW!\n"), 8);
    clFile.Close();

    clFile.Open( &clNLFS, "/e.txt", NLFS_FILE_APPEND | NLFS_FILE_WRITE);
    clFile.Write((void*)("FunkSW!\n"), 8);
    clFile.Close();

}

//---------------------------------------------------------------------------
int main(void)
{
    Kernel::Init();						//!< MUST be before other kernel ops

    AppThread.Init(	aucAppStack,		//!< Pointer to the stack
                    STACK_SIZE_APP,		//!< Size of the stack
                    1,					//!< Thread priority
                    (ThreadEntry_t)AppEntry,	//!< Entry function
                    (void*)&AppThread );//!< Entry function argument

    IdleThread.Init( aucIdleStack,		//!< Pointer to the stack
                     STACK_SIZE_IDLE,	//!< Size of the stack
                     0,					//!< Thread priority
                     (ThreadEntry_t)IdleEntry,	//!< Entry function
                     NULL );			//!< Entry function argument

    AppThread.Start();					//!< Schedule the threads
    IdleThread.Start();

    clUART.SetName("/dev/tty");			//!< Add the serial driver
    clUART.Init();

    DriverList::Add( &clUART );

    Kernel::Start();					//!< Start the kernel!
}

//---------------------------------------------------------------------------
static void PrintString(const char *szStr_)
{
    char *szTemp = (char*)szStr_;
    while (*szTemp)
    {
        while( 1 != clUART.Write( 1, (uint8_t*)szTemp ) ) { /* Do nothing */ }
        szTemp++;
    }
}

//---------------------------------------------------------------------------
static void PrintString(const char *szStr_, int iLen_)
{
    char *szTemp = (char*)szStr_;
    while (*szTemp && iLen_)
    {
        while( 1 != clUART.Write( 1, (uint8_t*)szTemp ) ) { /* Do nothing */ }
        szTemp++;
        iLen_--;
    }
}

static Token_t astTokens[12];
static CommandLine_t stCommand;
//---------------------------------------------------------------------------
void AppEntry(void)
{
    {
        Driver *my_uart = DriverList::FindByPath("/dev/tty");

        my_uart->Control( CMD_SET_BUFFERS, aucRxBuffer, UART_SIZE_RX,
                                             aucTxBuffer, UART_SIZE_TX);
        my_uart->Open();
    }

    NLFS_Prepare();

    // Set up a pre-seeded command to execute in the shell
    {
        const char *szCmd = "cat /e.txt"; //dir /";
        uint8_t u8NumTokens;


        u8NumTokens = MemUtil::Tokenize(szCmd, astTokens, 12);
        ShellSupport::TokensToCommandLine(astTokens, u8NumTokens, &stCommand);
        ShellSupport::RunCommand(&stCommand, astCommands);
    }

    while(1)
    {
        Thread::Sleep(1000);
    }
}

//---------------------------------------------------------------------------
void IdleEntry(void)
{
    while(1)
    {
        // LPM code;
        set_sleep_mode(SLEEP_MODE_IDLE);
        cli();
        sleep_enable();
        sei();
        sleep_cpu();
        sleep_disable();
        sei();
    }
}

