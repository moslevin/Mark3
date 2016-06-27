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

    \file   kernelswi.cpp

    \brief  Kernel Software interrupt implementation for ATMega328p

*/

#include "kerneltypes.h"
#include "kernelswi.h"

#include <avr/io.h>
#include <avr/interrupt.h>

//---------------------------------------------------------------------------
void KernelSWI::Config(void)
{
    PORTB &= ~0x04; // Clear INT2
    DDRB |= 0x04;    // Set PortB, bit 2 (INT2) As Output
    EICRA |= (1 << ISC20) | (1 << ISC21);    // Rising edge on INT2
}

//---------------------------------------------------------------------------
void KernelSWI::Start(void)
{        
    EIFR &= ~(1 << INTF2);    // Clear any pending interrupts on INT2
    EIMSK |= (1 << INT2);    // Enable INT2 interrupt (as int32_t as I-bit is set)
}

//---------------------------------------------------------------------------
void KernelSWI::Stop(void)
{
    EIMSK &= ~(1 << INT2);    // Disable INT0 interrupts
}

//---------------------------------------------------------------------------
uint8_t KernelSWI::DI()
{
    bool bEnabled = ((EIMSK & (1 << INT2)) != 0);
    EIMSK &= ~(1 << INT2);
    return bEnabled;
}

//---------------------------------------------------------------------------
void KernelSWI::RI(bool bEnable_)
{
    if (bEnable_)
    {
        EIMSK |= (1 << INT2);    
    }
    else
    {
        EIMSK &= ~(1 << INT2);    
    }
}

//---------------------------------------------------------------------------
void KernelSWI::Clear(void)
{
    EIFR &= ~(1 << INTF2);     // Clear the interrupt flag for INT0
}

//---------------------------------------------------------------------------
void KernelSWI::Trigger(void)
{
    //if(Thread_IsSchedulerEnabled())
    {
        PORTB &= ~0x04;
        PORTB |= 0x04;    
    }    
}
