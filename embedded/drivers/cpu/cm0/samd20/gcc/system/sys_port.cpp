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
  \file sys_port.cpp
  \brief Class Implementation for Atmel SAM-D20 Port and Pinmux functionality
*/

#include "sys_port.h"

//---------------------------------------------------------------------------
SysPort::SysPort()
{
    m_ePort = SYS_PORT_A;
    m_pstPort = GetPortPointer();
}

//---------------------------------------------------------------------------
SysPort::SysPort(PortIdentity_t ePort_)
{
    if (ePort_ > SYS_PORT_COUNT)
    {
        m_ePort = SYS_PORT_A;
    } else {
        m_ePort = ePort_;
    }

    m_pstPort = GetPortPointer();
}

//---------------------------------------------------------------------------
void SysPort::SetDir(uint8_t u8PinIndex_, bool bOutput_)
{
    if (bOutput_)
    {
        m_pstPort->DIRSET.reg = (1 << u8PinIndex_);
    }
    else
    {
        m_pstPort->DIRCLR.reg = (1 << u8PinIndex_);
    }
}

//---------------------------------------------------------------------------
void SysPort::SetOut(uint8_t u8PinIndex_, bool bLevel_)
{
    if (bLevel_)
    {
        m_pstPort->OUTSET.reg = (1 << u8PinIndex_);
    }
    else
    {
        m_pstPort->OUTCLR.reg = (1 << u8PinIndex_);
    }
}

//---------------------------------------------------------------------------
void SysPort::ToggleOut(uint8_t u8PinIndex_)
{
    m_pstPort->OUTTGL.reg = (1 << u8PinIndex_);
}

//---------------------------------------------------------------------------
bool SysPort::GetIn(uint8_t u8PinIndex_)
{
    return ((m_pstPort->IN.reg >> u8PinIndex_) & 0x1);
}

//---------------------------------------------------------------------------
void SysPort::SetPinConfig(uint8_t u8PinIndex_, bool bpu32lUp_, bool bInputEnable_, bool bMuxWithPeripheral_)
{
    uint32_t u32Reg;
    uint32_t u32PinBit = 1 << ((uint32_t)(u8PinIndex_ & 0x0F));

    u32Reg = PORT_WRCONFIG_WRPINCFG
            | (u32PinBit << PORT_WRCONFIG_PINMASK_Pos)
            ;

    if (u8PinIndex_ >= 16)
    {
        u32Reg |= PORT_WRCONFIG_HWSEL; // Accessing upper bits in the register
    }
    if (bpu32lUp_)
    {
        u32Reg |= PORT_WRCONFIG_pu32LEN;
    }
    if (bInputEnable_)
    {
        u32Reg |= PORT_WRCONFIG_INEN;
    }
    if (bMuxWithPeripheral_)
    {
        u32Reg |= PORT_WRCONFIG_PMUXEN;
    }

    m_pstPort->WRCONFIG.reg = u32Reg;
}

//---------------------------------------------------------------------------
void SysPort::SetPortMux(uint8_t u8PinIndex_, PinMux_t eMuxFunction_)
{
    uint32_t u32Reg;
    uint32_t u32PinBit = 1 << ((uint32_t)(u8PinIndex_ & 0x0F));

    u32Reg =  PORT_WRCONFIG_WRPMUX
            | (u32PinBit << PORT_WRCONFIG_PINMASK_Pos)
            | (eMuxFunction_ << PORT_WRCONFIG_PMUX_Pos)
            ;

    if (u8PinIndex_ >= 16)
    {
        u32Reg |= PORT_WRCONFIG_HWSEL; // Accessing upper bits in the register
    }

    m_pstPort->WRCONFIG.reg = u32Reg;
}

//---------------------------------------------------------------------------
PortGroup *SysPort::GetPortPointer()
{
    PortGroup *pstPort = 0;
    switch (m_ePort)
    {
        case SYS_PORT_A:
        {
            pstPort = &PORT->Group[0];
        }
            break;
        case SYS_PORT_B:
        {
            pstPort = &PORT->Group[1];
        }
            break;
        default:
            pstPort = &PORT->Group[0];
            break;
    }
    return pstPort;
}

//---------------------------------------------------------------------------
SysPort PortA(SYS_PORT_A);
SysPort PortB(SYS_PORT_B);

