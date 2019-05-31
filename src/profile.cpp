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

    @file   profile.cpp

    @brief  Code profiling utilities
*/

#include "mark3.h"
namespace Mark3
{
//---------------------------------------------------------------------------
void ProfileTimer::Init()
{
    m_u32StartTicks = 0;
    m_u32Cumulative = 0;
    m_u16Iterations = 0;
    m_bActive       = false;
}

//---------------------------------------------------------------------------
void ProfileTimer::Start()
{
    if (!m_bActive) {
        { // Begin critical section
            const auto cs = CriticalGuard{};
            m_u32StartTicks = Kernel::GetTicks();
        } // End Critical Section
        m_bActive = true;
    }
}

//---------------------------------------------------------------------------
void ProfileTimer::Stop()
{
    if (m_bActive) {
        uint32_t u32Final;
        { // Begin critical section
            const auto cs = CriticalGuard{};
            u32Final = Kernel::GetTicks();
            // Compute total for current iteration...
            m_u32CurrentIteration = u32Final - m_u32StartTicks;
            m_u32Cumulative += m_u32CurrentIteration;
            m_u16Iterations++;
        } // End critical section
        m_bActive = false;
    }
}

//---------------------------------------------------------------------------
uint32_t ProfileTimer::GetAverage()
{
    if (0u != m_u16Iterations) {
        return (m_u32Cumulative + static_cast<uint32_t>(m_u16Iterations / 2)) / static_cast<uint32_t>(m_u16Iterations);
    }
    return 0;
}

//---------------------------------------------------------------------------
uint32_t ProfileTimer::GetCurrent()
{
    if (m_bActive) {
        uint32_t u32Current;
        { // Begin critical section
            const auto cs = CriticalGuard{};
            u32Current = Kernel::GetTicks() - m_u32StartTicks;
        } // End critical section
        return u32Current;
    }
    return m_u32CurrentIteration;
}
} // namespace Mark3
