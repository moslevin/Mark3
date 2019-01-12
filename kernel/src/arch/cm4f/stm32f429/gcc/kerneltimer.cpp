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

    @file   kerneltimer.cpp

    @brief  Kernel Timer Implementation for ARM Cortex-M4
*/

#include "kerneltypes.h"
#include "kerneltimer.h"
#include "threadport.h"
#include "kernel.h"
#include "ksemaphore.h"
#include "kernelprofile.h"
#include "thread.h"
#include "quantum.h"

#include "stm32f4xx.h"

extern "C" {
	extern void HAL_IncTick(void);
};

using namespace Mark3;
namespace
{
//---------------------------------------------------------------------------
// Static objects implementing the timer thread and its synchronization objects
Thread    s_clTimerThread;
K_WORD    s_clTimerThreadStack[PORT_KERNEL_TIMERS_THREAD_STACK];
Semaphore s_clTimerSemaphore;
} // anonymous namespace

using namespace Mark3;

//---------------------------------------------------------------------------
extern "C" {
void SysTick_Handler(void)
{
	HAL_IncTick();

    if (!Kernel::IsStarted()) {
        return;
    }

    s_clTimerSemaphore.Post();

    // Clear the systick interrupt pending bit.
    SCB->ICSR = SCB_ICSR_PENDSTCLR_Msk;
}
}

namespace Mark3
{
//---------------------------------------------------------------------------
static void KernelTimer_Task(void* unused)
{
    (void)unused;
    while (1) {
        s_clTimerSemaphore.Pend();
#if KERNEL_ROUND_ROBIN
        Quantum::SetInTimer();
#endif // #if KERNEL_ROUND_ROBIN
        TimerScheduler::Process();
#if KERNEL_ROUND_ROBIN
        Quantum::ClearInTimer();
#endif // #if KERNEL_ROUND_ROBIN
    }
}

//---------------------------------------------------------------------------
void KernelTimer::Config(void)
{
    s_clTimerSemaphore.Init(0, 1);
    s_clTimerThread.Init(s_clTimerThreadStack,
                         sizeof(s_clTimerThreadStack) / sizeof(K_WORD),
                         KERNEL_TIMERS_THREAD_PRIORITY,
                         KernelTimer_Task,
                         0);
#if KERNEL_ROUND_ROBIN
    Quantum::SetTimerThread(&s_clTimerThread);
#endif // #if KERNEL_ROUND_ROBIN
    s_clTimerThread.Start();
}

//---------------------------------------------------------------------------
void KernelTimer::Start(void)
{
    // Barely higher priority than the SVC and PendSV interrupts.
    uint8_t u8Priority = static_cast<uint8_t>((1 << __NVIC_PRIO_BITS) - 2);

    SysTick_Config(PORT_TIMER_FREQ); // 1KHz fixed clock...
    NVIC_SetPriority(SysTick_IRQn, u8Priority);
    NVIC_EnableIRQ(SysTick_IRQn);
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
    return 0;
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
void KernelTimer::RI(bool bEnable_) {}

//---------------------------------------------------------------------------
} // namespace Mark3
