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

    \brief ATMega328p Profiling timer implementation
*/

#include "kerneltypes.h"
#include "mark3cfg.h"
#include "profile.h"
#include "kernelprofile.h"
#include "threadport.h"

#if KERNEL_USE_PROFILER
namespace Mark3 {
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
    return 0;
}

//---------------------------------------------------------------------------
void Profiler::Process()
{
    CS_ENTER();
    m_u32Epoch++;
    CS_EXIT();
}
} // namespace Mark3

using namespace Mark3;
#if 0
//---------------------------------------------------------------------------
ISR(TIMER0_OVF_vect)
{
    Profiler::Process();
}
#endif

#endif
