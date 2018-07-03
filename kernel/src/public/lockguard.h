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
/*!

    \file   lockguard.h

    \brief  Mutex RAII helper class
*/
#pragma once

#include "mark3.h"

#if KERNEL_USE_MUTEX

namespace Mark3 {

/**
 * Implement RAII-style locks based on Mark3's kernel Mutex object.  Note that
 * Mark3 does not support exceptions, so
 */
class LockGuard {
public:
    /**
     * @param pclMutex mutex to lock during construction
     */
    LockGuard(Mutex* pclMutex);

#if KERNEL_USE_TIMEOUTS

    /**
     * @param pclMutex mutex to lock during construction
     * @param u32TimeoutMs_ timeout (in ms) to wait before bailng
     */
    LockGuard(Mutex* pclMutex, uint32_t u32TimeoutMs_);
#endif

    ~LockGuard();

    /**
     * Verify that lock was correctly initialized and locked during acquisition.
     * This is used to provide error-checking for timed RAII locks, where Mark3
     * does not use exceptions, and a kernel-panic is too heavy-handed.
     * @return true if the lock was initialed correctly, false on error
     */
    bool isAcquired() { return m_bIsAcquired; }

private:
    bool m_bIsAcquired;
    Mutex* m_pclMutex;
};
} // namespace Mark3

#endif // KERNEL_USE_MUTEX
