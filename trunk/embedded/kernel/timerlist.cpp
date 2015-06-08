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

    \file   timerlist.cpp

    \brief  Implements timer list processing algorithms, responsible for all
            timer tick and expiry logic.

*/

#include "kerneltypes.h"
#include "mark3cfg.h"

#include "timerlist.h"
#include "kerneltimer.h"
#include "threadport.h"
#include "kerneldebug.h"
#include "quantum.h"

//---------------------------------------------------------------------------
#if defined __FILE_ID__
    #undef __FILE_ID__
#endif
#define __FILE_ID__     TIMERLIST_CPP       //!< File ID used in kernel trace calls

#if KERNEL_USE_TIMERS

//---------------------------------------------------------------------------
TimerList TimerScheduler::m_clTimerList;

//---------------------------------------------------------------------------
void TimerList::Init(void)
{
    m_bTimerActive = 0;    
    m_ulNextWakeup = 0;    
}

//---------------------------------------------------------------------------
void TimerList::Add(Timer *pclListNode_)
{
#if KERNEL_TIMERS_TICKLESS
    K_BOOL bStart = 0;
    K_LONG lDelta;
#endif

    CS_ENTER();

#if KERNEL_TIMERS_TICKLESS
    if (GetHead() == NULL)
    {
        bStart = 1;
    }
#endif

    pclListNode_->ClearNode();    
    DoubleLinkList::Add(pclListNode_);
    
    // Set the initial timer value
    pclListNode_->m_ulTimeLeft = pclListNode_->m_ulInterval;    

#if KERNEL_TIMERS_TICKLESS
    if (!bStart)
    {
        // If the new interval is less than the amount of time remaining...
        lDelta = KernelTimer::TimeToExpiry() - pclListNode_->m_ulInterval;
    
        if (lDelta > 0)
        {
            // Set the new expiry time on the timer.
            m_ulNextWakeup = KernelTimer::SubtractExpiry((K_ULONG)lDelta);
        }
    }
    else    
    {
        m_ulNextWakeup = pclListNode_->m_ulInterval;
        KernelTimer::SetExpiry(m_ulNextWakeup);
        KernelTimer::Start();        
    }
#endif

    // Set the timer as active.
    pclListNode_->m_ucFlags |= TIMERLIST_FLAG_ACTIVE;    
    CS_EXIT();
}

//---------------------------------------------------------------------------
void TimerList::Remove(Timer *pclLinkListNode_)
{
    CS_ENTER();
    
    DoubleLinkList::Remove(pclLinkListNode_);

#if KERNEL_TIMERS_TICKLESS
    if (this->GetHead() == NULL)
    {
        KernelTimer::Stop();
    }
#endif
    
    CS_EXIT();
}

//---------------------------------------------------------------------------
void TimerList::Process(void)
{
#if KERNEL_TIMERS_TICKLESS
    K_ULONG ulNewExpiry;
    K_ULONG ulOvertime;
    K_BOOL bContinue;
#endif
    
    Timer *pclNode;
    Timer *pclPrev;

#if KERNEL_USE_QUANTUM
    Quantum::SetInTimer();
#endif
#if KERNEL_TIMERS_TICKLESS
    // Clear the timer and its expiry time - keep it running though
    KernelTimer::ClearExpiry();  
    do 
    {        
#endif
        pclNode = static_cast<Timer*>(GetHead());
        pclPrev = NULL;

#if KERNEL_TIMERS_TICKLESS
        bContinue = 0;
        ulNewExpiry = MAX_TIMER_TICKS;
#endif

        // Subtract the elapsed time interval from each active timer.
        while (pclNode)
        {        
            // Active timers only...
            if (pclNode->m_ucFlags & TIMERLIST_FLAG_ACTIVE)
            {
                // Did the timer expire?
#if KERNEL_TIMERS_TICKLESS
                if (pclNode->m_ulTimeLeft <= m_ulNextWakeup)
#else
                pclNode->m_ulTimeLeft--;
                if (0 == pclNode->m_ulTimeLeft)
#endif
                {
                    // Yes - set the "callback" flag - we'll execute the callbacks later
                    pclNode->m_ucFlags |= TIMERLIST_FLAG_CALLBACK;
                                
                    if (pclNode->m_ucFlags & TIMERLIST_FLAG_ONE_SHOT)
                    {
                        // If this was a one-shot timer, deactivate the timer.
                        pclNode->m_ucFlags |= TIMERLIST_FLAG_EXPIRED;
                        pclNode->m_ucFlags &= ~TIMERLIST_FLAG_ACTIVE;                    
                    }
                    else
                    {
                        // Reset the interval timer.
                        //!ToDo - figure out if we need to deal with any overtime here.
                        // I think we're good though...                        
                        pclNode->m_ulTimeLeft = pclNode->m_ulInterval;
                        
#if KERNEL_TIMERS_TICKLESS
                        // If the time remaining (plus the length of the tolerance interval)
                        // is less than the next expiry interval, set the next expiry interval.
                        K_ULONG ulTmp = pclNode->m_ulTimeLeft + pclNode->m_ulTimerTolerance;

                        if (ulTmp < ulNewExpiry)
                        {
                            ulNewExpiry = ulTmp;
                        }
#endif
                    }
                }
#if KERNEL_TIMERS_TICKLESS
                else
                {
                    // Not expiring, but determine how K_LONG to run the next timer interval for.
                    pclNode->m_ulTimeLeft -= m_ulNextWakeup;
                    if (pclNode->m_ulTimeLeft < ulNewExpiry)
                    {
                        ulNewExpiry = pclNode->m_ulTimeLeft;
                    }
                }
#endif
            }
            pclNode = static_cast<Timer*>(pclNode->GetNext());        
        }
    
        // Process the expired timers callbacks.
        pclNode = static_cast<Timer*>(GetHead());
        while (pclNode)
        {
            pclPrev = NULL;
        
            // If the timer expired, run the callbacks now.
            if (pclNode->m_ucFlags & TIMERLIST_FLAG_CALLBACK)
            {
                // Run the callback. these callbacks must be very fast...
                pclNode->m_pfCallback( pclNode->m_pclOwner, pclNode->m_pvData );
                pclNode->m_ucFlags &= ~TIMERLIST_FLAG_CALLBACK;
            
                // If this was a one-shot timer, let's remove it.
                if (pclNode->m_ucFlags & TIMERLIST_FLAG_ONE_SHOT)
                {
                    pclPrev = pclNode;
                }
            }
            pclNode = static_cast<Timer*>(pclNode->GetNext());
        
            // Remove one-shot-timers
            if (pclPrev)
            {
                Remove(pclPrev);
            }        
        }    

#if KERNEL_TIMERS_TICKLESS
        // Check to see how much time has elapsed since the time we 
        // acknowledged the interrupt... 
        ulOvertime = KernelTimer::GetOvertime();
        
        if( ulOvertime >= ulNewExpiry ) {
            m_ulNextWakeup = ulOvertime;
            bContinue = 1;
        }
        
    // If it's taken longer to go through this loop than would take us to
    // the next expiry, re-run the timing loop

    } while (bContinue);

    // This timer elapsed, but there's nothing more to do...
    // Turn the timer off.
    if (ulNewExpiry >= MAX_TIMER_TICKS)
    {
        KernelTimer::Stop();
    }
    else 
    {
        // Update the timer with the new "Next Wakeup" value, plus whatever
        // overtime has accumulated since the last time we called this handler

        m_ulNextWakeup = KernelTimer::SetExpiry(ulNewExpiry + ulOvertime);        
    }
#endif
#if KERNEL_USE_QUANTUM
    Quantum::ClearInTimer();
#endif
}


#endif //KERNEL_USE_TIMERS
