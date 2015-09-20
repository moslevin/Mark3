/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2013 Funkenstein Software Consulting, all rights reserved.
See license.txt for more information
===========================================================================*/
/*!
  \file  sys_clocks.cpp
  \brief Class implementation for Atmel SAM-D20 clocks
 */

#include "sys_clocks.h"

//---------------------------------------------------------------------------
SysClock::SysClock()
{
    m_eClock = CLK_DFLL48;
    m_eClockGen = GCLK_0;
    m_bLock = false;
}

//---------------------------------------------------------------------------
SysClock::SysClock( Clock_t eClock_ )
{
    SysClock();
    if (eClock_ < CLK_COUNT)
    {
        m_eClock = eClock_;
    }
}

//---------------------------------------------------------------------------
ClockError_t SysClock::SetClockID( Clock_t eClock_ )
{
    if (eClock_ >= CLK_COUNT)
    {
        return CLK_ERR_RANGE;
    }
    m_eClock = eClock_;
    return CLK_ERR_EOK;
}

//---------------------------------------------------------------------------
ClockError_t SysClock::SetGenerator( ClockGenerator_t eClockGen_ )
{
    if (eClockGen_ >= GCLK_COUNT)
    {
        return CLK_ERR_RANGE;
    }
    m_eClockGen = eClockGen_;
    return CLK_ERR_EOK;
}

//---------------------------------------------------------------------------
void SysClock::LockOnEnable( bool bLock_ )
{
    m_bLock = bLock_;
}

//---------------------------------------------------------------------------
ClockError_t SysClock::Setup( Clock_t eClock_, ClockGenerator_t eClockGen_, bool bLock_ )
{
    ClockError_t eErr;
    eErr = SetClockID(eClock_);
    if (eErr != CLK_ERR_EOK)
    {
        return eErr;
    }
    eErr = SetGenerator(eClockGen_);
    if (eErr != CLK_ERR_EOK)
    {
        return eErr;
    }
    LockOnEnable(bLock_);
    return CLK_ERR_EOK;
}

//---------------------------------------------------------------------------
void SysClock::CommitConfig( void )
{
    uint32_t u32Config = 0;
    u32Config |= ((uint32_t)m_eClock) << GCLK_CLKCTRL_ID_Pos;
    u32Config |= ((uint32_t)m_eClockGen) << GCLK_CLKCTRL_GEN_Pos;
    if (m_bLock)
    {
        u32Config |= GCLK_CLKCTRL_WRTLOCK;
    }
    Enable(false);
    GCLK->CLKCTRL.reg = u32Config;
}

//---------------------------------------------------------------------------
void SysClock::Enable( bool bEnable_ )
{
    WriteChannel();
    if (bEnable_)
    {
        GCLK->CLKCTRL.reg |= GCLK_CLKCTRL_CLKEN;
    }
    else
    {
        GCLK->CLKCTRL.reg &= ~GCLK_CLKCTRL_CLKEN;
        while (GCLK->CLKCTRL.reg & GCLK_CLKCTRL_CLKEN) {}
    }
}

//---------------------------------------------------------------------------
void SysClock::WriteChannel()
{
    *((uint8_t*)&GCLK->CLKCTRL.reg) = (uint8_t)m_eClock;
}

//---------------------------------------------------------------------------
SysClockGenerator::SysClockGenerator()
{
    m_eClockGen			= GCLK_0;
    m_eDivMode			= DIV_MODE_LINEAR;
    m_eClockSrc			= GCLK_SRC_OSC8M;
    m_u16Divisor			= 0;
    m_bRunInStandby		= true;
    m_bOutputToPin		= false;
    m_bOutputOffValue	= false;
    m_bImproveDutyCycle = true;
}

//---------------------------------------------------------------------------
SysClockGenerator::SysClockGenerator( ClockGenerator_t eClockGen_ )
{
    SysClockGenerator();
    m_eClockGen = eClockGen_;
}

//---------------------------------------------------------------------------
ClockError_t SysClockGenerator::SetGenerator( ClockGenerator_t eGen_ )
{
    if (eGen_ >= GCLK_COUNT)
    {
        return CLK_ERR_RANGE;
    }
    m_eClockGen = eGen_;
    return CLK_ERR_EOK;
}

//---------------------------------------------------------------------------
void SysClockGenerator::SetClockSource( ClockGenSource_t eClockSrc_ )
{
    m_eClockSrc = eClockSrc_;
}

//---------------------------------------------------------------------------
void SysClockGenerator::OutputOffState( bool bState_ )
{
    m_bOutputOffValue = bState_;
}

//---------------------------------------------------------------------------
void SysClockGenerator::PinOutput( bool bOutput_ )
{
    m_bOutputToPin = bOutput_;
}

//---------------------------------------------------------------------------
void SysClockGenerator::RunInStandby( bool bOutput_ )
{
    m_bRunInStandby = bOutput_;
}

//---------------------------------------------------------------------------
void SysClockGenerator::ImproveDutyCycle( bool bImprove_ )
{
    m_bImproveDutyCycle = bImprove_;
}

//---------------------------------------------------------------------------
void SysClockGenerator::SetDivisor( ClockDivideMode_t eMode_, uint16_t u16Divisor_ )
{
    m_eDivMode = eMode_;
    m_u16Divisor = u16Divisor_;
}

//---------------------------------------------------------------------------
void SysClockGenerator::CommitConfig(void)
{
    uint32_t u32GenCtrl = 0;
    uint32_t u32GenDiv = 0;

    u32GenCtrl = m_eClockGen << GCLK_GENCTRL_ID_Pos
              | m_eClockSrc << GCLK_GENCTRL_SRC_Pos;

    if (m_bImproveDutyCycle)
    {
        u32GenCtrl |= GCLK_GENCTRL_IDC;
    }
    if (m_bRunInStandby)
    {
        u32GenCtrl |= GCLK_GENCTRL_RUNSTDBY;
    }
    if (m_eDivMode)
    {
        u32GenCtrl |= GCLK_GENCTRL_DIVSEL;
    }
    if (m_bOutputToPin)
    {
        u32GenCtrl |= GCLK_GENCTRL_OE;
    }
    if (m_bOutputOffValue)
    {
        u32GenCtrl |= GCLK_GENCTRL_OOV;
    }

    u32GenDiv = m_eClockGen << GCLK_GENDIV_ID_Pos
             | m_u16Divisor << GCLK_GENDIV_DIV_Pos;

    WriteSync();
    WriteGeneratorDiv();
    WriteSync();

    GCLK->GENDIV.reg = u32GenDiv;

    WriteSync();

    GCLK->GENCTRL.reg = u32GenCtrl | (GCLK->GENCTRL.reg & GCLK_GENCTRL_GENEN);
}

//---------------------------------------------------------------------------
bool SysClockGenerator::IsSyncing()
{
    if (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY)
    {
        return true;
    }
    return false;
}

//---------------------------------------------------------------------------
void SysClockGenerator::WriteSync()
{
    while (IsSyncing());
}

//---------------------------------------------------------------------------
void SysClockGenerator::WriteGenerator()
{
    *((uint8_t*)&GCLK->GENCTRL.reg) = (uint8_t)m_eClockGen;
}

//---------------------------------------------------------------------------
void SysClockGenerator::WriteGeneratorDiv()
{
    *((uint8_t*)&GCLK->GENDIV.reg) = (uint8_t)m_eClockGen;
}

//---------------------------------------------------------------------------
Source_OSC8M::Source_OSC8M()
{
	m_ePrescalar = OSC8M_PRESCALAR_1;
	m_bOnDemand = true;
	m_bRunStandby = false;
}

//---------------------------------------------------------------------------
void Source_OSC8M::Prescalar(OSC8M_Prescaler_t ePrescalar_)
{
	m_ePrescalar = ePrescalar_;
}

//---------------------------------------------------------------------------
void Source_OSC8M::RunOnDemand(bool bRunOnDemand_)
{
	m_bOnDemand = bRunOnDemand_;
}

//---------------------------------------------------------------------------
void Source_OSC8M::RunInStandby(bool bRunInStandby_)
{
	m_bRunStandby = bRunInStandby_;
}

//---------------------------------------------------------------------------
void Source_OSC8M::CommitConfig()
{
	uint32_t u32Reg;
	u32Reg = SYSCTRL->OSC8M.reg;
	u32Reg &= ~( SYSCTRL_OSC8M_PRESC_Msk
				| SYSCTRL_OSC8M_ONDEMAND
				| SYSCTRL_OSC8M_RUNSTDBY
			  );

	if (m_ePrescalar)
	{
		u32Reg |= (((uint32_t)m_ePrescalar) << SYSCTRL_OSC8M_PRESC_Pos);
	}
	if (m_bRunStandby)
	{
		u32Reg |= SYSCTRL_OSC8M_RUNSTDBY;
	}
	if (m_bOnDemand)
	{
		u32Reg |= SYSCTRL_OSC8M_ONDEMAND;
	}

	SYSCTRL->OSC8M.reg = u32Reg;
}

//---------------------------------------------------------------------------
void Source_OSC8M::Enable(bool bEnable_)
{
	if (bEnable_)
	{
		SYSCTRL->OSC8M.reg |= SYSCTRL_OSC8M_ENABLE;
	}
	else
	{
		SYSCTRL->OSC8M.reg &= ~SYSCTRL_OSC8M_ENABLE;
	}
}

//---------------------------------------------------------------------------
Source_XOSC32K::Source_XOSC32K()
{
	m_eStartupTime = XOSC32K_STARTUP_32K;
	m_bEnable32KHz = true;
	m_bEnable1KHz = false;
	m_bAutoAmp = true;
	m_bExternalXtal = true;
	m_bOnDemand = true;
	m_bRunStandby = false;
}

//---------------------------------------------------------------------------
void Source_XOSC32K::StartupTime(XOSC32K_StartupTime_t eTime_)
{
	m_eStartupTime = eTime_;
}

//---------------------------------------------------------------------------
void Source_XOSC32K::Enable32KHz(bool bEnable_)
{
	m_bEnable32KHz = bEnable_;
}

//---------------------------------------------------------------------------
void Source_XOSC32K::Enable1KHz(bool bEnable_)
{
	m_bEnable1KHz = bEnable_;
}

//---------------------------------------------------------------------------
void Source_XOSC32K::GainAmplifier(bool bEnable_)
{
	m_bAutoAmp = bEnable_;
}

//---------------------------------------------------------------------------
void Source_XOSC32K::ExternalCrystal(bool bEnable_)
{
	m_bExternalXtal = bEnable_;
}

//---------------------------------------------------------------------------
void Source_XOSC32K::RunOnDemand(bool bRunOnDemand_)
{
	m_bOnDemand = bRunOnDemand_;
}

//---------------------------------------------------------------------------
void Source_XOSC32K::RunInStandby(bool bRunInStandby_)
{
	m_bRunStandby = bRunInStandby_;
}

//---------------------------------------------------------------------------
void Source_XOSC32K::CommitConfig()
{
	uint32_t u32Reg;
	u32Reg = SYSCTRL->XOSC32K.reg;
	u32Reg &= ~( SYSCTRL_XOSC32K_STARTUP_Msk
				| SYSCTRL_XOSC32K_EN1K
				| SYSCTRL_XOSC32K_EN32K
				| SYSCTRL_XOSC32K_AAMPEN
				| SYSCTRL_XOSC32K_ENABLE
				| SYSCTRL_XOSC32K_XTALEN
				| SYSCTRL_XOSC32K_ONDEMAND
				| SYSCTRL_XOSC32K_RUNSTDBY
				);

	u32Reg |= ((uint32_t)m_eStartupTime << SYSCTRL_XOSC32K_STARTUP_Pos);

	if (m_bAutoAmp)
	{
		u32Reg |= SYSCTRL_XOSC32K_AAMPEN;
	}
	if (m_bEnable1KHz)
	{
		u32Reg |= SYSCTRL_XOSC32K_EN1K;
	}
	if (m_bEnable32KHz)
	{
		u32Reg |= SYSCTRL_XOSC32K_EN32K;
	}
	if (m_bExternalXtal)
	{
		u32Reg |= SYSCTRL_XOSC32K_XTALEN;
	}
	if (m_bOnDemand)
	{
		u32Reg |= SYSCTRL_XOSC32K_ONDEMAND;
	}
	if (m_bRunStandby)
	{
		u32Reg |= SYSCTRL_XOSC32K_RUNSTDBY;
	}
}

//---------------------------------------------------------------------------
void Source_XOSC32K::Enable(bool bEnable_)
{
	if (bEnable_)
	{
		SYSCTRL->XOSC32K.reg |= SYSCTRL_XOSC32K_ENABLE;
	}
	else
	{
		SYSCTRL->XOSC32K.reg &= ~SYSCTRL_XOSC32K_ENABLE;
	}
}

//---------------------------------------------------------------------------
Source_DFLL48M::Source_DFLL48M()
{
	m_bRunStandby = false;
	m_bOnDemand = true;
	m_bChillCycle = true;
	m_bQuickLock = true;
	m_bLoseLockAfterWake = true;
	m_bClockMode = true;
	
	m_u16Multiplier = 6;
	m_u8MaxCoarseStep = 1;
	m_u16MaxFineStep = 1;
	
	m_u8CoarseAdjust = (0x1F / 4);
	m_u16FineAdjust = (0xFF / 4);
}

//---------------------------------------------------------------------------
void Source_DFLL48M::ClockMode(bool bClosedLoop_)
{
	m_bClockMode = bClosedLoop_;
}

//---------------------------------------------------------------------------
void Source_DFLL48M::RunOnDemand(bool bRunOnDemand_)
{
	m_bOnDemand = bRunOnDemand_;
}

//---------------------------------------------------------------------------
void Source_DFLL48M::RunInStandby(bool bRunInStandby_)
{
	m_bRunStandby = bRunInStandby_;
}

//---------------------------------------------------------------------------
void Source_DFLL48M::ChillCycle(bool bEnable_)
{
	m_bChillCycle = bEnable_;
}

//---------------------------------------------------------------------------
void Source_DFLL48M::LoseLockAfterWakeup(bool bEnable_)
{
	m_bLoseLockAfterWake = bEnable_;
}

//---------------------------------------------------------------------------
void Source_DFLL48M::QuickLock(bool bEnable_)
{
	m_bQuickLock = bEnable_;
}

//---------------------------------------------------------------------------
void Source_DFLL48M::Multiplier(uint16_t u16Multiplier_)
{
	m_u16Multiplier = u16Multiplier_;
}

//---------------------------------------------------------------------------
void Source_DFLL48M::MaxCoarseStep(uint8_t u8Step_)
{
	m_u8MaxCoarseStep = u8Step_;
}

//---------------------------------------------------------------------------
void Source_DFLL48M::MaxFineStep(uint16_t u16Step_)
{
	m_u16MaxFineStep = u16Step_;
}

//---------------------------------------------------------------------------
void Source_DFLL48M::CoarseAdjust(uint8_t u8Val_)
{
	m_u8CoarseAdjust = u8Val_;
}

//---------------------------------------------------------------------------
void Source_DFLL48M::FineAdjust(uint16_t u16Val_)
{
	m_u16FineAdjust = u16Val_;
}

//---------------------------------------------------------------------------
void Source_DFLL48M::CommitConfig()
{
	uint32_t u32Mul = 0;
	uint32_t u32Val = 0;
	uint32_t u32Ctrl = 0;
	SysClock clClock;
	
	if (m_bClockMode)
	{
		u32Ctrl |= SYSCTRL_DFLLCTRL_MODE;
	}
	
	if (!m_bChillCycle)	// In the register,  Disabled = 1
	{
		u32Ctrl |= SYSCTRL_DFLLCTRL_CCDIS;
	}
	
	if (!m_bQuickLock) // In the register, Disabled = 1
	{
		u32Ctrl |= SYSCTRL_DFLLCTRL_QLDIS;
	}
	
	if (m_bLoseLockAfterWake)
	{
		u32Ctrl |= SYSCTRL_DFLLCTRL_LLAW;
	}
	
	if (m_bOnDemand)
	{
		u32Ctrl |= SYSCTRL_DFLLCTRL_ONDEMAND;
	}
	
	if (m_bRunStandby)
	{
		u32Ctrl |= SYSCTRL_DFLLCTRL_RUNSTDBY;
	}
	
	// Only set multiplier in closed-loop mode.
	if (m_bClockMode)
	{
		u32Mul = ( ((uint32_t)m_u16Multiplier) << SYSCTRL_DFLLMUL_MUL_Pos )
		| (((uint32_t)m_u8MaxCoarseStep) << SYSCTRL_DFLLMUL_CSTEP_Pos)
		| (((uint32_t)m_u16MaxFineStep) << SYSCTRL_DFLLMUL_FSTEP_Pos)
		;
	}
	
	u32Val =   ( ((uint32_t)m_u8CoarseAdjust) << SYSCTRL_DFLLVAL_COARSE_Pos)
	| ( ((uint32_t)m_u16FineAdjust) << SYSCTRL_DFLLVAL_FINE_Pos)
	;

	SYSCTRL->DFLLCTRL.reg = u32Ctrl & ~SYSCTRL_DFLLCTRL_ONDEMAND;

	WaitSync();
	
	SYSCTRL->DFLLMUL.reg = u32Mul;
	SYSCTRL->DFLLVAL.reg = u32Val;
	
	SYSCTRL->DFLLCTRL.reg = u32Ctrl;
	WaitSync();
}

void Source_DFLL48M::Enable(bool bEnable_)
{
	WaitSync();
	if (bEnable_)
	{
		SYSCTRL->DFLLCTRL.reg |= SYSCTRL_DFLLCTRL_ENABLE;
	}
	else
	{
		SYSCTRL->DFLLCTRL.reg &= ~SYSCTRL_DFLLCTRL_ENABLE;
	}
}

void Source_DFLL48M::WaitSync()
{
	while (!(SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_DFLLRDY)) {}
}