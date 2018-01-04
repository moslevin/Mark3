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
/*!

    \file   kernel++.cpp

    \brief  Test harness

*/

//---------------------------------------------------------------------------

#include "kerneltypes.h"
#include "kernel.h"
#include "thread.h"
#include "drvATMegaUART.h"
#include "slip.h"
#include "fixed_heap.h"
#include "nlfs.h"
#include "nlfs_file.h"
#include "nlfs_ram.h"
#include "memutil.h"
#include "nlfs_eeprom.h"

#include <avr/io.h>
#include <avr/sleep.h>

//---------------------------------------------------------------------------
// Need to have this to handle pure-virtual functions
extern "C" void __cxa_pure_virtual()
{
}

namespace {
using namespace Mark3;
//---------------------------------------------------------------------------
// Global objects
Thread AppThread;  //!< Main "application" thread
Thread IdleThread; //!< Idle thread - runs when app can't

ATMegaUART clUART; //!< UART device driver object
NLFS_EEPROM clNLFS; //!< Filesystem object

//---------------------------------------------------------------------------
#define STACK_SIZE_APP (384)  //!< Size of the main app's stack
#define STACK_SIZE_IDLE (128) //!< Size of the idle thread stack

//---------------------------------------------------------------------------
#define UART_SIZE_RX (8)  //!< UART RX Buffer size
#define UART_SIZE_TX (32) //!< UART TX Buffer size

//---------------------------------------------------------------------------
uint8_t aucAppStack[STACK_SIZE_APP];
uint8_t aucIdleStack[STACK_SIZE_IDLE];

//---------------------------------------------------------------------------
uint8_t aucTxBuffer[UART_SIZE_TX];
uint8_t aucRxBuffer[UART_SIZE_RX];

NLFS_Host_t clHost;
NLFS_File   clFile;

void PrintString(const char* szStr_)
{
    char* szTemp = (char*)szStr_;
    while (*szTemp) {
        while (1 != clUART.Write(1, (uint8_t*)szTemp)) { /* Do nothing */
        }
        szTemp++;
    }
}

//---------------------------------------------------------------------------
void NLFS_Test(void)
{
    uint8_t aucRead[6];

    clHost.u32Data = 0; // Format at EEPROM address 0

    //!! Test 1 - Format the filesystem.
    clNLFS.Format(&clHost, 512, 4, 16);

    PrintString("F_OK\n");

    //!! Test 2 - Create a file in the filesystem
    if (INVALID_NODE == clNLFS.Create_File("/a.txt")) {
        PrintString("CF\n");
    } else {
        PrintString("CK\n");
    }

    //!! Test 3 - Open a file in the filesystem, write to it, and close it
    if (-1 == clFile.Open(&clNLFS, "/a.txt", NLFS_FILE_TRUNCATE)) {
        PrintString("OF\n");
    } else {
        PrintString("OK\n");
    }

    clFile.Write((void*)"Hello", 6);
    clFile.Close();

    //!! Test 4 - Read the file into a local array.
    MemUtil::SetMemory((void*)aucRead, 0, 6);
    clFile.Open(&clNLFS, "/a.txt", NLFS_FILE_APPEND);
    clFile.Read((void*)aucRead, 6);
    PrintString((const char*)aucRead);

    if (MemUtil::CompareStrings((const char*)aucRead, "Hello")) {
        PrintString("RK");
    } else {
        PrintString("RF");
    }

    clFile.Close();
}

//---------------------------------------------------------------------------
void AppEntry(void)
{
    {
        UartDriver* my_uart = static_cast<UartDriver*>(DriverList::FindByPath("/dev/tty"));
        my_uart->SetBuffers(aucRxBuffer, UART_SIZE_RX, aucTxBuffer, UART_SIZE_TX);
        my_uart->Open();
    }

    while (1) {
        NLFS_Test();
    }
}

//---------------------------------------------------------------------------
void IdleEntry(void)
{
    while (1) {
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
} // anonymous namespace

using namespace Mark3;

//---------------------------------------------------------------------------
int main(void)
{
    Kernel::Init(); //!< MUST be before other kernel ops

    AppThread.Init(aucAppStack,             //!< Pointer to the stack
                   STACK_SIZE_APP,          //!< Size of the stack
                   1,                       //!< Thread priority
                   (ThreadEntryFunc)AppEntry, //!< Entry function
                   (void*)&AppThread);      //!< Entry function argument

    IdleThread.Init(aucIdleStack,             //!< Pointer to the stack
                    STACK_SIZE_IDLE,          //!< Size of the stack
                    0,                        //!< Thread priority
                    (ThreadEntryFunc)IdleEntry, //!< Entry function
                    NULL);                    //!< Entry function argument

    AppThread.Start(); //!< Schedule the threads
    IdleThread.Start();

    clUART.SetName("/dev/tty"); //!< Add the serial driver
    clUART.Init();

    DriverList::Add(&clUART);

    Kernel::Start(); //!< Start the kernel!
}
