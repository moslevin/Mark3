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

    \file   mutex.h    

    \brief  Mutual exclusion class declaration
    
    Resource locks are implemented using mutual exclusion semaphores (Mutex_t).
    Protected blocks can be placed around any resource that may only be accessed
    by one thread at a time.  If additional threads attempt to access the
    protected resource, they will be placed in a wait queue until the resource
    becomes available.  When the resource becomes available, the thread with
    the highest original priority claims the resource and is activated.
    Priority inheritance is included in the implementation to prevent priority
    inversion.  Always ensure that you claim and release your mutex objects
    consistently, otherwise you may end up with a deadlock scenario that's
    hard to debug.

    \section MInit Initializing

    Initializing a mutex object by calling:

    \code
    clMutex.Init();
    \endcode

    \section MUsage Resource protection example

    \code
    clMutex.Claim();
    ...
    <resource protected block>
    ...
    clMutex.Release();
    \endcode

*/
#ifndef __MUTEX_H_
#define __MUTEX_H_

#include "kerneltypes.h"
#include "mark3cfg.h"

#include "blocking.h"

#if KERNEL_USE_MUTEX

#if KERNEL_USE_TIMERS
#include "timerlist.h"
#endif

//---------------------------------------------------------------------------
/*!
    Mutual-exclusion locks, based on BlockingObject.
*/
class Mutex : public BlockingObject
{
public:
    /*!
        \fn void Init()
        
        Initialize a mutex object for use - must call this function before using
        the object.
    */
    void Init();

    /*!
        \fn void Claim()
        
        Claim the mutex.  When the mutex is claimed, no other thread can claim a
        region protected by the object.
    */
    void Claim();

#if KERNEL_USE_TIMERS

	/*!
		\fn bool Claim(K_ULONG ulWaitTimeMS_)
		
		\param ulWaitTimeMS_
		
		\return true - mutex was claimed within the time period specified
				false - mutex operation timed-out before the claim operation.
	*/
	bool Claim(K_ULONG ulWaitTimeMS_);
	
	/*!
		\fn void WakeMe( Thread *pclOwner_ )
	    
        Wake a thread blocked on the mutex.  This is an
        internal function used for implementing timed mutexes
        relying on timer callbacks.  Since these do not have
        access to the private data of the mutex and its base
        classes, we have to wrap this as a public method - do not
        use this for any other purposes.
    
		\param pclOwner_ Thread to unblock from this object.		
	*/
	void WakeMe( Thread *pclOwner_ );
	
	void SetExpired( bool bExpired_ ) { m_bExpired = bExpired_; }
#endif

    /*!
        \fn void Release()
        
        Release the mutex.  When the mutex is released, another object can enter
        the mutex-protected region.
    */
    void Release();
    
private:

    /*!
        \fn K_UCHAR WakeNext();
        
        Wake the next thread waiting on the Mutex.
    */
    K_UCHAR WakeNext();
    
    K_UCHAR m_bReady;
    K_UCHAR m_ucMaxPri;
    Thread *m_pclOwner;
	
#if KERNEL_USE_TIMERS
	bool	m_bExpired;
#endif	
};

#endif //KERNEL_USE_MUTEX

#endif //__MUTEX_H_
