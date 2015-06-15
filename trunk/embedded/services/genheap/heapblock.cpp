#include "heapblock.h"

//---------------------------------------------------------------------------
void HeapBlock::RootInit( uint32_t u32Size_ )
{
    Init();
    m_u32DataSize = ROUND_DOWN_U32(u32Size_);

    SetCookie( HEAP_COOKIE_FREE );

    SetRightSibling(0);
    SetLeftSibling(0);
}

//---------------------------------------------------------------------------
HeapBlock *HeapBlock::Split( uint32_t u32Size_ )
{
    // Allocate minimum amount of data for this operation on the left side

    uint32_t u32LeftBlockSize = ROUND_UP_U32(u32Size_) + sizeof(HeapBlock);

    uint32_t u32ThisBlockSize = m_u32DataSize + sizeof(HeapBlock);
    uint32_t u32RightBlockSize = u32ThisBlockSize - u32LeftBlockSize;

    m_u32DataSize = ROUND_UP_U32(u32Size_);

    PTR_INT uNewAddr = (PTR_INT)this + u32LeftBlockSize;
    HeapBlock *pclRightBlock = (HeapBlock*)uNewAddr;

    pclRightBlock->Init();
    pclRightBlock->SetDataSize( u32RightBlockSize - sizeof(HeapBlock) );
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
uint32_t HeapBlock::GetDataSize( void )
{
    return m_u32DataSize;
}

//---------------------------------------------------------------------------
uint32_t HeapBlock::GetBlockSize( void )
{
    return ( sizeof(HeapBlock) + m_u32DataSize );
}

//---------------------------------------------------------------------------
void HeapBlock::SetArenaIndex( uint32_t u32List_ )
{
    m_u32ArenaIndex = u32List_;
}

//---------------------------------------------------------------------------
uint32_t HeapBlock::GetArenaIndex( void )
{
    return m_u32ArenaIndex;
}

//---------------------------------------------------------------------------
void HeapBlock::SetDataSize( uint32_t u32BlockSize_ )
{
    m_u32DataSize = u32BlockSize_;
}


