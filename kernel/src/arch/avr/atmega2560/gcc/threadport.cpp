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

    @brief  ATMega328p Multithreading

*/

#include "kerneltypes.h"
#include "mark3cfg.h"
#include "thread.h"
#include "threadport.h"
#include "kernelprofile.h"
#include "kernelswi.h"
#include "kerneltimer.h"
#include "timerlist.h"
#include "quantum.h"
#include "kernel.h"

#include <avr/io.h>
#include <avr/interrupt.h>

namespace Mark3
{
//---------------------------------------------------------------------------
Thread* g_pclCurrentThread;

//---------------------------------------------------------------------------
void ThreadPort::InitStack(Thread* pclThread_)
{
    // Initialize the stack for a Thread
    uint16_t u16Addr;
    uint8_t* pu8Stack;
    uint16_t i;

    // Get the address of the thread's entry function
    u16Addr = (uint16_t)(pclThread_->m_pfEntryPoint);

    // Start by finding the bottom of the stack
    pu8Stack = (uint8_t*)pclThread_->m_pwStackTop;

#if KERNEL_STACK_CHECK
    // clear the stack, and initialize it to a known-default value (easier
    // to debug when things go sour with stack corruption or overflow)
    for (i = 0; i < pclThread_->m_u16StackSize; i++) { pclThread_->m_pwStack[i] = 0xFF; }
#endif // #if KERNEL_STACK_CHECK

    // Our context starts with the entry function
    PUSH_TO_STACK(pu8Stack, (uint8_t)(u16Addr & 0x00FF));
    PUSH_TO_STACK(pu8Stack, (uint8_t)((u16Addr >> 8) & 0x00FF));

    // 2560 Uses 3-byte addresses internally, but the tools only support
    // 16-bit pointers for addresses.  As a result, a limitation of this
    // port is that thread entry point addresses must be located in the
    // lower 64K of address space.
    PUSH_TO_STACK(pu8Stack, 0);

    // R0
    PUSH_TO_STACK(pu8Stack, 0x00); // R0

    // Push status register and R1 (which is used as a constant zero)
    PUSH_TO_STACK(pu8Stack, 0x80); // SR
    PUSH_TO_STACK(pu8Stack, 0x00); // R1

    // Push other registers
    for (i = 2; i <= 23; i++) // R2-R23
    {
        PUSH_TO_STACK(pu8Stack, i);
    }

    // Assume that the argument is the only stack variable
    PUSH_TO_STACK(pu8Stack, (uint8_t)(((uint16_t)(pclThread_->m_pvArg)) & 0x00FF));        // R24
    PUSH_TO_STACK(pu8Stack, (uint8_t)((((uint16_t)(pclThread_->m_pvArg)) >> 8) & 0x00FF)); // R25

    // Push the rest of the registers in the context
    for (i = 26; i <= 31; i++) { PUSH_TO_STACK(pu8Stack, i); }

    // Push the RAMPZ and EIND registers
    PUSH_TO_STACK(pu8Stack, 'z');
    PUSH_TO_STACK(pu8Stack, 'd');

    // Set the top o' the stack.
    pclThread_->m_pwStackTop = (uint8_t*)pu8Stack;

    // That's it!  the thread is ready to run now.
}

//---------------------------------------------------------------------------
static void Thread_Switch(void)
{
    g_pclCurrent = (Thread*)g_pclNext;
}

//---------------------------------------------------------------------------
void ThreadPort::StartThreads()
{
    KernelSWI::Config();   // configure the task switch SWI
    KernelTimer::Config(); // configure the kernel timer

    Profiler::Init();

    Scheduler::SetScheduler(1); // enable the scheduler
    Scheduler::Schedule();      // run the scheduler - determine the first thread to run

    Thread_Switch(); // Set the next scheduled thread to the current thread

    KernelTimer::Start(); // enable the kernel timer
    KernelSWI::Start();   // enable the task switch SWI

#if KERNEL_ROUND_ROBIN
    // Restart the thread quantum timer, as any value held prior to starting
    // the kernel will be invalid.  This fixes a bug where multiple threads
    // started with the highest priority before starting the kernel causes problems
    // until the running thread voluntarily blocks.
    Quantum::Update(g_pclCurrent);
#endif // #if KERNEL_ROUND_ROBIN

    // Restore the context...
    Thread_RestoreContext(); // restore the context of the first running thread

    ASM("reti"); // return from interrupt - will return to the first scheduled thread
}
} // namespace Mark3

using namespace Mark3;
//---------------------------------------------------------------------------
/**
 *  @brief ISR(INT0_vect)
 *   SWI using INT0 - used to trigger a context switch
 */
//---------------------------------------------------------------------------
ISR(INT0_vect) __attribute__((signal, naked));
ISR(INT0_vect)
{
    Thread_SaveContext();    // Push the context (registers) of the current task
    Thread_Switch();         // Switch to the next task
    Thread_RestoreContext(); // Pop the context (registers) of the next task
    ASM("reti");             // Return to the next task
}
