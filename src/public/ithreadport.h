#pragma once

#include <stdint.h>

namespace Mark3 {
//------------------------------------------------------------------------
class Thread;
/**
 *  Class defining the architecture specific functions required by the 
 *  kernel.  
 *  
 *  This is limited (at this point) to a function to start the scheduler,
 *  and a function to initialize the default stack-frame for a thread.
 */
class ThreadPort
{
public:
    /**
     * @brief Init
     *
     * Function to perform early init of the target environment prior to
     * using OS primatives.
     */
    static void Init() {}

    /**        
     *  @brief StartThreads
     *
     *  Function to start the scheduler, initial threads, etc.
     */
    static void StartThreads();
    friend class Thread;
private:

    /**
     *  @brief InitStack
     *
     *  Initialize the thread's stack.
     *  
     *  @param pstThread_ Pointer to the thread to initialize
     */
    static void InitStack(Thread *pstThread_);
};
} // namespace Mark3

