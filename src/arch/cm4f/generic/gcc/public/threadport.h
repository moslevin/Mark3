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

    @brief  Cortex M4  Multithreading support.
 */
#pragma once

#include "portcfg.h"
#include "kerneltypes.h"

namespace Mark3
{
// clang-format off
//---------------------------------------------------------------------------
//! ASM Macro - simplify the use of ASM directive in C
#define ASM      asm volatile

//---------------------------------------------------------------------------
//! Macro to find the top of a stack given its size and top address
#define PORT_TOP_OF_STACK(x, y)         (reinterpret_cast<K_WORD*>(reinterpret_cast<K_ADDR>(x) + (static_cast<K_ADDR>(y) - sizeof(K_WORD))))
//! Push a value y to the stack pointer x and decrement the stack pointer
#define PORT_PUSH_TO_STACK(x, y)        *x = y; x--;

#define PORT_CLZ(x)      __builtin_clz((x))

//------------------------------------------------------------------------
#ifndef xDMB
    #define xDMB()                    ASM(" dmb \n");
#endif

//------------------------------------------------------------------------
extern "C" {
   extern uint8_t g_u8SR;
   extern K_WORD g_kwCriticalCount;
}

//------------------------------------------------------------------------
inline void PORT_IRQ_ENABLE()
{
    ASM(" cpsie i \n");
}

//------------------------------------------------------------------------
inline void PORT_IRQ_DISABLE()
{
    ASM(" cpsid i \n");
}

//------------------------------------------------------------------------
inline void PORT_CS_ENTER()
{
    volatile uint8_t __sr;
    ASM (
    " mrs   r0, PRIMASK\n "
    " cpsid i \n"
    " strb r0, %[output] \n"
    : [output] "=m" (__sr) :: "r0");

    if (!g_kwCriticalCount) {
        g_u8SR = __sr;
    }
    g_kwCriticalCount++;
}

//------------------------------------------------------------------------
inline void PORT_CS_EXIT()
{
    g_kwCriticalCount--;
    if (!g_kwCriticalCount) {
        ASM (
        " ldrb r0, %[input]\n "
        " msr PRIMASK, r0 \n "
        ::[input] "m" (g_u8SR) : "r0");
    }
}

//---------------------------------------------------------------------------
inline K_WORD PORT_CS_NESTING()
{
    return g_kwCriticalCount;
}
} // namespace Mark3

