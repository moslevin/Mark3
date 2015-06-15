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

    \file   arena.cpp

    \brief  Traditional heap memory allocator.
*/

#ifndef __ARENA_H__
#define __ARENA_H__

#include <stdint.h>
#include "arenalist.h"
#include "heapblock.h"

//---------------------------------------------------------------------------
#define ARENA_LIST_COUNT        (12)

//---------------------------------------------------------------------------
#define ARENA_SIZE_0            (4)
#define ARENA_SIZE_1            (16)
#define ARENA_SIZE_2            (32)
#define ARENA_SIZE_3            (64)
#define ARENA_SIZE_4            (128)
#define ARENA_SIZE_5            (256)
#define ARENA_SIZE_6            (512)
#define ARENA_SIZE_7            (1024)
#define ARENA_SIZE_8            (2048)
#define ARENA_SIZE_9            (4096)
#define ARENA_SIZE_10           (8192)
#define ARENA_SIZE_11           (16384)

//---------------------------------------------------------------------------
#define MAX_ARENA_SIZE          (ARENA_SIZE_11)

//---------------------------------------------------------------------------
#define ARENA_EXHAUSTED         (0xC0FFEEEE)

//---------------------------------------------------------------------------
#define MIN_ALLOC_SIZE          (sizeof(HeapBlock) + ARENA_SIZE_0)

//---------------------------------------------------------------------------
class Arena
{
public:
    int Init( void *pvBuffer_, uint32_t u32Size_ );

    void *Allocate( uint32_t u32Size_ );

    void Free( void *pvBlock_ );

    void Print( void );
private:

    uint32_t ListForSize( uint32_t u32Size_ );

    uint32_t ListToSatisfy( uint32_t u32Size_ );

    ArenaList  m_aclBlockList[ARENA_LIST_COUNT + 1];
    void      *m_pvData;
};

#endif

