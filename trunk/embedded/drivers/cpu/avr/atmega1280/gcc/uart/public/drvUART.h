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

    \file   drvUART.h

    \brief  Atmega328p serial port driver

 */
#ifndef __DRVUART_H_
#define __DRVUART_H_

#include "kerneltypes.h"
#include "driver.h"

//---------------------------------------------------------------------------
// UART defines - user-configurable for different targets
//---------------------------------------------------------------------------
#define UART_SRA                (UCSR0A)
#define UART_SRB                (UCSR0B)
#define UART_SRC                (UCSR0C)
#define UART_BAUDH              (UBRR0H)
#define UART_BAUDL              (UBRR0L)
#define UART_RXEN               (RXEN0)
#define UART_TXEN               (TXEN0)
#define UART_TXCIE              (TXCIE0)
#define UART_RXCIE              (RXCIE0)
#define UART_UDR                (UDR0)
#define UART_UDRE               (UDRE0)
#define UART_RXC                (RXC0)

#define UART_DEFAULT_BAUD       ((uint32_t)57600)

#define UART_RX_ISR             (UART0_RX_vect)
#define UART_TX_ISR             (UART0_TX_vect)

//---------------------------------------------------------------------------
typedef enum
{
    CMD_SET_BAUDRATE = 0x80,
    CMD_SET_BUFFERS,    
    CMD_SET_RX_ESCAPE,
    CMD_SET_RX_CALLBACK,
    CMD_SET_RX_ECHO,
    CMD_SET_RX_ENABLE,
    CMD_SET_RX_DISABLE
} CMD_UART;

class ATMegaUART;
//---------------------------------------------------------------------------
typedef struct _UartData_t UartData_t;

//---------------------------------------------------------------------------
typedef void (*UART_Rx_Callback_t)( ATMegaUART *pclUART );

//---------------------------------------------------------------------------
/*!
 *  Implements a UART driver on the ATMega328p
 */
class ATMegaUART : public Driver
{
    
public:        
    virtual void Init();
    virtual uint8_t Open();
    virtual uint8_t Close();
    virtual uint16_t Read( uint16_t u16Bytes_, 
                           uint8_t *pu8Data_ );
                                 
    virtual uint16_t Write( uint16_t u16Bytes_, 
                            uint8_t *pu8Data_ );
                                  
    virtual uint16_t Control( uint16_t u16Event_, 
                              void *pvIn_, 
                              uint16_t u16SizeIn_, 
                              void *pvOut_, 
                              uint16_t u16SizeOut_ );
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
     *  \fn uint8_t *GetRxBuffer(void)
     *  
     *  Return a pointer to the receive buffer for this UART.
     *
     *  \return pointer to the driver's RX buffer
     */
    uint8_t *GetRxBuffer(void) { return m_pu8RxBuffer; }
    
    /*! 
     *  \fn uint8_t *GetTxBuffer(void)
     *  
     *  Return a pointer to the transmit buffer for this UART.
     *  
     *  \return pointer to the driver's TX buffer
     */
    uint8_t *GetTxBuffer(void) { return m_pu8TxBuffer; }

private:

    void SetBaud(void);
    void StartTx(void);
    
    uint8_t m_u8TxSize;                //!< Size of the TX Buffer
    uint8_t m_u8TxHead;                //!< Head index
    uint8_t m_u8TxTail;                //!< Tail index
    
    uint8_t m_u8RxSize;                //!< Size of the RX Buffer
    uint8_t m_u8RxHead;                //!< Head index
    uint8_t m_u8RxTail;                //!< Tail index
    
    bool m_bRxOverflow;                //!< Receive buffer overflow
    bool m_bEcho;                      //!< Whether or not to echo RX characters to TX
    
    uint8_t *m_pu8RxBuffer;            //!< Receive buffer pointer
    uint8_t *m_pu8TxBuffer;            //!< Transmit buffer pointer
    
    uint32_t m_u32BaudRate;            //!< Baud rate
    
    uint8_t m_u8RxEscape;              //!< Escape character
    
    UART_Rx_Callback_t    pfCallback;  //!< Callback function on matched escape character        
};

#endif 
