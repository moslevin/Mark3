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
#include "mark3.h"

/*===========================================================================

Lab Example 6:  using Periodic and One-shot timers.

Lessons covered in this example include:

Takeaway:

===========================================================================*/
extern "C" {
void __cxa_pure_virtual(void) { }
}

//---------------------------------------------------------------------------
// This block declares the thread data for one main application thread.  It
// defines a thread object, stack (in word-array form), and the entry-point
// function used by the application thread.
#define APP1_STACK_SIZE      (320/sizeof(K_WORD))
static Thread  clApp1Thread;
static K_WORD  awApp1Stack[APP1_STACK_SIZE];
static void    App1Main(void *unused_);

//---------------------------------------------------------------------------
static void PeriodicCallback(Thread *owner, void *pvData_);
static void OneShotCallback(Thread *owner, void *pvData_);

//---------------------------------------------------------------------------
int main(void)
{
    // See the annotations in previous labs for details on init.
    Kernel::Init();

    clApp1Thread.Init(  awApp1Stack,  sizeof(awApp1Stack),  1, App1Main,  0);

    clApp1Thread.Start();

    Kernel::Start();

    return 0;
}

//---------------------------------------------------------------------------
void PeriodicCallback(Thread *owner, void *pvData_)
{
    // Timer callback function used to post a semaphore.  Posting the semaphore
    // will wake up a thread that's pending on that semaphore.
    Semaphore *pclSem = (Semaphore*)pvData_;
    pclSem->Post();
}

//---------------------------------------------------------------------------
void OneShotCallback(Thread *owner, void *pvData_)
{
    KernelAware::Print("One-shot timer expired.\n");
}

//---------------------------------------------------------------------------
void App1Main(void *unused_)
{
    Timer       clMyTimer;  // Periodic timer object
    Timer       clOneShot;  // One-shot timer object

    Semaphore   clMySem;    // Semaphore used to wake this thread

    // Initialize a binary semaphore (maximum value of one, initial value of
    // zero).
    clMySem.Init(0,1);

    // Start a timer that triggers every 500ms that will call PeriodicCallback.
    // This timer simulates an external stimulus or event that would require
    // an action to be taken by this thread, but would be serviced by an
    // interrupt or other high-priority context.

    // PeriodicCallback will post the semaphore which wakes the thread
    // up to perform an action.  Here that action consists of a trivial message
    // print.
    clMyTimer.Start(true, 500, PeriodicCallback, (void*)&clMySem);

    // Set up a one-shot timer to print a message after 2.5 seconds, asynchronously
    // from the execution of this thread.
    clOneShot.Start(false, 2500, OneShotCallback, 0);

    while(1)
    {
        // Wait until the semaphore is posted from the timer expiry
        clMySem.Pend();

        // Take some action after the timer posts the semaphore to wake this
        // thread.
        KernelAware::Print("Thread Triggered.\n");
    }
}

