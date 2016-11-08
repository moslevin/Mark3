/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012-2016 Funkenstein Software Consulting, all rights reserved.
See license.txt for more information
===========================================================================*/
#include "mark3.h"
#include "drvUART.h"
#include "m3shell.h"
#include "memutil.h"

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
#define APP_STACK_SIZE (320 / sizeof(K_WORD))
static Thread clAppThread;
static K_WORD awAppStack[APP_STACK_SIZE];
static void AppMain(void* unused_);

//---------------------------------------------------------------------------
// This block declares the thread data for the idle thread.  It defines a
// thread object, stack (in word-array form), and the entry-point function
// used by the idle thread.
#define IDLE_STACK_SIZE (320 / sizeof(K_WORD))
static Thread clIdleThread;
static K_WORD awIdleStack[IDLE_STACK_SIZE];
static void IdleMain(void* unused_);

//---------------------------------------------------------------------------
static uint8_t    aucTxBuffer[UART_SIZE_TX];
static uint8_t    aucRxBuffer[UART_SIZE_RX];
static ATMegaUART clUART; //!< UART device driver object

static CommandHandler clActionHandler;
static CommandHandler clDefaultHandler;

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

void ActionHandler(const char* args)
{
    StringWrite("Hello World!: ");
    StringWrite(args);
}

void DefaultHandler(const char* args)
{
    StringWrite(" Unknown command: ");
    StringWrite(args);
}


static M3Shell clShell;

void AppMain(void* unused_)
{
    Driver* my_uart = DriverList::FindByPath("/dev/tty");

    uint32_t new_baud = 57600;
    my_uart->Control(CMD_SET_BAUDRATE, &new_baud, 0,0,0);

    my_uart->Control(CMD_SET_BUFFERS, aucRxBuffer, UART_SIZE_RX, aucTxBuffer, UART_SIZE_TX);

    my_uart->Open();

    clShell.Init();
    clActionHandler.Set("action", ActionHandler);
    clDefaultHandler.Set("default", DefaultHandler);

    clShell.AddCommand(&clActionHandler);
    clShell.AddCommand(&clDefaultHandler);
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
