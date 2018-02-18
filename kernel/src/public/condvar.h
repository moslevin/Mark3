#pragma once

/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012 - 2018 Funkenstein Software Consulting, all rights reserved.
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
 */
class ConditionVariable {
public:
    void* operator new(size_t sz, void*pv) { return (ConditionVariable*)pv; }
    ~ConditionVariable();

    /*!
     * @brief Init
     */
    void Init();

    /*!
     * @brief Wait
     * @param pclMutex_
     */
    void Wait(Mutex* pclMutex_);

#if KERNEL_USE_TIMEOUTS
    /*!
     * @brief Wait
     * @param pclMutex_
     * @param u32WaitTimeMS_
     * @return
     */
    bool Wait(Mutex* pclMutex_, uint32_t u32WaitTimeMS_);
#endif

    /*!
     * @brief Signal
     */
    void Signal();

    /*!
     * @brief Broadcast
     */
    void Broadcast();

private:
    Mutex m_clMutex;
    Semaphore m_clSemaphore;
    uint8_t m_u8Waiters;
};


} // namespace Mark3
