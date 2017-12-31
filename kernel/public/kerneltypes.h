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

#pragma once
namespace Mark3
{
//---------------------------------------------------------------------------
/*!
 * Function pointer type used to implement kernel-panic handlers.
 */
using PanicFunc = void (*)(uint16_t u16PanicCode_);

//---------------------------------------------------------------------------
/*!
 * Function pointer type used to implement the idle function, where support
 * for an idle function (as opposed to an idle thread) exists.
 */
using IdleFunc = void (*)();

//---------------------------------------------------------------------------
/*!
 *  Function pointer type used for thread entrypoint functions
 */
using ThreadEntryFunc = void (*)(void* pvArg_);

//---------------------------------------------------------------------------
/*!
 * This enumeration describes the different operations supported by the
 * event flag blocking object.
 */
enum class EventFlagOperation : uint8_t {
    All_Set = 0,    //!< Block until all bits in the specified bitmask are set
    Any_Set,        //!< Block until any bits in the specified bitmask are set
    All_Clear,      //!< Block until all bits in the specified bitmask are cleared
    Any_Clear,      //!< Block until any bits in the specified bitmask are cleared
    Pending_Unblock //!< Special code.  Not used by user
};

//---------------------------------------------------------------------------
/*!
 *   Enumeration representing the different states a thread can exist in
 */
enum class ThreadState : uint8_t {
    Exit = 0,
    Ready,
    Blocked,
    Stop,
    Invalid
};

} //namespace Mark3
