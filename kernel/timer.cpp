/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012 - 2017 Funkenstein Software Consulting, all rights reserved.
See license.txt for more information
===========================================================================*/
/*!

    \file   timer.cpp

    \brief  Timer implementations

*/

#include "kerneltypes.h"
#include "mark3cfg.h"

#include "timer.h"
#include "timerlist.h"
#include "timerscheduler.h"
#include "kerneltimer.h"
#include "threadport.h"
#include "quantum.h"

#define _CAN_HAS_DEBUG
//--[Autogenerated - Do Not Modify]------------------------------------------
#include "dbg_file_list.h"
#include "buffalogger.h"
#if defined(DBG_FILE)
#error "Debug logging file token already defined!  Bailing."
#else
#define DBG_FILE _DBG___KERNEL_TIMER_CPP
#endif
//--[End Autogenerated content]----------------------------------------------

#include "kerneldebug.h"

#if KERNEL_USE_TIMERS

//---------------------------------------------------------------------------
Timer::Timer()
{
#if KERNEL_EXTRA_CHECKS
    m_u8Initialized = TIMER_INVALID_COOKIE;
#endif
    m_u8Flags = 0;
}

//---------------------------------------------------------------------------
void Timer::Init()
{
#if KERNEL_EXTRA_CHECKS
    if (IsInitialized()) {
        KERNEL_ASSERT((m_u8Flags & TIMERLIST_FLAG_ACTIVE) == 0);
    }
#endif

    ClearNode();
    m_u32Interval       = 0;
    m_u32TimerTolerance = 0;
    m_u32TimeLeft       = 0;
    m_u8Flags           = 0;

#if KERNEL_EXTRA_CHECKS
    SetInitialized();
#endif
}

//---------------------------------------------------------------------------
void Timer::Start(bool bRepeat_, uint32_t u32IntervalMs_, TimerCallback_t pfCallback_, void* pvData_)
{
#if KERNEL_EXTRA_CHECKS
    KERNEL_ASSERT(IsInitialized());
#endif

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
    bool bRepeat_, uint32_t u32IntervalMs_, uint32_t u32ToleranceMs_, TimerCallback_t pfCallback_, void* pvData_)
{
#if KERNEL_EXTRA_CHECKS
    KERNEL_ASSERT(IsInitialized());
#endif

    if ((m_u8Flags & TIMERLIST_FLAG_ACTIVE) != 0) {
        return;
    }

    m_u32TimerTolerance = MSECONDS_TO_TICKS(u32ToleranceMs_);
    Start(bRepeat_, u32IntervalMs_, pfCallback_, pvData_);
}

//---------------------------------------------------------------------------
void Timer::Start()
{
#if KERNEL_EXTRA_CHECKS
    KERNEL_ASSERT(IsInitialized());
#endif

    if ((m_u8Flags & TIMERLIST_FLAG_ACTIVE) != 0) {
        return;
    }

    m_pclOwner = Scheduler::GetCurrentThread();
    TimerScheduler::Add(this);
}

//---------------------------------------------------------------------------
void Timer::Stop()
{
#if KERNEL_EXTRA_CHECKS
    KERNEL_ASSERT(IsInitialized());
#endif

    if ((m_u8Flags & TIMERLIST_FLAG_ACTIVE) == 0) {
            return;
    }
    TimerScheduler::Remove(this);
}

//---------------------------------------------------------------------------
void Timer::SetIntervalTicks(uint32_t u32Ticks_)
{
#if KERNEL_EXTRA_CHECKS
    KERNEL_ASSERT(IsInitialized());
#endif

    m_u32Interval = u32Ticks_;
}

//---------------------------------------------------------------------------
//!! The next three cost u16 330 bytes of flash on AVR...
//---------------------------------------------------------------------------
void Timer::SetIntervalSeconds(uint32_t u32Seconds_)
{
#if KERNEL_EXTRA_CHECKS
    KERNEL_ASSERT(IsInitialized());
#endif

    m_u32Interval = SECONDS_TO_TICKS(u32Seconds_);
}

//---------------------------------------------------------------------------
void Timer::SetIntervalMSeconds(uint32_t u32MSeconds_)
{
#if KERNEL_EXTRA_CHECKS
    KERNEL_ASSERT(IsInitialized());
#endif

    m_u32Interval = MSECONDS_TO_TICKS(u32MSeconds_);
}

//---------------------------------------------------------------------------
void Timer::SetIntervalUSeconds(uint32_t u32USeconds_)
{
#if KERNEL_EXTRA_CHECKS
    KERNEL_ASSERT(IsInitialized());
#endif

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
#if KERNEL_EXTRA_CHECKS
    KERNEL_ASSERT(IsInitialized());
#endif
    m_u32TimerTolerance = u32Ticks_;
}

#endif
