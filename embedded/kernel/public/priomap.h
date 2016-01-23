/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012-2016 Funkenstein Software Consulting, all rights reserved.
See license.txt for more information
===========================================================================*/
/*!
    \file priomap.h
    \brief Priority map data structure
*/

#ifndef __PRIOMAP_H__
#define __PRIOMAP_H__

#include "kerneltypes.h"
#include "mark3cfg.h"

//---------------------------------------------------------------------------
// Optimize the types used for storing priorities based on number of
// priorities defined.
#if (KERNEL_NUM_PRIORITIES <= 64)
# define PRIO_TYPE           uint8_t
#elif (KERNEL_NUM_PRIORITIES <= 256)
# define PRIO_TYPE           uint16_t
#elif (KERNEL_NUM_PRIORITIES <= 1024)
# define PRIO_TYPE           uint32_t
#else
# error "Mark3 supports a maximum of 1024 priorities"
#endif

//---------------------------------------------------------------------------
// Define the type used to store the priority map based on the word size of
// the underlying host architecture.
#if (K_WORD == uint8_t)
# define PRIO_MAP_WORD_SIZE  (1)
#elif (K_WORD == uint16_t)
# define PRIO_MAP_WORD_SIZE  (2)
#elif (K_WORD == uint32_t)
# define PRIO_MAP_WORD_SIZE  (4)
#endif
#define PRIO_MAP_WORD_TYPE  K_WORD

// Size of the map index type in bits
#define PRIO_MAP_BITS                (8 * PRIO_MAP_WORD_SIZE)

// # of bits in an integer used to represent the number of bits in the map.
// Used for bitshifting the bit index away from the map index.
// i.e. 3 == 8 bits, 4 == 16 bits, 5 == 32 bits, etc...
#define PRIO_MAP_WORD_SHIFT          (2 + PRIO_MAP_WORD_SIZE)

// Bitmask used to separate out the priorities first-level bitmap from its
// second-level map index for a given priority
#define PRIO_MAP_BIT_MASK            ((1 << PRIO_MAP_WORD_SHIFT) - 1)

// Get the priority bit for a given thread
#define PRIO_BIT(x)                  ((x) & PRIO_MAP_BIT_MASK)

// Macro used to get the map index for a given priroity
#define PRIO_MAP_WORD_INDEX(prio)    ((prio) >> PRIO_MAP_WORD_SHIFT)

// Required size of the bitmap array in words
#define PRIO_MAP_NUM_WORDS           ((KERNEL_NUM_PRIORITIES + (PRIO_MAP_BITS - 1)) / (PRIO_MAP_BITS))

//---------------------------------------------------------------------------
#if (PRIO_MAP_NUM_WORDS == 1)
// If there is only 1 word required to store the priority information, we don't
// need an array, or a secondary bitmap.
# define PRIO_MAP_MULTI_LEVEL          (0)
#else
// An array of bitmaps are required, and a secondary index is required to
// efficiently track which priority levels are active.
# define PRIO_MAP_MULTI_LEVEL          (1)
#endif

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
    void Set( PRIO_TYPE uXPrio_ );

    /*!
     * \brief Clear     Clear the priority map bitmap data, at all levels, for the
     *                  given priority.
     * \param uXPrio_   Priority level to clear the bitmap data for.
     */
    void Clear( PRIO_TYPE uXPrio_ );

    /*!
     * \brief HighestPriority
     *
     * Computes the numeric priority of the highest-priority thread represented in the
     * priority map.
     *
     * \return Highest priority ready-thread's number.
     */
    PRIO_TYPE HighestPriority( void );

private:
#if PRIO_MAP_MULTI_LEVEL
    PRIO_MAP_WORD_TYPE     m_auXPriorityMap[PRIO_MAP_NUM_WORDS];
    PRIO_MAP_WORD_TYPE     m_uXPriorityMapL2;
#else
    PRIO_MAP_WORD_TYPE     m_uXPriorityMap;
#endif
};

#endif
