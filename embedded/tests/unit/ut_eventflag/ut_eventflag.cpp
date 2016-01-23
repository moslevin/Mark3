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

//---------------------------------------------------------------------------

#include "kerneltypes.h"
#include "kernel.h"
#include "../ut_platform.h"
#include "eventflag.h"
#include "thread.h"
#include "memutil.h"
#include "driver.h"

//===========================================================================
// Local Defines
//===========================================================================
Thread clThread1;
Thread clThread2;

#define THREAD1_STACK_SIZE      (256)
K_WORD aucThreadStack1[THREAD1_STACK_SIZE];
#define THREAD2_STACK_SIZE      (160)
K_WORD aucThreadStack2[THREAD2_STACK_SIZE];

EventFlag clFlagGroup;
volatile uint8_t u8FlagCount = 0;
volatile uint8_t u8TimeoutCount = 0;

//---------------------------------------------------------------------------
void WaitOnFlag1Any(void *unused_)
{
    clFlagGroup.Wait(0x0001, EVENT_FLAG_ANY);
    u8FlagCount++;

    Scheduler::GetCurrentThread()->Exit();
}

//---------------------------------------------------------------------------
void WaitOnMultiAny(void *unused_)
{
    clFlagGroup.Wait(0x5555, EVENT_FLAG_ANY);
    u8FlagCount++;

    Scheduler::GetCurrentThread()->Exit();
}

//---------------------------------------------------------------------------
void WaitOnMultiAll(void *unused_)
{
    clFlagGroup.Wait(0x5555, EVENT_FLAG_ALL);
    u8FlagCount++;

    Scheduler::GetCurrentThread()->Exit();
}

//---------------------------------------------------------------------------
void WaitOnAny(void *mask_)
{
    uint16_t u16Mask = *((uint16_t*)mask_);
    while(1)
    {
        clFlagGroup.Wait(u16Mask, EVENT_FLAG_ANY);
        u8FlagCount++;
        clFlagGroup.Clear(u16Mask);
    }
}

//---------------------------------------------------------------------------
void WaitOnAll(void *mask_)
{
    uint16_t u16Mask = *((uint16_t*)mask_);
    while(1)
    {
        clFlagGroup.Wait(u16Mask, EVENT_FLAG_ALL);
        u8FlagCount++;
        clFlagGroup.Clear(u16Mask);
    }
}

//---------------------------------------------------------------------------
void TimedWait(void *time_)
{
    uint16_t u16Ret;
    uint16_t u16Time = *((uint16_t*)time_);
    u16Ret = clFlagGroup.Wait(0x0001, EVENT_FLAG_ALL, u16Time);
    if (u16Ret == 0x0001)
    {
        u8FlagCount++;
    }
    else if (u16Ret == 0x0000)
    {
        u8TimeoutCount++;
    }
    clFlagGroup.Clear(0x0001);
    Scheduler::GetCurrentThread()->Exit();

}


//---------------------------------------------------------------------------
void TimedWaitAll(void *time_)
{
    uint16_t u16Ret;
    uint16_t u16Time = *((uint16_t*)time_);
    while(1)
    {
        u16Ret = clFlagGroup.Wait(0x0001, EVENT_FLAG_ALL, 200);
        if (u16Ret == 0x0001)
        {
            u8FlagCount++;
        }
        else if (u16Ret == 0x0000)
        {            
            Scheduler::GetCurrentThread()->SetExpired(false);
            u8TimeoutCount++;
        }
        clFlagGroup.Clear(0x0001);
    }

    Scheduler::GetCurrentThread()->Exit();
}

//===========================================================================
// Define Test Cases Here
//===========================================================================
TEST(ut_waitany)
{
    // Test - ensure that threads can block using the "waitany" mechanism, and
    // only wake up when bits from its pattern are encountered.
    uint16_t i;
    uint16_t u16Mask = 0x8000;

    clFlagGroup.Init();
    u8FlagCount = 0;

    clThread1.Init(aucThreadStack1, THREAD1_STACK_SIZE, 7, WaitOnAny, (void*)(&u16Mask));
    clThread1.Start();

    Thread::Sleep(100);

    EXPECT_EQUALS(u8FlagCount, 0);

    u16Mask = 0x0001;
    while(u16Mask)
    {
        clFlagGroup.Set(u16Mask);
        Thread::Sleep(100);

        if (u16Mask != 0x8000)
        {
            EXPECT_EQUALS(u8FlagCount, 0);
        }
        else
        {
            EXPECT_EQUALS(u8FlagCount, 1);
        }

        u16Mask <<= 1;
    }
    clThread1.Exit();

    // Okay, that was a single bit-flag test.  Now let's try using a multi-bit flag
    // and verify that any matching pattern will cause a wakeup

    clFlagGroup.Init();
    u8FlagCount = 0;
    u16Mask = 0xAAAA;

    clThread1.Init(aucThreadStack1, THREAD1_STACK_SIZE, 7, WaitOnAny, (void*)(&u16Mask));
    clThread1.Start();

    Thread::Sleep(100);

    EXPECT_EQUALS(u8FlagCount, 0);

    // Test point - the flag set should kick the test thread on even-indexed
    // counters indexes.
    for (i = 0; i < 16; i++)
    {
        uint8_t u8LastFlagCount = u8FlagCount;

        clFlagGroup.Set((uint16_t)(1 << i));

        Thread::Sleep(100);
        if ((i & 1) == 0)
        {
            EXPECT_EQUALS(u8FlagCount, u8LastFlagCount);
        }
        else
        {
            EXPECT_EQUALS(u8FlagCount, u8LastFlagCount+1);
        }
    }

    clThread1.Exit();
}
TEST_END


//===========================================================================
TEST(ut_waitall)
{
    // Test - ensure that threads can block using the "waitany" mechanism, and
    // only wake up when bits from its pattern are encountered.
    uint16_t i;
    uint16_t u16Mask = 0x8000;

    clFlagGroup.Init();
    u8FlagCount = 0;

    clThread1.Init(aucThreadStack1, THREAD1_STACK_SIZE, 7, WaitOnAll, (void*)(&u16Mask));
    clThread1.Start();

    Thread::Sleep(100);

    EXPECT_EQUALS(u8FlagCount, 0);

    u16Mask = 0x0001;
    while(u16Mask)
    {
        clFlagGroup.Set(u16Mask);
        Thread::Sleep(100);

        if (u16Mask != 0x8000)
        {
            EXPECT_EQUALS(u8FlagCount, 0);
        }
        else
        {
            EXPECT_EQUALS(u8FlagCount, 1);
        }

        u16Mask <<= 1;
    }
    clThread1.Exit();

    // Okay, that was a single bit-flag test.  Now let's try using a multi-bit flag
    // and verify that any matching pattern will cause a wakeup

    clFlagGroup.Init();
    u8FlagCount = 0;
    u16Mask = 0xAAAA;

    clThread1.Init(aucThreadStack1, THREAD1_STACK_SIZE, 7, WaitOnAll, (void*)(&u16Mask));
    clThread1.Start();

    Thread::Sleep(100);

    EXPECT_EQUALS(u8FlagCount, 0);

    // Test point - the flag set should kick the test thread on even-indexed
    // counters indexes.
    for (i = 0; i < 16; i++)
    {
        uint8_t u8LastFlagCount = u8FlagCount;

        clFlagGroup.Set((uint16_t)(1 << i));

        Thread::Sleep(100);
        if (i != 15)
        {
            EXPECT_EQUALS(u8FlagCount, u8LastFlagCount);
        }
        else
        {
            EXPECT_EQUALS(u8FlagCount, u8LastFlagCount+1);
        }
    }

    clThread1.Exit();
}
TEST_END

//---------------------------------------------------------------------------
TEST(ut_flag_multiwait)
{

    // Test - ensure that all forms of event-flag unblocking work when there
    // are multiple threads blocked on the same flag.

    clFlagGroup.Init();

    // Test point - 2 threads blocking on an event flag, bit 1.  Wait on these
    // threads until this thread sets bit 0x0001.  When that bit is set, the
    // threads should wake up, incrementing the "ucFlagCount" variable.
    u8FlagCount = 0;
    clFlagGroup.Clear(0xFFFF);

    clThread1.Init(aucThreadStack1, THREAD1_STACK_SIZE, 7, WaitOnFlag1Any, 0);
    clThread2.Init(aucThreadStack2, THREAD2_STACK_SIZE, 7, WaitOnFlag1Any, 0);

    clThread1.Start();
    clThread2.Start();

    Thread::Sleep(100);

    EXPECT_EQUALS(u8FlagCount, 0);

    clFlagGroup.Set(0x0001);

    Thread::Sleep(100);

    EXPECT_EQUALS(u8FlagCount, 2);

    u8FlagCount = 0;
    clFlagGroup.Clear(0xFFFF);

    // Test point - 2 threads blocking on an event flag, bits 0x5555.  Block
    // on these threads, and verify that only bits in the pattern will cause
    // the threads to awaken
    clThread1.Init(aucThreadStack1, THREAD1_STACK_SIZE, 7, WaitOnMultiAny, 0);
    clThread2.Init(aucThreadStack2, THREAD2_STACK_SIZE, 7, WaitOnMultiAny, 0);

    clThread1.Start();
    clThread2.Start();

    Thread::Sleep(100);

    EXPECT_EQUALS(u8FlagCount, 0);

    clFlagGroup.Set(0xAAAA);
    Thread::Sleep(100);

    EXPECT_EQUALS(u8FlagCount, 0);

    clFlagGroup.Set(0x5555);
    Thread::Sleep(100);

    EXPECT_EQUALS(u8FlagCount, 2);

    u8FlagCount = 0;
    clFlagGroup.Clear(0xFFFF);


    clThread1.Init(aucThreadStack1, THREAD1_STACK_SIZE, 7, WaitOnMultiAny, 0);
    clThread2.Init(aucThreadStack2, THREAD2_STACK_SIZE, 7, WaitOnMultiAny, 0);

    clThread1.Start();
    clThread2.Start();

    Thread::Sleep(100);

    EXPECT_EQUALS(u8FlagCount, 0);

    clFlagGroup.Set(0xA000);
    Thread::Sleep(100);

    EXPECT_EQUALS(u8FlagCount, 0);

    clFlagGroup.Set(0x0005);
    Thread::Sleep(100);

    EXPECT_EQUALS(u8FlagCount, 2);
    // Test point - same thing as above, but with the "ALL" flags set.

    u8FlagCount = 0;
    clFlagGroup.Clear(0xFFFF);

    clThread1.Init(aucThreadStack1, THREAD1_STACK_SIZE, 7, WaitOnMultiAll, 0);
    clThread2.Init(aucThreadStack2, THREAD2_STACK_SIZE, 7, WaitOnMultiAll, 0);

    clThread1.Start();
    clThread2.Start();

    Thread::Sleep(100);

    EXPECT_EQUALS(u8FlagCount, 0);

    clFlagGroup.Set(0xAAAA);
    Thread::Sleep(100);

    EXPECT_EQUALS(u8FlagCount, 0);

    clFlagGroup.Set(0x5555);
    Thread::Sleep(100);

    EXPECT_EQUALS(u8FlagCount, 2);


    u8FlagCount = 0;
    clFlagGroup.Clear(0xFFFF);

    // "All" mode - each flag must be set in order to ensure that the threads
    // unblock.
    clThread1.Init(aucThreadStack1, THREAD1_STACK_SIZE, 7, WaitOnMultiAll, 0);
    clThread2.Init(aucThreadStack2, THREAD2_STACK_SIZE, 7, WaitOnMultiAll, 0);

    clThread1.Start();
    clThread2.Start();

    Thread::Sleep(100);

    EXPECT_EQUALS(u8FlagCount, 0);

    clFlagGroup.Set(0xAAAA);
    Thread::Sleep(100);

    EXPECT_EQUALS(u8FlagCount, 0);

    clFlagGroup.Set(0x5500);
    Thread::Sleep(100);

    EXPECT_EQUALS(u8FlagCount, 0);

    clFlagGroup.Set(0x0055);
    Thread::Sleep(100);

    EXPECT_EQUALS(u8FlagCount, 2);
}
TEST_END

//===========================================================================
TEST(ut_timedwait)
{
    uint16_t u16Interval;

    // Test point - verify positive test case (no timeout, no premature
    // unblocking)
    u8TimeoutCount = 0;
    u8FlagCount = 0;
    u16Interval = 200;

    clFlagGroup.Init();

    clThread1.Init(aucThreadStack1, THREAD1_STACK_SIZE, 7, TimedWait, (void*)&u16Interval);
    clThread1.Start();

    Thread::Sleep(100);

    EXPECT_EQUALS(u8TimeoutCount, 0);
    EXPECT_EQUALS(u8FlagCount, 0);

    clFlagGroup.Set(0x0001);

    EXPECT_EQUALS(u8TimeoutCount, 0);
    EXPECT_EQUALS(u8FlagCount, 1);


    // Test point - verify negative test case (timeouts), followed by a
    // positive test result.
    u8TimeoutCount = 0;
    u8FlagCount = 0;
    u16Interval = 200;

    clFlagGroup.Init();
    clFlagGroup.Clear(0xFFFF);

    clThread1.Init(aucThreadStack1, THREAD1_STACK_SIZE, 7, TimedWait, (void*)&u16Interval);
    clThread1.Start();

    Thread::Sleep(100);

    EXPECT_EQUALS(u8TimeoutCount, 0);
    EXPECT_EQUALS(u8FlagCount, 0);

    Thread::Sleep(200);

    EXPECT_EQUALS(u8TimeoutCount, 1);
    EXPECT_EQUALS(u8FlagCount, 0);

    // Test point - verify negative test case (timeouts), followed by a
    // positive test result.
    u8TimeoutCount = 0;
    u8FlagCount = 0;
    u16Interval = 200;

    clFlagGroup.Init();
    clFlagGroup.Clear(0xFFFF);

    clThread1.Init(aucThreadStack1, THREAD1_STACK_SIZE, 7, TimedWaitAll, (void*)&u16Interval);
    clThread1.Start();

    Thread::Sleep(210);
    EXPECT_EQUALS(u8TimeoutCount, 1);
    EXPECT_EQUALS(u8FlagCount, 0);

    Thread::Sleep(210);
    EXPECT_EQUALS(u8TimeoutCount, 2);
    EXPECT_EQUALS(u8FlagCount, 0);

    Thread::Sleep(210);
    EXPECT_EQUALS(u8TimeoutCount, 3);
    EXPECT_EQUALS(u8FlagCount, 0);

    Thread::Sleep(210);
    EXPECT_EQUALS(u8TimeoutCount, 4);
    EXPECT_EQUALS(u8FlagCount, 0);

    Thread::Sleep(210);
    EXPECT_EQUALS(u8TimeoutCount, 5);
    EXPECT_EQUALS(u8FlagCount, 0);

    Thread::Sleep(80);
    clFlagGroup.Set(0x0001);

    EXPECT_EQUALS(u8TimeoutCount, 5);
    EXPECT_EQUALS(u8FlagCount, 1);

    Thread::Sleep(80);
    clFlagGroup.Set(0x0001);

    EXPECT_EQUALS(u8TimeoutCount, 5);
    EXPECT_EQUALS(u8FlagCount, 2);

    Thread::Sleep(80);
    clFlagGroup.Set(0x0001);

    EXPECT_EQUALS(u8TimeoutCount, 5);
    EXPECT_EQUALS(u8FlagCount, 3);

    Thread::Sleep(80);
    clFlagGroup.Set(0x0001);

    EXPECT_EQUALS(u8TimeoutCount, 5);
    EXPECT_EQUALS(u8FlagCount, 4);

    Thread::Sleep(80);
    clFlagGroup.Set(0x0001);

    EXPECT_EQUALS(u8TimeoutCount, 5);
    EXPECT_EQUALS(u8FlagCount, 5);


}
TEST_END

//===========================================================================
// Test Whitelist Goes Here
//===========================================================================
TEST_CASE_START
  TEST_CASE(ut_waitany),
  TEST_CASE(ut_waitall),
  TEST_CASE(ut_flag_multiwait),
  TEST_CASE(ut_timedwait),
TEST_CASE_END
