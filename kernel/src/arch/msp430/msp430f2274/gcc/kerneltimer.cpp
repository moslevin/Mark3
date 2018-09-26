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

#include "kerneltypes.h"
#include "kerneltimer.h"
#include "mark3cfg.h"

#include <msp430.h>

#include "mark3.h"

using namespace Mark3;
namespace
{
// Static objects implementing the timer thread and its synchronization objects
Thread    s_clTimerThread;
K_WORD    s_clTimerThreadStack[PORT_KERNEL_TIMERS_THREAD_STACK];
Semaphore s_clTimerSemaphore;
} // anonymous namespace

//---------------------------------------------------------------------------
/**
 * Kernel Timer Interrupt
 */
//---------------------------------------------------------------------------
void __attribute__((interrupt(TIMERA0_VECTOR))) isr_KernelTIMER(void)
{
    if (!Kernel::IsStarted()) {
        return;
    }
    s_clTimerSemaphore.Post();
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
    TACTL = 0;         // Reset the register
    TAR   = 0;         // Clear Timer A
    TACTL |= TACLR;    // Reset the clock divider, etc.
    TACTL |= TASSEL_2; // Set the timer to use SMCLK
    TACTL &= ~TAIFG;   // Clear any pending interrupts
    TACCTL0 &= ~CCIFG; // Clear pending interrupts
    TACCR0 = (uint16_t)PORT_TIMER_FREQ; // Set interrupts to occur at tick freq.

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
    TAR = 0;

    TACTL &= ~TAIFG;   // Clear any pending interrupt on timer A
    TACCTL0 &= ~CCIFG; // Clear pending interrupts on the

    TACCTL0 |= CCIE; // Enable interrupts on TimerA0 CCR

    TACTL |= MC_1; // Set timer mode to count to TACCR0
}

//---------------------------------------------------------------------------
void KernelTimer::Stop(void)
{
    TACTL &= ~MC_1;

    TACCTL0 &= ~CCIE; // Disable TA0 CCR

    TAR    = 0;
    TACCR0 = 0;
}

//---------------------------------------------------------------------------
uint16_t KernelTimer::Read(void)
{
    uint16_t u16Val;
    TACCTL0 &= ~MC_1;
    u16Val = TAR;
    TACCTL0 |= MC_1;
    return u16Val;
}

//---------------------------------------------------------------------------
uint8_t KernelTimer::DI(void)
{
    uint8_t u8Ret = ((TACCTL0 & CCIE) != 0);
    TACCTL0 &= ~CCIE;
    TACCTL0 &= ~CCIFG;
    return u8Ret;
}

//---------------------------------------------------------------------------
void KernelTimer::EI(void)
{
    KernelTimer::RI(1);
}

//---------------------------------------------------------------------------
void KernelTimer::RI(bool bEnable_)
{
    if (bEnable_) {
        TACCTL0 |= CCIE;
    } else {
        TACCTL0 &= ~CCIE;
    }
}
} // namespace Mark3
