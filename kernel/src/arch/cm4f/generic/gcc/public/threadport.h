/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012 - 2018 m0slevin, all rights reserved.
See license.txt for more information
=========================================================================== */
/**

    @file   threadport.h

    @brief  Cortex M4  Multithreading support.
 */
#pragma once

#include "kerneltypes.h"
#include "thread.h"
#include "ithreadport.h"

// clang-format off
//---------------------------------------------------------------------------
//! ASM Macro - simplify the use of ASM directive in C
#define ASM      asm volatile

//---------------------------------------------------------------------------
//! Macro to find the top of a stack given its size and top address
#define TOP_OF_STACK(x, y)        (K_WORD*) ( ((uint32_t)x) + (y - sizeof(K_WORD)) )
//! Push a value y to the stack pointer x and decrement the stack pointer
#define PUSH_TO_STACK(x, y)        *x = y; x--;
#define STACK_GROWS_DOWN           (1)

//------------------------------------------------------------------------
// Use hardware accelerated count-leading zero
#define HW_CLZ (1)
//extern unsigned char __clz(unsigned int x);
#define CLZ(x)      __builtin_clz((x))

//------------------------------------------------------------------------
#ifndef xDMB
    #define xDMB()                    ASM(" dmb \n");
#endif
#ifndef xdisable_irq
    #define xdisable_irq()            ASM(" cpsid i \n");
#endif
#ifndef xenable_irq
    #define xenable_irq()             ASM(" cpsie i \n");
#endif

//------------------------------------------------------------------------
//! Enter critical section (copy current PRIMASK register value, disable interrupts)
#define CS_ENTER()                      \
do {                                    \
    volatile uint8_t __sr;              \
    ASM (                               \
    " mrs   r0, PRIMASK\n "             \
    " cpsid i \n"                       \
    " strb r0, %[output] \n"            \
    : [output] "=m" (__sr) :: "r0");

//------------------------------------------------------------------------
//! Exit critical section (restore previous PRIMASK status register value)
#define CS_EXIT()                       \
    ASM (                               \
    " ldrb r0, %[input]\n "             \
    " msr PRIMASK, r0 \n "              \
    ::[input] "m" (__sr) : "r0");       \
} while(0);

