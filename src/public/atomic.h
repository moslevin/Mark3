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

    @file   atomic.h

    @brief  Basic Atomic Operations
*/

#pragma once

#include "kerneltypes.h"
#include "mark3cfg.h"
#include "ithreadport.h"
#include "kerneldebug.h"

namespace Mark3
{
/**
 * @brief The Atomic namespace
 *
 * This utility module provides primatives for atomic operations - that is,
 * operations that are guaranteed to execute uninterrupted.  Basic atomic
 * primatives provided here include Set/Add/Subtract, as well as an atomic
 * test-and-set.
 *
 */
namespace Atomic
{
    /**
     * @brief Set Set a variable to a given value in an uninterruptable operation
     * @param pSource_ Pointer to a variable to set the value of
     * @param val_ New value to set in the variable
     * @return Previously-set value
     */
    template <typename T> T Set(T* pSource_, T val_)
    {
        KERNEL_ASSERT(nullptr != pSource_);

        auto ret = T {};
        CS_ENTER();
        ret       = *pSource_;
        *pSource_ = val_;
        CS_EXIT();
        return ret;
    }

    /**
     * @brief Add Add a value to a variable in an uninterruptable operation
     * @param pSource_ Pointer to a variable
     * @param val_ Value to add to the variable
     * @return Previously-held value in pSource_
     */
    template <typename T> T Add(T* pSource_, T val_)
    {
        KERNEL_ASSERT(nullptr != pSource_);

        auto ret = T {};
        CS_ENTER();
        ret = *pSource_;
        *pSource_ += val_;
        CS_EXIT();
        return ret;
    }

    /**
     * @brief Sub Subtract a value from a variable in an uninterruptable operation
     * @param pSource_ Pointer to a variable
     * @param val_ Value to subtract from the variable
     * @return Previously-held value in pSource_
     */
    template <typename T> T Sub(T* pSource_, T val_)
    {
        KERNEL_ASSERT(nullptr != pSource_);

        auto ret = T {};
        CS_ENTER();
        ret = *pSource_;
        *pSource_ -= val_;
        CS_EXIT();
        return ret;
    }

    /**
     * @brief TestAndSet Test to see if a variable is set, and set it if
     *        is not already set.  This is an uninterruptable operation.
     *
     *        If the value is false, set the variable to true, and return
     *        the previously-held value.
     *
     *        If the value is already true, return true.
     *
     * @param pbLock Pointer to a value to test against.  This will always
     *        be set to "true" at the end of a call to TestAndSet.
     *
     * @return true - Lock value was "true" on entry, false - Lock was set
     */
    bool TestAndSet(bool* pbLock);
} // namespace Atomic
} // namespace Mark3
