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
#pragma once

#include "kerneltypes.h"
#include "driver.h"
#include "drvUART.h"
#include "notify.h"
#include "streamer.h"
#include "timer.h"
//---------------------------------------------------------------------------
// UART defines - user-configurable for different targets
//---------------------------------------------------------------------------
#define UART0_SRA (UCSR0A)
#define UART0_SRB (UCSR0B)
#define UART0_SRC (UCSR0C)
#define UART0_BAUDH (UBRR0H)
#define UART0_BAUDL (UBRR0L)
#define UART0_RXEN (RXEN0)
#define UART0_TXEN (TXEN0)
#define UART0_TXCIE (TXCIE0)
#define UART0_RXCIE (RXCIE0)
#define UART0_UDR (UDR0)
#define UART0_UDRE (UDRE0)
#define UART0_RXC (RXC0)

#define UART0_RX_ISR (USART0_RX_vect)
#define UART0_TX_ISR (USART0_TX_vect)

//---------------------------------------------------------------------------
#define UART1_SRA (UCSR1A)
#define UART1_SRB (UCSR1B)
#define UART1_SRC (UCSR1C)
#define UART1_BAUDH (UBRR1H)
#define UART1_BAUDL (UBRR1L)
#define UART1_RXEN (RXEN1)
#define UART1_TXEN (TXEN1)
#define UART1_TXCIE (TXCIE1)
#define UART1_RXCIE (RXCIE1)
#define UART1_UDR (UDR1)
#define UART1_UDRE (UDRE1)
#define UART1_RXC (RXC1)

#define UART1_RX_ISR (USART1_RX_vect)
#define UART1_TX_ISR (USART1_TX_vect)

#define UART_DEFAULT_BAUD ((uint32_t)57600)

namespace Mark3
{

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

} //namespace Mark3
