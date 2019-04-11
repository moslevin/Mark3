/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012 - 2018 m0slevin, all rights reserved.
See license.txt for more information
===========================================================================*/
/**

    @file   threadlist.cpp

    @brief  Thread linked-list definitions

*/

#include "mark3.h"
namespace Mark3
{
//---------------------------------------------------------------------------
void ThreadList::SetPriority(PORT_PRIO_TYPE uXPriority_)
{
    m_uXPriority = uXPriority_;
}

//---------------------------------------------------------------------------
void ThreadList::SetMapPointer(PriorityMap* pclMap_)
{
    KERNEL_ASSERT(pclMap_ != nullptr);
    m_pclMap = pclMap_;
}

//---------------------------------------------------------------------------
void ThreadList::Add(LinkListNode* node_)
{
    KERNEL_ASSERT(node_ != nullptr);
    CircularLinkList::Add(node_);
    CircularLinkList::PivotForward();

    // We've specified a bitmap for this threadlist
    if (nullptr != m_pclMap) {
        // Set the flag for this priority level
        m_pclMap->Set(m_uXPriority);
    }
}

//---------------------------------------------------------------------------
void ThreadList::AddPriority(LinkListNode* node_)
{
    KERNEL_ASSERT(node_ != nullptr);
    auto* pclCurr = static_cast<Thread*>(GetHead());
    if (nullptr == pclCurr) {
        Add(node_);
        return;
    }
    auto uXHeadPri = pclCurr->GetCurPriority();

    auto* pclTail = static_cast<Thread*>(GetTail());
    auto* pclNode = static_cast<Thread*>(node_);

    // Set the threadlist's priority level, flag pointer, and then add the
    // thread to the threadlist
    auto uXPriority = pclNode->GetCurPriority();
    do {
        if (uXPriority > pclCurr->GetCurPriority()) {
            break;
        }
        pclCurr = static_cast<Thread*>(pclCurr->GetNext());
    } while (pclCurr != pclTail);

    // Insert pclNode before pclCurr in the linked list.
    InsertNodeBefore(pclNode, pclCurr);

    // If the priority is greater than current head, reset
    // the head pointer.
    if (uXPriority > uXHeadPri) {
        m_pclHead = pclNode;
        m_pclTail = m_pclHead->prev;
    } else if (pclNode->GetNext() == m_pclHead) {
        m_pclTail = pclNode;
    }
}

//---------------------------------------------------------------------------
void ThreadList::Add(LinkListNode* node_, PriorityMap* pclMap_, PORT_PRIO_TYPE uXPriority_)
{
    // Set the threadlist's priority level, flag pointer, and then add the
    // thread to the threadlist
    SetPriority(uXPriority_);
    SetMapPointer(pclMap_);
    Add(node_);
}

//---------------------------------------------------------------------------
void ThreadList::Remove(LinkListNode* node_)
{
    // Remove the thread from the list
    CircularLinkList::Remove(node_);

    // If the list is empty...
    if ((nullptr == m_pclHead) && (nullptr != m_pclMap)) {
        // Clear the bit in the bitmap at this priority level
        m_pclMap->Clear(m_uXPriority);
    }
}

//---------------------------------------------------------------------------
Thread* ThreadList::HighestWaiter()
{
    return static_cast<Thread*>(GetHead());
}
} // namespace Mark3
