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

    @file   blocking.cpp

    @brief  Implementation of base class for blocking objects
*/

#include "mark3.h"

namespace Mark3
{
//---------------------------------------------------------------------------
void BlockingObject::Block(Thread* pclThread_)
{
    KERNEL_ASSERT(nullptr != pclThread_);

    // Remove the thread from its current thread list (the "owner" list)
    // ... And add the thread to this object's block list
    Scheduler::Remove(pclThread_);
    m_clBlockList.Add(pclThread_);

    // Set the "current" list location to the blocklist for this thread
    pclThread_->SetCurrent(&m_clBlockList);
    pclThread_->SetState(ThreadState::Blocked);
}

//---------------------------------------------------------------------------
void BlockingObject::BlockPriority(Thread* pclThread_)
{
    KERNEL_ASSERT(nullptr != pclThread_);

    // Remove the thread from its current thread list (the "owner" list)
    // ... And add the thread to this object's block list
    Scheduler::Remove(pclThread_);
    m_clBlockList.AddPriority(pclThread_);

    // Set the "current" list location to the blocklist for this thread
    pclThread_->SetCurrent(&m_clBlockList);
    pclThread_->SetState(ThreadState::Blocked);
}

//---------------------------------------------------------------------------
void BlockingObject::UnBlock(Thread* pclThread_)
{
    KERNEL_ASSERT(nullptr != pclThread_);

    // Remove the thread from its current thread list (the "owner" list)
    pclThread_->GetCurrent()->Remove(pclThread_);

    // Put the thread back in its active owner's list.  This is usually
    // the ready-queue at the thread's original priority.
    Scheduler::Add(pclThread_);

    // Tag the thread's current list location to its owner
    pclThread_->SetCurrent(pclThread_->GetOwner());
    pclThread_->SetState(ThreadState::Ready);
}
} // namespace Mark3
