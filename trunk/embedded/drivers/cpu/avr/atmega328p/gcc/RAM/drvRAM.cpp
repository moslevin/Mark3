/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012-2015 Funkenstein Software Consulting, all rights reserved.
See license.txt for more information
===========================================================================*/
/*!

    \file   drvRAM.cpp

    \brief  Generic RAM-buffer driver

*/

#include "kerneltypes.h"
#include "drvRAM.h"
#include "driver.h"

//---------------------------------------------------------------------------
void RAMDriver::Init()
{
    m_u16Address = 0;
}

//---------------------------------------------------------------------------
uint8_t RAMDriver::Open()
{
    return 0;
}

//---------------------------------------------------------------------------
uint8_t RAMDriver::Close()
{
    return 0;
}

//---------------------------------------------------------------------------
uint16_t RAMDriver::Read( uint16_t u16Bytes_, uint8_t *pu8Data_ )
{
    uint16_t u16NumBytes = u16Bytes_;
    uint16_t i;
    uint8_t *pcTemp;
    
    if ((m_u16Address + u16Bytes_) >= m_u16Size)
    {
        u16NumBytes = m_u16Size - m_u16Address;
    }
    
    if (!u16NumBytes)
    {
        return 0;
    }

    pcTemp = &(m_pcData[m_u16Address]);
    for (i = 0; i < u16NumBytes; i++)
    {
        pu8Data_[i] = pcTemp[i];
    }
    
    m_u16Address += u16NumBytes;
    
    return u16NumBytes;
}

//---------------------------------------------------------------------------
uint16_t RAMDriver::Write( uint16_t u16Bytes_, uint8_t *pu8Data_ )
{
    uint16_t u16NumBytes = u16Bytes_;
    uint16_t i;
    uint8_t *pcTemp;

    if ((m_u16Address + u16Bytes_) >= m_u16Size )
    {
        u16NumBytes = m_u16Size - m_u16Address;
    }
    
    if (!u16NumBytes)
    {
        return 0;
    }

    pcTemp = &(m_pcData[m_u16Address]);
    for (i = 0; i < u16NumBytes; i++)
    {
        pcTemp[i] = pu8Data_[i];
    }

    m_u16Address += u16NumBytes;
    
    return u16NumBytes;
}

//---------------------------------------------------------------------------
uint16_t RAMDriver::Control( uint16_t u16Event_, void *pvIn_, uint16_t u16SizeIn_, void *pvOut_, uint16_t u16SizeOut_ )
{
    switch (u16Event_)
    {
        case EEPROM_CMD_SEEK:
        {
            if (u16SizeOut_ < m_u16Size)
            {
                m_u16Address = u16SizeIn_;
            }
            return 1;
        }        
        case EEPROM_CMD_SET_BUFFER:
        {
            m_pcData = (uint8_t*)pvIn_;
            m_u16Size = u16SizeIn_;
        }
    }
    return 0;
}
