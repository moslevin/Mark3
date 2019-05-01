/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012 - 2019 m0slevin, all rights reserved.
See license.txt for more information
=========================================================================== */
/**

    @file   threadlistlist.h

    @brief  Class implementing a doubly-linked list of thread lists

 */
#pragma once

#include "mark3.h"

//---------------------------------------------------------------------------
namespace Mark3 {
class ThreadList;

//---------------------------------------------------------------------------
/**
 * @brief The ThreadListList class
 */
class ThreadListList {
public:
    /**
     * @brief Add
     * @param pclThreadList_
     */
    static void Add(ThreadList* pclThreadList_)
    {
        m_clThreadListList.Add(reinterpret_cast<LinkListNode*>(pclThreadList_));
    }

    /**
     * @brief Remove
     * @param pclThreadList_
     */
    static void Remove(ThreadList* pclThreadList_)
    {
        m_clThreadListList.Remove(reinterpret_cast<LinkListNode*>(pclThreadList_));
    }

    /**
     * @brief GetHead
     * @return
     */
    static ThreadList* GetHead()
    {
        return reinterpret_cast<ThreadList*>(m_clThreadListList.GetHead());
    }

private:
    static DoubleLinkList m_clThreadListList;
};
} // namespace Mark3
