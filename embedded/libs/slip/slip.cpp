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
    \file slip.cpp
    \brief Serial Line IP framing code.
*/

#include "kerneltypes.h"
#include "slip.h"
#include "driver.h"

//---------------------------------------------------------------------------
#define FRAMING_BYTE            (192)   //!< Byte indicating end-of-frame
#define FRAMING_ENC_BYTE        (219)   //!< Byte used to indicate substitution
#define FRAMING_SUB_BYTE        (220)   //!< Byte to substitute for framing byte
#define FRAMING_SUB_ENC_BYTE    (221)   //!< Byte to substitute for the substitute-byte

//---------------------------------------------------------------------------
#define ACchar                (69)    //!< Acknowledgement character
#define NACchar               (96)    //!< Non-acknowledgement character

//---------------------------------------------------------------------------
uint16_t Slip::EncodeByte( uint8_t u8Char_, uint8_t *aucBuf_ )
{
    uint16_t u16Len = 1;
    switch (u8Char_)
    {
        case FRAMING_BYTE:
            aucBuf_[0] = FRAMING_ENC_BYTE;
            aucBuf_[1] = FRAMING_SUB_BYTE;
            u16Len = 2;
            break;
        case FRAMING_ENC_BYTE:
            aucBuf_[0] = FRAMING_ENC_BYTE;
            aucBuf_[1] = FRAMING_SUB_ENC_BYTE;
            u16Len = 2;
            break;
        default:
            aucBuf_[0] = u8Char_;
    }
    return u16Len;
}

//---------------------------------------------------------------------------
uint16_t Slip::DecodeByte( uint8_t *ucChar_, const uint8_t *aucBuf_ )
{
    uint16_t u16Len = 1;
    
    if (aucBuf_[0] == FRAMING_ENC_BYTE)
    {
        if(aucBuf_[1] == FRAMING_SUB_BYTE)
        {
            *ucChar_ = FRAMING_BYTE;
            u16Len = 2;
        }
        else if(aucBuf_[1] == FRAMING_SUB_ENC_BYTE)
        {
            *ucChar_ = FRAMING_ENC_BYTE;
            u16Len = 2;
        }
        else
        {
            *ucChar_ = 0;
            u16Len = 0;
        }
    }
    else if (aucBuf_[0] == FRAMING_BYTE)
    {
        u16Len = 0;
        *ucChar_ = 0;
    }
    else
    {
        *ucChar_ = aucBuf_[0];
    }
    return u16Len;
}

//---------------------------------------------------------------------------
void Slip::WriteByte( uint8_t u8Data_)
{
    uint16_t u16Size = 0;
    uint16_t u16Idx = 0;
    uint8_t aucBuf[2];
    u16Size = EncodeByte(u8Data_, aucBuf);
    while (u16Idx < u16Size)
    {
        u16Idx += m_pclDriver->Write(u16Size, &aucBuf[u16Idx]);    
    }
}

//---------------------------------------------------------------------------
uint16_t Slip::ReadData(uint8_t *pu8Channel_, char *aucBuf_, uint16_t u16Len_)
{
    uint16_t u16ReadCount;
    uint8_t u8TempCount;
    uint16_t u16Valid = 0;
    uint16_t u16CRC;
    uint16_t u16CRC_Calc = 0;
    uint16_t u16Len;
    uint8_t *pu8Src = (uint8_t*)aucBuf_;
    uint8_t *pu8Dst = (uint8_t*)aucBuf_;
    
    u16ReadCount = m_pclDriver->Read(u16Len_, (uint8_t*)aucBuf_);

    while (u16ReadCount)
    {
        uint8_t u8Read;
        u8TempCount = DecodeByte(&u8Read, pu8Src);
    
        *pu8Dst = u8Read;
        
        // Encountered a FRAMING_BYTE - end of message
        if (!u8TempCount) 
        {
            break;
        }
        
        // Add to the CRC
        u16CRC_Calc += u8Read;
        
        // Adjust iterators, source, and destination pointers.
        u16ReadCount -= u8TempCount;
        pu8Src += u8TempCount;
        pu8Dst++;
        u16Valid++;
    }

    // Ensure we have enough data to try a match.
    if (u16Valid < 5) {
        return 0;
    }
    
    u16CRC_Calc -= aucBuf_[u16Valid-2];
    u16CRC_Calc -= aucBuf_[u16Valid-1];
    
    u16Len = ((uint16_t)aucBuf_[1]) << 8;
    u16Len += ((uint16_t)aucBuf_[2]);
    u16CRC = ((uint16_t)aucBuf_[u16Valid-2]) << 8;
    u16CRC += ((uint16_t)aucBuf_[u16Valid-1]);
    
    if (u16CRC != u16CRC_Calc)
    {
        return 0;
    }
    
    *pu8Channel_ = aucBuf_[0];

    return u16Len;
}

//---------------------------------------------------------------------------
void Slip::WriteData(uint8_t u8Channel_, const char *aucBuf_, uint16_t u16Len_)
{
    uint8_t aucTmp[2];    
    uint16_t u16CRC = 0;

    // Lightweight protocol built on-top of SLIP.
    // 1) Channel ID (8-bit)
    // 2) Data Size (16-bit)
    // 3) Data blob
    // 4) CRC16 (16-bit)
    aucTmp[0] = FRAMING_BYTE;
    while( !m_pclDriver->Write(1, aucTmp) ) {}
                    
    if (!u16Len_)    // Read to end-of-line (\0)
    {
        uint8_t *pu8Buf = (uint8_t*)aucBuf_;
        while (*pu8Buf != '\0')
        {
            u16Len_++;
            pu8Buf++;
        }                
    }
    
    WriteByte(u8Channel_);
    u16CRC = u8Channel_;

    WriteByte((uint8_t)(u16Len_ >> 8));
    u16CRC += (u16Len_ >> 8);

    WriteByte((uint8_t)(u16Len_ & 0x00FF));
    u16CRC += (u16Len_ & 0x00FF);
        
    while (u16Len_--)
    {
        WriteByte(*aucBuf_);
        u16CRC += (uint16_t)*aucBuf_;
        aucBuf_++;    
    }
    
    WriteByte((uint8_t)(u16CRC >> 8));
    WriteByte((uint8_t)(u16CRC & 0x00FF));
    
    aucTmp[0] = FRAMING_BYTE;
    while( !m_pclDriver->Write(1, aucTmp) ) {}
}

//---------------------------------------------------------------------------
void Slip::SendAck()
{
    WriteByte(ACchar);
}

//---------------------------------------------------------------------------
void Slip::SendNack()
{
    WriteByte(NACchar);
}

//---------------------------------------------------------------------------
void Slip::WriteVector(uint8_t u8Channel_, SlipDataVector *astData_, uint16_t u16Len_)
{
    uint8_t aucTmp[2];    
    uint16_t u16CRC = 0;
    uint8_t i, j;
    uint16_t u16TotalLen = 0;
    
    // Calculate the total length of all message fragments
    for (i = 0; i < u16Len_; i++)
    {        
        u16TotalLen += astData_[i].u8Size;
    }    
    
    // Send a FRAMING_BYTE to start framing a message
    aucTmp[0] = FRAMING_BYTE;
    while( !m_pclDriver->Write(1, aucTmp) ) {}
    
    // Write a the channel
    WriteByte(u8Channel_);
    u16CRC = u8Channel_;

    // Write the length
    WriteByte((uint8_t)(u16TotalLen >> 8));
    u16CRC += (u16TotalLen >> 8);

    WriteByte((uint8_t)(u16TotalLen & 0x00FF));    
    u16CRC += (u16TotalLen & 0x00FF);
        
    // Write the message fragments
    for (i = 0; i < u16Len_; i++)
    {
        uint8_t *aucBuf = astData_[i].pu8Data;
        for (j = 0; j < astData_[i].u8Size; j++ )
        {
            WriteByte(*aucBuf);
            u16CRC += (uint16_t)*aucBuf;
            aucBuf++;
        }        
    }
    
    // Write the CRC 
    WriteByte((uint8_t)(u16CRC >> 8));
    WriteByte((uint8_t)(u16CRC & 0x00FF));
    
    // Write the end-of-message
    aucTmp[0] = FRAMING_BYTE;
    while( !m_pclDriver->Write(1, aucTmp) ) {}
}
