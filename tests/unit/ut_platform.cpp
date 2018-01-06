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

//---------------------------------------------------------------------------

#include "mark3.h"
#include "drvATMegaUART.h"
#include "unit_test.h"
#include "ut_platform.h"
#include "memutil.h"

#if defined(AVR)
#include <avr/io.h>
#include <avr/sleep.h>
#endif

extern "C" {
void __cxa_pure_virtual(void)
{
}
}

namespace Mark3
{
Thread AppThread; //!< Main "application" thread
K_WORD aucAppStack[(PORT_KERNEL_DEFAULT_STACK_SIZE * 3) / 2];

ATMegaUART clUART; //!< UART device driver object

//---------------------------------------------------------------------------
#if !KERNEL_USE_IDLE_FUNC
Thread  IdleThread; //!< Idle thread - runs when app can't
uint8_t aucIdleStack[PORT_KERNEL_DEFAULT_STACK_SIZE];
#endif
//---------------------------------------------------------------------------
uint8_t aucTxBuffer[UART_SIZE_TX];
uint8_t aucRxBuffer[UART_SIZE_RX];

using FuncPtr = void (*)(void);

//---------------------------------------------------------------------------
void init_tests()
{
    auto* pstTestCase = astTestCases;

    while (pstTestCase->pclTestCase) {
        pstTestCase->pclTestCase->SetName(pstTestCase->szName);
        pstTestCase++;
    }
}

//---------------------------------------------------------------------------
void run_tests()
{
    auto* pstTestCase = astTestCases;

    while (pstTestCase->pclTestCase) {
        pstTestCase->pfTestFunc();
        pstTestCase++;
    }
    PrintString("--DONE--\n");
    Thread::Sleep(100);

    FuncPtr pfReset = 0;
    pfReset();
}

//---------------------------------------------------------------------------
void AppEntry(void* /*unused*/)
{
    {
        auto* my_uart = static_cast<UartDriver*>(DriverList::FindByPath("/dev/tty"));
        my_uart->SetBuffers(aucRxBuffer, UART_SIZE_RX, aucTxBuffer, UART_SIZE_TX);
        my_uart->Open();

        init_tests();
    }

    while (1) {
        run_tests();
    }
}

//---------------------------------------------------------------------------
void IdleEntry(void)
{
#if !KERNEL_USE_IDLE_FUNC
    while (1) {
#endif

#if defined(AVR)
        // LPM code;
        set_sleep_mode(SLEEP_MODE_IDLE);
        cli();
        sleep_enable();
        sei();
        sleep_cpu();
        sleep_disable();
        sei();
#endif

#if !KERNEL_USE_IDLE_FUNC
    }
#endif
}

//---------------------------------------------------------------------------
void PrintString(const char* szStr_)
{
    auto* szTemp = szStr_;
    while (*szTemp) {
        while (1 != clUART.Write(1, (uint8_t*)szTemp)) { /* Do nothing */
        }
        szTemp++;
    }
}
//---------------------------------------------------------------------------
void MyUnitTest::PrintTestResult()
{
    char acTemp[6];

    PrintString("Test ");
    PrintString(GetName());
    PrintString(": ");

    auto iLen = MemUtil::StringLength(GetName());
    if (iLen >= 32) {
        iLen = 32;
    }
    for (uint16_t i = 0; i < 32 - iLen; i++) {
        PrintString(".");
    }
    if (GetPassed() == GetTotal()) {
        PrintString("(PASS)[");
    } else {
        PrintString("(FAIL)[");
    }
    MemUtil::DecimalToString(GetPassed(), (char*)acTemp);
    PrintString((const char*)acTemp);
    PrintString("/");
    MemUtil::DecimalToString(GetTotal(), (char*)acTemp);
    PrintString((const char*)acTemp);
    PrintString("]\n");
}
} //namespace Mark3

using namespace Mark3;
//---------------------------------------------------------------------------
int main(void)
{
    Kernel::Init(); //!< MUST be before other kernel ops

    AppThread.Init(aucAppStack,             //!< Pointer to the stack
                   sizeof(aucAppStack),          //!< Size of the stack
                   1,                       //!< Thread priority
                   AppEntry, //!< Entry function
                   (void*)&AppThread);      //!< Entry function argument

    AppThread.Start(); //!< Schedule the threads

#if KERNEL_USE_IDLE_FUNC
    Kernel::SetIdleFunc(IdleEntry);
#else
    IdleThread.Init(aucIdleStack,             //!< Pointer to the stack
                    sizeof(aucIdleStack),          //!< Size of the stack
                    0,                        //!< Thread priority
                    (ThreadEntry_t)IdleEntry, //!< Entry function
                    NULL);                    //!< Entry function argument

    IdleThread.Start();
#endif

    clUART.SetName("/dev/tty"); //!< Add the serial driver
    clUART.Init();

    DriverList::Add(&clUART);

    Kernel::Start(); //!< Start the kernel!
    return 0;
}
