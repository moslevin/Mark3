/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012 - 2019 m0slevin, all rights reserved.
See license.txt for more information
=========================================================================== */
/**
    @file streamer.h

    @brief Thread/Interrupt-safe byte-based data streaming.
 */

#include "kerneltypes.h"
#include "mark3.h"

#pragma once

namespace Mark3
{
//---------------------------------------------------------------------------
/**
 * @brief The Streamer class.
 * This class implements a circular byte-buffer with thread and interrupt
 * safe methods for writing-to and reading-from the buffer.  Objects of this
 * class type are designed to be shared between threads, or between threads
 * and interrupts.
 */
class Streamer
{
public:
    /**
     * @brief Init.
     * Initialize the Streamer object prior to its use, providing a blob
     * of memory for the object to manage.
     *
     * @param pau8Buffer_   Blob of memory to use as a circular buffer
     * @param u16Size_      Size of the supplied buffer in bytes
     */
    void Init(uint8_t* pau8Buffer_, uint16_t u16Size_);

    /**
     * @brief Read.
     * Read a byte of data from the stream, if available.
     *
     * @param pu8Data_      Pointer to read data into from the stream
     * @return              true if data was read, false if data unavailable
     *                      or buffer locked
     */
    bool Read(uint8_t* pu8Data_);

    /**
     * @brief Read.
     * Read from the stream until a specified number of bytes have been read,
     * or the stream is exhausted.
     *
     * @param pu8Data_ pointer to an array of data read into
     * @param u16Len_ maximum number of bytes to read
     * @return number of bytes read
     */
    uint16_t Read(uint8_t* pu8Data_, uint16_t u16Len_);

    /**
     * @brief Write.
     * Write a byte of data into the stream.
     *
     * @param u8Data_       Data byte to be written into the stream
     * @return              true if byte was written, false on buffer full
     *                      or buffer locked at index.
     */
    bool Write(uint8_t u8Data_);

    /**
     * @brief Write.
     * Write a maximum number of bytes to the stream.
     *
     * @param pu8Data_ pointer to an array of bytes to write out to the stream
     * @param u16Len_ Length of data held in the array
     * @return number of bytes written to the stream
     */
    uint16_t Write(const uint8_t* pu8Data_, uint16_t u16Len_);

    /**
     * @brief Claim.
     * Claim a byte of data for writing, without actually writing into it.  When
     * the writer is ready to write into the data byte as a result of another operation,
     * it can then populate the byte.
     *
     * This method is useful when encoding data from a raw format into a packet-based
     * format, where one byte of input may result in multiple bytes of output being generated.
     * Especially in cases where a user wants to write to a stream while a peripheral
     * reads from it to transmisit its contents asynchronously (i.e. an interrupt-driven
     * UART consuming a data packet, where the packet is being framed and consumed
     * simultaneously).
     *
     * This should be used in conjunction with the Lock method and judicious
     * use of critical sections to prevent race conditions.
     *
     * @param pu8Addr_   [out] Pointer to a byte pointer that will contain the address
     * of the "claimed" byte on success.
     * @return           true if successful, false if buffer full or locked.
     */
    bool Claim(uint8_t** pu8Addr_);

    /**
     * @brief Lock.
     * When the lock is set, a client can neither read from, or write to the buffer at the
     * index specified.  This is used to in conjunction with Claim to safely reserve data
     * from the buffer, while preventing race conditions occurring as a result of a consumer
     * acting on the data before it is ready.
     *
     * @param pu8LockAddr_  Address (within the stream) to set as the lockpoint.
     */
    void Lock(uint8_t* pu8LockAddr_);

    /**
     * @brief Unlock.
     * Reset the lock pointer in the object, allowing a consumer to read any previously
     * unavailable data that might still be in the stream.
     *
     */
    void Unlock(void);

    /**
     * @brief GetAvailable
     * @return The current number of bytes available for write in the streams
     */
    uint16_t GetAvailable(void) { return m_u16Size; }
    /**
     * @brief CanRead
     * @return true if the stream has any unread data
     */
    bool CanRead(void);

    /**
     * @brief CanWrite
     * @return true if the stream has any free space
     */
    bool CanWrite(void);

    /**
     * @brief IsEmpty     
     * @return true if the stream is empty
     */
    bool IsEmpty(void);

private:
    uint8_t* m_pau8Buffer;  //!< Pointer to the buffer managed in this object
    uint8_t* m_pu8LockAddr; //!< Address of the lock point in the stream

    uint16_t m_u16Size;  //!< Size of the stream's circular buffer (in bytes)
    uint16_t m_u16Avail; //!< Number of bytes free in the stream
    uint16_t m_u16Head;  //!< Current head index (write to) of the stream
    uint16_t m_u16Tail;  //!< Current tail index (read from) of the stream
};
} // namespace Mark3
