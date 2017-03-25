/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012 - 2017 Funkenstein Software Consulting, all rights reserved.
See license.txt for more information
=========================================================================== */
/*!
    \file sw_spi.cpp
    \brief Software (bit-bang) spi master driver
 */

#include "sw_spi.h"

//---------------------------------------------------------------------------
void SoftwareSPI::Init(uint32_t u32Freq_, spi_mode_t eMode_)
{
    m_u32BitDelayUS = 1000000 / (u32Freq_ / 2000);

    switch (eMode_) {
        case SPI_MODE_0:
            m_bPolarity = false;
            m_bPhase = false;
            break;
        case SPI_MODE_1:
            m_bPolarity = false;
            m_bPhase = true;
            break;
        case SPI_MODE_2:
            m_bPolarity = true;
            m_bPhase = false;
            break;
        case SPI_MODE_3:
            m_bPolarity = true;
            m_bPhase = true;
            break;
    }

    // Set initial clock level based on polarity
    if (m_bPolarity == true) {
        SetClock(SPI_LEVEL_HIGH);
    } else {
        SetClock(SPI_LEVEL_LOW);
    }
}

//---------------------------------------------------------------------------
void SoftwareSPI::Transfer(uint8_t* pu8Input_, uint8_t* pu8Output_, uint16_t u16Len_)
{
    uint8_t* pu8Src = pu8Input_;
    uint8_t* pu8Dst = pu8Output_;

    for (uint16_t i = 0; i < u16Len_; i++) {
        uint8_t u8Out   = *pu8Src++;
        uint8_t u8In    = 0;

        uint8_t u8Mask  = 0x80;
        while (u8Mask) {
            // If phase is set to false, we assume that data is clocked in on the
            // first clock edge. Ensure data out is set early in the phase.
            if (m_bPhase == false) {
                if (u8Mask & u8Out) { WriteBit(SPI_LEVEL_HIGH); }
                else { WriteBit(SPI_LEVEL_LOW); }

                // Delay for half a cycle
                BitDelay();
            }

            // Set the first clock edge
            if (m_bPolarity == false) { SetClock(SPI_LEVEL_HIGH); }
            else { SetClock(SPI_LEVEL_LOW); }

            // If the phase is set to true, we assume the data is clocked on the
            // second clock edge.
            if (m_bPhase == true) {
                if (u8Mask & u8Out) { WriteBit(SPI_LEVEL_HIGH); }
                else { WriteBit(SPI_LEVEL_LOW); }

                // Delay for half a cycle
                BitDelay();
            } else {
            // If the phase is set to false, data is latched on the first clock edge,
            // so it's safe to read now.
                spi_level_t eLevel;
                ReadBit(&eLevel);
                if (eLevel == SPI_LEVEL_HIGH) {
                    u8In |= u8Mask;
                }
                BitDelay();
            }

            // Set the second clock edge
            if (m_bPolarity == false) { SetClock(SPI_LEVEL_LOW); }
            else { SetClock(SPI_LEVEL_HIGH); }

            // Phase is true - data can be read after the second clock edge
            if (m_bPhase == true) {
                BitDelay();
                spi_level_t eLevel;
                ReadBit(&eLevel);
                if (eLevel == SPI_LEVEL_HIGH) {
                    u8In |= u8Mask;
                }
            }

            u8Mask >>= 1;
        }
        *pu8Dst++ = u8In;
    }
}

