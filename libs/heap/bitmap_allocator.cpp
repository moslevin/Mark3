/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012 - 2017 Funkenstein Software Consulting, all rights reserved.
See license.txt for more information
=========================================================================== */
/*!
    \file bitmap_allocator.cpp
    \brief Fixed block allocator using a 2-level bitmap structure to track
           allocations.
*/

#include "bitmap_allocator.h"
namespace Mark3 {
//---------------------------------------------------------------------------
void BitmapAllocator::Init(void* pvMemBlock_, uint32_t u32BlockSize_, uint32_t u32ElementSize)
{        
    // Add allocator metadata to the size of the element
    u32ElementSize += sizeof(bitmap_alloc_t) - sizeof(K_WORD);
    m_u32ObjSize = u32ElementSize;

    // Compute the amount of the memory block used for metadata
    uint32_t u32AllocableBytes = u32BlockSize_;
    uint32_t u32MaxAllocs = u32AllocableBytes / u32ElementSize;
    uint32_t u32MetaDataSize = UINT32_ROUND_UP(u32MaxAllocs) * sizeof(uint32_t);

    // Set metadata address from block
    m_pu32MapL2 = static_cast<uint32_t*>(pvMemBlock_);

    // Set address of first allocable chunk (after metadata)
    m_pvMemBlock = (void*)((uint32_t)pvMemBlock_ + u32MetaDataSize);

    // Get true number of allocable elements.
    m_u32NumElements = u32AllocableBytes / u32ElementSize;
    m_u32NumFree = m_u32NumElements;
    // Clear the bitmap allocator's metadata
    m_u32MapL1 = 0;
    for (uint32_t i = 0; i < UINT32_ROUND_UP(m_u32NumElements); i++) {
        m_pu32MapL2[i] = 0;
    }
    
    for (uint32_t i = 0; i < m_u32NumElements; i++) {
        SetFree(i);
    }
}

//---------------------------------------------------------------------------
void* BitmapAllocator::Allocate(void* pvTag_)
{
    if (!m_u32NumFree) {            
        return NULL;
    }
    
    m_u32NumFree--;
    uint32_t u32Index = NextFreeIndex();
    SetAllocated(u32Index);
    
    bitmap_alloc_t* pstAllocData = (bitmap_alloc_t*)((uint32_t)m_pvMemBlock + (m_u32ObjSize * u32Index));
    
    pstAllocData->pclSource = this;
    pstAllocData->pvTag = pvTag_;
    pstAllocData->u32Index = u32Index;        
    
    return (void*)pstAllocData->data;
}

//---------------------------------------------------------------------------
void BitmapAllocator::Free(void* alloc)
{        
    bitmap_alloc_t* pstAlloc = (bitmap_alloc_t*)((uint32_t)alloc - (sizeof(bitmap_alloc_t) - sizeof(K_WORD)));
    pstAlloc->pclSource->SetFree(pstAlloc->u32Index);
    m_u32NumFree++;
}

//---------------------------------------------------------------------------
uint32_t BitmapAllocator::GetNumFree(void)
{
    return m_u32NumFree;
}

//---------------------------------------------------------------------------
bool BitmapAllocator::IsEmpty(void)
{
    if (m_u32NumElements == m_u32NumFree) {
        return true;
    }
    return false;
}

//---------------------------------------------------------------------------
bool BitmapAllocator::IsFull(void)
{
    if (!m_u32NumFree) {
        return true;
    }
    return false;
}

//---------------------------------------------------------------------------
uint8_t BitmapAllocator::CountLeadingZeros(uint32_t u32Value_)
{
    uint32_t u32Mask = 0x80000000;
    uint8_t u8Zeros = 0;

    while (u32Mask) {
        if (u32Mask & u32Value_) {
            return ((UINT32_BITS - 1) - u8Zeros);
        }

        u32Mask >>= 1;
        u8Zeros++;
    }
    return UINT32_BITS;
}

//---------------------------------------------------------------------------
uint32_t BitmapAllocator::NextFreeIndex(void)
{
    uint32_t u32WordIndex = CountLeadingZeros(m_u32MapL1);
    uint32_t u32BitIndex = CountLeadingZeros(m_pu32MapL2[u32WordIndex]);
    
    return (u32WordIndex * UINT32_BITS) + u32BitIndex;
}

//---------------------------------------------------------------------------
void BitmapAllocator::SetFree(uint32_t u32Index_)
{
    uint32_t u32WordIndex = u32Index_ >> UINT32_SHIFT;
    uint32_t u32BitIndex = u32Index_ & (UINT32_BITS - 1);

    m_pu32MapL2[u32WordIndex] |= (1 << u32BitIndex);
    m_u32MapL1 |= (1 << u32WordIndex);
}

//---------------------------------------------------------------------------
void BitmapAllocator::SetAllocated(uint32_t u32Index_)
{
    uint32_t u32WordIndex = u32Index_ >> UINT32_SHIFT;
    uint32_t u32BitIndex = u32Index_ & (UINT32_BITS - 1);

    m_pu32MapL2[u32WordIndex] &= ~(1 << u32BitIndex);
    if (!m_pu32MapL2[u32WordIndex]) {
        m_u32MapL1 &= ~(1 << u32WordIndex);
    }
}

//---------------------------------------------------------------------------
bool BitmapAllocator::IsAllocated(uint32_t u32Index_)
{
    uint32_t u32WordIndex = u32Index_ >> UINT32_SHIFT;
    uint32_t u32BitIndex = u32Index_ & (UINT32_BITS - 1);

    if (m_pu32MapL2[u32WordIndex] & (1 << u32BitIndex)) {
        return true;
    }
    return false;
}
} //namespace Mark3
