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
/*!

    \file   threadport.cpp   

    \brief  MSP430 Multithreading

*/

#include "kerneltypes.h"
#include "mark3cfg.h"
#include "thread.h"
#include "threadport.h"
#include "kernelswi.h"
#include "kerneltimer.h"
#include "timerlist.h"
#include "quantum.h"
#include "kernel.h"
#include "kernelaware.h"

#include <msp430.h>
#include <in430.h>

//---------------------------------------------------------------------------
volatile K_UCHAR g_ucCSCount;
volatile K_USHORT g_usSR;
//---------------------------------------------------------------------------
void ThreadPort::InitStack(Thread *pclThread_)
{
    // Initialize the stack for a Thread
    K_USHORT usAddr;
    K_USHORT *pusStack;
    K_USHORT i;

    // Get the address of the thread's entry function
    usAddr = (K_USHORT)(pclThread_->m_pfEntryPoint);

    // Start by finding the bottom of the stack
    pusStack = (K_USHORT*)pclThread_->m_pwStackTop;

    // clear the stack, and initialize it to a known-default value (easier
    // to debug when things go sour with stack corruption or overflow)
    for (i = 0; i < pclThread_->m_usStackSize; i++)
    {
        pclThread_->m_pwStack[i] = 0xFFFF;
    }

    // 1st - push start address... (R0/PC)
    PUSH_TO_STACK(pusStack, usAddr);

    //!! Note - R1 is the dedicted stack pointer (not in context)

    // 2nd - Interrupts enabled status register (R2/SR)
    PUSH_TO_STACK(pusStack, 0x08);

    //!! Note - R3 is a zero register (not in context)

    // Push other registers  (R4-R11)
    for (i = 4; i < 12; i++)
    {
        PUSH_TO_STACK(pusStack, i);
    }

    // Function parameter (R12)
    PUSH_TO_STACK(pusStack, (K_USHORT)pclThread_->m_pvArg);

    // Push other registers (R13-R15)
    for (i = 13; i < 16; i++)
    {
        PUSH_TO_STACK(pusStack, i);
    }

    // Set the top o' the stack.
    pclThread_->m_pwStackTop = (K_USHORT*)(pusStack+1);
}

//---------------------------------------------------------------------------
static void Thread_Switch(void)
{
#if KERNEL_USE_IDLE_FUNC
    // If there's no next-thread-to-run...
    if (g_pstNext == Kernel::GetIdleThread())
    {
        g_pstCurrent = Kernel::GetIdleThread();

        // Disable the SWI, and re-enable interrupts -- enter nested interrupt
        // mode.
        KernelSWI::DI();

        // So long as there's no "next-to-run" thread, keep executing the Idle
        // function to conclusion...
        while (g_pstNext == Kernel::GetIdleThread())
        {
           // Ensure that we run this block in an interrupt enabled context (but
           // with the rest of the checks being performed in an interrupt disabled
           // context).
           __eint();
           Kernel::IdleFunc();
           __dint();
        }

        // Progress has been achieved -- an interrupt-triggered event has caused
        // the scheduler to run, and choose a new thread.  Since we've already
        // saved the context of the thread we've hijacked to run idle, we can
        // proceed to disable the nested interrupt context and switch to the
        // new thread.
        KernelSWI::RI( true );
    }
#endif
    KernelSWI::Clear();
    g_pstCurrent = (Thread*)g_pstNext;
}


//---------------------------------------------------------------------------
void ThreadPort::StartThreads()
{
    KernelSWI::Config();                 // configure the task switch SWI
    KernelTimer::Config();               // configure the kernel timer
    
    Scheduler::SetScheduler(1);          // enable the scheduler
    Scheduler::Schedule();               // run the scheduler - determine the first thread to run

    Thread_Switch();                     // Set the next scheduled thread to the current thread

#if !KERNEL_TIMERS_TICKLESS
    KernelTimer::Start();                // enable the kernel timer
#endif
    KernelSWI::Start();                  // enable the task switch SWI

    g_ucCSCount = 0;                     // Reset the critical section counter
    g_usSR = 0;
    // Restore the context...
    Thread_RestoreContext();        // restore the context of the first running thread
    ASM("reti");                    // return from interrupt - will return to the first scheduled thread
}

//---------------------------------------------------------------------------
/*!

*/
//---------------------------------------------------------------------------
void __attribute__ ((__interrupt__(PORT1_VECTOR), naked))
isr_KernelSWI(void)
{
    Thread_SaveContext();       // Push the context (registers) of the current task
    Thread_Switch();            // Switch to the next task
    Thread_RestoreContext();    // Pop the context (registers) of the next task
    ASM("reti");                // Return to the next task
}

//---------------------------------------------------------------------------
/*!

*/
//---------------------------------------------------------------------------
void __attribute__ ((__interrupt__(TIMERA0_VECTOR)))
isr_KernelTIMER(void)
{
#if KERNEL_USE_TIMERS    
    TimerScheduler::Process();
#endif    
#if KERNEL_USE_QUANTUM    
    Quantum::UpdateTimer();
#endif
}

