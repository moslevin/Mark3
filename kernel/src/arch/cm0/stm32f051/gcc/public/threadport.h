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

    @brief  Cortex M-0 Multithreading support.
 */

#ifndef __THREADPORT_H_
#define __THREADPORT_H_

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
//! These macros *must* be used in matched-pairs !
//! Nesting *is* supported !
extern volatile uint32_t g_ulCriticalCount;

//------------------------------------------------------------------------
#ifndef xDMB
    #define xDMB()                    ASM(" dmb \n");
#endif
#ifndef xdisable_irq
    #define xdisable_irq()            ASM(" cpsid i \n");
#endif
#ifndef xenable_irq
    #define xenable_irq()            ASM(" cpsie i \n");
#endif

#define ENABLE_INTS()        { xDMB(); xenable_irq(); }
#define DISABLE_INTS()        { xdisable_irq(); xDMB(); }

//------------------------------------------------------------------------
//! Enter critical section (copy current PRIMASK register value, disable interrupts)
#define CS_ENTER()    \
{ \
    DISABLE_INTS(); \
    g_ulCriticalCount++;\
}
//------------------------------------------------------------------------
//! Exit critical section (restore previous PRIMASK status register value)
#define CS_EXIT() \
{ \
    g_ulCriticalCount--; \
    if( 0 == g_ulCriticalCount ) { \
        ENABLE_INTS(); \
    } \
}

#endif //__ThreadPORT_H_
