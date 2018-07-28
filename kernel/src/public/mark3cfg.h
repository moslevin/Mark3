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
/*!
    \file mark3cfg.h

    \brief Mark3 Kernel Configuration

    This file is used to configure the kernel for your specific application
    in order to provide the optimal set of features for a given use case.

    Since you only pay the price (code space/RAM) for the features you use,
    you can usually find a sweet spot between features and resource usage
    by picking and choosing features a-la-carte.  This config file is
    written in an "interactive" way, in order to minimize confusion about
    what each option provides, and to make dependencies obvious.
*/
#pragma once

/*!
    Define the number of thread priorities that the kernel's scheduler will
    support.  The number of thread priorities is limited only by the memory
    of the host CPU, as a ThreadList object is statically-allocated for each
    thread priority.

    In practice, systems rarely need more than 32 priority levels, with the
    most complex having the capacity for 256.
*/
#define KERNEL_NUM_PRIORITIES (256)

/*!
    If you've opted to use the kernel timers module, you have an option
    as to which timer implementation to use:  Tick-based or Tick-less.

    Tick-based timers provide a "traditional" RTOS timer implementation
    based on a fixed-frequency timer interrupt.  While this provides
    very accurate, reliable timing, it also means that the CPU is being
    interrupted far more often than may be necessary (as not all timer
    ticks result in "real work" being done).

    Tick-less timers still rely on a hardware timer interrupt, but uses
    a dynamic expiry interval to ensure that the interrupt is only
    called when the next timer expires.  This increases the complexity
    of the timer interrupt handler, but reduces the number and frequency.

    Note that the CPU port (kerneltimer.cpp) must be implemented for the
    particular timer variant desired.
*/
#if defined(ARM)
# define KERNEL_TIMERS_TICKLESS (0)
#else
# define KERNEL_TIMERS_TICKLESS (1)
#endif

#if KERNEL_TIMERS_TICKLESS
/*!
    When using tickless timers, it is useful to define a minimum sleep
    value.  In the event that a delay/sleep/timeout value lower than this
    is provided to a timer-based API, the minimum value will be substituted.
*/
#define KERNEL_TIMERS_MINIMUM_DELAY_US (25)
#endif

#define KERNEL_TIMERS_THREAD_PRIORITY  (KERNEL_NUM_PRIORITIES - 1)


#define THREAD_QUANTUM_DEFAULT  (4)
/*!
    Provides extra logic for kernel debugging, and instruments the kernel
    with extra asserts, and kernel trace functionality.
*/
#define KERNEL_USE_DEBUG (0)

#define KERNEL_STACK_GUARD_DEFAULT (32) // words

#include "portcfg.h"    //!< include CPU/Port specific configuration options
