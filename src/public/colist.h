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
    @file   colist.h

    @brief  CoRoutine List structure implementation
 */
#pragma once

#include "mark3cfg.h"
#include "coroutine.h"

namespace Mark3 {
class Coroutine;

/**
 * The CoList class implements a circular-linked-listed structure for
 * coroutine objects.  The intent of this object is to maintain a list of
 * active coroutine objects with a specific priority or state, to ensure
 * that a freshly-schedulable co-routine always exists at the head of the
 * list.
 */
class CoList : public TypedCircularLinkList<Coroutine>
{
public:

    /**
     * @brief SetPrioMap
     *
     * Assign a priority map object to this co-routine list
     *
     * @param pclPrioMap_ priority map object to assign
     */
    void SetPrioMap(CoPrioMap* pclPrioMap_);

    /**
     * @brief SetPriority
     *
     * Set the scheduling priority of this coroutine liss; has no effect unless
     * a SetPrioMap has been called with a valid coroutine priority map object.
     *
     * @param uPriority_ Priority of coroutines associated with this list
     */
    void SetPriority(PORT_PRIO_TYPE uPriority_);

    /**
     * @brief Add
     *
     * Add a coroutine object to this list.
     *
     * @param pclCoroutine_ Pointer to the coroutine object to add
     */
    void Add(Coroutine* pclCoroutine_);

    /**
     * @brief Remove
     *
     * Remove a given coroutine object from this list
     *
     * @param pclCoroutine_ Pointer to the coroutine object to remove
     */
    void Remove(Coroutine* pclCoroutine_);

private:
    CoPrioMap* m_pclPrioMap;
    uint8_t m_uPriority;
};
} // namespace Mark3
