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
=========================================================================== */
/*!

    \file   timerlist.h

    \brief  Timer list declarations

    These classes implements a linked list of timer objects attached to the
    global kernel timer scheduler.
 */

#ifndef __TIMERLIST_H__
#define __TIMERLIST_H__

#include "kerneltypes.h"
#include "mark3cfg.h"

#include "mutex.h"
#if KERNEL_USE_TIMERS

class Timer;

//---------------------------------------------------------------------------
/*!
 *   TimerList class - a doubly-linked-list of timer objects.
 */
class TimerList : public DoubleLinkList
{
public:
    /*!
     *  \brief Init
     *
     *  Initialize the TimerList object.  Must be called before
     *  using the object.
     */
    void Init();

    /*!
     *  \brief Add
     *
     *  Add a timer to the TimerList.
     *
     *  \param pclListNode_ Pointer to the Timer to Add
     */
    void Add(Timer* pclListNode_);

    /*!
     *  \brief Remove
     *
     *  Remove a timer from the TimerList, cancelling its expiry.
     *
     *  \param pclListNode_ Pointer to the Timer to remove
     */
    void Remove(Timer* pclLinkListNode_);

    /*!
     *  \brief Process
     *
     *  Process all timers in the timerlist as a result of the timer expiring.
     *  This will select a new timer epoch based on the next timer to expire.
     */
    void Process();

private:
    //! The time (in system clock ticks) of the next wakeup event
    uint32_t m_u32NextWakeup;

    //! Whether or not the timer is active
    bool m_bTimerActive;

#if KERNEL_TIMERS_THREADED
    //! Guards against concurrent access to the timer list - Only needed when running threaded.
    Mutex m_clMutex;
#endif

};

#endif // KERNEL_USE_TIMERS

#endif
