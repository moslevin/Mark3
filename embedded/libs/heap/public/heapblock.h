/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012-2016 Funkenstein Software Consulting, all rights reserved.
See license.txt for more information
===========================================================================*/
/*!

    \file   heapblock.h

    \brief  Metadata object used to manage a heap allocation
*/

#ifndef __HEAPBLOCK_H__
#define __HEAPBLOCK_H__

#include <stdlib.h>
#include "mark3cfg.h"
#include "kerneltypes.h"
#include "mark3.h"
#include "ll.h"
//---------------------------------------------------------------------------
#if defined(AVR) || defined (MSP430)
  #define PTR_SIZE	(2)
#else
  #define PTR_SIZE	(4)
#endif

//---------------------------------------------------------------------------
#if (PTR_SIZE == 2)
  #define HEAP_COOKIE_FREE                (0xCAFE)
  #define HEAP_COOKIE_ALLOCATED           (0xDEAD)
#elif (PTR_SIZE == 4)  
  #define HEAP_COOKIE_FREE                (0xCAFED00D)
  #define HEAP_COOKIE_ALLOCATED           (0xDEADBEEF)
#elif (PTR_SIZE == 8)  
  #define HEAP_COOKIE_FREE                (0xCAFED00DFEEDBABE)
  #define HEAP_COOKIE_ALLOCATED           (0xDEADBEEFABACABB0)
#else 
  #error PTR_SIZE invalid!
#endif

//---------------------------------------------------------------------------
#define ROUND_UP(x)                  ((((K_ADDR)x) + (PTR_SIZE-1)) & ~(PTR_SIZE-1))
#define ROUND_DOWN(x)                (((K_ADDR)x) & ~(PTR_SIZE-1))

#define BLOCK_DATA_SIZE(x)           (ROUND_DOWN(x) - sizeof(HeapBlock))

//---------------------------------------------------------------------------
/*!
 * \brief The HeapBlock class
 *
 * This class represents the metadata portion of any allocation, which
 * occurs before the data-portion of the allocation in memory.
 *
 * A heapblock is essentially a multi-dimensional linked-list node, which
 * allows the block to keep track of two things:
 *
 * 1) Its neighboring blocks within the heap
 * 2) The size-list to which the block belongs in the heap, and the
 *    next/previousblocks in that list.
 *
 * HeapBlock objects can be split - that is, a larger block can be split into
 * two smaller blocks, provided the requested block data can be satisfied
 * from the existing object's data.
 *
 * Likewise, HeapBlocks can be coalesced (joined together) when adjacent
 * blocks of memory are freed, allowing for existing blocks to effectivley
 * grow larger.
 *
 * Splitting is used during a typical heap allocation, and coalescing is
 * performed to join blocks together during deallocation/free operations.
 *
 */
class HeapBlock : public LinkListNode
{

public:
    void* operator new (size_t sz, void* pv) { return (HeapBlock*)pv; };
    /*!
     * \brief RootInit
     *
     * Initialize this object as a "root" block.  A root block is a single
     * chunk of memory that can be split into any number of smaller blocks,
     * but can not grow or be coalesced beyond the initial (maximum0 size
     * set here.
     *
     * Initializing a heap block as a root block of 1000 byts would create
     * a single object with an allocatable data size of 1000 bytes, less
     * HeapBlock object metadata.
     *
     * A heap may have many root blocks, depending on its configuration, and
     * the size of the buffer used for the heap.
     *
     * \param usize_ Size of the memory blob (in bytes) that the HeapBlock
     *        object occupies.
     */
    void RootInit( K_ADDR usize_ );

    /*!
     * \brief Split
     *
     * Split the current HeapBlock object into two objects.  The current
     * HeapBlock is resized to usize_ bytes of data, while any remaining
     * slack is allocated to a newly-created object.
     *
     * \param usize_ Size (in bytes) to reserve for the current object,
     *        with the remaining slack allocated towards
     *
     * \return Newly created object.
     */
    HeapBlock *Split( K_ADDR usize_ );

    /*!
     * \brief Coalesce
     *
     * Attempt to join the current block with its neighboring right-side
     * block in contiguous memory.  This operation only succeeds has
     * an effect if the block's right neighbor is non-null, and not
     * allocated.
     */
    void Coalesce( void );

    /*!
     * \brief GetDataPointer
     * \return Pointer to the block's data section
     */
    void *GetDataPointer( void );

    /*!
     * \brief GetDataSize
     * \return Size of the data-section of this block
     */
    K_ADDR GetDataSize( void );

    /*!
     * \brief GetBlockSize
     * \return Size of the block, including data-section and metadata
     */
    K_ADDR GetBlockSize( void );

    /*!
     * \brief SetArenaIndex
     * \param u8List_ Arena-list index to associate this block with
     */
    void SetArenaIndex( uint8_t u8List_ );

    /*!
     * \brief GetArenaIndex
     * \return Return the arena-index associated with this block
     */
    uint8_t GetArenaIndex( void );

    /*!
     * \brief SetCookie
     * \param uCookie_ Cookie used to tag the object's state
     */
    void SetCookie( K_ADDR uCookie_ )
    {
        m_uCookie = uCookie_;
    }

    /*!
     * \brief GetCookie
     * \return Return the object's current state-cookie
     */
    K_ADDR GetCookie( void )
    {
        return m_uCookie;
    }

    /*!
     * \brief GetLeftSibling
     * \return Pointer to the the HeapBlock object immediately
     *         preceding this object in memory, or null if this
     *         is the leftmost block in the heap.
     */
    HeapBlock *GetLeftSibling( void )
    {
        return m_pclLeft;
    }

    /*!
     * \brief GetRightSibling
     * \return Pointer to the the HeapBlock object immediately
     *         following this object in memory, or null if this
     *         is the rightmost block in the heap.
     */
    HeapBlock *GetRightSibling( void )
    {
        return m_pclRight;
    }

    /*!
     * \brief SetRightSibling
     * \param pclRight_ Pointer to the HeapBlock that immediately
     *        follows this object in memory.
     */
    void SetRightSibling( HeapBlock *pclRight_ )
    {
        m_pclRight = pclRight_;
    }

    /*!
     * \brief SetLeftSibling
     * \param pclRight_ Pointer to the HeapBlock that immediately
     *        precedes this object in memory.
     */
    void SetLeftSibling( HeapBlock * pclLeft_ )
    {
        m_pclLeft = pclLeft_;
    }

private:

    /*!
     * \brief Init
     *
     * used as a pseudo-constructor function, as we create HeapBlocks
     * dynamically from an existing, unmanaged blob of memory.  We could
     * also use a constructor and placement-new, but this is just simpler.
     */
    void Init(void)
    {
        m_uDataSize = 0;
        m_u8ArenaIndex = 0;
        m_uCookie = 0;
        m_pclRight = 0;
        m_pclLeft = 0;
        LinkListNode::ClearNode();
    }

    /*!
     * \brief SetDataSize
     *
     * Set the data-size of this HeapBlock object (in bytes) to the
     * specified value.
     *
     * \param uBlockSize_ Size of the data portion of this allocatable
     *        object (in bytes).
     */
    void SetDataSize( K_ADDR uBlockSize_ );

    K_ADDR   m_uDataSize;
    K_ADDR   m_uCookie;

    uint8_t   m_u8ArenaIndex;

    HeapBlock *m_pclRight;
    HeapBlock *m_pclLeft;

};

#endif
