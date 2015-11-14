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
#include "mark3.h"
#include "arena.h"

#if DEBUG
  #include <stdlib.h>
  #include <stdio.h>
  #define DEBUG_PRINT(...)        fprintf(stderr, "[%s: %d] ", __FILE__, __LINE__); fprintf(stderr, __VA_ARGS__ );
#else
  #define DEBUG_PRINT(...)
#endif

//---------------------------------------------------------------------------
void Arena::Init( void *pvBuffer_, K_ADDR u32Size_, K_ADDR *au32Sizes_, uint8_t u8NumSizes_ )
{
    // Initialize the array of blocklists used in this Arena
    ArenaList* pclList = (ArenaList*)pvBuffer_;
    m_aclBlockList = (ArenaList*)pvBuffer_;
    m_u8LargestList = u8NumSizes_ - 1;

    DEBUG_PRINT("Initializing Arena @ 0x%X, %d bytes long\n", pvBuffer_, u32Size_ );
    for (uint8_t i = 0; i < u8NumSizes_; i++)
    {
        void *p = pclList;
        ArenaList *pclTemp = new(p) ArenaList();

        pclList->Init(au32Sizes_[i]);
        pclList++;
    }

    uint32_t u32MetaSize;
    u32MetaSize = (sizeof(ArenaList) * (uint32_t)(u8NumSizes_));

    // Pre-populate the block-list with the largest-size blocks
    // possible, until the whole contiguous buffer is completely
    // accounted for.
    uint32_t u32SizeRemain = u32Size_ - u32MetaSize;
    K_ADDR uPtr = (K_ADDR)((uint32_t)pvBuffer_ + u32MetaSize);
    while (u32SizeRemain >= (sizeof(HeapBlock) + au32Sizes_[0]))
    {
        HeapBlock *pclBlock = new ((void*)uPtr) HeapBlock();
        
        DEBUG_PRINT(" Heap Blob - %d bytes remain\n", u32SizeRemain )
        DEBUG_PRINT(" Creating new Root block @ 0x%X\n", pclBlock );

        // Figure out the best size-list to accommodate the remaining space
        uint8_t uList = ListForSize(u32SizeRemain - sizeof(HeapBlock));

        if (uList == ARENA_EXHAUSTED)
        {
            DEBUG_PRINT("  Bigger than the largest arena available\n");
            // Bigger than the biggest list -- use the biggest list.
            uList = u8NumSizes_ - 1;
        }
        else
        {
            DEBUG_PRINT("  using arena size list [%d] - %d bytes\n", uList, m_aclBlockList[uList].GetBlockSize());
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
void *Arena::Allocate( K_ADDR usize_ )
{
    // Figure out which list to grab the buffer from.
    DEBUG_PRINT("Request to allocate %d bytes\n", usize_ );
    HeapBlock *pclRet;

    uint8_t uList;

    CS_ENTER(); 
    uList = ListToSatisfy( usize_ );
    CS_EXIT();

    if ((uList == ARENA_EXHAUSTED) || (uList == ARENA_FULL))
    {
        DEBUG_PRINT(" Arena Exhausted, bailing\n" );
        return 0;
    }
    if (usize_ < m_aclBlockList[0].GetBlockSize())
    {
        usize_ = m_aclBlockList[0].GetBlockSize();
    }

    CS_ENTER();
    // Pop the first block from the arena list
    pclRet = m_aclBlockList[uList].PopBlock();

    DEBUG_PRINT(" Returned block: 0x%X, size %d\n", pclRet, pclRet ? pclRet->GetDataSize() : 0 );

    // If the size of the heap block's data buffer is large
    // enough to accommodate both the allocation request, and
    // another block, then split the block and add the
    // remainder back into the arena list.
    if (pclRet->GetDataSize() >= (usize_ + sizeof(HeapBlock) + m_aclBlockList[0].GetBlockSize()) )
    {
        DEBUG_PRINT("  Block size %d is large enough to split (min size: %d)\n", pclRet->GetDataSize(), m_aclBlockList[0].GetBlockSize() );
        HeapBlock *pclNew = pclRet->Split(usize_);
        
        uList = ListForSize( pclNew->GetDataSize() );

        // If the block is full, don't bother...
        if (uList != ARENA_FULL) {
            pclNew->SetArenaIndex( uList );
            m_aclBlockList[uList].PushBlock( pclNew );
        }
    }


    // Mark this block as allocated and return a pointer to the block's data
    // pointer.
    pclRet->SetCookie(HEAP_COOKIE_ALLOCATED);

    CS_EXIT();
    return pclRet->GetDataPointer();
}

//---------------------------------------------------------------------------
void Arena::Free( void *pvBlock_ )
{
    K_ADDR uBlockAddr = (K_ADDR)pvBlock_ - sizeof(HeapBlock);
    HeapBlock *pclBlock = (HeapBlock*)uBlockAddr;
    HeapBlock *pclRight = pclBlock->GetRightSibling();
    HeapBlock *pclTemp = pclRight;

    uint8_t uArenaIndex;

    // Block coalescing
    //   Merge right, absorb into current-node
    CS_ENTER();

    pclTemp = pclRight;
    DEBUG_PRINT(" Data Pointer: 0x%X, Object 0x%X, Cookie %08X\n",
                    pvBlock_, pclBlock, pclBlock->GetCookie() );
    while (pclTemp && (pclTemp->GetCookie() == HEAP_COOKIE_FREE))
    {
        // Remove this free block from its currently allocated arena
        uArenaIndex = pclTemp->GetArenaIndex();
        m_aclBlockList[uArenaIndex].RemoveBlock( pclTemp );

        pclBlock->Coalesce();

        // Check out the next object in the list, and rebuild the sibling-node connections
        pclTemp = pclBlock->GetRightSibling();
    }

    CS_EXIT();

    CS_ENTER();
    // Merge left, absorb into left-node.
    pclTemp = pclBlock->GetLeftSibling();
    pclRight = pclBlock;
    while (pclTemp && (pclTemp->GetCookie() == HEAP_COOKIE_FREE))
    {
        // Remove this free block from its currently allocated arena
        uArenaIndex = pclTemp->GetArenaIndex();
        m_aclBlockList[uArenaIndex].RemoveBlock( pclTemp );

        pclTemp->Coalesce();

        pclBlock = pclTemp;
        pclTemp = pclTemp->GetLeftSibling();
    }

    pclBlock->SetCookie( HEAP_COOKIE_FREE );

    // Now that all adjacent blocks have been coalesced, add the single block
    // back to the correct arena, and we're done!
    uArenaIndex = ListForSize( pclBlock->GetDataSize() );
    if (uArenaIndex == ARENA_EXHAUSTED)
    {
        // Big error...
    }
    pclBlock->SetArenaIndex(uArenaIndex);
    m_aclBlockList[uArenaIndex].PushBlock( pclBlock );

    CS_EXIT();
}

//---------------------------------------------------------------------------
uint8_t Arena::ListForSize( K_ADDR usize_ )
{
    if (usize_ < m_aclBlockList[0].GetBlockSize())
    {
        return ARENA_FULL;
    }
    else if (usize_ > m_aclBlockList[m_u8LargestList].GetBlockSize())
    {
        return ARENA_EXHAUSTED;
    }

    for (uint8_t i = 0; i <= m_u8LargestList ; i++)
    {
        if (usize_ <= m_aclBlockList[i].GetBlockSize())
        {
            DEBUG_PRINT("   Size %d goes in List: %d\n", usize_, i - 1);
            return (i -  1);
        }
    }
    return m_u8LargestList;
}
//---------------------------------------------------------------------------
uint8_t Arena::ListToSatisfy( K_ADDR usize_ )
{
    for (uint8_t i = 0; i <= m_u8LargestList; i++)
    {
        if ( (usize_ <= m_aclBlockList[i].GetBlockSize()) &&
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
#if DEBUG

    for (uint8_t i = 0; i <= m_u8LargestList; i++)
    {
        printf( " List %d (%d bytes): %d blocks free\n",
                i, m_aclBlockList[i].GetBlockSize(), m_aclBlockList[i].GetBlockCount() );
    }
#endif
}
