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

    \file   drvRAM.h

    \brief  Generic RAM-buffer driver

*/
#ifndef __DRVEEPROM_H_
#define __DRVEEPROM_H_

#include "kerneltypes.h"
#include "driver.h"

typedef enum
{
    EEPROM_CMD_SEEK = 0x80,  //!< Set the current EEPROM address, used for read/write
    EEPROM_CMD_SET_BUFFER    //!< Set the data buffer and buffer size before use
} EEPROM_Cmd_t;

class RAMDriver;

class RAMDriver : public Driver
{
public:
    virtual void Init();
    virtual uint8_t Open();
    virtual uint8_t Close();
    virtual uint16_t Read( uint16_t u16Bytes_,
                            uint8_t *pu8Data_ );
        
    virtual uint16_t Write( uint16_t u16Bytes_,
                            uint8_t *pu8Data_ );
        
    virtual uint16_t Control( uint16_t u16Event_,
                              void *pvIn_,
                              uint16_t u16SizeIn_,
                              void *pvOut_,
                              uint16_t u16SizeOut_ );
private:
    uint16_t m_u16Address;        
    uint16_t m_u16Size;
    uint8_t *m_pcData;
};

#endif 
