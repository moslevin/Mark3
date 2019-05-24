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

    @file   message.cpp

    @brief  Inter-thread communications via message passing

*/

#include "mark3.h"
namespace Mark3
{
//---------------------------------------------------------------------------
void MessagePool::Init()
{
    m_clList.Init();
}

//---------------------------------------------------------------------------
void MessagePool::Push(Message* pclMessage_)
{
    KERNEL_ASSERT(pclMessage_);

    const auto cs = CriticalGuard{};
    m_clList.Add(pclMessage_);
}

//---------------------------------------------------------------------------
Message* MessagePool::Pop()
{
    const auto cs = CriticalGuard{};
    auto* pclRet = m_clList.GetHead();
    if (nullptr != pclRet) {
        m_clList.Remove(pclRet);
    }
    return pclRet;
}

//------------------------------------------------------------------------
Message* MessagePool::GetHead()
{
    return m_clList.GetHead();
}

//---------------------------------------------------------------------------
void MessageQueue::Init()
{
    m_clSemaphore.Init(0, 255);
}

//---------------------------------------------------------------------------
Message* MessageQueue::Receive()
{
    return Receive_i(0);
}

//---------------------------------------------------------------------------
Message* MessageQueue::Receive(uint32_t u32TimeWaitMS_)
{
    return Receive_i(u32TimeWaitMS_);
}

//---------------------------------------------------------------------------
Message* MessageQueue::Receive_i(uint32_t u32TimeWaitMS_)
{
    // Block the current thread on the counting semaphore
    if (!m_clSemaphore.Pend(u32TimeWaitMS_)) {
        return nullptr;
    }

    const auto cs = CriticalGuard{};
    // Pop the head of the message queue and return it
    auto* pclRet = m_clLinkList.GetHead();
    m_clLinkList.Remove(pclRet);

    return pclRet;
}

//---------------------------------------------------------------------------
void MessageQueue::Send(Message* pclSrc_)
{
    KERNEL_ASSERT(pclSrc_);

    CriticalSection::Enter();

    // Add the message to the head of the linked list
    m_clLinkList.Add(pclSrc_);

    CriticalSection::Exit();

    // Post the semaphore, waking the blocking thread for the queue.
    m_clSemaphore.Post();
}

//---------------------------------------------------------------------------
uint16_t MessageQueue::GetCount()
{
    return m_clSemaphore.GetCount();
}
} // namespace Mark3
