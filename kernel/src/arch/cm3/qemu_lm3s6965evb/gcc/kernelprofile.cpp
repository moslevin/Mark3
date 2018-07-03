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
/*!
    \file kernelprofile.cpp

    \brief Profiling timer implementation
*/

#include "kerneltypes.h"
#include "mark3cfg.h"
#include "profile.h"
#include "kernelprofile.h"
#include "kerneltimer.h"
#include "threadport.h"

#include "m3_core_cm3.h"

#if KERNEL_USE_PROFILER
namespace Mark3 {
uint32_t Profiler::m_u32Epoch;
bool Profiler::m_bActive;

//---------------------------------------------------------------------------
void Profiler::Init()
{
    m_u32Epoch = 0;
    m_bActive = false;
}

//---------------------------------------------------------------------------
void Profiler::Start()
{
    m_bActive = true;
}

//---------------------------------------------------------------------------
void Profiler::Stop()
{
    m_bActive = false;
}
//---------------------------------------------------------------------------
uint16_t Profiler::Read()
{
    uint16_t rc = 0;
    CS_ENTER();
    rc = KernelTimer::Read();
    CS_EXIT();
    return rc;
}

//---------------------------------------------------------------------------
void Profiler::Process()
{
    CS_ENTER();
    m_u32Epoch++;
    CS_EXIT();
}

} // namespace Mark3
#endif
