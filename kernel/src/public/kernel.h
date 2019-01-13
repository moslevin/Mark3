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
===========================================================================*/
/**

    @file   kernel.h

    @brief  Kernel initialization and startup class

    The Kernel namespace provides functions related to initializing and
    starting up the kernel.

    The Kernel::Init() function must be called before any of the other
    functions in the kernel can be used.

    Once the initial kernel configuration has been completed (i.e. first
    threads have been added to the scheduler), the Kernel::Start()
    function can then be called, which will transition code execution from
    the "main()" context to the threads in the scheduler.
*/

#pragma once

#include "mark3cfg.h"
#include "kerneltypes.h"
#include "paniccodes.h"
#include "thread.h"

using DebugPrintFunction = void (*)(const char* szString_);

namespace Mark3
{
//---------------------------------------------------------------------------
/**
 *    Class that encapsulates all of the kernel startup functions.
 */
class Kernel
{
public:
    /**
     *  @brief
     *
     *  Kernel Initialization Function, call before any other OS function
     *
     *  Initializes all global resources used by the operating system.  This
     *  must be called before any other kernel function is invoked.
     */
    static void Init(void);

    /**
     *  @brief
     *
     *  Start the operating system kernel - the current execution context is
     *  cancelled, all kernel services are started, and the processor resumes
     *  execution at the entrypoint for the highest-priority thread.
     *
     *  You must have at least one thread added to the kernel before calling
     *  this function, otherwise the behavior is undefined.  The exception to
     *  this is if the system is configured to use the threadless idle hook,
     *  in which case the kernel is allowed to run without any ready threads.
     */
    static void Start(void);

    /**
     * @brief IsStarted
     *
     * @return Whether or not the kernel has started - true = running, false =
     *        not started
     */
    static bool IsStarted() { return m_bIsStarted; }
    /**
     * @brief SetPanic Set a function to be called when a kernel panic occurs,
     *        giving the user to determine the behavior when a catastrophic
     *        failure is observed.
     *
     * @param pfPanic_ Panic function pointer
     */
    static void SetPanic(PanicFunc pfPanic_) { m_pfPanic = pfPanic_; }
    /**
     * @brief IsPanic Returns whether or not the kernel is in a panic state
     * @return Whether or not the kernel is in a panic state
     */
    static bool IsPanic() { return m_bIsPanic; }
    /**
     * @brief Panic Cause the kernel to enter its panic state
     * @param u16Cause_ Reason for the kernel panic
     */
    static void Panic(uint16_t u16Cause_);

#if KERNEL_THREAD_CREATE_CALLOUT
    /**
     * @brief SetThreadCreateCallout
     *
     * Set a function to be called on creation of a new thread.  This
     * callout is executed on the successful completion of a Thread::Init()
     * call.  A callout is only executed if this method has been called
     * to set a valid handler function.
     *
     * @param pfCreate_ Pointer to a function to call on thread creation
     */
    static void SetThreadCreateCallout(ThreadCreateCallout pfCreate_) { m_pfThreadCreateCallout = pfCreate_; }
#endif // #if KERNEL_THREAD_CREATE_HOOK

#if KERNEL_THREAD_EXIT_CALLOUT
    /**
     * @brief SetThreadExitCallout
     *
     * Set a function to be called on thread exit.  This callout is executed
     * from the beginning of Thread::Exit().
     *
     * A callout is only executed if this method has been called to set a
     * valid handler function.
     *
     * @param pfExit_ Pointer to a function to call on thread exit
     */
    static void SetThreadExitCallout(ThreadExitCallout pfExit_) { m_pfThreadExitCallout = pfExit_; }
#endif // #if KERNEL_THREAD_EXIT_CALLOUT

#if KERNEL_CONTEXT_SWITCH_CALLOUT
    /**
     * @brief  SetThreadContextSwitchCallout
     *
     * Set a function to be called on each context switch.
     *
     * A callout is only executed if this method has been called to set a
     * valid handler function.
     *
     * @param pfContext_ Pointer to a function to call on context switch
     */
    static void SetThreadContextSwitchCallout(ThreadContextCallout pfContext_)
    {
        m_pfThreadContextCallout = pfContext_;
    }
#endif // KERNEL_CONTEXT_SWITCH_CALLOUT

    /**
     * @brief SetDebugPrintFunction
     *
     * Set the function to be used when printing kernel debug information
     *
     * @param pfPrintFunction_
     */
    static void SetDebugPrintFunction(DebugPrintFunction pfPrintFunction_)
    {
        m_pfDebugPrintFunction = pfPrintFunction_;
    }

    /**
     * @brief DebugPrint
     *
     * Print a string to the configured output interface.  Has no effect
     * if Kernel::SetDebugPrintFunction() has not been called with a valid
     * print handler.
     *
     * @param szString_ string to print
     */
    static void DebugPrint(const char* szString_);

#if KERNEL_THREAD_CREATE_CALLOUT
    /**
     * @brief GetThreadCreateCallout
     *
     * Return the current function called on every Thread::Init();
     *
     * @return Pointer to the currently-installed callout function,
     *         or nullptr if not set.
     */
    static ThreadCreateCallout GetThreadCreateCallout(void) { return m_pfThreadCreateCallout; }
#endif // #if KERNEL_THREAD_CREATE_HOOK
#if KERNEL_THREAD_EXIT_CALLOUT
    /**
     * @brief GetThreadExitCallout
     * Return the current function called on every Thread::Exit();
     *
     * @return Pointer to the currently-installed callout function,
     *         or nullptr if not set.
     */
    static ThreadExitCallout GetThreadExitCallout(void) { return m_pfThreadExitCallout; }
#endif // #if KERNEL_THREAD_EXIT_HOOK
#if KERNEL_CONTEXT_SWITCH_CALLOUT
    /**
     * @brief GetThreadContextSwitchCallout
     * Return the current function called on every Thread::ContextSwitchSWI()
     *
     * @return Pointer to the currently-installed callout function,
     *         or nullptr if not set.
     */
    static ThreadContextCallout GetThreadContextSwitchCallout(void) { return m_pfThreadContextCallout; }
#endif // #if KERNEL_CONTEXT_SWITCH_CALLOUT
#if KERNEL_STACK_CHECK
    static void     SetStackGuardThreshold(uint16_t u16Threshold_) { m_u16GuardThreshold = u16Threshold_; }
    static uint16_t GetStackGuardThreshold(void) { return m_u16GuardThreshold; }
#endif // #if KERNEL_STACK_CHECK

private:
    static bool      m_bIsStarted; //!< true if kernel is running, false otherwise
    static bool      m_bIsPanic;   //!< true if kernel is in panic state, false otherwise
    static PanicFunc m_pfPanic;    //!< set panic function

#if KERNEL_THREAD_CREATE_CALLOUT
    static ThreadCreateCallout m_pfThreadCreateCallout; //!< Function to call on thread creation
#endif                                                  // #if KERNEL_THREAD_CREATE_HOOK
#if KERNEL_THREAD_EXIT_CALLOUT
    static ThreadExitCallout m_pfThreadExitCallout; //!< Function to call on thread exit
#endif                                              // #if KERNEL_THREAD_EXIT_HOOK
#if KERNEL_CONTEXT_SWITCH_CALLOUT
    static ThreadContextCallout m_pfThreadContextCallout; //!< Function to call on context switch
#endif                                                    // #if KERNEL_CONTEXT_SWITCH_CALLOUT
    static DebugPrintFunction m_pfDebugPrintFunction;     //!< Function to call to print debug info
#if KERNEL_STACK_CHECK
    static uint16_t m_u16GuardThreshold;
#endif // #if KERNEL_STACK_CHECK
};

} // namespace Mark3
