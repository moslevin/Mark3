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
    \file eventflag.h
    \brief Event Flag Blocking Object/IPC-Object definition.
*/

#ifndef __EVENTFLAG_H__
#define __EVENTFLAG_H__

#include "mark3cfg.h"
#include "kernel.h"
#include "kerneltypes.h"
#include "blocking.h"
#include "thread.h"

#if KERNEL_USE_EVENTFLAG

//---------------------------------------------------------------------------
/*!
 * \brief The EventFlag class is a blocking object, similar to a semaphore or
 * mutex, commonly used for synchronizing thread execution based on events
 * occurring within the system.
 *
 * Each EventFlag object contains a 16-bit bitmask, which is used to trigger
 * events on associated threads.  Threads wishing to block, waiting for
 * a specific event to occur can wait on any pattern within this 16-bit bitmask
 * to be set.  Here, we provide the ability for a thread to block, waiting
 * for ANY bits in a specified mask to be set, or for ALL bits within a
 * specific mask to be set.  Depending on how the object is configured, the
 * bits that triggered the wakeup can be automatically cleared once a match
 * has occurred.
 *
 */
class EventFlag : public BlockingObject
{
public:
    void* operator new (size_t sz, void* pv) { return (EventFlag*)pv; };

    ~EventFlag();

    /*!
     * \brief Init Initializes the EventFlag object prior to use.
     */
    void Init() { m_u16SetMask = 0; m_clBlockList.Init(); }

    /*!
     * \brief Wait - Block a thread on the specific flags in this event flag group
     * \param u16Mask_ - 16-bit bitmask to block on
     * \param eMode_ - EVENT_FLAG_ANY:  Thread will block on any of the bits in the mask
     *               - EVENT_FLAG_ALL:  Thread will block on all of the bits in the mask
     * \return Bitmask condition that caused the thread to unblock, or 0 on error or timeout
     */
    uint16_t Wait(uint16_t u16Mask_, EventFlagOperation_t eMode_);

#if KERNEL_USE_TIMEOUTS
    /*!
     * \brief Wait - Block a thread on the specific flags in this event flag group
     * \param u16Mask_ - 16-bit bitmask to block on
     * \param eMode_ - EVENT_FLAG_ANY:  Thread will block on any of the bits in the mask
     *               - EVENT_FLAG_ALL:  Thread will block on all of the bits in the mask
     * \param u32TimeMS_ - Time to block (in ms)
     * \return Bitmask condition that caused the thread to unblock, or 0 on error or timeout
     */
    uint16_t Wait(uint16_t u16Mask_, EventFlagOperation_t eMode_, uint32_t u32TimeMS_);

    /*!
     * \brief WakeMe
     *
     * Wake the given thread, currently blocking on this object
     *
     * \param pclOwner_ Pointer to the owner thread to unblock.
     */
    void WakeMe(Thread *pclOwner_);

#endif

    /*!
     * \brief Set - Set additional flags in this object (logical OR).  This API can potentially
     *              result in threads blocked on Wait() to be unblocked.
     * \param u16Mask_ - Bitmask of flags to set.
     */
    void Set(uint16_t u16Mask_);

    /*!
     * \brief ClearFlags - Clear a specific set of flags within this object, specific by bitmask
     * \param u16Mask_ - Bitmask of flags to clear
     */
    void Clear(uint16_t u16Mask_);

    /*!
     * \brief GetMask Returns the state of the 16-bit bitmask within this object
     * \return The state of the 16-bit bitmask
     */
    uint16_t GetMask();

private:

#if KERNEL_USE_TIMEOUTS
    /*!
     * \brief Wait_i
     *
     * Interal abstraction used to manage both timed and untimed wait operations
     *
     * \param u16Mask_ - 16-bit bitmask to block on
     * \param eMode_ - EVENT_FLAG_ANY:  Thread will block on any of the bits in the mask
     *               - EVENT_FLAG_ALL:  Thread will block on all of the bits in the mask
     * \param u32TimeMS_ - Time to block (in ms)
     *
     * \return Bitmask condition that caused the thread to unblock, or 0 on error or timeout
     */
    uint16_t Wait_i(uint16_t u16Mask_, EventFlagOperation_t eMode_, uint32_t u32TimeMS_);
#else
    /*!
     * \brief Wait_i
     * Interal abstraction used to manage wait operations
     *
     * \param u16Mask_ - 16-bit bitmask to block on
     * \param eMode_ - EVENT_FLAG_ANY:  Thread will block on any of the bits in the mask
     *               - EVENT_FLAG_ALL:  Thread will block on all of the bits in the mask
     *
     * \return Bitmask condition that caused the thread to unblock.
     */
    uint16_t Wait_i(uint16_t u16Mask_, EventFlagOperation_t eMode_);
#endif

    uint16_t m_u16SetMask;       //!< Event flags currently set in this object
};

#endif //KERNEL_USE_EVENTFLAG
#endif //__EVENTFLAG_H__

