/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012 - 2019 m0slevin, all rights reserved.
See license.txt for more information
===========================================================================*/
/**

    @file   kernelswi.cpp

    @brief  Kernel Software interrupt implementation for ATMega328p

*/

#include "kerneltypes.h"
#include "kernelswi.h"

#include <avr/io.h>
#include <avr/interrupt.h>

namespace Mark3
{
//---------------------------------------------------------------------------
void KernelSWI::Config(void)
{
    PORTD &= ~0x01;                       // Clear INT0
    DDRD |= 0x01;                         // Set PortD, bit 0 (INT0) As Output
    EICRA |= (1 << ISC00) | (1 << ISC01); // Rising edge on INT0
}

//---------------------------------------------------------------------------
void KernelSWI::Start(void)
{
    EIFR &= ~(1 << INTF0); // Clear any pending interrupts on INT0
    EIMSK |= (1 << INT0);  // Enable INT0 interrupt (as int32_t as I-bit is set)
}

//---------------------------------------------------------------------------
void KernelSWI::Trigger(void)
{
    // if(Thread_IsSchedulerEnabled())
    {
        PORTD &= ~0x01;
        PORTD |= 0x01;
    }
}
} // namespace Mark3
