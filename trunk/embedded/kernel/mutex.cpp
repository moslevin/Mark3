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
    \file mutex.cpp
    
    \brief Mutual-exclusion object
*/

#include "kerneltypes.h"
#include "mark3cfg.h"

#include "kernel.h"
#include "blocking.h"
#include "mutex.h"
#include "kernel_debug.h"
#include "transaction.h"

//---------------------------------------------------------------------------
#if defined __FILE_ID__
    #undef __FILE_ID__
#endif
#define __FILE_ID__     MUTEX_CPP


#if KERNEL_USE_MUTEX

//---------------------------------------------------------------------------
#define MUTEX_TRANSACTION_CLAIM		(0)
#define MUTEX_TRANSACTION_RELEASE	(1)
#define MUTEX_TRANSACTION_TIMEOUT	(2)

#if KERNEL_USE_TIMERS

//---------------------------------------------------------------------------
void TimedMutex_Calback(Thread *pclOwner_, void *pvData_)
{
    Mutex *pclMutex = static_cast<Mutex*>(pvData_);    
        
    // Wake up the thread that was blocked on this semaphore.
    pclMutex->Timeout(pclOwner_);
}

//---------------------------------------------------------------------------
void Mutex::Timeout(Thread *pclOwner_)
{
    // Take a lock on the object - if the object is already locked, it means
    // that another context is currently operating within the locked context.
    // In that case, queue an event in the kernel transaction queue, and
    // return out immediately.  The operation will be executed on the
    // thread currently holding the lock.

    K_BOOL bSchedState;
    if (LockAndQueue( MUTEX_TRANSACTION_TIMEOUT, (void*)pclOwner_, &bSchedState))
    {
        return;
    }

    // Drain the FIFO - this will ensure that the operation above is executed,
    // as well as any other queued operations that occur as a reuslt of
    // processing through interrupts.
	if (ProcessQueue()) {
		Thread::Yield();
	}

    // Re-enable the scheduler to its previous state.
	Scheduler::SetScheduler(bSchedState);	
}

#endif

//---------------------------------------------------------------------------
K_BOOL Mutex::ProcessQueue()
{
	Transaction *pclTRX;
	K_BOOL bReschedule = false;

	do
	{
		pclTRX = m_clKTQ.Dequeue();
		KERNEL_ASSERT(pclTRX);

		switch (pclTRX->GetCode())
		{
			case MUTEX_TRANSACTION_CLAIM:
				ClaimTransaction(pclTRX, &bReschedule);
				break;
			case MUTEX_TRANSACTION_RELEASE:
				ReleaseTransaction(pclTRX, &bReschedule);
				break;
#if KERNEL_USE_TIMERS				
			case MUTEX_TRANSACTION_TIMEOUT:
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
void Mutex::ClaimTransaction(Transaction *pclTRX_, K_BOOL *pbReschedule_)
{
	Thread *pclThread = static_cast<Thread*>(pclTRX_->GetData());

	// Check to see if the mutex is claimed or not
	if (m_bReady != 0)
	{
		// Mutex isn't claimed, claim it.
		m_bReady = 0;
		m_ucRecurse = 0;
		m_ucMaxPri = pclThread->GetPriority();
		m_pclOwner = pclThread;
	}
	else
	{
		// If the mutex is already claimed, check to see if this is the owner thread,
		// since we allow the mutex to be claimed recursively.
		if (pclThread == m_pclOwner)
		{
			// Ensure that we haven't exceeded the maximum recursive-lock count
			KERNEL_ASSERT( (m_ucRecurse < 255) );
			m_ucRecurse++;
			return;
		}

		// The mutex is claimed already - we have to block now.  Move the
		// current thread to the list of threads waiting on the mutex.
#if KERNEL_USE_TIMERS
		K_ULONG ulWaitTimeMS = pclThread->GetTimer()->GetInterval();
		pclThread->SetExpired(false);
		if (ulWaitTimeMS)
		{
			pclThread->GetTimer()->Start(0, ulWaitTimeMS, (TimerCallback_t)TimedMutex_Calback, (void*)this);
		}
#endif
		
		Block(pclThread);

		// Check if priority inheritence is necessary.  We do this in order
		// to ensure that we don't end up with priority inversions in case
		// multiple threads are waiting on the same resource.
		
		// We can get away with doing this outside of a critical section, as all
		// transactions are serialized by the transaction queue, and the scheduler
		// is disabled.
		
		if(m_ucMaxPri <= pclThread->GetPriority())
		{
			m_ucMaxPri = pclThread->GetPriority();
			
			{
				Thread *pclTemp = static_cast<Thread*>(m_clBlockList.GetHead());
				while(pclTemp)
				{
					pclTemp->InheritPriority(m_ucMaxPri);
					if(pclTemp == static_cast<Thread*>(m_clBlockList.GetTail()) )
					{
						break;
					}
					pclTemp = static_cast<Thread*>(pclTemp->GetNext());
				}
				m_pclOwner->InheritPriority(m_ucMaxPri);
			}
		}

		*pbReschedule_ = true;
	}
}
	
//---------------------------------------------------------------------------
void Mutex::ReleaseTransaction(Transaction *pclTRX_, K_BOOL *pbReschedule_)
{
	Thread *pclThread;

	// Disable the scheduler while we deal with internal data structures.	
	pclThread = Scheduler::GetCurrentThread();

	// This thread had better be the one that owns the mutex currently...
	KERNEL_ASSERT((pclThread == m_pclOwner));

	// If the owner had claimed the lock multiple times, decrease the lock
	// count and return immediately.
	if (m_ucRecurse)
	{
		m_ucRecurse--;		
		return;
	}

	// Restore the thread's original priority
	if (pclThread->GetCurPriority() != pclThread->GetPriority())
	{
		pclThread->SetPriority(pclThread->GetPriority());
		
		// In this case, we want to reschedule
		*pbReschedule_ = true;
	}

	// No threads are waiting on this semaphore?
	if (m_clBlockList.GetHead() == NULL)
	{
		// Re-initialize the mutex to its default values
		m_bReady = 1;
		m_ucMaxPri = 0;
		m_pclOwner = NULL;
	}
	else
	{
		// Wake the highest priority Thread pending on the mutex
		if(WakeNext())
		{
			// Switch threads if it's higher or equal priority than the current thread
			*pbReschedule_ = true;
		}
	}
}

#if KERNEL_USE_TIMERS
//---------------------------------------------------------------------------
void Mutex::TimeoutTransaction(Transaction *pclTRX_, K_BOOL *pbReschedule_)
{
	Thread *pclChosenOne = static_cast<Thread*>(pclTRX_->GetData());
	
	UnBlock(pclChosenOne);

	pclChosenOne->SetExpired(true);

	if (pclChosenOne->GetPriority() > Scheduler::GetCurrentThread()->GetPriority())
	{
		*pbReschedule_ = true;	
	}	
}
#endif

//---------------------------------------------------------------------------
K_UCHAR Mutex::WakeNext()
{
    Thread *pclChosenOne = NULL;

    // Get the highest priority waiter thread
    pclChosenOne = m_clBlockList.HighestWaiter();
    
    // Unblock the thread
    UnBlock(pclChosenOne);
    
    // The chosen one now owns the mutex
    m_pclOwner = pclChosenOne;

    // Signal a context switch if it's a greater than or equal to the current priority
    if (pclChosenOne->GetPriority() >= Scheduler::GetCurrentThread()->GetPriority())
    {
        return 1;
    }
    return 0;
}

//---------------------------------------------------------------------------
void Mutex::Init()
{
    // Reset the data in the mutex
    m_bReady = 1;             // The mutex is free.
    m_ucMaxPri = 0;           // Set the maximum priority inheritence state
    m_pclOwner = NULL;        // Clear the mutex owner
    m_ucRecurse = 0;          // Reset recurse count
}

//---------------------------------------------------------------------------
#if KERNEL_USE_TIMERS
    void Mutex::Claim()
    {
        Claim(0);
    }
    bool Mutex::Claim(K_ULONG ulWaitTimeMS_)
#else
    void Mutex::Claim()
#endif
{
    KERNEL_TRACE_1( STR_MUTEX_CLAIM_1, (K_USHORT)g_pstCurrent->GetID() );
    
    // Claim the lock (we know only one thread can hold the lock, only one thread can
    // execute at a time, and only threads can call wait)
    K_BOOL bSchedState;
    if (LockAndQueue( MUTEX_TRANSACTION_CLAIM, (void*)g_pstCurrent, &bSchedState))
    {
        Kernel::Panic( PANIC_MUTEX_LOCK_VIOLATION );
    }

    // Set data on the current thread that needs to be passed into the transaction
    // handler (and can't be queued in the simple key-value pair in the transaciton
    // object)
#if KERNEL_USE_TIMERS
	g_pstCurrent->GetTimer()->SetIntervalTicks(ulWaitTimeMS_);
    g_pstCurrent->SetExpired(false);
#endif		

	if (ProcessQueue()) {
		Thread::Yield();
	}

	Scheduler::SetScheduler(bSchedState);
	
#if KERNEL_USE_TIMERS
	if (ulWaitTimeMS_)
	{
		g_pstCurrent->GetTimer()->Stop();
	}
	return (g_pstCurrent->GetExpired() == false);
#endif    
}

//---------------------------------------------------------------------------
void Mutex::Release()
{
    KERNEL_TRACE_1( STR_MUTEX_RELEASE_1, (K_USHORT)g_pstCurrent->GetID() );

    K_BOOL bSchedState;
    if (LockAndQueue( MUTEX_TRANSACTION_RELEASE, (void*)g_pstCurrent, &bSchedState))
    {
        return;
    }

	if (ProcessQueue()) {
		Thread::Yield();
	}

	Scheduler::SetScheduler(bSchedState);    
}

#endif //KERNEL_USE_MUTEX
