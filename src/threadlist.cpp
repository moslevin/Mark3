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
#include "threadlistlist.h"
namespace Mark3
{
//---------------------------------------------------------------------------
ThreadList::ThreadList()
    : m_uXPriority(0)
    , m_pclMap(nullptr)
{
}

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
void ThreadList::Add(Thread* pclThread_)
{
    KERNEL_ASSERT(pclThread_ != nullptr);

    // If list was empty, add the object for global threadlist tracking
    if (!GetHead()) {
        ThreadListList::Add(this);
    }
    TypedCircularLinkList<Thread>::Add(pclThread_);
    PivotForward();

    // We've specified a bitmap for this threadlist
    if (nullptr != m_pclMap) {
        // Set the flag for this priority level
        m_pclMap->Set(m_uXPriority);
    }
}

//---------------------------------------------------------------------------
void ThreadList::AddPriority(Thread* pclThread_)
{
    KERNEL_ASSERT(node_ != nullptr);
    auto* pclCurr = GetHead();
    if (nullptr == pclCurr) {
        Add(pclThread_);
        return;
    }
    auto uXHeadPri = pclCurr->GetCurPriority();
    auto* pclTail = GetTail();

    // Set the threadlist's priority level, flag pointer, and then add the
    // thread to the threadlist
    auto uXPriority = pclThread_->GetCurPriority();
    do {
        if (uXPriority > pclCurr->GetCurPriority()) {
            break;
        }
        pclCurr = pclCurr->GetNext();
    } while (pclCurr != pclTail);

    // Insert pclNode before pclCurr in the linked list.
    InsertNodeBefore(pclThread_, pclCurr);

    // If the priority is greater than current head, reset
    // the head pointer.
    if (uXPriority > uXHeadPri) {
        SetHead(pclThread_);
        SetTail(GetHead()->GetPrev());
    } else if (pclThread_->GetNext() == GetHead()) {
        SetTail(pclThread_);
    }
}

//---------------------------------------------------------------------------
void ThreadList::Add(Thread* node_, PriorityMap* pclMap_, PORT_PRIO_TYPE uXPriority_)
{
    // Set the threadlist's priority level, flag pointer, and then add the
    // thread to the threadlist
    SetPriority(uXPriority_);
    SetMapPointer(pclMap_);
    Add(node_);
}

//---------------------------------------------------------------------------
void ThreadList::Remove(Thread* node_)
{
    // Remove the thread from the list
    TypedCircularLinkList<Thread>::Remove(node_);

    // If the list is empty...
    if (nullptr == GetHead()) {
        // No more threads - remove this object from global threadlist tracking
        ThreadListList::Remove(this);
        if (nullptr != m_pclMap) {
            // Clear the bit in the bitmap at this priority level
            m_pclMap->Clear(m_uXPriority);
        }
    }
}

//---------------------------------------------------------------------------
Thread* ThreadList::HighestWaiter()
{
    return GetHead();
}
} // namespace Mark3
