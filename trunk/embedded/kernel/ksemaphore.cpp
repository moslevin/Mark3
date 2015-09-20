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

    \file   ksemaphore.cpp

    \brief  Semaphore Blocking-Object Implemenation 

*/

#include "kerneltypes.h"
#include "mark3cfg.h"

#include "ksemaphore.h"
#include "blocking.h"   
#include "kerneldebug.h"
//---------------------------------------------------------------------------
#if defined __FILE_ID__
	#undef __FILE_ID__
#endif
#define __FILE_ID__ 	SEMAPHORE_CPP       //!< File ID used in kernel trace calls

#if KERNEL_USE_SEMAPHORE

#if KERNEL_USE_TIMEOUTS
#include "timerlist.h"

//---------------------------------------------------------------------------
/*!
 * \brief TimedSemaphore_Callback
 *
 * This function is called from the timer-expired context to trigger a timeout
 * on this semphore.  This results in the waking of the thread that generated
 * the semaphore pend call that was not completed in time.
 *
 * \param pclOwner_ Pointer to the thread to wake
 * \param pvData_   Pointer to the semaphore object that the thread is blocked on
 */
void TimedSemaphore_Callback(Thread *pclOwner_, void *pvData_)
{
	Semaphore *pclSemaphore = static_cast<Semaphore*>(pvData_);
	
	// Indicate that the semaphore has expired on the thread
	pclOwner_->SetExpired(true);
	
	// Wake up the thread that was blocked on this semaphore.
	pclSemaphore->WakeMe(pclOwner_);
	

    if (pclOwner_->GetCurPriority() >= Scheduler::GetCurrentThread()->GetCurPriority())
	{
		Thread::Yield();
	}	
}

//---------------------------------------------------------------------------
void Semaphore::WakeMe(Thread *pclChosenOne_)
{ 
    // Remove from the semaphore waitlist and back to its ready list.
    UnBlock(pclChosenOne_);
}

#endif // KERNEL_USE_TIMEOUTS

//---------------------------------------------------------------------------
uint8_t Semaphore::WakeNext()
{
    Thread *pclChosenOne;
    
    pclChosenOne = m_clBlockList.HighestWaiter();
    
    // Remove from the semaphore waitlist and back to its ready list.
    UnBlock(pclChosenOne);

    // Call a task switch if higher or equal priority thread
    if (pclChosenOne->GetCurPriority() >= Scheduler::GetCurrentThread()->GetCurPriority())
    {
        return 1;
    }
    return 0;
}

//---------------------------------------------------------------------------
void Semaphore::Init(uint16_t u16InitVal_, uint16_t u16MaxVal_)
{
    // Copy the paramters into the object - set the maximum value for this
    // semaphore to implement either binary or counting semaphores, and set
    // the initial count.  Clear the wait list for this object.
    m_u16Value = u16InitVal_;
    m_u16MaxValue = u16MaxVal_;    

    m_clBlockList.Init();
}

//---------------------------------------------------------------------------
bool Semaphore::Post()
{
	KERNEL_TRACE_1( STR_SEMAPHORE_POST_1, (uint16_t)g_pclCurrent->GetID() );
	
    bool bThreadWake = 0;
    bool bBail = false;
    // Increment the semaphore count - we can mess with threads so ensure this
    // is in a critical section.  We don't just disable the scheudler since
    // we want to be able to do this from within an interrupt context as well.
    CS_ENTER();

    // If nothing is waiting for the semaphore
    if (m_clBlockList.GetHead() == NULL)
    {
        // Check so see if we've reached the maximum value in the semaphore
        if (m_u16Value < m_u16MaxValue)
        {
            // Increment the count value
            m_u16Value++;
        }
        else
        {
            // Maximum value has been reached, bail out.
            bBail = true;
        }
    }
    else
    {
        // Otherwise, there are threads waiting for the semaphore to be
        // posted, so wake the next one (highest priority goes first).
        bThreadWake = WakeNext();
    }

    CS_EXIT();

    // If we weren't able to increment the semaphore count, fail out.
    if (bBail)
    {
        return false;
    }

    // if bThreadWake was set, it means that a higher-priority thread was
    // woken.  Trigger a context switch to ensure that this thread gets
    // to execute next.
    if (bThreadWake)
    {
        Thread::Yield();
    }
    return true;
}

//---------------------------------------------------------------------------
#if KERNEL_USE_TIMEOUTS
bool Semaphore::Pend_i( uint32_t u32WaitTimeMS_ )
#else
void Semaphore::Pend_i( void )
#endif
{
    KERNEL_TRACE_1( STR_SEMAPHORE_PEND_1, (uint16_t)g_pclCurrent->GetID() );

#if KERNEL_USE_TIMEOUTS
    Timer clSemTimer;
    bool bUseTimer = false;
#endif

    // Once again, messing with thread data - ensure
    // we're doing all of these operations from within a thread-safe context.
    CS_ENTER();

    // Check to see if we need to take any action based on the semaphore count
    if (m_u16Value != 0)
    {
        // The semaphore count is non-zero, we can just decrement the count
        // and go along our merry way.
        m_u16Value--;
    }
    else
    {
        // The semaphore count is zero - we need to block the current thread
        // and wait until the semaphore is posted from elsewhere.
#if KERNEL_USE_TIMEOUTS        
        if (u32WaitTimeMS_)
        {
            g_pclCurrent->SetExpired(false);
            clSemTimer.Init();
            clSemTimer.Start(0, u32WaitTimeMS_, TimedSemaphore_Callback, (void*)this);
            bUseTimer = true;
        }
#endif
        Block(g_pclCurrent);

        // Switch Threads immediately
        Thread::Yield();
    }

    CS_EXIT();

#if KERNEL_USE_TIMEOUTS
    if (bUseTimer)
    {
        clSemTimer.Stop();
        return (g_pclCurrent->GetExpired() == 0);
    }
    return true;
#endif
}

//---------------------------------------------------------------------------
// Redirect the untimed pend API to the timed pend, with a null timeout.
void Semaphore::Pend()
{
#if KERNEL_USE_TIMEOUTS
    Pend_i(0);
#else
    Pend_i();
#endif
}

#if KERNEL_USE_TIMEOUTS
//---------------------------------------------------------------------------	
bool Semaphore::Pend( uint32_t u32WaitTimeMS_ )
{
    return Pend_i( u32WaitTimeMS_ );
}
#endif

//---------------------------------------------------------------------------
uint16_t Semaphore::GetCount()
{
	uint16_t u16Ret;
	CS_ENTER();
	u16Ret = m_u16Value;
	CS_EXIT();
	return u16Ret;
}

#endif
