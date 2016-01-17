/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012-2015 Funkenstein Software Consulting, all rights reserved.
See license.txt for more information
===========================================================================*/
/*!
    \file priomap.cpp
    \brief Priority map data structure
*/

#include "mark3.h"
#include "priomap.h"

#include <stdint.h>
#include <stdbool.h>

//---------------------------------------------------------------------------
static uint8_t priority_from_bitmap( PRIO_TYPE uXPrio_ )
{
    PRIO_TYPE uXMask =  (1 << (PRIO_MAP_BITS - 1));
    uint8_t u8Zeros = 0;

    while (uXMask)
    {
        if (uXMask & uXPrio_)
        {
            return (PRIO_MAP_BITS - u8Zeros);
        }

        uXMask >>= 1;
        u8Zeros++;
    }
    return 0;
}

//---------------------------------------------------------------------------
PriorityMap::PriorityMap()
{
#if PRIO_MAP_MULTI_LEVEL
    m_uXPriorityMapL2 = 0;
    for (int i = 0; i < PRIO_MAP_NUM_WORDS; i++)
    {
        m_auXPriorityMap[ i ] = 0;
    }
#else
    m_uXPriorityMap = 0;
#endif
}

//---------------------------------------------------------------------------
void PriorityMap::Set( PRIO_TYPE uXPrio_ )
{
    PRIO_TYPE uxPrioBit = (uXPrio_ & PRIO_MAP_BIT_MASK);
#if PRIO_MAP_MULTI_LEVEL
    PRIO_TYPE uXWordIdx = PRIO_MAP_WORD_INDEX( prio );
    PRIO_TYPE uXMapIdx  = SCHEDULER_L2_WORD_INDEX( prio );

    m_auXPriorityMap[ uxWordIdx ] |= (1 << uxPrioBit);
    m_uXPriorityMapL2 |= (1 << uXMapIdx);
#else
    m_uXPriorityMap   |= (1 << uxPrioBit);
#endif
}

//---------------------------------------------------------------------------
void PriorityMap::Clear( PRIO_TYPE uXPrio_ )
{
    PRIO_TYPE uxPrioBit = (uXPrio_ & PRIO_MAP_BIT_MASK);
#if PRIO_MAP_MULTI_LEVEL
    PRIO_TYPE uXWordIdx = PRIO_MAP_WORD_INDEX( prio );
    PRIO_TYPE uXMapIdx  = SCHEDULER_L2_WORD_INDEX( prio );

    m_auXPriorityMap[ uxWordIdx ] &= ~(1 << uxPrioBit);
    if (!m_auXPriorityMap[ uxWordIdx ])
    {
        m_uXPriorityMapL2 &= ~(1 << uXMapIdx);
    }
#else
    m_uXPriorityMap   &= ~(1 << uxPrioBit);
#endif
}

//---------------------------------------------------------------------------
PRIO_TYPE PriorityMap::HighestPriority( void )
{
#if PRIO_MAP_MULTI_LEVEL
    PRIO_TYPE uXMapIdx = priority_from_bitmap( m_uXPriorityMapL2 );
    if (!uXMapIdx)
    {
        return 0;
    }
    uXMapIdx--;
    PRIO_TYPE uXPrio   = priority_from_bitmap( m_auXPriorityMap[ uXMapIdx ] );
    uXPrio += (uXMapIdx * PRIO_MAP_BITS);
#else
    PRIO_TYPE uXPrio   = priority_from_bitmap( m_uXPriorityMap );
#endif
    return uXPrio;
}
