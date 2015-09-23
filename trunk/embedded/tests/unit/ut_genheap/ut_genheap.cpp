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
#include "memutil.h"
#include "driver.h"
#include "thread.h"
#include "../ut_platform.h"
#include "arena.h"

//===========================================================================
// Local Defines
//===========================================================================
#define MAX_ALLOCS          (32)
#define TEST_STACK_SIZE     (160)
static uint16_t u16MaxAllocs;
static uint16_t u16MaxAllocSize;

static volatile void *apvAllocs[MAX_ALLOCS]; // assuming we have < 128 system heap allocs...

#define ARENA_SIZE (128)
static uint8_t au8Arena[ARENA_SIZE];
static Arena clArena;

//===========================================================================
// Define Test Cases Here

//===========================================================================
// Calibration test - check the maximum # of elements that can be alloc'd
// and their maximum sizes.  Very basic sanity testing, and testing that we
// start returning NULL when allocating too much/too large data.
TEST(ut_arena_calibrate)
{
    uint16_t u16Min, u16Max, u16Avg;
    void *pvData;

    // recreate the system heap (you wouldn't do this in a real system, but
    // here, we're the only clients of the heap so we can clobber it)
    clArena.Init(au8Arena,ARENA_SIZE);

    // Get the maximum number of allocations before heap exhaustion:
    u16MaxAllocs = 0;
    while( 0 != (pvData = clArena.Allocate(1)) )
    {
        u16MaxAllocs++;
    }

    // Re-init the heap...
    clArena.Init(au8Arena,ARENA_SIZE);

    // Find the maximum heap block size.
    u16Min = 0;
    u16Max = 65534;
    u16Avg = (u16Min + u16Max + 1) / 2;

    while((u16Max - u16Min) >= 2)
    {
        if( 0 != (pvData = clArena.Allocate(u16Avg)) )
        {
            clArena.Free(pvData);
            // Too low
            u16Min = u16Avg;
        }
        else
        {
            // Too high
            u16Max = u16Avg;
        }
        u16Avg = (u16Min + u16Max + 1) / 2;
    }

    //Disambiguate between min/max
    if( 0 == (pvData = clArena.Allocate(u16Max)))
    {
        u16MaxAllocSize = u16Min;
    } else {
        u16MaxAllocSize = u16Max;
        clArena.Free(pvData);
    }

    clArena.Init(au8Arena,ARENA_SIZE);

    // This test was more for getting the limits, we care less about the actual
    // results, but will take the opportunity to do a rough check.
    EXPECT_GT( u16MaxAllocSize, 0 );
    EXPECT_LT( u16MaxAllocSize, 65535 );

    EXPECT_GT( u16MaxAllocs, 0 );
    EXPECT_LT( u16MaxAllocs, MAX_ALLOCS );

}
TEST_END

//===========================================================================
// This test checks that we don't run into problems alloc'ing and freeing
// data in differet locations.  This tests the list data structures primarily,
// as well as bookkeeping.
TEST(ut_arena_alloc_free)
{
    uint16_t i, j;

    for (j = 0; j < 100; j++)
    {
        // Alloc all/free all.
        for (i = 0; i < u16MaxAllocs; i++)
        {
            apvAllocs[i] = clArena.Allocate(1);
        }
        for (i = 0; i < u16MaxAllocs; i++)
        {
            clArena.Free((void*)apvAllocs[i]);
            apvAllocs[i] = 0;
        }
        // Alloc all, free all odd, then evens
        for (i = 0; i < u16MaxAllocs; i++)
        {
            apvAllocs[i] = clArena.Allocate(1);
        }
        for (i = 1; i < u16MaxAllocs; i+=2)
        {
            clArena.Free((void*)apvAllocs[i]);
            apvAllocs[i] = 0;
        }
        for (i = 0; i < u16MaxAllocs; i+=2)
        {
            clArena.Free((void*)apvAllocs[i]);
            apvAllocs[i] = 0;
        }

        // Alloc all, free in step-3
        for (i = 0; i < u16MaxAllocs; i++)
        {
            apvAllocs[i] = clArena.Allocate(1);
        }
        for (i = 1; i < u16MaxAllocs; i+=3)
        {
            clArena.Free((void*)apvAllocs[i]);
            apvAllocs[i] = 0;
        }
        for (i = 0; i < u16MaxAllocs; i+=3)
        {
            clArena.Free((void*)apvAllocs[i]);
            apvAllocs[i] = 0;
        }
        for (i = 2; i < u16MaxAllocs; i+=3)
        {
            clArena.Free((void*)apvAllocs[i]);
            apvAllocs[i] = 0;
        }

        // free in non-sequential order...
        for(i = 0; i < u16MaxAllocs; i+=2)
        {
            apvAllocs[i] = clArena.Allocate(1);
        }
        for(i = 1; i < u16MaxAllocs; i+=2)
        {
            apvAllocs[i] = clArena.Allocate(1);
        }
        for (i = 0; i < u16MaxAllocs; i++)
        {
            clArena.Free((void*)apvAllocs[i]);
            apvAllocs[i] = 0;
        }

        // Test point - we didn't crash out magnificently doing low-level
        // memory management.
        EXPECT_TRUE(1);
    }
}
TEST_END

//===========================================================================
void HeapScriptTest(void *pvParam_)
{
    uint16_t u16Index = ((uint16_t)pvParam_);
    uint16_t i;

    void *pvData;

    while(1)
    {
        for (i = u16Index; i < u16MaxAllocs; i+=2)
        {
            apvAllocs[i] = clArena.Allocate(1);
        }
        for (i = u16Index; i < u16MaxAllocs; i+=2)
        {
            clArena.Free((void*)apvAllocs[i]);
            apvAllocs[i] = 0;
        }
        for (i = u16Index; i < u16MaxAllocs; i+=2)
        {
            apvAllocs[i] = clArena.Allocate(1);
            clArena.Free((void*)apvAllocs[i]);
            apvAllocs[i] = 0;
        }
        for (i = 0; i < 200; i++)
        {
            switch(i & 7)
            {
                case 0:
                case 2:
                case 6:
                    pvData = clArena.Allocate(u16MaxAllocSize);
                    if (pvData)
                    {
                        MemUtil::SetMemory(pvData, 0xFF, u16MaxAllocSize);
                        clArena.Free(pvData);
                    }
                    break;
                case 1:
                case 3:
                case 5:
                    pvData = clArena.Allocate(ARENA_SIZE_0);
                    if (pvData)
                    {
                        MemUtil::SetMemory(pvData, 0xFF, ARENA_SIZE_0);
                        clArena.Free(pvData);
                    }
                    break;
                case 7:
                    pvData = clArena.Allocate(ARENA_SIZE_1);
                    if (pvData)
                    {
                        MemUtil::SetMemory(pvData, 0xFF, ARENA_SIZE_1);
                        clArena.Free(pvData);
                    }
                    break;
                default:
                    break;
            }
        }
    }
}

//===========================================================================
Thread clTestThread1;
Thread clTestThread2;

K_WORD aucTestStack1[TEST_STACK_SIZE];
K_WORD aucTestStack2[TEST_STACK_SIZE];

//===========================================================================
// Test out how the heap handles constant, multi-threaded access.
TEST(ut_arena_multithread)
{
    // Create two test threads, have them do nothing but allocate and free
    // data according to a script.  Give each thread a different quantum to
    // ensure the allocation/free patterns are exercised more rigorously.
    // Note that there's no interaction between objects alloc'd in one thread
    // and free'd in another

    clTestThread1.Init( aucTestStack1, TEST_STACK_SIZE, 1, HeapScriptTest, (void*)0);
    clTestThread2.Init( aucTestStack2, TEST_STACK_SIZE, 1, HeapScriptTest, (void*)1);

    Scheduler::GetCurrentThread()->SetPriority(7);

    clTestThread1.SetQuantum(7);
    clTestThread2.SetQuantum(13);

    Scheduler::GetCurrentThread()->SetPriority(7);

    clTestThread1.Start();
    clTestThread2.Start();

    for (int i = 0; i < 10; i++)
    {
        Thread::Sleep(500);

        // 1 point for each 500ms of testing
        EXPECT_TRUE(1);
    }
    Scheduler::GetCurrentThread()->SetPriority(1);

    clTestThread1.Exit();
    clTestThread2.Exit();
}
TEST_END

//===========================================================================
// Test Whitelist Goes Here
//===========================================================================
TEST_CASE_START
  TEST_CASE(ut_arena_calibrate),
  TEST_CASE(ut_arena_alloc_free),
  TEST_CASE(ut_arena_multithread),
TEST_CASE_END
