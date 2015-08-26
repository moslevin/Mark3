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

    \file   heapblock.cpp

    \brief  Metadata object used to manage a heap allocation
*/


#include "heapblock.h"

//---------------------------------------------------------------------------
void HeapBlock::RootInit( PTR_INT uSize_ )
{
    Init();
    m_uDataSize = ROUND_DOWN(uSize_);

    SetCookie( HEAP_COOKIE_FREE );

    SetRightSibling(0);
    SetLeftSibling(0);
}

//---------------------------------------------------------------------------
HeapBlock *HeapBlock::Split( PTR_INT uSize_ )
{
    // Allocate minimum amount of data for this operation on the left side

    PTR_INT uLeftBlockSize = ROUND_UP(uSize_) + sizeof(HeapBlock);

    PTR_INT uThisBlockSize = m_uDataSize + sizeof(HeapBlock);
    PTR_INT uRightBlockSize = uThisBlockSize - uLeftBlockSize;

    m_uDataSize = ROUND_UP(uSize_);

    PTR_INT uNewAddr = (PTR_INT)this + uLeftBlockSize;
    HeapBlock *pclRightBlock = (HeapBlock*)uNewAddr;

    pclRightBlock->Init();
    pclRightBlock->SetDataSize( uRightBlockSize - sizeof(HeapBlock) );
    pclRightBlock->SetCookie( HEAP_COOKIE_FREE );

    pclRightBlock->SetLeftSibling( this );
    pclRightBlock->SetRightSibling( GetRightSibling() );

    SetRightSibling( pclRightBlock );

    return pclRightBlock;
}

//---------------------------------------------------------------------------
// Merge this block with RIGHT neighbor.
void HeapBlock::Coalesce( void )
{
    HeapBlock *pclTemp;

    pclTemp = GetRightSibling();
    // Add the size of this object to the left object.
    SetDataSize( GetDataSize() + pclTemp->GetBlockSize() );

    // Reconnect sibling pointers between this block and "absorbed" right block
    SetRightSibling( pclTemp->GetRightSibling() );
    if (GetRightSibling())
    {
        GetRightSibling()->SetLeftSibling( this );
    }
}

//---------------------------------------------------------------------------
void *HeapBlock::GetDataPointer( void )
{
    PTR_INT uAddr = (PTR_INT)this;
    uAddr += sizeof(HeapBlock);
    return (void*)uAddr;
}

//---------------------------------------------------------------------------
PTR_INT HeapBlock::GetDataSize( void )
{
    return m_uDataSize;
}

//---------------------------------------------------------------------------
PTR_INT HeapBlock::GetBlockSize( void )
{
    return ( sizeof(HeapBlock) + m_uDataSize );
}

//---------------------------------------------------------------------------
void HeapBlock::SetArenaIndex( uint8_t u8List_ )
{
    m_u8ArenaIndex = u8List_;
}

//---------------------------------------------------------------------------
uint8_t HeapBlock::GetArenaIndex( void )
{
    return m_u8ArenaIndex;
}

//---------------------------------------------------------------------------
void HeapBlock::SetDataSize( PTR_INT uBlockSize )
{
    m_uDataSize = uBlockSize;
}

