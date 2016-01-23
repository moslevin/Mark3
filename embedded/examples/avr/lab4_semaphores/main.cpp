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

Lab Example 4:  using binary semaphores

In this example, we implement two threads, synchronized using a semaphore to
model the classic producer-consumer pattern.  One thread does work, and then
posts the semaphore indicating that the other thread can consume that work.
The blocking thread just waits idly until there is data for it to consume.

Lessons covered in this example include:
-Use of a binary semaphore to implement the producer-consumer pattern
-Synchronization of threads (within a single priority, or otherwise)
 using a semaphore

Takeaway:

Semaphores can be used to control which threads execute at which time.  This
allows threads to work cooperatively to achieve a goal in the system.

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
// This block declares the thread data for one main application thread.  It
// defines a thread object, stack (in word-array form), and the entry-point
// function used by the application thread.
#define APP2_STACK_SIZE      (320/sizeof(K_WORD))
static Thread  clApp2Thread;
static K_WORD  awApp2Stack[APP2_STACK_SIZE];
static void    App2Main(void *unused_);

//---------------------------------------------------------------------------
// This is the semaphore that we'll use to synchronize two threads in this
// demo application
static Semaphore   clMySem;

//---------------------------------------------------------------------------
int main(void)
{
    // See the annotations in previous labs for details on init.
    Kernel::Init();

    // In this example we create two threads to illustrate the use of a
    // binary semaphore as a synchronization method between two threads.

    // Thread 1 is a "consumer" thread -- It waits, blocked on the semaphore
    // until thread 2 is done doing some work.  Once the semaphore is posted,
    // the thread is unblocked, and does some work.

    // Thread 2 is thus the "producer" thread -- It does work, and once that
    // work is done, the semaphore is posted to indicate that the other thread
    // can use the producer's work product.

    clApp1Thread.Init(  awApp1Stack,  APP1_STACK_SIZE,  1, App1Main,  0);
    clApp2Thread.Init(  awApp2Stack,  APP2_STACK_SIZE,  1, App2Main,  0);

    clApp1Thread.Start();
    clApp2Thread.Start();

    // Initialize a binary semaphore (maximum value of one, initial value of
    // zero).
    clMySem.Init(0,1);

    Kernel::Start();

    return 0;
}

//---------------------------------------------------------------------------
void App1Main(void *unused_)
{
    while(1)
    {
        // Wait until the semaphore is posted from the other thread
        KernelAware::Print("Wait\n");
        clMySem.Pend();

        // Producer thread has finished doing its work -- do something to
        // consume its output.  Once again - a contrived example, but we
        // can imagine that printing out the message is "consuming" the output
        // from the other thread.
        KernelAware::Print("Triggered!\n");
    }
}

//---------------------------------------------------------------------------
void App2Main(void *unused_)
{
    volatile uint32_t u32Counter = 0;

    while(1)
    {
        // Do some work.  Once the work is complete, post the semaphore.  This
        // will cause the other thread to wake up and then take some action.
        // It's a bit contrived, but imagine that the results of this process
        // are necessary to drive the work done by that other thread.
        u32Counter++;
        if (u32Counter == 1000000)
        {
            u32Counter = 0;
            KernelAware::Print("Posted\n");
            clMySem.Post();
        }
    }
}

