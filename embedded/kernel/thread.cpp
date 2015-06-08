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

    \file   thread.cpp

    \brief  Platform-Independent thread class Definition

*/

#include "kerneltypes.h"
#include "mark3cfg.h"

#include "thread.h" 
#include "scheduler.h"
#include "kernelswi.h"
#include "timerlist.h"
#include "ksemaphore.h"
#include "quantum.h"
#include "kernel.h"
#include "kerneldebug.h"

//---------------------------------------------------------------------------
#if defined __FILE_ID__
    #undef __FILE_ID__
#endif
#define __FILE_ID__     THREAD_CPP       //!< File ID used in kernel trace calls

//---------------------------------------------------------------------------
void Thread::Init(  K_WORD *pwStack_,
                K_USHORT usStackSize_,
                K_UCHAR ucPriority_,                
                ThreadEntry_t pfEntryPoint_,
                void *pvArg_ )
{
    static K_UCHAR ucThreadID = 0;

    KERNEL_ASSERT( pwStack_ );
    KERNEL_ASSERT( pfEntryPoint_ );
    
    ClearNode();

    m_ucThreadID = ucThreadID++;
    
    KERNEL_TRACE_1( STR_STACK_SIZE_1, usStackSize_ );
    KERNEL_TRACE_1( STR_PRIORITY_1, (K_UCHAR)ucPriority_ );
    KERNEL_TRACE_1( STR_THREAD_ID_1, (K_USHORT)m_ucThreadID );
    KERNEL_TRACE_1( STR_ENTRYPOINT_1, (K_USHORT)pfEntryPoint_ );
    
    // Initialize the thread parameters to their initial values.
    m_pwStack = pwStack_;
    m_pwStackTop = TOP_OF_STACK(pwStack_, usStackSize_);
    
    m_usStackSize = usStackSize_;
    
#if KERNEL_USE_QUANTUM    
    m_usQuantum = THREAD_QUANTUM_DEFAULT;
#endif

    m_ucPriority = ucPriority_ ;
    m_ucCurPriority = m_ucPriority;
    m_pfEntryPoint = pfEntryPoint_;
    m_pvArg = pvArg_;
    m_eState = THREAD_STATE_STOP;
    
#if KERNEL_USE_THREADNAME    
    m_szName = NULL;
#endif
#if KERNEL_USE_TIMERS
    m_clTimer.Init();
#endif

    // Call CPU-specific stack initialization
    ThreadPort::InitStack(this);
    
    // Add to the global "stop" list.
    CS_ENTER();
    m_pclOwner = Scheduler::GetThreadList(m_ucPriority);
    m_pclCurrent = Scheduler::GetStopList();
    m_pclCurrent->Add(this);
    CS_EXIT();
}

//---------------------------------------------------------------------------
void Thread::Start(void)
{
    // Remove the thread from the scheduler's "stopped" list, and add it 
    // to the scheduler's ready list at the proper priority.
    KERNEL_TRACE_1( STR_THREAD_START_1, (K_USHORT)m_ucThreadID );
    
    CS_ENTER();
    Scheduler::GetStopList()->Remove(this);
    Scheduler::Add(this);    
    m_pclOwner = Scheduler::GetThreadList(m_ucPriority);
    m_pclCurrent = m_pclOwner;
    m_eState = THREAD_STATE_READY;

#if KERNEL_USE_QUANTUM
    if (GetCurPriority() >= Scheduler::GetCurrentThread()->GetCurPriority())
    {
        // Deal with the thread Quantum
        Quantum::RemoveThread();
        Quantum::AddThread(this);
    }
#endif

    if (Kernel::IsStarted())
    {
        if (GetCurPriority() >= Scheduler::GetCurrentThread()->GetCurPriority())
        {
            Thread::Yield();
        }
    }
    CS_EXIT();
}

//---------------------------------------------------------------------------
void Thread::Stop()
{
    K_BOOL bReschedule = 0;

    CS_ENTER();

    // If a thread is attempting to stop itself, ensure we call the scheduler
    if (this == Scheduler::GetCurrentThread())
    {
        bReschedule = true;
    }

    // Add this thread to the stop-list (removing it from active scheduling)
    // Remove the thread from scheduling
    if (m_eState == THREAD_STATE_READY)
    {
        Scheduler::Remove(this);
    }
    else if (m_eState == THREAD_STATE_BLOCKED)
    {
        m_pclCurrent->Remove(this);
    }

    m_pclOwner = Scheduler::GetStopList();
    m_pclCurrent = m_pclOwner;
    m_pclOwner->Add(this);
    m_eState = THREAD_STATE_STOP;

#if KERNEL_USE_TIMERS
    // Just to be safe - attempt to remove the thread's timer
    // from the timer-scheduler (does no harm if it isn't
    // in the timer-list)
    TimerScheduler::Remove(&m_clTimer);
#endif

    CS_EXIT();

    if (bReschedule)
    {
        Thread::Yield();
    }
}

#if KERNEL_USE_DYNAMIC_THREADS
//---------------------------------------------------------------------------
void Thread::Exit()
{
    K_BOOL bReschedule = 0;
    
    KERNEL_TRACE_1( STR_THREAD_EXIT_1, m_ucThreadID );
    
    CS_ENTER();
    
    // If this thread is the actively-running thread, make sure we run the
    // scheduler again.
    if (this == Scheduler::GetCurrentThread())
    {
        bReschedule = 1;            
    }
    
    // Remove the thread from scheduling
    if (m_eState == THREAD_STATE_READY)
    {
        Scheduler::Remove(this);
    }
    else if (m_eState == THREAD_STATE_BLOCKED)
    {
        m_pclCurrent->Remove(this);
    }

    m_pclCurrent = 0;
    m_pclOwner = 0;
    m_eState = THREAD_STATE_EXIT;

    // We've removed the thread from scheduling, but interrupts might
    // trigger checks against this thread's currently priority before
    // we get around to scheduling new threads.  As a result, set the
    // priority to idle to ensure that we always wind up scheduling
    // new threads.
    m_ucCurPriority = 0;
    m_ucPriority = 0;

#if KERNEL_USE_TIMERS
    // Just to be safe - attempt to remove the thread's timer
    // from the timer-scheduler (does no harm if it isn't
    // in the timer-list)
    TimerScheduler::Remove(&m_clTimer);
#endif

    CS_EXIT();
    
    if (bReschedule) 
    {
        // Choose a new "next" thread if we must
        Thread::Yield();
    }
}
#endif

#if KERNEL_USE_SLEEP
//---------------------------------------------------------------------------
//! This callback is used to wake up a thread once the interval has expired
static void ThreadSleepCallback( Thread *pclOwner_, void *pvData_ )
{
    Semaphore *pclSemaphore = static_cast<Semaphore*>(pvData_);
    // Post the semaphore, which will wake the sleeping thread.
    pclSemaphore->Post();
}

//---------------------------------------------------------------------------
void Thread::Sleep(K_ULONG ulTimeMs_)
{    
    Semaphore clSemaphore;
    Timer *pclTimer = g_pstCurrent->GetTimer();

    // Create a semaphore that this thread will block on
    clSemaphore.Init(0, 1);
    
    // Create a one-shot timer that will call a callback that posts the 
    // semaphore, waking our thread.
    pclTimer->Init();
    pclTimer->SetIntervalMSeconds(ulTimeMs_);
    pclTimer->SetCallback(ThreadSleepCallback);
    pclTimer->SetData((void*)&clSemaphore);
    pclTimer->SetFlags(TIMERLIST_FLAG_ONE_SHOT);
    
    // Add the new timer to the timer scheduler, and block the thread
    TimerScheduler::Add(pclTimer);
    clSemaphore.Pend();
}

//---------------------------------------------------------------------------
void Thread::USleep(K_ULONG ulTimeUs_)
{    
    Semaphore clSemaphore;
    Timer *pclTimer = g_pstCurrent->GetTimer();

    // Create a semaphore that this thread will block on
    clSemaphore.Init(0, 1);

    // Create a one-shot timer that will call a callback that posts the
    // semaphore, waking our thread.
    pclTimer->Init();
    pclTimer->SetIntervalUSeconds(ulTimeUs_);
    pclTimer->SetCallback(ThreadSleepCallback);
    pclTimer->SetData((void*)&clSemaphore);
    pclTimer->SetFlags(TIMERLIST_FLAG_ONE_SHOT);

    // Add the new timer to the timer scheduler, and block the thread
    TimerScheduler::Add(pclTimer);
    clSemaphore.Pend();
}
#endif // KERNEL_USE_SLEEP

//---------------------------------------------------------------------------
K_USHORT Thread::GetStackSlack()
{
    K_USHORT usCount = 0;
    
    CS_ENTER();
    
    //!! ToDo: Take into account stacks that grow up
    for (usCount = 0; usCount < m_usStackSize; usCount++)
    {
        if (m_pwStack[usCount] != 0xFF)
        {
            break;
        }
    }
    
    CS_EXIT();
    
    return usCount;
}

//---------------------------------------------------------------------------
void Thread::Yield()
{
    CS_ENTER();
    // Run the scheduler
    if (Scheduler::IsEnabled())
    {
        Scheduler::Schedule();

        // Only switch contexts if the new task is different than the old task
        if (Scheduler::GetCurrentThread() != Scheduler::GetNextThread())
        {
#if KERNEL_USE_QUANTUM
            // new thread scheduled.  Stop current quantum timer (if it exists),
            // and restart it for the new thread (if required).
            Quantum::RemoveThread();
            Quantum::AddThread((Thread*)g_pstNext);
#endif
            Thread::ContextSwitchSWI();
        }
    }
    else
    {
        Scheduler::QueueScheduler();
    }

    CS_EXIT();
}

//---------------------------------------------------------------------------
void Thread::SetPriorityBase(K_UCHAR ucPriority_)
{
     GetCurrent()->Remove(this);
        
     SetCurrent(Scheduler::GetThreadList(m_ucPriority));
    
     GetCurrent()->Add(this);
}

//---------------------------------------------------------------------------
void Thread::SetPriority(K_UCHAR ucPriority_)
{
    K_BOOL bSchedule = 0;

    CS_ENTER();
    // If this is the currently running thread, it's a good idea to reschedule
    // Or, if the new priority is a higher priority than the current thread's.
    if ((g_pstCurrent == this) || (ucPriority_ > g_pstCurrent->GetPriority()))
    {
        bSchedule = 1;
    }
    Scheduler::Remove(this);
    CS_EXIT();

    m_ucCurPriority = ucPriority_;
    m_ucPriority = ucPriority_;
    
    CS_ENTER();    
    Scheduler::Add(this);
    CS_EXIT();
    
    if (bSchedule)
    {
        if (Scheduler::IsEnabled())
        {
            CS_ENTER();
            Scheduler::Schedule();
    #if KERNEL_USE_QUANTUM
            // new thread scheduled.  Stop current quantum timer (if it exists),
            // and restart it for the new thread (if required).
            Quantum::RemoveThread();
            Quantum::AddThread((Thread*)g_pstNext);
    #endif
            CS_EXIT();
            Thread::ContextSwitchSWI();
        }
        else
        {
            Scheduler::QueueScheduler();
        }
    }
}

//---------------------------------------------------------------------------
void Thread::InheritPriority(K_UCHAR ucPriority_)
{    
    SetOwner(Scheduler::GetThreadList(ucPriority_));
    m_ucCurPriority = ucPriority_;
}

//---------------------------------------------------------------------------
void Thread::ContextSwitchSWI()
{
    // Call the context switch interrupt if the scheduler is enabled.
    if (Scheduler::IsEnabled() == 1)
    {        
        KERNEL_TRACE_1( STR_CONTEXT_SWITCH_1, (K_USHORT)((Thread*)g_pstNext)->GetID() );
        KernelSWI::Trigger();
    }
}

#if KERNEL_USE_TIMEOUTS
//---------------------------------------------------------------------------
Timer *Thread::GetTimer()						{ return &m_clTimer; }

//---------------------------------------------------------------------------
void Thread::SetExpired( K_BOOL bExpired_ )		{ m_bExpired = bExpired_; }

//---------------------------------------------------------------------------
K_BOOL Thread::GetExpired()						{ return m_bExpired; }
#endif

#if KERNEL_USE_IDLE_FUNC
//---------------------------------------------------------------------------
void Thread::InitIdle( void )
{
    ClearNode();

    m_ucPriority = 0;
    m_ucCurPriority = 0;
    m_pfEntryPoint = 0;
    m_pvArg = 0;
    m_ucThreadID = 255;
    m_eState = THREAD_STATE_READY;
#if KERNEL_USE_THREADNAME
    m_szName = "IDLE";
#endif
}
#endif
