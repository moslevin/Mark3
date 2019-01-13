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

#if KERNEL_ROUND_ROBIN
namespace Mark3
{
//---------------------------------------------------------------------------
uint16_t Quantum::m_u16TicksRemain;
Thread*  Quantum::m_pclActiveThread;
Thread*  Quantum::m_pclTimerThread;
bool     Quantum::m_bInTimer;

//---------------------------------------------------------------------------
void Quantum::SetInTimer()
{
    CS_ENTER();
    m_bInTimer = true;

    // Timer is active
    if (m_u16TicksRemain) {
        m_u16TicksRemain--;
    }
    CS_EXIT();
}

//---------------------------------------------------------------------------
void Quantum::ClearInTimer()
{
    CS_ENTER();
    m_bInTimer = false;

    // Timer expired - Pivot the thread list.
    if (m_pclActiveThread && (!m_u16TicksRemain)) {
        auto* pclThreadList = m_pclActiveThread->GetCurrent();
        if (pclThreadList->GetHead() != pclThreadList->GetTail()) {
            pclThreadList->PivotForward();
        }
        m_pclActiveThread = nullptr;
    }
    CS_EXIT();
}

//---------------------------------------------------------------------------
void Quantum::Update(Thread* pclTargetThread_)
{
    // Don't cancel the current RR interval if we're being interrupted by
    // the timer thread, or are in the middle of running the timer thread.
    // OR if the thread list only has one thread
    auto* pclThreadList = pclTargetThread_->GetCurrent();
    if ((pclThreadList->GetHead() == pclThreadList->GetTail()) || (pclTargetThread_ == m_pclTimerThread)
        || (pclTargetThread_ == m_pclActiveThread) || m_bInTimer) {
        return;
    }

    // Update with a new thread and timeout.
    m_pclActiveThread = pclTargetThread_;
    m_u16TicksRemain  = pclTargetThread_->GetQuantum();
}

//---------------------------------------------------------------------------
void Quantum::Cancel()
{
    m_pclActiveThread = nullptr;
    m_u16TicksRemain  = 0;
}
} // namespace Mark3
#endif // #if KERNEL_ROUND_ROBIN
