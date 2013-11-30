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
class Transaction : public LinkListNode
{
public:
    void Set( K_USHORT usCode_, void *pvData_ )
    {
        m_usCode = usCode_;
        m_pvData = pvData_;
    }

    K_USHORT GetCode()
    {
        return m_usCode;
    }

    void *GetData()
    {
        return m_pvData;
    }

private:
    K_USHORT    m_usCode;
    void        *m_pvData;
};

//---------------------------------------------------------------------------
class TransactionQueue : public DoubleLinkList
{
public:
    static void GlobalQueueInit();

    void Enqueue( K_USHORT usData_, void *pvData_);
    
    Transaction *Dequeue();
    
    void Finish( Transaction *pclTransaction_ );
    
private:

    static DoubleLinkList m_clGlobalQueue;
    static Transaction    m_aclTransactions[TRANSACTION_QUEUE_SIZE];
};

#endif
