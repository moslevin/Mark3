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
    @file   cosched.h

    @brief  CoRoutine Scheduler implementation
 */
#pragma once

#include "mark3cfg.h"
#include "coroutine.h"
#include "colist.h"

namespace Mark3 {
/**
 * @brief The CoScheduler class.
 * This class implements the coroutine scheduler.  Similar to the Mark3
 * thread scheduler, the highest-priority active object is scheduled / returned
 * for execution.  If no active co-routines are available to be scheduled, then
 * the scheduler returns nullptr.
 */
class CoScheduler
{
public:

    /**
     * @brief Init
     * Initialize the coroutine scheduler prior to use.  Must be called prior
     * to using any other functions in the coroutine scheduler.
     */
    static void Init();

    /**
     * @brief GetPrioMap
     * Get the pointer to the priority map object used by the scheduler
     *
     * @return Return the priority map object owned by the schedule
     */
    static CoPrioMap* GetPrioMap();

    /**
     * @brief GetStopList
     * Get the pointer to the coroutine list managing initialized coroutines
     * that are not awaiting execution.
     *
     * @return Pointer to the coroutine stop list
     */
    static CoList* GetStopList();

    /**
     * @brief GetCoList
     * Retrieve the coroutine list associated with a given priority.
     *
     * @param uPriority_ Priority to get the coroutine list of.
     * @return coroutine list pointer or nullptr on invalid priority.
     */
    static CoList* GetCoList(PORT_PRIO_TYPE uPriority_);

    /**
     * @brief Schedule
     * Determine what coroutine (if any) is the next to be executed.
     *
     * @return next coroutine to execute, or nullptr if no coroutines are
     * ready to be scheduled.
     */
    static Coroutine* Schedule();

private:
    static CoList m_aclPriorities[PORT_COROUTINE_PRIORITIES];
    static CoList m_clStopList;
    static CoPrioMap m_clPrioMap;
};
} // namespace Mark3
