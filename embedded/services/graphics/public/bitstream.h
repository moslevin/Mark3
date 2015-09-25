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

#ifndef __BITSTREAM_H__
#define __BITSTREAM_H__

//---------------------------------------------------------------------------
class BitStreamer
{
public:
    /*!
     * \brief Init
     *
     * Initialize the BitStreamer object prior to use
     *
     * \param pu8Data_ Pointer to raw data to be streamed
     * \param u16Size_ Size of pu8Data_ in bytes
     */
    void Init(uint8_t *pu8Data_, uint16_t u16Size_);

    /*!
     * \brief AdvanceByte
     *
     * Advance byte index to the next full byte if the current bit index
     * is non-zero.  If the current bit index is zero, no action is taken.
     * This is used to byte-align 2-dimensional data, such as images.
     */
    void AdvanceByte(void);

    /*!
     * \brief ReadBits
     *
     * Read the next "n" bits from the stream, returning the result into an
     * 8-bit unsigned integer.
     *
     * \param u8NumBits_ Number of bits to read (less than 8)
     * \return Bits read as an 8-bit unsigned integer.
     */
    uint8_t ReadBits(uint8_t u8NumBits_);

private:
    uint8_t *m_pu8Data;      //!< Pointer to the data being streamed
    uint8_t m_u8BitIndex;    //!< Current "bit" index in the current byte
    uint16_t m_u16ByteIndex; //!< Current "byte" index in the stream
    uint16_t m_u16Size;      //!< Length of data (in bytes)
};

#endif // __BITSTREAM_H__
