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

Lab Example 2:

The following example demonstrates how the Mark3 Coroutine Scheudler can be
used to implement cooperative scheduling in the system.

Cooperatively-scheduled tasks run to completion and are not pre-emptable,
unlike threads.  They can, however be assigned a priority to ensure that
the highest-priority tasks are executed first.  Cooperatively-scheduled
tasks should not block, but may use other Mark3 kernel APIs that cause other
threads to unblock (post semaphores, send messages, etc.).  Note that the
cooperatively-scheduled tasks are also intended to be run from a single
thread context - and so the notion of priority is relative only to the set
of coroutines in the system, and is unrelated to the priority of the threads
in the system.

Lessons covered in this example include:

- Initialize the coroutine scheduler and run the scheduler loop from the
idle thread.
- Activate and run coroutines based on stimulus from outside the thread
running the coroutine scheduler.

Exercise:

- Add a coroutine that is activated by an interrupt
- Add a coroutine that is activated by a timer

Takeaway:

Coroutines are an effective way of providing cooperative multitasking in a
system for tasks that do not require pre-emption, and can benefit from having
relative priorities.

===========================================================================*/
extern "C" {
void __cxa_pure_virtual(void) {}
void DebugPrint(const char* szString_);
}

namespace
{
using namespace Mark3;
//---------------------------------------------------------------------------
// This block declares the thread data for the main application thread.  It
// defines a thread object, stack (in word-array form), and the entry-point
// function used by the application thread.
Thread clAppThread;
K_WORD awAppStack[PORT_KERNEL_DEFAULT_STACK_SIZE / sizeof(K_WORD)];
void   AppMain(void* unused_);

//---------------------------------------------------------------------------
// This block declares the thread data for the idle thread.  It defines a
// thread object, stack (in word-array form), and the entry-point function
// used by the idle thread.
Thread clIdleThread;
K_WORD awIdleStack[PORT_KERNEL_DEFAULT_STACK_SIZE / sizeof(K_WORD)];
void   IdleMain(void* unused_);

//---------------------------------------------------------------------------
// Declare 3 co-routines, each with a variable which will be passed into its
// handle function.
Coroutine clCoroutine1;
int iCounter1;

Coroutine clCoroutine2;
int iCounter2;

Coroutine clCoroutine3;
int iCounter3;

//---------------------------------------------------------------------------
// Declare handler functions used by the three coroutines.  In this example,
// the coroutine tasks simply increments a counter and print their identity.
void CoroutineTask1(Coroutine* pclCaller_, void* pvArg_)
{
    Kernel::DebugPrint("Task1\n");
    int* piCounter = static_cast<int*>(pvArg_);
    (*piCounter)++;
}

//---------------------------------------------------------------------------
void CoroutineTask2(Coroutine* pclCaller_, void* pvArg_)
{
    Kernel::DebugPrint("Task2\n");
    int* piCounter = static_cast<int*>(pvArg_);
    (*piCounter)++;
}

//---------------------------------------------------------------------------
void CoroutineTask3(Coroutine* pclCaller_, void* pvArg_)
{
    Kernel::DebugPrint("Task3\n");
    int* piCounter = static_cast<int*>(pvArg_);
    (*piCounter)++;
}

//---------------------------------------------------------------------------
void AppMain(void* unused_)
{
    // In this example, the application task is responsible for activating the
    // various coroutines in a round-robin fashion.
    while (1) {
        Thread::Sleep(100);
        clCoroutine1.Activate();
        Thread::Sleep(100);
        clCoroutine2.Activate();
        Thread::Sleep(100);
        clCoroutine3.Activate();
    }
}

//---------------------------------------------------------------------------
void IdleMain(void* unused_)
{
    while (1) {
        // In this example, use the Idle context to run coroutines as they
        // are scheduled
        auto* pclCoRoutine = CoScheduler::Schedule();
        if (pclCoRoutine != nullptr) {
            pclCoRoutine->Run();
        }
    }
}
} // anonymous namespace

using namespace Mark3;
//---------------------------------------------------------------------------
int main(void)
{
    // See Lab1 for detailed description of the kernel + initial thread
    // bringup sequence.

    Kernel::Init();
    Kernel::SetDebugPrintFunction(DebugPrint);

    CoScheduler::Init();
    clCoroutine1.Init(1, CoroutineTask1, &iCounter1);
    clCoroutine2.Init(2, CoroutineTask2, &iCounter2);
    clCoroutine3.Init(3, CoroutineTask3, &iCounter3);

    clAppThread.Init(awAppStack, sizeof(awAppStack), 1, AppMain, 0);
    clIdleThread.Init(awIdleStack, sizeof(awIdleStack), 0, IdleMain, 0);

    clAppThread.Start();
    clIdleThread.Start();

    Kernel::Start();

    return 0;
}
