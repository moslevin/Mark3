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
#include "mark3c.h"

/*===========================================================================

Lab Example 5:  Using Mutexes.

Lessons covered in this example include:
-You can use mutexes to lock accesses to a shared resource

Takeaway:

===========================================================================*/

//---------------------------------------------------------------------------
// This block declares the thread data for one main application thread.  It
// defines a thread object, stack (in word-array form), and the entry-point
// function used by the application thread.
#define APP1_STACK_SIZE      (320/sizeof(K_WORD))
DECLARE_THREAD(hApp1Thread);
static K_WORD  awApp1Stack[APP1_STACK_SIZE];
static void    App1Main(void *unused_);

//---------------------------------------------------------------------------
// This block declares the thread data for one main application thread.  It
// defines a thread object, stack (in word-array form), and the entry-point
// function used by the application thread.
#define APP2_STACK_SIZE      (320/sizeof(K_WORD))
DECLARE_THREAD(hApp2Thread);
static K_WORD  awApp2Stack[APP2_STACK_SIZE];
static void    App2Main(void *unused_);

//---------------------------------------------------------------------------
// This is the mutex that we'll use to synchronize two threads in this
// demo application.
DECLARE_MUTEX(hMyMutex);

// This counter variable is the "shared resource" in the example, protected
// by the mutex.  Only one thread should be given access to the counter at
// any time.
static volatile uint32_t ulCounter = 0;

//---------------------------------------------------------------------------
int main(void)
{
    // See the annotations in previous labs for details on init.
    Kernel_Init();

    Thread_Init( hApp1Thread, awApp1Stack,  APP1_STACK_SIZE,  1, App1Main,  0);
    Thread_Init( hApp2Thread, awApp2Stack,  APP2_STACK_SIZE,  1, App2Main,  0);

    Thread_Start( hApp1Thread );
    Thread_Start( hApp2Thread );

    // Initialize the mutex used in this example.
    Mutex_Init( hMyMutex );

    Kernel_Start();

    return 0;
}

//---------------------------------------------------------------------------
void App1Main(void *unused_)
{
    while(1)
    {
        // Claim the mutex.  This will prevent any other thread from claiming
        // this lock simulatenously.  As a result, the other thread has to
        // wait until we're done before it can do its work.  You will notice
        // that the Start/Done prints for the thread will come as a pair (i.e.
        // you won't see "Thread2: Start" then "Thread1: Start").

        Mutex_Claim( hMyMutex );
        
        // Start our work (incrementing a counter).  Notice that the Start and
        // Done prints wind up as a pair when simuated with flAVR.

        KernelAware_Print("Thread1: Start\n");
        ulCounter++;
        while (ulCounter <= 10000)
        {
            ulCounter++;
        }
        ulCounter = 0;
        KernelAware_Print("Thread1: Done\n");

        // Release the lock, allowing the other thread to do its thing.
        Mutex_Release( hMyMutex );
    }
}

//---------------------------------------------------------------------------
void App2Main(void *unused_)
{
    while(1)
    {
        // Claim the mutex.  This will prevent any other thread from claiming
        // this lock simulatenously.  As a result, the other thread has to
        // wait until we're done before it can do its work.  You will notice
        // that the Start/Done prints for the thread will come as a pair (i.e.
        // you won't see "Thread2: Start" then "Thread1: Start").
        Mutex_Claim( hMyMutex );
        
        // Start our work (incrementing a counter).  Notice that the Start and
        // Done prints wind up as a pair when simuated with flAVR.

        KernelAware_Print("Thread2: Start\n");
        ulCounter++;
        while (ulCounter <= 10000)
        {
            ulCounter++;
        }
        ulCounter = 0;
        KernelAware_Print("Thread2: Done\n");

        // Release the lock, allowing the other thread to do its thing.
        Mutex_Release( hMyMutex );
    }
}

