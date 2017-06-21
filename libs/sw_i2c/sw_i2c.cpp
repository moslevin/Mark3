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
    \file sw_i2c.cpp
    \brief Software (bit-bang) i2c master driver implementation
 */

#include "sw_i2c.h"

//---------------------------------------------------------------------------
void SoftwareI2C::Init(uint32_t u32Freq_)
{
    m_u32BitDelayUS = 1000000l / (u32Freq_ * 2);
    BusIdle();
}

//---------------------------------------------------------------------------
i2c_return_t SoftwareI2C::SendStart(void)
{
    if (m_bStarted) {
        // Restart condition
        SetSDA(I2C_HIGH);
        BitDelay();

        SetSCL(I2C_HIGH);
        if (!ClockStretch()) {
            return I2C_TIMEOUT;
        }
        BitDelay();
    }

    if (!CheckBus()) {
        return I2C_BUS_ERROR;
    }

    SetSDA(I2C_LOW);
    BitDelay();

    SetSCL(I2C_LOW);
    m_bStarted = true;

    return I2C_OK;
}

//---------------------------------------------------------------------------
i2c_return_t SoftwareI2C::SendStop(void)
{
    SetSDA(I2C_LOW);
    BitDelay();

    SetSCL(I2C_HIGH);
    if (!ClockStretch()) {
        return I2C_TIMEOUT;
    }
    BitDelay();

    SetSDA(I2C_HIGH);
    BitDelay();

    if (!CheckBus()) {
        return I2C_BUS_ERROR;
    }

    BitDelay();
    m_bStarted = false;

    return I2C_OK;
}

//---------------------------------------------------------------------------
i2c_return_t SoftwareI2C::WriteAddress(uint8_t u8Address_, bool bRead_, bool* pbAck_)
{
    uint8_t u8Byte = u8Address_ << 1;
    if (bRead_) {
        u8Byte |= 0x01;
    }

    i2c_return_t rc = WriteByte(u8Byte, pbAck_);
    return rc;
}

//---------------------------------------------------------------------------
i2c_return_t SoftwareI2C::WriteByte(uint8_t pu8Byte_, bool* pbAck_)
{
    uint8_t u8Mask = 0x80;

    while (u8Mask) {
        bool bit = false;
        if ((u8Mask & pu8Byte_) == u8Mask) {
            bit = true;
        }
        i2c_return_t rc = WriteBit(bit);
        if (I2C_OK != rc) {
            return rc;
        }
        u8Mask >>= 1;
    }

    return RecvAck(pbAck_);
}

//---------------------------------------------------------------------------
i2c_return_t SoftwareI2C::ReadByte(uint8_t* pu8Byte_, bool bAck_)
{
    uint8_t u8Mask = 0x80;

    *pu8Byte_ = 0;
    while (u8Mask) {
        bool bit;
        i2c_return_t rc = ReadBit(&bit);
        if (I2C_OK != rc) {
            return rc;
        }
        if (bit) {
            *pu8Byte_ |= u8Mask;
        }
        u8Mask >>= 1;
    }
    return SendAck(bAck_);
}

//---------------------------------------------------------------------------
void SoftwareI2C::BusIdle(void)
{
    SetSCL(I2C_HIGH);
    SetSDA(I2C_HIGH);
}

//---------------------------------------------------------------------------
bool SoftwareI2C::CheckBus(void)
{
    i2c_level_t eLevel;
    GetSDA(&eLevel);
    if (I2C_LOW == eLevel) {
        return false;
    }
    return true;
}

//---------------------------------------------------------------------------
bool SoftwareI2C::ClockStretch(void)
{
    i2c_level_t eLevel;
    uint8_t u8Retries = 100;

    GetSCL(&eLevel);
    while (I2C_LOW == eLevel) {
        u8Retries--;
        if (!u8Retries) {
            break;
        }
        BitDelay();
    }

    if (!u8Retries) {
        return false;
    }
    return true;
}

//---------------------------------------------------------------------------
i2c_return_t SoftwareI2C::WriteBit(bool bBit_)
{
    if (bBit_) {
        SetSDA(I2C_HIGH);
    } else {
        SetSDA(I2C_LOW);
    }
    BitDelay();

    SetSCL(I2C_HIGH);
    BitDelay();

    if (!ClockStretch()) {
        return I2C_TIMEOUT;
    }

    if (bBit_) {
        if (!CheckBus()) {
            return I2C_BUS_ERROR;
        }
    }
    SetSCL(I2C_LOW);

    return I2C_OK;
}

//---------------------------------------------------------------------------
i2c_return_t SoftwareI2C::ReadBit(bool* pbBit_)
{
    SetSDA(I2C_HIGH);
    BitDelay();

    SetSCL(I2C_HIGH);
    if (!ClockStretch()) {
        return I2C_TIMEOUT;
    }
    BitDelay();

    i2c_level_t eLevel;
    GetSDA(&eLevel);

    if (eLevel) {
        *pbBit_ = true;
    } else {
        *pbBit_ = false;
    }

    SetSCL(I2C_LOW);
    return I2C_OK;
}

//---------------------------------------------------------------------------
i2c_return_t SoftwareI2C::SendAck(bool bAck_)
{
    return WriteBit(bAck_ == false);
}

//---------------------------------------------------------------------------
i2c_return_t SoftwareI2C::RecvAck(bool* pbAck_)
{
    bool bBit;
    i2c_return_t eReturn = ReadBit(&bBit);
    *pbAck_ = (bBit == false);
    return eReturn;
}

