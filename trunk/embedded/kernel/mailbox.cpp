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
void Mailbox::Init( void *pvBuffer_, K_USHORT usBufferSize_, K_USHORT usElementSize_ )
{
    KERNEL_ASSERT(usBufferSize_);
    KERNEL_ASSERT(usElementSize_);
    KERNEL_ASSERT(pvBuffer_);

    m_pvBuffer = pvBuffer_;
    m_usElementSize = usElementSize_;

    m_usCount = (usBufferSize_ / usElementSize_);
    m_usFree = m_usCount;

    m_usHead = 0;
    m_usTail = 0;

    // We use the counting semaphore to implement blocking - with one element
    // in the mailbox corresponding to a post/pend operation in the semaphore.
    m_clSem.Init(0, m_usFree);
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
bool Mailbox::Receive( void *pvData_, K_ULONG ulTimeoutMS_ )
{
    KERNEL_ASSERT( pvData_ );
    return Receive_i( pvData_, false, 0 );
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
bool Mailbox::ReceiveTail( void *pvData_, K_ULONG ulTimeoutMS_ )
{
    KERNEL_ASSERT( pvData_ );
    return Receive_i( pvData_, true, 0 );
}
#endif

//---------------------------------------------------------------------------
bool Mailbox::Send( void *pvData_ )
{
    KERNEL_ASSERT( pvData_ );

    return Send_i( pvData_, false );
}

//---------------------------------------------------------------------------
bool Mailbox::SendTail( void *pvData_ )
{
    KERNEL_ASSERT( pvData_ );

    return Send_i( pvData_, true );
}

//---------------------------------------------------------------------------
bool Mailbox::Send_i( const void *pvData_, bool bTail_)
{
    const void *pvDst;

    bool bRet = false;
    bool bSchedState = Scheduler::SetScheduler( false );

    CS_ENTER();

    // Ensure we have a free slot before we attempt to write data
    if (m_usFree)
    {
        m_usFree--;

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
    }

    CS_EXIT();

    // Copy data to the claimed slot, and post the counting semaphore
    if (bRet)
    {
        CopyData( pvData_, pvDst, m_usElementSize );
    }

    Scheduler::SetScheduler( bSchedState );

    if (bRet)
    {
        m_clSem.Post();
    }

    return bRet;
}

//---------------------------------------------------------------------------
#if KERNEL_USE_TIMEOUTS
bool Mailbox::Receive_i( const void *pvData_, bool bTail_, K_ULONG ulWaitTimeMS_ )
#else
void Mailbox::Receive_i( const void *pvData_, bool bTail_ )
#endif
{
    const void *pvSrc;

#if KERNEL_USE_TIMEOUTS
    if (!m_clSem.Pend( ulWaitTimeMS_ ))
    {
        // Failed to get the notification from the counting semaphore in the
        // time allotted.  Bail.
        return false;
    }
#else
    m_clSem.Pend();
#endif

    // Disable the scheduler while we do this -- this ensures we don't have
    // multiple concurrent readers off the same queue, which could be problematic
    // if multiple writes occur during reads, etc.
    bool bSchedState = Scheduler::SetScheduler( false );

    // Update the head/tail indexes, and get the associated data pointer for
    // the read operation.
    CS_ENTER();

    m_usFree++;
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

    CopyData( pvSrc, pvData_, m_usElementSize );

    Scheduler::SetScheduler( bSchedState );

#if KERNEL_USE_TIMEOUTS
    return true;
#endif
}

#endif
