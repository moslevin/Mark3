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
/*!

    \file   drvRAM.cpp

    \brief  Generic RAM-buffer driver

*/

#include "kerneltypes.h"
#include "drvRAM.h"
#include "driver.h"

namespace Mark3
{
//---------------------------------------------------------------------------
int RAMDriver::Init()
{
    m_u16Address = 0;
    return 0;
}

//---------------------------------------------------------------------------
int RAMDriver::Open()
{
    return 0;
}

//---------------------------------------------------------------------------
int RAMDriver::Close()
{
    return 0;
}

//---------------------------------------------------------------------------
size_t RAMDriver::Read(void* pvData_, size_t uBytes_)
{
    auto u16NumBytes = uBytes_;
    if ((m_u16Address + uBytes_) >= m_u16Size) {
        u16NumBytes = m_u16Size - m_u16Address;
    }

    if (!u16NumBytes) {
        return 0;
    }

    auto* pcTemp = &(m_pcData[m_u16Address]);
    auto *pvOut = reinterpret_cast<uint8_t*>(pvData_);
    for (size_t i = 0; i < u16NumBytes; i++) {
        pvOut[i] = pcTemp[i];
    }

    m_u16Address += u16NumBytes;
    return u16NumBytes;
}

//---------------------------------------------------------------------------
size_t RAMDriver::Write(const void* pvData_, size_t uBytes_)
{
    auto u16NumBytes = uBytes_;
    if ((m_u16Address + uBytes_) >= m_u16Size) {
        u16NumBytes = m_u16Size - m_u16Address;
    }

    if (!u16NumBytes) {
        return 0;
    }

    auto* pcTemp = &(m_pcData[m_u16Address]);
    auto* pvIn = reinterpret_cast<const uint8_t*>(pvData_);
    for (size_t i = 0; i < u16NumBytes; i++) {
        pcTemp[i] = pvIn[i];
    }

    m_u16Address += u16NumBytes;
    return u16NumBytes;
}

//---------------------------------------------------------------------------
int RAMDriver::Control(uint16_t u16Event_, void* pvIn_, size_t uSizeIn_, const void* pvOut_, size_t uSizeOut_)
{
    switch (u16Event_) {
        case EEPROM_CMD_SEEK: {
            if (uSizeOut_ < m_u16Size) {
                m_u16Address = uSizeIn_;
            }
            return 1;
        }
        case EEPROM_CMD_SET_BUFFER: {
            m_pcData  = (uint8_t*)pvIn_;
            m_u16Size = uSizeIn_;
        }
    }
    return 0;
}
} //namespace Mark3
