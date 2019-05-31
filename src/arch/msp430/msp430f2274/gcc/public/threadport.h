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

    @brief  MSP430 Multithreading support.
 */
#pragma once

#include "portcfg.h"
#include "kerneltypes.h"

#include <msp430.h>
#include <in430.h>

namespace Mark3
{
// clang-format off
//---------------------------------------------------------------------------
//! ASM Macro - simplify the use of ASM directive in C
#define ASM(x)      asm volatile(x);

//---------------------------------------------------------------------------
//! Macro to find the top of a stack given its size and top address
#define PORT_TOP_OF_STACK(x, y)         (reinterpret_cast<K_WORD*>(reinterpret_cast<K_ADDR>(x) + (static_cast<K_ADDR>(y) - sizeof(K_WORD))))

//! Push a value y to the stack pointer x and decrement the stack pointer
#define PORT_PUSH_TO_STACK(x, y)        *x = y; x--;

//---------------------------------------------------------------------------
//! Save the context of the Thread
#define Thread_SaveContext()\
    ASM("push    r4\n");    \
    ASM("push    r5\n");    \
    ASM("push    r6\n");    \
    ASM("push    r7\n");    \
    ASM("push    r8\n");    \
    ASM("push    r9\n");    \
    ASM("push    r10\n"); \
    ASM("push    r11\n"); \
    ASM("push    r12\n"); \
    ASM("push    r13\n"); \
    ASM("push    r14\n"); \
    ASM("push    r15\n"); \
    ASM("mov.w    &g_pclCurrent, r12\n"); \
    ASM("add.w  #4, r12"); \
    ASM("mov.w    r1, 0(r12)\n");

//---------------------------------------------------------------------------
//! Restore the context of the Thread
#define Thread_RestoreContext()\
    ASM("mov.w  &g_pclCurrent, r12"); \
    ASM("add.w  #4, r12"); \
    ASM("mov.w  @r12, r1"); \
    ASM("pop    r15"); \
    ASM("pop    r14"); \
    ASM("pop    r13"); \
    ASM("pop    r12"); \
    ASM("pop    r11"); \
    ASM("pop    r10"); \
    ASM("pop     r9"); \
    ASM("pop    r8"); \
    ASM("pop    r7"); \
    ASM("pop    r6"); \
    ASM("pop    r5"); \
    ASM("pop    r4"); \
    ASM("bic.w    #0x00F0, 0(r1)");

//---------------------------------------------------------------------------
extern uint8_t g_u8CSCount;
extern uint16_t g_u16SR;

//------------------------------------------------------------------------
inline void PORT_IRQ_ENABLE()
{
    __eint();
}

//------------------------------------------------------------------------
inline void PORT_IRQ_DISABLE()
{
    __dint();
}
//------------------------------------------------------------------------
inline void PORT_CS_ENTER()
{
    uint16_t u16IntState = __get_interrupt_state();
    __dint();
    if (0 == Mark3::g_u8CSCount)
    {
        Mark3::g_u16SR = u16IntState;
    }
    Mark3::g_u8CSCount++;
}

//------------------------------------------------------------------------
//! Exit critical section (restore GIE bit in
inline void PORT_CS_EXIT()
{
    Mark3::g_u8CSCount--;
    if (1 == Mark3::g_u8CSCount)
    {
        if((Mark3::g_u16SR & 0x0008) == 0x0008)
        {
            __nop();
            __eint();
        }
    }
}

//---------------------------------------------------------------------------
inline uint8_t PORT_CS_NESTING()
{
    return Mark3::g_u8CSCount;
}
} // namespace Mark3
