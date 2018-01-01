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

    \file   drvUART.h

    \brief  Atmega328p serial port driver

 */
#pragma once

#include "kerneltypes.h"
#include "driver.h"
#include "drvUART.h"

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

//---------------------------------------------------------------------------

namespace Mark3
{

class ATMegaUART;
//---------------------------------------------------------------------------
typedef struct _UartData_t UartData_t;

//---------------------------------------------------------------------------
/*!
 *   Implements a UART driver on the ATMega328p
 */
class ATMegaUART : public UartDriver
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
     *  \brief GetRxBuffer
     *
     *  Return a pointer to the receive buffer for this UART.
     *
     *  \return pointer to the driver's RX buffer
     */
    uint8_t* GetRxBuffer(void) { return m_pu8RxBuffer; }
    /*!
     *  \brief GetTxBuffer
     *
     *  Return a pointer to the transmit buffer for this UART.
     *
     *  \return pointer to the driver's TX buffer
     */
    uint8_t* GetTxBuffer(void) { return m_pu8TxBuffer; }
private:
    void SetBaud(void);
    void StartTx(void);

    uint8_t m_u8TxSize; //!< Size of the TX Buffer
    uint8_t m_u8TxHead; //!< Head index
    uint8_t m_u8TxTail; //!< Tail index

    uint8_t m_u8RxSize; //!< Size of the RX Buffer
    uint8_t m_u8RxHead; //!< Head index
    uint8_t m_u8RxTail; //!< Tail index

    uint8_t* m_pu8RxBuffer; //!< Receive buffer pointer
    uint8_t* m_pu8TxBuffer; //!< Transmit buffer pointer

    uint32_t m_u32BaudRate; //!< Baud rate
    bool m_bRxOverflow; //!< indicates received overflow
    uint8_t m_u8Identity; //!< port number
};

} //namespace Mark3
