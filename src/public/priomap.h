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
    @file priomap.h
    @brief Priority map data structure
*/

#pragma once

#include "kerneltypes.h"
#include "mark3cfg.h"

#include "priomapl1.h"
#include "priomapl2.h"

namespace Mark3 {
#if KERNEL_NUM_PRIORITIES <= (PORT_PRIO_MAP_WORD_SIZE * 8u)
using PriorityMap = PriorityMapL1<PORT_PRIO_TYPE, KERNEL_NUM_PRIORITIES>;
#else
using PriorityMap = PriorityMapL2<PORT_PRIO_TYPE, KERNEL_NUM_PRIORITIES>;
#endif
} // namespace Mark3
