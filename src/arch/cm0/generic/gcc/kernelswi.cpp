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

    @brief  Kernel Software interrupt implementation for ARM Cortex-M0

*/

#include "kerneltypes.h"
#include "kernelswi.h"
#include "threadport.h"
#include "m3_core_cm0.h"

namespace Mark3
{
//---------------------------------------------------------------------------
void KernelSWI::Config(void)
{
    M3_NVIC_SetPriority(M3_SVC_IRQn, (1 << __NVIC_PRIO_BITS) - 1);
    M3_NVIC_SetPriority(M3_PENDSV_IRQn, (1 << __NVIC_PRIO_BITS) - 1);
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
