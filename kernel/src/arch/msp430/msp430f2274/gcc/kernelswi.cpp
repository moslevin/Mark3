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

    @file   kernelswi.cpp

    @brief  Kernel Software interrupt implementation for ATMega328p

*/

#include "kerneltypes.h"
#include "kernelswi.h"
#include <msp430.h>

namespace Mark3
{
//---------------------------------------------------------------------------
void KernelSWI::Config(void)
{
    P1DIR &= ~0x04;
}

//---------------------------------------------------------------------------
void KernelSWI::Start(void)
{
    P1IE = 0x04;
}

//---------------------------------------------------------------------------
void KernelSWI::Trigger(void)
{
    P1IFG |= 0x04;
}

} // using namespace Mark3
