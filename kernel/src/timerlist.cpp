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
    KERNEL_ASSERT(pclListNode_ != nullptr);
    auto lock = LockGuard{ &m_clMutex };

    pclListNode_->ClearNode();
    DoubleLinkList::Add(pclListNode_);

    // Set the initial timer value
    pclListNode_->m_u32TimeLeft = pclListNode_->m_u32Interval;

    // Set the timer as active.
    pclListNode_->m_u8Flags |= TIMERLIST_FLAG_ACTIVE;
}

//---------------------------------------------------------------------------
void TimerList::Remove(Timer* pclLinkListNode_)
{
    KERNEL_ASSERT(pclLinkListNode_ != nullptr);
    auto lock = LockGuard{ &m_clMutex };

    DoubleLinkList::Remove(pclLinkListNode_);
    pclLinkListNode_->m_u8Flags &= ~TIMERLIST_FLAG_ACTIVE;
}

//---------------------------------------------------------------------------
void TimerList::Process(void)
{
    Timer* pclNode;
    Timer* pclPrev;

    auto lock = LockGuard{ &m_clMutex };

    pclNode = static_cast<Timer*>(GetHead());
    pclPrev = NULL;

    // Subtract the elapsed time interval from each active timer.
    while (pclNode != 0) {
        // Active timers only...
        if ((pclNode->m_u8Flags & TIMERLIST_FLAG_ACTIVE) != 0) {
        pclNode->m_u32TimeLeft--;
        if (0 == pclNode->m_u32TimeLeft)
            {
                // Yes - set the "callback" flag - we'll execute the callbacks later
                pclNode->m_u8Flags |= TIMERLIST_FLAG_CALLBACK;

                if ((pclNode->m_u8Flags & TIMERLIST_FLAG_ONE_SHOT) != 0) {
                    // If this was a one-shot timer, deactivate the timer.
                    pclNode->m_u8Flags |= TIMERLIST_FLAG_EXPIRED;
                    pclNode->m_u8Flags &= ~TIMERLIST_FLAG_ACTIVE;
                } else {
                    // Reset the interval timer.
                    pclNode->m_u32TimeLeft = pclNode->m_u32Interval;
                }
            }
        }
        pclNode = static_cast<Timer*>(pclNode->GetNext());
    }

    // Process the expired timers callbacks.
    pclNode = static_cast<Timer*>(GetHead());

    while (pclNode != 0) {
        pclPrev = pclNode;
        pclNode = static_cast<Timer*>(pclNode->GetNext());

        // If the timer expired, run the callbacks now.
        if ((pclPrev->m_u8Flags & TIMERLIST_FLAG_CALLBACK) != 0) {
            bool bRemove = false;
            // If this was a one-shot timer, tag it for removal
            if ((pclPrev->m_u8Flags & TIMERLIST_FLAG_ONE_SHOT) != 0) {
                bRemove = true;
            }

            // Run the callback. these callbacks must be very fast...
            if (pclPrev->m_pfCallback != nullptr) {
                pclPrev->m_pfCallback(pclPrev->m_pclOwner, pclPrev->m_pvData);
            }
            pclPrev->m_u8Flags &= ~TIMERLIST_FLAG_CALLBACK;

            // Remove one-shot-timers
            if (bRemove) {
                Remove(pclPrev);
            }
        }
    }
}
} // namespace Mark3
