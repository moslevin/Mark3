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
//===========================================================================
// Local Defines
//===========================================================================
Notify           clNotify;
Thread           clThread;
K_WORD           awStack[PORT_KERNEL_DEFAULT_STACK_SIZE];
volatile uint8_t u8Count = 0;
}

namespace Mark3
{
//===========================================================================
// Define Test Cases Here
//===========================================================================
TEST(ut_notify)
{
    clNotify.Init();

    auto lNotifyFunc = [](void* param_) {
        while (1) {
            Thread::Sleep(50);
            u8Count++;
            clNotify.Signal();
        }
    };

    clThread.Init(awStack, PORT_KERNEL_DEFAULT_STACK_SIZE, 2, lNotifyFunc, nullptr);
    clThread.Start();
    for (int i = 0; i < 10; i++) { clNotify.Wait(0); }
    clThread.Stop();
    EXPECT_EQUALS(u8Count, 10);
}

//===========================================================================
// Define Test Cases Here
//===========================================================================
TEST(ut_notify_timeout)
{
    clNotify.Init();

    u8Count = 0;
    clThread.Start();
    for (int i = 0; i < 10; i++) { clNotify.Wait(100, 0); }
    clThread.Stop();
    EXPECT_EQUALS(u8Count, 10);

    clThread.Start();
    u8Count = 0;
    for (int i = 0; i < 10; i++) { clNotify.Wait(3, 0); }
    clThread.Stop();
    EXPECT_EQUALS(u8Count, 0);
}

//===========================================================================
// Test Whitelist Goes Here
//===========================================================================
TEST_CASE_START
TEST_CASE(ut_notify), TEST_CASE(ut_notify_timeout), TEST_CASE_END
} // namespace Mark3
