/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012-2016 Funkenstein Software Consulting, all rights reserved.
See license.txt for more information
===========================================================================*/
/*!

    \file   kerneltimer.cpp

    \brief  Kernel Timer Implementation for SAM D20
*/

#include "kerneltypes.h"
#include "kerneltimer.h"
#include "mark3cfg.h"

#include <samd20.h>

#if KERNEL_TIMERS_TICKLESS
//---------------------------------------------------------------------------
#define KERNEL_TIMER_TC         TC0
#define KERNEL_TIMER_GCLK       (0)     //GCLK_0
#define KERNEL_TIMER_CLK        (19)    //CLK_TC_0

#define COUNT_OFFSET            (0x10)
#define CC0_OFFSET              (0x18)
#define CC1_OFFSET              (0x1A)

//---------------------------------------------------------------------------
static bool bEnabled = 1;

//---------------------------------------------------------------------------
static void WriteSync()
{
    while (KERNEL_TIMER_TC->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY)
    {
        /* Do Nothing */
    }
}

//---------------------------------------------------------------------------
static void ReadSync(uint16_t u16Offset_)
{
    KERNEL_TIMER_TC->COUNT16.READREQ.reg = TC_READREQ_RREQ |
                                    (((uint32_t)u16Offset_) << TC_READREQ_ADDR_Pos);

    WriteSync();
}

//---------------------------------------------------------------------------
void KernelTimer::Config(void)
{        
    uint32_t u32Reg;
    uint32_t u32CtrlA;
    uint32_t u32CtrlB;
    uint32_t u32CtrlC;
    uint32_t u32EventCtrl;
    uint32_t u32ReadReq;

    //--- Enable the clock in the power-management registers ---
    PM->APBCMASK.reg |= 1 << (PM_APBCMASK_TC0_Pos);

    u32Reg = (((uint32_t)KERNEL_TIMER_CLK) << GCLK_CLKCTRL_ID_Pos)
          | (((uint32_t)KERNEL_TIMER_GCLK) << GCLK_CLKCTRL_GEN_Pos);

    //--- Set the clock ID ---
    *((uint8_t*)&GCLK->CLKCTRL.reg) = (uint8_t)KERNEL_TIMER_CLK;

    //--- Disable clock, set config, Re-enable Clock
    GCLK->CLKCTRL.reg &= ~GCLK_CLKCTRL_CLKEN;
    GCLK->CLKCTRL.reg = u32Reg;
    GCLK->CLKCTRL.reg |= GCLK_CLKCTRL_CLKEN;

    //--- Setup Registers ---
    u32CtrlA = (((uint32_t)1) << TC_CTRLA_WAVEGEN_Pos)        // MATCH
            | (((uint32_t)0) << TC_CTRLA_MODE_Pos)           // 16-bit timer
            | (((uint32_t)6) << TC_CTRLA_PRESCALER_Pos)      // 256x prescalar
            | (((uint32_t)0) << TC_CTRLA_PRESCSYNC_Pos);     // Normal presync

    u32CtrlB = (((uint32_t)0) << TC_CTRLBSET_ONESHOT_Pos)     // Continuous mode (no 1-shot)
            | (((uint32_t)0) << TC_CTRLBSET_DIR_Pos);        // Count-up

    u32CtrlC = (((uint32_t)1) << TC_CTRLC_CPTEN_Pos)          // Enable Capture
            | (((uint32_t)0) << TC_CTRLC_INVEN_Pos);         // Disable invert

    u32EventCtrl = (((uint32_t)0) << TC_EVCTRL_MCEO_Pos)
            | (((uint32_t)1) << TC_EVCTRL_OVFEO_Pos)         // Enable overflow event
            | (((uint32_t)0) << TC_EVCTRL_TCEI_Pos)
            | (((uint32_t)0) << TC_EVCTRL_TCINV_Pos);

    u32ReadReq = (((uint32_t)1) << TC_READREQ_RCONT_Pos)
            | (((uint32_t)0x10) << TC_READREQ_ADDR_Pos);     // Constantly sync the COUNT value

    KERNEL_TIMER_TC->COUNT16.CTRLA.reg = u32CtrlA;
    WriteSync();
    KERNEL_TIMER_TC->COUNT16.CTRLBSET.reg = u32CtrlB;
    WriteSync();
    KERNEL_TIMER_TC->COUNT16.CTRLC.reg = u32CtrlC;
    WriteSync();
    KERNEL_TIMER_TC->COUNT16.READREQ.reg = u32ReadReq;
    WriteSync();
    KERNEL_TIMER_TC->COUNT16.EVCTRL.reg = u32EventCtrl;
    
    //--- Enable The Counter ---
    WriteSync();
    KERNEL_TIMER_TC->COUNT16.CTRLA.reg |= TC_CTRLA_ENABLE;

    KERNEL_TIMER_TC->COUNT16.INTFLAG.reg = TC_INTFLAG_OVF;
    KERNEL_TIMER_TC->COUNT16.INTENSET.reg = TC_INTENSET_OVF;

    WriteSync();
    KERNEL_TIMER_TC->COUNT16.CC[0].reg = 0xFFFF;

    //--- Enable Interrupts ---
    NVIC_EnableIRQ(TC0_IRQn);

    //--- Stop the counter ---
    
}

//---------------------------------------------------------------------------
void KernelTimer::Start(void)
{   
    WriteSync();
    KERNEL_TIMER_TC->COUNT16.CTRLA.reg |= TC_CTRLA_ENABLE;
}

//---------------------------------------------------------------------------
void KernelTimer::Stop(void)
{
    WriteSync();
    KERNEL_TIMER_TC->COUNT16.CTRLA.reg &= ~TC_CTRLA_ENABLE;
}

//---------------------------------------------------------------------------
uint16_t KernelTimer::Read(void)
{
    // If we're not continuously reading the count register...
    ReadSync(COUNT_OFFSET);
    return KERNEL_TIMER_TC->COUNT16.COUNT.reg;
}

//---------------------------------------------------------------------------
uint32_t KernelTimer::SubtractExpiry(uint32_t u32Interval_)
{
    uint16_t u16Val;

    ReadSync(CC0_OFFSET);
    u16Val = KERNEL_TIMER_TC->COUNT16.CC[0].reg;
    u16Val -= (uint16_t)u32Interval_;

    WriteSync();
    KERNEL_TIMER_TC->COUNT16.CC[0].reg = u16Val;

    return (uint32_t)u16Val;
}

//---------------------------------------------------------------------------
uint32_t KernelTimer::TimeToExpiry(void)
{
    uint16_t u16Read;
    uint16_t u16Top;

    u16Read = KernelTimer::Read();

    ReadSync(CC0_OFFSET);
    u16Top = KERNEL_TIMER_TC->COUNT16.CC[0].reg;

    if (u16Read >= u16Top)
    {
        return 0;
    }
    else
    {
        return (uint32_t)(u16Top - u16Read);
    }
}

//---------------------------------------------------------------------------
uint32_t KernelTimer::GetOvertime(void)
{
    return KernelTimer::Read();
}

//---------------------------------------------------------------------------
uint32_t KernelTimer::SetExpiry(uint32_t u32Interval_)
{
    uint16_t u16SetInterval;    
    if (u32Interval_ > 65535)
    {
        u16SetInterval = 65535;
    } 
    else 
    {
        u16SetInterval = (uint16_t)u32Interval_; 
    }

    WriteSync();
    KERNEL_TIMER_TC->COUNT16.CC[0].reg = u16SetInterval;
    return (uint32_t)u16SetInterval;
}

//---------------------------------------------------------------------------
void KernelTimer::ClearExpiry(void)
{
    WriteSync();
    KERNEL_TIMER_TC->COUNT16.CC[0].reg = 0xFFFF;
}

//---------------------------------------------------------------------------
uint8_t KernelTimer::DI(void)
{    
    uint8_t u8Ret = (bEnabled == true);

    KernelTimer::RI(0);
    if (bEnabled == true)
    {
        bEnabled = false;
    }
    return u8Ret;
}

//---------------------------------------------------------------------------
void KernelTimer::EI(void)
{
    KernelTimer::RI(1);
}

//---------------------------------------------------------------------------
void KernelTimer::RI(bool bEnable_)
{
    if (bEnable_)
    {
        NVIC_EnableIRQ(TC0_IRQn);
        bEnabled = true;
    }
    else
    {
        NVIC_DisableIRQ(TC0_IRQn);
        bEnabled = false;
    }
}
#else
//---------------------------------------------------------------------------
void KernelTimer::Config(void)
{
    
}

//---------------------------------------------------------------------------
void KernelTimer::Start(void)
{
    SysTick_Config(TIMER_FREQ); // 1KHz fixed clock...
    NVIC_EnableIRQ(SysTick_IRQn);
}

//---------------------------------------------------------------------------
void KernelTimer::Stop(void)
{
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

//---------------------------------------------------------------------------
uint16_t KernelTimer::Read(void)
{
    // Not implemented in this port
    return 0;
}

//---------------------------------------------------------------------------
uint32_t KernelTimer::SubtractExpiry(uint32_t u32Interval_)
{
    return 0;
}

//---------------------------------------------------------------------------
uint32_t KernelTimer::TimeToExpiry(void)
{
    return 0;
}

//---------------------------------------------------------------------------
uint32_t KernelTimer::GetOvertime(void)
{
    return 0;
}

//---------------------------------------------------------------------------
uint32_t KernelTimer::SetExpiry(uint32_t u32Interval_)
{
    return 0;
}

//---------------------------------------------------------------------------
void KernelTimer::ClearExpiry(void)
{
}

//-------------------------------------------------------------------------
uint8_t KernelTimer::DI(void)
{
    return 0;
}

//---------------------------------------------------------------------------
void KernelTimer::EI(void)
{
    KernelTimer::RI(0);
}

//---------------------------------------------------------------------------
void KernelTimer::RI(bool bEnable_)
{
}

//---------------------------------------------------------------------------
#endif
