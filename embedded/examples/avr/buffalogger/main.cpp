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
#include "mark3.h"
#include "kerneldebug.h"
#include "drvUART.h"
#include "tracebuffer.h"
#include "ksemaphore.h"

/*===========================================================================

Example - Logging data via buffalogger/debug APIs.

===========================================================================*/

#define _CAN_HAS_DEBUG
//--[Autogenerated - Do Not Modify]------------------------------------------
#include "dbg_file_list.h"
#include "buffalogger.h"
#if defined(DBG_FILE)
# error "Debug logging file token already defined!  Bailing."
#else
# define DBG_FILE _DBG___EXAMPLES_AVR_BUFFALOGGER_MAIN_CPP
#endif
//--[End Autogenerated content]----------------------------------------------

//---------------------------------------------------------------------------
// This block declares the thread data for the main application thread.  It
// defines a thread object, stack (in word-array form), and the entry-point
// function used by the application thread.
#define APP_STACK_SIZE      (192/sizeof(K_WORD))
static Thread  clAppThread;
static K_WORD  awAppStack[APP_STACK_SIZE];
static void    AppMain(void *unused_);

#define IDLE_STACK_SIZE      (192/sizeof(K_WORD))
static Thread  clIdleThread;
static K_WORD  awIdleStack[APP_STACK_SIZE];
static void    IdleMain(void *unused_);

#define LOGGER_STACK_SIZE   (192/sizeof(K_WORD))
static Thread  clLoggerThread;
static K_WORD  awLoggerStack[APP_STACK_SIZE];
static void    LoggerMain(void *unused_);
static volatile bool bPingPong;
static Semaphore clSem;

//---------------------------------------------------------------------------
static ATMegaUART clUART;

//---------------------------------------------------------------------------
#define UART_SIZE_TX        (32)
#define UART_SIZE_RX        (8)

static uint8_t aucTxBuffer[UART_SIZE_TX];
static uint8_t aucRxBuffer[UART_SIZE_RX];

static volatile uint16_t *pu16Log;
static volatile uint16_t u16LogLen;

extern "C" {
void __cxa_pure_virtual(void) { }
}

void IdleMain(void* unused_)
{
    while(1)
    {

    }
}

//---------------------------------------------------------------------------
void LoggerCallback(uint16_t *pu16Data_, uint16_t u16Len_, bool bPingPong_)
{
    CS_ENTER();
    bPingPong = bPingPong_;
    pu16Log = pu16Data_;
    u16LogLen = u16Len_;
    CS_EXIT();

    clSem.Post();
}

//---------------------------------------------------------------------------
void LoggerMain(void* unused_)
{
    while (1)
    {
        uint8_t* src;
        uint16_t len;

        clSem.Pend();

        CS_ENTER();
        src = (uint8_t*)pu16Log;
        len = u16LogLen * sizeof(uint16_t);
        CS_EXIT();

        uint16_t written = 0;
        while (len != written)
        {
            written += clUART.Write(len - written, src + written);
        }
    }
}

//---------------------------------------------------------------------------
int main(void)
{
    Kernel::Init();

    // Example assumes use of built-in idle.
    clAppThread.Init(  awAppStack,  APP_STACK_SIZE,  2, AppMain,  0);
    clAppThread.Start();

    clLoggerThread.Init(  awLoggerStack,  LOGGER_STACK_SIZE,  1, LoggerMain,  0);
    clLoggerThread.Start();

    clIdleThread.Init(  awIdleStack,  IDLE_STACK_SIZE,  0, IdleMain,  0);
    clIdleThread.Start();

    clUART.SetName("/dev/tty");			//!< Add the serial driver
    clUART.Init();
    clUART.Open();

    DriverList::Add( &clUART );

    Kernel::Start();

    return 0;
}

//---------------------------------------------------------------------------
void AppMain(void *unused_)
{
    {
        uint32_t u32Baud = 57600 * 4;
        clUART.Control( CMD_SET_BAUDRATE, &u32Baud, 0, 0, 0);
    }
    clUART.Control( CMD_SET_BUFFERS, (void*)aucRxBuffer, UART_SIZE_RX, (void*)aucTxBuffer, UART_SIZE_TX );

    clSem.Init(0,1);

    TraceBuffer::SetCallback( LoggerCallback );
    volatile uint16_t u16Iteration = 0;
    while(1)
    {

        Thread::Sleep(100);
        KERNEL_TRACE("Beginning of the main application loop!");
    
        Thread::Sleep(100);
        KERNEL_TRACE_1(" Iteration: %d", u16Iteration++);
        
        Thread::Sleep(100);
        KERNEL_TRACE("End of the main application loop!");
    }
}
