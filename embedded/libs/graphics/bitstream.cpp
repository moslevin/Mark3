/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2014 Funkenstein Software Consulting, all rights reserved.
See license.txt for more information
===========================================================================*/
#include "mark3cfg.h"
#include "kerneltypes.h"
#include "bitstream.h"

//---------------------------------------------------------------------------
void BitStreamer::Init(uint8_t *pu8Data_, uint16_t u16Size_)
{
    m_pu8Data = pu8Data_;
    m_u8BitIndex = 0;
    m_u16ByteIndex = 0;
    m_u16Size = 0;
}

//---------------------------------------------------------------------------
void BitStreamer::AdvanceByte(void)
{
    if (m_u8BitIndex != 0)
    {
        m_u8BitIndex = 0;
        m_u16ByteIndex++;
    }
}

//---------------------------------------------------------------------------
uint8_t BitStreamer::ReadBits(uint8_t u8NumBits_)
{
    uint8_t u8Dat = m_pu8Data[m_u16ByteIndex];
    uint8_t u8Ret = 0;
    uint8_t u8IterBits;
    uint8_t u8Shift = 0;

    while (u8NumBits_)
    {
        // Check to see whether or not the bitstream read will past the end
        // of the current byte's index.
        if (u8NumBits_ < (8 - m_u8BitIndex))
        {
            // If not, read everything all in one iteration
            u8IterBits = u8NumBits_;
        }
        else
        {
            // Otherwise, read what we can in this iteration.
            u8IterBits = (8 - m_u8BitIndex);
        }

        // Shift the bits to be read in this iteration to the lowest bits in the variable
        u8Dat = ((u8Dat >> m_u8BitIndex) & ((1 << u8IterBits) - 1));

        // Shift into the output variable in the correct index
        u8Ret += (u8Dat << u8Shift);

        // Update the bit index (and byte index, if we've reached the end of the word)
        m_u8BitIndex += u8NumBits_;
        if (m_u8BitIndex == 8)
        {
            m_u8BitIndex = 0;
            m_u16ByteIndex++;
        }

        // Subtract the bits we've read from the total remaining
        u8NumBits_ -= u8IterBits;

        // If there are bits yet to be read
        if (u8NumBits_)
        {
            // Figure out how much to shift the current return value by
            u8Shift += u8IterBits;

            // Read the next byte of data to use in the next iteration
            u8Dat = m_pu8Data[m_u16ByteIndex];
        }
    }
    return u8Ret;
}
