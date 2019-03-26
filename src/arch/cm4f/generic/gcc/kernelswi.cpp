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

    @brief  Kernel Software interrupt implementation for ARM Cortex-M4

*/

#include "kerneltypes.h"
#include "kernelswi.h"

#include "m3_core_cm4.h"

namespace Mark3
{
//---------------------------------------------------------------------------
void KernelSWI::Config(void)
{
    uint8_t u8MinPriority = (uint8_t)((1 << __NVIC_PRIO_BITS) - 1);
    M3_NVIC_SetPriority(M3_SVC_IRQn, u8MinPriority);
    M3_NVIC_SetPriority(M3_PENDSV_IRQn, u8MinPriority);
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
