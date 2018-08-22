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

    \file   drvRAM.h

    \brief  Generic RAM-buffer driver

*/
#pragma once

#include "kerneltypes.h"
#include "driver.h"

namespace Mark3
{

typedef enum {
    EEPROM_CMD_SEEK = 0x80, //!< Set the current EEPROM address, used for read/write
    EEPROM_CMD_SET_BUFFER   //!< Set the data buffer and buffer size before use
} EEPROM_Cmd_t;

class RAMDriver;

class RAMDriver : public Driver
{
public:
    virtual int  Init();
    virtual int  Open();
    virtual int  Close();
    virtual size_t Read(void* pvData_, size_t uBytes_);

    virtual size_t Write(const void* pvData_, size_t uBytes_);

    virtual int Control(uint16_t u16Event_, void* pvIn_, size_t uSizeIn_, const void* pvOut_, size_t uSizeOut_);

private:
    uint16_t m_u16Address;
    uint16_t m_u16Size;
    uint8_t* m_pcData;
};
} //namespace Mark3
