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
/**

    @file   timer.cpp

    @brief  Timer implementations

*/

#include "mark3.h"

namespace Mark3
{
TimerList TimerScheduler::m_clTimerList;

//---------------------------------------------------------------------------
Timer::Timer()
{
    m_u8Initialized = TIMER_INVALID_COOKIE;
    m_u8Flags       = 0;
}

//---------------------------------------------------------------------------
void Timer::Init()
{
    if (IsInitialized()) {
        KERNEL_ASSERT((m_u8Flags & TIMERLIST_FLAG_ACTIVE) == 0);
    }

    ClearNode();
    m_u32Interval       = 0;
    m_u32TimeLeft       = 0;
    m_u8Flags           = 0;

    SetInitialized();
}

//---------------------------------------------------------------------------
void Timer::Start(bool bRepeat_, uint32_t u32IntervalMs_, TimerCallback pfCallback_, void* pvData_)
{
    KERNEL_ASSERT(IsInitialized());

    if ((m_u8Flags & TIMERLIST_FLAG_ACTIVE) != 0) {
        return;
    }

    m_u32Interval       = u32IntervalMs_;
    m_pfCallback        = pfCallback_;
    m_pvData            = pvData_;

    if (!bRepeat_) {
        m_u8Flags = TIMERLIST_FLAG_ONE_SHOT;
    } else {
        m_u8Flags = 0;
    }

    Start();
}

//---------------------------------------------------------------------------
void Timer::Start()
{
    KERNEL_ASSERT(IsInitialized());

    if ((m_u8Flags & TIMERLIST_FLAG_ACTIVE) != 0) {
        return;
    }

    m_pclOwner = Scheduler::GetCurrentThread();
    TimerScheduler::Add(this);
}

//---------------------------------------------------------------------------
void Timer::Stop()
{
    KERNEL_ASSERT(IsInitialized());
    if ((m_u8Flags & TIMERLIST_FLAG_ACTIVE) == 0) {
        return;
    }
    TimerScheduler::Remove(this);
}
} // namespace Mark3
