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

    @file   kerneltimer.cpp

    @brief  Kernel Timer Implementation for ATMega328p
*/

#include "mark3.h"

#include <avr/io.h>
#include <avr/interrupt.h>

namespace
{
using namespace Mark3;
//---------------------------------------------------------------------------
// Static objects implementing the timer thread and its synchronization objects
Thread    s_clTimerThread;
K_WORD    s_clTimerThreadStack[PORT_KERNEL_TIMERS_THREAD_STACK];
Semaphore s_clTimerSemaphore;
} // anonymous namespace

namespace Mark3
{
//---------------------------------------------------------------------------
static void KernelTimer_Task(void* unused)
{
    (void)unused;
    while (1) {
        s_clTimerSemaphore.Pend();
#if KERNEL_ROUND_ROBIN
        Quantum::SetInTimer();
#endif // #if KERNEL_ROUND_ROBIN
        TimerScheduler::Process();
#if KERNEL_ROUND_ROBIN
        Quantum::ClearInTimer();
#endif // #if KERNEL_ROUND_ROBIN
    }
}
#endif

//---------------------------------------------------------------------------
void KernelTimer::Config(void)
{
    TCC1.CTRLA = 0;
    TCC1.CTRLB = 0; // Disable individual capture/compares on this timer since all we care about is overflow.
    TCC1.CTRLC = 0;
    TCC1.CTRLD = 0x20;

    TCC1.CTRLE = 0x00; // using word mode (not byte mode)

    TCC1.CTRLFCLR |= 0x0F; // Reset the controller for upcount, no controller update
    TCC1.INTFLAGS |= 0x01; // Clear interrupt on overflow

    TCC1.PER = 0;
    TCC1.CNT = 0;

    TCC1.INTCTRLB = 0; // Disable ABCD Capture compare interrupts.

    s_clTimerSemaphore.Init(0, 1);
    s_clTimerThread.Init(s_clTimerThreadStack,
                         sizeof(s_clTimerThreadStack) / sizeof(K_WORD),
                         KERNEL_TIMERS_THREAD_PRIORITY,
                         KernelTimer_Task,
                         0);
    Quantum::SetTimerThread(&s_clTimerThread);
    s_clTimerThread.Start();
}

//---------------------------------------------------------------------------
void KernelTimer::Start(void)
{
    TCC1.CTRLA = 0x05;     // Clock / 64
    TCC1.INTFLAGS |= 0x01; // Clear interrupt on overflow
    TCC1.INTCTRLA |= 0x01;
}

//---------------------------------------------------------------------------
void KernelTimer::Stop(void)
{
    TCC1.INTFLAGS |= 0x01;    // Clear interrupt on overflow
    TCC1.INTCTRLA &= ~(0x01); // Disable the interrupt
    TCC1.CTRLA = 0;           // Clock source disable.
}

//---------------------------------------------------------------------------
/**
 *   @brief ISR(TIMER1_COMPA_vect)
 *   Timer interrupt ISR - service the timer thread
 */
//---------------------------------------------------------------------------
} // namespace Mark3
using namespace Mark3;
ISR(TIMER1_COMPA_vect)
{
    Kernel::Tick();
    s_clTimerSemaphore.Post();
}
