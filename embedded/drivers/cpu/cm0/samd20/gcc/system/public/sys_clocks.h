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
  \file sys_clocks.h
  \brief Classes and type definitions for Atmel SAM-D20 clocks
*/

#ifndef __SYS_CLOCKS_H__
#define __SYS_CLOCKS_H__

#include "kerneltypes.h"
#include <samd20.h>

//---------------------------------------------------------------------------
// Enumeration defining the various Clock Generator IDs in the system.
typedef enum
{
    GCLK_0 = 0,
    GCLK_1,
    GCLK_2,
    GCLK_3,
    GCLK_4,
    GCLK_5,
    GCLK_6,
    GCLK_7,
//---
    GCLK_COUNT  //!< Number of GCLKs in the system
} ClockGenerator_t;

//---------------------------------------------------------------------------
// Enumeration of various clocks in the system
typedef enum
{
    CLK_DFLL48 = 0,
    CLK_WDT,
    CLK_RTC,
    CLK_EIC,
    CLK_EVSYS_CHANNEL_0,
    CLK_EVSYS_CHANNEL_1,
    CLK_EVSYS_CHANNEL_2,
    CLK_EVSYS_CHANNEL_3,
    CLK_EVSYS_CHANNEL_4,
    CLK_EVSYS_CHANNEL_5,
    CLK_EVSYS_CHANNEL_6,
    CLK_EVSYS_CHANNEL_7,
    CLK_SERCOMX_SLOW,
    CLK_SERCOM0_CORE,
    CLK_SERCOM1_CORE,
    CLK_SERCOM2_CORE,
    CLK_SERCOM3_CORE,
    CLK_SERCOM4_CORE,
    CLK_SERCOM5_CORE,
    CLK_TC0_1,
    CLK_TC2_3,
    CLK_TC4_5,
    CLK_TC6_7,
    CLK_ADC,
    CLK_AC_DIG,
    CLK_AC_ANA,
    CLK_DAC,
    CLK_PTC,
//----
    CLK_COUNT //!< Number of clocks in the system
} Clock_t;

//---------------------------------------------------------------------------
// Different clock divider modes
typedef enum
{
    DIV_MODE_LINEAR = 0,
    DIV_MODE_EXPONENTIAL,
//----
    DIV_MODE_NUM    //!< Number of clock divider modes
} ClockDivideMode_t;

//---------------------------------------------------------------------------
// Error codes used by the Clock APIs
typedef enum
{
    CLK_ERR_EOK = 0,
    CLK_ERR_ARGS,
    CLK_ERR_RANGE,
    CLK_ERR_LOCKED,
//----
    SYS_ERR_NUM    //!< Number of error codes
} ClockError_t;

//---------------------------------------------------------------------------
// Enumeration of all clock sources in the system
typedef enum
{
	GCLK_SRC_XOSC = 0,
	GCLK_SRC_GCLKIN,
	GCLK_SRC_GCLKGEN1,
	GCLK_SRC_OSCULP32K,
	GCLK_SRC_OSC32K,
	GCLK_SRC_XOSC32K,
	GCLK_SRC_OSC8M,
	GCLK_SRC_DFLL48M,
//----
    GCLK_SRC_NUM	//!< Number of clock generator sources
} ClockGenSource_t;

//---------------------------------------------------------------------------
class SysClock
{
public:
    SysClock();

    SysClock( Clock_t eClock_ );

    ClockError_t SetClockID( Clock_t eClock_ );

    ClockError_t SetGenerator( ClockGenerator_t eClockGen_ );

    void LockOnEnable( K_BOOL bLock_ );

    ClockError_t Setup( Clock_t eClock_, ClockGenerator_t eClockGen_, K_BOOL bLock_ );

    void CommitConfig( void );

    void Enable( K_BOOL bEnable_ );

private:
    void WriteChannel();

    Clock_t             m_eClock;
    ClockGenerator_t    m_eClockGen;
    K_BOOL                m_bLock;
};

//---------------------------------------------------------------------------
class SysClockGenerator
{
public:
    SysClockGenerator();

    SysClockGenerator( ClockGenerator_t eClockGen_ );

    ClockError_t SetGenerator( ClockGenerator_t eGen_ );

    void SetClockSource( ClockGenSource_t eClockSrc_ );

    void OutputOffState( K_BOOL bState_ );

    void PinOutput( K_BOOL bOutput_ );

    void RunInStandby( K_BOOL bOutput_ );

    void ImproveDutyCycle( K_BOOL bImprove_ );

    void SetDivisor( ClockDivideMode_t eMode_, K_USHORT usDivisor_ );

    void CommitConfig(void);

private:

    K_BOOL IsSyncing();
    void WriteSync();
    void WriteGenerator();
    void WriteGeneratorDiv();

	ClockGenSource_t	m_eClockSrc;
    ClockGenerator_t    m_eClockGen;

    K_USHORT            m_usDivisor;
    ClockDivideMode_t   m_eDivMode;

    K_BOOL                m_bRunInStandby;
    K_BOOL                m_bOutputToPin;
    K_BOOL                m_bOutputOffValue;
    K_BOOL                m_bImproveDutyCycle;
};

//---------------------------------------------------------------------------
typedef enum
{
    OSC8M_PRESCALAR_1 = 0,
    OSC8M_PRESCALAR_2,
    OSC8M_PRESCALAR_4,
    OSC8M_PRESCALAR_8,
//----
    OSC8M_PRESCALAR_COUNT
} OSC8M_Prescaler_t;

//---------------------------------------------------------------------------
class Source_OSC8M
{
public:
    Source_OSC8M();

    void Prescalar(OSC8M_Prescaler_t ePrescalar_);

    void RunOnDemand(K_BOOL bRunOnDemand_);

    void RunInStandby(K_BOOL bRunInStandby_);

    void CommitConfig();
	
    void Enable(K_BOOL bEnable_);

private:
    OSC8M_Prescaler_t m_ePrescalar;
    K_BOOL             m_bOnDemand;
    K_BOOL             m_bRunStandby;
};

//---------------------------------------------------------------------------
typedef enum
{
    XOSC32K_STARTUP_1 = 0,
    XOSC32K_STARTUP_32,
    XOSC32K_STARTUP_2K,
    XOSC32K_STARTUP_4K,
    XOSC32K_STARTUP_16K,
    XOSC32K_STARTUP_32K,
    XOSC32K_STARTUP_64K,
    XOSC32K_STARTUP_128K,
//----
    XOSCS32K_STARTUP_COUNT
} XOSC32K_StartupTime_t;

//---------------------------------------------------------------------------
class Source_XOSC32K
{
public:
    Source_XOSC32K();
    
    void StartupTime(XOSC32K_StartupTime_t eTime_);
    
    void Enable32KHz(K_BOOL bEnable_);

    void Enable1KHz(K_BOOL bEnable_);

    void GainAmplifier(K_BOOL bEnable_);

    void ExternalCrystal(K_BOOL bEnable_);

    void RunOnDemand(K_BOOL bRunOnDemand_);

    void RunInStandby(K_BOOL bRunInStandby_);

    void CommitConfig();
	
    void Enable(K_BOOL bEnable_);

private:
    XOSC32K_StartupTime_t m_eStartupTime;
    K_BOOL            m_bEnable32KHz;
    K_BOOL            m_bEnable1KHz;
    K_BOOL            m_bAutoAmp;
    K_BOOL            m_bExternalXtal;
    K_BOOL            m_bOnDemand;
    K_BOOL            m_bRunStandby;
};

//---------------------------------------------------------------------------
class Source_DFLL48M
{
public:

	Source_DFLL48M();
	
	void ClockMode(K_BOOL bClosedLoop_);
	
	void RunOnDemand(K_BOOL bRunOnDemand_);
	
    void RunInStandby(K_BOOL bRunInStandby_);
	
	void ChillCycle(K_BOOL bEnable_);
	
	void LoseLockAfterWakeup(K_BOOL bEnable_);
	
	void QuickLock(K_BOOL bEnable_);
	
	void Multiplier(K_USHORT usMultiplier_);
	
	void MaxCoarseStep(K_UCHAR ucStep_);
	
	void MaxFineStep(K_USHORT usStep_);
	
	void CoarseAdjust(K_UCHAR ucVal_);
	
	void FineAdjust(K_USHORT usVal_);
	
	void CommitConfig();
	
	void Enable(K_BOOL bEnable_);
	
private:
	void WaitSync();
	
	//-- Control register values
	K_BOOL		m_bRunStandby;
	K_BOOL		m_bOnDemand;
	K_BOOL		m_bChillCycle;
	K_BOOL		m_bQuickLock;
	K_BOOL		m_bLoseLockAfterWake;
	K_BOOL		m_bClockMode;
	
	//-- Multiplier register values
	K_USHORT	m_usMultiplier;
	K_USHORT	m_usMaxFineStep;
	K_UCHAR		m_ucMaxCoarseStep;
	
	//-- Value register values
	K_USHORT	m_usFineAdjust;
	K_UCHAR		m_ucCoarseAdjust;
};

#endif // __SYS_CLOCKS_H__
