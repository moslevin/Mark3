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

#ifndef __ARENA_H__
#define __ARENA_H__

#include <stdint.h>
#include "arenalist.h"
#include "heapblock.h"

//---------------------------------------------------------------------------
#define ARENA_EXHAUSTED         (255)
#define ARENA_FULL              (254)

//---------------------------------------------------------------------------
/*!
 * \brief The Arena class
 *
 * This implements a heap composed of a blob of contiguous memory, managed
 * in a series of lists, where each list corresponds to a minimum allocation
 * size for blocks within the list.
 *
 * As a general-purpose heap, it offers basic "malloc/free" style dynamic
 * memory allocation, with few bells or whistles.
 *
 */
class Arena
{
public:
    /*!
     * \brief Init
     *
     * Initialize the arena prior to use.
     *
     * \param pvBuffer_ Pointer to the memory blob to manage as a heap
     *                  from this object.
     * \param usize_ Size of the heap memory blob in bytes
     * \return
     */
    void Init( void *pvBuffer_, K_ADDR u32Size_, K_ADDR *au32Sizes_, uint8_t u8NumSizes_ );

    /*!
     * \brief Allocate
     *
     * Allocate a block of dynamic memory from the heap.
     *
     * \param usize_ Size of object to allocate (in bytes)
     * \return pointer to a chunk of dynamic memory, or 0 on exhaustion.
     */
    void *Allocate( K_ADDR usize_ );

    /*!
     * \brief Free
     *
     * Free the block of memory, returning it back to the pool for use.
     *
     * \param pvBlock_ Pointer to the beginning of the object to be freed.
     */
    void Free( void *pvBlock_ );

    /*!
     * \brief Print
     *
     * Show details about the print via standard output.
     */
    void Print( void );
private:

    /*!
     * \brief ListForSize
     *
     * Determine the arena list with the smallest allocation size
     * to handle an allocation of a given size.
     *
     * \param usize_ Size of the object to check
     * \return INdex representing the arena/arena-size
     */
    uint8_t ListForSize( K_ADDR usize_ );

    /*!
     * \brief ListToSatisfy
     *
     * Determine the arena list that can satisfy the size request,
     * and has vacant objects available to be allocated.
     *
     * \param usize_ Size of data to check
     * \return Index representing the arena/arena-size, or 0xF...F on invalid
     */
    uint8_t ListToSatisfy( K_ADDR usize_ );

    uint8_t    m_u8LargestList;    //!< Index of the largest arena
    ArenaList  *m_aclBlockList;    //!< Arena linked-list data
    void       *m_pvData;          //!< Pointer to the raw memory blob managed by this object as a heap.
};

#endif

