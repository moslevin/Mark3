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

    \brief  ATMega328p Multithreading support.
*/

#ifndef __THREADPORT_H_
#define __THREADPORT_H_

#include "kerneltypes.h"
#include "thread.h"

//---------------------------------------------------------------------------
//! ASM Macro - simplify the use of ASM directive in C
#define ASM(x)      asm volatile(x);

//---------------------------------------------------------------------------
//! Macro to find the top of a stack given its size and top address
#define TOP_OF_STACK(x, y)        (K_ULONG*) ( ((K_ULONG)x) + (y-1) )
//! Push a value y to the stack pointer x and decrement the stack pointer
#define PUSH_TO_STACK(x, y)        *x = y; x--;

//---------------------------------------------------------------------------
//! Save the context of the Thread
#define Thread_SaveContext()

//---------------------------------------------------------------------------
//! Restore the context of the Thread
#define Thread_RestoreContext()

//------------------------------------------------------------------------
//! These macros *must* be used in pairs !
//------------------------------------------------------------------------
//! Enter critical section (copy status register, disable interrupts)
#define CS_ENTER()

//------------------------------------------------------------------------
//! Exit critical section (restore status register)
#define CS_EXIT()

//------------------------------------------------------------------------
//! Initiate a contex switch without using the SWI
#define ENABLE_INTS()
#define DISABLE_INTS()

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
