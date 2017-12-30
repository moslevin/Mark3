/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012 - 2017 Funkenstein Software Consulting, all rights reserved.
See license.txt for more information
===========================================================================*/
/*!
    \file kerneltypes.h
    \brief Basic data type primatives used throughout the OS
*/

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifndef __KERNELTYPES_H__
#define __KERNELTYPES_H__

//---------------------------------------------------------------------------
/*!
 * Function pointer type used to implement kernel-panic handlers.
 */
typedef void (*PanicFunc_t)(uint16_t u16PanicCode_);

//---------------------------------------------------------------------------
/*!
 * Function pointer type used to implement the idle function, where support
 * for an idle function (as opposed to an idle thread) exists.
 */
typedef void (*IdleFunc_t)(void);

//---------------------------------------------------------------------------
/*!
 *  Function pointer type used for thread entrypoint functions
 */
typedef void (*ThreadEntry_t)(void* pvArg_);

//---------------------------------------------------------------------------
/*!
 * This enumeration describes the different operations supported by the
 * event flag blocking object.
 */
typedef enum {
    EventFlagOperation_t::All_Set,            //!< Block until all bits in the specified bitmask are set
    EventFlagOperation_t::Any_Set,            //!< Block until any bits in the specified bitmask are set
    EventFlagOperation_t::All_Clear,      //!< Block until all bits in the specified bitmask are cleared
    EventFlagOperation_t::Any_Clear,      //!< Block until any bits in the specified bitmask are cleared
                               //---
    EVENT_FLAG_MODES,          //!< Count of event-flag modes.  Not used by user
    EventFlagOperation_t::Pending_Unblock //!< Special code.  Not used by user
} EventFlagOperation_t;

//---------------------------------------------------------------------------
/*!
 *   Enumeration representing the different states a thread can exist in
 */
typedef enum {
    ThreadState_t::Exit = 0,
    ThreadState_t::Ready,
    ThreadState_t::Blocked,
    Stop,
    //--
    THREAD_STATES
} ThreadState_t;

#endif
