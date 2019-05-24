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

    @brief  ATMega328p Multithreading support.
 */
#pragma once

#include "portcfg.h"
#include "kerneltypes.h"

#include <avr/io.h>
#include <avr/interrupt.h>

namespace Mark3
{
// clang-format off
//---------------------------------------------------------------------------
#define ASM(x)      asm volatile(x);

//---------------------------------------------------------------------------
//! Macro to find the top of a stack given its size and top address
#define PORT_TOP_OF_STACK(x, y)    (reinterpret_cast<K_WORD*>(reinterpret_cast<K_ADDR>(x) + (static_cast<K_ADDR>(y) - 1)))
//! Push a value y to the stack pointer x and decrement the stack pointer
#define PORT_PUSH_TO_STACK(x, y)        *x = y; x--;

//---------------------------------------------------------------------------
//! Save the context of the Thread
#define Thread_SaveContext() \
ASM("push r0"); \
ASM("in r0, __SREG__"); \
ASM("cli"); \
ASM("push r0"); \
ASM("push r1"); \
ASM("clr r1"); \
ASM("push r2"); \
ASM("push r3"); \
ASM("push r4"); \
ASM("push r5"); \
ASM("push r6"); \
ASM("push r7"); \
ASM("push r8"); \
ASM("push r9"); \
ASM("push r10"); \
ASM("push r11"); \
ASM("push r12"); \
ASM("push r13"); \
ASM("push r14"); \
ASM("push r15"); \
ASM("push r16"); \
ASM("push r17"); \
ASM("push r18"); \
ASM("push r19"); \
ASM("push r20"); \
ASM("push r21"); \
ASM("push r22"); \
ASM("push r23"); \
ASM("push r24"); \
ASM("push r25"); \
ASM("push r26"); \
ASM("push r27"); \
ASM("push r28"); \
ASM("push r29"); \
ASM("push r30"); \
ASM("push r31"); \
ASM("in    r0, 0x3B"); \
ASM("push r0"); \
ASM("lds r26, g_pclCurrent"); \
ASM("lds r27, g_pclCurrent + 1"); \
ASM("adiw r26, 4"); \
ASM("in    r0, 0x3D"); \
ASM("st    x+, r0"); \
ASM("in    r0, 0x3E"); \
ASM("st    x+, r0");

//---------------------------------------------------------------------------
//! Restore the context of the Thread
#define Thread_RestoreContext() \
ASM("lds r26, g_pclCurrent"); \
ASM("lds r27, g_pclCurrent + 1");\
ASM("adiw r26, 4"); \
ASM("ld     r28, x+"); \
ASM("out 0x3D, r28"); \
ASM("ld     r29, x+"); \
ASM("out 0x3E, r29"); \
ASM("pop r0"); \
ASM("out 0x3B, r0"); \
ASM("pop r31"); \
ASM("pop r30"); \
ASM("pop r29"); \
ASM("pop r28"); \
ASM("pop r27"); \
ASM("pop r26"); \
ASM("pop r25"); \
ASM("pop r24"); \
ASM("pop r23"); \
ASM("pop r22"); \
ASM("pop r21"); \
ASM("pop r20"); \
ASM("pop r19"); \
ASM("pop r18"); \
ASM("pop r17"); \
ASM("pop r16"); \
ASM("pop r15"); \
ASM("pop r14"); \
ASM("pop r13"); \
ASM("pop r12"); \
ASM("pop r11"); \
ASM("pop r10"); \
ASM("pop r9"); \
ASM("pop r8"); \
ASM("pop r7"); \
ASM("pop r6"); \
ASM("pop r5"); \
ASM("pop r4"); \
ASM("pop r3"); \
ASM("pop r2"); \
ASM("pop r1"); \
ASM("pop r0"); \
ASM("out __SREG__, r0"); \
ASM("pop r0");

//---------------------------------------------------------------------------
static constexpr auto SR_ = uint8_t{0x3F};
extern "C" {
   extern K_WORD g_kwSFR;
   extern K_WORD g_kwCriticalCount;
}

//---------------------------------------------------------------------------
inline uint8_t PORT_CLZ(uint8_t in_)
{
    static const uint8_t u8Lookup[] = {4, 3, 2, 2, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
    uint8_t hi = __builtin_avr_swap(in_) & 0x0F;
    if (hi) {
        return u8Lookup[hi];
    }
    return 4 + u8Lookup[in_];
}

//---------------------------------------------------------------------------
inline void PORT_IRQ_ENABLE()
{
    ASM("sei");
}

//---------------------------------------------------------------------------
inline void PORT_IRQ_DISABLE()
{
    ASM("cli");
}

//---------------------------------------------------------------------------
inline void PORT_CS_ENTER()
{
    auto u8SFR = _SFR_IO8(SR_);
    ASM("cli");
    if (!g_kwCriticalCount) {
        g_kwSFR = u8SFR;
    }
    g_kwCriticalCount++;
}

//---------------------------------------------------------------------------
inline void PORT_CS_EXIT()
{
    g_kwCriticalCount--;
    if (!g_kwCriticalCount) {
        _SFR_IO8(SR_) = g_kwSFR;
    }
}

//---------------------------------------------------------------------------
inline K_WORD PORT_CS_NESTING()
{
    return g_kwCriticalCount;
}
} // namespace Mark3
