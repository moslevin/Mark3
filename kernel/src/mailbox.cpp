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

    @file   mailbox.cpp

    @brief  Mailbox + Envelope IPC mechanism
*/

#include "mark3.h"
namespace Mark3
{
//---------------------------------------------------------------------------
Mailbox::~Mailbox()
{
    // If the mailbox isn't empty on destruction, kernel panic.
    if (m_u16Free != m_u16Count) {
        Kernel::Panic(PANIC_ACTIVE_MAILBOX_DESCOPED);
    }
}

//---------------------------------------------------------------------------
void Mailbox::Init(void* pvBuffer_, uint16_t u16BufferSize_, uint16_t u16ElementSize_)
{
    KERNEL_ASSERT(u16BufferSize_);
    KERNEL_ASSERT(u16ElementSize_);
    KERNEL_ASSERT(pvBuffer_ != nullptr);

    m_pvBuffer       = pvBuffer_;
    m_u16ElementSize = u16ElementSize_;

    m_u16Count = (u16BufferSize_ / u16ElementSize_);
    m_u16Free  = m_u16Count;

    m_u16Head = 0;
    m_u16Tail = 0;

    // We use the counting semaphore to implement blocking - with one element
    // in the mailbox corresponding to a post/pend operation in the semaphore.
    m_clRecvSem.Init(0, m_u16Free);

    // Binary semaphore is used to track any threads that are blocked on a
    // "send" due to lack of free slots.
    m_clSendSem.Init(0, 1);
}

//---------------------------------------------------------------------------
Mailbox* Mailbox::Init(uint16_t u16BufferSize_, uint16_t u16ElementSize_)
{
    KERNEL_ASSERT(u16BufferSize_);
    KERNEL_ASSERT(u16ElementSize_);

    auto* pclNew   = AutoAlloc::NewObject<Mailbox, AutoAllocType::MailBox>();
    auto* pvBuffer = AutoAlloc::NewRawData(u16BufferSize_);

    KERNEL_ASSERT(pclNew != nullptr);
    KERNEL_ASSERT(pvBuffer != nullptr);

    if (!pclNew) {
        return nullptr;
    }
    if (!pvBuffer) {
        AutoAlloc::DestroyObject<Mailbox, AutoAllocType::MailBox>(pclNew);
        return nullptr;
    }

    pclNew->Init(pvBuffer, u16BufferSize_, u16ElementSize_);
    return pclNew;
}

//---------------------------------------------------------------------------
void Mailbox::Receive(void* pvData_)
{
    KERNEL_ASSERT(pvData_ != nullptr);
    Receive_i(pvData_, false, 0);
}

//---------------------------------------------------------------------------
bool Mailbox::Receive(void* pvData_, uint32_t u32TimeoutMS_)
{
    KERNEL_ASSERT(pvData_ != nullptr);
    return Receive_i(pvData_, false, u32TimeoutMS_);
}

//---------------------------------------------------------------------------
void Mailbox::ReceiveTail(void* pvData_)
{
    KERNEL_ASSERT(pvData_ != nullptr);
    Receive_i(pvData_, true, 0);
}

//---------------------------------------------------------------------------
bool Mailbox::ReceiveTail(void* pvData_, uint32_t u32TimeoutMS_)
{
    KERNEL_ASSERT(pvData_ != nullptr);
    return Receive_i(pvData_, true, u32TimeoutMS_);
}

//---------------------------------------------------------------------------
bool Mailbox::Send(void* pvData_)
{
    KERNEL_ASSERT(pvData_ != nullptr);
    return Send_i(pvData_, false, 0);
}

//---------------------------------------------------------------------------
bool Mailbox::SendTail(void* pvData_)
{
    KERNEL_ASSERT(pvData_ != nullptr);
    return Send_i(pvData_, true, 0);
}

//---------------------------------------------------------------------------
bool Mailbox::Send(void* pvData_, uint32_t u32TimeoutMS_)
{
    KERNEL_ASSERT(pvData_ != nullptr);
    return Send_i(pvData_, false, u32TimeoutMS_);
}

//---------------------------------------------------------------------------
bool Mailbox::SendTail(void* pvData_, uint32_t u32TimeoutMS_)
{
    KERNEL_ASSERT(pvData_ != nullptr);
    return Send_i(pvData_, true, u32TimeoutMS_);
}

//---------------------------------------------------------------------------
bool Mailbox::Send_i(const void* pvData_, bool bTail_, uint32_t u32TimeoutMS_)
{
    KERNEL_ASSERT(pvData_ != nullptr);

    const void* pvDst = nullptr;

    auto bRet        = false;
    auto bSchedState = Scheduler::SetScheduler(false);
    auto bBlock      = false;
    auto bDone       = false;

    while (!bDone) {
        // Try to claim a slot first before resorting to blocking.
        if (bBlock) {
            bDone = true;
            Scheduler::SetScheduler(bSchedState);
            m_clSendSem.Pend(u32TimeoutMS_);
            Scheduler::SetScheduler(false);
        }

        CS_ENTER();
        // Ensure we have a free slot before we attempt to write data
        if (m_u16Free != 0u) {
            m_u16Free--;

            if (bTail_) {
                pvDst = GetTailPointer();
                MoveTailBackward();
            } else {
                MoveHeadForward();
                pvDst = GetHeadPointer();
            }
            bRet  = true;
            bDone = true;
        } else if (u32TimeoutMS_ != 0u) {
            bBlock = true;
        } else {
            bDone = true;
        }

        CS_EXIT();
    }

    // Copy data to the claimed slot, and post the counting semaphore
    if (bRet) {
        CopyData(pvData_, pvDst, m_u16ElementSize);
    }

    Scheduler::SetScheduler(bSchedState);

    if (bRet) {
        m_clRecvSem.Post();
    }

    return bRet;
}

//---------------------------------------------------------------------------
bool Mailbox::Receive_i(const void* pvData_, bool bTail_, uint32_t u32WaitTimeMS_)
{
    KERNEL_ASSERT(pvData_ != nullptr);
    const void* pvSrc;

    if (!m_clRecvSem.Pend(u32WaitTimeMS_)) {
        // Failed to get the notification from the counting semaphore in the
        // time allotted.  Bail.
        return false;
    }

    // Disable the scheduler while we do this -- this ensures we don't have
    // multiple concurrent readers off the same queue, which could be problematic
    // if multiple writes occur during reads, etc.
    auto bSchedState = Scheduler::SetScheduler(false);

    // Update the head/tail indexes, and get the associated data pointer for
    // the read operation.
    CS_ENTER();

    m_u16Free++;
    if (bTail_) {
        MoveTailForward();
        pvSrc = GetTailPointer();
    } else {
        pvSrc = GetHeadPointer();
        MoveHeadBackward();
    }

    CS_EXIT();

    KERNEL_ASSERT(pvSrc);
    CopyData(pvSrc, pvData_, m_u16ElementSize);

    Scheduler::SetScheduler(bSchedState);

    // Unblock a thread waiting for a free slot to send to
    m_clSendSem.Post();

    return true;
}
} // namespace Mark3
