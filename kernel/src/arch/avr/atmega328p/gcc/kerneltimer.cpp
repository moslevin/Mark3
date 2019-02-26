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

#include <avr/common.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define TCCR1B_INIT ((1 << WGM12) | (1 << CS12))
#define TIMER_IMSK (1 << OCIE1A)
#define TIMER_IFR (1 << OCF1A)

namespace
{
using namespace Mark3;
//---------------------------------------------------------------------------
// Static objects implementing the timer thread and its synchronization objects
Thread    s_clTimerThread;
K_WORD    s_clTimerThreadStack[PORT_KERNEL_TIMERS_THREAD_STACK];
Semaphore s_clTimerSemaphore;
}

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

//---------------------------------------------------------------------------
void KernelTimer::Config(void)
{
    TCCR1B = TCCR1B_INIT;
    s_clTimerSemaphore.Init(0, 1);
    s_clTimerThread.Init(s_clTimerThreadStack,
                         sizeof(s_clTimerThreadStack) / sizeof(K_WORD),
                         KERNEL_TIMERS_THREAD_PRIORITY,
                         KernelTimer_Task,
                         0);
#if KERNEL_ROUND_ROBIN
    Quantum::SetTimerThread(&s_clTimerThread);
#endif // #if KERNEL_ROUND_ROBIN
    s_clTimerThread.Start();
}

//---------------------------------------------------------------------------
void KernelTimer::Start(void)
{
    TCCR1B = ((1 << WGM12) | (1 << CS11) | (1 << CS10));
    OCR1A  = ((PORT_SYSTEM_FREQ / 1000) / 64);
    TCNT1  = 0;
    TIFR1 &= ~TIMER_IFR;
    TIMSK1 |= TIMER_IMSK;
}

//---------------------------------------------------------------------------
void KernelTimer::Stop(void)
{
    TIFR1 &= ~TIMER_IFR;
    TIMSK1 &= ~TIMER_IMSK;
    TCCR1B &= ~(1 << CS12); // Disable count...
    TCNT1 = 0;
    OCR1A = 0;
}
} // namespace Mark3

//---------------------------------------------------------------------------
/**
 *   @brief ISR(TIMER1_COMPA_vect)
 *   Timer interrupt ISR - service the timer thread
 */
//---------------------------------------------------------------------------
using namespace Mark3;
ISR(TIMER1_COMPA_vect)
{
    Kernel::Tick();
    s_clTimerSemaphore.Post();
}
