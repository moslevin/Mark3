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
===========================================================================*/
/*!
    @brief UART abstraction interface
*/
#ifndef __UARTDRIVER_H__
#define __UARTDRIVER_H__

#include "driver.h"
#include "mark3.h"

//---------------------------------------------------------------------------
typedef enum {
    UART_OPCODE_SET_BAUDRATE = 0x80,
    UART_OPCODE_SET_BUFFERS,
    UART_OPCODE_SET_RX_ENABLE,
    UART_OPCODE_SET_RX_DISABLE,
    UART_OPCODE_SET_IDENTITY,
    UART_OPCODE_SET_BLOCKING,
    UART_OPCODE_SET_NONBLOCKING,
} UartOpcode_t;

//---------------------------------------------------------------------------
/**
 * @brief The UartDriver class
 * Present a consistent interface for configuring and accessing a UART, built
 * on top of the generic Driver class.  With this class, App code can be written
 * to communicate with any UART via the UartDriver layer without having to
 * consider a specific hardware or driver implementation under the covers.  This
 * vastly improves portability of UART-based code in applications.
 */
class UartDriver : public Driver
{
public:

    /**
     * @brief SetBaudRate
     * Set the baudrate for a given UART, invalidating any other previous
     * setting that may have been applied to the port.
     * @param u32BaudRate_ New baudrate to apply to the port
     * @return true on success, false on error or not supported.
     */
    bool SetBaudRate(uint32_t u32BaudRate_);

    /**
     * @brief SetBuffers
     * Set the raw data buffers managed by the UART driver.  Buffers
     * must persist for the duration of the object.
     * @param pu8RxBuffer_ Pointer to the receive buffer
     * @param u32RxBufferSize_ Size of the receive buffer in bytes
     * @param pu8TxBuffer_ Pointer to the transmit buffer
     * @param u32TxBufferSize_ Size of the transmit buffer in bytes
     * @return true on success, false on error or not supported
     */
    bool SetBuffers(uint8_t* pu8RxBuffer_, uint32_t u32RxBufferSize_,
                    uint8_t* pu8TxBuffer_, uint32_t u32TxBufferSize_);

    /**
     * @brief EnableRx
     * Enable or disable receive functionality on the UART.
     * @param bEnable_ true - enable receive mode, false - disable receive mode
     * @return true on success, false on error or not supported
     */
    bool EnableRx(bool bEnable_);

    /**
     * @brief SetPortIdentity
     * Set the physical port associated with an instance of a port driver
     * @param u8PortIdentity_ Index associated with a physical UART port
     * @return true on success, false on error
     */
    bool SetPortIdentity(uint8_t u8PortIdentity_);

    /**
     * @brief SetPortBlocking
     * Set the driver to blocking or non-blocking mode (where supported)
     * @param bBlocking_ true - blocking mode, false - non-blocking
     * @return true on success, false on error or unsupported
     */
    bool SetPortBlocking(bool bBlocking_);
   
private:
};


#endif // __UARTDRIVER_H__


