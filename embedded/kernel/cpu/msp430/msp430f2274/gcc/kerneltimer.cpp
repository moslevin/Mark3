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

    \file   kerneltimer.cpp

    \brief  Kernel Timer Implementation for ATMega328p
*/

#include "kerneltypes.h"
#include "kerneltimer.h"
#include "mark3cfg.h"

#include <msp430.h>


//---------------------------------------------------------------------------
void KernelTimer::Config(void)
{
    TACTL = 0;                        // Reset the register
    TAR = 0;                          // Clear Timer A
    TACTL |= TACLR;                   // Reset the clock divider, etc.
    TACTL |= TASSEL_2;                // Set the timer to use SMCLK
    TACTL &= ~TAIFG;                  // Clear any pending interrupts
    TACCTL0 &= ~CCIFG;                // Clear pending interrupts
#if KERNEL_TIMERS_TICKLESS
    TACTL |= ID_3; // Divide-by-8
#else
    TACCR0 = (uint16_t)TIMER_FREQ; // Set interrupts to occur at tick freq.
#endif
}

//---------------------------------------------------------------------------
void KernelTimer::Start(void)
{
    TAR = 0;

    TACTL &= ~TAIFG;                // Clear any pending interrupt on timer A
    TACCTL0 &= ~CCIFG;              // Clear pending interrupts on the

    TACCTL0 |= CCIE;                // Enable interrupts on TimerA0 CCR

    TACTL |= MC_1;                  // Set timer mode to count to TACCR0

}

//---------------------------------------------------------------------------
void KernelTimer::Stop(void)
{
	TACTL &= ~MC_1;

    TACCTL0 &= ~CCIE;               // Disable TA0 CCR

    TAR=0;
    TACCR0=0;
}

//---------------------------------------------------------------------------
uint16_t KernelTimer::Read(void)
{
#if KERNEL_TIMERS_TICKLESS
	uint16_t u16Val;
	TACCTL0 &= ~MC_1;
	u16Val = TAR;
	TACCTL0 |= MC_1;
	return u16Val;
#else
	return 0;
#endif
}

//---------------------------------------------------------------------------
uint32_t KernelTimer::SubtractExpiry(uint32_t u32Interval_)
{
#if KERNEL_TIMERS_TICKLESS
	TACCR0 -= u32Interval_;
	return (uint32_t)TACCR0;
#else
    return 0;
#endif
}

//---------------------------------------------------------------------------
uint32_t KernelTimer::TimeToExpiry(void)
{
#if KERNEL_TIMERS_TICKLESS
	uint16_t u16Current = KernelTimer::Read();
	uint16_t u16Max = TACCR0;
	if (u16Max >= u16Current)
	{
		return 0;
	}
	return (u16Max - u16Current);
#else
    return 0;
#endif
}

//---------------------------------------------------------------------------
uint32_t KernelTimer::GetOvertime(void)
{
#if KERNEL_TIMERS_TICKLESS
    (uint32_t)KernelTimer::Read();
#else
    return 0;
#endif
}

//---------------------------------------------------------------------------
uint32_t KernelTimer::SetExpiry(uint32_t u32Interval_)
{
#if KERNEL_TIMERS_TICKLESS
	uint32_t u32Ret;
	if (u32Interval_ >= 65535)
	{
		u32Ret = 65535;
	}
	else
	{
		u32Ret = u32Interval_;
	}

	TACCR0 = (uint16_t)u32Ret;
	TACCTL0 |= CCIE;                // Enable interrupts on TimerA0 CCR
	TACTL |= MC_1;

	return u32Ret;
#else
	return 0;
#endif
}

//---------------------------------------------------------------------------
void KernelTimer::ClearExpiry(void)
{
#if KERNEL_TIMERS_TICKLESS
    TACCR0 = 65535;
#endif
}

//---------------------------------------------------------------------------
uint8_t KernelTimer::DI(void)
{
#if KERNEL_TIMERS_TICKLESS
	uint8_t u8Ret = ((TACCTL0 & CCIE) != 0);
	TACCTL0 &= ~CCIE;
	TACCTL0 &= ~CCIFG;
	return u8Ret;
#else
    return 0;
#endif
}

//---------------------------------------------------------------------------
void KernelTimer::EI(void)
{
    KernelTimer::RI(1);
}

//---------------------------------------------------------------------------
void KernelTimer::RI(bool bEnable_)
{
#if KERNEL_TIMERS_TICKLESS
	if (bEnable_)
	{
		TACCTL0 |= CCIE;
	}
	else
	{
		TACCTL0 &= ~CCIE;
	}
#endif
}
