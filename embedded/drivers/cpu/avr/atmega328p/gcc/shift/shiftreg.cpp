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
    \file shiftreg.cpp
    \brief Shift register functionality to allow for efficient I/O expansion
*/
#include "kerneltypes.h"
#include "driver.h"
#include "shiftreg.h"

#include <avr/io.h>
#include <avr/interrupt.h>

//---------------------------------------------------------------------------
uint8_t ShiftDriver::Open()
{
    *m_pu8EnablePort |= m_u8EnableBit;
    return 0;
}

//---------------------------------------------------------------------------
uint8_t ShiftDriver::Close()
{
    *m_pu8EnablePort &= ~m_u8EnableBit;
    return 0;
}

//---------------------------------------------------------------------------
uint16_t ShiftDriver::Write( uint16_t u16Bytes_, 
                              uint8_t *pu8Data_)
{
    uint16_t i;
    
    // Only update the strobe after clocking out all of the required bytes.
    // This allows the user to make the scan chain as long as possible without
    // having to modify the driver.
    *m_pu8StrobePort &= ~m_u8StrobeBit;    
    for (i = 0; i < u16Bytes_; i++)
    {
        WriteByte(pu8Data_[i]);
    }    
    *m_pu8StrobePort |= m_u8StrobeBit;
    return u16Bytes_;
    
}                              
//---------------------------------------------------------------------------
uint16_t ShiftDriver::Control( uint16_t u16Event_, 
                                void *pvDataIn_, 
                                uint16_t u16SizeIn_, 
                                void *pvDataOut_, 
                                uint16_t u16SizeOut_ )                                
{
    switch (u16Event_)
    {
        case SHIFT_SET_CLOCK:
            m_pu8ClockPort = (volatile uint8_t*)pvDataIn_;
            m_u8ClockBit = (uint8_t)u16SizeIn_;
        break;
        case SHIFT_SET_DATA:
            m_pu8DataPort = (volatile uint8_t*)pvDataIn_;
            m_u8DataBit = (uint8_t)u16SizeIn_;
        break;
        case SHIFT_SET_STROBE:
            m_pu8StrobePort = (volatile uint8_t*)pvDataIn_;
            m_u8StrobeBit = (uint8_t)u16SizeIn_;
        break;
        case SHIFT_SET_ENABLE:
            m_pu8EnablePort = (volatile uint8_t*)pvDataIn_;
            m_u8EnableBit = (uint8_t)u16SizeIn_;
        break;
        default:
            break;
    }
    return 0;
}
//---------------------------------------------------------------------------
void ShiftDriver::SetClock( volatile uint8_t *pu8Port_, uint8_t u8Bit_ )
{
    Control( SHIFT_SET_CLOCK, (void*)pu8Port_, (uint16_t)u8Bit_, 0, 0 );
}
//---------------------------------------------------------------------------
void ShiftDriver::SetData( volatile uint8_t *pu8Port_, uint8_t u8Bit_ )
{
    Control( SHIFT_SET_DATA, (void*)pu8Port_, (uint16_t)u8Bit_, 0, 0 );
}
//---------------------------------------------------------------------------
void ShiftDriver::SetStrobe( volatile uint8_t *pu8Port_, uint8_t u8Bit_ )
{
    Control( SHIFT_SET_STROBE, (void*)pu8Port_, (uint16_t)u8Bit_, 0, 0 );
}
//---------------------------------------------------------------------------
void ShiftDriver::SetEnable( volatile uint8_t *pu8Port_, uint8_t u8Bit_ )
{
    Control( SHIFT_SET_ENABLE, (void*)pu8Port_, (uint16_t)u8Bit_, 0, 0 );
}
//---------------------------------------------------------------------------
void ShiftDriver::WriteByte( uint8_t u8Byte_ )
{
    uint8_t u8Mask = 0x80;
    
    // Clock data out, MSB first, data latched on the RISING clock edge.
    while (u8Mask)
    {
        *m_pu8ClockPort &= ~m_u8ClockBit;
        if (u8Byte_ & u8Mask)
        {
            *m_pu8DataPort |= m_u8DataBit;
        }
        else
        {
            *m_pu8DataPort &= ~m_u8DataBit;
        }
        u8Mask >>= 1;
        asm volatile("nop");
        asm volatile("nop");
        asm volatile("nop");
        asm volatile("nop");
        asm volatile("nop");
        asm volatile("nop");
        asm volatile("nop");
        asm volatile("nop");
        *m_pu8ClockPort |= m_u8ClockBit;
        asm volatile("nop");
        asm volatile("nop");
        asm volatile("nop");
        asm volatile("nop");
        asm volatile("nop");
        asm volatile("nop");
        asm volatile("nop");
        asm volatile("nop");
    }
}
