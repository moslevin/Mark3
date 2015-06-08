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

    \file   threadport.h    

    \brief  MSP430 Multithreading support.
*/

#ifndef __THREADPORT_H_
#define __THREADPORT_H_

#include "kerneltypes.h"
#include "thread.h"

#include <msp430.h>
#include <in430.h>

//---------------------------------------------------------------------------
extern volatile K_UCHAR g_ucCSCount;
extern volatile K_USHORT g_usSR;

//---------------------------------------------------------------------------
//! ASM Macro - simplify the use of ASM directive in C
#define ASM(x)      asm volatile(x);

//---------------------------------------------------------------------------
//! Macro to find the top of a stack given its size and top address
#define TOP_OF_STACK(x, y)        (K_ADDR*) ( ((K_USHORT)x) + ((y-1) * sizeof(K_WORD)))
//! Push a value y to the stack pointer x and decrement the stack pointer
#define PUSH_TO_STACK(x, y)        *x = y; x--;

//---------------------------------------------------------------------------
//! Save the context of the Thread
#define Thread_SaveContext()\
    ASM("push	r4\n");	\
    ASM("push	r5\n");	\
    ASM("push	r6\n");	\
    ASM("push	r7\n");	\
    ASM("push	r8\n");	\
    ASM("push	r9\n");	\
    ASM("push	r10\n"); \
    ASM("push	r11\n"); \
    ASM("push	r12\n"); \
    ASM("push	r13\n"); \
    ASM("push	r14\n"); \
    ASM("push	r15\n"); \
    ASM("mov.w	&g_pstCurrent, r12\n"); \
    ASM("add.w  #4, r12"); \
    ASM("mov.w	r1, 0(r12)\n");

//---------------------------------------------------------------------------
//! Restore the context of the Thread
#define Thread_RestoreContext()\
    ASM("mov.w  &g_pstCurrent, r12"); \
    ASM("add.w  #4, r12"); \
    ASM("mov.w  @r12, r1"); \
    ASM("pop	r15"); \
    ASM("pop	r14"); \
    ASM("pop	r13"); \
    ASM("pop	r12"); \
    ASM("pop	r11"); \
    ASM("pop	r10"); \
    ASM("pop 	r9"); \
    ASM("pop	r8"); \
    ASM("pop	r7"); \
    ASM("pop	r6"); \
    ASM("pop	r5"); \
    ASM("pop	r4"); \
    ASM("bic.w	#0x00F0, 0(r1)");

//------------------------------------------------------------------------
//! These macros *must* be used in pairs !
//------------------------------------------------------------------------
//! Enter critical section (cache GIE bit in the status register, disable interrupts)
#define CS_ENTER() \
do { \
	K_USHORT usIntState = __get_interrupt_state(); \
	__dint(); \
	if (0 == g_ucCSCount) \
	{ \
		g_usSR = usIntState; \
    } \
	g_ucCSCount++; \
} while(0);

//------------------------------------------------------------------------
//! Exit critical section (restore GIE bit in
#define CS_EXIT() \
do { \
    if (1 == g_ucCSCount) \
    { \
        if((g_usSR & 0x0008) == 0x0008) \
		{ \
			__eint(); \
		} \
    } \
	g_ucCSCount--; \
} while(0);

//------------------------------------------------------------------------
#define ENABLE_INTS()   __eint();
#define DISABLE_INTS()  __dint();

//------------------------------------------------------------------------
class Thread;
/*!
    Class defining the architecture specific functions required by the 
    kernel.  
    
    This is limited (at this point) to a function to start the scheduler,
    and a function to initialize the default stack-frame for a thread.
*/
class ThreadPort
{
public:
    /*!        
        \fn void StartThreads()
       
        Function to start the scheduler, initial threads, etc.
    */
    static void StartThreads();
    friend class Thread;
private:

    /*!
        \fn void InitStack(Thread *pstThread_)
        
        Initialize the thread's stack.
        
        \param pstThread_ Pointer to the thread to initialize
    */
    static void InitStack(Thread *pstThread_);
};

#endif //__ThreadPORT_H_
