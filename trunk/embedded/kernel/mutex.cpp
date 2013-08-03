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

#include "blocking.h"
#include "mutex.h"
#include "kernel_debug.h"
//---------------------------------------------------------------------------
#if defined __FILE_ID__
	#undef __FILE_ID__
#endif
#define __FILE_ID__ 	MUTEX_CPP


#if KERNEL_USE_MUTEX

#if KERNEL_USE_TIMERS

//---------------------------------------------------------------------------
void TimedMutex_Calback(Thread *pclOwner_, void *pvData_)
{
	Mutex *pclMutex = static_cast<Mutex*>(pvData_);
		
	// Indicate that the semaphore has expired on the thread
	pclMutex->SetExpired(true);
		
	// Wake up the thread that was blocked on this semaphore.
	pclMutex->WakeMe(pclOwner_);
		
	if (pclOwner_->GetPriority() > Scheduler::GetCurrentThread()->GetPriority())
	{
		Thread::Yield();
	}
}

//---------------------------------------------------------------------------
void Mutex::WakeMe(Thread *pclOwner_)
{
	// Remove from the semaphore waitlist and back to its ready list.
	UnBlock(pclOwner_);
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
	
    K_UCHAR bSchedule = 0;
    Thread *pclThread;

#if KERNEL_USE_TIMERS
	Timer clTimer;
		
	m_bExpired = false;
#endif

    // Disable the scheduler while claiming the mutex - we're dealing with all
    // sorts of private thread data, can't have a thread switch while messing
    // with internal data structures.
    Scheduler::SetScheduler(0);

    // Get the current thread pointer
    pclThread = Scheduler::GetCurrentThread();

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

            // Increment the lock count and bail
            Scheduler::SetScheduler(1);
#if KERNEL_USE_TIMERS
            return true;
#else
            return;
#endif
        }

        // The mutex is claimed already - we have to block now.  Move the
        // current thread to the list of threads waiting on the mutex.
#if KERNEL_USE_TIMERS		
		if (ulWaitTimeMS_)		
		{
			clTimer.Start(0, ulWaitTimeMS_, (TimerCallback_t)TimedMutex_Calback, (void*)this);	
		}
#endif		
		
        Block(pclThread);

        // Check if priority inheritence is necessary.  We do this in order
        // to ensure that we don't end up with priority inversions in case
        // multiple threads are waiting on the same resource.
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

        // Switch Threads when we exit the critical section.
        bSchedule = 1;
    }

    // Done with thread data -reenable the scheduler
    Scheduler::SetScheduler(1);
    
    if (bSchedule)
    {
        // Switch threads if this thread acquired the mutex
        Thread::Yield();
    }
	
#if KERNEL_USE_TIMERS
	if (ulWaitTimeMS_)
	{
		clTimer.Stop();
	}
	return (m_bExpired == 0);
#endif
}

//---------------------------------------------------------------------------
void Mutex::Release()
{
	KERNEL_TRACE_1( STR_MUTEX_RELEASE_1, (K_USHORT)g_pstCurrent->GetID() );

    K_UCHAR bSchedule = 0;
    Thread *pclThread;

    // Disable the scheduler while we deal with internal data structures.
    Scheduler::SetScheduler(0);
    pclThread = Scheduler::GetCurrentThread();

    // This thread had better be the one that owns the mutex currently...
    KERNEL_ASSERT( (pclThread == m_pclOwner) );

    // If the owner had claimed the lock multiple times, decrease the lock
    // count and return immediately.
    if (m_ucRecurse)
    {
        m_ucRecurse--;
        Scheduler::SetScheduler(1);
        return;
    }

    // Restore the thread's original priority
    if (pclThread->GetCurPriority() != pclThread->GetPriority())
    {
        pclThread->SetPriority(pclThread->GetPriority());
        
        // In this case, we want to reschedule
        bSchedule = 1;
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
            bSchedule = 1;
        }
    }

    // Must enable the scheduler again in order to switch threads.
    Scheduler::SetScheduler(1);
    if(bSchedule)
    {
        // Switch threads if a higher-priority thread was woken
        Thread::Yield();
    }
}

#endif //KERNEL_USE_MUTEX
