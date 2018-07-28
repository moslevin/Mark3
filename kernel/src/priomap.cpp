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
    \file priomap.cpp
    \brief Priority map data structure
*/

#include "mark3.h"
#include "priomap.h"
#include "threadport.h"

#include <stdint.h>
#include <stdbool.h>
namespace Mark3
{
//---------------------------------------------------------------------------
namespace {
inline uint8_t priority_from_bitmap(PORT_PRIO_TYPE uXPrio_)
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
} // anonymous namespace

//---------------------------------------------------------------------------
PriorityMap::PriorityMap()
{
    m_uXPriorityMapL2 = 0;
    for (int i = 0; i < PRIO_MAP_NUM_WORDS; i++) {
        m_auXPriorityMap[i] = 0;
    }
}

//---------------------------------------------------------------------------
void PriorityMap::Set(PORT_PRIO_TYPE uXPrio_)
{
    PORT_PRIO_TYPE uXPrioBit = PRIO_BIT(uXPrio_);
    PORT_PRIO_TYPE uXWordIdx = PRIO_MAP_WORD_INDEX(uXPrio_);

    m_auXPriorityMap[uXWordIdx] |= (1 << uXPrioBit);
    m_uXPriorityMapL2 |= (1 << uXWordIdx);
}

//---------------------------------------------------------------------------
void PriorityMap::Clear(PORT_PRIO_TYPE uXPrio_)
{
    PORT_PRIO_TYPE uXPrioBit = PRIO_BIT(uXPrio_);
    PORT_PRIO_TYPE uXWordIdx = PRIO_MAP_WORD_INDEX(uXPrio_);

    m_auXPriorityMap[uXWordIdx] &= ~(1 << uXPrioBit);
    if (!m_auXPriorityMap[uXWordIdx]) {
        m_uXPriorityMapL2 &= ~(1 << uXWordIdx);
    }
}

//---------------------------------------------------------------------------
PORT_PRIO_TYPE PriorityMap::HighestPriority(void)
{
    PORT_PRIO_TYPE uXMapIdx = priority_from_bitmap(m_uXPriorityMapL2);
    if (!uXMapIdx) {
        return 0;
    }
    uXMapIdx--;
    PORT_PRIO_TYPE uXPrio = priority_from_bitmap(m_auXPriorityMap[uXMapIdx]);
    uXPrio += (uXMapIdx * PRIO_MAP_BITS);
    return uXPrio;
}
} //namespace Mark3
