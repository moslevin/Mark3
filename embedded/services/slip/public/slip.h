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
=========================================================================== */
/*!
    \file slip.h

    Serial Line IP framing code.  Also includes code to frame data in 
    FunkenSlip format for use with SlipTerm on a host PC.  
    
    FunkenSlip uses SLIP-framed messages with a pre-defined packet format 
    as follows:
    
    [ Channel ][ Size ][ Data Buffer ][ CRC8 ]
    
    Channel is 1 byte, indicating the type of data carried in the message

    Size is 2 bytes, indicating the length of the binary blob that follows

    Data Buffer is n bytes, and contains the raw packet data.

    CRC16 is 2 byte, Providing an error detection mechanism
 */

#include "kerneltypes.h"
#include "driver.h"

#ifndef __SLIP_H__
#define __SLIP_H__

//---------------------------------------------------------------------------
typedef enum
{
    SLIP_CHANNEL_TERMINAL = 0,   //!< ASCII text mode terminal
    SLIP_CHANNEL_UNISCOPE,       //!< Uniscope VM command channel
    SLIP_CHANNEL_NVM,            //!< Non-volatile memory configuration
    SLIP_CHANNEL_RESET,          //!< Channel used to reset the device...
    SLIP_CHANNEL_GRAPHICS,       //!< Encoded drawing commands    
    SLIP_CHANNEL_HID,            //!< HID commands
//---    
    SLIP_CHANNEL_COUNT
} SlipChannel;

//---------------------------------------------------------------------------
/*!
    Data structure used for vector-based SLIP data transmission.  Allows for
    building and transmitting complex data structures without having to copy
    data into intermediate buffers.
 */
typedef struct  
{
    uint8_t u8Size;             //!< Size of the data buffer
    uint8_t *pu8Data;           //!< Pointer to the data buffer
}SlipDataVector;

//---------------------------------------------------------------------------
/*!
    Object used to frame communications over an abstract device using
    the serial-line internet protocol (SLIP).
 */
class Slip
{
public:
    /*!
     *  Set the driver to attach to this object.
     *  
     *  \param pclDriver_ Pointer to the driver to attach
     */
    void SetDriver( Driver *pclDriver_ ){ m_pclDriver = pclDriver_; }
    
    /*!
     *  Return the pointer to the driver attached to this object
     *  
     *  \return Pointer to the driver attached
     */
    Driver *GetDriver() { return m_pclDriver; }

    /*!
     *  \fn uint16_t EncodeByte( uint8_t u8Char_, uint8_t *aucBuf_ )    
     *  
     *  Encode a single byte into a stream, returning the size of
     *  the encoded value (either 1 or 2 bytes).
     *
     *  \param u8Char_ Character to encode
     *  \param aucBuf_ Buffer to encode into
     *  
     *  \return # bytes read
     */
    static uint16_t EncodeByte( uint8_t u8Char_, uint8_t *aucBuf_ );

    /*!
     *  \fn uint16_t DecodeByte( uint8_t *ucChar_, const uint8_t *aucBuf_ )
     *  
     *  Decode a byte from a stream into a specified value
     *  
     *  Returns    the number of bytes from the source array that were processed,
     *  (1 or 2), or 0 if an end-of-packet (192) was encountered.
     *  
     *  
     *  \param u8Char_ Destination char
     *  \param aucBuf_ Source buffer
     *  
     *  \return # bytes read, or 0 on terminating character (192)
     */
    static uint16_t DecodeByte( uint8_t *ucChar_, const uint8_t *aucBuf_ );
    
    /*!
     *  \fn void WriteData( uint8_t u8Channel_, const char *aucBuf_, uint8_t u8Len_ )
     *  
     *  Write a packet of data in the FunkenSlip format.
     *  
     *  Returns    the number of bytes from the source array that were processed,
     *  (1 or 2), or 0 if an end-of-packet (192) was encountered.
     *  
     *  \param u8Channel_ Channel to encode the packet to
     *  \param aucBuf_ Payload to encode
     *  \param u16Len_ Length of payload data
     */
    void WriteData( uint8_t u8Channel_, const char *aucBuf_, uint16_t u16Len_ );

    /*!
     *  \fn uint16_t ReadData( uint8_t *pu8Channel_, char *aucBuf_, uint16_t u16Len_ )
     *  
     *  Read a packet from a specified device, parse, and copy to a specified 
     *  output buffer.
     *  
     *  \param pu8Channel_ Pointer to a u8har that stores the message channel
     *  \param aucBuf_ Buffer where the message will be decoded
     *  \param u16Len_ Length of the buffer to decode
     *  
     *  \return data bytes read, 0 on failure.
     */
    uint16_t ReadData( uint8_t *pu8Channel_, char *aucBuf_, uint16_t u16Len_ );

    /*!
     *  \fn void WriteVector( uint8_t u8Channel_, SlipDataVector_t *astData, uint16_t u16Len_ )
     *  
     *  Write a single message composed of multiple data-vector fragments.  
     *  
     *  Allows for transmitting complex data structures without requiring buffering.
     *  This operation is zero-copy.
     *  
     *  \param u8Channel_ Message channel
     *  \param astData_ Pointer to the data vector 
     *  \param u16Len_ Number of elements in the data vector
     */
    void WriteVector( uint8_t u8Channel_, SlipDataVector *astData_, uint16_t u16Len_ );
    
    /*!
     *  \fn void SendAck()        
     *  
     *  Send an acknowledgement character to the host
     */
    void SendAck();

    /*!
     *  \fn void SendNack()
     *  
     *  Send a negative-acknowledgement character to the host
     */
    void SendNack();
    
private:
    void WriteByte(uint8_t u8Data_);    
    Driver *m_pclDriver;    
};

#endif
