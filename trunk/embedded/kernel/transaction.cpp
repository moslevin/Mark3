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

    \file   transaction.cpp

    \brief  Transaction Queue Implementation
*/

#include "transaction.h"

//---------------------------------------------------------------------------
DoubleLinkList TransactionQueue::m_clGlobalQueue;
Transaction    TransactionQueue::m_aclTransactions[TRANSACTION_QUEUE_SIZE];

//---------------------------------------------------------------------------
void TransactionQueue::GlobalQueueInit()
{
	for (K_UCHAR i = 0; i < TRANSACTION_QUEUE_SIZE; i++)
	{
		m_clGlobalQueue.Add(&m_aclTransactions[i]);
	}
}

//---------------------------------------------------------------------------
void TransactionQueue::Enqueue( K_USHORT usData_, void *pvData_)
{
	Transaction *pclTrx;

	CS_ENTER();
	pclTrx = static_cast<Transaction*>(m_clGlobalQueue.GetHead());

	KERNEL_ASSERT(pclTrx);

	m_clGlobalQueue.Remove(pclTrx);
	CS_EXIT();

	pclTrx->Set( usData_, pvData_ );
	Add(pclTrx);
}

//---------------------------------------------------------------------------
Transaction *TransactionQueue::Dequeue()
{
	Transaction *pclTrx;

	CS_ENTER();
	pclTrx = static_cast<Transaction*>(GetHead());

	KERNEL_ASSERT(pclTrx);

	Remove(pclTrx);
	CS_EXIT();

	return pclTrx;
}

//---------------------------------------------------------------------------
void TransactionQueue::Finish( Transaction *pclTransaction_ )
{
	CS_ENTER();
	m_clGlobalQueue.Add(pclTransaction_);
	CS_EXIT();
}

