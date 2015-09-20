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
#include "mark3.h"

//===========================================================================
// Local Defines
//===========================================================================
static Notify clNotify;
static Thread clThread;
static K_WORD awStack[192];
static volatile uint8_t u8Count = 0;

static void NotifyThread(void *unused_)
{
    while(1)
    {
        Thread::Sleep(50);
        clNotify.Signal();
        u8Count++;
    }
}

//===========================================================================
// Define Test Cases Here
//===========================================================================
TEST(ut_notify)
{
    clThread.Init(awStack, 192, 2, NotifyThread, NULL);
    clThread.Start();

    for (int i = 0; i < 10; i++)
    {
        clNotify.Wait(0);
    }
    clThread.Stop();
    EXPECT_EQUALS(u8Count, 10);
}
TEST_END


//===========================================================================
// Define Test Cases Here
//===========================================================================
TEST(ut_notify_timeout)
{
    clThread.Start();
    u8Count = 0;
    for (int i = 0; i < 10; i++)
    {
        clNotify.Wait(100, 0);
    }
    clThread.Stop();
    EXPECT_EQUALS(u8Count, 10);


    clThread.Start();
    u8Count = 0;
    for (int i = 0; i < 10; i++)
    {
        clNotify.Wait(3, 0);
    }
    clThread.Stop();
    EXPECT_EQUALS(u8Count, 0);
}
TEST_END

//===========================================================================
// Test Whitelist Goes Here
//===========================================================================
TEST_CASE_START
  TEST_CASE(ut_notify),
  TEST_CASE(ut_notify_timeout),
TEST_CASE_END
