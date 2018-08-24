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
    @file kerneldebug.h

    @brief Macros and functions used for assertions, kernel traces, etc.
*/
#pragma once

#include "mark3cfg.h"
#include "paniccodes.h"
#include "kernel.h"

//---------------------------------------------------------------------------
namespace Mark3
{
#if KERNEL_DEBUG
#define KERNEL_ASSERT(x)                                                                                               \
    do {                                                                                                               \
        if ((x) == 0) {                                                                                                \
            Kernel::Panic(PANIC_ASSERT_FAILED);                                                                        \
        }                                                                                                              \
    } while (0);
#else
#define KERNEL_ASSERT(x)
#endif

} // namespace Mark3
