/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2013 Funkenstein Software Consulting, all rights reserved.
See license.txt for more information
===========================================================================*/
/*!

    \file   transaction.h

    \brief  Transaction Queue Implementation
*/

#ifndef __TRANSACTION_H__
#define __TRANSACTION_H__

#include "kerneltypes.h"
#include "mark3cfg.h"
#include "threadport.h"
#include "ll.h"
#include "kernel_debug.h"

//---------------------------------------------------------------------------
#define TRANSACTION_QUEUE_SIZE          (8)

//---------------------------------------------------------------------------
/*!
 * \brief The Transaction class
 *
 * The Transaction class implements "kernel transaction" functionality used by
 * blocking objects within the kernel.
 *
 * Each Transaction object is essentially a FIFO node, which is used to
 * represent an operation that takes place on a blocking object.  These
 * operations include things like posting or pending a semaphore, claiming or
 * releasing a mutex, or a thread timeout on a blocking object.  Transactions
 * are used exclusively with TransactionQueue's to serialize access to blocking
 * objects in order to implement lockless kernel operations with interrupts enabled.
 *
 * For simplicity, each transaction is implemented as a simple Key/Value
 * pair - the "Code" value is interpreted differently based on the type of
 * blocking object, and the "Data" value is depending on the value held in the
 * code.  For examples of how Transactions are used, see the kernel, mutex and
 * event-flag code.
 *
 */
class Transaction : public LinkListNode
{
public:
    /*!
     * \brief Set Provide access to set the code/data fields in the object
     * \param usCode_ Code value to set
     * \param pvData_ Abstract data value to set
     */
    void Set( K_USHORT usCode_, void *pvData_ )
    {
        m_usCode = usCode_;
        m_pvData = pvData_;
    }

    /*!
     * \brief GetCode Return the value held by the Code field
     * \return value of the Code field
     */
    K_USHORT GetCode()
    {
        return m_usCode;
    }

    /*!
     * \brief GetData Return the abstract data value held in the object
     * \return Abstract data value held in the object
     */
    void *GetData()
    {
        return m_pvData;
    }

private:
    K_USHORT    m_usCode;       //!< Data code, defined by the blocking object using transactions
    void        *m_pvData;      //!< Abstract data, which is defined by the code
};

//---------------------------------------------------------------------------
/*!
 * \brief The TransactionQueue class
 *
 *
 * A kernel transaction queue is a construct used to build blocking objects
 * which disable interrupts for as short a period of time as possible. Instead
 * of disabling interrupts for the duration of a blocking object operation
 * (i.e. mutex claim or semaphore post), we instead serialize access to the
 * object using a FIFO containing a list of pending actions, Coupled with
 * Atomic locking operations, the
 * kernel can guarantee that only one thread has permission to process the
 * object's transaction queue, while all other concurrent threads/interrupts
 * (which then fail to claim the object's lock) are only allowed to add
 * transactions to it.  In this way, we can keep interrupts enabled for
 * the vast majority of kernel/blocking-object calls, resulting in a
 * much more deterministic, responsive system.
 *
 * Transactions are very short-lived - i.e. a queue will only have more than
 * 1 pending transaction if pre-empted by interrupts during queue processing
 * within a kernel call.  As a result, we maintain a small, global pool of
 * transaction objects which are allocated as-necessary in order to service
 * demand.  These Transaction objects are shared among all blocking objects
 * within the system.
 *
 */
class TransactionQueue : public DoubleLinkList
{
public:
    /*!
     * \brief GlobalQueueInit
     *
     * This static method is called to initialize the global transaction
     * pool and its included transaction objects.
     *
     */
    static void GlobalQueueInit();

    /*!
     * \brief Enqueue
     * \param usData_
     * \param pvData_
     */
    void Enqueue( K_USHORT usData_, void *pvData_);
    
    /*!
     * \brief Dequeue
     * \return
     */
    Transaction *Dequeue();
        
    /*!
     * \brief Finish
     * \param pclTransaction_
     */
    void Finish( Transaction *pclTransaction_ );
    
private:

    static DoubleLinkList m_clGlobalQueue;
    static Transaction    m_aclTransactions[TRANSACTION_QUEUE_SIZE];
};

#endif
