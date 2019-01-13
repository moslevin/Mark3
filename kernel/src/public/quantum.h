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
=========================================================================== */
/**

    @file   quantum.h

    @brief  Thread Quantum declarations for Round-Robin Scheduling

 */

#pragma once

#include "kerneltypes.h"
#include "mark3cfg.h"

#include "thread.h"
#include "timer.h"
#include "timerlist.h"
#include "timerscheduler.h"

#if KERNEL_ROUND_ROBIN
namespace Mark3
{
class Timer;

/**
 *  Static-class used to implement Thread quantum functionality, which is
 *  a key part of round-robin scheduling.
 */

class Quantum
{
public:
    static void Init();

    /**
     *  @brief SetInTimer
     *
     *  Set a flag to indicate that the CPU is currently running within the
     *  timer-callback routine.  This prevents the Quantum timer from being
     *  updated in the middle of a callback cycle, potentially resulting in
     *  the kernel timer becoming disabled.
     */
    static void SetInTimer();

    /**
     * @brief ClearInTimer
     *
     *  Clear the flag once the timer callback function has been completed.
     */
    static void ClearInTimer();

    /**
     * @brief Update
     *
     * Update the current thread being tracked for round-robing scheduling.
     * Note - this has no effect if called from the Timer thread, or if
     * the Timer thread is active.
     *
     * @param pclTargetThread_ New thread to track.
     */
    static void Update(Thread* pclTargetThread_);

    /**
     * @brief SetTimerThread
     *
     * Pass the timer thread's Thread pointer to the Quantum module to track
     * against requests to update the round-robin timer.
     *
     * @param pclTimerThread_ Pointer to the Timer thread's Thread object.
     */
    static void SetTimerThread(Thread* pclTimerThread_) { m_pclTimerThread = pclTimerThread_; }

    /**
     * Cancel the round-robin timer.
     */
    static void Cancel();

private:
    static Thread*  m_pclActiveThread;
    static Thread*  m_pclTimerThread;
    static uint16_t m_u16TicksRemain;
    static bool     m_bInTimer;
};
} // namespace Mark3
#endif // #if KERNEL_ROUND_ROBIN
