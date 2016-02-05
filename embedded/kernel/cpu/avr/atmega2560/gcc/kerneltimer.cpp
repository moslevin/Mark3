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
/*!

    \file   kerneltimer.cpp

    \brief  Kernel Timer Implementation for ATMega328p
*/

#include "kerneltypes.h"
#include "kerneltimer.h"
#include "mark3cfg.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#define TCCR1B_INIT        ((1 << WGM12) | (1 << CS12))
#define TIMER_IMSK        (1 << OCIE1A)
#define TIMER_IFR        (1 << OCF1A)

//---------------------------------------------------------------------------
void KernelTimer::Config(void)
{
    TCCR1B = TCCR1B_INIT;
}

//---------------------------------------------------------------------------
void KernelTimer::Start(void)
{
#if !KERNEL_TIMERS_TICKLESS
    TCCR1B = ((1 << WGM12) | (1 << CS11) | (1 << CS10));
    OCR1A = ((SYSTEM_FREQ / 1000) / 64);
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
    TCCR1B &= ~(1 << CS12);      // Disable count...
    TCNT1 = 0;
    OCR1A = 0;
#endif
}

//---------------------------------------------------------------------------
uint16_t KernelTimer::Read(void)
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
uint32_t KernelTimer::SubtractExpiry(uint32_t u32Interval_)
{
#if KERNEL_TIMERS_TICKLESS
    OCR1A -= (uint16_t)u32Interval_;
    return (uint32_t)OCR1A;
#else
    return 0;
#endif
}

//---------------------------------------------------------------------------
uint32_t KernelTimer::TimeToExpiry(void)
{
#if KERNEL_TIMERS_TICKLESS
    uint16_t u16Read = KernelTimer::Read();
    uint16_t u16OCR1A = OCR1A;

    if (u16Read >= u16OCR1A)
    {
        return 0;
    }
    else
    {
        return (uint32_t)(u16OCR1A - u16Read);
    }
#else
    return 0;
#endif
}

//---------------------------------------------------------------------------
uint32_t KernelTimer::GetOvertime(void)
{
    return KernelTimer::Read();
}

//---------------------------------------------------------------------------
uint32_t KernelTimer::SetExpiry(uint32_t u32Interval_)
{
#if KERNEL_TIMERS_TICKLESS
    uint16_t u16SetInterval;
    if (u32Interval_ > 65535)
    {
        u16SetInterval = 65535;
    }
    else
    {
        u16SetInterval = (uint16_t)u32Interval_ ;
    }

    OCR1A = u16SetInterval;
    return (uint32_t)u16SetInterval;
#else
    return 0;
#endif
}

//---------------------------------------------------------------------------
void KernelTimer::ClearExpiry(void)
{
#if KERNEL_TIMERS_TICKLESS
    OCR1A = 65535;                    // Clear the compare value
#endif
}

//---------------------------------------------------------------------------
uint8_t KernelTimer::DI(void)
{
#if KERNEL_TIMERS_TICKLESS
    bool bEnabled = ((TIMSK1 & (TIMER_IMSK)) != 0);
    TIFR1 &= ~TIMER_IFR;      // Clear interrupt flags
    TIMSK1 &= ~TIMER_IMSK;    // Disable interrupt
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
    if (bEnable_)
    {
        TIMSK1 |= (1 << OCIE1A);    // Enable interrupt
    }
    else
    {
        TIMSK1 &= ~(1 << OCIE1A);
    }
#endif
}
