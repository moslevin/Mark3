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

#include <stdint.h>

#include "arena.h"

//#include <stdlib.h>
//#include <stdio.h>
//#define DEBUG_PRINT(...)        fprintf(stderr, "[%s: %d] ", __FILE__, __LINE__); fprintf(stderr, __VA_ARGS__ );
#define DEBUG_PRINT(...)

//---------------------------------------------------------------------------
const static uint32_t au32ArenaSizes[ ARENA_LIST_COUNT ] =
{
    ARENA_SIZE_0,
    ARENA_SIZE_1,
    ARENA_SIZE_2,
    ARENA_SIZE_3,
    ARENA_SIZE_4,
    ARENA_SIZE_5,
    ARENA_SIZE_6,
    ARENA_SIZE_7,
    ARENA_SIZE_8,
    ARENA_SIZE_9,
    ARENA_SIZE_10,
    ARENA_SIZE_11
};

//---------------------------------------------------------------------------
int Arena::Init( void *pvBuffer_, uint32_t u32Size_ )
{
    // Initialize the array of blocklists used in this Arena
    m_pvData = pvBuffer_;
    DEBUG_PRINT("Initializing Arena @ 0x%X, %d bytes long\n", pvBuffer_, u32Size_ );
    for (int i = 0; i < ARENA_LIST_COUNT; i++)
    {
        m_aclBlockList[i].Init( au32ArenaSizes[i] );
    }

    // Pre-populate the block-list with the largest-size blocks
    // possible, until the whole contiguous buffer is completely
    // accounted for.
    uint32_t u32SizeRemain = u32Size_;
    PTR_INT uPtr = (PTR_INT)pvBuffer_;
    while (u32SizeRemain >= MIN_ALLOC_SIZE)
    {
        HeapBlock *pclBlock = (HeapBlock*)uPtr;

        DEBUG_PRINT(" Heap Blob - %d bytes remain\n", u32SizeRemain )
        DEBUG_PRINT(" Creating new Root block @ 0x%X\n", pclBlock );

        // Figure out the best size-list to accommodate the remaining space
        uint32_t uList = ListForSize(u32SizeRemain - sizeof(HeapBlock));

        if (uList == ARENA_EXHAUSTED)
        {
            DEBUG_PRINT("  Bigger than the largest arena available\n");
            // Bigger than the biggest list -- use the biggest list.
            uList = ARENA_LIST_COUNT - 1;
        }
        else
        {
            DEBUG_PRINT("  Using arena size list [%d] - %d bytes\n", uList, m_aclBlockList[uList].GetBlockSize());
        }
        pclBlock->RootInit( m_aclBlockList[uList].GetBlockSize() );

        // Add the heap block to the list
        DEBUG_PRINT("  Adding to arena\n");
        pclBlock->SetArenaIndex(uList);
        DEBUG_PRINT("  Push block to list\n");
        m_aclBlockList[uList].PushBlock(pclBlock);

        DEBUG_PRINT("  Recalculating size\n");
        // Update the remaining buffer size
        u32SizeRemain -= pclBlock->GetBlockSize();
        uPtr += pclBlock->GetBlockSize();
    }
}

//---------------------------------------------------------------------------
void *Arena::Allocate( uint32_t u32Size_ )
{
    // Figure out which list to grab the buffer from.
    DEBUG_PRINT("Request to allocate %d bytes\n", u32Size_ );
    uint32_t uList = ListToSatisfy( u32Size_ );

    if (uList == ARENA_EXHAUSTED)
    {
        DEBUG_PRINT(" Arena Exhausted, bailing\n" );
        return 0;
    }

    // Pop the first block from the arena list
    HeapBlock *pclRet = m_aclBlockList[uList].PopBlock();

    DEBUG_PRINT(" Returned block: 0x%X, size %d\n", pclRet, pclRet ? pclRet->GetDataSize() : 0 );

    // If the size of the heap block's data buffer is large
    // enough to accommodate both the allocation request, and
    // another block, then split the block and add the
    // remainder back into the arena list.
    if (pclRet->GetDataSize() >= (MIN_ALLOC_SIZE + u32Size_ ))
    {
        DEBUG_PRINT("  Block size %d is large enough to split (min size: %d)\n", pclRet->GetDataSize(), MIN_ALLOC_SIZE );
        HeapBlock *pclNew = pclRet->Split(u32Size_);

        uList = ListForSize( pclNew->GetDataSize() );
        pclNew->SetArenaIndex( uList );
        m_aclBlockList[uList].PushBlock( pclNew );
    }

    // Mark this block as allocated and return a pointer to the block's data
    // pointer.
    pclRet->SetCookie(HEAP_COOKIE_ALLOCATED);
    return pclRet->GetDataPointer();
}

//---------------------------------------------------------------------------
void Arena::Free( void *pvBlock_ )
{
    PTR_INT uBlockAddr = (PTR_INT)pvBlock_ - sizeof(HeapBlock);
    HeapBlock *pclBlock = (HeapBlock*)uBlockAddr;
    HeapBlock *pclRight = pclBlock->GetRightSibling();
    HeapBlock *pclTemp = pclRight;

    uint32_t u32ArenaIndex;

    // Block coalescing
    //   Merge right, absorb into current-node
    pclTemp = pclRight;
    DEBUG_PRINT(" Data Pointer: 0x%X, Object 0x%X, Cookie %08X\n",
                    pvBlock_, pclBlock, pclBlock->GetCookie() );
    while (pclTemp && (pclTemp->GetCookie() == HEAP_COOKIE_FREE))
    {
        // Remove this free block from its currently allocated arena
        u32ArenaIndex = pclTemp->GetArenaIndex();
        m_aclBlockList[u32ArenaIndex].RemoveBlock( pclTemp );

        pclBlock->Coalesce();

        // Check out the next object in the list, and rebuild the sibling-node connections
        pclTemp = pclBlock->GetRightSibling();
    }

    // Merge left, absorb into left-node.
    pclTemp = pclBlock->GetLeftSibling();
    pclRight = pclBlock;
    while (pclTemp && (pclTemp->GetCookie() == HEAP_COOKIE_FREE))
    {
        // Remove this free block from its currently allocated arena
        u32ArenaIndex = pclTemp->GetArenaIndex();
        m_aclBlockList[u32ArenaIndex].RemoveBlock( pclTemp );

        pclTemp->Coalesce();

        pclBlock = pclTemp;
        pclTemp = pclTemp->GetLeftSibling();
    }

    pclBlock->SetCookie( HEAP_COOKIE_FREE );

    // Now that all adjacent blocks have been coalesced, add the single block
    // back to the correct arena, and we're done!
    u32ArenaIndex = ListForSize( pclBlock->GetDataSize() );
    if (u32ArenaIndex == ARENA_EXHAUSTED)
    {
        // Big error...
    }
    pclBlock->SetArenaIndex(u32ArenaIndex);
    m_aclBlockList[u32ArenaIndex].PushBlock( pclBlock );
}

//---------------------------------------------------------------------------
uint32_t Arena::ListForSize( uint32_t u32Size_ )
{
    if (u32Size_ < ARENA_SIZE_0)
    {
        return ARENA_EXHAUSTED;
    }

    for (int i = 0; i < ARENA_LIST_COUNT; i++)
    {
        if (u32Size_ < m_aclBlockList[i].GetBlockSize())
        {
            DEBUG_PRINT("   Size %d goes in List: %d\n", u32Size_, i - 1);
            return i -  1;
        }
    }
    return ARENA_LIST_COUNT-1;
}
//---------------------------------------------------------------------------
uint32_t Arena::ListToSatisfy( uint32_t u32Size_ )
{
    for (int i = 0; i < ARENA_LIST_COUNT; i++)
    {
        if ( (u32Size_ <= m_aclBlockList[i].GetBlockSize()) &&
             (m_aclBlockList[i].GetBlockCount()) )
        {
            DEBUG_PRINT("  Allocate from List : %d (%d bytes, %d blocks)\n", i, m_aclBlockList[i].GetBlockSize(), m_aclBlockList[i].GetBlockCount() );
            return i;
        }
    }
    DEBUG_PRINT("  Arena Exhausted\n" );
    return ARENA_EXHAUSTED;
}

//---------------------------------------------------------------------------
void Arena::Print( void )
{
    for (int i = 0; i < ARENA_LIST_COUNT; i++)
    {
        printf( " List %d (%d bytes): %d blocks free\n",
                i, m_aclBlockList[i].GetBlockSize(), m_aclBlockList[i].GetBlockCount() );
    }
}
