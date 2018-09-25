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

    @file   kernel.cpp

    @brief  Kernel initialization and startup code
*/

#include "mark3.h"
namespace Mark3
{
bool      Kernel::m_bIsStarted; //!< true if kernel is running, false otherwise
bool      Kernel::m_bIsPanic;   //!< true if kernel is in panic state, false otherwise
PanicFunc Kernel::m_pfPanic;    //!< set panic function

#if KERNEL_THREAD_CREATE_CALLOUT
ThreadCreateCallout  Kernel::m_pfThreadCreateCallout;  //!< Function to call on thread creation
#endif // #if KERNEL_THREAD_CREATE_CALLOUT
#if KERNEL_THREAD_EXIT_CALLOUT
ThreadExitCallout    Kernel::m_pfThreadExitCallout;    //!< Function to call on thread exit
#endif // #if KERNEL_THREAD_EXIT_CALLOUT
#if KERNEL_CONTEXT_SWITCH_CALLOUT
ThreadContextCallout Kernel::m_pfThreadContextCallout; //!< Function to call on context switch
#endif // #if KERNEL_CONTEXT_SWITCH_CALLOUT
DebugPrintFunction   Kernel::m_pfDebugPrintFunction;   //!< Function to call when printing debug info
#if KERNEL_STACK_CHECK
uint16_t             Kernel::m_u16GuardThreshold;
#endif // #if KERNEL_STACK_CHECK

//---------------------------------------------------------------------------
void Kernel::Init(void)
{
    AutoAlloc::Init();

    // Initialize the global kernel data - thread-scheduler, and timer-scheduler.
    Scheduler::Init();
    TimerScheduler::Init();
#if KERNEL_STACK_CHECK
    m_u16GuardThreshold = KERNEL_STACK_GUARD_DEFAULT;
#endif // #if KERNEL_STACK_CHECK
}

//---------------------------------------------------------------------------
void Kernel::Start(void)
{
    m_bIsStarted = true;
    ThreadPort::StartThreads();
}

//---------------------------------------------------------------------------
void Kernel::Panic(uint16_t u16Cause_)
{
    m_bIsPanic = true;
    if (m_pfPanic != nullptr) {
        m_pfPanic(u16Cause_);
    } else {
        while (true) {}
    }
}

//---------------------------------------------------------------------------
void Kernel::DebugPrint(const char* szString_)
{
    KERNEL_ASSERT(szString_ != nullptr);
    if (m_pfDebugPrintFunction != nullptr) {
        m_pfDebugPrintFunction(szString_);
    }
}

} // namespace Mark3
