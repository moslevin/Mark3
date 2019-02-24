/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012 - 2018 m0slevin, all rights reserved.
See license.txt for more information
===========================================================================*/
/**

    @file   threadport.cpp

    @brief  MSP430 Multithreading

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

#include <msp430.h>
#include <in430.h>

namespace Mark3
{
//---------------------------------------------------------------------------
volatile uint8_t  g_u8CSCount;
volatile uint16_t g_u16SR;
//---------------------------------------------------------------------------
void ThreadPort::InitStack(Thread* pclThread_)
{
    // Initialize the stack for a Thread
    uint16_t  u16Addr;
    uint16_t* pu16Stack;
    uint16_t  i;

    // Get the address of the thread's entry function
    u16Addr = (uint16_t)(pclThread_->m_pfEntryPoint);

    // Start by finding the bottom of the stack
    pu16Stack = (uint16_t*)pclThread_->m_pwStackTop;

#if KERNEL_STACK_CHECK
    // clear the stack, and initialize it to a known-default value (easier
    // to debug when things go sour with stack corruption or overflow)
    for (i = 0; i < pclThread_->m_u16StackSize / sizeof(uint16_t); i++) { pclThread_->m_pwStack[i] = 0xFFFF; }
#endif // #if KERNEL_STACK_CHECK

    // 1st - push start address... (R0/PC)
    PUSH_TO_STACK(pu16Stack, u16Addr);

    //!! Note - R1 is the dedicted stack pointer (not in context)

    // 2nd - Interrupts enabled status register (R2/SR)
    PUSH_TO_STACK(pu16Stack, 0x08);

    //!! Note - R3 is a zero register (not in context)

    // Push other registers  (R4-R11)
    for (i = 4; i < 12; i++) { PUSH_TO_STACK(pu16Stack, i); }

    // Function parameter (R12)
    PUSH_TO_STACK(pu16Stack, (uint16_t)pclThread_->m_pvArg);

    // Push other registers (R13-R15)
    for (i = 13; i < 16; i++) { PUSH_TO_STACK(pu16Stack, i); }

    // Set the top o' the stack.
    pclThread_->m_pwStackTop = (uint16_t*)(pu16Stack + 1);
}

//---------------------------------------------------------------------------
static void Thread_Switch(void)
{
    KernelSWI::Clear();
    g_pclCurrent = (Thread*)g_pclNext;
}

//---------------------------------------------------------------------------
void ThreadPort::StartThreads()
{
    KernelSWI::Config();   // configure the task switch SWI
    KernelTimer::Config(); // configure the kernel timer

    // Tell the kernel that we're ready to start scheduling threads
    // for the first time.
    Kernel::CompleteStart();

    Scheduler::SetScheduler(1); // enable the scheduler
    Scheduler::Schedule();      // run the scheduler - determine the first thread to run

    Thread_Switch(); // Set the next scheduled thread to the current thread

#if KERNEL_ROUND_ROBIN
    Quantum::Update(g_pclCurrent);
#endif // #if KERNEL_ROUND_ROBIN

    KernelTimer::Start(); // enable the kernel timer
    KernelSWI::Start();   // enable the task switch SWI

    g_u8CSCount = 0; // Reset the critical section counter
    g_u16SR     = 0;
    // Restore the context...
    Thread_RestoreContext(); // restore the context of the first running thread
    ASM("reti");             // return from interrupt - will return to the first scheduled thread
}

} // namespace Mark3

//---------------------------------------------------------------------------
/**
 * Kernel Context-switch SWI
 */
//---------------------------------------------------------------------------
using namespace Mark3;
void __attribute__((interrupt(PORT1_VECTOR), naked)) isr_KernelSWI(void)
{
    Thread_SaveContext();    // Push the context (registers) of the current task
    Thread_Switch();         // Switch to the next task
    Thread_RestoreContext(); // Pop the context (registers) of the next task
    ASM("reti");             // Return to the next task
}
