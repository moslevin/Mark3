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
=========================================================================== */
/**

    @file   timerscheduler.h

    @brief  Timer scheduler declarations
 */

#pragma once

#include "kerneltypes.h"
#include "mark3cfg.h"

#include "ll.h"
#include "timer.h"
#include "timerlist.h"

namespace Mark3
{
//---------------------------------------------------------------------------
/**
 * @brief The TimerScheduler Class.
 * This implements a "Static" class used to manage a global list of timers used
 * throughout the system.
 */
class TimerScheduler
{
public:
    /**
     *  @brief Init
     *  Initialize the timer scheduler.  Must be called before any timer, or
     *  timer-derived functions are used.
     */
    static void Init() { m_clTimerList.Init(); }
    /**
     *  @brief Add
     *  Add a timer to the timer scheduler.  Adding a timer implicitly starts
     *  the timer as well.
     *
     *  @param pclListNode_ Pointer to the timer list node to add
     */
    static void Add(Timer* pclListNode_) { m_clTimerList.Add(pclListNode_); }
    /**
     *  @brief Remove
     *  Remove a timer from the timer scheduler.  May implicitly stop the
     *  timer if this is the only active timer scheduled.
     *
     *  @param pclListNode_ Pointer to the timer list node to remove
     */
    static void Remove(Timer* pclListNode_) { m_clTimerList.Remove(pclListNode_); }
    /**
     *  @brief Process
     *  This function must be called on timer expiry (from the timer's ISR
     *  context).  This will result in all timers being updated based on
     *  the epoch that just elapsed.  The next timer epoch is set based on the
     *  next Timer object to expire.
     */
    static void Process() { m_clTimerList.Process(); }

private:
    //! TimerList object manipu32ated by the Timer Scheduler
    static TimerList m_clTimerList;
};
} // namespace Mark3
