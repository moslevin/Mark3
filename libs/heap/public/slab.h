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
    \file slab.h
    \brief Slab allocator class implementation
*/

#pragma once

#include "bitmap_allocator.h"
#include "mark3.h"

namespace Mark3
{

//---------------------------------------------------------------------------
// Page allocation functions
typedef void* (*slab_alloc_page_function_t)(uint32_t* pu32PageSize_);
typedef void (*slab_free_page_function_t)(void* pvPage_);

//---------------------------------------------------------------------------
/**
 * @brief The SlabPage class
 *
 * This class defines an object that manages a blob of memory, broken down into
 * fixed-sized elements.  These elements are allocated using a bitmap allocator,
 * which is deterministic and time-bound.
 *
 * Lists of slab pages can be chained together as necessary in order to fulfill
 * the allocation requirements of the system for allocations of a given size.
 */
class SlabPage : public LinkListNode
{
public:
    /**
     * @brief InitPage
     *
     * Initialize this object, aliased to the beginning of a page of memory
     * for use as a slab page.
     *
     * @param u32PageSize_ Size of the page (in bytes)
     * @param u32ObjSize_ Size of individual allocations from this page (in bytes)
     */
    void InitPage(uint32_t u32PageSize_, uint32_t u32ObjSize_);

    /**
     * @brief Alloc
     *
     * Allocate an object from the slab page.
     *
     * @param pvTag_ User-provided tag pointer, used to assign context to an
     * allocated object.
     * @return NULL if no page is exhausted, pointer to a user-accessible memory
     * blob on success.
     */
    void* Alloc(void* pvTag_);

    /**
     * @brief Free
     *
     * Free a previously-allocated block of memory from the slab page
     *
     * @param pvObject_ Pointer to the block of memory allocated within this page
     */
    void Free(void* pvObject_);

    /**
     * @brief IsEmpty
     *
     * @return true if no allocations have been made from the page
     */
    bool IsEmpty(void);

    /**
     * @brief IsFull
     * @return true if the page is exhausted
     */
    bool IsFull(void);

private:
    BitmapAllocator m_clAllocator;
};

//---------------------------------------------------------------------------
/**
 * @brief The Slab class
 *
 * This class manages lists of slab pages - blocks of memory, each of which
 * is managed by a bitmap allocator object.  Individual pages are dynamically
 * allocated and freed as required to dynamically adapt to the system.
 */
class Slab
{
public:
    /**
     * @brief Init
     *
     * Initialize the slab allocator prior to use.
     *
     * @param u32ObjSize_ Size of elements allocatoed
     * @param pfAlloc_ Function to allocate slab pages
     * @param pfFree_ Functin to free previously-allocated slab pages
     */
    void Init(uint32_t u32ObjSize_, slab_alloc_page_function_t pfAlloc_, slab_free_page_function_t pfFree_);

    /**
     * @brief Alloc
     *
     * Allocate an element from the slab, returning a user-accessible
     * pointer to a memory blob.
     *
     * @return NULL on error/out of memory, data-pointer otherwise.
     */
    void* Alloc(void);

    /**
     * @brief Free
     *
     * Free a previously allocated element from the slab.
     *
     * @param pvObj_ Pointer to the object allocated from the slab
     */
    void Free(void* pvObj_);
    
private:

    /**
     * @brief AllocSlabPage
     *
     * Allocate a new page to be managed by the slab allocator
     *
     * @return Newly-allocated slab page, or NULL if out-of-memory
     */
    SlabPage* AllocSlabPage(void);
    
    /**
     * @brief FreeSlabPage
     *
     * Free a previously allocated page of slab memory
     *
     * @param pclPage_ Pointer to the page of memory to be freed
     */
    void FreeSlabPage(SlabPage* pclPage_);

    /**
     * @brief MoveToFull
     *
     * Move a slab page to the "full" list - indiciating that it's
     * no longer available for allocation.
     *
     * @param pclPage_ Page to move to the full list
     */
    void MoveToFull(SlabPage* pclPage_);
    
    /**
     * @brief MoveToFree
     *
     * Move a slab page to the "free" list - indicating that it has
     * element available to be allocated
     *
     * @param pclPage_ Page to move to the free list.
     */
    void MoveToFree(SlabPage* pclPage_);

    uint32_t m_u32ObjSize;
    
    DoubleLinkList m_clFreeList;
    DoubleLinkList m_clFullList;
    
    slab_alloc_page_function_t m_pfSlabAlloc;
    slab_free_page_function_t m_pfSlabFree;
};
} //namespace Mark3
