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
    @file   coroutine.cpp

    @brief  Coroutine object implementation
 */

#include "coroutine.h"
#include "cosched.h"
#include "criticalguard.h"
#include "kernel.h"

namespace Mark3 {

//---------------------------------------------------------------------------
Coroutine::~Coroutine()
{
    if (m_pclOwner != CoScheduler::GetStopList()) {
        Kernel::Panic(PANIC_ACTIVE_COROUTINE_DESCOPED);
    }

    const auto cs = CriticalGuard{};
    m_pclOwner->Remove(this);
}

//---------------------------------------------------------------------------
void Coroutine::Init(PORT_PRIO_TYPE uPriority_, CoroutineHandler pfHandler_, void* pvContext_)
{
    m_bQueued = false;
    m_pfHandler = pfHandler_;
    m_pvContext = pvContext_;
    m_uPriority = uPriority_;

    m_pclOwner = CoScheduler::GetStopList();

    const auto cs = CriticalGuard{};
    m_pclOwner->Add(this);
}

//---------------------------------------------------------------------------
void Coroutine::Run()
{    
    { // Begin critical section
        const auto cs = CriticalGuard{};
        m_pclOwner->Remove(this);
        m_pclOwner = CoScheduler::GetStopList();
        m_pclOwner->Add(this);
        m_bQueued = false;
    } // end critical section

    m_pfHandler(this, m_pvContext);
}

//---------------------------------------------------------------------------
void Coroutine::Activate()
{
    const auto cs = CriticalGuard{};

    if (m_bQueued) {
        return;
    }

    m_pclOwner->Remove(this);
    m_pclOwner = CoScheduler::GetCoList(m_uPriority);
    m_pclOwner->Add(this);
    m_bQueued = true;
}

//---------------------------------------------------------------------------
void Coroutine::SetPriority(PORT_PRIO_TYPE uPriority_)
{
    const auto cs = CriticalGuard{};

    m_pclOwner->Remove(this);
    m_uPriority = uPriority_;
    if (m_bQueued) {
        m_pclOwner = CoScheduler::GetCoList(m_uPriority);
    } else {
        m_pclOwner = CoScheduler::GetStopList();
    }
    m_pclOwner->Add(this);
}

//---------------------------------------------------------------------------
PORT_PRIO_TYPE Coroutine::GetPriority()
{
    return m_uPriority;
}
} // namespace Mark3
