/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012 - 2019 m0slevin, all rights reserved.
See license.txt for more information
===========================================================================*/

//---------------------------------------------------------------------------

#include "kerneltypes.h"
#include "kernel.h"
#include "../ut_platform.h"
#include "timerlist.h"
#include "thread.h"
#include "profile.h"
#include "kernel.h"
#include "ksemaphore.h"
#include "kerneltimer.h"
#include "driver.h"
#include "memutil.h"

namespace
{
using namespace Mark3;
//===========================================================================
// Local Defines
//===========================================================================
Timer             clTimer1;
Timer             clTimer2;
Timer             clTimer3;
Semaphore         clTimerSem;
ProfileTimer      clProfileTimer;
ProfileTimer      clProfileTimer2;
ProfileTimer      clProfileTimer3;
uint32_t          u32TimeVal;
uint32_t          u32TempTime;
volatile uint32_t u32CallbackCount = 0;

void TimerExpired(Thread* pclOwner_, void* pvVal_)
{
    clTimerSem.Post();
    u32CallbackCount++;
}
} // anonymous namespace

namespace Mark3
{
//===========================================================================
// Define Test Cases Here
//===========================================================================
TEST(ut_timer_tolerance)
{
    clTimerSem.Init(0, 1);
    clTimer1.Init();

    // Test point - 1ms timer should take at least 1ms
    clProfileTimer.Init();
    clProfileTimer.Start();
    clTimer1.Start(false, 1 + 1, TimerExpired, 0);
    clTimerSem.Pend();
    clProfileTimer.Stop();

    u32TimeVal  = clProfileTimer.GetCurrent();
    u32TempTime = 1;
    EXPECT_GTE(u32TimeVal, u32TempTime - 1);

    // Test point - 1ms timer should be no more than 3ms
    u32TempTime += 2;
    EXPECT_LTE(u32TimeVal, u32TempTime);

    // Test point - 10ms timer should take at least 10ms
    clProfileTimer.Init();
    clProfileTimer.Start();
    clTimer1.Start(false, 10 + 1, TimerExpired, 0);
    clTimerSem.Pend();
    clProfileTimer.Stop();

    u32TimeVal  = clProfileTimer.GetCurrent();
    u32TempTime = 10;

    EXPECT_GTE(u32TimeVal, u32TempTime - 1);

    // Test point - 10ms timer should be no more than 12ms
    u32TempTime += 2;
    EXPECT_LTE(u32TimeVal, u32TempTime);

    // Test point - 100ms timer should take at least 100ms
    clProfileTimer.Init();
    clProfileTimer.Start();
    clTimer1.Start(false, 100 + 1, TimerExpired, 0);
    clTimerSem.Pend();
    clProfileTimer.Stop();

    u32TimeVal  = clProfileTimer.GetCurrent();
    u32TempTime = 100;

    EXPECT_GTE(u32TimeVal, u32TempTime - 1);

    // Test point - 100ms timer should be no more than 102ms
    u32TempTime += 2;
    EXPECT_LTE(u32TimeVal, u32TempTime);

    // Test point - 500ms timer should take at least 500ms
    clProfileTimer.Init();
    clProfileTimer.Start();
    clTimer1.Start(false, 1000 + 1, TimerExpired, 0);
    clTimerSem.Pend();
    clProfileTimer.Stop();

    u32TimeVal  = clProfileTimer.GetCurrent();
    u32TempTime = 1000;

    EXPECT_GTE(u32TimeVal, u32TempTime - 1);

    // Test point - 500ms timer should be no more than 502ms
    u32TempTime += 2;
    EXPECT_LTE(u32TimeVal, u32TempTime);
}

TEST(ut_timer_longrun)
{
    // Profiling timer is not really designed for long profiling
    // operations (1.2 seconds is about as high as we get, since it's
    // so high resolution).  So, use sleeps and multiple iterations
    // w/averaging in order to verify.
    clTimer1.Init();

    uint32_t u32SleepCount = 0;
    clTimerSem.Init(0, 1);

    // Test point - long running timer accuracy; 10-second timer
    // expires after 10 seconds.
    clProfileTimer.Init();
    clTimer1.Start(false, 10000 + 1, TimerExpired, 0);
    u32CallbackCount = 0;

    while (!u32CallbackCount) {
        clProfileTimer.Start();
        Thread::Sleep(100);
        clProfileTimer.Stop();
        u32SleepCount++;
    }

    clProfileTimer.Stop();

    u32TimeVal  = clProfileTimer.GetAverage() * u32SleepCount;
    u32TempTime = 10000;

    EXPECT_GTE(u32TimeVal, u32TempTime - 1);

    // Test point - 100ms accuracy over 10 seconds
    u32TempTime += 102;
    EXPECT_LTE(u32TimeVal, u32TempTime);
}

TEST(ut_timer_repeat)
{
    // Profiling timer is not really designed for long profiling
    // operations (1.2 seconds is about as high as we get, since it's
    // so high resolution).  So, use sleeps and multiple iterations
    // w/averaging in order to verify.
    clTimerSem.Init(0, 1);

    // Repeated timer case - run a 10ms timer 100 times and measure
    // accuracy.  Average iteration must be > 10ms
    u32CallbackCount = 0;

    clProfileTimer.Init();
    clProfileTimer.Start();

    clTimer1.Start(true, 10, TimerExpired, 0);

    while (u32CallbackCount < 100) { clTimerSem.Pend(); }

    clProfileTimer.Stop();
    clTimer1.Stop();

    u32TimeVal  = clProfileTimer.GetCurrent();
    u32TempTime = 1000;

    EXPECT_GTE(u32TimeVal, u32TempTime - 1);

    // Test point - 100ms (10%) maximum tolerance for callback overhead, etc.
    u32TempTime += 100;
    EXPECT_LTE(u32TimeVal, u32TempTime);
}

TEST(ut_timer_multi)
{
    clTimerSem.Init(0, 3);

    // Test using multiple timers simultaneously, verify that
    // each of them expire at the expected times within a specific
    // tolerance

    clProfileTimer.Init();
    clProfileTimer2.Init();
    clProfileTimer3.Init();

    clTimer1.Init();
    clTimer2.Init();
    clTimer3.Init();

    clProfileTimer.Start();
    clTimer1.Start(false, 100, TimerExpired, 0);
    clProfileTimer2.Start();
    clTimer2.Start(false, 200, TimerExpired, 0);
    clProfileTimer3.Start();
    clTimer3.Start(false, 50, TimerExpired, 0);

    // Each timer expiry will post the semaphore.
    clTimerSem.Pend();
    clProfileTimer3.Stop();

    clTimerSem.Pend();
    clProfileTimer.Stop();

    clTimerSem.Pend();
    clProfileTimer2.Stop();

    // Test Point - Timer 1 expired @ 100ms, with a 1 ms tolerance
    u32TimeVal  = clProfileTimer.GetCurrent();
    u32TempTime = 100;
    EXPECT_GTE(u32TimeVal, u32TempTime - 1);

    u32TempTime += 1;
    EXPECT_LTE(u32TimeVal, u32TempTime);

    // Test Point - Timer 2 expired @ 200ms, with a 1 ms tolerance
    u32TimeVal  = clProfileTimer2.GetCurrent();
    u32TempTime = 200;
    EXPECT_GTE(u32TimeVal, u32TempTime - 1);

    u32TempTime += 1;
    EXPECT_LTE(u32TimeVal, u32TempTime);

    // Test Point - Timer 3 expired @ 50ms, with a 1 ms tolerance
    u32TimeVal  = clProfileTimer3.GetCurrent();
    u32TempTime = 50;
    EXPECT_GTE(u32TimeVal, u32TempTime - 1);

    u32TempTime += 1;
    EXPECT_LTE(u32TimeVal, u32TempTime);
}

//===========================================================================
// Test Whitelist Goes Here
//===========================================================================
TEST_CASE_START
TEST_CASE(ut_timer_tolerance)
, TEST_CASE(ut_timer_longrun), TEST_CASE(ut_timer_repeat), TEST_CASE(ut_timer_multi), TEST_CASE_END
} // namespace Mark3
