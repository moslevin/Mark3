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
    @file mark3cfg.h

    @brief Mark3 Kernel Configuration
    This file is used to configure the kernel for your specific application
    in order to provide the optimal set of features for a given use case.

    Note: in the R7 and beyond version of the kernel, all options are enabled
    by default.  As a result, the only configuration options presented are
    now located within the architecture-specific "portcfg.h".
*/
#pragma once

/**
 * Enable kernel asserts at runtime.
 */
#define KERNEL_DEBUG (0)

/**
 * Perform stack-depth checks on threads at each context switch, which is useful
 * in detecting stack overflows / near overflows.  Near-overflow detection uses
 * thresholds defined in the target's portcfg.h.  Enabling this also adds the
 * Thread::GetStackSlack() method, which allows a thread's stack to be profiled
 * on-demand.
 *
 * Note:  When enabled, the additional stack checks result in a performance hit
 * to context switches and thread initialization.
 *
 */
#define KERNEL_STACK_CHECK (1)

/**
 * Enabling this provides the Thread::SetName() and Thread::GetName() methods,
 * allowing for each thread to be named with a null-terminated const char* string.
 *
 * Note: the string passed to Thread::SetName() must persist for the lifetime of
 * the thread
 */
#define KERNEL_NAMED_THREADS (1)

/**
 * This flag enables the event-flags synchronization object.  This feature allows
 * threads to be blocked, waiting on specific condition bits to be set or cleared
 * on an EventFlag object.
 *
 * While other synchronization objects are enabled by default, this one is configurable
 * because it impacts the Thread object's member data.
 */
#define KERNEL_EVENT_FLAGS (1)

/**
 * When enabled, this feature allows a user to define a callback to be executed
 * whenever a context switch occurs.  Enabling this provides a means for a user
 * to track thread statistics, but it does result in additional overhead during
 * a context switch.
 */
#define KERNEL_CONTEXT_SWITCH_CALLOUT (1)

/**
 * This feature provides a user-defined kernel callback that is executed whenever
 * a thread is started.
 */
#define KERNEL_THREAD_CREATE_CALLOUT (1)

/**
 * This feature provides a user-defined kernel callback that is executed whenever
 * a thread is terminated.
 */
#define KERNEL_THREAD_EXIT_CALLOUT (1)

/**
 * Enable round-robin scheduling within each priority level.  When selected, this
 * results in a small performance hit during context switching and in the system
 * tick handler, as a special software timer is used to manage the running thread's
 * quantum.  Can be disabled to optimize performance if not required.
 */
#define KERNEL_ROUND_ROBIN (1)

/**
 * Provide a special data pointer in the thread object, which may be used to add
 * additional context to a thread.  Typically this would be used to implement
 * thread-local-storage.
 */
#define KERNEL_EXTENDED_CONTEXT (1)

#include "portcfg.h" //!< include CPU/Port specific configuration options
