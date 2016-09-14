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
    \file kerneltypes.h
    \brief Basic data type primatives used throughout the OS
*/

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifndef __KERNELTYPES_H__
#define __KERNELTYPES_H__

//---------------------------------------------------------------------------
#if !defined(K_ADDR)
# if defined(AVR)
#  warning "using platform default size for K_ADDR - uint8_t"
#  define K_ADDR uint16_t
# elif defined(MSP430)
#  warning "using platform default size for K_ADDR - uint16_t"
#  define K_ADDR uint16_t
# elif defined(ARM)
#  warning "using platform default size for K_ADDR - uint32_t"
#  define K_ADDR uint32_t
# else
#  warning "No K_ADDR defined -- defaulting to 32 bits"
#  define K_ADDR uint32_t
# endif
#endif

#if !defined(K_WORD)
# if defined(AVR)
#  warning "using platform default size for K_WORD - uint8_t"
#  define K_WORD uint8_t
# elif defined(MSP430)
#  warning "using platform default size for K_WORD - uint16_t"
#  define K_WORD uint16_t
# elif defined(ARM)
#  warning "using platform default size for K_WORD - uint32_t"
#  define K_WORD uint32_t
# else
#  warning "No K_WORD defined -- defaulting to 32 bits"
#  define K_ADDR uint32_t
# endif
#endif

#if !defined(PRIO_MAP_WORD_SIZE)
# if defined(AVR)
#  warning "using platform default size for PRIO_MAP_WORD_SIZE - sizeof(uint8_t)"
#  define PRIO_MAP_WORD_SIZE    (1)
# elif defined(MSP430)
#  warning "using platform default size for PRIO_MAP_WORD_SIZE - sizeof(uint16_t)"
#  define PRIO_MAP_WORD_SIZE    (2)
# elif defined(ARM)
#  warning "using platform default size for PRIO_MAP_WORD_SIZE - sizeof(uint32_t)"
#  define PRIO_MAP_WORD_SIZE    (4)
# else
#  warning "No PRIO_MAP_WORD_SIZE defined - default to sizeof(uint32_t)"
#  define PRIO_MAP_WORD_SIZE    (4)
#endif

#endif

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
    EVENT_FLAG_ALL,            //!< Block until all bits in the specified bitmask are set
    EVENT_FLAG_ANY,            //!< Block until any bits in the specified bitmask are set
    EVENT_FLAG_ALL_CLEAR,      //!< Block until all bits in the specified bitmask are cleared
    EVENT_FLAG_ANY_CLEAR,      //!< Block until any bits in the specified bitmask are cleared
                               //---
    EVENT_FLAG_MODES,          //!< Count of event-flag modes.  Not used by user
    EVENT_FLAG_PENDING_UNBLOCK //!< Special code.  Not used by user
} EventFlagOperation_t;

//---------------------------------------------------------------------------
/*!
 *   Enumeration representing the different states a thread can exist in
 */
typedef enum {
    THREAD_STATE_EXIT = 0,
    THREAD_STATE_READY,
    THREAD_STATE_BLOCKED,
    THREAD_STATE_STOP,
    //--
    THREAD_STATES
} ThreadState_t;

#endif
