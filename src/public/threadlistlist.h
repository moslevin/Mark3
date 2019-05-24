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
 *
 * Class used to track all threadlists active in the OS kernel.  At any point
 * in time, the list can be traversed to get a complete view of all running,
 * blocked, or stopped threads in the system.
 */
class ThreadListList {
public:
    /**
     * @brief Add
     * Add a ThreadList to the list for tracking
     * @param pclThreadList_ threadlist to add for tracking
     */
    static void Add(ThreadList* pclThreadList_)
    {
        m_clThreadListList.Add(pclThreadList_);
    }

    /**
     * @brief Remove
     * Remove a threadlist from tracking
     * @param pclThreadList_ threadlist to remove from tracking
     */
    static void Remove(ThreadList* pclThreadList_)
    {
        m_clThreadListList.Remove(pclThreadList_);
    }

    /**
     * @brief GetHead
     * @return The threadlist at the beginning of the list
     */
    static ThreadList* GetHead()
    {
        return m_clThreadListList.GetHead();
    }

private:
    static TypedDoubleLinkList<ThreadList> m_clThreadListList;
};
} // namespace Mark3
