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
===========================================================================*/
/**

    @file   ithreadport.h

    @brief  Thread porting interface
*/
#pragma once

#include <stdint.h>

namespace Mark3
{
//------------------------------------------------------------------------
class Thread;
/**
 *  @brief The ThreadPort Class defines the target-specific functions
 *  required by the kernel for threading.
 *
 *  This is limited (at this point) to a function to start the scheduler,
 *  and a function to initialize the default stack-frame for a thread.
 */
class ThreadPort
{
public:
    /**
     * @brief Init
     * Function to perform early init of the target environment prior to
     * using OS primatives.
     */
    static void Init() {}

    /**
     *  @brief StartThreads
     *  Function to start the scheduler, initial threads, etc.
     */
    static void StartThreads();
    friend class Thread;

private:
    /**
     *  @brief InitStack
     *  Initialize the thread's stack.
     *
     *  @param pstThread_ Pointer to the thread to initialize
     */
    static void InitStack(Thread* pstThread_);
};
} // namespace Mark3
