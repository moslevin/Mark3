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
/**
    @file priomap.h
    @brief Priority map data structure
*/

#pragma once

#include "kerneltypes.h"
#include "mark3cfg.h"

//---------------------------------------------------------------------------
// Define the type used to store the priority map based on the word size of
// the underlying host architecture.
#if !defined(PORT_PRIO_MAP_WORD_SIZE)
#error "undefined PORT_PRIO_MAP_WORD_SIZE"
#endif

//---------------------------------------------------------------------------
#if (KERNEL_NUM_PRIORITIES < (PORT_PRIO_MAP_WORD_SIZE * 8u))
// If there is only 1 word required to store the priority information, we don't
// need an array, or a secondary bitmap.
#define PRIO_MAP_MULTI_LEVEL (0)
#else
// An array of bitmaps are required, and a secondary index is required to
// efficiently track which priority levels are active.
#define PRIO_MAP_MULTI_LEVEL (1)
#endif

//---------------------------------------------------------------------------
// # of bits in an integer used to represent the number of bits in the map.
// Used for bitshifting the bit index away from the map index.
// i.e. 3 == 8 bits, 4 == 16 bits, 5 == 32 bits, etc...
#if (PORT_PRIO_MAP_WORD_SIZE == 1)
#define PRIO_MAP_WORD_SHIFT (3u)
#elif (PORT_PRIO_MAP_WORD_SIZE == 2)
#define PRIO_MAP_WORD_SHIFT (4u)
#elif (PORT_PRIO_MAP_WORD_SIZE == 4)
#define PRIO_MAP_WORD_SHIFT (5u)
#elif (PORT_PRIO_MAP_WORD_SIZE == 8)
#define PRIO_MAP_WORD_SHIFT (6u)
#endif

namespace Mark3
{
//---------------------------------------------------------------------------
/**
 * @brief The PriorityMap class
 */
class PriorityMap
{
public:
    /**
     * @brief PriorityMap
     *
     * Initialize the priority map object, clearing the bitamp data to all 0's.
     */
    PriorityMap();

    /**
     * @brief Set       Set the priority map bitmap data, at all levels, for the
     *                  given priority
     * @param uXPrio_   Priority level to set the bitmap data for.
     */
    void Set(PORT_PRIO_TYPE uXPrio_);

    /**
     * @brief Clear     Clear the priority map bitmap data, at all levels, for the
     *                  given priority.
     * @param uXPrio_   Priority level to clear the bitmap data for.
     */
    void Clear(PORT_PRIO_TYPE uXPrio_);

    /**
     * @brief HighestPriority
     *
     * Computes the numeric priority of the highest-priority thread represented in the
     * priority map.
     *
     * @return Highest priority ready-thread's number.
     */
    PORT_PRIO_TYPE HighestPriority(void);

private:
    static inline PORT_PRIO_TYPE PrioBit(PORT_PRIO_TYPE prio) { return prio & m_uXPrioMapBitMask; }

    // function used to get the map index for a given priroity
    static inline PORT_PRIO_TYPE PrioMapWordIndex(PORT_PRIO_TYPE prio) { return prio >> m_uXPrioMapWordShift; }

    static inline PORT_PRIO_TYPE priority_from_bitmap(PORT_PRIO_TYPE uXPrio_)
    {
#if HW_CLZ
        // Support hardware-accelerated Count-leading-zeros instruction
        return m_uXPrioMapBits - CLZ(uXPrio_);
#else
        // Default un-optimized count-leading zeros operation
        PORT_PRIO_TYPE uXMask  = 1 << (m_uXPrioMapBits - 1);
        auto           u8Zeros = PORT_PRIO_TYPE { 0 };

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

    static constexpr auto m_uXPrioMapWordShift = PORT_PRIO_TYPE { PRIO_MAP_WORD_SHIFT };

    // Bitmask used to separate out the priorities first-level bitmap from its
    // second-level map index for a given priority
    static constexpr auto m_uXPortPrioMapWordSize = PORT_PRIO_TYPE { PORT_PRIO_MAP_WORD_SIZE };

    // Size of the map index type in bits
    static constexpr auto m_uXPrioMapBits    = PORT_PRIO_TYPE { 8 * m_uXPortPrioMapWordSize };
    static constexpr auto m_uXPrioMapBitMask = PORT_PRIO_TYPE { (1 << m_uXPrioMapWordShift) - 1 };

    // Required size of the bitmap array in words
    static constexpr auto m_uXPrioMapNumWords
        = PORT_PRIO_TYPE { (KERNEL_NUM_PRIORITIES + (m_uXPrioMapBits - 1)) / m_uXPrioMapBits };

#if PRIO_MAP_MULTI_LEVEL
    PORT_PRIO_TYPE m_auXPriorityMap[m_uXPrioMapNumWords];
    PORT_PRIO_TYPE m_uXPriorityMapL2;
#else
    PORT_PRIO_TYPE m_uXPriorityMap;
#endif
};
} // namespace Mark3
