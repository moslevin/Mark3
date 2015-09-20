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

#if defined(bool)
    #define bool            bool    
#else
    #define bool            uint8_t
#endif
    
#define char          char
#define uint8_t         uint8_t
#define uint16_t        uint16_t
#define int16_t         int16_t
#define uint32_t         uint32_t
#define int32_t          int32_t

#define K_ADDR      uint16_t
#define K_WORD      uint8_t

//---------------------------------------------------------------------------
typedef void (*panic_func_t)( uint16_t u16PanicCode_ );

//---------------------------------------------------------------------------
typedef enum
{
    EVENT_FLAG_ALL,
    EVENT_FLAG_ANY,
    EVENT_FLAG_ALL_CLEAR,
    EVENT_FLAG_ANY_CLEAR,
    EVENT_FLAG_MODES,
    EVENT_FLAG_PENDING_UNBLOCK
} EventFlagOperation_t;


#endif
