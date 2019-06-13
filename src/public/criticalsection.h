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

    @file   criticalsection.h

    @brief  Critical Section Support
*/
#pragma once

#include "mark3cfg.h"
#include "threadport.h"

namespace Mark3 {

//---------------------------------------------------------------------------
/**
 * @brief The CriticalSection class.
 * This class implements a portable CriticalSection interface
 * based on macros/inline functions that are implemented as part of each port.
 *
 * Critical sections *can* be safely nested, that is, multiple calls
 * to Enter() may be called before matching calls to Exit() are called.  In
 * such cases, only the *final* call to Exit() will cause the caller to
 * relinquish control of the CPU.
 *
 * Critical sections can be safely used within interrupts; although they have
 * no effect in the general case.  The exception is in the case where a system
 * has multiple levels of interrupt nesting; at which point calling a critical
 * section from an interrupt temporarily disables subsequent levels of
 * nesting.
 *
 * Care, however, must be taken to ensure that the currently executing thread
 * does *not* block when a critical section is active.  This condition is
 * guaranteed to break the system.
 */
class CriticalSection {
public:

    /**
     * @brief Enter
     * Enter a critical section, disabling all kernel-aware interrupts, and
     * giving exclusive control of the CPU to the curreninttly running task.
     */
    static inline void Enter() {
        PORT_CS_ENTER();
    }

    /**
     * @brief Exit
     * Exit a critical section, re-enabling kernel-aware interrupts, and
     * releasing exclusive control of the CPU.
     */
    static inline void Exit() {
        PORT_CS_EXIT();
    }

    /**
     * @brief NestingCount
     * @return Number of Enter() calls awaiting an Exit() call.
     */
    static inline K_WORD NestingCount() {
        return PORT_CS_NESTING();
    }
};

} // namespace Mark3
