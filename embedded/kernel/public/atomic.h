/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012-2015 Funkenstein Software Consulting, all rights reserved.
See license.txt for more information
===========================================================================*/
/*!

    \file   atomic.h

    \brief  Basic Atomic Operations
*/

#ifndef __ATOMIC_H__
#define __ATOMIC_H__

#include "kerneltypes.h"
#include "mark3cfg.h"
#include "threadport.h"

#if KERNEL_USE_ATOMIC

/*!
 * \brief The Atomic class
 *
 * This utility class provides primatives for atomic operations - that is,
 * operations that are guaranteed to execute uninterrupted.  Basic atomic
 * primatives provided here include Set/Add/Delete for 8, 16, and 32-bit
 * integer types, as well as an atomic test-and-set.
 *
 */
class Atomic
{
public:
    /*!
     * \brief Set Set a variable to a given value in an uninterruptable operation
     * \param pu8Source_ Pointer to a variable to set the value of
     * \param u8Val_ New value to set in the variable
     * \return Previously-set value
     */
    static uint8_t Set( uint8_t *pu8Source_, uint8_t u8Val_ );
    static uint16_t Set( uint16_t *pu16Source_, uint16_t u16Val_ );
    static uint32_t Set( uint32_t *pu32Source_, uint32_t u32Val_ );

    /*!
     * \brief Add Add a value to a variable in an uninterruptable operation
     * \param pu8Source_ Pointer to a variable
     * \param u8Val_ Value to add to the variable
     * \return Previously-held value in pu8Source_
     */
    static uint8_t Add( uint8_t *pu8Source_, uint8_t u8Val_ );
    static uint16_t Add( uint16_t *pu16Source_, uint16_t u16Val_ );
    static uint32_t Add( uint32_t *pu32Source_, uint32_t u32Val_ );

    /*!
     * \brief Sub Subtract a value from a variable in an uninterruptable operation
     * \param pu8Source_ Pointer to a variable
     * \param u8Val_ Value to subtract from the variable
     * \return Previously-held value in pu8Source_
     */
    static uint8_t Sub( uint8_t *pu8Source_, uint8_t u8Val_ );
    static uint16_t Sub( uint16_t *pu16Source_, uint16_t u16Val_ );
    static uint32_t Sub( uint32_t *pu32Source_, uint32_t u32Val_ );

    /*!
     * \brief TestAndSet Test to see if a variable is set, and set it if
     *        is not already set.  This is an uninterruptable operation.
     *
     *        If the value is false, set the variable to true, and return
     *        the previously-held value.
     *
     *        If the value is already true, return true.
     *
     * \param pbLock Pointer to a value to test against.  This will always
     *        be set to "true" at the end of a call to TestAndSet.
     *
     * \return true - Lock value was "true" on entry, false - Lock was set
     */
    static bool TestAndSet( bool *pbLock );
};

#endif // KERNEL_USE_ATOMIC

#endif //__ATOMIC_H__
