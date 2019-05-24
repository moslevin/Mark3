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

    @file   schedulerguard.h

    @brief  RAII Scheduler Locking
*/

#pragma once

#include "mark3cfg.h"
#include "scheduler.h"

namespace Mark3 {
/**
 * The SchedulerGuard class implements RAII-based control of the scheduler's
 * global state.  Upon object construction, the state of the scheduler is
 * cached before the scheduler is disabled.
 *
 * Upon object destruction, the scheduler's previous state is restored.
 *
 * This object is interrupt-safe, although it has no effect when called from
 * an interrupt, given that interrupts are inherently higher-priority than
 * threads.
 */
class SchedulerGuard {
public:
    SchedulerGuard()
    {
        m_bSchedState = Scheduler::SetScheduler(false);
    }

    ~SchedulerGuard()
    {
        Scheduler::SetScheduler(m_bSchedState);
    }

private:
    bool m_bSchedState;
};

} // namespace Mark3
