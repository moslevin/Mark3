/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012 - 2017 Funkenstein Software Consulting, all rights reserved.
See license.txt for more information
===========================================================================*/
#include "mark3.h"

/*===========================================================================

Lab Example 10:  Thread Notifications

Lessons covered in this example include:
- Create a notification object, and use it to synchronize execution of Threads.

Takeaway:
- Notification objects are a lightweight mechanism to signal thread execution
  in situations where even a semaphore would be a heavier-weigth option.

===========================================================================*/
#if !KERNEL_USE_IDLE_FUNC
#error "This demo requires KERNEL_USE_IDLE_FUNC"
#endif
extern "C" {
void __cxa_pure_virtual(void)
{
}
}

namespace {
using namespace Mark3;
//---------------------------------------------------------------------------
#define APP_STACK_SIZE (256 / sizeof(K_WORD))
Thread clApp1Thread;
K_WORD awApp1Stack[APP_STACK_SIZE];
void App1Main(void* unused_);

//---------------------------------------------------------------------------
Thread clApp2Thread;
K_WORD awApp2Stack[APP_STACK_SIZE];
void App2Main(void* unused_);

//---------------------------------------------------------------------------
// Notification object used in the example.
Notify clNotify;

//---------------------------------------------------------------------------
void App1Main(void* unused_)
{
    while (1) {
        bool bNotified = false;
        // Block the thread until the notification object is signalled from
        // elsewhere.
        clNotify.Wait(&bNotified);

        KernelAware::Print("T1: Notified\n");
    }
}

//---------------------------------------------------------------------------
void App2Main(void* unused_)
{
    while (1) {
        // Wait a while, then signal the notification object

        KernelAware::Print("T2: Wait 1s\n");
        Thread::Sleep(1000);

        KernelAware::Print("T2: Notify\n");
        clNotify.Signal();
    }
}
} // anonymous namespace

using namespace Mark3;
//---------------------------------------------------------------------------
int main(void)
{
    // See the annotations in previous labs for details on init.
    Kernel::Init();

    // Initialize notifer and notify-ee threads
    clApp1Thread.Init(awApp1Stack, sizeof(awApp1Stack), 1, App1Main, 0);
    clApp1Thread.Start();

    clApp2Thread.Init(awApp2Stack, sizeof(awApp2Stack), 1, App2Main, 0);
    clApp2Thread.Start();

    // Initialize the Notify objects
    clNotify.Init();

    Kernel::Start();

    return 0;
}
