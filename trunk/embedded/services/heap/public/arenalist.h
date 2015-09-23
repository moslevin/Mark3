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

    \file   arenalist.h

    \brief  Data structure used to manage list of available heap blocks.
*/

#ifndef __ARENALIST_H__
#define __ARENALIST_H__

#include <stdint.h>
#include "heapblock.h"
#include "ll.h"

//---------------------------------------------------------------------------
/*!
 * \brief The ArenaList class
 *
 * This clas keeps track of a doubly-linked-list of HeapBlock objects which
 * all have a size greater than the initialized value.  In this way, when
 * a memory allocator requests a block of a given size, we can guarantee
 * that any block in the list (i.e. the first block in the list) will satisfy
 * the requirement without having to search through the list iteratively.
 *
 */
class ArenaList : private DoubleLinkList
{
public:
    /*!
     * \brief Init
     *
     * Initialize the member data and objects inherited by this class
     *
     * \param uBlockSize_ Minimum data size for blocks.
     */
    void Init( K_ADDR uBlockSize_ )
    {
        m_u16Count = 0;
        m_uBlockSize = uBlockSize_;

        DoubleLinkList::Init();
    }

    /*!
     * \brief GetBlockSize
     * \return The minimum block size for objects in this structure
     */
    K_ADDR GetBlockSize( void )
    {
        return m_uBlockSize;
    }

    /*!
     * \brief PushBlock
     *
     * Add a vacant HeapBlock to the beginning of the list
     *
     * \param pclBlock_ Pointer of the block to add
     */
    void PushBlock( HeapBlock *pclBlock_ )
    {
        if (m_u16Count != 65535)
        {
            m_u16Count++;
            Add((LinkListNode*)pclBlock_);
        }
    }

    /*!
     * \brief PopBlock
     *
     * Remove the first object in the list, and return its pointer to
     * the caller
     *
     * \return Pointer to the first HeapBlock object in the list, or
     *         0 on error (list exhausted).
     */
    HeapBlock *PopBlock( void )
    {
        if (m_u16Count)
        {
            m_u16Count--;
            HeapBlock *pclReturn = (HeapBlock*)GetHead();
            if (pclReturn)
            {
                Remove( GetHead( ) );
            }
            return pclReturn;
        }
        return 0;
    }

    /*!
     * \brief RemoveBlock
     *
     * Remove a given object (which exists in the list) from the list.
     *
     * \param pclBlock_ Pointer of the block to remove
     */
    void RemoveBlock( HeapBlock *pclBlock_ )
    {
        if (m_u16Count && GetHead( ) )
        {
            m_u16Count--;
            Remove((LinkListNode*)pclBlock_);
        }
    }

    /*!
     * \brief GetBlockCount
     * \return The current number of available allocations in this list.
     */
    uint32_t GetBlockCount( void )
    {
        return m_u16Count;
    }

private:
    K_ADDR    m_uBlockSize;    //!< The minimum data-size for blocks held in this arena
    uint16_t   m_u16Count;      //!< Current number of available blocks in this list
};

#endif
