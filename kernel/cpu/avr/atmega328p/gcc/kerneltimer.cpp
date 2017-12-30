/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012 - 2017 Funkenstein Software Consulting, all rights reserved.
See license.txt for more information
===========================================================================*/
/*!

    \file   kerneltimer.cpp

    \brief  Kernel Timer Implementation for ATMega328p
*/

#include "kerneltypes.h"
#include "kerneltimer.h"
#include "mark3cfg.h"

#include "ksemaphore.h"
#include "thread.h"

#include <avr/common.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define TCCR1B_INIT ((1 << WGM12) | (1 << CS12))
#define TIMER_IMSK (1 << OCIE1A)
#define TIMER_IFR (1 << OCF1A)

//---------------------------------------------------------------------------
// Static objects implementing the timer thread and its synchronization objects
#if KERNEL_TIMERS_THREADED
static Thread s_clTimerThread;
static K_WORD s_clTimerThreadStack[PORT_KERNEL_TIMERS_THREAD_STACK];
static Semaphore s_clTimerSemaphore;
#endif

//---------------------------------------------------------------------------
/*!
 *   \brief ISR(TIMER1_COMPA_vect)
 *   Timer interrupt ISR - service the timer thread
 */
//---------------------------------------------------------------------------
ISR(TIMER1_COMPA_vect)
{
#if KERNEL_TIMERS_THREADED
    KernelTimer::ClearExpiry();
    s_clTimerSemaphore.Post();
#else
    #if KERNEL_USE_TIMERS
        TimerScheduler::GetInstance()->Process();
    #endif
    #if KERNEL_USE_QUANTUM
        Quantum::GetInstance()->UpdateTimer();
    #endif
#endif
}

//---------------------------------------------------------------------------
#if KERNEL_TIMERS_THREADED
static void KernelTimer_Task(void* unused)
{
    (void)unused;
    while(1) {
        s_clTimerSemaphore.Pend();
#if KERNEL_USE_TIMERS
        TimerScheduler::GetInstance()->Process();
#endif
#if KERNEL_USE_QUANTUM
        Quantum::GetInstance()->UpdateTimer();
#endif
    }
}
#endif

//---------------------------------------------------------------------------
void KernelTimer::Config(void)
{
    TCCR1B = TCCR1B_INIT;
#if KERNEL_TIMERS_THREADED
    s_clTimerSemaphore.Init(0, 1);
    s_clTimerThread.Init(s_clTimerThreadStack,
                        sizeof(s_clTimerThreadStack) / sizeof(K_WORD),
                        KERNEL_TIMERS_THREAD_PRIORITY,
                        KernelTimer_Task,
                        0);
    Quantum::GetInstance()->SetTimerThread(&s_clTimerThread);
    s_clTimerThread.Start();
#endif
}

//---------------------------------------------------------------------------
void KernelTimer::Start(void)
{
#if !KERNEL_TIMERS_TICKLESS
    TCCR1B = ((1 << WGM12) | (1 << CS11) | (1 << CS10));
    OCR1A  = ((PORT_SYSTEM_FREQ / 1000) / 64);
#else
    TCCR1B |= (1 << CS12);
#endif

    TCNT1 = 0;
    TIFR1 &= ~TIMER_IFR;
    TIMSK1 |= TIMER_IMSK;
}

//---------------------------------------------------------------------------
void KernelTimer::Stop(void)
{
#if KERNEL_TIMERS_TICKLESS
    TIFR1 &= ~TIMER_IFR;
    TIMSK1 &= ~TIMER_IMSK;
    TCCR1B &= ~(1 << CS12); // Disable count...
    TCNT1 = 0;
    OCR1A = 0;
#endif
}

//---------------------------------------------------------------------------
PORT_TIMER_COUNT_TYPE KernelTimer::Read(void)
{
#if KERNEL_TIMERS_TICKLESS
    volatile uint16_t u16Read1;
    volatile uint16_t u16Read2;

    do {
        u16Read1 = TCNT1;
        u16Read2 = TCNT1;
    } while (u16Read1 != u16Read2);

    return u16Read1;
#else
    return 0;
#endif
}

//---------------------------------------------------------------------------
PORT_TIMER_COUNT_TYPE KernelTimer::SubtractExpiry(PORT_TIMER_COUNT_TYPE uInterval)
{
#if KERNEL_TIMERS_TICKLESS
    OCR1A -= uInterval;
    return OCR1A;
#else
    return 0;
#endif
}

//---------------------------------------------------------------------------
PORT_TIMER_COUNT_TYPE KernelTimer::TimeToExpiry(void)
{
#if KERNEL_TIMERS_TICKLESS
    uint16_t u16Read  = KernelTimer::Read();
    uint16_t u16OCR1A = OCR1A;

    if (u16Read >= u16OCR1A) {
        return 0;
    } else {
        return (u16OCR1A - u16Read);
    }
#else
    return 0;
#endif
}

//---------------------------------------------------------------------------
PORT_TIMER_COUNT_TYPE KernelTimer::GetOvertime(void)
{
    return KernelTimer::Read();
}

//---------------------------------------------------------------------------
PORT_TIMER_COUNT_TYPE KernelTimer::SetExpiry(uint32_t u32Interval_)
{
#if KERNEL_TIMERS_TICKLESS
    uint16_t u16SetInterval;
    if (u32Interval_ > 65535) {
        u16SetInterval = 65535;
    } else {
        u16SetInterval = (uint16_t)u32Interval_;
    }

    OCR1A = u16SetInterval;
    return u16SetInterval;
#else
    return 0;
#endif
}

//---------------------------------------------------------------------------
void KernelTimer::ClearExpiry(void)
{
#if KERNEL_TIMERS_TICKLESS
    OCR1A = 65535; // Clear the compare value
#endif
}

//---------------------------------------------------------------------------
uint8_t KernelTimer::DI(void)
{
#if KERNEL_TIMERS_TICKLESS
    bool bEnabled = ((TIMSK1 & (TIMER_IMSK)) != 0);
    TIFR1 &= ~TIMER_IFR;   // Clear interrupt flags
    TIMSK1 &= ~TIMER_IMSK; // Disable interrupt
    return bEnabled;
#else
    return 0;
#endif
}

//---------------------------------------------------------------------------
void KernelTimer::EI(void)
{
    KernelTimer::RI(0);
}

//---------------------------------------------------------------------------
void KernelTimer::RI(bool bEnable_)
{
#if KERNEL_TIMERS_TICKLESS
    if (bEnable_) {
        TIMSK1 |= (1 << OCIE1A); // Enable interrupt
    } else {
        TIMSK1 &= ~(1 << OCIE1A);
    }
#endif
}
