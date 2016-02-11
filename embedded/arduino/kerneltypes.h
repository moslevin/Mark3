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

#ifndef __KERNELTYPES_H__
#define __KERNELTYPES_H__

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

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

