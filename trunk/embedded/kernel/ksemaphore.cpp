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

    \file   ksemaphore.cpp

    \brief  Semaphore Blocking-Object Implemenation 

*/

#include "kerneltypes.h"
#include "mark3cfg.h"

#include "kernel.h"
#include "ksemaphore.h"
#include "blocking.h"   
#include "kernel_debug.h"
#include "scheduler.h"
#include "transaction.h"
//---------------------------------------------------------------------------
#if defined __FILE_ID__
    #undef __FILE_ID__
#endif
#define __FILE_ID__     SEMAPHORE_CPP

#if KERNEL_USE_SEMAPHORE

//---------------------------------------------------------------------------
#define SEMAPHORE_TRANSACTION_POST      (0)
#define SEMAPHORE_TRANSACTION_PEND      (1)
#define SEMAPHORE_TRANSACTION_UNBLOCK   (2)

#if KERNEL_USE_TIMERS
#include "timerlist.h"

//---------------------------------------------------------------------------
void TimedSemaphore_Callback(Thread *pclOwner_, void *pvData_)
{
    Semaphore *pclSemaphore = static_cast<Semaphore*>(pvData_);
    
    // Indicate that the semaphore has expired on the thread
    pclOwner_->SetExpired(true);
    
    // Wake up the thread that was blocked on this semaphore.
    pclSemaphore->Timeout(pclOwner_);        
}

//---------------------------------------------------------------------------
void Semaphore::Timeout(Thread *pclChosenOne_)
{    
    K_BOOL bSchedState;
    if (LockAndQueue(SEMAPHORE_TRANSACTION_UNBLOCK, pclChosenOne_, &bSchedState))
    {
        return;
    }

    if (ProcessQueue()) {
        Thread::Yield();
    }

    Scheduler::SetScheduler(bSchedState);
}

#endif // KERNEL_USE_TIMERS

//---------------------------------------------------------------------------
K_BOOL Semaphore::ProcessQueue()
{
    Transaction *pclTRX;
    K_BOOL bReschedule = false;

    do
    {
        pclTRX = m_clKTQ.Dequeue();
        KERNEL_ASSERT(pclTRX);

        switch (pclTRX->GetCode())
        {
            case SEMAPHORE_TRANSACTION_POST:
				PostTransaction(pclTRX, &bReschedule);
                break;
			case SEMAPHORE_TRANSACTION_PEND:
				PendTransaction(pclTRX, &bReschedule);
				break;
            case SEMAPHORE_TRANSACTION_UNBLOCK:
				TimeoutTransaction(pclTRX, &bReschedule);
				break;
            default:
                break;
        }
        m_clKTQ.Finish(pclTRX);
    } while (UnLock() > 1);

    return bReschedule;
}

//---------------------------------------------------------------------------
void Semaphore::PostTransaction(Transaction *pclTRX_, K_BOOL *pbReschedule_)
{
    // If nothing is waiting for the semaphore
	if (m_clBlockList.GetHead() == NULL)
	{
		// Check so see if we've reached the maximum value in the semaphore
		if (m_usValue < m_usMaxValue)
		{
			// Increment the count value
			m_usValue++;
		}
	}
	else
	{
		// Otherwise, there are threads waiting for the semaphore to be
		// posted, so wake the next one (highest priority goes first).
		*pbReschedule_ = WakeNext();
	}            	
}

//---------------------------------------------------------------------------
void Semaphore::PendTransaction(Transaction *pclTRX_, K_BOOL *pbReschedule_)
{
    // Decrement-and-set the semaphore value
    if (0 == m_usValue)
	{
        // The semaphore count is zero - we need to block the current thread
        // and wait until the semaphore is posted from elsewhere.
        *pbReschedule_ = true;

		// Get the current thread pointer.
		Thread *pclThread = static_cast<Thread*>(pclTRX_->GetData());

#if KERNEL_USE_TIMERS
		Timer *pclSemTimer = pclThread->GetTimer();
		pclThread->SetExpired(false);
		K_ULONG ulWaitTimeMS = pclSemTimer->GetInterval();

		if (ulWaitTimeMS)
		{
			pclSemTimer->Start(0, ulWaitTimeMS, TimedSemaphore_Callback, (void*)this);
		}
#endif
		Block(pclThread);
	}	
    else
    {
        m_usValue--;
    }
}
	
//---------------------------------------------------------------------------
void Semaphore::TimeoutTransaction(Transaction *pclTRX_, K_BOOL *pbReschedule_)
{
	Thread *pclChosenOne = static_cast<Thread*>(pclTRX_->GetData());

	UnBlock(pclChosenOne);

	// Call a task switch only if higher priority thread
	if (pclChosenOne->GetPriority() > Scheduler::GetCurrentThread()->GetPriority())
	{
		*pbReschedule_ = true;
	}	
}
	
//---------------------------------------------------------------------------
K_BOOL Semaphore::WakeNext()
{
    Thread *pclChosenOne;
    
    pclChosenOne = m_clBlockList.HighestWaiter();
    
    // Remove from the semaphore waitlist and back to its ready list.
    UnBlock(pclChosenOne);

    // Call a task switch only if higher priority thread
    if (pclChosenOne->GetPriority() > Scheduler::GetCurrentThread()->GetPriority())
    {
        return true;
    }

    return false;
}

//---------------------------------------------------------------------------
void Semaphore::Init(K_USHORT usInitVal_, K_USHORT usMaxVal_)
{
    // Copy the paramters into the object - set the maximum value for this
    // semaphore to implement either binary or counting semaphores, and set
    // the initial count.  Clear the wait list for this object.
    m_usValue = usInitVal_;
    m_usMaxValue = usMaxVal_;    

    m_clBlockList.Init();
}

//---------------------------------------------------------------------------
void Semaphore::Post()
{
    KERNEL_TRACE_1( STR_SEMAPHORE_POST_1, (K_USHORT)Scheduler::GetCurrentThread()->GetID() );

    K_BOOL bSchedState;
    if (LockAndQueue(SEMAPHORE_TRANSACTION_POST, 0, &bSchedState))
    {
        return;
    }

    if (ProcessQueue()) {
        Thread::Yield();
    }

    Scheduler::SetScheduler(bSchedState);

    return;
}

#if !KERNEL_USE_TIMERS
//---------------------------------------------------------------------------
    // No timers, no timed pend
    void Semaphore::Pend()
#else
//---------------------------------------------------------------------------
    // Redirect the untimed pend API to the timed pend, with a null timeout.
    void Semaphore::Pend()
    {
        Pend(0);
    }
//---------------------------------------------------------------------------    
    bool Semaphore::Pend( K_ULONG ulWaitTimeMS_ )
#endif
{
    KERNEL_TRACE_1( STR_SEMAPHORE_PEND_1, (K_USHORT)Scheduler::GetCurrentThread()->GetID() );

	// By locking the queue, we ensure that any post/unblock operations on this
	// semaphore that interrupt our normal execution wind up being queued flushed
	// before we exit.

    K_BOOL bSchedState;
    if (LockAndQueue(SEMAPHORE_TRANSACTION_PEND, (void*)Scheduler::GetCurrentThread(), &bSchedState))
    {
        // This should never happen - kernel panic if we do.
        Kernel::Panic( PANIC_PEND_LOCK_VIOLATION );
    }

    // Set data on the current thread that needs to be passed into the transaction
    // handler (and can't be queued in the simple key-value pair in the transaciton
    // object)

#if KERNEL_USE_TIMERS
    // Pre-set the interval, since we can't cache it in the transaction
    Scheduler::GetCurrentThread()->GetTimer()->SetIntervalTicks(ulWaitTimeMS_);
    Scheduler::GetCurrentThread()->SetExpired(false);
#endif	

    if (ProcessQueue())
    {
        // Switch Threads immediately
        Thread::Yield();
    }
    
	Scheduler::SetScheduler(bSchedState);
    
#if KERNEL_USE_TIMERS
    if (ulWaitTimeMS_)
    {
        Scheduler::GetCurrentThread()->GetTimer()->Stop();
    }
    K_BOOL retVal = (Scheduler::GetCurrentThread()->GetExpired() == false);
	
    return retVal;
#endif
}

//---------------------------------------------------------------------------
K_USHORT Semaphore::GetCount()
{
    K_USHORT usRet;
    CS_ENTER();
    usRet = m_usValue;
    CS_EXIT();
    return usRet;
}

#endif
