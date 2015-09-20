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

    \file   drvEEPROM.cpp

    \brief  Atmega eeprom driver

*/
#include "kerneltypes.h"

#include "drvEEPROM.h"
#include "driver.h"

#include <avr/io.h>
#include <avr/eeprom.h>

//---------------------------------------------------------------------------

#define EEPROM_SIZE        (4096)

//---------------------------------------------------------------------------
void ATMegaEEPROM::Init()
{ 
    m_u16Address = 0;
}

//---------------------------------------------------------------------------
uint8_t ATMegaEEPROM::Open()
{ 
    return 0; 
}

//---------------------------------------------------------------------------
uint8_t ATMegaEEPROM::Close()
{ 
    return 0; 
}
    
//---------------------------------------------------------------------------
uint16_t ATMegaEEPROM::Read( uint16_t u16Bytes_, uint8_t *pu8Data_ )
{
    uint16_t u16NumBytes = u16Bytes_;

    if ((m_u16Address + u16Bytes_) >= EEPROM_SIZE)
    {
        u16NumBytes = EEPROM_SIZE - m_u16Address;
    }
        
    if (!u16NumBytes)
    {
        return 0;
    }

    eeprom_read_block((void*)pu8Data_, (const void*)m_u16Address, (size_t)u16NumBytes);
    m_u16Address += u16NumBytes;
        
    return u16NumBytes;
}

//---------------------------------------------------------------------------
uint16_t ATMegaEEPROM::Write( uint16_t u16Bytes_, uint8_t *pu8Data_ )
{
    uint16_t u16NumBytes = u16Bytes_;

    if ((m_u16Address + u16Bytes_) >= EEPROM_SIZE)
    {
        u16NumBytes = EEPROM_SIZE - m_u16Address;
    }
    
    if (!u16NumBytes)
    {
        return 0;
    }

    eeprom_write_block((void*)pu8Data_, (void*)m_u16Address, u16NumBytes);
    m_u16Address += u16NumBytes;
    
    return u16NumBytes;
}

//---------------------------------------------------------------------------
uint16_t ATMegaEEPROM::Control( uint16_t u16Event_, void *pvIn_, uint16_t u16SizeIn_, void *pvOut_, uint16_t u16SizeOut_ )
{
    switch (u16Event_)
    {    
        case EEPROM_CMD_SEEK:
        {
            if (u16SizeOut_ < EEPROM_SIZE)
            {
                m_u16Address = u16SizeIn_;
            }
            return 1;
        }
    }
    return 0;
}
