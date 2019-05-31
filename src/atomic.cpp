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

    @file   atomic.cpp

    @brief  Basic Atomic Operations
*/

#include "mark3.h"

namespace Mark3
{
//---------------------------------------------------------------------------
bool Atomic::TestAndSet(bool* pbLock_)
{
    KERNEL_ASSERT(nullptr != pbLock_);

    auto cs = CriticalGuard{};
    auto bRet = *pbLock_;
    if (!bRet) {
        *pbLock_ = 1;
    }
    return bRet;
}
} // namespace Mark3
