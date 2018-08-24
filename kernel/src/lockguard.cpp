/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2018 m0slevin, all rights reserved.
See license.txt for more information
===========================================================================*/
/**

    @file   lockguard.cpp

    @brief  Mutex RAII helper class
*/
#include "mark3.h"

namespace Mark3
{
LockGuard::LockGuard(Mutex* pclMutex_)
    : m_bIsAcquired{ true }
    , m_pclMutex{ pclMutex_ }
{
    KERNEL_ASSERT(m_pclMutex != nullptr);
    m_pclMutex->Claim();
}

LockGuard::LockGuard(Mutex* pclMutex_, uint32_t u32TimeoutMs_)
    : m_pclMutex{ pclMutex_ }
{
    KERNEL_ASSERT(pclMutex_ != nullptr);
    m_bIsAcquired = m_pclMutex->Claim(u32TimeoutMs_);
}

LockGuard::~LockGuard()
{
    if (m_bIsAcquired) {
        m_pclMutex->Release();
    }
}

} // namespace Mark3
