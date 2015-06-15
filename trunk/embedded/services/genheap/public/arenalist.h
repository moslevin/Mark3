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
class ArenaList : private DoubleLinkList
{
public:
    void Init( uint32_t u32BlockSize_ )
    {
        m_u32Count = 0;
        m_u32BlockSize = u32BlockSize_;

        DoubleLinkList::Init();
    }

    uint32_t GetBlockSize( void )
    {
        return m_u32BlockSize;
    }

    void PushBlock( HeapBlock *pclBlock_ )
    {
        m_u32Count++;
        Add((LinkListNode*)pclBlock_);
    }

    HeapBlock *PopBlock( void )
    {
        m_u32Count--;
        HeapBlock *pclReturn = (HeapBlock*)GetHead();
        if (pclReturn)
        {
            Remove( GetHead( ) );
        }
        return pclReturn;
    }

    void RemoveBlock( HeapBlock *pclBlock_ )
    {
        m_u32Count--;
        Remove((LinkListNode*)pclBlock_);
    }

    uint32_t GetBlockCount( void )
    {
        return m_u32Count;
    }

private:
    uint32_t   m_u32BlockSize;
    uint32_t   m_u32Count;
};

#endif
