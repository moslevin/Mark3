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
===========================================================================*/
/**

    @file   timerlist.cpp

    @brief  Implements timer list processing algorithms, responsible for all
            timer tick and expiry logic.

*/

#include "mark3.h"
namespace Mark3
{
//---------------------------------------------------------------------------
void TimerList::Init(void)
{
    m_bTimerActive  = false;
    m_u32NextWakeup = 0;
    m_clMutex.Init();
}

//---------------------------------------------------------------------------
void TimerList::Add(Timer* pclListNode_)
{
    KERNEL_ASSERT(nullptr != pclListNode_);
    auto lock = LockGuard { &m_clMutex };

    pclListNode_->ClearNode();
    TypedDoubleLinkList<Timer>::Add(pclListNode_);

    // Set the initial timer value
    pclListNode_->m_u32TimeLeft = pclListNode_->m_u32Interval;

    // Set the timer as active.
    pclListNode_->m_u8Flags |= uTimerFlagActive;
}

//---------------------------------------------------------------------------
void TimerList::Remove(Timer* pclLinkListNode_)
{
    KERNEL_ASSERT(nullptr != pclLinkListNode_);
    auto lock = LockGuard { &m_clMutex };

    TypedDoubleLinkList<Timer>::Remove(pclLinkListNode_);
    pclLinkListNode_->m_u8Flags &= ~uTimerFlagActive;
}

//---------------------------------------------------------------------------
void TimerList::Process(void)
{
    auto lock = LockGuard { &m_clMutex };

    auto* pclCurr = GetHead();
    // Subtract the elapsed time interval from each active timer.
    while (nullptr != pclCurr) {
        auto* pclNext = pclCurr->GetNext();

        // Active timers only...
        if ((pclCurr->m_u8Flags & uTimerFlagActive) != 0) {
            pclCurr->m_u32TimeLeft--;
            if (0 == pclCurr->m_u32TimeLeft) {
                // Expired -- run the callback. these callbacks must be very fast...
                if (nullptr != pclCurr->m_pfCallback) {
                    pclCurr->m_pfCallback(pclCurr->m_pclOwner, pclCurr->m_pvData);
                }
                if ((pclCurr->m_u8Flags & uTimerFlagOneShot) != 0) {
                    // If this was a one-shot timer, deactivate the timer + remove
                    pclCurr->m_u8Flags |= uTimerFlagExpired;
                    pclCurr->m_u8Flags &= ~uTimerFlagActive;
                    Remove(pclCurr);
                } else {
                    // Reset the interval timer.
                    pclCurr->m_u32TimeLeft = pclCurr->m_u32Interval;
                }
            }
        }
        pclCurr = pclNext;
    }
}

} // namespace Mark3
