/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012 - 2017 Funkenstein Software Consulting, all rights reserved.
See license.txt for more information
===========================================================================*/
#include "mark3.h"
#include "drvUARTplus.h"
#include "m3shell.h"
#include "memutil.h"
#include "nlfs.h"
#include "nlfs_file.h"
#include "nlfs_eeprom.h"

using namespace Mark3;

extern "C" {
void __cxa_pure_virtual(void)
{
}
}

#define UART_SIZE_TX 32
#define UART_SIZE_RX 32

//---------------------------------------------------------------------------
// This block declares the thread data for the main application thread.  It
// defines a thread object, stack (in word-array form), and the entry-point
// function used by the application thread.
#define APP_STACK_SIZE (512 / sizeof(K_WORD))
static Thread clAppThread;
static K_WORD awAppStack[APP_STACK_SIZE];
static void AppMain(void* unused_);

//---------------------------------------------------------------------------
// This block declares the thread data for the idle thread.  It defines a
// thread object, stack (in word-array form), and the entry-point function
// used by the idle thread.
#define IDLE_STACK_SIZE (512 / sizeof(K_WORD))
static Thread clIdleThread;
static K_WORD awIdleStack[IDLE_STACK_SIZE];
static void IdleMain(void* unused_);

//---------------------------------------------------------------------------
static uint8_t    aucTxBuffer[UART_SIZE_TX];
static uint8_t    aucRxBuffer[UART_SIZE_RX];

static ATMegaUARTPlus clUART; //!< UART device driver object

static CommandHandler clDefaultHandler;
static CommandHandler clDirHandler;
static CommandHandler clCatHandler;
static CommandHandler clHelpHandler;

static M3Shell clShell;

static NLFS_EEPROM clNLFS;
static NLFS_Host_t clHost;

const void StringWrite(const char* szStr_)
{
    uint8_t*    src       = (uint8_t*)szStr_;
    uint16_t    u16Remain = MemUtil::StringLength(szStr_);

    while (u16Remain) {
        uint16_t u16Written = clUART.Write(u16Remain, src);
        src += u16Written;
        u16Remain -= u16Written;
    }
}

const void WriteBytes(const char* szStr_, uint16_t u16Len_)
{
    uint8_t*    src       = (uint8_t*)szStr_;
    uint16_t    u16Remain = u16Len_;

    while (u16Remain) {
        uint16_t u16Written = clUART.Write(u16Remain, src);
        src += u16Written;
        u16Remain -= u16Written;
    }
}

//---------------------------------------------------------------------------
int main(void)
{
    Kernel::Init();

    clAppThread.Init(awAppStack, sizeof(awAppStack), 1, AppMain, 0);
    clIdleThread.Init(awIdleStack, sizeof(awIdleStack), 0, IdleMain, 0);

    clAppThread.Start();
    clIdleThread.Start();

    clUART.SetName("/dev/tty"); //!< Add the serial driver
    clUART.Init();
    DriverList::Add(&clUART);

    Kernel::Start();

    return 0;
}

static void DefaultHandler(const char* args)
{
    StringWrite(" Unknown command: ");
    StringWrite(args);
}

static void HelpHandler(const char* args)
{
    StringWrite("Available commands:\r\n");
    StringWrite("\tcat\r\n"
                "\tdir\r\n"
                "\thelp\r\n");
}

static void DirHandler(const char* args)
{
    uint16_t u16Root;

    uint8_t u8Count   = 0;
    char    acVal[16] = { 0 };

    NLFS_File_Stat_t stStats;

    // Get the "/" file node
    u16Root = FS_ROOT_BLOCK;

    StringWrite("Directory Listing For: ");
    clNLFS.GetStat(u16Root, &stStats);
    StringWrite((const char*)stStats.acFileName);
    StringWrite("\r\n");

    u16Root = clNLFS.GetFirstChild(u16Root);

    // Iterate through all child nodes in the FS.
    while (u16Root && INVALID_NODE != u16Root) {
        if (clNLFS.GetStat(u16Root, &stStats)) {
            // Print the filename and size for each file.
            StringWrite("    ");
            StringWrite((const char*)stStats.acFileName);
            StringWrite("    ");
            MemUtil::DecimalToString(stStats.u32FileSize, acVal);
            StringWrite((const char*)acVal);
            StringWrite(" Bytes");
            StringWrite("\r\n");
        }
        u8Count++;
        u16Root = clNLFS.GetNextPeer(u16Root);
    }

    // Display total number of files found
    StringWrite(" Found ");
    MemUtil::DecimalToString(u8Count, acVal);
    StringWrite((const char*)acVal);
    StringWrite(" Files\n");
}

//---------------------------------------------------------------------------
// Print the contents of a file (as ascii) to the terminal
static void CatHandler(const char* args)
{
    char      acBuf[16];
    int       iBytesRead;
    NLFS_File clFile;

    if (!args) {
        StringWrite("File Not Found\n");
        return;
    }

    if (-1 == clFile.Open(&clNLFS, &args[1], NLFS_FILE_READ)) {
        StringWrite("File Not Found\n");
        return;
    }

    while ((iBytesRead = clFile.Read((void*)acBuf, 16)) > 0) {
        WriteBytes(acBuf, iBytesRead);
    }

    clFile.Close();
}

//---------------------------------------------------------------------------
// Prepare an NLFS filesystem
static void NLFS_Prepare(void)
{
    NLFS_File clFile;

    clHost.u32Data = 0; // Format at EEPROM address 0

    if (clNLFS.Mount(&clHost)) {
        return;
    }

    clNLFS.Format(&clHost, 2048, 8, 16);

    clFile.Open(&clNLFS, "/a.txt", NLFS_FILE_CREATE | NLFS_FILE_WRITE);
    clFile.Write((void*)("Hello World!\n"), 13);
    clFile.Close();

    clFile.Open(&clNLFS, "/b.txt", NLFS_FILE_CREATE | NLFS_FILE_WRITE);
    clFile.Write((void*)("Hello!\n"), 7);
    clFile.Close();

    clFile.Open(&clNLFS, "/c.txt", NLFS_FILE_CREATE | NLFS_FILE_WRITE);
    clFile.Write((void*)("World!\n"), 7);
    clFile.Close();

    clFile.Open(&clNLFS, "/d.txt", NLFS_FILE_CREATE | NLFS_FILE_WRITE);
    clFile.Write((void*)("Mark3 Rulez!\n"), 13);
    clFile.Close();

    clFile.Open(&clNLFS, "/e.txt", NLFS_FILE_CREATE | NLFS_FILE_WRITE);
    clFile.Write((void*)("FunkSW!\n"), 8);
    clFile.Close();

    clFile.Open(&clNLFS, "/e.txt", NLFS_FILE_APPEND | NLFS_FILE_WRITE);
    clFile.Write((void*)("FunkSW!\n"), 8);
    clFile.Close();
}

//---------------------------------------------------------------------------
void AppMain(void* unused_)
{
    UartDriver* my_uart = static_cast<UartDriver*>(DriverList::FindByPath("/dev/tty"));

    my_uart->SetBaudRate(57600);
    my_uart->SetBuffers(aucRxBuffer, UART_SIZE_RX, aucTxBuffer, UART_SIZE_TX);
    my_uart->Open();

    NLFS_Prepare();

    clShell.Init();
    clDefaultHandler.Set("default", DefaultHandler);
    clCatHandler.Set("cat", CatHandler);
    clDirHandler.Set("dir", DirHandler);
    clHelpHandler.Set("help", HelpHandler);

    clShell.AddCommand(&clDefaultHandler);
    clShell.AddCommand(&clCatHandler);
    clShell.AddCommand(&clDirHandler);
    clShell.AddCommand(&clHelpHandler);

    clShell.SetDefaultHandler(&clDefaultHandler);
    clShell.SetDriver(my_uart);

    while (1) {
        clShell.InputLoop();
    }
}

//---------------------------------------------------------------------------
void IdleMain(void* unused_)
{
    while (1) {
        // Low priority task + power management routines go here.
        // The actions taken in this context must *not* cause the thread
        // to block, as the kernel requires that at least one thread is
        // schedulable at all times when not using an idle thread.

        // Note that if you have no special power-management code or idle
        // tasks, an empty while(1){} loop is sufficient to guarantee that
        // condition.
    }
}
