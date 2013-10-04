/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012 Funkenstein Software Consulting, all rights reserved.
See license.txt for more information
===========================================================================*/
/*!

    \file   threadport.h    

    \brief  Cortex M-0 Multithreading support.
*/

#ifndef __THREADPORT_H_
#define __THREADPORT_H_

#include "kerneltypes.h"
#include "thread.h"

//---------------------------------------------------------------------------
//! ASM Macro - simplify the use of ASM directive in C
#define ASM      asm volatile

//---------------------------------------------------------------------------
//! Macro to find the top of a stack given its size and top address
#define TOP_OF_STACK(x, y)        (K_WORD*) ( ((K_ULONG)x) + (y - sizeof(K_WORD)) )
//! Push a value y to the stack pointer x and decrement the stack pointer
#define PUSH_TO_STACK(x, y)        *x = y; x--;

//------------------------------------------------------------------------
//! These macros *must* be used in matched-pairs !
//! Nesting *is* supported !
//------------------------------------------------------------------------
//! Enter critical section (copy current PRIMASK register value, disable interrupts)
#define CS_ENTER()	\
{	\
	K_ULONG __ulRegState;	\
	asm	( \
	" mrs r0, PRIMASK \n"	\
	" mov %[STATUS], r0 \n" \
	" cpsid i \n "	\
	: [STATUS] "=r" (__ulRegState) \
	);

//------------------------------------------------------------------------
//! Exit critical section (restore previous PRIMASK status register value)
#define CS_EXIT() \
	asm	( \
	" mov r0, %[STATUS] \n" \
	" msr primask, r0 \n"	\
	: \
	: [STATUS] "r" (__ulRegState) \
	); \
}
 
//------------------------------------------------------------------------
#define ENABLE_INTS()		ASM(" cpsie i \n");
#define DISABLE_INTS()		ASM(" cpsid i \n");

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
