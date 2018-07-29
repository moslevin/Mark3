
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
    \file   condvar.cpp

    \brief  Condition Variable implementation
*/

#include "mark3.h"

namespace Mark3 {

//---------------------------------------------------------------------------
void ConditionVariable::Init()

{
    m_clMutex.Init();
    m_clSemaphore.Init(0, 255);
}

//---------------------------------------------------------------------------
void ConditionVariable::Wait(Mutex* pclMutex_)
{
    KERNEL_ASSERT(pclMutex_ != nullptr);

    m_clMutex.Claim();

    pclMutex_->Release();
    m_u8Waiters++;

    m_clMutex.Release();

    m_clSemaphore.Pend();
    pclMutex_->Claim();
}

//---------------------------------------------------------------------------
bool ConditionVariable::Wait(Mutex* pclMutex_, uint32_t u32WaitTimeMS_)
{
    KERNEL_ASSERT(pclMutex_ != nullptr);

    m_clMutex.Claim();

    pclMutex_->Release();
    m_u8Waiters++;

    m_clMutex.Release();

    if (!m_clSemaphore.Pend(u32WaitTimeMS_)) {
        return false;
    }
    return pclMutex_->Claim(u32WaitTimeMS_);
}

//---------------------------------------------------------------------------
void ConditionVariable::Signal()
{
    m_clMutex.Claim();
    if (m_u8Waiters) {
        m_u8Waiters--;
        m_clSemaphore.Post();
    }
    m_clMutex.Release();
}

//---------------------------------------------------------------------------
void ConditionVariable::Broadcast()
{
    m_clMutex.Claim();

    while (m_u8Waiters > 0) {
        m_u8Waiters--;
        m_clSemaphore.Post();
    }

    m_clMutex.Release();
}

} // namespace Mark3
