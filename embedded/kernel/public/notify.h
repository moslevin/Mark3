/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012-2016 Funkenstein Software Consulting, all rights reserved.
See license.txt for more information
===========================================================================*/
/*!
    \file   notify.h

    \brief  Lightweight thread notification - blocking object

*/

#ifndef __NOTIFY_H__
#define __NOTIFY_H__

#include "mark3cfg.h"
#include "blocking.h"

#if KERNEL_USE_NOTIFY

/*!
 * \brief The Notify class is a blocking object type, that allows one or more
 * threads to wait for an event to occur before resuming operation.
 */
class Notify : public BlockingObject
{
public:

    void* operator new (size_t sz, void* pv) { return (Notify*)pv; };

    ~Notify();

    /*!
     *  \brief Init
     *
     * Initialze the Notification object prior to use.
     */
    void Init(void);

    /*!
     *  \brief Signal
     *
     * Signal the notification object.  This will cause the
     * highest priority thread currently blocking on the object
     * to wake.  If no threads are currently blocked on the
     * object, the call has no effect.
     *
     */
    void Signal(void);

    /*!
     *  \brief Wait
     *
     * Block the current thread, waiting for a signal on the
     * object.
     *
     * \param pbFlag_ Flag set to false on block, and true
     *                upon wakeup.
     */
    void Wait( bool *pbFlag_ );

#if KERNEL_USE_TIMEOUTS
    /*!
     * \brief Wait
     *
     *  Block the current thread, waiting for a signal on the
     *  object.
     *
     * \param u32WaitTimeMS_ Time to wait for the notification
     *                      event.
     * \param pbFlag_       Flag set to false on block, and
     *                      true upon wakeup.
     * \return true on notification, false on timeout
     */
    bool Wait( uint32_t u32WaitTimeMS_, bool *pbFlag_ );
#endif

    /*!
     * \brief WakeMe
     *
     * Wake the specified thread from its current blocking queue.
     * Note that this is only public in order to be accessible
     * from a timer callack.
     *
     * \param pclChosenOne_ Thread to wake up
     */
    void WakeMe(Thread *pclChosenOne_);
};

#endif

#endif
