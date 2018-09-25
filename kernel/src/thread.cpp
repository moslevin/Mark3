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
/**

    @file   thread.cpp

    @brief  Platform-Independent thread class Definition

*/

#include "kerneltypes.h"
#include "mark3cfg.h"

#include "mark3.h"

namespace Mark3
{
//---------------------------------------------------------------------------
Thread::~Thread()
{
    // On destruction of a thread located on a stack,
    // ensure that the thread is either stopped, or exited.
    // If the thread is stopped, move it to the exit state.
    // If not in the exit state, kernel panic -- it's catastrophic to have
    // running threads on stack suddenly disappear.
    if (m_eState == ThreadState::Stop) {
        CS_ENTER();
        m_pclCurrent->Remove(this);
        m_pclCurrent = 0;
        m_pclOwner   = 0;
        m_eState     = ThreadState::Exit;
        CS_EXIT();
    } else if (m_eState != ThreadState::Exit) {
        Kernel::Panic(PANIC_RUNNING_THREAD_DESCOPED);
    }
}

//---------------------------------------------------------------------------
void Thread::Init(
    K_WORD* pwStack_, uint16_t u16StackSize_, PORT_PRIO_TYPE uXPriority_, ThreadEntryFunc pfEntryPoint_, void* pvArg_)
{
    static uint8_t u8ThreadID = 0;

    KERNEL_ASSERT(pwStack_);
    KERNEL_ASSERT(pfEntryPoint_);

    ClearNode();

    m_u8ThreadID = u8ThreadID++;

    // Initialize the thread parameters to their initial values.
    m_pwStack    = pwStack_;
    m_pwStackTop = TOP_OF_STACK(pwStack_, u16StackSize_);

    m_u16StackSize  = u16StackSize_;
    m_uXPriority    = uXPriority_;
    m_uXCurPriority = m_uXPriority;
    m_pfEntryPoint  = pfEntryPoint_;
    m_pvArg         = pvArg_;

#if KERNEL_NAMED_THREADS
    m_szName        = NULL;
#endif
#if KERNEL_ROUND_ROBIN
    m_u16Quantum    = THREAD_QUANTUM_DEFAULT;
#endif

    m_clTimer.Init();

    // Call CPU-specific stack initialization
    ThreadPort::InitStack(this);

    // Add to the global "stop" list.
    CS_ENTER();
    m_pclOwner   = Scheduler::GetThreadList(m_uXPriority);
    m_pclCurrent = Scheduler::GetStopList();
    m_eState     = ThreadState::Stop;
    m_pclCurrent->Add(this);
    CS_EXIT();

#if KERNEL_THREAD_CREATE_HOOK
    ThreadCreateCallout pfCallout = Kernel::GetThreadCreateCallout();
    if (pfCallout != nullptr) {
        pfCallout(this);
    }
#endif
}

//---------------------------------------------------------------------------
Thread* Thread::Init(uint16_t u16StackSize_, PORT_PRIO_TYPE uXPriority_, ThreadEntryFunc pfEntryPoint_, void* pvArg_)
{
    auto* pclNew  = AutoAlloc::NewThread();
    auto* pwStack = static_cast<K_WORD*>(AutoAlloc::NewRawData(u16StackSize_));
    pclNew->Init(pwStack, u16StackSize_, uXPriority_, pfEntryPoint_, pvArg_);
    return pclNew;
}

//---------------------------------------------------------------------------
void Thread::Start(void)
{
    KERNEL_ASSERT(IsInitialized());

    // Remove the thread from the scheduler's "stopped" list, and add it
    // to the scheduler's ready list at the proper priority.

    CS_ENTER();
    Scheduler::GetStopList()->Remove(this);
    Scheduler::Add(this);
    m_pclOwner   = Scheduler::GetThreadList(m_uXPriority);
    m_pclCurrent = m_pclOwner;
    m_eState     = ThreadState::Ready;

#if KERNEL_ROUND_ROBIN
    if (Kernel::IsStarted()) {
        if (GetCurPriority() >= Scheduler::GetCurrentThread()->GetCurPriority()) {
            // Deal with the thread Quantum
            Quantum::Update(this);
        }
    }
#endif

    if (Kernel::IsStarted()) {
        if (GetCurPriority() >= Scheduler::GetCurrentThread()->GetCurPriority()) {
            Thread::Yield();
        }
    }
    CS_EXIT();
}

//---------------------------------------------------------------------------
void Thread::Stop()
{
    KERNEL_ASSERT(IsInitialized());

    auto bReschedule = false;
    if (m_eState == ThreadState::Stop) {
        return;
    }

    CS_ENTER();

    // If a thread is attempting to stop itself, ensure we call the scheduler
    if (this == Scheduler::GetCurrentThread()) {
        bReschedule = true;
#if KERNEL_ROUND_ROBIN
        // Cancel RR scheduling
        Quantum::Cancel();
#endif
    }

    // Add this thread to the stop-list (removing it from active scheduling)
    // Remove the thread from scheduling
    if (m_eState == ThreadState::Ready) {
        Scheduler::Remove(this);
    } else if (m_eState == ThreadState::Blocked) {
        m_pclCurrent->Remove(this);
    }

    m_pclOwner   = Scheduler::GetStopList();
    m_pclCurrent = m_pclOwner;
    m_pclOwner->Add(this);
    m_eState = ThreadState::Stop;

    // Just to be safe - attempt to remove the thread's timer
    // from the timer-scheduler (does no harm if it isn't
    // in the timer-list)
    TimerScheduler::Remove(&m_clTimer);

    CS_EXIT();

    if (bReschedule) {
        Thread::Yield();
    }
}

//---------------------------------------------------------------------------
void Thread::Exit()
{
    KERNEL_ASSERT(IsInitialized());

    bool bReschedule = false;

    if (m_eState == ThreadState::Exit) {
        return;
    }

    CS_ENTER();

    // If this thread is the actively-running thread, make sure we run the
    // scheduler again.
    if (this == Scheduler::GetCurrentThread()) {
        bReschedule = true;
#if KERNEL_ROUND_ROBIN
        // Cancel RR scheduling
        Quantum::Cancel();
#endif
    }

    // Remove the thread from scheduling
    if (m_eState == ThreadState::Ready) {
        Scheduler::Remove(this);
    } else if ((m_eState == ThreadState::Blocked) || (m_eState == ThreadState::Stop)) {
        m_pclCurrent->Remove(this);
    }

    m_pclCurrent = 0;
    m_pclOwner   = 0;
    m_eState     = ThreadState::Exit;

    // We've removed the thread from scheduling, but interrupts might
    // trigger checks against this thread's currently priority before
    // we get around to scheduling new threads.  As a result, set the
    // priority to idle to ensure that we always wind up scheduling
    // new threads.
    m_uXCurPriority = 0;
    m_uXPriority    = 0;

    // Just to be safe - attempt to remove the thread's timer
    // from the timer-scheduler (does no harm if it isn't
    // in the timer-list)
    TimerScheduler::Remove(&m_clTimer);
    CS_EXIT();

#if KERNEL_THREAD_EXIT_HOOK
    ThreadExitCallout pfCallout = Kernel::GetThreadExitCallout();
    if (pfCallout != nullptr) {
        pfCallout(this);
    }
#endif

    if (bReschedule) {
        // Choose a new "next" thread if we must
        Thread::Yield();
    }
}

//---------------------------------------------------------------------------
void Thread::Sleep(uint32_t u32TimeMs_)
{
    Semaphore clSemaphore;
    auto*     pclTimer       = g_pclCurrent->GetTimer();
    auto      lTimerCallback = [](Thread* /*pclOwner*/, void* pvData_) {
        auto* pclSemaphore = static_cast<Semaphore*>(pvData_);
        pclSemaphore->Post();
    };

    // Create a semaphore that this thread will block on
    clSemaphore.Init(0, 1);

    // Create a one-shot timer that will call a callback that posts the
    // semaphore, waking our thread.
    pclTimer->Init();
    pclTimer->SetIntervalMSeconds(u32TimeMs_);
    pclTimer->SetCallback(lTimerCallback);
    pclTimer->SetData((void*)&clSemaphore);
    pclTimer->SetFlags(TIMERLIST_FLAG_ONE_SHOT);

    // Add the new timer to the timer scheduler, and block the thread
    TimerScheduler::Add(pclTimer);
    clSemaphore.Pend();
}

#if KERNEL_STACK_CHECK
//---------------------------------------------------------------------------
uint16_t Thread::GetStackSlack()
{
    KERNEL_ASSERT(IsInitialized());

    K_ADDR wBottom = 0;
    auto   wTop    = static_cast<K_ADDR>(m_u16StackSize - 1);
    auto   wMid    = ((wTop + wBottom) + 1) / 2;

    CS_ENTER();

    // Logarithmic bisection - find the point where the contents of the
    // stack go from 0xFF's to non 0xFF.  Not Definitive, but accurate enough
    while ((wTop - wBottom) > 1) {
#if STACK_GROWS_DOWN
        if (m_pwStack[wMid] != (K_WORD)(-1))
#else
        if (m_pwStack[wMid] == (K_WORD)(-1))
#endif
        {
            //! ToDo : Reverse the logic for MCUs where stack grows UP instead of down
            wTop = wMid;
        } else {
            wBottom = wMid;
        }
        wMid = (wTop + wBottom + 1) / 2;
    }

    CS_EXIT();

    return wMid;
}
#endif

//---------------------------------------------------------------------------
void Thread::Yield()
{
    CS_ENTER();
    // Run the scheduler
    if (Scheduler::IsEnabled()) {
        Scheduler::Schedule();

        // Only switch contexts if the new task is different than the old task
        if (g_pclCurrent != g_pclNext) {
#if KERNEL_ROUND_ROBIN
            Quantum::Update((Thread*)g_pclNext);
#endif
            Thread::ContextSwitchSWI();
        }
    } else {
        Scheduler::QueueScheduler();
    }
    CS_EXIT();
}

//---------------------------------------------------------------------------
void Thread::CoopYield(void)
{
    g_pclCurrent->GetCurrent()->PivotForward();
    Yield();
}

//---------------------------------------------------------------------------
void Thread::SetPriorityBase(PORT_PRIO_TYPE /*uXPriority_*/)
{
    KERNEL_ASSERT(IsInitialized());

    GetCurrent()->Remove(this);
    SetCurrent(Scheduler::GetThreadList(m_uXPriority));
    GetCurrent()->Add(this);
}

//---------------------------------------------------------------------------
void Thread::SetPriority(PORT_PRIO_TYPE uXPriority_)
{
    KERNEL_ASSERT(IsInitialized());
    auto bSchedule = false;

    CS_ENTER();

    // If this is the currently running thread, it's a good idea to reschedule
    // Or, if the new priority is a higher priority than the current thread's.
    if ((g_pclCurrent == this) || (uXPriority_ > g_pclCurrent->GetPriority())) {
        bSchedule = true;
#if KERNEL_ROUND_ROBIN
        Quantum::Cancel();
#endif
    }
    Scheduler::Remove(this);
    CS_EXIT();

    m_uXCurPriority = uXPriority_;
    m_uXPriority    = uXPriority_;

    CS_ENTER();
    Scheduler::Add(this);
    CS_EXIT();

    if (bSchedule) {
        if (Scheduler::IsEnabled()) {
            CS_ENTER();            
            Scheduler::Schedule();
#if KERNEL_ROUND_ROBIN
            Quantum::Update((Thread*)g_pclNext);
#endif
            CS_EXIT();
            Thread::ContextSwitchSWI();
        } else {
            Scheduler::QueueScheduler();
        }
    }
}

//---------------------------------------------------------------------------
void Thread::InheritPriority(PORT_PRIO_TYPE uXPriority_)
{
    KERNEL_ASSERT(IsInitialized());

    SetOwner(Scheduler::GetThreadList(uXPriority_));
    m_uXCurPriority = uXPriority_;
}

//---------------------------------------------------------------------------
void Thread::ContextSwitchSWI()
{
    // Call the context switch interrupt if the scheduler is enabled.
    if (Scheduler::IsEnabled()) {
#if KERNEL_STACK_CHECK
        if (g_pclCurrent && (g_pclCurrent->GetStackSlack() <= Kernel::GetStackGuardThreshold())) {
            Kernel::Panic(PANIC_STACK_SLACK_VIOLATED);
        }
#endif
#if KERNEL_CONTEXT_SWITCH_CALLOUT
        auto pfCallout = Kernel::GetThreadContextSwitchCallout();
        if (pfCallout != nullptr) {
            pfCallout(g_pclCurrent);
        }
#endif
        KernelSWI::Trigger();
    }
}

//---------------------------------------------------------------------------
Timer* Thread::GetTimer()
{
    KERNEL_ASSERT(IsInitialized());
    return &m_clTimer;
}
//---------------------------------------------------------------------------
void Thread::SetExpired(bool bExpired_)
{
    KERNEL_ASSERT(IsInitialized());
    m_bExpired = bExpired_;
}

//---------------------------------------------------------------------------
bool Thread::GetExpired()
{
    KERNEL_ASSERT(IsInitialized());
    return m_bExpired;
}
} // namespace Mark3
