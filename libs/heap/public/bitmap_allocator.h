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
    \file bitmap_allocator.h
    \brief Fixed block allocator using a 2-level bitmap structure to track
           allocations.
*/
#pragma once

#include "mark3.h"

//---------------------------------------------------------------------------
#define UINT32_SHIFT            (5)
#define UINT32_BITS             (32)
#define UINT32_ROUND_UP(bits)   (((uint32_t)(bits) + (UINT32_BITS - 1)) >> UINT32_SHIFT)

namespace Mark3
{

//---------------------------------------------------------------------------
/**
 * @brief The BitmapAllocator class
 *
 * This class implements a 2-level bitmap allocator.  Objects allocated from the
 * alloctor are the same size, and each object's state (available/allocated) is
 * indicated by a single bit in an array of words.  A second level bitmap is used
 * to further identify which words contain free allocations.
 *
 */
class BitmapAllocator
{
public:

    /**
     * @brief Init
     *
     * Initialize the allocator object, and initialize the blob of memory managed
     * by it.
     *
     * @param pvMemBlock_ Block of memory to manage with this allocator object
     * @param u32BlockSize_ Size of the block of memory to manage
     * @param u32ElementSize Size of the
     */
    void Init(void* pvMemBlock_, uint32_t u32BlockSize_, uint32_t u32ElementSize);

    /**
     * @brief Allocate
     *
     * Allocate a single fixed-size block from the allocator
     *
     * @param pvTag_ User-supplied metadata to assign to the allocated object
     * @return Pointer to a blob of memory, or NULL on out-of-memory
     */
    void* Allocate(void* pvTag_);
    
    /**
     * @brief Free
     *
     * Return a previously-allocated object back to the allocator
     *
     * @param alloc Previously-allocated block managed by this object
     */
    void Free(void* alloc);

    /**
     * @brief GetNumFree
     * @return Number of free elements in the allocator
     */
    uint32_t GetNumFree(void);
    
    /**
     * @brief IsEmpty
     * @return true if there are no elements in-use.
     */
    bool IsEmpty(void);
    
    /**
     * @brief IsFull
     * @return true if there are no free elements.
     */
    bool IsFull(void);

private:

    /**
     * @brief CountLeadingZeros
     *
     * Utility function to count the number of leading zero-bits in an
     * unsigned integer
     *
     * @param u32Value_ Value to count leading zeros on
     * @return number of leading zeros in the object.  Returns 32 if all bits are zero.
     */
    static uint8_t CountLeadingZeros(uint32_t u32Value_);
    
    /**
     * @brief NextFreeIndex
     * @return Bit index of the next free allocation
     */
    uint32_t NextFreeIndex(void);
    
    /**
     * @brief SetFree
     * @param u32Index_ Index of the bit to mark as free in the bitmap
     */
    void SetFree(uint32_t u32Index_);
    
    /**
     * @brief SetAllocated
     * @param u32Index_ Idex of the bit to mark as allocated in the bitmap
     */
    void SetAllocated(uint32_t u32Index_);

    /**
     * @brief IsAllocated
     *
     * Check to see if the object at a given index is in-use.
     *
     * @param u32Index_ Index of object to check
     * @return true if object in-use, false otherwise.
     */
    bool IsAllocated(uint32_t u32Index_);

    uint32_t m_u32MapL1;
    uint32_t* m_pu32MapL2;
    uint32_t m_u32NumElements;
    uint32_t m_u32NumFree;
    uint32_t m_u32ObjSize;
    void* m_pvMemBlock;
};

//---------------------------------------------------------------------------
// Metadata structure attached to each allocation.
typedef struct __attribute__((packed)) {
    BitmapAllocator* pclSource;
    void* pvTag;
    uint32_t u32Index;
    K_WORD data[1];
} bitmap_alloc_t;
} //namespace Mark3
