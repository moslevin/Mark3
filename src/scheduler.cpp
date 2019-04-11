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

    @file   scheduler.cpp

    @brief  Strict-Priority + Round-Robin thread scheduler implementation

*/

#include "mark3.h"

Mark3::Thread* g_pclNext;
Mark3::Thread* g_pclCurrent;

namespace Mark3
{
bool        Scheduler::m_bEnabled;
bool        Scheduler::m_bQueuedSchedule;
ThreadList  Scheduler::m_clStopList;
ThreadList  Scheduler::m_aclPriorities[KERNEL_NUM_PRIORITIES];
PriorityMap Scheduler::m_clPrioMap;

//---------------------------------------------------------------------------
void Scheduler::Init()
{
    for (size_t i = 0; i < m_uNumPriorities; i++) {
        m_aclPriorities[i].SetPriority(i);
        m_aclPriorities[i].SetMapPointer(&m_clPrioMap);
    }
}

//---------------------------------------------------------------------------
void Scheduler::Schedule()
{
    auto uXPrio = m_clPrioMap.HighestPriority();
    if (0 == uXPrio) {
        Kernel::Panic(PANIC_NO_READY_THREADS);
    }
    // Priorities are one-indexed
    uXPrio--;

    // Get the thread node at this priority.
    g_pclNext = static_cast<Thread*>(m_aclPriorities[uXPrio].GetHead());
}

//---------------------------------------------------------------------------
void Scheduler::Add(Thread* pclThread_)
{
    KERNEL_ASSERT(pclThread_ != nullptr);

    m_aclPriorities[pclThread_->GetPriority()].Add(pclThread_);
}

//---------------------------------------------------------------------------
void Scheduler::Remove(Thread* pclThread_)
{
    KERNEL_ASSERT(pclThread_ != nullptr);

    m_aclPriorities[pclThread_->GetPriority()].Remove(pclThread_);
}

//---------------------------------------------------------------------------
bool Scheduler::SetScheduler(bool bEnable_)
{
    auto bRet = bool{false};
    CS_ENTER();
    bRet       = m_bEnabled;
    m_bEnabled = bEnable_;
    // If there was a queued scheduler evevent, dequeue and trigger an
    // immediate Yield
    if (m_bEnabled && m_bQueuedSchedule) {
        m_bQueuedSchedule = false;
        Thread::Yield();
    }
    CS_EXIT();
    return bRet;
}
} // namespace Mark3
