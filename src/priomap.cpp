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
    @file priomap.cpp
    @brief Priority map data structure
*/

#include "mark3.h"

namespace Mark3
{
//---------------------------------------------------------------------------
PriorityMap::PriorityMap()
{
#if PRIO_MAP_MULTI_LEVEL
    m_uXPriorityMapL2 = 0;
    for (PORT_PRIO_TYPE i = 0; i < m_uXPrioMapNumWords; i++) { m_auXPriorityMap[i] = 0; }
#else
    m_uXPriorityMap = 0;
#endif
}

//---------------------------------------------------------------------------
void PriorityMap::Set(PORT_PRIO_TYPE uXPrio_)
{
    auto uXPrioBit = PrioBit(uXPrio_);
#if PRIO_MAP_MULTI_LEVEL
    auto uXWordIdx = PrioMapWordIndex(uXPrio_);

    m_auXPriorityMap[uXWordIdx] |= (1 << uXPrioBit);
    m_uXPriorityMapL2 |= (1 << uXWordIdx);
#else
    m_uXPriorityMap |= (1 << uXPrioBit);
#endif
}

//---------------------------------------------------------------------------
void PriorityMap::Clear(PORT_PRIO_TYPE uXPrio_)
{
    auto uXPrioBit = PrioBit(uXPrio_);
#if PRIO_MAP_MULTI_LEVEL
    auto uXWordIdx = PrioMapWordIndex(uXPrio_);

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
    auto uXMapIdx = priority_from_bitmap(m_uXPriorityMapL2);
    if (!uXMapIdx) {
        return 0;
    }
    uXMapIdx--;
    auto uXPrio = priority_from_bitmap(m_auXPriorityMap[uXMapIdx]);
    uXPrio += (uXMapIdx * m_uXPrioMapBits);
#else
    auto uXPrio = priority_from_bitmap(m_uXPriorityMap);
#endif
    return uXPrio;
}
} // namespace Mark3
