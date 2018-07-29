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

    \file   timer.cpp

    \brief  Timer implementations

*/

#include "mark3.h"

namespace Mark3
{
TimerList TimerScheduler::m_clTimerList;

//---------------------------------------------------------------------------
Timer::Timer()
{
    m_u8Initialized = TIMER_INVALID_COOKIE;
    m_u8Flags = 0;
}

//---------------------------------------------------------------------------
void Timer::Init()
{
    if (IsInitialized()) {
        KERNEL_ASSERT((m_u8Flags & TIMERLIST_FLAG_ACTIVE) == 0);
    }

    ClearNode();
    m_u32Interval       = 0;
    m_u32TimerTolerance = 0;
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

    SetIntervalMSeconds(u32IntervalMs_);
    m_u32TimerTolerance = 0;
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
void Timer::Start(
    bool bRepeat_, uint32_t u32IntervalMs_, uint32_t u32ToleranceMs_, TimerCallback pfCallback_, void* pvData_)
{
    KERNEL_ASSERT(IsInitialized());

    if ((m_u8Flags & TIMERLIST_FLAG_ACTIVE) != 0) {
        return;
    }

    m_u32TimerTolerance = MSECONDS_TO_TICKS(u32ToleranceMs_);
    Start(bRepeat_, u32IntervalMs_, pfCallback_, pvData_);
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

//---------------------------------------------------------------------------
void Timer::SetIntervalTicks(uint32_t u32Ticks_)
{
    KERNEL_ASSERT(IsInitialized());
    m_u32Interval = u32Ticks_;
}

//---------------------------------------------------------------------------
//!! The next three cost u16 330 bytes of flash on AVR...
//---------------------------------------------------------------------------
void Timer::SetIntervalSeconds(uint32_t u32Seconds_)
{
    KERNEL_ASSERT(IsInitialized());
    m_u32Interval = SECONDS_TO_TICKS(u32Seconds_);
}

//---------------------------------------------------------------------------
void Timer::SetIntervalMSeconds(uint32_t u32MSeconds_)
{
    KERNEL_ASSERT(IsInitialized());
    m_u32Interval = MSECONDS_TO_TICKS(u32MSeconds_);
}

//---------------------------------------------------------------------------
void Timer::SetIntervalUSeconds(uint32_t u32USeconds_)
{
    KERNEL_ASSERT(IsInitialized());

#if KERNEL_TIMERS_TICKLESS
    if (u32USeconds_ < KERNEL_TIMERS_MINIMUM_DELAY_US) {
        u32USeconds_ = KERNEL_TIMERS_MINIMUM_DELAY_US;
    }
#endif

    m_u32Interval = USECONDS_TO_TICKS(u32USeconds_);
}

//---------------------------------------------------------------------------
void Timer::SetTolerance(uint32_t u32Ticks_)
{
    KERNEL_ASSERT(IsInitialized());
    m_u32TimerTolerance = u32Ticks_;
}
} //namespace Mark3
