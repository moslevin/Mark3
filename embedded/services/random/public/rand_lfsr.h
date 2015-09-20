/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2014 Funkenstein Software Consulting, all rights reserved.
See license.txt for more information
===========================================================================*/
/*!
    \file rand_lfsr.h
    \brief Linear Feedback Shift Register based Pseudo-Random Number Generator
*/

#ifndef __RAND_LFSR_H__
#define __RAND_LFSR_H_

#include "kerneltypes.h"

//---------------------------------------------------------------------------
/*!
 * \brief The PseudoRandom class
 *
 * Pseudro-Random Number Generator based on a 32-bit linear-feedback
 * shift register.  For reference, the implementation is based on the
 * Maxim IC application note "AN4000"
 *
 * http://www.maximintegrated.com/app-notes/index.mvp/id/4400
 *
 */

//---------------------------------------------------------------------------
class PseudoRandom
{
public:
    /*!
     * \brief PseudoRandom
     *
     * Default constructor
     */
    PseudoRandom();

    /*!
     * \brief Seed
     *
     * Initialize the LFSR registers in the object, seeding the random
     * number generator.
     *
     * \param u32Seed1_ Seed value for LFSR register 1
     * \param u32Seed2_ Seed value for LFSR register 2
     */
    void Seed( uint32_t u32Seed1_, uint32_t u32Seed2_ );

    /*!
     * \brief GetRandom
     *
     * Return a 16-bit random number.
     *
     * \return 16-bit random number
     */
    uint16_t GetRandom();

private:

    /*!
     * \brief Shift
     *
     * Deep magic to drive the the LFSR state machine
     *
     * \param pu32LFSR_ Pointer to the register to drive
     * \param u32Mask_ Bitmask to apply as an XOR
     * \return new value in the LFSR register
     */
    uint32_t Shift( uint32_t *pu32LFSR_, uint32_t u32Mask_ );

    uint32_t m_u32LFSR1;
    uint32_t m_u32LFSR2;

};

#endif

