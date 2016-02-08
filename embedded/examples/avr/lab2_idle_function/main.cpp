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

Lab Example 2: Initializing the Mark3 RTOS kernel with one thread.

The following example code presents a working example of how to initialize
the Mark3 RTOS kernel, configured to use an application thread and the special
Kernel-Idle function.  This example is functionally identical to lab1, although
it uses less memory as a result of only requiring one thread.  This example also
uses the flAVR kernel-aware module to print out messages when run through the
flAVR AVR Simulator.

Lessons covered in this example include:

- usage of the Kernel::SetIdleFunc() API
- Changing an idle thread into an idle function
- You can save a thread and a stack by using an idle function instead of a
  dedicated idle thread.

Takeaway:

The Kernel-Idle context allows you to run the Mark3 RTOS without running
a dedicated idle thread (where supported).  This results in a lower overall
memory footprint for the application, as you can avoid having to declare
a thread object and stack for Idle functionality.

===========================================================================*/
#if !KERNEL_USE_IDLE_FUNC
# error "This demo requires KERNEL_USE_IDLE_FUNC"
#endif

extern "C" {
void __cxa_pure_virtual(void) { }
}

//---------------------------------------------------------------------------
// This block declares the thread data for the main application thread.  It
// defines a thread object, stack (in word-array form), and the entry-point
// function used by the application thread.
#define APP_STACK_SIZE      (320/sizeof(K_WORD))
static Thread  clAppThread;
static K_WORD  awAppStack[APP_STACK_SIZE];
static void    AppMain(void *unused_);

//---------------------------------------------------------------------------
// This block declares the special function called from with the special
// Kernel-Idle context.  We use the Kernel::SetIdleFunc() API to ensure that
// this function is called to provide our idle context.
static void    IdleMain(void);

//---------------------------------------------------------------------------
int main(void)
{
    // See the annotations in lab1.
    Kernel::Init();

    // Initialize the main application thread, as in lab1.  Note that even
    // though we're using an Idle function and not a dedicated thread, priority
    // level 0 is still reserved for idle functionality.  Application threads
    // should never be scheduled at priority level 0 when the idle function is
    // used instead of an idle thread.
    clAppThread.Init(  awAppStack,  sizeof(awAppStack),  1, AppMain,  0);
    clAppThread.Start();

    // This function is used to install our specified idle function to be called
    // whenever there are no ready threads in the system.  Note that if no
    // Idle function is specified, a default will be used.  Note that this default
    // function is essentially a null operation.
    Kernel::SetIdleFunc(IdleMain);

    Kernel::Start();

    return 0;
}

//---------------------------------------------------------------------------
void AppMain(void *unused_)
{
    // Same as in lab1.
    while(1)
    {
        KernelAware::Print("Hello World!\n");
        Thread::Sleep(1000);
    }
}

//---------------------------------------------------------------------------
void IdleMain(void)
{
    // Low priority task + power management routines go here.
    // The actions taken in this context must *not* cause a blocking call,
    // similar to the requirements for an idle thread.

    // Note that unlike an idle thread, the idle function must run to
    // completion.  As this is also called from a nested interrupt context,
    // it's worthwhile keeping this function brief, limited to absolutely
    // necessary functionality, and with minimal stack use.
}

