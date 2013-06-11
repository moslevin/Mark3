/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012 Funkenstein Software Consulting, all rights reserved.
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
static K_UCHAR aucAppStack[STACK_SIZE_APP];
static K_UCHAR aucIdleStack[STACK_SIZE_IDLE];

//---------------------------------------------------------------------------
static K_UCHAR aucTxBuffer[UART_SIZE_TX];
static K_UCHAR aucRxBuffer[UART_SIZE_RX];

//---------------------------------------------------------------------------
static void AppEntry(void);
static void IdleEntry(void);

static void PrintString(const K_CHAR *szStr_);
static K_CHAR cmd_dir( CommandLine_t *pstCommand_ );

//---------------------------------------------------------------------------
// Dummy command list, our shell only support the "dir" command
static ShellCommand_t astCommands[] =
{
    { "dir",    cmd_dir },
    { 0 , 0 }
};

//---------------------------------------------------------------------------
// Dir command implementation.  Since there's no cd command, assume we're
// dir'ing from the root directory "/".
static K_CHAR cmd_dir( CommandLine_t *pstCommand_ )
{
    K_USHORT usRoot;

    K_UCHAR ucCount = 0;
    K_CHAR acVal[16] = {0};

    // Get the "/" file node
    usRoot = FS_ROOT_BLOCK;
    usRoot = clNLFS.GetFirstChild(usRoot);

    // Iterate through all child nodes in the FS.
    while (usRoot && INVALID_NODE != usRoot)
    {
        NLFS_File_Stat_t stStats;
        if (clNLFS.GetStat( usRoot, &stStats))
        {
            // Print the filename and size for each file.
            PrintString((const K_CHAR*)stStats.acFileName);
            PrintString("    ");
            MemUtil::DecimalToString( stStats.ulFileSize, acVal );
            PrintString((const K_CHAR*)acVal);
            PrintString(" Bytes");
            PrintString("\n");
        }
        ucCount++;
        usRoot = clNLFS.GetNextPeer(usRoot);
    }

    // Display total number of files found
    PrintString(" Found ");
    MemUtil::DecimalToString( ucCount, acVal);
    PrintString((const K_CHAR*)acVal);
    PrintString(" Files\n");

    return 0;
}

//---------------------------------------------------------------------------
// Prepare an NLFS filesystem
static void NLFS_Prepare(void)
{
    clHost.u32Data = 0; //Format at EEPROM address 0

    clNLFS.Format(&clHost, 2048, 8, 16);

    clNLFS.Create_File("/a.txt");
    clNLFS.Create_File("/b.txt");
    clNLFS.Create_File("/c.txt");
    clNLFS.Create_File("/d.txt");
    clNLFS.Create_File("/e.txt");
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
static void PrintString(const K_CHAR *szStr_)
{
    K_CHAR *szTemp = (K_CHAR*)szStr_;
    while (*szTemp)
    {
        while( 1 != clUART.Write( 1, (K_UCHAR*)szTemp ) ) { /* Do nothing */ }
        szTemp++;
    }
}

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
    cmd_dir(0);

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

