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
    
}

//---------------------------------------------------------------------------
void Profiler::Start()
{
    
}    

//---------------------------------------------------------------------------
void Profiler::Stop()
{
    
}    
//---------------------------------------------------------------------------
uint16_t Profiler::Read()
{
    
}

//---------------------------------------------------------------------------
void Profiler::Process()
{
    
}

//---------------------------------------------------------------------------
//ISR(TIMER0_OVF_vect)
//{
//    Profiler::Process();
//}

#endif
