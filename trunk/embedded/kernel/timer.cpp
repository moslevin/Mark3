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
void Timer::Start( K_BOOL bRepeat_, K_ULONG ulIntervalMs_, TimerCallback_t pfCallback_, void *pvData_ )
{
    SetIntervalMSeconds(ulIntervalMs_);
    m_ulTimerTolerance = 0;
    m_pfCallback = pfCallback_;
    m_pvData = pvData_;
    if (!bRepeat_)
    {
        m_ucFlags = TIMERLIST_FLAG_ONE_SHOT;
    }
    else
    {
        m_ucFlags = 0;
    }
    m_pclOwner = Scheduler::GetCurrentThread();
    TimerScheduler::Add(this);
}

//---------------------------------------------------------------------------
void Timer::Start( K_BOOL bRepeat_, K_ULONG ulIntervalMs_, K_ULONG ulToleranceMs_, TimerCallback_t pfCallback_, void *pvData_ )
{
    m_ulTimerTolerance = MSECONDS_TO_TICKS(ulToleranceMs_);
    Start(bRepeat_, ulIntervalMs_, pfCallback_, pvData_);
}

//---------------------------------------------------------------------------
void Timer::Stop()
{
    TimerScheduler::Remove(this);
}

//---------------------------------------------------------------------------
void Timer::SetIntervalTicks( K_ULONG ulTicks_ )
{
    m_ulInterval = ulTicks_;
}

//---------------------------------------------------------------------------
//!! The next three cost us 330 bytes of flash on AVR...
//---------------------------------------------------------------------------
void Timer::SetIntervalSeconds( K_ULONG ulSeconds_)
{
    m_ulInterval = SECONDS_TO_TICKS(ulSeconds_);
}

//---------------------------------------------------------------------------
void Timer::SetIntervalMSeconds( K_ULONG ulMSeconds_)
{
    m_ulInterval = MSECONDS_TO_TICKS(ulMSeconds_);
}

//---------------------------------------------------------------------------
void Timer::SetIntervalUSeconds( K_ULONG ulUSeconds_)
{
    m_ulInterval = USECONDS_TO_TICKS(ulUSeconds_);
}

//---------------------------------------------------------------------------
void Timer::SetTolerance(K_ULONG ulTicks_)
{
    m_ulTimerTolerance = ulTicks_;
}

#endif

