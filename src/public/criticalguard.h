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
===========================================================================*/
/**

    @file   criticalguard.h

    @brief  RAII Critical Section Implementation
*/
#pragma once

#include "mark3cfg.h"
#include "criticalsection.h"

namespace Mark3 {

//---------------------------------------------------------------------------
/**
 * @brief The CriticalGuard class.
 * This class provides an implemention of RAII for critical sections.  Object
 * creation results in a critical section being invoked.  The subsequent
 * destructor call results in the critical section being released.
 *
 * This is similar to the LockGuard class, except that class operates on a
 * single Mutex, and this class operates on a global interrupt-disabled
 * lock.
 */
class CriticalGuard {
public:
    CriticalGuard() {
        CriticalSection::Enter();
    }

    ~CriticalGuard() {
        CriticalSection::Exit();
    }
};
} // namespace Mark3
