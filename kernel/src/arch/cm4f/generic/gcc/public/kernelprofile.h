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
    @file kernelprofile.h

    @brief Profiling timer hardware interface
 */
#pragma once

#include "kerneltypes.h"
#include "mark3cfg.h"
#include "ll.h"

//---------------------------------------------------------------------------
#define TICKS_PER_OVERFLOW (256)
#define CLOCK_DIVIDE (8)

namespace Mark3
{
//---------------------------------------------------------------------------
/**
    System profiling timer interface
 */
class Profiler
{
public:
    /**
     *  @brief Init
     *
     *  Initialize the global system profiler.  Must be
     *  called prior to use.
     */
    static void Init();

    /**
     *  @brief Start
     *
     *  Start the global profiling timer service.
     */
    static void Start();

    /**
     *  @brief Stop
     *
     *  Stop the global profiling timer service
     */
    static void Stop();

    /**
     *  @brief Read
     *
     *  Read the current tick count in the timer.
     */
    static uint16_t Read();

    /**
     *  @brief Process
     *
     *  Process the profiling counters from ISR.
     */
    static void Process();

    /**
     *  @brief GetEpoch
     *
     *  Return the current timer epoch
     */
    static uint32_t GetEpoch() { return m_u32Epoch; }

private:
    static uint32_t m_u32Epoch;
};
} // namespace Mark3

