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

//---------------------------------------------------------------------------

#include "kerneltypes.h"
#include "kernel.h"
#include "../ut_platform.h"
#include "timerlist.h"
#include "thread.h"
#include "kernelprofile.h"
#include "profile.h"
#include "kernel.h"
#include "ksemaphore.h"
#include "kerneltimer.h"
#include "driver.h"
#include "memutil.h"

//===========================================================================
// Local Defines
//===========================================================================
static Timer clTimer1;
static Timer clTimer2;
static Timer clTimer3;
static Semaphore clTimerSem;
static ProfileTimer clProfileTimer;
static ProfileTimer clProfileTimer2;
static ProfileTimer clProfileTimer3;
static uint32_t u32TimeVal;
static uint32_t u32TempTime;
static volatile uint32_t u32CallbackCount = 0;

static void TimerCallback( Thread *pclOwner_, void *pvVal_ )
{
    clTimerSem.Post();
    u32CallbackCount++;
}

//===========================================================================
// Define Test Cases Here
//===========================================================================
TEST(ut_timer_tolerance)
{
    Profiler::Start();
    clTimerSem.Init(0, 1);

    // Test point - 1ms timer should take at least 1ms
    clProfileTimer.Init();
    clProfileTimer.Start();
    clTimer1.Start( false, 1, TimerCallback, 0 );
    clTimerSem.Pend();
    clProfileTimer.Stop();

    u32TimeVal = clProfileTimer.GetCurrent() * CLOCK_DIVIDE;
    u32TempTime = SYSTEM_FREQ / 1000;
    EXPECT_GT(u32TimeVal, u32TempTime);

    // Test point - 1ms timer should be no more than 3ms
    u32TempTime *= 3;
    EXPECT_LT(u32TimeVal, u32TempTime);

    // Test point - 10ms timer should take at least 10ms
    clProfileTimer.Init();
    clProfileTimer.Start();
    clTimer1.Start( false, 10, TimerCallback, 0 );
    clTimerSem.Pend();
    clProfileTimer.Stop();

    u32TimeVal = clProfileTimer.GetCurrent() * CLOCK_DIVIDE;
    u32TempTime = SYSTEM_FREQ / 100;

    EXPECT_GT(u32TimeVal, u32TempTime);

    // Test point - 10ms timer should be no more than 12ms
    u32TempTime += 2* (SYSTEM_FREQ / 1000);
    EXPECT_LT(u32TimeVal, u32TempTime);

    // Test point - 100ms timer should take at least 100ms
    clProfileTimer.Init();
    clProfileTimer.Start();
    clTimer1.Start( false, 100, TimerCallback, 0 );
    clTimerSem.Pend();
    clProfileTimer.Stop();

    u32TimeVal = clProfileTimer.GetCurrent() * CLOCK_DIVIDE;
    u32TempTime = SYSTEM_FREQ / 10;

    EXPECT_GT(u32TimeVal, u32TempTime);

    // Test point - 100ms timer should be no more than 102ms
    u32TempTime += 2 * (SYSTEM_FREQ / 1000);
    EXPECT_LT(u32TimeVal, u32TempTime);

    // Test point - 1000ms timer should take at least 100ms
    clProfileTimer.Init();
    clProfileTimer.Start();
    clTimer1.Start( false, 1000, TimerCallback, 0 );
    clTimerSem.Pend();
    clProfileTimer.Stop();

    u32TimeVal = clProfileTimer.GetCurrent() * CLOCK_DIVIDE;
    u32TempTime = SYSTEM_FREQ;

    EXPECT_GT(u32TimeVal, u32TempTime);

    // Test point - 1000ms timer should be no more than 1002ms
    u32TempTime += 2* (SYSTEM_FREQ / 1000);
    EXPECT_LT(u32TimeVal, u32TempTime);

    Profiler::Stop();
}
TEST_END


TEST(ut_timer_longrun)
{
    // Profiling timer is not really designed for long profiling
    // operations (1.2 seconds is about as high as we get, since it's
    // so high resolution).  So, use sleeps and multiple iterations
    // w/averaging in order to verify.

    uint32_t u32SleepCount = 0;
    Profiler::Start();
    clTimerSem.Init(0, 1);

    // Test point - long running timer accuracy; 10-second timer
    // expires after 10 seconds.
    clProfileTimer.Init();
    clTimer1.Start( false, 10000, TimerCallback, 0 );
    u32CallbackCount = 0;

    while (!u32CallbackCount)
    {
        clProfileTimer.Start();
        Thread::Sleep(100);
        clProfileTimer.Stop();
        u32SleepCount++;
    }

    clProfileTimer.Stop();

    u32TimeVal = clProfileTimer.GetAverage() * CLOCK_DIVIDE * u32SleepCount;
    u32TempTime = SYSTEM_FREQ * 10;

    EXPECT_GT(u32TimeVal, u32TempTime);

    // Test point - 100ms accuracy over 10 seconds
    u32TempTime += SYSTEM_FREQ / 10;
    EXPECT_LT(u32TimeVal, u32TempTime);

    Profiler::Stop();
}
TEST_END

TEST(ut_timer_repeat)
{
    // Profiling timer is not really designed for long profiling
    // operations (1.2 seconds is about as high as we get, since it's
    // so high resolution).  So, use sleeps and multiple iterations
    // w/averaging in order to verify.
    uint32_t u32SleepCount = 0;
    Profiler::Start();
    clTimerSem.Init(0, 1);

    // Repeated timer case - run a 10ms timer 100 times and measure
    // accuracy.  Average iteration must be > 10ms
    u32CallbackCount = 0;

    clProfileTimer.Init();
    clProfileTimer.Start();

    clTimer1.Start( true, 10, TimerCallback, 0 );

    while (u32CallbackCount < 100)
    {
        clTimerSem.Pend();
    }

    clProfileTimer.Stop();
    clTimer1.Stop();

    u32TimeVal = clProfileTimer.GetCurrent() * CLOCK_DIVIDE;
    u32TempTime = SYSTEM_FREQ;

    EXPECT_GT(u32TimeVal, u32TempTime);

#if KERNEL_TIMERS_TICKLESS
    // Test point - 50ms (5%) maximum tolerance for callback overhead, etc.
    u32TempTime += SYSTEM_FREQ / 20;
    EXPECT_LT(u32TimeVal, u32TempTime);
#else
    // Test point - 100ms (10%) maximum tolerance for callback overhead, etc.
    u32TempTime += SYSTEM_FREQ / 10;
    EXPECT_LT(u32TimeVal, u32TempTime);
#endif

#if 0
    // Debug code to print out the profiling times
    Driver *pclDriver = DriverList::FindByPath("/dev/tty");
    char acData[13];
    MemUtil::DecimalToString(u32TimeVal, acData);
    pclDriver->Write( MemUtil::StringLength(acData), (uint8_t*)acData);
    pclDriver->Write(1, (uint8_t*)(" "));
    MemUtil::DecimalToString(u32TempTime, acData);
    pclDriver->Write( MemUtil::StringLength(acData), (uint8_t*)acData);
#endif

    Profiler::Stop();

}
TEST_END

TEST(ut_timer_multi)
{
    Profiler::Start();
    clTimerSem.Init(0, 3);

    // Test using multiple timers simultaneously, verify that
    // each of them expire at the expected times within a specific
    // tolerance

    clProfileTimer.Init();
    clProfileTimer2.Init();
    clProfileTimer3.Init();

    clProfileTimer.Start();
    clTimer1.Start( false, 100, TimerCallback, 0 );
    clProfileTimer2.Start();
    clTimer2.Start( false, 200, TimerCallback, 0 );
    clProfileTimer3.Start();
    clTimer3.Start( false, 50, TimerCallback, 0 );

    // Each timer expiry will post the semaphore.
    clTimerSem.Pend();
    clProfileTimer3.Stop();

    clTimerSem.Pend();
    clProfileTimer.Stop();

    clTimerSem.Pend();
    clProfileTimer2.Stop();

    // Test Point - Timer 1 expired @ 100ms, with a 1 ms tolerance
    u32TimeVal = clProfileTimer.GetCurrent() * CLOCK_DIVIDE;
    u32TempTime = SYSTEM_FREQ / 10;
    EXPECT_GT(u32TimeVal, u32TempTime);

    u32TempTime += SYSTEM_FREQ / 1000;
    EXPECT_LT(u32TimeVal, u32TempTime);

    // Test Point - Timer 2 expired @ 200ms, with a 1 ms tolerance
    u32TimeVal = clProfileTimer2.GetCurrent() * CLOCK_DIVIDE;
    u32TempTime = SYSTEM_FREQ / 5;
    EXPECT_GT(u32TimeVal, u32TempTime);

    u32TempTime += SYSTEM_FREQ / 1000;
    EXPECT_LT(u32TimeVal, u32TempTime);

    // Test Point - Timer 3 expired @ 50ms, with a 1 ms tolerance
    u32TimeVal = clProfileTimer3.GetCurrent() * CLOCK_DIVIDE;
    u32TempTime = SYSTEM_FREQ / 20;
    EXPECT_GT(u32TimeVal, u32TempTime);

    u32TempTime += SYSTEM_FREQ / 1000;
    EXPECT_LT(u32TimeVal, u32TempTime);

    Profiler::Stop();
}
TEST_END


//===========================================================================
// Test Whitelist Goes Here
//===========================================================================
TEST_CASE_START
  TEST_CASE(ut_timer_tolerance),
  TEST_CASE(ut_timer_longrun),
  TEST_CASE(ut_timer_repeat),
  TEST_CASE(ut_timer_multi),
TEST_CASE_END
