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
#include "threadport.h"

/*===========================================================================

Lab Example 1: Initializing the Mark3 RTOS kernel with two threads.

The following example code presents a working example of how to initialize
the Mark3 RTOS kernel, configure two application threads, and execute the
configured tasks.  This example also uses the flAVR kernel-aware module to
print out messages when run through the flAVR AVR Simulator.  This is a
turnkey-ready example of how to use the Mark3 RTOS at its simplest level,
and should be well understood before moving on to other examples.

Lessons covered in this example include:

- usage of the Kernel class - configuring and starting the kernel
- usage of the Thread class - initializing and starting static threads.
- Demonstrate the relationship between Thread objects, stacks, and entry
  functions.
- usage of Thread::Sleep() to block execution of a thread for a period of time
- When using an idle thread, the idle thread MUST not block.

Exercise:

- Add another application thread that prints a message, flashes an LED, etc.
  using the code below as an example.

Takeaway:

At the end of this example, the reader should be able to use the Mark3
Kernel and Thread APIs to initialize and start the kernel with any number
of static threads.

===========================================================================*/
extern "C" {
void __cxa_pure_virtual(void) { }
}

//---------------------------------------------------------------------------
// This block declares the thread data for the main application thread.  It
// defines a thread object, stack (in word-array form), and the entry-point
// function used by the application thread.
#define APP_STACK_SIZE      (1024/sizeof(K_WORD))
static Thread  clAppThread;
static K_WORD  awAppStack[APP_STACK_SIZE] __attribute__((aligned (8)));
static void    AppMain(void *unused_);

//---------------------------------------------------------------------------
// This block declares the thread data for the idle thread.  It defines a
// thread object, stack (in word-array form), and the entry-point function
// used by the idle thread.
#define IDLE_STACK_SIZE     (1024/sizeof(K_WORD))
static Thread  clIdleThread;
static K_WORD  awIdleStack[IDLE_STACK_SIZE] __attribute__((aligned (8)));
static void    IdleMain(void *unused_);

//---------------------------------------------------------------------------
int main(void)
{
    // Before any Mark3 RTOS APIs can be called, the user must call Kernel::Init().
    // Note that if you have any hardware-specific init code, it can be called
    // before Kernel::Init, so long as it does not enable interrupts, or
    // rely on hardware peripherals (timer, software interrupt, etc.) used by the
    // kernel.
    Kernel::Init();

    // Once the kernel initialization has been complete, the user can add their
    // application thread(s) and idle thread.  Threads added before the kerel
    // is started are refered to as the "static threads" in the system, as they
    // are the default working-set of threads that make up the application on
    // kernel startup.

    // Initialize the application thread to use a specified word-array as its stack.
    // The thread will run at priority level "1", and start execution the
    // "AppMain" function when it's started.
    clAppThread.Init(  awAppStack,  APP_STACK_SIZE,  1, AppMain,  0);

    // Initialize the idle thread to use a specific word-array as its stack.
    // The thread will run at priority level "0", which is reserved for the idle
    // priority thread.  IdleMain will be run when the thread is started.
    clIdleThread.Init( awIdleStack, IDLE_STACK_SIZE, 0, IdleMain, 0);

    // Once the static threads have been added, the user must then ensure that the
    // threads are ready to execute.  By default, creating a thread is created
    // in a STOPPED state.  All threads must manually be started using the
    // Start() API before they will be scheduled by the system.  Here, we are
    // starting the application and idle threads before starting the kernel - and
    // that's OK.  When the kernel is started, it will choose which thread to run
    // first from the pool of ready threads.

    clAppThread.Start();
    clIdleThread.Start();

    // All threads have been initialized and made ready.  The kernel will now
    // select the first thread to run, enable the hardware required to run the
    // kernel (Timers, software interrupts, etc.), and then do whatever is
    // necessary to maneuver control of thread execution to the kernel.  At this
    // point, execution will transition to the highest-priority ready thread.
    // This function will not return.

    Kernel::Start();

    // As Kernel::Start() results in the operating system being executed, control
    // will not be relinquished back to main().  The "return 0" is simply to
    // avoid warnings.

    return 0;
}

//---------------------------------------------------------------------------
void AppMain(void *unused_)
{
    // This function is run from within the application thread.  Here, we
    // simply print a friendly greeting and allow the thread to sleep for a
    // while before repeating the message.  Note that while the thread is
    // sleeping, CPU execution will transition to the Idle thread.

    while(1)
    {
        KernelAware::Print("Hello World!\n");
        Thread::Sleep(1000);
    }
}

//---------------------------------------------------------------------------
void IdleMain(void *unused_)
{
    while(1)
    {
	volatile uint32_t ctr = 0;
	ctr++;
        // Low priority task + power management routines go here.
        // The actions taken in this context must *not* cause the thread
        // to block, as the kernel requires that at least one thread is
        // schedulable at all times when not using an idle thread.

        // Note that if you have no special power-management code or idle
        // tasks, an empty while(1){} loop is sufficient to guarantee that
        // condition.
    }
}

