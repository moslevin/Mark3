/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012 - 2017 Funkenstein Software Consulting, all rights reserved.
See license.txt for more information
===========================================================================*/
/*!
    \file priomap.cpp
    \brief Priority map data structure
*/

#include "mark3.h"
#include "priomap.h"
#include "threadport.h"

#include <stdint.h>
#include <stdbool.h>

//---------------------------------------------------------------------------
static inline uint8_t priority_from_bitmap(PORT_PRIO_TYPE uXPrio_)
{
#if HW_CLZ
    // Support hardware-accelerated Count-leading-zeros instruction
    uint8_t rc = PRIO_MAP_BITS - CLZ(uXPrio_);
    return rc;
#else
    // Default un-optimized count-leading zeros operation
    PORT_PRIO_TYPE uXMask  = (1 << (PRIO_MAP_BITS - 1));
    uint8_t   u8Zeros = 0;

    while (uXMask) {
        if (uXMask & uXPrio_) {
            return (PRIO_MAP_BITS - u8Zeros);
        }

        uXMask >>= 1;
        u8Zeros++;
    }
    return 0;
#endif
}

//---------------------------------------------------------------------------
PriorityMap::PriorityMap()
{
#if PRIO_MAP_MULTI_LEVEL
    m_uXPriorityMapL2 = 0;
    for (int i = 0; i < PRIO_MAP_NUM_WORDS; i++) {
        m_auXPriorityMap[i] = 0;
    }
#else
    m_uXPriorityMap = 0;
#endif
}

//---------------------------------------------------------------------------
void PriorityMap::Set(PORT_PRIO_TYPE uXPrio_)
{
    PORT_PRIO_TYPE uXPrioBit = PRIO_BIT(uXPrio_);
#if PRIO_MAP_MULTI_LEVEL
    PORT_PRIO_TYPE uXWordIdx = PRIO_MAP_WORD_INDEX(uXPrio_);

    m_auXPriorityMap[uXWordIdx] |= (1 << uXPrioBit);
    m_uXPriorityMapL2 |= (1 << uXWordIdx);
#else
    m_uXPriorityMap |= (1 << uXPrioBit);
#endif
}

//---------------------------------------------------------------------------
void PriorityMap::Clear(PORT_PRIO_TYPE uXPrio_)
{
    PORT_PRIO_TYPE uXPrioBit = PRIO_BIT(uXPrio_);
#if PRIO_MAP_MULTI_LEVEL
    PORT_PRIO_TYPE uXWordIdx = PRIO_MAP_WORD_INDEX(uXPrio_);

    m_auXPriorityMap[uXWordIdx] &= ~(1 << uXPrioBit);
    if (!m_auXPriorityMap[uXWordIdx]) {
        m_uXPriorityMapL2 &= ~(1 << uXWordIdx);
    }
#else
    m_uXPriorityMap &= ~(1 << uXPrioBit);
#endif
}

//---------------------------------------------------------------------------
PORT_PRIO_TYPE PriorityMap::HighestPriority(void)
{
#if PRIO_MAP_MULTI_LEVEL
    PORT_PRIO_TYPE uXMapIdx = priority_from_bitmap(m_uXPriorityMapL2);
    if (!uXMapIdx) {
        return 0;
    }
    uXMapIdx--;
    PORT_PRIO_TYPE uXPrio = priority_from_bitmap(m_auXPriorityMap[uXMapIdx]);
    uXPrio += (uXMapIdx * PRIO_MAP_BITS);
#else
    PORT_PRIO_TYPE uXPrio = priority_from_bitmap(m_uXPriorityMap);
#endif
    return uXPrio;
}
