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
#include "kerneldebug.h"
#include "quantum.h"

//---------------------------------------------------------------------------
#if defined __FILE_ID__
    #undef __FILE_ID__
#endif
#define __FILE_ID__     TIMER_CPP       //!< File ID used in kernel trace calls

#if KERNEL_USE_TIMERS

//---------------------------------------------------------------------------
void Timer::Start( bool bRepeat_, uint32_t u32IntervalMs_, TimerCallback_t pfCallback_, void *pvData_ )
{
    SetIntervalMSeconds(u32IntervalMs_);
    m_u32TimerTolerance = 0;
    m_pfCallback = pfCallback_;
    m_pvData = pvData_;
    if (!bRepeat_)
    {
        m_u8Flags = TIMERLIST_FLAG_ONE_SHOT;
    }
    else
    {
        m_u8Flags = 0;
    }
    m_pclOwner = Scheduler::GetCurrentThread();
    TimerScheduler::Add(this);
}

//---------------------------------------------------------------------------
void Timer::Start( bool bRepeat_, uint32_t u32IntervalMs_, uint32_t u32ToleranceMs_, TimerCallback_t pfCallback_, void *pvData_ )
{
    m_u32TimerTolerance = MSECONDS_TO_TICKS(u32ToleranceMs_);
    Start(bRepeat_, u32IntervalMs_, pfCallback_, pvData_);
}

//---------------------------------------------------------------------------
void Timer::Stop()
{
    TimerScheduler::Remove(this);
}

//---------------------------------------------------------------------------
void Timer::SetIntervalTicks( uint32_t u32Ticks_ )
{
    m_u32Interval = u32Ticks_;
}

//---------------------------------------------------------------------------
//!! The next three cost u16 330 bytes of flash on AVR...
//---------------------------------------------------------------------------
void Timer::SetIntervalSeconds( uint32_t u32Seconds_)
{
    m_u32Interval = SECONDS_TO_TICKS(u32Seconds_);
}

//---------------------------------------------------------------------------
void Timer::SetIntervalMSeconds( uint32_t u32MSeconds_)
{
    m_u32Interval = MSECONDS_TO_TICKS(u32MSeconds_);
}

//---------------------------------------------------------------------------
void Timer::SetIntervalUSeconds( uint32_t u32USeconds_)
{
    m_u32Interval = useCONDS_TO_TICKS(u32USeconds_);
}

//---------------------------------------------------------------------------
void Timer::SetTolerance(uint32_t u32Ticks_)
{
    m_u32TimerTolerance = u32Ticks_;
}

#endif

