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
 * \brief The EventFlag class
 */
class EventFlag : public BlockingObject
{
public:
    /*!
     * \brief Init Initializes the EventFlag object prior to use.
     */
    void Init() { m_usSetMask = 0; m_clBlockList.Init(); }

    /*!
     * \brief Wait - Block a thread on the specific flags in this event flag group
     * \param usMask_ - 16-bit bitmask to block on
     * \param eMode_ - EVENT_FLAG_ANY:  Thread will block on any of the bits in the mask
     *               - EVENT_FLAG_ALL:  Thread will block on all of the bits in the mask
     */
    void Wait(K_USHORT usMask_, EventFlagOperation_t eMode_);

    /*!
     * \brief SetFlags
     * \param usMask_
     */
    void SetFlags(K_USHORT usMask_);

    /*!
     * \brief ClearFlags
     * \param usMask_
     */
    void ClearFlags(K_USHORT usMask_);

    /*!
     * \brief GetMask
     * \return
     */
    K_USHORT GetMask();

private:
    K_USHORT m_usSetMask;
};

#endif //KERNEL_USE_EVENTFLAG
#endif //__EVENTFLAG_H__

