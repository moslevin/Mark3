#pragma once

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
    \file   condvar.h

    \brief  Condition Variable implementation
*/

#include "mark3cfg.h"
#include "ksemaphore.h"
#include "mutex.h"

namespace Mark3 {

/*!
 * @brief The ConditionVariable class
 * This class implements a condition variable.  This is a synchronization object that
 * allows multiple threads to block, each waiting for specific signals unique to them.
 * Access to the specified condition is guarded by a mutex that is supplied by the
 * caller.  This object can permit multiple waiters that can be unblocked one-at-a-time
 * via signalling, or unblocked all at once via broadcasting.  This object is built upon
 * lower-level primatives, and is somewhat more heavyweight than the primative types
 * supplied by the kernel.
 */
class ConditionVariable {
public:
    void* operator new(size_t sz, void*pv) { return (ConditionVariable*)pv; }

    /*!
     * @brief Init
     * Initialize the condition variable prior to use.  Must be called before the
     * object can be used
     */
    void Init();

    /*!
     * @brief Wait
     * Block the current thread, and wait for the object to be signalled.  The specified
     * mutex will be locked when the thread returns.
     * @param pclMutex_ Mutex to claim once the calling thread has access to the condvar
     */
    void Wait(Mutex* pclMutex_);

    /*!
     * @brief Wait
     * Block the current thread, and wait for the object to be signalled.  The specified
     * mutex will be locked when the thread returns.
     * @param pclMutex_ Mutex to claim once the calling thread has access to the condvar
     * @param u32WaitTimeMS_ Maximum time in ms to wait before abandoning the operation
     * @return true on success, false on timeout
     */
    bool Wait(Mutex* pclMutex_, uint32_t u32WaitTimeMS_);

    /*!
     * @brief Signal
     * Signal/Unblock the next thread currently blocked on this condition variable
     */
    void Signal();

    /*!
     * @brief Broadcast
     * Unblock all threads currently blocked on this condition variable
     */
    void Broadcast();

private:
    Mutex m_clMutex;
    Semaphore m_clSemaphore;
    uint8_t m_u8Waiters;
};
} // namespace Mark3
