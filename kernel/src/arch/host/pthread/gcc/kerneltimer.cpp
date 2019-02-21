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

    @brief  Kernel Timer Implementation for ATMega328p
*/

#include "mark3.h"

#include <stdio.h>

namespace
{
using namespace Mark3;
//---------------------------------------------------------------------------
// Static objects implementing the timer thread and its synchronization objects
Thread    s_clTimerThread;
K_WORD    s_clTimerThreadStack[PORT_KERNEL_TIMERS_THREAD_STACK];
Semaphore s_clTimerSemaphore;
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
}

//---------------------------------------------------------------------------
void KernelTimer::Stop(void)
{
}

//---------------------------------------------------------------------------
PORT_TIMER_COUNT_TYPE KernelTimer::Read(void)
{
    return 0;
}

//---------------------------------------------------------------------------
uint8_t KernelTimer::DI(void)
{
}

//---------------------------------------------------------------------------
void KernelTimer::EI(void)
{
}

//---------------------------------------------------------------------------
void KernelTimer::RI(bool bEnable_)
{

}

//---------------------------------------------------------------------------
void KernelTimer::ISR()
{
    Kernel::Tick();
    s_clTimerSemaphore.Post();
}
} // namespace Mark3



