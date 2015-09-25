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
    \file shiftreg.cpp
    \brief Shift register functionality to allow for efficient I/O expansion
*/
#include "kerneltypes.h"
#include "driver.h"

#ifndef __SHIFTREG_H__
#define __SHIFTREG_H__

//---------------------------------------------------------------------------
typedef enum
{
    SHIFT_SET_CLOCK = 0x80,
    SHIFT_SET_DATA,
    SHIFT_SET_STROBE,
    SHIFT_SET_ENABLE
} ShiftDriverCommands;

//---------------------------------------------------------------------------
/*!
 *   Shift register driver class
 */
class ShiftDriver : public Driver
{
public:
/*!
 *   Driver functions
 */
    virtual void Init() {}    //Stubbed
    virtual uint8_t Open();
    virtual uint8_t Close();
    virtual uint16_t Read( uint16_t u16Bytes_, 
                           uint8_t *pu8Data_) {return u16Bytes_;}    //Stubbed
    virtual uint16_t Write( uint16_t u16Bytes_, 
                            uint8_t *pu8Data_);
    
    virtual uint16_t Control( uint16_t u16Event_, 
                              void *pvDataIn_, 
                              uint16_t u16SizeIn_, 
                              void *pvDataOut_, 
                              uint16_t u16SizeOut_ );

    void SetClock( volatile uint8_t *pu8Port_, uint8_t u8Bit_ );
    void SetData( volatile uint8_t *pu8Port_, uint8_t u8Bit_ );
    void SetStrobe( volatile uint8_t *pu8Port_, uint8_t u8Bit_ );
    void SetEnable( volatile uint8_t *pu8Port_, uint8_t u8Bit_ );
    
private:
    
    void WriteByte( uint8_t u8Byte_ );

    volatile uint8_t *m_pu8ClockPort;
    uint8_t m_u8ClockBit;

    volatile uint8_t *m_pu8DataPort;
    uint8_t m_u8DataBit;

    volatile uint8_t *m_pu8StrobePort;
    uint8_t m_u8StrobeBit;
    
    volatile uint8_t *m_pu8EnablePort;
    uint8_t m_u8EnableBit;    
};

#endif
