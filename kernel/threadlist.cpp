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
===========================================================================*/
/*!

    \file   threadlist.cpp

    \brief  Thread linked-list definitions

*/

#include "kerneltypes.h"
#include "ll.h"
#include "threadlist.h"
#include "thread.h"

#define _CAN_HAS_DEBUG
//--[Autogenerated - Do Not Modify]------------------------------------------
#include "dbg_file_list.h"
#include "buffalogger.h"
#if defined(DBG_FILE)
#error "Debug logging file token already defined!  Bailing."
#else
#define DBG_FILE _DBG___KERNEL_THREADLIST_CPP
#endif
//--[End Autogenerated content]----------------------------------------------
#include "kerneldebug.h"

//---------------------------------------------------------------------------
void ThreadList::SetPriority(PORT_PRIO_TYPE uXPriority_)
{
    m_uXPriority = uXPriority_;
}

//---------------------------------------------------------------------------
void ThreadList::SetMapPointer(PriorityMap* pclMap_)
{
    m_pclMap = pclMap_;
}

//---------------------------------------------------------------------------
void ThreadList::Add(LinkListNode* node_)
{
    CircularLinkList::Add(node_);
    CircularLinkList::PivotForward();

    // We've specified a bitmap for this threadlist
    if (m_pclMap) {
        // Set the flag for this priority level
        m_pclMap->Set(m_uXPriority);
    }
}

//---------------------------------------------------------------------------
void ThreadList::AddPriority(LinkListNode* node_)
{
    Thread* pclCurr = static_cast<Thread*>(GetHead());
    if (!pclCurr) {
        Add(node_);
        return;
    }
    PORT_PRIO_TYPE uXHeadPri = pclCurr->GetCurPriority();

    Thread* pclTail = static_cast<Thread*>(GetTail());
    Thread* pclNode = static_cast<Thread*>(node_);

    // Set the threadlist's priority level, flag pointer, and then add the
    // thread to the threadlist
    PORT_PRIO_TYPE uXPriority = pclNode->GetCurPriority();
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
        m_pstHead = pclNode;
        m_pstTail = m_pstHead->prev;
    } else if (pclNode->GetNext() == m_pstHead) {
        m_pstTail = pclNode;
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
    if (!m_pstHead && m_pclMap) {
        // Clear the bit in the bitmap at this priority level
        m_pclMap->Clear(m_uXPriority);
    }
}

//---------------------------------------------------------------------------
Thread* ThreadList::HighestWaiter()
{
    return static_cast<Thread*>(GetHead());
}
