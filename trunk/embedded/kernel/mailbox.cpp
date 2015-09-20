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

    \file   mailbox.cpp

    \brief  Mailbox + Envelope IPC mechanism
*/

#include "mark3cfg.h"
#include "kerneltypes.h"
#include "ksemaphore.h"
#include "kerneldebug.h"
#include "mailbox.h"

#if KERNEL_USE_MAILBOX

//---------------------------------------------------------------------------
void Mailbox::Init( void *pvBuffer_, uint16_t u16BufferSize_, uint16_t uselementSize_ )
{
    KERNEL_ASSERT(u16BufferSize_);
    KERNEL_ASSERT(u16ElementSize_);
    KERNEL_ASSERT(pvBuffer_);

    m_pvBuffer = pvBuffer_;
    m_uselementSize = uselementSize_;

    m_u16Count = (u16BufferSize_ / uselementSize_);
    m_u16Free = m_u16Count;

    m_u16Head = 0;
    m_u16Tail = 0;

    // We use the counting semaphore to implement blocking - with one element
    // in the mailbox corresponding to a post/pend operation in the semaphore.
    m_clRecvSem.Init(0, m_u16Free);

#if KERNEL_USE_TIMEOUTS
    // Binary semaphore is used to track any threads that are blocked on a
    // "send" due to lack of free slots.
    m_clSendSem.Init(0, 1);
#endif
}

//---------------------------------------------------------------------------
void Mailbox::Receive( void *pvData_ )
{
    KERNEL_ASSERT( pvData_ );

#if KERNEL_USE_TIMEOUTS
    Receive_i( pvData_, false, 0 );
#else
    Receive_i( pvData_, false );
#endif
}

#if KERNEL_USE_TIMEOUTS
//---------------------------------------------------------------------------
bool Mailbox::Receive( void *pvData_, uint32_t u32TimeoutMS_ )
{
    KERNEL_ASSERT( pvData_ );
    return Receive_i( pvData_, false, u32TimeoutMS_ );
}
#endif

//---------------------------------------------------------------------------
void Mailbox::ReceiveTail( void *pvData_ )
{
    KERNEL_ASSERT( pvData_ );

#if KERNEL_USE_TIMEOUTS
    Receive_i( pvData_, true, 0 );
#else
    Receive_i( pvData_, true );
#endif
}

#if KERNEL_USE_TIMEOUTS
//---------------------------------------------------------------------------
bool Mailbox::ReceiveTail( void *pvData_, uint32_t u32TimeoutMS_ )
{
    KERNEL_ASSERT( pvData_ );
    return Receive_i( pvData_, true, u32TimeoutMS_ );
}
#endif

//---------------------------------------------------------------------------
bool Mailbox::Send( void *pvData_ )
{
    KERNEL_ASSERT( pvData_ );

#if KERNEL_USE_TIMEOUTS
    return Send_i( pvData_, false, 0 );
#else
    return Send_i( pvData_, false );
#endif
}

//---------------------------------------------------------------------------
bool Mailbox::SendTail( void *pvData_ )
{
    KERNEL_ASSERT( pvData_ );

#if KERNEL_USE_TIMEOUTS
    return Send_i( pvData_, true, 0 );
#else
    return Send_i( pvData_, true );
#endif
}

#if KERNEL_USE_TIMEOUTS
//---------------------------------------------------------------------------
bool Mailbox::Send( void *pvData_, uint32_t u32TimeoutMS_ )
{
    KERNEL_ASSERT( pvData_ );

    return Send_i( pvData_, false, u32TimeoutMS_ );
}

//---------------------------------------------------------------------------
bool Mailbox::SendTail( void *pvData_, uint32_t u32TimeoutMS_ )
{
    KERNEL_ASSERT( pvData_ );

    return Send_i( pvData_, true, u32TimeoutMS_ );
}
#endif

//---------------------------------------------------------------------------
#if KERNEL_USE_TIMEOUTS
bool Mailbox::Send_i( const void *pvData_, bool bTail_, uint32_t u32TimeoutMS_)
#else
bool Mailbox::Send_i( const void *pvData_, bool bTail_)
#endif
{
    const void *pvDst;

    bool bRet = false;
    bool bSchedState = Scheduler::SetScheduler( false );

#if KERNEL_USE_TIMEOUTS
    bool bBlock = false;
    bool bDone = false;
    while (!bDone)
    {
        // Try to claim a slot first before resorting to blocking.
        if (bBlock)
        {
            bDone = true;
            Scheduler::SetScheduler( bSchedState );
            m_clSendSem.Pend( u32TimeoutMS_ );
            Scheduler::SetScheduler( false );
        }
#endif

        CS_ENTER();
        // Ensure we have a free slot before we attempt to write data
        if (m_u16Free)
        {
            m_u16Free--;

            if (bTail_)
            {
                pvDst = GetTailPointer();
                MoveTailBackward();
            }
            else
            {
                MoveHeadForward();
                pvDst = GetHeadPointer();
            }
            bRet = true;
#if KERNEL_USE_TIMEOUTS
            bDone = true;
#endif
        }

#if KERNEL_USE_TIMEOUTS
        else if (u32TimeoutMS_)
        {
            bBlock = true;
        }
        else
        {
            bDone = true;
        }
#endif

        CS_EXIT();

#if KERNEL_USE_TIMEOUTS
    }
#endif

    // Copy data to the claimed slot, and post the counting semaphore
    if (bRet)
    {
        CopyData( pvData_, pvDst, m_uselementSize );
    }

    Scheduler::SetScheduler( bSchedState );

    if (bRet)
    {
        m_clRecvSem.Post();
    }

    return bRet;
}

//---------------------------------------------------------------------------
#if KERNEL_USE_TIMEOUTS
bool Mailbox::Receive_i( const void *pvData_, bool bTail_, uint32_t u32WaitTimeMS_ )
#else
void Mailbox::Receive_i( const void *pvData_, bool bTail_ )
#endif
{
    const void *pvSrc;

#if KERNEL_USE_TIMEOUTS
    if (!m_clRecvSem.Pend( u32WaitTimeMS_ ))
    {
        // Failed to get the notification from the counting semaphore in the
        // time allotted.  Bail.
        return false;
    }    
#else
    m_clRecvSem.Pend();
#endif

    // Disable the scheduler while we do this -- this ensures we don't have
    // multiple concurrent readers off the same queue, which could be problematic
    // if multiple writes occur during reads, etc.
    bool bSchedState = Scheduler::SetScheduler( false );

    // Update the head/tail indexes, and get the associated data pointer for
    // the read operation.
    CS_ENTER();

    m_u16Free++;
    if (bTail_)
    {
        MoveTailForward();
        pvSrc = GetTailPointer();
    }
    else
    {
        pvSrc = GetHeadPointer();
        MoveHeadBackward();
    }

    CS_EXIT();

    CopyData( pvSrc, pvData_, m_uselementSize );

    Scheduler::SetScheduler( bSchedState );

    // Unblock a thread waiting for a free slot to send to
    m_clSendSem.Post();

#if KERNEL_USE_TIMEOUTS
    return true;
#endif
}

#endif
