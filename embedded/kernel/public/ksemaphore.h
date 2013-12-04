/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012 Funkenstein Software Consulting, all rights reserved.
See license.txt for more information
===========================================================================*/
/*!

    \file   ksemaphore.h    

    \brief  Semaphore Blocking Object class declarations

*/

#ifndef __KSEMAPHORE_H__
#define __KSEMAPHORE_H__

#include "kerneltypes.h"
#include "mark3cfg.h"

#include "blocking.h"
#include "threadlist.h"
#include "transaction.h"
#include "atomic.h"

#if KERNEL_USE_SEMAPHORE

//---------------------------------------------------------------------------
/*!
    Counting semaphore, based on BlockingObject base class.
*/
class Semaphore : public BlockingObject
{
public:
    /*!
        \fn void Init(K_USHORT usInitVal_, K_USHORT usMaxVal_)

        Initialize a semaphore before use.  Must be called
        before post/pend operations.
        
        \param usInitVal_ Initial value held by the semaphore
        \param usMaxVal_ Maximum value for the semaphore
    */
    void Init(K_USHORT usInitVal_, K_USHORT usMaxVal_);
    
    /*!
        \fn void Post();
        
        Increment the semaphore count.

        \return true if the semaphore was posted, false if the count
                is already maxed out.
    */
    void Post();
    
    /*!
        \fn void Pend();
        
        Decrement the semaphore count.  If the count is zero,
        the thread will block until the semaphore is pended.        
    */
    void Pend();
    
    /*!
        \fn K_USHORT GetCount()
        
        Return the current semaphore counter. This can be 
        used by a thread to bypass blocking on a semaphore -
        allowing it to do other things until a non-zero count
        is returned, instead of blocking until the semaphore
        is posted.
        
        \return The current semaphore counter value.
    */
    K_USHORT GetCount();
    
#if KERNEL_USE_TIMERS    
    /*!
        \fn bool Pend( K_ULONG ulWaitTimeMS_ );
        
        Decrement the semaphore count.  If the count is zero,
        the thread will block until the semaphore is pended.
        If the specified interval expires before the thread is
        unblocked, then the status is returned back to the user.
        
        \return true - semaphore was acquired before the timeout
                false - timeout occurred before the semaphore was claimed.
    */
    bool Pend( K_ULONG ulWaitTimeMS_);
    
    /*!
        \fn void Timeout(Thread *pclChosenOne_)
        
        Wake a thread blocked on the semaphore.  This is an
        internal function used for implementing timed semaphores
        relying on timer callbacks.  Since these do not have
        access to the private data of the semaphore and its base
        classes, we have to wrap this as a public method - do not
        use this for any other purposes.
    */
    void Timeout(Thread *pclChosenOne_);
    
#endif    
    
private:

    /*!
        \fn K_UCHAR WakeNext();
        
        Wake the next thread waiting on the semaphore.
    */
    K_UCHAR WakeNext();

    /*!
     * \brief ProcessQueue
     *
     * Process all pending actions on the semaphore's transaction queue.
	 * This should only be called from within a context where the blocking 
	 * object's Lock() value has already been called.  When ProcessQueue
	 * returns, the Lock() value will be reset to 0 - as all pending
	 * transactions have been processed.
	 *
	 * \return true - A thread scheduling operation must be performed.
	 *		   false - No rescheduling is required.
     */
    K_BOOL ProcessQueue();

	/*!
	 * \brief PostTransaction
	 *
	 * Perform a semaphore "post" operation, as specified from an object on
	 * the transaction queue.
	 *
	 * \param pclTRX_ - Pointer to the transaction object
	 * \param pbReschedule_ - Pointer to boolean to be set true if rescheduling
	 *		  is required.
	 */
	void PostTransaction(Transaction *pclTRX_, K_BOOL *pbReschedule_);
	
	/*!
	 * \brief PendTransaction
	 *
	 * Perform a semaphore "pend" operation, as specified from an object on
	 * the transaction queue.
	 *
	 * \param pclTRX_ - Pointer to the transaction object
	 * \param pbReschedule_ - Pointer to boolean to be set true if rescheduling
	 *		  is required.
	 */
	void PendTransaction(Transaction *pclTRX_, K_BOOL *pbReschedule_);
	
	/*!
	 * \brief TimeoutTransaction
	 *
	 * Perform a semaphore "timeout" operation, as specified from an object on
	 * the transaction queue.
	 *
	 * \param pclTRX_ - Pointer to the transaction object
	 * \param pbReschedule_ - Pointer to boolean to be set true if rescheduling
	 *		  is required.
	 */
	void TimeoutTransaction(Transaction *pclTRX_, K_BOOL *pbReschedule_);
		
    K_USHORT m_usValue;			//!< Current value in the semaphore
    K_USHORT m_usMaxValue;		//!< Maximum value that the semaphore can hold    
};

#endif //KERNEL_USE_SEMAPHORE

#endif
