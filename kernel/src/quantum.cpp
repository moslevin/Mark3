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

    @file   quantum.cpp

    @brief  Thread Quantum Implementation for Round-Robin Scheduling

*/

#include "mark3.h"
namespace Mark3
{
namespace
{
    volatile bool bAddQuantumTimer; // Indicates that a timer add is pending
} // anonymous namespace

Thread* Quantum::m_pclTimerThread;
Thread* Quantum::m_pclActiveThread;

Timer Quantum::m_clQuantumTimer;
bool  Quantum::m_bActive;
bool  Quantum::m_bInTimer;

//---------------------------------------------------------------------------
void Quantum::SetTimer(Thread* pclThread_)
{
    KERNEL_ASSERT(pclThread_ != nullptr);

    auto lQuantumCallback = [](Thread* pclThread_, void* /*pvData_*/) {
        if (pclThread_->GetCurrent()->GetHead() != pclThread_->GetCurrent()->GetTail()) {
            bAddQuantumTimer = true;
            pclThread_->GetCurrent()->PivotForward();
        }
    };

    m_pclActiveThread = pclThread_;
    m_clQuantumTimer.SetIntervalMSeconds(pclThread_->GetQuantum());
    m_clQuantumTimer.SetFlags(TIMERLIST_FLAG_ONE_SHOT);
    m_clQuantumTimer.SetData(NULL);
    m_clQuantumTimer.SetCallback(lQuantumCallback);
    m_clQuantumTimer.SetOwner(pclThread_);
}

//---------------------------------------------------------------------------
void Quantum::AddThread(Thread* pclThread_)
{
    KERNEL_ASSERT(pclThread_ != nullptr);

    if (m_bActive) {
        return;
    }

    // If this is called from the timer callback, queue a timer add, and defer
    // until later.
    if (m_bInTimer) {
        bAddQuantumTimer = true;
        return;
    }

    // If this isn't the only thread in the list.
    if (pclThread_->GetCurrent()->GetHead() != pclThread_->GetCurrent()->GetTail()) {
        m_clQuantumTimer.Init();
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
    m_bActive         = false;
    m_pclActiveThread = nullptr;
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
void Quantum::SetTimerThread(Thread* pclThread_)
{
    KERNEL_ASSERT(pclThread_ != nullptr);

    m_pclTimerThread = pclThread_;
}

//---------------------------------------------------------------------------
Thread* Quantum::GetTimerThread()
{
    return m_pclTimerThread;
}
} // namespace Mark3
