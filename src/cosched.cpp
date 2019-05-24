/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012 - 2019 m0slevin, all rights reserved.
See license.txt for more information
=========================================================================== */
/**
    @file   cosched.cpp

    @brief  CoRoutine Scheduler implementation
 */

#include "cosched.h"
#include "criticalguard.h"

namespace Mark3 {
CoList CoScheduler::m_aclPriorities[PORT_COROUTINE_PRIORITIES];
CoList CoScheduler::m_clStopList;
CoPrioMap CoScheduler::m_clPrioMap;

//---------------------------------------------------------------------------
void CoScheduler::Init()
{
    m_clStopList.Init();
    for (auto i = 0; i < PORT_COROUTINE_PRIORITIES; i++) {
        m_aclPriorities[i].SetPriority(i);
        m_aclPriorities[i].SetPrioMap(&m_clPrioMap);
    }
}

//---------------------------------------------------------------------------
CoPrioMap* CoScheduler::GetPrioMap()
{
    return &m_clPrioMap;
}

//---------------------------------------------------------------------------
CoList* CoScheduler::GetStopList()
{
    return &m_clStopList;
}

//---------------------------------------------------------------------------
CoList* CoScheduler::GetCoList(PORT_PRIO_TYPE uPriority_)
{
    if (uPriority_ >= PORT_COROUTINE_PRIORITIES) {
        return nullptr;
    }
    return &m_aclPriorities[uPriority_];
}

//---------------------------------------------------------------------------
Coroutine* CoScheduler::Schedule()
{
    const auto cs = CriticalGuard{};

    auto uPriority = m_clPrioMap.HighestPriority();
    if (0 == uPriority) {
        return nullptr;
    }
    uPriority--;
    return m_aclPriorities[uPriority].GetHead();
}
} // namespace Mark3
