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

    \file   quantum.cpp

    \brief  Thread Quantum Implementation for Round-Robin Scheduling

*/

#include "kerneltypes.h"
#include "mark3cfg.h"

#include "thread.h"
#include "timerlist.h"
#include "quantum.h"
#include "kerneldebug.h"
#include "kernelaware.h"
//---------------------------------------------------------------------------
#if defined __FILE_ID__
	#undef __FILE_ID__
#endif
#define __FILE_ID__ 	QUANTUM_CPP       //!< File ID used in kernel trace calls

#if KERNEL_USE_QUANTUM

//---------------------------------------------------------------------------
static volatile bool bAddQuantumTimer;	// Indicates that a timer add is pending

//---------------------------------------------------------------------------
Timer   Quantum::m_clQuantumTimer;	// The global timernodelist_t object
bool Quantum::m_bActive;
bool Quantum::m_bInTimer;
//---------------------------------------------------------------------------
/*!
 * \brief QuantumCallback
 *
 * This is the timer callback that is invoked whenever a thread has exhausted
 * its current execution quantum and a new thread must be chosen from within
 * the same priority level.
 *
 * \param pclThread_ Pointer to the thread currently executing
 * \param pvData_ Unused in this context.
 */
static void QuantumCallback(Thread *pclThread_, void *pvData_)
{
    // Validate thread pointer, check that source/destination match (it's
    // in its real priority list).  Also check that this thread was part of
    // the highest-running priority level.
    if (pclThread_->GetPriority() >= Scheduler::GetCurrentThread()->GetPriority())
    {
        if (pclThread_->GetCurrent()->GetHead() != pclThread_->GetCurrent()->GetTail() )
        {
            bAddQuantumTimer = true;
            pclThread_->GetCurrent()->PivotForward();
        }
    }
}

//---------------------------------------------------------------------------
void Quantum::SetTimer(Thread *pclThread_)
{
    m_clQuantumTimer.SetIntervalMSeconds(pclThread_->GetQuantum());
    m_clQuantumTimer.SetFlags(TIMERLIST_FLAG_ONE_SHOT);
    m_clQuantumTimer.SetData(NULL);
    m_clQuantumTimer.SetCallback((TimerCallback_t)QuantumCallback);
    m_clQuantumTimer.SetOwner(pclThread_);
}

//---------------------------------------------------------------------------
void Quantum::AddThread(Thread *pclThread_)
{
    if (m_bActive
#if KERNEL_USE_IDLE_FUNC
            || (pclThread_ == Kernel::GetIdleThread())
#endif
       )
	{
		return;	
	}		
	
	// If this is called from the timer callback, queue a timer add...
	if (m_bInTimer)
	{
		bAddQuantumTimer = true;
		return;
	}
	
    // If this isn't the only thread in the list.
    if ( pclThread_->GetCurrent()->GetHead() != 
           pclThread_->GetCurrent()->GetTail() )	   
    {
        Quantum::SetTimer(pclThread_);   
        TimerScheduler::Add(&m_clQuantumTimer);
		m_bActive = 1;
    }    
}

//---------------------------------------------------------------------------
void Quantum::RemoveThread(void)
{
	if (!m_bActive)
	{
		return;
	}		

    // Cancel the current timer
    TimerScheduler::Remove(&m_clQuantumTimer);
	m_bActive = 0;
}

//---------------------------------------------------------------------------
void Quantum::UpdateTimer(void)
{
    // If we have to re-add the quantum timer (more than 2 threads at the 
    // high-priority level...)
    if (bAddQuantumTimer)
    {
        // Trigger a thread yield - this will also re-schedule the 
		// thread *and* reset the round-robin scheduler. 
        Thread::Yield();
		bAddQuantumTimer = false;		
    }    
}

#endif //KERNEL_USE_QUANTUM
