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

    \file   timerlist.h    

    \brief  Timer list declarations
    
    These classes implements a linked list of timer objects attached to the 
    global kernel timer scheduler.
*/

#ifndef __TIMERLIST_H__
#define __TIMERLIST_H__

#include "kerneltypes.h"
#include "mark3cfg.h"

#include "timer.h"
#if KERNEL_USE_TIMERS

//---------------------------------------------------------------------------
/*!
    TimerList class - a doubly-linked-list of timer objects.
*/
class TimerList : public DoubleLinkList
{
public:
    /*!
        \fn void Init()

        Initialize the TimerList object.  Must be called before
        using the object.
    */
    void Init();

    /*!
        \fn void Add(Timer *pclListNode_)

        Add a timer to the TimerList.

        \param pclListNode_ Pointer to the Timer to Add
    */
    void Add(Timer *pclListNode_);

    /*!
        \fn void Remove(Timer *pclListNode_)

        Remove a timer from the TimerList, cancelling its expiry.

        \param pclListNode_ Pointer to the Timer to remove
    */
    void Remove(Timer *pclListNode_);

    /*!
        \fn void Process()

        Process all timers in the timerlist as a result of the timer expiring.
        This will select a new timer epoch based on the next timer to expire.
    */
    void Process();

private:
    //! The time (in system clock ticks) of the next wakeup event
    uint32_t m_u32NextWakeup;

    //! Whether or not the timer is active
    bool m_bTimerActive;
};

#endif // KERNEL_USE_TIMERS

#endif
