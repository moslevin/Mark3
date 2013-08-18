/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012 Funkenstein Software Consulting, all rights reserved.
See license.txt for more information
===========================================================================*/
/*!
    \file eventflag.cpp
    \brief Event Flag Blocking Object/IPC-Object implementation.
*/

#include "mark3cfg.h"
#include "blocking.h"
#include "kernel.h"
#include "thread.h"
#include "eventflag.h"

//---------------------------------------------------------------------------
void EventFlag::Wait(K_USHORT usMask_, EventFlagOperation_t eMode_)
{
    bool bThreadYield = false;
    bool bMatch = false;
    K_USHORT usThreadMask;
    Thread *pclThread = Scheduler::GetCurrentThread();

    // Ensure we're operating in a critical section while we determine
    // whether or not we need to block the current thread on this object.
    CS_ENTER();

    // Check to see whether or not the current mask matches any of the
    // desired bits.
    usThreadMask = pclThread->GetEventFlagMask();

    if ((eMode_ == EVENT_FLAG_ALL) || (eMode_ == EVENT_FLAG_ALL_CLEAR))
    {
        // Check to see if the flags in their current state match all of
        // the set flags in the event flag group, with this mask.
        if ((m_usSetMask & usMask_) == (usThreadMask & usMask_))
        {
            bMatch = true;
        }
    }
    else if ((eMode_ == EVENT_FLAG_ANY) || (eMode_ == EVENT_FLAG_ANY_CLEAR))
    {
        // Check to see if the existing flags match any of the set flags in
        // the event flag group  with this mask
        if (m_usSetMask & usMask_& usThreadMask)
        {
            bMatch = true;
        }
    }

    // We're unable to match this pattern as-is, so we must block.
    if (!bMatch)
    {
        // Reset the current thread's event flag mask & mode
        pclThread->SetEventFlagMask(usMask_);
        pclThread->SetEventFlagMode(eMode_);

        // Add the thread to the object's block-list.
        Block(pclThread);

        // Trigger that
        bThreadYield = true;
    }

    // If bThreadYield is set, it means that we've blocked the current thread,
    // and must therefore rerun the scheduler to determine what thread to
    // switch to.
    if (bThreadYield)
    {
        // Switch threads immediately
        Thread::Yield();
    }

    // Exit the critical section and return back to normal execution
    CS_EXIT();

    //!! If the Yield operation causes a new thread to be chosen, there will
    //!! Be a context switch at the above CS_EXIT().  The original calling
    //!! thread will not return back until a matching SetFlags call is made.
}

//---------------------------------------------------------------------------
void EventFlag::SetFlags(K_USHORT usMask_)
{
    Thread *pclPrev;
    Thread *pclCurrent;
    bool bReschedule = false;

    CS_ENTER();

    // Walk through the whole block list, checking to see whether or not
    // the current flag set now matches any/all of the masks and modes of
    // the threads involved.

    m_usSetMask |= usMask_;

    // Start at the head of the list, and iterate through until we hit the
    // "head" element in the list again.  Ensure that we handle the case where
    // we remove the first or last elements in the list, or if there's only
    // one element in the list.
    pclCurrent = static_cast<Thread*>(m_clBlockList.GetHead());

    if (pclCurrent)
    {
        do
        {
            pclPrev = pclCurrent;
            pclCurrent = static_cast<Thread*>(pclCurrent->GetNext());

            K_USHORT usThreadMask = pclPrev->GetEventFlagMask();
            EventFlagOperation_t eThreadMode = pclPrev->GetEventFlagMode();

            if (pclPrev->GetEventFlagMask() & usMask_)
            {
                pclPrev->SetEventFlagMode(EVENT_FLAG_PENDING_UNBLOCK);
                bReschedule = true;
            }
        }
        // To keep looping, ensure that there's something in the list, and
        // that the next item isn't the head of the list.
        while (pclPrev != m_clBlockList.GetTail());

        pclCurrent = static_cast<Thread*>(m_clBlockList.GetHead());
        bool bIsTail = false;
        do
        {

            pclPrev = pclCurrent;
            pclCurrent = static_cast<Thread*>(pclCurrent->GetNext());

            if (pclPrev == m_clBlockList.GetTail())
            {
                bIsTail = true;
            }

            if (pclPrev->GetEventFlagMode() == EVENT_FLAG_PENDING_UNBLOCK)
            {
                UnBlock(pclPrev);
            }
        }
        while ( !bIsTail );
    }

    if (bReschedule)
    {
        Thread::Yield();
    }

    CS_EXIT();
}

//---------------------------------------------------------------------------
void EventFlag::ClearFlags(K_USHORT usMask_)
{
    CS_ENTER();
    m_usSetMask &= ~usMask_;
    CS_EXIT();
}

//---------------------------------------------------------------------------
K_USHORT EventFlag::GetMask()
{
    K_USHORT usReturn;
    CS_ENTER();
    usReturn = m_usSetMask;
    CS_EXIT();
    return usReturn;
}
