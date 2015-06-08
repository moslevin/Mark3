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
    TACCR0 = (K_USHORT)TIMER_FREQ; // Set interrupts to occur at tick freq.
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
K_USHORT KernelTimer::Read(void)
{
#if KERNEL_TIMERS_TICKLESS
	K_USHORT usVal;
	TACCTL0 &= ~MC_1;
	usVal = TAR;
	TACCTL0 |= MC_1;
	return usVal;
#else
	return 0;
#endif
}

//---------------------------------------------------------------------------
K_ULONG KernelTimer::SubtractExpiry(K_ULONG ulInterval_)
{
#if KERNEL_TIMERS_TICKLESS
	TACCR0 -= ulInterval_;
	return (K_ULONG)TACCR0;
#else
    return 0;
#endif
}

//---------------------------------------------------------------------------
K_ULONG KernelTimer::TimeToExpiry(void)
{
#if KERNEL_TIMERS_TICKLESS
	K_USHORT usCurrent = KernelTimer::Read();
	K_USHORT usMax = TACCR0;
	if (usMax >= usCurrent)
	{
		return 0;
	}
	return (usMax - usCurrent);
#else
    return 0;
#endif
}

//---------------------------------------------------------------------------
K_ULONG KernelTimer::GetOvertime(void)
{
#if KERNEL_TIMERS_TICKLESS
    (K_ULONG)KernelTimer::Read();
#else
    return 0;
#endif
}

//---------------------------------------------------------------------------
K_ULONG KernelTimer::SetExpiry(K_ULONG ulInterval_)
{
#if KERNEL_TIMERS_TICKLESS
	K_ULONG ulRet;
	if (ulInterval_ >= 65535)
	{
		ulRet = 65535;
	}
	else
	{
		ulRet = ulInterval_;
	}

	TACCR0 = (K_USHORT)ulRet;
	TACCTL0 |= CCIE;                // Enable interrupts on TimerA0 CCR
	TACTL |= MC_1;

	return ulRet;
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
K_UCHAR KernelTimer::DI(void)
{
#if KERNEL_TIMERS_TICKLESS
	K_UCHAR ucRet = ((TACCTL0 & CCIE) != 0);
	TACCTL0 &= ~CCIE;
	TACCTL0 &= ~CCIFG;
	return ucRet;
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
void KernelTimer::RI(K_BOOL bEnable_)
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
