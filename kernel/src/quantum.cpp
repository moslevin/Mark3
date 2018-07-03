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
/*!

    \file   quantum.cpp

    \brief  Thread Quantum Implementation for Round-Robin Scheduling

*/

#include "kerneltypes.h"
#include "mark3cfg.h"

#include "thread.h"
#include "timerlist.h"
#include "quantum.h"
#include "kernelaware.h"

#define _CAN_HAS_DEBUG
//--[Autogenerated - Do Not Modify]------------------------------------------
#include "dbg_file_list.h"
#include "buffalogger.h"
#if defined(DBG_FILE)
#error "Debug logging file token already defined!  Bailing."
#else
#define DBG_FILE _DBG___KERNEL_QUANTUM_CPP
#endif
//--[End Autogenerated content]----------------------------------------------
#include "kerneldebug.h"

#if KERNEL_USE_QUANTUM
namespace Mark3
{
namespace
{
volatile bool bAddQuantumTimer; // Indicates that a timer add is pending
} // anonymous namespace

#if KERNEL_TIMERS_THREADED
Thread* Quantum::m_pclTimerThread;
#endif

Timer Quantum::m_clQuantumTimer;
bool  Quantum::m_bActive;
bool  Quantum::m_bInTimer;

//---------------------------------------------------------------------------
void Quantum::SetTimer(Thread* pclThread_)
{
    auto lQuantumCallback = [](Thread* pclThread_, void*  /*pvData_*/) {
        if (pclThread_->GetCurrent()->GetHead() != pclThread_->GetCurrent()->GetTail()) {
            bAddQuantumTimer = true;
            pclThread_->GetCurrent()->PivotForward();
        }
    };

    m_clQuantumTimer.SetIntervalMSeconds(pclThread_->GetQuantum());
    m_clQuantumTimer.SetFlags(TIMERLIST_FLAG_ONE_SHOT);
    m_clQuantumTimer.SetData(NULL);
    m_clQuantumTimer.SetCallback(lQuantumCallback);
    m_clQuantumTimer.SetOwner(pclThread_);
}

//---------------------------------------------------------------------------
void Quantum::AddThread(Thread* pclThread_)
{
    if (m_bActive
#if KERNEL_USE_IDLE_FUNC
        || (pclThread_ == Kernel::GetIdleThread())
#endif
            ) {
        return;
    }

    // If this is called from the timer callback, queue a timer add...
    if (m_bInTimer) {
        bAddQuantumTimer = true;
        return;
    }

    // If this isn't the only thread in the list.
    if (pclThread_->GetCurrent()->GetHead() != pclThread_->GetCurrent()->GetTail()) {
#if KERNEL_EXTRA_CHECKS
        m_clQuantumTimer.Init();
#endif
        Quantum::SetTimer(pclThread_);
        TimerScheduler::Add(&m_clQuantumTimer);
        m_bActive = true;
    }
}

//---------------------------------------------------------------------------
void Quantum::RemoveThread(void)
{
    if (!m_bActive) {
        return;
    }

    // Cancel the current timer
    TimerScheduler::Remove(&m_clQuantumTimer);
    m_bActive = false;
}

//---------------------------------------------------------------------------
void Quantum::UpdateTimer(void)
{
    // If we have to re-add the quantum timer (more than 2 threads at the
    // high-priority level...)
    if (bAddQuantumTimer) {
        // Trigger a thread yield - this will also re-schedule the
        // thread *and* reset the round-robin scheduler.
        Thread::Yield();
        bAddQuantumTimer = false;
    }
}

//---------------------------------------------------------------------------
#if KERNEL_TIMERS_THREADED
void Quantum::SetTimerThread(Thread* pclThread_)
{
    m_pclTimerThread = pclThread_;
}

Thread* Quantum::GetTimerThread()
{
    return m_pclTimerThread;
}

#endif // KERNEL_TIMERS_THREADED
} //namespace Mark3
#endif // KERNEL_USE_QUANTUM
