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
=========================================================================== */
/*!
    \file sw_spi.h
    \brief Software (bit-bang) spi master driver
 */

#ifndef __SW_SPI_H__
#define __SW_SPI_H__

#include <stdint.h>

//---------------------------------------------------------------------------
typedef enum {
    SPI_LEVEL_LOW  = 0,
    SPI_LEVEL_HIGH = 1
} spi_level_t;

//---------------------------------------------------------------------------
typedef enum {
    SPI_MODE_0 = 0, //!< Clock polarity low, data clocked on first edge
    SPI_MODE_1,     //!< Clock polarity high, data clocked on first edge
    SPI_MODE_2,     //!< Clock polarity low, data clocked on second edge
    SPI_MODE_3      //!< Clock polarity high, data clocked on second edge
} spi_mode_t;

//---------------------------------------------------------------------------
class SoftwareSPI
{
public:
    void Init(uint32_t u32Freq_, spi_mode_t eMode_);

    void Transfer(uint8_t* pu8Input_, uint8_t* pu8Output_, uint16_t u16Len_);
private:
    virtual void BitDelay(void) = 0;

    virtual void WriteBit(spi_level_t bBit_) = 0;

    virtual void ReadBit(spi_level_t* pbBit_) = 0;

    virtual void SetClock(spi_level_t eLevel_) = 0;

    uint32_t    m_u32BitDelayUS;
    bool        m_bPolarity;
    bool        m_bPhase;
};


#endif // __SW_SPI_H__

