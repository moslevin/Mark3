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

    \file   timerlist.cpp

    \brief  Implements timer list processing algorithms, responsible for all
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

#if KERNEL_TIMERS_TICKLESS
    bool    bStart = false;
    int32_t lDelta;
#endif
    auto lock = LockGuard{&m_clMutex};

#if KERNEL_TIMERS_TICKLESS
    if (GetHead() == NULL) {
        bStart = true;
    }
    if (pclListNode_->m_u32Interval < PORT_MIN_TIMER_TICKS) {
        pclListNode_->m_u32Interval = PORT_MIN_TIMER_TICKS;
    }
#endif

    pclListNode_->ClearNode();
    DoubleLinkList::Add(pclListNode_);

    // Set the initial timer value
    pclListNode_->m_u32TimeLeft = pclListNode_->m_u32Interval;

#if KERNEL_TIMERS_TICKLESS
    if (!bStart) {
        // If the new interval is less than the amount of time remaining...
        lDelta = (int32_t)((uint32_t)KernelTimer::TimeToExpiry() - pclListNode_->m_u32Interval);

        if (lDelta > 0) {
            // Set the new expiry time on the timer.
            m_u32NextWakeup = (uint32_t)KernelTimer::SubtractExpiry((uint32_t)lDelta);
        }
    } else {
        m_u32NextWakeup = pclListNode_->m_u32Interval;
        KernelTimer::SetExpiry(m_u32NextWakeup);
        KernelTimer::Start();
    }
#endif

    // Set the timer as active.
    pclListNode_->m_u8Flags |= TIMERLIST_FLAG_ACTIVE;
}

//---------------------------------------------------------------------------
void TimerList::Remove(Timer* pclLinkListNode_)
{
    KERNEL_ASSERT(pclLinkListNode_ != nullptr);
    auto lock = LockGuard{&m_clMutex};

    DoubleLinkList::Remove(pclLinkListNode_);
    pclLinkListNode_->m_u8Flags &= ~TIMERLIST_FLAG_ACTIVE;

#if KERNEL_TIMERS_TICKLESS
    if (this->GetHead() == NULL) {
        KernelTimer::Stop();
    }
#endif
}

//---------------------------------------------------------------------------
void TimerList::Process(void)
{
#if KERNEL_TIMERS_TICKLESS
    uint32_t u32NewExpiry;
    uint32_t u32Overtime;
    bool     bContinue;
#endif

    Timer* pclNode;
    Timer* pclPrev;

    auto lock = LockGuard{&m_clMutex};

    Quantum::SetInTimer();

#if KERNEL_TIMERS_TICKLESS
    do {
        bContinue    = false;
        u32NewExpiry = MAX_TIMER_TICKS; // Used to indicate that no timers are pending
#endif
        pclNode = static_cast<Timer*>(GetHead());
        pclPrev = NULL;

        // Subtract the elapsed time interval from each active timer.
        while (pclNode != 0) {
            // Active timers only...
            if ((pclNode->m_u8Flags & TIMERLIST_FLAG_ACTIVE) != 0) {
// Did the timer expire?
#if KERNEL_TIMERS_TICKLESS
                if (pclNode->m_u32TimeLeft <= m_u32NextWakeup)
#else
                pclNode->m_u32TimeLeft--;
                if (0 == pclNode->m_u32TimeLeft)
#endif
                {
                    // Yes - set the "callback" flag - we'll execute the callbacks later
                    pclNode->m_u8Flags |= TIMERLIST_FLAG_CALLBACK;

                    if ((pclNode->m_u8Flags & TIMERLIST_FLAG_ONE_SHOT) != 0) {
                        // If this was a one-shot timer, deactivate the timer.
                        pclNode->m_u8Flags |= TIMERLIST_FLAG_EXPIRED;
                        pclNode->m_u8Flags &= ~TIMERLIST_FLAG_ACTIVE;
                    } else {
                        // Reset the interval timer.
                        //! ToDo - figure out if we need to deal with any overtime here.
                        // I think we're good though...
                        pclNode->m_u32TimeLeft = pclNode->m_u32Interval;

#if KERNEL_TIMERS_TICKLESS
                        // If the time remaining (plus the length of the tolerance interval)
                        // is less than the next expiry interval, set the next expiry interval.
                        uint32_t u32Tmp = pclNode->m_u32TimeLeft + pclNode->m_u32TimerTolerance;

                        if (u32Tmp < u32NewExpiry) {
                            u32NewExpiry = u32Tmp;
                        }
#endif
                    }
                }
#if KERNEL_TIMERS_TICKLESS
                else {
                    // Not expiring, but determine how long to run the next timer interval for.
                    pclNode->m_u32TimeLeft -= m_u32NextWakeup;
                    if (pclNode->m_u32TimeLeft < u32NewExpiry) {
                        u32NewExpiry = pclNode->m_u32TimeLeft;
                    }
                }
#endif
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

#if KERNEL_TIMERS_TICKLESS
        // Check to see how much time has elapsed since the time we
        // acknowledged the interrupt...
        u32Overtime = (uint32_t)KernelTimer::GetOvertime();

        if (u32Overtime >= u32NewExpiry) {
            m_u32NextWakeup = u32Overtime;
            bContinue       = true;
        }

        // If it's taken longer to go through this loop than would take us to
        // the next expiry, re-run the timing loop

    } while (bContinue);

    // This timer elapsed, but there's nothing more to do...
    // Turn the timer off.
    if (u32NewExpiry >= MAX_TIMER_TICKS) {
        KernelTimer::Stop();
    } else {
        // Update the timer with the new "Next Wakeup" value, plus whatever
        // overtime has accumulated since the last time we called this handler

        m_u32NextWakeup = (uint32_t)KernelTimer::SetExpiry(u32NewExpiry + u32Overtime);
    }
#endif
    Quantum::ClearInTimer();
}
} //namespace Mark3
