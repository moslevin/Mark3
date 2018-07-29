/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012 - 2018 m0slevin, all rights reserved.
See license.txt for more information
===========================================================================*/

//---------------------------------------------------------------------------

#include "mark3.h"
#include "driver.h"
#include "unit_test.h"
#include "ut_platform.h"
#include "memutil.h"
#include "ut_support.h"

extern "C" {
void __cxa_pure_virtual(void)
{
}
}

namespace Mark3
{
Thread AppThread; //!< Main "application" thread
K_WORD aucAppStack[(PORT_KERNEL_DEFAULT_STACK_SIZE * 3) / 2];


//---------------------------------------------------------------------------
Thread  IdleThread; //!< Idle thread - runs when app can't
K_WORD aucIdleStack[PORT_KERNEL_DEFAULT_STACK_SIZE];

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

    UnitTestSupport::OnExit(0);

    FuncPtr pfReset = 0;
    pfReset();
}

//---------------------------------------------------------------------------
void AppEntry(void* /*unused*/)
{
    {
        UnitTestSupport::OnStart();
        init_tests();
    }

    while (1) {
        run_tests();
    }
}

//---------------------------------------------------------------------------
void IdleEntry(void* args)
{
    while (1) {
        UnitTestSupport::OnIdle();
    }
}

//---------------------------------------------------------------------------
void PrintString(const char* szStr_)
{
    auto* pclDriver = DriverList::FindByPath("/dev/tty");
    auto* szTemp = szStr_;
    while (*szTemp) {
        while (1 != pclDriver->Write(1, (uint8_t*)szTemp)) { /* Do nothing */
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
                   &AppThread);      //!< Entry function argument

    AppThread.Start(); //!< Schedule the threads

    IdleThread.Init(aucIdleStack,             //!< Pointer to the stack
                    sizeof(aucIdleStack),          //!< Size of the stack
                    0,                        //!< Thread priority
                    (ThreadEntryFunc)IdleEntry, //!< Entry function
                    NULL);                    //!< Entry function argument

    IdleThread.Start();

    UnitTestSupport::OnInit();

    Kernel::Start(); //!< Start the kernel!
    return 0;
}
