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
    @file mark3cfg.h

    @brief Mark3 Kernel Configuration

    This file is used to configure the kernel for your specific application
    in order to provide the optimal set of features for a given use case.

    Note: in the full-throttle version of the kernel, all options are enabled
    by default.  As a result, the only configuration options presented are
    now located within the architecture-specific "portcfg.h".
*/
#pragma once

#define KERNEL_DEBUG (0)

#define KERNEL_STACK_CHECK (0)

#define KERNEL_NAMED_THREADS (0)

#define KERNEL_EVENT_FLAGS (1)

#define KERNEL_CONTEXT_SWITCH_CALLOUT (1)

#define KERNEL_THREAD_CREATE_CALLOUT (1)

#define KERNEL_THREAD_EXIT_CALLOUT (1)

#define KERNEL_ROUND_ROBIN (1)

#define KERNEL_EXTENDED_CONTEXT (0)

#include "portcfg.h" //!< include CPU/Port specific configuration options
