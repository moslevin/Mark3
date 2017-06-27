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

    \file   drvUARTplus.h

    \brief  Atmega serial port driver

 */
#ifndef __DRVUARTPLUS_H_
#define __DRVUARTPLUS_H_

#include "kerneltypes.h"
#include "driver.h"
#include "drvUART.h"
#include "notify.h"
#include "streamer.h"
#include "timer.h"
//---------------------------------------------------------------------------
// UART defines - user-configurable for different targets
//---------------------------------------------------------------------------
#define UART_SRA (UCSR0A)
#define UART_SRB (UCSR0B)
#define UART_SRC (UCSR0C)
#define UART_BAUDH (UBRR0H)
#define UART_BAUDL (UBRR0L)
#define UART_RXEN (RXEN0)
#define UART_TXEN (TXEN0)
#define UART_TXCIE (TXCIE0)
#define UART_RXCIE (RXCIE0)
#define UART_UDR (UDR0)
#define UART_UDRE (UDRE0)
#define UART_RXC (RXC0)

#define UART_DEFAULT_BAUD ((uint32_t)57600)

#define UART_RX_ISR (USART_RX_vect)
#define UART_TX_ISR (USART_TX_vect)

//---------------------------------------------------------------------------
typedef enum {
    CMD_UARTPLUS_SET_BAUDRATE = 0x80,
    CMD_UARTPLUS_SET_BUFFERS,
    CMD_UARTPLUS_SET_RX_ENABLE,
    CMD_UARTPLUS_SET_RX_DISABLE,
    CMD_UARTPLUS_SET_IDENTITY,
    CMD_UARTPLUS_SET_BLOCKING,
    CMD_UARTPLUS_SET_NONBLOCKING,
} CMD_UARTPLUS;

//---------------------------------------------------------------------------
/*!
    Implements a UART driver on the ATMega1284p
 */
class ATMegaUARTPlus : public UartDriver
{
public:
    virtual void     Init();
    virtual uint8_t  Open();
    virtual uint8_t  Close();
    virtual uint16_t Read(uint16_t u16Bytes_, uint8_t* pu8Data_);

    virtual uint16_t Write(uint16_t u16Bytes_, uint8_t* pu8Data_);

    virtual uint16_t Control(uint16_t u16Event_, void* pvIn_, uint16_t u16SizeIn_, void* pvOut_, uint16_t u16SizeOut_);
    /*!
     *  Called from the transmit complete ISR - implements a
     *  callback/transmit state-machine
     */
    void TxISR();

    /*!
     *  Called from the receive-complete ISR - implements a
     *  callback/receive state-machine
     */
    void RxISR();

    /*!
     * \brief StreamInCallback
     */
    void StreamInCallback();

private:

    /*!
     * \brief StreamOutByte
     * \param u8In_
     * \return
     */
    bool StreamOutByte(uint8_t u8In_);

    /*!
     * \brief StartTx
     */
    void StartTx();

    /*!
     * \brief SetBaud
     * \param u32Baud_
     */
    void SetBaud(uint32_t u32Baud_);

    Streamer m_clStreamIn;
    Streamer m_clStreamOut;

    Notify m_clNotifyIn;
    Notify m_clNotifyOut;

    Timer m_clTimerIn;

    uint8_t m_u8Identity; //!< Which physical UART this driver corresponds to
    bool    m_bBlocking;
    bool    m_bStartTx;
};

#endif
