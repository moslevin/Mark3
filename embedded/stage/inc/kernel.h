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

    \file   kernel.h    

    \brief  Kernel initialization and startup class
    
    The Kernel namespace provides functions related to initializing and 
    starting up the kernel.
    
    The Kernel::Init() function must be called before any of the other
    functions in the kernel can be used.
    
    Once the initial kernel configuration has been completed (i.e. first 
    threads have been added to the scheduler), the Kernel::Start() 
    function can then be called, which will transition code execution from
    the "main()" context to the threads in the scheduler.    
*/

#ifndef __KERNEL_H__
#define __KERNEL_H__

#include "kerneltypes.h"
//---------------------------------------------------------------------------
/*!
	Class that encapsulates all of the kernel startup functions.
*/
class Kernel
{
public:    
    /*!
	    Kernel Initialization Function, call before any other OS function
        
		\fn Init()
        
        Initializes all global resources used by the operating system.  This 
        must be called before any other kernel function is invoked.
    */
	static void Init(void);
        
    /*!
		Start the kernel; function never returns
	
        \fn Start()
        
        Start the operating system kernel - the current execution context is
        cancelled, all kernel services are started, and the processor resumes
        execution at the entrypoint for the highest-priority thread.

        You must have at least one thread added to the kernel before calling
        this function, otherwise the behavior is undefined.
    */
	static void Start(void);	
};

#endif

