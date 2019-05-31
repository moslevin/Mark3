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

    @file   kernelswi.cpp

    @brief  Kernel Software interrupt implementation for ARM Cortex-M4

*/

#include "kerneltypes.h"
#include "kernelswi.h"

#include "stm32f4xx.h"

namespace Mark3
{
//---------------------------------------------------------------------------
void KernelSWI::Config(void)
{
    uint8_t u8MinPriority = (uint8_t)((1 << __NVIC_PRIO_BITS) - 1);
    NVIC_SetPriority(SVCall_IRQn, u8MinPriority);
    NVIC_SetPriority(PendSV_IRQn, u8MinPriority);
    SCB->ICSR = SCB_ICSR_PENDSVCLR_Msk;
}

//---------------------------------------------------------------------------
void KernelSWI::Start(void)
{
    // Nothing to do...
}

//---------------------------------------------------------------------------
void KernelSWI::Trigger(void)
{
    SCB->ICSR = SCB_ICSR_PENDSVSET_Msk;
}

} // namespace Mark3
