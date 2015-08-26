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

#include "mark3cfg.h"
#include "kerneltypes.h"
#include "mark3.h"
//---------------------------------------------------------------------------
#if defined(AVR) || defined (MSP430)
  #define PTR_SIZE	(2)
#else
  #define PTR_SIZE	(4)
#endif

//---------------------------------------------------------------------------
#if (PTR_SIZE == 2)
  #define PTR_INT       uint16_t
  #define HEAP_COOKIE_FREE                (0xCAFE)
  #define HEAP_COOKIE_ALLOCATED           (0xDEAD)
#elif (PTR_SIZE == 4)
  #define PTR_INT       uint32_t
  #define HEAP_COOKIE_FREE                (0xCAFED00D)
  #define HEAP_COOKIE_ALLOCATED           (0xDEADBEEF)
#elif (PTR_SIZE == 8)
  #define PTR_INT       uint64_t
  #define HEAP_COOKIE_FREE                (0xCAFED00DFEEDBABE)
  #define HEAP_COOKIE_ALLOCATED           (0xDEADBEEFABACABB0)
#else 
  #error PTR_SIZE invalid!
#endif

//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
#define ROUND_UP(x)                  ((((PTR_INT)x) + (PTR_SIZE-1)) & ~(PTR_SIZE-1))
#define ROUND_DOWN(x)                (((PTR_INT)x) & ~(PTR_SIZE-1))

#define BLOCK_DATA_SIZE(x)           (ROUND_DOWN(x) - sizeof(HeapBlock))

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
    void RootInit( PTR_INT uSize_ );

    HeapBlock *Split( PTR_INT uSize_ );

    // Merge this block with RIGHT neighbor.
    void Coalesce( void );

    void *GetDataPointer( void );

    PTR_INT GetDataSize( void );

    PTR_INT GetBlockSize( void );

    void SetArenaIndex( uint8_t u8List_ );

    uint8_t GetArenaIndex( void );

    void SetCookie( PTR_INT uCookie_ )
    {
        m_uCookie = uCookie_;
    }

    PTR_INT GetCookie( void )
    {
        return m_uCookie;
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
        m_uDataSize = 0;
        m_u8ArenaIndex = 0;
        m_uCookie = 0;
        m_pclRight = 0;
        m_pclLeft = 0;
        LinkListNode::ClearNode();
    }

    void SetDataSize( PTR_INT uBlockSize_ );

    PTR_INT   m_uDataSize;
    PTR_INT   m_uCookie; 

    uint8_t   m_u8ArenaIndex;

    HeapBlock *m_pclRight;
    HeapBlock *m_pclLeft;

};

#endif
