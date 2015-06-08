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

    \file   timerscheduler.h

    \brief  Timer scheduler declarations
*/

#ifndef __TIMERSCHEDULER_H__
#define __TIMERSCHEDULER_H__

#include "kerneltypes.h"
#include "mark3cfg.h"

#include "ll.h"
#include "timer.h"
#include "timerlist.h"

#if KERNEL_USE_TIMERS

//---------------------------------------------------------------------------
/*!
    "Static" Class used to interface a global TimerList with the rest of the
    kernel.
*/
class TimerScheduler
{
public:
    /*!
        \fn void Init()

        Initialize the timer scheduler.  Must be called before any timer, or
        timer-derived functions are used.
    */
    static void Init() { m_clTimerList.Init(); }

    /*!
        \fn void Add(Timer *pclListNode_)

        Add a timer to the timer scheduler.  Adding a timer implicitly starts
        the timer as well.

        \param pclListNode_ Pointer to the timer list node to add
    */
    static void Add(Timer *pclListNode_)
        {m_clTimerList.Add(pclListNode_); }

    /*!
        \fn void Remove(Timer *pclListNode_)

        Remove a timer from the timer scheduler.  May implicitly stop the
        timer if this is the only active timer scheduled.

        \param pclListNode_ Pointer to the timer list node to remove
    */
    static void Remove(Timer *pclListNode_)
        {m_clTimerList.Remove(pclListNode_); }

    /*!
        \fn void Process()

        This function must be called on timer expiry (from the timer's ISR
        context).  This will result in all timers being updated based on
        the epoch that just elapsed.  The next timer epoch is set based on the
        next Timer object to expire.
    */
    static void Process() {m_clTimerList.Process();}
private:

    //! TimerList object manipulated by the Timer Scheduler
    static TimerList m_clTimerList;
};

#endif //KERNEL_USE_TIMERS

#endif //__TIMERSCHEDULER_H__

