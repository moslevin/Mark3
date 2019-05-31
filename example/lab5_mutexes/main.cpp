/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012 - 2019 m0slevin, all rights reserved.
See license.txt for more information
===========================================================================*/
#include "mark3.h"

/*===========================================================================

Lab Example 5:  using Mutexes.

Lessons covered in this example include:
-You can use mutexes to lock accesses to a shared resource

Takeaway:

===========================================================================*/
extern "C" {
void __cxa_pure_virtual(void) {}
void DebugPrint(const char* szString_);
}
namespace
{
using namespace Mark3;

//---------------------------------------------------------------------------
// This block declares the thread data for one main application thread.  It
// defines a thread object, stack (in word-array form), and the entry-point
// function used by the application thread.
#define APP1_STACK_SIZE (PORT_KERNEL_DEFAULT_STACK_SIZE)
Thread clApp1Thread;
K_WORD awApp1Stack[APP1_STACK_SIZE];
void   App1Main(void* unused_);

//---------------------------------------------------------------------------
// This block declares the thread data for one main application thread.  It
// defines a thread object, stack (in word-array form), and the entry-point
// function used by the application thread.
#define APP2_STACK_SIZE (PORT_KERNEL_DEFAULT_STACK_SIZE)
Thread clApp2Thread;
K_WORD awApp2Stack[APP2_STACK_SIZE];
void   App2Main(void* unused_);

//---------------------------------------------------------------------------
// idle thread -- do nothing
Thread clIdleThread;
K_WORD awIdleStack[PORT_KERNEL_DEFAULT_STACK_SIZE];
void   IdleMain(void* /*unused_*/)
{
    while (1) {}
}

//---------------------------------------------------------------------------
// This is the mutex that we'll use to synchronize two threads in this
// demo application.
Mutex clMyMutex;

// This counter variable is the "shared resource" in the example, protected
// by the mutex.  Only one thread should be given access to the counter at
// any time.
volatile uint32_t u32Counter = 0;

//---------------------------------------------------------------------------
void App1Main(void* unused_)
{
    while (1) {
        // Claim the mutex.  This will prevent any other thread from claiming
        // this lock simulatenously.  As a result, the other thread has to
        // wait until we're done before it can do its work.  You will notice
        // that the Start/Done prints for the thread will come as a pair (i.e.
        // you won't see "Thread2: Start" then "Thread1: Start").

        clMyMutex.Claim();

        // Start our work (incrementing a counter).  Notice that the Start and
        // Done prints wind up as a pair when simuated with flAVR.

        Kernel::DebugPrint("Thread1: Start\n");
        u32Counter++;
        while (u32Counter <= 1000000) { u32Counter++; }
        u32Counter = 0;
        Kernel::DebugPrint("Thread1: Done\n");

        // Release the lock, allowing the other thread to do its thing.
        clMyMutex.Release();
    }
}

//---------------------------------------------------------------------------
void App2Main(void* unused_)
{
    while (1) {
        // Claim the mutex.  This will prevent any other thread from claiming
        // this lock simulatenously.  As a result, the other thread has to
        // wait until we're done before it can do its work.  You will notice
        // that the Start/Done prints for the thread will come as a pair (i.e.
        // you won't see "Thread2: Start" then "Thread1: Start").

        clMyMutex.Claim();

        // Start our work (incrementing a counter).  Notice that the Start and
        // Done prints wind up as a pair when simuated with flAVR.

        Kernel::DebugPrint("Thread2: Start\n");
        u32Counter++;
        while (u32Counter <= 1000000) { u32Counter++; }
        u32Counter = 0;
        Kernel::DebugPrint("Thread2: Done\n");

        // Release the lock, allowing the other thread to do its thing.
        clMyMutex.Release();
    }
}
} // anonymous namespace

using namespace Mark3;

//---------------------------------------------------------------------------
int main(void)
{
    // See the annotations in previous labs for details on init.
    Kernel::Init();
    Kernel::SetDebugPrintFunction(DebugPrint);

    clIdleThread.Init(awIdleStack, sizeof(awIdleStack), 0, IdleMain, 0);
    clIdleThread.Start();

    clApp1Thread.Init(awApp1Stack, sizeof(awApp1Stack), 1, App1Main, 0);
    clApp2Thread.Init(awApp2Stack, sizeof(awApp2Stack), 1, App2Main, 0);

    clApp1Thread.Start();
    clApp2Thread.Start();

    // Initialize the mutex used in this example.
    clMyMutex.Init();

    Kernel::Start();

    return 0;
}
