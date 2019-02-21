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

    @brief  Kernel Timer Implementation for ARM Cortex-A53
*/

#include "mark3.h"

using namespace Mark3;

// Generic timer address definitions
#define CORE0_TIMER_IRQCNTL (*(volatile uint32_t*)0x40000040)
#define CORE0_IRQ_SOURCE (*(volatile uint32_t*)0x40000060)

uint32_t g_uGenericTimerFreq;

namespace
{
//---------------------------------------------------------------------------
// Static objects implementing the timer thread and its synchronization objects
Thread    s_clTimerThread;
K_WORD    s_clTimerThreadStack[PORT_KERNEL_TIMERS_THREAD_STACK];
Semaphore s_clTimerSemaphore;

//---------------------------------------------------------------------------
void enable_cntv(void)
{
    uint32_t u32Val = 1;
    ASM("msr cntv_ctl_el0, %0 \n" ::"r"(u32Val));
}

//---------------------------------------------------------------------------
void write_cntv_tval(uint32_t u32Val)
{
    ASM(" msr cntv_tval_el0, %0 \n" ::"r"(u32Val));
}

//---------------------------------------------------------------------------
uint32_t read_cntfrq(void)
{
    uint32_t u32Val;
    ASM(" mrs %0, cntfrq_el0 \n" : "=r"(u32Val));
    return u32Val;
}
} // anonymous namespace

//---------------------------------------------------------------------------
extern "C" {

void TimerTick_Handler(void)
{
    if (!Kernel::IsStarted()) {
        return;
    }
    KernelTimer::ClearExpiry();
    Kernel::Tick();
    s_clTimerSemaphore.Post();
}
}

namespace Mark3
{
//---------------------------------------------------------------------------
static void KernelTimer_Task(void* unused)
{
    (void)unused;

    KernelTimer::Start();
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
    Quantum::SetTimerThread(&s_clTimerThread);
    s_clTimerThread.Start();
}

//---------------------------------------------------------------------------
void KernelTimer::Start(void)
{
    g_uGenericTimerFreq = read_cntfrq();

    write_cntv_tval(PORT_TIMER_FREQ); // Set timer tick

    CORE0_TIMER_IRQCNTL = 0x08; // Route timer interrupt to core 0

    enable_cntv();
}

//---------------------------------------------------------------------------
void KernelTimer::Stop(void)
{
    // Not implemented in this port
}

//---------------------------------------------------------------------------
PORT_TIMER_COUNT_TYPE KernelTimer::Read(void)
{
    // Not implemented in this port
    return 0;
}

//---------------------------------------------------------------------------
void KernelTimer::ClearExpiry(void)
{
    if (CORE0_IRQ_SOURCE & 0x08) {
        write_cntv_tval(PORT_TIMER_FREQ);
    }
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
