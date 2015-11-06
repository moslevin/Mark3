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

#include "rand_lfsr.h"

//---------------------------------------------------------------------------
#define POLY_MASK_32 0xB4BCD35C
#define POLY_MASK_31 0x7A5BC2E3

//---------------------------------------------------------------------------
PseudoRandom::PseudoRandom()
{
    m_u32LFSR1 = 0xABCDE;
    m_u32LFSR2 = 0x23456789;
}

//---------------------------------------------------------------------------
void PseudoRandom::Seed( uint32_t u32Seed1_, uint32_t u32Seed2_ )
{
    m_u32LFSR1 = u32Seed1_;
    m_u32LFSR2 = u32Seed2_;
}

//---------------------------------------------------------------------------
uint16_t PseudoRandom::GetRandom()
{
    Shift(&m_u32LFSR2, POLY_MASK_32);
    return (uint16_t)( Shift(&m_u32LFSR2, POLY_MASK_32) ^
                       Shift(&m_u32LFSR1, POLY_MASK_31) );
}

//---------------------------------------------------------------------------
uint32_t PseudoRandom::Shift( uint32_t *pu32LFSR_, uint32_t u32Mask_ )
{
    uint32_t u32Feedback;

    u32Feedback = *pu32LFSR_ & 1;
    *pu32LFSR_ >>= 1;
    if (u32Feedback)
    {
        *pu32LFSR_ ^= u32Mask_;
    }
    return *pu32LFSR_;
}
