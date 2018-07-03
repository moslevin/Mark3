/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012 - 2017 Funkenstein Software Consulting, all rights reserved.
See license.txt for more information
===========================================================================*/
/*!

    \file   kerneltimer.cpp

    \brief  Kernel Timer Implementation for ARM Cortex-M3
*/

#include "kerneltypes.h"
#include "kerneltimer.h"
#include "threadport.h"
#include "m3_core_cm3.h"
#include "kernel.h"
#include "ksemaphore.h"
#include "kernelprofile.h"
#include "thread.h"

namespace {
//---------------------------------------------------------------------------
// Static objects implementing the timer thread and its synchronization objects
#if KERNEL_TIMERS_THREADED
Thread s_clTimerThread;
K_WORD s_clTimerThreadStack[PORT_KERNEL_TIMERS_THREAD_STACK];
Semaphore s_clTimerSemaphore;
#endif

} // anonymous namespace

using namespace Mark3;

//---------------------------------------------------------------------------
extern "C" {
void SysTick_Handler(void)
{
    if (!Kernel::IsStarted()) {
        return;
    }
#if KERNEL_TIMERS_THREADED
    KernelTimer::ClearExpiry();
    s_clTimerSemaphore.Post();
#else
    #if KERNEL_USE_TIMERS
        TimerScheduler::Process();
    #endif
    #if KERNEL_USE_QUANTUM
        Quantum::UpdateTimer();
    #endif
#endif
#if KERNEL_USE_PROFILER
    Profiler::Process();
#endif

    // Clear the systick interrupt pending bit.
    SCB->ICSR = SCB_ICSR_PENDSTCLR_Msk;
}
}

namespace Mark3 {

//---------------------------------------------------------------------------
#if KERNEL_TIMERS_THREADED
static void KernelTimer_Task(void* unused)
{
    (void)unused;
    while(1) {
        s_clTimerSemaphore.Pend();
#if KERNEL_USE_TIMERS
        TimerScheduler::Process();
#endif
#if KERNEL_USE_QUANTUM
        Quantum::UpdateTimer();
#endif
    }
}
#endif

//---------------------------------------------------------------------------
void KernelTimer::Config(void)
{
#if KERNEL_TIMERS_THREADED
    s_clTimersSemaphore.Init(0, 1);
    s_clTimerThread.Init(s_clTimerThreadStack,
                        sizeof(s_clTimerThreadStack) / sizeof(K_WORD),
                        KERNEL_TIMERS_THREAD_PRIORITY,
                        KernelTimer_Task,
                        0);
    Quantum::SetTimerThread(&s_clTimerThread);
    s_clTimerThread.Start();
#endif
}

//---------------------------------------------------------------------------
void KernelTimer::Start(void)
{
    // Barely higher priority than the SVC and PendSV interrupts.
    uint8_t u8Priority = static_cast<uint8_t>((1 << __NVIC_PRIO_BITS) - 2);

    M3_SysTick_Config(PORT_TIMER_FREQ); // 1KHz fixed clock...
    M3_NVIC_SetPriority(M3_SYSTICK_IRQn, u8Priority);
    M3_NVIC_EnableIRQ(M3_SYSTICK_IRQn);
}

//---------------------------------------------------------------------------
void KernelTimer::Stop(void)
{
    SysTick->CTRL = ~SysTick_CTRL_ENABLE_Msk;
}

//---------------------------------------------------------------------------
uint16_t KernelTimer::Read(void)
{
    // Not implemented in this port
    return (uint16_t)(SysTick->VAL);
}

//---------------------------------------------------------------------------
uint32_t KernelTimer::SubtractExpiry(uint32_t u32Interval_)
{
    return 0;
}

//---------------------------------------------------------------------------
uint32_t KernelTimer::TimeToExpiry(void)
{
    return 0;
}

//---------------------------------------------------------------------------
uint32_t KernelTimer::GetOvertime(void)
{
    return 0;
}

//---------------------------------------------------------------------------
uint32_t KernelTimer::SetExpiry(uint32_t u32Interval_)
{
    return 0;
}

//---------------------------------------------------------------------------
void KernelTimer::ClearExpiry(void)
{
}

//-------------------------------------------------------------------------
uint8_t KernelTimer::DI(void)
{
    return 0;
}

//---------------------------------------------------------------------------
void KernelTimer::EI(void)
{
    KernelTimer::RI(0);
}

//---------------------------------------------------------------------------
void KernelTimer::RI(bool bEnable_)
{
}

//---------------------------------------------------------------------------
} // namespace Mark3
