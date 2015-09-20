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

    \file   profile.cpp

    \brief  Code profiling utilities
*/

#include "kerneltypes.h"
#include "mark3cfg.h"
#include "profile.h"
#include "kernelprofile.h"
#include "threadport.h"
#include "kerneldebug.h"
//---------------------------------------------------------------------------
#if defined __FILE_ID__
	#undef __FILE_ID__
#endif
#define __FILE_ID__ 	PROFILE_CPP       //!< File ID used in kernel trace calls


#if KERNEL_USE_PROFILER

//---------------------------------------------------------------------------
void ProfileTimer::Init()
{
    m_u32Cumulative = 0;
    m_u32CurrentIteration = 0;
    m_u16Iterations = 0;
    m_bActive = 0;
}

//---------------------------------------------------------------------------    
void ProfileTimer::Start()
{
    if (!m_bActive)
    {
        CS_ENTER();
        m_u32CurrentIteration = 0;
		m_u32InitialEpoch = Profiler::GetEpoch();		
        m_u16Initial = Profiler::Read();        
        CS_EXIT();
        m_bActive = 1;
    }
}

//---------------------------------------------------------------------------    
void ProfileTimer::Stop()
{
    if (m_bActive)
    {
        uint16_t u16Final;
        uint32_t u32Epoch;
        CS_ENTER();
        u16Final = Profiler::Read();
        u32Epoch = Profiler::GetEpoch();
        // Compute total for current iteration...
        m_u32CurrentIteration = ComputeCurrentTicks(u16Final, u32Epoch);
        m_u32Cumulative += m_u32CurrentIteration;
        m_u16Iterations++;        
        CS_EXIT();
        m_bActive = 0;
    }
}

//---------------------------------------------------------------------------    
uint32_t ProfileTimer::GetAverage()
{
    if (m_u16Iterations)
    {
        return m_u32Cumulative / (uint32_t)m_u16Iterations;
    }
    return 0;
}
 
//---------------------------------------------------------------------------     
uint32_t ProfileTimer::GetCurrent()
{
	
    if (m_bActive)
    {
		uint16_t u16Current;
		uint32_t u32Epoch;
		CS_ENTER();
		u16Current = Profiler::Read();
		u32Epoch = Profiler::GetEpoch();
		CS_EXIT();
        return ComputeCurrentTicks(u16Current, u32Epoch);
    }
	return m_u32CurrentIteration;
}

//---------------------------------------------------------------------------
uint32_t ProfileTimer::ComputeCurrentTicks(uint16_t u16Current_, uint32_t u32Epoch_)
{    
    uint32_t u32Total;
	uint32_t u32Overflows;
	
	u32Overflows = u32Epoch_ - m_u32InitialEpoch;
	
	// More than one overflow...
	if (u32Overflows > 1)
	{
        u32Total = ((uint32_t)(u32Overflows-1) * TICKS_PER_OVERFLOW)
				+ (uint32_t)(TICKS_PER_OVERFLOW - m_u16Initial) +
				(uint32_t)u16Current_;		
	}
	// Only one overflow, or one overflow that has yet to be processed
	else if (u32Overflows || (u16Current_ < m_u16Initial))
	{
		u32Total = (uint32_t)(TICKS_PER_OVERFLOW - m_u16Initial) +
				(uint32_t)u16Current_;		
	}
	// No overflows, none pending.
	else
	{
		u32Total = (uint32_t)(u16Current_ - m_u16Initial);					
	}

    return u32Total;
}

#endif
