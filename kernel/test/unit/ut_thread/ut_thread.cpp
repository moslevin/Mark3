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
#include "thread.h"
#include "ksemaphore.h"
#include "profile.h"
#include "kerneltimer.h"
#include "driver.h"
#include "memutil.h"

namespace
{
using namespace Mark3;
//===========================================================================
// Local Defines
//===========================================================================
K_WORD aucStack1[PORT_KERNEL_DEFAULT_STACK_SIZE];
K_WORD aucStack2[PORT_KERNEL_DEFAULT_STACK_SIZE];
K_WORD aucStack3[PORT_KERNEL_DEFAULT_STACK_SIZE];

Thread clThread1;
Thread clThread2;
Thread clThread3;

Semaphore clSem1;
Semaphore clSem2;

volatile uint32_t u32RR1;
volatile uint32_t u32RR2;
volatile uint32_t u32RR3;

ProfileTimer clProfiler1;
} // anonymous namespace

namespace Mark3
{
//===========================================================================
// Define Test Cases Here
//===========================================================================
TEST(ut_threadcreate)
{
    auto lThreadEntry = [](void* /*unused_*/) {
        while (1) {
            clSem2.Pend(); // block until the test thread kicks u16
            clSem1.Post();
        }
    };

    // Test point - Create a thread, verify that the thread actually starts.
    clSem1.Init(0, 1);
    clSem2.Init(0, 1);

    // Initialize our thread
    clThread1.Init(aucStack1, sizeof(aucStack1), 7, lThreadEntry, nullptr);

    // Start the thread (threads are created in the stopped state)
    clThread1.Start();

    // Poke the thread using a semaphore, verify it's working
    clSem2.Post();
    clSem1.Pend(10);

    // Ensure that the semaphore was posted before we got to the 10ms timeout
    EXPECT_FALSE(g_pclCurrent->GetExpired());
}

//===========================================================================
TEST(ut_threadstop)
{
    // Test point - stop and restart a thread
    clThread1.Stop();
    Thread::Sleep(10);
    clThread1.Start();

    // Poke the thread using a semaphore, verify it's still responding
    clSem2.Post();
    clSem1.Pend(10);

    EXPECT_FALSE(g_pclCurrent->GetExpired());
}

//===========================================================================
TEST(ut_threadexit)
{
    // Test point - force a thread exit; ensure it doesn't respond once
    // it's un-scheduled.
    clThread1.Exit();
    clSem2.Post();
    clSem1.Pend(10);

    EXPECT_TRUE(g_pclCurrent->GetExpired());
}

//===========================================================================
TEST(ut_threadsleep)
{
    auto lThreadSleepEntryPoint = [](void* /*unused_*/) {
        // Thread will sleep for various intervals, synchronized
        // to semaphore-based IPC.

        // +1 tick for initial, incomplete tick.
        clSem1.Pend();
        Thread::Sleep(5);
        clSem2.Post();

        clSem1.Pend();
        Thread::Sleep(50);
        clSem2.Post();

        clSem1.Pend();
        Thread::Sleep(200);
        clSem2.Post();

        // Exit this thread.
        Scheduler::GetCurrentThread()->Exit();
    };

    // Start another thread, which sleeps for a various length of time
    clSem1.Init(0, 1);
    clSem2.Init(0, 1);

    // Initialize our thread
    clThread1.Init(aucStack1, sizeof(aucStack1), 7, lThreadSleepEntryPoint, nullptr);

    // Start the thread (threads are created in the stopped state)
    clThread1.Start();

    clProfiler1.Init();

    clProfiler1.Start();
    clSem1.Post();
    clSem2.Pend();
    clProfiler1.Stop();

    EXPECT_GTE(clProfiler1.GetCurrent(), 5);
    EXPECT_LTE(clProfiler1.GetCurrent(), 7);

    clSem1.Post();
    clProfiler1.Start();
    clSem2.Pend();
    clProfiler1.Stop();

    EXPECT_GTE(clProfiler1.GetCurrent(), 50);
    EXPECT_LTE(clProfiler1.GetCurrent(), 52);

    clSem1.Post();
    clProfiler1.Start();
    clSem2.Pend();
    clProfiler1.Stop();

    EXPECT_GTE(clProfiler1.GetCurrent(), 200);
    EXPECT_LTE(clProfiler1.GetCurrent(), 202);
}

//===========================================================================
TEST(ut_roundrobin)
{
    auto lRREntryPoint = [](void* value_) {
        volatile uint32_t* pu32Value = (uint32_t*)value_;
        while (1) { (*pu32Value)++; }
    };

    static volatile uint32_t u32Avg;
    static volatile uint32_t u32Max;
    static volatile uint32_t u32Min;
    static volatile uint32_t u32Range;

    // Create three threads that only increment counters, and keep them at
    // the same priority in order to test the roundrobin functionality of
    // the scheduler
    clThread1.Init(aucStack1, sizeof(aucStack1), 1, lRREntryPoint, (void*)&u32RR1);
    clThread2.Init(aucStack2, sizeof(aucStack2), 1, lRREntryPoint, (void*)&u32RR2);
    clThread3.Init(aucStack3, sizeof(aucStack3), 1, lRREntryPoint, (void*)&u32RR3);

    u32RR1 = 0;
    u32RR2 = 0;
    u32RR3 = 0;

    // Adjust thread priority before starting test threads to ensure
    // they all start at the same time (when we hit the 1 second sleep)
    Scheduler::GetCurrentThread()->SetPriority(2);
    clThread1.Start();
    clThread2.Start();
    clThread3.Start();

    Thread::Sleep(5000);

    // When the sleep ends, this will preempt the thread in progress,
    // allowing u16 to stop them, and drop priority.
    clThread1.Stop();
    clThread2.Stop();
    clThread3.Stop();
    Scheduler::GetCurrentThread()->SetPriority(1);

    // Compare the three counters - they should be nearly identical
    if (u32RR1 > u32RR2) {
        u32Max = u32RR1;
    } else {
        u32Max = u32RR2;
    }
    if (u32Max < u32RR3) {
        u32Max = u32RR3;
    }

    if (u32RR1 < u32RR2) {
        u32Min = u32RR1;
    } else {
        u32Min = u32RR2;
    }
    if (u32Min > u32RR3) {
        u32Min = u32RR3;
    }
    u32Range = u32Max - u32Min;
    u32Avg   = (u32RR1 + u32RR2 + u32RR3) / 3;

    // Max-Min delta should not exceed 1% of average for this simple test
    EXPECT_LT(u32Range, u32Avg / 100);

    // Make sure none of the component values are 0
    EXPECT_FAIL_EQUALS(u32RR1, 0);
    EXPECT_FAIL_EQUALS(u32RR2, 0);
    EXPECT_FAIL_EQUALS(u32RR3, 0);
}

//===========================================================================
TEST(ut_quanta)
{
    auto lRREntryPoint = [](void* value_) {
        volatile uint32_t* pu32Value = (uint32_t*)value_;
        while (1) { (*pu32Value)++; }
    };

    uint32_t u32Avg;
    uint32_t u32Max;
    uint32_t u32Min;
    uint32_t u32Range;

    // Create three threads that only increment counters - similar to the
    // previous test.  However, modify the thread quanta such that each thread
    // will get a different proportion of the CPU cycles.
    clThread1.Init(aucStack1, sizeof(aucStack1), 1, lRREntryPoint, (void*)&u32RR1);
    clThread2.Init(aucStack2, sizeof(aucStack2), 1, lRREntryPoint, (void*)&u32RR2);
    clThread3.Init(aucStack3, sizeof(aucStack3), 1, lRREntryPoint, (void*)&u32RR3);

    u32RR1 = 0;
    u32RR2 = 0;
    u32RR3 = 0;

    // Adjust thread priority before starting test threads to ensure
    // they all start at the same time (when we hit the 1 second sleep)
    Scheduler::GetCurrentThread()->SetPriority(2);

    // Set a different execution quanta for each thread
    clThread1.SetQuantum(3);
    clThread2.SetQuantum(6);
    clThread3.SetQuantum(9);

    clThread1.Start();
    clThread2.Start();
    clThread3.Start();

    Thread::Sleep(1800);

    // When the sleep ends, this will preempt the thread in progress,
    // allowing u16 to stop them, and drop priority.
    clThread1.Stop();
    clThread2.Stop();
    clThread3.Stop();
    Scheduler::GetCurrentThread()->SetPriority(1);

    // Test point - make sure that Q3 > Q2 > Q1
    EXPECT_GT(u32RR2, u32RR1);
    EXPECT_GT(u32RR3, u32RR2);

    // scale the counters relative to the largest value, and compare.
    u32RR1 *= 3;
    u32RR2 *= 3;
    u32RR2 = (u32RR2 + 1) / 2;

    // After scaling, they should be nearly identical (well, close at least)
    if (u32RR1 > u32RR2) {
        u32Max = u32RR1;
    } else {
        u32Max = u32RR2;
    }
    if (u32Max < u32RR3) {
        u32Max = u32RR3;
    }

    if (u32RR1 < u32RR2) {
        u32Min = u32RR1;
    } else {
        u32Min = u32RR2;
    }
    if (u32Min > u32RR3) {
        u32Min = u32RR3;
    }
    u32Range = u32Max - u32Min;
    u32Avg   = (u32RR1 + u32RR2 + u32RR3) / 3;

    // Max-Min delta should not exceed 20% of average for this test -- tick-based timers
    // are coarse, and prone to thread preference due to phase.
    EXPECT_LT(u32Range, u32Avg / 3);

    // Make sure none of the component values are 0
    EXPECT_FAIL_EQUALS(u32RR1, 0);
    EXPECT_FAIL_EQUALS(u32RR2, 0);
    EXPECT_FAIL_EQUALS(u32RR3, 0);
}

//===========================================================================
// Test Whitelist Goes Here
//===========================================================================
TEST_CASE_START
TEST_CASE(ut_threadcreate)
, TEST_CASE(ut_threadstop), TEST_CASE(ut_threadexit), TEST_CASE(ut_threadsleep), TEST_CASE(ut_roundrobin),
    TEST_CASE(ut_quanta), TEST_CASE_END
} // namespace Mark3
