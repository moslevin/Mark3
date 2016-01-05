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
#include "mark3.h"

/*===========================================================================

Lab Example 10:  Thread Notifications

Lessons covered in this example include:
- Create a notification object, and use it to synchronize execution of Threads.

Takeaway:
- Notification objects are a lightweight mechanism to signal thread execution
  in situations where even a semaphore would be a heavier-weigth option.

===========================================================================*/
extern "C" {
void __cxa_pure_virtual(void) { }
}

//---------------------------------------------------------------------------
#define APP_STACK_SIZE      (256/sizeof(K_WORD))
static Thread  clApp1Thread;
static K_WORD  awApp1Stack[APP_STACK_SIZE];
static void    App1Main(void *unused_);

//---------------------------------------------------------------------------
static Thread  clApp2Thread;
static K_WORD  awApp2Stack[APP_STACK_SIZE];
static void    App2Main(void *unused_);

//---------------------------------------------------------------------------
// Notification object used in the example.
static Notify  clNotify;

//---------------------------------------------------------------------------
int main(void)
{
    // See the annotations in previous labs for details on init.
    Kernel::Init();

    // Initialize notifer and notify-ee threads
    clApp1Thread.Init(  awApp1Stack,  APP_STACK_SIZE,  1, App1Main,  0);
    clApp1Thread.Start();

    clApp2Thread.Init(  awApp2Stack,  APP_STACK_SIZE,  1, App2Main,  0);
    clApp2Thread.Start();

    // Initialize the Notify objects
    clNotify.Init();

    Kernel::Start();

    return 0;
}

//---------------------------------------------------------------------------
void App1Main(void *unused_)
{
    while (1)
    {
        // Block the thread until the notification object is signalled from
        // elsewhere.
        clNotify.Wait();

        KernelAware::Print("T1: Notified\n");
    }
}


//---------------------------------------------------------------------------
void App2Main(void *unused_)
{
    while (1)
    {
        // Wait a while, then signal the notification object

        KernelAware::Print("T2: Wait 1s\n");
        Thread::Sleep(1000);

        KernelAware::Print("T2: Notify\n");
        clNotify.Signal();
    }
}
