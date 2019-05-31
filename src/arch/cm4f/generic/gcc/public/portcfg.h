/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012 - 2019 m0slevin, all rights reserved.
See license.txt for more information
===========================================================================*/
/**
    @file portcfg.h

    @brief Mark3 Port Configurationd

    This file is used to configure the kernel for your specific target CPU
    in order to provide the optimal set of features for a given use case.

    !! NOTE:  This file must ONLY be included from mark3cfg.h
*/
#pragma once

#include <stdint.h>

/**
    Define the number of thread priorities that the kernel's scheduler will
    support.  The number of thread priorities is limited only by the memory
    of the host CPU, as a ThreadList object is statically-allocated for each
    thread priority.

    In practice, systems rarely need more than 32 priority levels, with the
    most complex having the capacity for 256.
*/
#define KERNEL_NUM_PRIORITIES (32)

#define KERNEL_TIMERS_THREAD_PRIORITY (KERNEL_NUM_PRIORITIES - 1)

#define THREAD_QUANTUM_DEFAULT (4)

#define KERNEL_STACK_GUARD_DEFAULT (32) // words

/**
    Set the number of priorities supported by the coroutine scheduler.  The
    number of coroutine priorities is limited by the memory of the host CPU.
*/
#define PORT_COROUTINE_PRIORITIES (8)

/**
    Define a macro indicating the CPU architecture for which this port belongs.

    This may also be set by the toolchain, but that's not guaranteed.
*/
#ifndef ARM
#define ARM (1)
#endif

/**
    Define types that map to the CPU Architecture's default data-word and address
    size.
*/
#define K_WORD uint32_t //!< Size of a data word
#define K_ADDR uint32_t //!< Size of an address (pointer size)
#define K_INT int32_t

/**
    Set a base datatype used to represent each element of the scheduler's
    priority bitmap.

    PORT_PRIO_MAP_WORD_SIZE should map to the *size* of an element of type
    PORT_PROI_TYPE.
*/
#define PORT_PRIO_TYPE uint32_t     //!< Type used for bitmap in the PriorityMap class
#define PORT_PRIO_MAP_WORD_SIZE (4) //!< size of PORT_PRIO_TYPE in bytes

/**
    Define the running CPU frequency.  This may be an integer constant, or an alias
    for another variable which holds the CPU's current running frequency.
*/
#define PORT_SYSTEM_FREQ (120000000)

/**
    Set the timer frequency.  If running in tickless mode, this is simply the frequency
    at which the free-running kernel timer increments.

    In tick-based mode, this is the frequency at which the fixed-frequency kernel tick
    interrupt occurs.
*/
#define PORT_TIMER_FREQ ((uint32_t)(PORT_SYSTEM_FREQ / 1000)) // Timer ticks per second...

/**
    Define the default/minimum size of a thread stack
*/
#define PORT_KERNEL_DEFAULT_STACK_SIZE ((K_ADDR)512)

/**
    Define the size of the kernel-timer thread stack (if one is configured)
*/
#define PORT_KERNEL_TIMERS_THREAD_STACK ((K_ADDR)512)

/**
    Define the native type corresponding to the kernel timer hardware's counter register.
*/
#define PORT_TIMER_COUNT_TYPE uint32_t //!< Timer counter type

/**
    Minimum number of timer ticks for any delay or sleep, required to ensure that a timer cannot
    be initialized to a negative value.
*/
#define PORT_MIN_TIMER_TICKS (0)

/**
    Set this to 1 if the stack grows down in the target architecture, or 0 if the stack grows up
*/
#define PORT_STACK_GROWS_DOWN (1)

/**
    Set this to 1 if the target CPU/toolchain supports an optimized Count-leading-zeros instruction,
    or count-leading-zeros intrinsic.  If such functionality is not available, a general-purpose
    implementation will be used.
*/
#define PORT_USE_HW_CLZ   (1)
