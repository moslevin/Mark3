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
    \file kernelprofile.cpp
    
    \brief ATMega328p Profiling timer implementation
*/

#include "kerneltypes.h"
#include "mark3cfg.h"
#include "profile.h"
#include "kernelprofile.h"
#include "threadport.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#if KERNEL_USE_PROFILER
uint32_t Profiler::m_u32Epoch;

//---------------------------------------------------------------------------
void Profiler::Init()
{
    TCCR0A = 0;
    TCCR0B = 0;
    TIFR0 = 0;
    TIMSK0 = 0;
    m_u32Epoch = 0;
}

//---------------------------------------------------------------------------
void Profiler::Start()
{
    TIFR0 = 0;
    TCNT0 = 0;
    TCCR0B |= (1 << CS01);
    TIMSK0 |= (1 << TOIE0);
}    

//---------------------------------------------------------------------------
void Profiler::Stop()
{
    TIFR0 = 0;
    TCCR0B &= ~(1 << CS01);
    TIMSK0 &= ~(1 << TOIE0);
}    
//---------------------------------------------------------------------------
uint16_t Profiler::Read()
{
    uint16_t u16Ret;
    CS_ENTER();
    TCCR0B &= ~(1 << CS01);
    u16Ret = TCNT0;
    TCCR0B |= (1 << CS01);
    CS_EXIT();
    return u16Ret;
}

//---------------------------------------------------------------------------
void Profiler::Process()
{
    CS_ENTER();
    m_u32Epoch++;
    CS_EXIT();
}

//---------------------------------------------------------------------------
ISR(TIMER0_OVF_vect)
{
    Profiler::Process();
}

#endif
