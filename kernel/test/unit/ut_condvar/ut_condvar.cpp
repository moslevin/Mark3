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

#include "kerneltypes.h"
#include "kernel.h"
#include "../ut_platform.h"
#include "mark3.h"

namespace
{
using namespace Mark3;

Thread clTestThread1;
K_WORD awThreadStack1[PORT_KERNEL_DEFAULT_STACK_SIZE];

Thread clTestThread2;
K_WORD awThreadStack2[PORT_KERNEL_DEFAULT_STACK_SIZE];

Thread clTestThread3;
K_WORD awThreadStack3[PORT_KERNEL_DEFAULT_STACK_SIZE];

volatile bool isSignalled  = false;
volatile bool isTimeout    = false;
volatile int  iSignalCount = 0;
Mutex         clMutex;

} // anonymous namespace

namespace Mark3
{
//===========================================================================
// Local Defines
//===========================================================================

//===========================================================================
// Define Test Cases Here
//===========================================================================

TEST(ut_condvar_wait_signal)
{
    ConditionVariable clCondVar;

    clCondVar.Init();
    clMutex.Init();
    isSignalled = false;
    isTimeout   = false;

    auto lWaiter = [](void* cv) {
        auto* pclCondVar = static_cast<ConditionVariable*>(cv);

        pclCondVar->Wait(&clMutex);
        isSignalled = true;
        clMutex.Release();

        // Explicitly exit the thread
        Scheduler::GetCurrentThread()->Exit();
    };

    clTestThread1.Init(awThreadStack1,
                       sizeof(awThreadStack1),
                       Scheduler::GetCurrentThread()->GetCurPriority() + 1,
                       lWaiter,
                       &clCondVar);

    clTestThread1.Start();

    Thread::Sleep(10);
    EXPECT_FALSE(isSignalled);

    clCondVar.Signal();

    Thread::Sleep(10);
    EXPECT_TRUE(isSignalled);
}


//===========================================================================
TEST(ut_condvar_wait_broadcast)
{
    ConditionVariable clCondVar;
    clMutex.Init();
    clCondVar.Init();

    isSignalled  = false;
    isTimeout    = false;
    iSignalCount = 0;

    auto lWaiter = [](void* cv) {
        auto* pclCondVar = static_cast<ConditionVariable*>(cv);

        pclCondVar->Wait(&clMutex);
        iSignalCount++;
        clMutex.Release();

        // Explicitly exit the thread
        Scheduler::GetCurrentThread()->Exit();
    };

    clTestThread1.Init(awThreadStack1,
                       sizeof(awThreadStack1),
                       Scheduler::GetCurrentThread()->GetCurPriority() + 1,
                       lWaiter,
                       &clCondVar);

    clTestThread1.Start();

    clTestThread2.Init(awThreadStack2,
                       sizeof(awThreadStack2),
                       Scheduler::GetCurrentThread()->GetCurPriority() + 1,
                       lWaiter,
                       &clCondVar);

    clTestThread2.Start();

    clTestThread3.Init(awThreadStack3,
                       sizeof(awThreadStack3),
                       Scheduler::GetCurrentThread()->GetCurPriority() + 1,
                       lWaiter,
                       &clCondVar);

    clTestThread3.Start();

    Thread::Sleep(10);
    EXPECT_EQUALS(iSignalCount, 0);

    clCondVar.Broadcast();

    Thread::Sleep(10);
    EXPECT_EQUALS(iSignalCount, 3);
}


//===========================================================================
TEST(ut_condvar_wait_multi_signal)
{
    ConditionVariable clCondVar;
    clMutex.Init();
    clCondVar.Init();

    isSignalled  = false;
    isTimeout    = false;
    iSignalCount = 0;

    auto lWaiter = [](void* cv) {
        auto* pclCondVar = static_cast<ConditionVariable*>(cv);

        pclCondVar->Wait(&clMutex);
        iSignalCount++;
        clMutex.Release();

        // Explicitly exit the thread
        Scheduler::GetCurrentThread()->Exit();
    };

    clTestThread1.Init(awThreadStack1,
                       sizeof(awThreadStack1),
                       Scheduler::GetCurrentThread()->GetCurPriority() + 1,
                       lWaiter,
                       &clCondVar);

    clTestThread1.Start();

    clTestThread2.Init(awThreadStack2,
                       sizeof(awThreadStack2),
                       Scheduler::GetCurrentThread()->GetCurPriority() + 1,
                       lWaiter,
                       &clCondVar);

    clTestThread2.Start();

    clTestThread3.Init(awThreadStack3,
                       sizeof(awThreadStack3),
                       Scheduler::GetCurrentThread()->GetCurPriority() + 1,
                       lWaiter,
                       &clCondVar);

    clTestThread3.Start();

    Thread::Sleep(10);
    EXPECT_EQUALS(iSignalCount, 0);

    clCondVar.Signal();

    Thread::Sleep(10);
    EXPECT_EQUALS(iSignalCount, 1);

    clCondVar.Signal();

    Thread::Sleep(10);
    EXPECT_EQUALS(iSignalCount, 2);

    clCondVar.Signal();

    Thread::Sleep(10);
    EXPECT_EQUALS(iSignalCount, 3);
}


//===========================================================================
TEST(ut_condvar_wait_multi_broadcast)
{
    ConditionVariable clCondVar;
    clMutex.Init();
    clCondVar.Init();

    isSignalled  = false;
    isTimeout    = false;
    iSignalCount = 0;

    auto lWaiter = [](void* cv) {
        auto* pclCondVar = static_cast<ConditionVariable*>(cv);

        pclCondVar->Wait(&clMutex);
        iSignalCount++;
        clMutex.Release();

        // Explicitly exit the thread
        Scheduler::GetCurrentThread()->Exit();
    };

    clTestThread1.Init(awThreadStack1,
                       sizeof(awThreadStack1),
                       Scheduler::GetCurrentThread()->GetCurPriority() + 1,
                       lWaiter,
                       &clCondVar);

    clTestThread1.Start();

    clTestThread2.Init(awThreadStack2,
                       sizeof(awThreadStack2),
                       Scheduler::GetCurrentThread()->GetCurPriority() + 1,
                       lWaiter,
                       &clCondVar);

    clTestThread2.Start();

    clTestThread3.Init(awThreadStack3,
                       sizeof(awThreadStack3),
                       Scheduler::GetCurrentThread()->GetCurPriority() + 1,
                       lWaiter,
                       &clCondVar);

    clTestThread3.Start();

    Thread::Sleep(10);
    EXPECT_EQUALS(iSignalCount, 0);

    clCondVar.Broadcast();

    Thread::Sleep(10);
    EXPECT_EQUALS(iSignalCount, 3);
}


//===========================================================================
TEST(ut_condvar_wait_timeout)
{
    ConditionVariable clCondVar;
    clMutex.Init();
    clCondVar.Init();

    isSignalled  = false;
    isTimeout    = false;
    iSignalCount = 0;

    auto lWaiter = [](void* cv) {
        auto* pclCondVar = static_cast<ConditionVariable*>(cv);

        if (!pclCondVar->Wait(&clMutex, 30)) {
            isTimeout = true;
        }

        Scheduler::GetCurrentThread()->Exit();
    };

    clTestThread1.Init(awThreadStack1,
                       sizeof(awThreadStack1),
                       Scheduler::GetCurrentThread()->GetCurPriority() + 1,
                       lWaiter,
                       &clCondVar);

    clTestThread1.Start();

    EXPECT_FALSE(isTimeout);
    Thread::Sleep(10);
    EXPECT_FALSE(isTimeout);
    Thread::Sleep(50);
    EXPECT_TRUE(isTimeout);
}


//===========================================================================
TEST(ut_condvar_timedwait_success)
{
    ConditionVariable clCondVar;
    clMutex.Init();
    clCondVar.Init();

    isSignalled  = false;
    isTimeout    = false;
    iSignalCount = 0;

    auto lWaiter = [](void* cv) {
        auto* pclCondVar = static_cast<ConditionVariable*>(cv);

        if (!pclCondVar->Wait(&clMutex, 30)) {
            isTimeout = true;
        } else {
            isSignalled = true;
        }

        Scheduler::GetCurrentThread()->Exit();
    };

    clTestThread1.Init(awThreadStack1,
                       sizeof(awThreadStack1),
                       Scheduler::GetCurrentThread()->GetCurPriority() + 1,
                       lWaiter,
                       &clCondVar);

    clTestThread1.Start();

    EXPECT_FALSE(isTimeout);
    Thread::Sleep(10);
    EXPECT_FALSE(isTimeout);
    Thread::Sleep(10);
    clCondVar.Signal();
    EXPECT_FALSE(isTimeout);
    EXPECT_TRUE(isSignalled);
}

//===========================================================================
// Test Whitelist Goes Here
//===========================================================================
TEST_CASE_START
TEST_CASE(ut_condvar_wait_signal), TEST_CASE(ut_condvar_wait_broadcast), TEST_CASE(ut_condvar_wait_multi_signal),
    TEST_CASE(ut_condvar_wait_multi_broadcast), TEST_CASE(ut_condvar_wait_timeout),
    TEST_CASE(ut_condvar_timedwait_success), TEST_CASE_END
} // namespace Mark3
