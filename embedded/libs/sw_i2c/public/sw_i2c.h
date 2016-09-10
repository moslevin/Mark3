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
    \file sw_i2c.h
    \brief Software (bit-bang) i2c master driver
 */

#ifndef __SW_I2C_H__
#define __SW_I2C_H__

#include <stdint.h>
//---------------------------------------------------------------------------
typedef enum {
    I2C_LOW       = 0,  //!< Logic low, zero, GND, etc, etc.
    I2C_HIGH      = 1   //!< Logic high, 1, VCC, etc, etc.
} i2c_level_t;

//---------------------------------------------------------------------------
typedef enum {
    I2C_OK       = 0,   //!< Successful operation
    I2C_NACK,           //!< Read or write was NACK'd
    I2C_TIMEOUT,        //!< Device did not respond after in clock stretch window
    I2C_BUS_ERROR       //!< Bus arbitration error (slave holds SDA low)
} i2c_return_t;

//---------------------------------------------------------------------------
/*!
 * \brief The SoftwareI2C class
 */
class SoftwareI2C
{
public:
    /*!
     * \brief Init
     *
     * Initialize the internal data of the software I2C instance.  Must be
     * called prior to use of any other methods.
     *
     * \param u32Freq_  Desired I2C bus frequency (in Hz).
     */
    void Init(uint32_t u32Freq_);

    /*!
     * \brief SendStart
     *
     * Set the I2C Start or Restart condition on the bus.  Called at the start
     * of any read or write operation.
     *
     * \return  I2C_OK on success, others on failure.
     */
    i2c_return_t SendStart(void);

    /*!
     * \brief SendStop
     *
     * Set the I2C Stop condition
     *
     * \return  I2C_OK on success, others on failure.
     */
    i2c_return_t SendStop(void);

    /*!
     * \brief WriteAddress
     *
     * Write a 7-bit address to the I2C bus.
     *
     * \param u8Address_    7-bit device address
     * \param bRead_        Whether or not the master is reading or writing
     *                      in this transaction.
     * \param pbAck_        Pointer to an acknowledgement flag - flag contains
     *                      true on successful ack, 'false' on nack.
     *
     * \return I2C_OK on success, others on failure.
     */
    i2c_return_t WriteAddress(uint8_t u8Address_, bool bRead_, bool* pbAck_);

    /*!
     * \brief WriteByte
     *
     * Write a single 8-bit byte to the I2C bus.
     *
     * \param pu8Byte_      Data to transmit
     * \param pbAck_        Pointer to an acknowledgement flag - flag contains
     *                      true on successful ack, 'false' on nack.  Expect an
     *                      ack for all but the last byte of a transfer.
     *
     * \return I2C_OK on success, others on failure
     */
    i2c_return_t WriteByte(uint8_t pu8Byte_, bool* pbAck_);

    /*!
     * \brief ReadByte
     *
     * Read a single 8-bit byte off the I2C bus.
     *
     * \param pu8Byte_      Pointer to data byte to read into
     * \param bAck_         Whether or not we should ack the read operation.
     *                      (Should be set for all but the last byte in a
     *                      transfer)
     * \return  I2C_OK on success, others on failure.
     */
    i2c_return_t ReadByte(uint8_t* pu8Byte_, bool bAck_);

    /*!
     * \brief BusIdle
     *
     * Set the bus idle condition (tri-state input on SDA/SCL).
     *
     */
    void BusIdle(void);

private:

    /*!
     * \brief BitDelay
     *
     * Or more accurately, half-bit delay.  This is used internally to delay
     * for half a cycle (used to implement clock high/low phases, etc.) based
     * on the bus frequency set during initialization.
     *
     */
    virtual void BitDelay(void) = 0;

    /*!
     * \brief SetSCL
     *
     * Set the level on the I2C Bus' SCL line.
     *
     * \param eLevel_ Level to set the SCL line to.
     */
    virtual void SetSCL(i2c_level_t eLevel_) = 0;

    /*!
     * \brief SetSDA
     *
     * Set the level on the I2C Bus' SDA line.
     *
     * \param eLevel_ Leve to set the SDA line to.
     */
    virtual void SetSDA(i2c_level_t eLevel_) = 0;

    /*!
     * \brief GetSCL
     *
     * Return the current status of the SCL line
     *
     * \param peLevel_ Pointer - receives the level of the SCL line
     */
    virtual void GetSCL(i2c_level_t* peLevel_) = 0;

    /*!
     * \brief GetSDA
     *
     * Return the current status of the SDA line
     *
     * \param peLevel_ Pointer - receives the level of the SDA line
     */
    virtual void GetSDA(i2c_level_t* peLevel_) = 0;

    /*!
     * \brief CheckBus
     *
     * Check the bus for proper data signal arbitration.  If the slave is
     * holding the bus low, there's nothing the master can do aside from
     * attempting to reset the slave.
     *
     * \return true on success, false on arbitration error.
     */
    bool CheckBus(void);

    /*!
     * \brief ClockStretch
     *
     * Query the bus and wait if necessary for the slave to release the clock
     * stretch condition.
     *
     * \return true on success, false on timeout.
     */
    bool ClockStretch(void);

    /*!
     * \brief WriteBit
     *
     * Write a single bit to the I2C bus.  Used internally to generate all
     * byte writes and read acks.
     *
     * \param bBit_ Bit to write to the bus.
     *
     * \return  I2C_OK on success, others on failure.
     */
    i2c_return_t WriteBit(bool bBit_);

    /*!
     * \brief ReadBit
     *
     * Read a single bit from the I2C bus.  Used internally to perform all
     * byte reads and write acks.
     *
     * \param pbBit_ Pointer to data bit.
     *
     * \return  I2C_OK on success, others on failure.
     */
    i2c_return_t ReadBit(bool* pbBit_);

    /*!
     * \brief SendAck
     *
     * Send an acknowledgement.  Used internally in response to byte reads.
     *
     * \param bAck_ true - acknowledge, false - negative acknowledge.
     *
     * \return  I2C_OK on success, others on failure.
     */
    i2c_return_t SendAck(bool bAck_);

    /*!
     * \brief RecvAck
     *
     * Receive an acknowledgement from the slave device.  Used internally
     * in response to byte writes.
     *
     * \param pbAck_ Pointer to ack bit being received.
     *               true - acknowledged, false - negative acknowledge.
     * \return  I2C_OK on success, others on failure.
     */
    i2c_return_t RecvAck(bool* pbAck_);

    uint32_t m_u32BitDelayUS;   //!< Half-bit delay in microseconds
    bool     m_bStarted;        //!< Transaction state -- true = started, false = idle
};


#endif //__SW_I2C_H__

