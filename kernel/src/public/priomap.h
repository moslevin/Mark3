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
    \file priomap.h
    \brief Priority map data structure
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

#define PRIO_MAP_WORD_TYPE K_WORD

// Size of the map index type in bits
#define PRIO_MAP_BITS (8 * PORT_PRIO_MAP_WORD_SIZE)

// # of bits in an integer used to represent the number of bits in the map.
// Used for bitshifting the bit index away from the map index.
// i.e. 3 == 8 bits, 4 == 16 bits, 5 == 32 bits, etc...
#define PRIO_MAP_WORD_SHIFT (2 + PORT_PRIO_MAP_WORD_SIZE)

// Bitmask used to separate out the priorities first-level bitmap from its
// second-level map index for a given priority
#define PRIO_MAP_BIT_MASK ((1 << PRIO_MAP_WORD_SHIFT) - 1)

// Get the priority bit for a given thread
#define PRIO_BIT(x) ((x)&PRIO_MAP_BIT_MASK)

// Macro used to get the map index for a given priroity
#define PRIO_MAP_WORD_INDEX(prio) ((prio) >> PRIO_MAP_WORD_SHIFT)

// Required size of the bitmap array in words
#define PRIO_MAP_NUM_WORDS ((KERNEL_NUM_PRIORITIES + (PRIO_MAP_BITS - 1)) / (PRIO_MAP_BITS))

//---------------------------------------------------------------------------
#if (PRIO_MAP_NUM_WORDS == 1)
// If there is only 1 word required to store the priority information, we don't
// need an array, or a secondary bitmap.
#define PRIO_MAP_MULTI_LEVEL (0)
#else
// An array of bitmaps are required, and a secondary index is required to
// efficiently track which priority levels are active.
#define PRIO_MAP_MULTI_LEVEL (1)
#endif
namespace Mark3
{
//---------------------------------------------------------------------------
/*!
 * \brief The PriorityMap class
 */
class PriorityMap
{
public:
    /*!
     * \brief PriorityMap
     *
     * Initialize the priority map object, clearing the bitamp data to all 0's.
     */
    PriorityMap();

    /*!
     * \brief Set       Set the priority map bitmap data, at all levels, for the
     *                  given priority
     * \param uXPrio_   Priority level to set the bitmap data for.
     */
    void Set(PORT_PRIO_TYPE uXPrio_);

    /*!
     * \brief Clear     Clear the priority map bitmap data, at all levels, for the
     *                  given priority.
     * \param uXPrio_   Priority level to clear the bitmap data for.
     */
    void Clear(PORT_PRIO_TYPE uXPrio_);

    /*!
     * \brief HighestPriority
     *
     * Computes the numeric priority of the highest-priority thread represented in the
     * priority map.
     *
     * \return Highest priority ready-thread's number.
     */
    PORT_PRIO_TYPE HighestPriority(void);

private:
    PRIO_MAP_WORD_TYPE m_auXPriorityMap[PRIO_MAP_NUM_WORDS];
    PRIO_MAP_WORD_TYPE m_uXPriorityMapL2;
};
} //namespace Mark3
