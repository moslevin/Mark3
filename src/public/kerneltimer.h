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

    @file   kerneltimer.h

    @brief  Kernel Timer Class declaration
 */

#pragma once

#include "kerneltypes.h"
#include "mark3cfg.h"

namespace Mark3
{
//---------------------------------------------------------------------------
/**
    Hardware timer interface, used by all scheduling/timer subsystems.
 */
class KernelTimer
{
public:
    /**
     *  @brief Config
     *
     *  Initializes the kernel timer before use
     */
    static void Config(void);

    /**
     *  @brief Start
     *
     *  Starts the kernel time (must be configured first)
     */
    static void Start(void);

    /**
     *  @brief Stop
     *
     *  Shut down the kernel timer, used when no timers are scheduled
     */
    static void Stop(void);
};
} // namespace Mark3
