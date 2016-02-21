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
#define MAX_ALLOCS          (16)
#define TEST_STACK_SIZE     (192)

static volatile void *apvAllocs[MAX_ALLOCS]; // assuming we have < 128 system heap allocs...

#define ARENA_SIZE (200)
static uint8_t au8Arena[ARENA_SIZE];
static Arena clArena;
const static K_ADDR au16Sizes[] = { 4, 8, 12, 20, 32, 52, 84, 0 };

//===========================================================================
// Define Test Cases Here

//===========================================================================
// This test checks that we don't run into problems alloc'ing and freeing
// data in differet locations.  This tests the list data structures primarily,
// as well as bookkeeping.
TEST(ut_arena_alloc_free)
{
    uint16_t i, j;
    clArena.Init(au8Arena, ARENA_SIZE, (K_ADDR*)au16Sizes, sizeof(au16Sizes)/sizeof(K_ADDR));

    for (j = 0; j < 100; j++)
    {
        // Alloc all/free all.
        for (i = 0; i < MAX_ALLOCS; i++)
        {
            apvAllocs[i] = clArena.Allocate(1);
        }
        for (i = 0; i < MAX_ALLOCS; i++)
        {
            if (apvAllocs[i] != 0) 
            {
                clArena.Free((void*)apvAllocs[i]);
                apvAllocs[i] = 0;
	    }
        }

        // Alloc all, free all odd, then evens
        for (i = 0; i < MAX_ALLOCS; i++)
        {
            apvAllocs[i] = clArena.Allocate(1);
        }
        for (i = 1; i < MAX_ALLOCS; i+=2)
        {
            if (apvAllocs[i] != 0) 
            {
                clArena.Free((void*)apvAllocs[i]);
                apvAllocs[i] = 0;
	    }
        }
        for (i = 0; i < MAX_ALLOCS; i+=2)
        {
            if (apvAllocs[i] != 0) 
            {
                clArena.Free((void*)apvAllocs[i]);
                apvAllocs[i] = 0;
	    }
        }

        // Alloc all, free in step-3
        for (i = 0; i < MAX_ALLOCS; i++)
        {
            apvAllocs[i] = clArena.Allocate(1);
        }
        for (i = 1; i < MAX_ALLOCS; i+=3)
        {
            if (apvAllocs[i] != 0) 
            {
                clArena.Free((void*)apvAllocs[i]);
                apvAllocs[i] = 0;
	    }
        }
        for (i = 0; i < MAX_ALLOCS; i+=3)
        {
            if (apvAllocs[i] != 0) 
            {
                clArena.Free((void*)apvAllocs[i]);
                apvAllocs[i] = 0;
	    }
        }
        for (i = 2; i < MAX_ALLOCS; i+=3)
        {
            if (apvAllocs[i] != 0) 
            {
                clArena.Free((void*)apvAllocs[i]);
                apvAllocs[i] = 0;
	    }
        }

        // free in non-sequential order...
        for(i = 0; i < MAX_ALLOCS; i+=2)
        {
            apvAllocs[i] = clArena.Allocate(1);
        }
        for(i = 1; i < MAX_ALLOCS; i+=2)
        {
            apvAllocs[i] = clArena.Allocate(1);
        }
        for (i = 0; i < MAX_ALLOCS; i++)
        {
            if (apvAllocs[i] != 0) 
            {
                clArena.Free((void*)apvAllocs[i]);
                apvAllocs[i] = 0;
	    }
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
        for (i = u16Index; i < MAX_ALLOCS; i+=2)
        {
            apvAllocs[i] = clArena.Allocate(1);
        }
        for (i = u16Index; i < MAX_ALLOCS; i+=2)
        {
            if (apvAllocs[i] != 0) 
            {
                clArena.Free((void*)apvAllocs[i]);
                apvAllocs[i] = 0;
	    }
        }
        for (i = u16Index; i < MAX_ALLOCS; i+=2)
        {
            apvAllocs[i] = clArena.Allocate(1);
            if (apvAllocs[i] != 0) 
            {
                clArena.Free((void*)apvAllocs[i]);
                apvAllocs[i] = 0;
	    }
        }
        for (i = 0; i < 200; i++)
        {
            switch(i & 7)
            {
                case 0:
                case 2:
                case 6:
                    pvData = clArena.Allocate(au16Sizes[3]);
                    if (pvData)
                    {
                        MemUtil::SetMemory(pvData, 0xFF, au16Sizes[3]);
                        clArena.Free(pvData);
                    }
                    break;
                case 1:
                case 3:
                case 5:
                    pvData = clArena.Allocate(au16Sizes[0]);
                    if (pvData)
                    {
                        MemUtil::SetMemory(pvData, 0xFF, au16Sizes[0]);
                        clArena.Free(pvData);
                    }
                    break;
                case 7:
                    pvData = clArena.Allocate(au16Sizes[1]);
                    if (pvData)
                    {
                        MemUtil::SetMemory(pvData, 0xFF, au16Sizes[1]);
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
  TEST_CASE(ut_arena_alloc_free),
  TEST_CASE(ut_arena_multithread),
TEST_CASE_END
