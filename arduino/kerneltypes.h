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
typedef void (*panic_func_t)(uint16_t u16PanicCode_);

//---------------------------------------------------------------------------
/*!
 * Function pointer type used to implement the idle function, where support
 * for an idle function (as opposed to an idle thread) exists.
 */
typedef void (*idle_func_t)(void);

//---------------------------------------------------------------------------
/*!
 *  Function pointer type used for thread entrypoint functions
 */
typedef void (*thread_entry_func_t)(void* pvArg_);

//---------------------------------------------------------------------------
/*!
 * This enumeration describes the different operations supported by the
 * event flag blocking object.
 */
typedef enum {
    EventFlagOperation::All_Set,            //!< Block until all bits in the specified bitmask are set
    EventFlagOperation::Any_Set,            //!< Block until any bits in the specified bitmask are set
    EventFlagOperation::All_Clear,      //!< Block until all bits in the specified bitmask are cleared
    EventFlagOperation::Any_Clear,      //!< Block until any bits in the specified bitmask are cleared
                               //---
    EVENT_FLAG_MODES,          //!< Count of event-flag modes.  Not used by user
    EventFlagOperation::Pending_Unblock //!< Special code.  Not used by user
} EventFlagOperation;

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
