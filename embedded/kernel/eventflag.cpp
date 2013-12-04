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

#if KERNEL_USE_EVENTFLAG

//---------------------------------------------------------------------------
#define EVENT_TRANSACTION_WAIT		(0)
#define EVENT_TRANSACTION_SET		(1)
#define EVENT_TRANSACTION_CLEAR		(2)
#define EVENT_TRANSACTION_TIMEOUT	(3)

#if KERNEL_USE_TIMERS
#include "timerlist.h"
//---------------------------------------------------------------------------
void TimedEventFlag_Callback(Thread *pclOwner_, void *pvData_)
{
    EventFlag *pclEventFlag = static_cast<EventFlag*>(pvData_);

    // The blocking operation timed out before it occurred.  Allow the
    // object to unblock the thread.
    pclEventFlag->Timeout(pclOwner_);    
}

//---------------------------------------------------------------------------
void EventFlag::Timeout(Thread *pclChosenOne_)
{
    // Take a lock on the object - if the object is already locked, it means
    // that another context is currently operating within the locked context.
    // In that case, queue an event in the kernel transaction queue, and
    // return out immediately.  The operation will be executed on the
    // thread currently holding the lock.
    K_BOOL bSchedState;
    if (LockAndQueue( EVENT_TRANSACTION_TIMEOUT, (void*)pclChosenOne_, &bSchedState))
    {
        return;
    }

    // Drain the FIFO - this will ensure that the operation above is executed,
    // as well as any other queued operations that occur as a reuslt of
    // processing through interrupts.
    if (ProcessQueue())
    {
        // If a new thread needs to be chosen, call yield
	    Thread::Yield();
    }

    // Re-enable the scheduler to its previous state.
    Scheduler::SetScheduler(bSchedState);
}

//---------------------------------------------------------------------------
K_USHORT EventFlag::Wait(K_USHORT usMask_, EventFlagOperation_t eMode_)
{
    return Wait(usMask_, eMode_, 0);
}
K_USHORT EventFlag::Wait(K_USHORT usMask_, EventFlagOperation_t eMode_, K_ULONG ulTimeMS_)
#else
K_USHORT EventFlag::Wait(K_USHORT usMask_, EventFlagOperation_t eMode_)
#endif
{
    // Claim the lock (we know only one thread can hold the lock, only one thread can
    // execute at a time, and only threads can call wait)
    K_BOOL bSchedState;
    if (LockAndQueue(EVENT_TRANSACTION_WAIT, (void*)usMask_, &bSchedState))
    {
        // This should never be able to happen with the logic implemented above
        Kernel::Panic( PANIC_EVENT_LOCK_VIOLATION );
    }
	
    // Set data on the current thread that needs to be passed into the transaction
    // handler (and can't be queued in the simple key-value pair in the transaciton
    // object)
	g_pstCurrent->SetEventFlagMode(eMode_);
#if KERNEL_USE_TIMERS
	g_pstCurrent->GetTimer()->SetIntervalTicks(ulTimeMS_);
    g_pstCurrent->SetExpired(false);
#endif
	
    // Drain the FIFO of all queued events and trigger a context switch if necessary
	if (ProcessQueue())
	{
		Thread::Yield();
	}
	
    // Re-enable the scheduler
	Scheduler::SetScheduler(bSchedState);

//!! If the Yield operation causes a new thread to be chosen, there will
//!! Be a context switch at the above CS_EXIT().  The original calling
//!! thread will not return back until a matching SetFlags call is made.

#if KERNEL_USE_TIMERS
    if (ulTimeMS_)
    {
        g_pstCurrent->GetTimer()->Stop();
    }
#endif

    return g_pstCurrent->GetEventFlagMask();
}

//---------------------------------------------------------------------------
K_BOOL EventFlag::ProcessQueue()
{
	Transaction *pclTRX;
	K_BOOL bReschedule = false;

	do
	{
		pclTRX = m_clKTQ.Dequeue();
		KERNEL_ASSERT(pclTRX);

		switch (pclTRX->GetCode())
		{
			case EVENT_TRANSACTION_WAIT:
				WaitTransaction(pclTRX, &bReschedule);
				break;
			case EVENT_TRANSACTION_SET:
				SetTransaction(pclTRX, &bReschedule);
				break;
			case EVENT_TRANSACTION_CLEAR:
				ClearTransaction(pclTRX, &bReschedule);
				break;
#if KERNEL_USE_TIMERS			
			case EVENT_TRANSACTION_TIMEOUT:
				TimeoutTransaction(pclTRX, &bReschedule);
				break;
#endif				
			default:
				break;
		}
		m_clKTQ.Finish(pclTRX);
	} while (UnLock() > 1);

	return bReschedule;
}

//---------------------------------------------------------------------------
void EventFlag::WaitTransaction( Transaction *pclTRX_, K_BOOL *pbReschedule_ )
{
	bool bMatch = false;
	Thread *pclThread = Scheduler::GetCurrentThread();
	K_USHORT usMask = (K_USHORT)(pclTRX_->GetData());
	
#if KERNEL_USE_TIMERS
	Timer *pclTimer = pclThread->GetTimer();
	pclThread->SetExpired(false);
#endif

	// Check to see whether or not the current mask matches any of the
	// desired bits.
		
	EventFlagOperation_t eMode = pclThread->GetEventFlagMode();
	if ((eMode == EVENT_FLAG_ALL) || (eMode == EVENT_FLAG_ALL_CLEAR))
	{
		// Check to see if the flags in their current state match all of
		// the set flags in the event flag group, with this mask.
		if ((m_usSetMask & usMask) == usMask)
		{
			bMatch = true;
			pclThread->SetEventFlagMask(usMask);
		}
	}
	else if ((eMode == EVENT_FLAG_ANY) || (eMode == EVENT_FLAG_ANY_CLEAR))
	{
		// Check to see if the existing flags match any of the set flags in
		// the event flag group  with this mask
		if (m_usSetMask & usMask)
		{
			bMatch = true;
			pclThread->SetEventFlagMask(m_usSetMask & usMask);
		}
	}

	// We're unable to match this pattern as-is, so we must block.
	if (!bMatch)
	{
		// Reset the current thread's event flag mask & mode
		pclThread->SetEventFlagMask(usMask);
		pclThread->SetEventFlagMode(eMode);

#if KERNEL_USE_TIMERS
		K_ULONG ulTimeMS = pclTimer->GetInterval();
		if (ulTimeMS)
		{
			pclTimer->Start(0, ulTimeMS, TimedEventFlag_Callback, (void*)this);
		}
#endif

		// Add the thread to the object's block-list.
		Block(pclThread);

		*pbReschedule_ = true;
	}
}

//---------------------------------------------------------------------------	
void EventFlag::SetTransaction( Transaction *pclTRX_, K_BOOL *pbReschedule_ )
{
	Thread *pclPrev;
	Thread *pclCurrent;
	   
	K_USHORT usNewMask;
	K_USHORT usMask = (K_USHORT)(pclTRX_->GetData());
	// Walk through the whole block list, checking to see whether or not
	// the current flag set now matches any/all of the masks and modes of
	// the threads involved.

	m_usSetMask |= usMask;
	usNewMask = m_usSetMask;

	// Start at the head of the list, and iterate through until we hit the
	// "head" element in the list again.  Ensure that we handle the case where
	// we remove the first or last elements in the list, or if there's only
	// one element in the list.
	pclCurrent = static_cast<Thread*>(m_clBlockList.GetHead());

	// Do nothing when there are no objects blocking.
	if (pclCurrent)
	{
		// First loop - process every thread in the block-list and check to
		// see whether or not the current flags match the event-flag conditions
		// on the thread.
		do
		{
			pclPrev = pclCurrent;
			pclCurrent = static_cast<Thread*>(pclCurrent->GetNext());

			// Read the thread's event mask/mode
			K_USHORT usThreadMask = pclPrev->GetEventFlagMask();
			EventFlagOperation_t eThreadMode = pclPrev->GetEventFlagMode();

			// For the "any" mode - unblock the blocked threads if one or more bits
			// in the thread's bitmask match the object's bitmask
			if ((EVENT_FLAG_ANY == eThreadMode) || (EVENT_FLAG_ANY_CLEAR == eThreadMode))
			{
				if (usThreadMask & m_usSetMask)
				{
					pclPrev->SetEventFlagMode(EVENT_FLAG_PENDING_UNBLOCK);
					pclPrev->SetEventFlagMask(m_usSetMask & usThreadMask);
					*pbReschedule_ = true;

					// If the "clear" variant is set, then clear the bits in the mask
					// that caused the thread to unblock.
					if (EVENT_FLAG_ANY_CLEAR == eThreadMode)
					{
						usNewMask &=~ (usThreadMask & usMask);
					}
				}
			}
			// For the "all" mode, every set bit in the thread's requested bitmask must
			// match the object's flag mask.
			else if ((EVENT_FLAG_ALL == eThreadMode) || (EVENT_FLAG_ALL_CLEAR == eThreadMode))
			{
				if ((usThreadMask & m_usSetMask) == usThreadMask)
				{
					pclPrev->SetEventFlagMode(EVENT_FLAG_PENDING_UNBLOCK);
					pclPrev->SetEventFlagMask(usThreadMask);
					*pbReschedule_ = true;

					// If the "clear" variant is set, then clear the bits in the mask
					// that caused the thread to unblock.
					if (EVENT_FLAG_ALL_CLEAR == eThreadMode)
					{
						usNewMask &=~ (usThreadMask & usMask);
					}
				}
			}
		}
		// To keep looping, ensure that there's something in the list, and
		// that the next item isn't the head of the list.
		while (pclPrev != m_clBlockList.GetTail());

		// Second loop - go through and unblock all of the threads that
		// were tagged for unblocking.
		pclCurrent = static_cast<Thread*>(m_clBlockList.GetHead());
		bool bIsTail = false;
		do
		{
			pclPrev = pclCurrent;
			pclCurrent = static_cast<Thread*>(pclCurrent->GetNext());

			// Check to see if this is the condition to terminate the loop
			if (pclPrev == m_clBlockList.GetTail())
			{
				bIsTail = true;
			}

			// If the first pass indicated that this thread should be
			// unblocked, then unblock the thread
			if (pclPrev->GetEventFlagMode() == EVENT_FLAG_PENDING_UNBLOCK)
			{
				UnBlock(pclPrev);
			}
		}
		while (!bIsTail);
	}

	// Update the bitmask based on any "clear" operations performed along
	// the way
	m_usSetMask = usNewMask;
}
	
//---------------------------------------------------------------------------
void EventFlag::ClearTransaction( Transaction *pclTRX_, K_BOOL *pbReschedule_ )
{
    m_usSetMask &= ~((K_USHORT)(pclTRX_->GetData()));
}

#if KERNEL_USE_TIMERS
//---------------------------------------------------------------------------
void EventFlag::TimeoutTransaction( Transaction *pclTRX_, K_BOOL *pbReschedule_ )
{
	Thread *pclChosenOne = static_cast<Thread*>(pclTRX_->GetData());
		
	UnBlock(pclChosenOne);
	
	pclChosenOne->SetExpired(true);
	pclChosenOne->SetEventFlagMask(0);

	if (pclChosenOne->GetPriority() > Scheduler::GetCurrentThread()->GetPriority())
	{
		*pbReschedule_ = true;
	}
}
#endif

//---------------------------------------------------------------------------
void EventFlag::Set(K_USHORT usMask_)
{
    // This function follows the signature of Wait() and Timeout()
    K_BOOL bSchedState;
    if (LockAndQueue( EVENT_TRANSACTION_SET, (void*)usMask_, &bSchedState))
    {
        return;
    }

	if (ProcessQueue())
	{
		Thread::Yield();
	}
	
	Scheduler::SetScheduler(bSchedState);	
}

//---------------------------------------------------------------------------
void EventFlag::Clear(K_USHORT usMask_)
{
    // This function follows the signature of Wait() and Timeout()
    K_BOOL bSchedState;
    if (LockAndQueue( EVENT_TRANSACTION_CLEAR, (void*)usMask_, &bSchedState))
    {
        return;
    }

	if (ProcessQueue())
	{
		Thread::Yield();
	}
	
	Scheduler::SetScheduler(bSchedState);
}

//---------------------------------------------------------------------------
K_USHORT EventFlag::GetMask()
{
    // Return the presently held event flag values in this object.  Ensure
    // we get this within a critical section to guarantee atomicity.
    K_USHORT usReturn;
    CS_ENTER();
    usReturn = m_usSetMask;
    CS_EXIT();
    return usReturn;
}

#endif // KERNEL_USE_EVENTFLAG
