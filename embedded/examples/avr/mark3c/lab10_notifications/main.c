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
#include "mark3c.h"

/*===========================================================================

Lab Example 10:  Thread Notifications

Lessons covered in this example include:
- Create a notification object, and use it to synchronize execution of Threads.

Takeaway:
- Notification objects are a lightweight mechanism to signal thread execution
  in situations where even a semaphore would be a heavier-weigth option.

===========================================================================*/
#if !KERNEL_USE_IDLE_FUNC
# error "This demo requires KERNEL_USE_IDLE_FUNC"
#endif

//---------------------------------------------------------------------------
#define APP_STACK_SIZE      (256/sizeof(K_WORD))
DECLARE_THREAD(hApp1Thread);
static K_WORD  awApp1Stack[APP_STACK_SIZE];
static void    App1Main(void *unused_);

//---------------------------------------------------------------------------
DECLARE_THREAD(hApp2Thread);
static K_WORD  awApp2Stack[APP_STACK_SIZE];
static void    App2Main(void *unused_);

//---------------------------------------------------------------------------
// Notification object used in the example.
DECLARE_NOTIFY(hNotify);

//---------------------------------------------------------------------------
int main(void)
{
    // See the annotations in previous labs for details on init.
    Kernel_Init();

    // Initialize notifer and notify-ee threads
    Thread_Init(hApp1Thread, awApp1Stack,  sizeof(awApp1Stack),  1, App1Main,  0);
    Thread_Start(hApp1Thread);

    Thread_Init(hApp2Thread, awApp2Stack,  sizeof(awApp2Stack),  1, App2Main,  0);
    Thread_Start(hApp2Thread);

    // Initialize the Notify objects
    Notify_Init(hNotify);

    Kernel_Start();

    return 0;
}

//---------------------------------------------------------------------------
void App1Main(void *unused_)
{
    while (1)
    {
        bool bNotified = false;
        // Block the thread until the notification object is signalled from
        // elsewhere.
        Notify_Wait(hNotify, &bNotified);

        KernelAware_Print("T1: Notified\n");
    }
}


//---------------------------------------------------------------------------
void App2Main(void *unused_)
{
    while (1)
    {
        // Wait a while, then signal the notification object

        KernelAware_Print("T2: Wait 1s\n");
        Thread_Sleep(1000);

        KernelAware_Print("T2: Notify\n");
        Notify_Signal(hNotify);
    }
}
