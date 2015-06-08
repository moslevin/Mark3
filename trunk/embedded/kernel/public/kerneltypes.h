/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012-2015 Funkenstein Software Consulting, all rights reserved.
See license.txt for more information
===========================================================================*/
/*!
    \file kerneltypes.h
    \brief Basic data type primatives used throughout the OS
*/

#include <stdint.h>

#ifndef __KERNELTYPES_H__
#define __KERNELTYPES_H__

//---------------------------------------------------------------------------
#if defined(bool)
    #define K_BOOL            bool          //!< Basic boolean data type (true = 1, false = 0)
#else
    #define K_BOOL            uint8_t       //!< Basic boolean data type (true = 1, false = 0)
#endif
    
#define K_CHAR          char                //!< The 8-bit signed integer type used by Mark3
#define K_UCHAR         uint8_t             //!< The 8-bit unsigned integer type used by Mark3
#define K_USHORT        uint16_t            //!< The 16-bit unsigned integer type used by Mark3
#define K_SHORT         int16_t             //!< The 16-bit signed integer type used by Mark3
#define K_ULONG         uint32_t            //!< The 32-bit unsigned integer type used by Mark3
#define K_LONG          int32_t             //!< The 32-bit signed integer type used by Mark3

#if !defined(K_ADDR)
    #define K_ADDR      uint32_t            //!< Primative datatype representing address-size
#endif
#if !defined(K_WORD)
    #define K_WORD      uint32_t            //!< Primative datatype representing a data word
#endif

//---------------------------------------------------------------------------
/*!
 * Function pointer type used to implement kernel-panic handlers.
 */
typedef void (*panic_func_t)( K_USHORT usPanicCode_ );

//---------------------------------------------------------------------------
/*!
 * This enumeration describes the different operations supported by the
 * event flag blocking object.
 */
typedef enum
{
    EVENT_FLAG_ALL,             //!< Block until all bits in the specified bitmask are set
    EVENT_FLAG_ANY,             //!< Block until any bits in the specified bitmask are set
    EVENT_FLAG_ALL_CLEAR,       //!< Block until all bits in the specified bitmask are cleared
    EVENT_FLAG_ANY_CLEAR,       //!< Block until any bits in the specified bitmask are cleared
//---
    EVENT_FLAG_MODES,           //!< Count of event-flag modes.  Not used by user
    EVENT_FLAG_PENDING_UNBLOCK  //!< Special code.  Not used by user
} EventFlagOperation_t;


#endif
