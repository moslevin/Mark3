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

    \file   heapblock.h

    \brief  Metadata object used to manage a heap allocation
*/

#ifndef __HEAPBLOCK_H__
#define __HEAPBLOCK_H__

#include <stdint.h>
#include "ll.h"
//---------------------------------------------------------------------------
#define PTR_SIZE    (4)

//---------------------------------------------------------------------------
#if (PTR_SIZE == 2)
  #define PTR_INT       uint16_t
#elif (PTR_SIZE == 4)
  #define PTR_INT       uint32_t
#elif (PTR_SIZE == 8)
  #define PTR_INT       uint64_t
#endif

//---------------------------------------------------------------------------
#define HEAP_COOKIE_FREE                (0xCAFED00D)
#define HEAP_COOKIE_ALLOCATED           (0xDEADBEEF)

//---------------------------------------------------------------------------
#define ROUND_UP_U32(x)                  ((((uint32_t)x) + 3) & ~0x03)
#define ROUND_DOWN_U32(x)                (((uint32_t)x) & ~0x3)

#define BLOCK_DATA_SIZE(x)               (ROUND_DOWN_U32(x) - sizeof(HeapBlock))

//---------------------------------------------------------------------------
// LinkListNode, because that's what's used in arena list management
//  (Logical)
//
// HepNode, because that's what's used to manage the contiguous buffer space
//  (Physical)
//

class HeapBlock : public LinkListNode
{

public:
    void RootInit( uint32_t u32Size_ );

    HeapBlock *Split( uint32_t u32Size_ );

    // Merge this block with RIGHT neighbor.
    void Coalesce( void );

    void *GetDataPointer( void );

    uint32_t GetDataSize( void );

    uint32_t GetBlockSize( void );

    void SetArenaIndex( uint32_t u32List_ );

    uint32_t GetArenaIndex( void );

    void SetCookie( uint32_t u32Cookie_ )
    {
        m_u32Cookie = u32Cookie_;
    }

    uint32_t GetCookie( void )
    {
        return m_u32Cookie;
    }

    HeapBlock *GetLeftSibling( void )
    {
        return m_pclLeft;
    }

    HeapBlock *GetRightSibling( void )
    {
        return m_pclRight;
    }

    void SetRightSibling( HeapBlock *pclRight_ )
    {
        m_pclRight = pclRight_;
    }

    void SetLeftSibling( HeapBlock * pclLeft_ )
    {
        m_pclLeft = pclLeft_;
    }

private:
    void Init(void)
    {
        m_u32DataSize = 0;
        m_u32ArenaIndex = 0;
        m_u32Cookie = 0;
        m_pclRight = 0;
        m_pclLeft = 0;
        LinkListNode::ClearNode();
    }

    void SetDataSize( uint32_t u32BlockSize_ );

    uint32_t  m_u32DataSize;
    uint32_t  m_u32ArenaIndex;

    uint32_t  m_u32Cookie;

    HeapBlock *m_pclRight;
    HeapBlock *m_pclLeft;

};

#endif
