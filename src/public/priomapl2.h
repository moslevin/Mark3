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

    @file   priomapl2.h

    @brief  2-Level priority allocator template-class used for scheduler implementation
*/
#pragma once

#include "kerneltypes.h"
#include "mark3cfg.h"
#include "threadport.h"

namespace Mark3
{
//---------------------------------------------------------------------------
/**
 * @brief The PriorityMapL2 class
 */
template <typename T, size_t C>
class PriorityMapL2
{
public:
    /**
     * @brief PriorityMap
     *
     * Initialize the priority map object, clearing the bitamp data to all 0's.
     */
    PriorityMapL2()
    {
        m_uXPriorityMapL2 = 0;
        for (auto i = PORT_PRIO_TYPE{0}; i < m_uXPrioMapNumWords; i++) { m_auXPriorityMap[i] = 0; }
    }

    /**
     * @brief Set       Set the priority map bitmap data, at all levels, for the
     *                  given priority
     * @param uXPrio_   Priority level to set the bitmap data for.
     */
    void Set(T uXPrio_)
    {
        auto uXPrioBit = PrioBit(uXPrio_);
        auto uXWordIdx = PrioMapWordIndex(uXPrio_);

        m_auXPriorityMap[uXWordIdx] |= (1 << uXPrioBit);
        m_uXPriorityMapL2 |= (1 << uXWordIdx);
    }

    /**
     * @brief Clear     Clear the priority map bitmap data, at all levels, for the
     *                  given priority.
     * @param uXPrio_   Priority level to clear the bitmap data for.
     */
    void Clear(T uXPrio_)
    {
        auto uXPrioBit = PrioBit(uXPrio_);
        auto uXWordIdx = PrioMapWordIndex(uXPrio_);

        m_auXPriorityMap[uXWordIdx] &= ~(1 << uXPrioBit);
        if (!m_auXPriorityMap[uXWordIdx]) {
            m_uXPriorityMapL2 &= ~(1 << uXWordIdx);
        }
    }

    /**
     * @brief HighestPriority
     *
     * Computes the numeric priority of the highest-priority thread represented in the
     * priority map.
     *
     * @return Highest priority ready-thread's number.
     */
    T HighestPriority(void)
    {
        auto uXMapIdx = PriorityFromBitmap(m_uXPriorityMapL2);
        if (!uXMapIdx) {
            return 0;
        }
        uXMapIdx--;
        auto uXPrio = PriorityFromBitmap(m_auXPriorityMap[uXMapIdx]);
        uXPrio += (uXMapIdx * m_uXPrioMapBits);
        return uXPrio;
    }

private:
    static inline T PrioBit(T prio) { return prio & m_uXPrioMapBitMask; }

    static inline T PrioMapWordIndex(T prio) { return prio >> m_uXPrioMapWordShift; }

    static inline T PriorityFromBitmap(T uXPrio_)
    {
#if PORT_USE_HW_CLZ
        // Support hardware-accelerated Count-leading-zeros instruction
        return m_uXPrioMapBits - PORT_CLZ(uXPrio_);
#else
        // Default un-optimized count-leading zeros operation
        T uXMask  = 1 << (m_uXPrioMapBits - 1);
        auto           u8Zeros = T { 0 };

        while (uXMask) {
            if (uXMask & uXPrio_) {
                return (m_uXPrioMapBits - u8Zeros);
            }

            uXMask >>= 1;
            u8Zeros++;
        }
        return 0;
#endif
    }

    static constexpr size_t m_uXPrioMapShiftLUT[9] = {0, 3, 4, 0, 5, 0, 0, 0, 6};
    static constexpr auto m_uXPrioMapWordShift = T { m_uXPrioMapShiftLUT[sizeof(T)] };
    static constexpr auto m_uXPrioMapBits    = T { 8 * sizeof(T) };
    static constexpr auto m_uXPrioMapBitMask = T { (1 << m_uXPrioMapWordShift) - 1 };

    // Required size of the bitmap array in words
    static constexpr auto m_uXPrioMapNumWords
        = T { (C + (m_uXPrioMapBits - 1)) / m_uXPrioMapBits };

    T m_auXPriorityMap[m_uXPrioMapNumWords];
    T m_uXPriorityMapL2;
};
} // namespace Mark3
