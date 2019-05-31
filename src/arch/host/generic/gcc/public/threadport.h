/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012 - 2019 m0slevin, all rights reserved.
See license.txt for more information
=========================================================================== */
/**

    @file   threadport.h

    @brief  ATMega328p Multithreading support.
 */
#pragma once

#include "portcfg.h"
#include "kerneltypes.h"

namespace Mark3
{

//---------------------------------------------------------------------------
//! Macro to find the top of a stack given its size and top address
#define PORT_TOP_OF_STACK(x, y)         (reinterpret_cast<K_WORD*>(reinterpret_cast<K_ADDR>(x) + (static_cast<K_ADDR>(y) - 1)))
//! Push a value y to the stack pointer x and decrement the stack pointer
#define PORT_PUSH_TO_STACK(x, y)        *x = y; x--;

//------------------------------------------------------------------------
extern "C" {
   extern uint8_t g_u8SR;
   extern K_WORD g_kwCriticalCount;
}

//------------------------------------------------------------------------
inline void PORT_IRQ_ENABLE()
{
}

//------------------------------------------------------------------------
inline void PORT_IRQ_DISABLE()
{
}

//------------------------------------------------------------------------
inline void PORT_CS_ENTER()
{
}
//------------------------------------------------------------------------
inline void PORT_CS_EXIT()
{
}

//---------------------------------------------------------------------------
inline K_WORD PORT_CS_NESTING()
{
    return 0;
}

} // namespace Mark3
