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

    \file   drvUART.cpp

    \brief  Atmega328p serial port driver
*/

#include "kerneltypes.h"
#include "drvATMegaUART.h"
#include "driver.h"
#include "thread.h"
#include "threadport.h"
#include "kerneltimer.h"

#include <avr/io.h>
#include <avr/interrupt.h>

//---------------------------------------------------------------------------
static ATMegaUART* pclActive0; // Pointer to the active object
static ATMegaUART* pclActive1; // Pointer to the active object

//---------------------------------------------------------------------------
void ATMegaUART::SetBaud(void)
{
    uint16_t u16BaudTemp;
    uint16_t u16PortTemp;

    // Calculate the baud rate from the value in the driver.
    u16BaudTemp = (uint16_t)(((PORT_SYSTEM_FREQ / 16) / m_u32BaudRate) - 1);

    if (m_u8Identity == 0) {
        // Save the current port config registers
        u16PortTemp = UART0_SRB;

        // Clear the registers (disable rx/tx/interrupts)
        UART0_SRB = 0;
        UART0_SRA = 0;

        // Set the baud rate high/low values.
        UART0_BAUDH = (uint8_t)(u16BaudTemp >> 8);
        UART0_BAUDL = (uint8_t)(u16BaudTemp & 0x00FF);

        // Restore the Rx/Tx flags to their previous state
        UART0_SRB = u16PortTemp;
    } else {
        // Save the current port config registers
        u16PortTemp = UART1_SRB;

        // Clear the registers (disable rx/tx/interrupts)
        UART1_SRB = 0;
        UART1_SRA = 0;

        // Set the baud rate high/low values.
        UART1_BAUDH = (uint8_t)(u16BaudTemp >> 8);
        UART1_BAUDL = (uint8_t)(u16BaudTemp & 0x00FF);

        // Restore the Rx/Tx flags to their previous state
        UART1_SRB = u16PortTemp;
    }
}

//---------------------------------------------------------------------------
void ATMegaUART::Init(void)
{
    // Set up the FIFOs
    m_u8TxHead    = 0;
    m_u8TxTail    = 0;
    m_u8RxHead    = 0;
    m_u8RxTail    = 0;
    m_bRxOverflow = 0;
    m_u32BaudRate = UART_DEFAULT_BAUD;

    // Clear flags

    SetBaud();
}

//---------------------------------------------------------------------------
uint8_t ATMegaUART::Open()
{
    // Enable Rx/Tx + Interrupts
    if (m_u8Identity == 0) {
        UART0_SRB |= (1 << UART0_RXEN) | (1 << UART0_TXEN);
        UART0_SRB |= (1 << UART0_RXCIE) | (1 << UART0_TXCIE);
        // Set frame format: 8 N 1
        UART0_SRC  = 0x06;
        pclActive0 = this;
    } else {
        UART1_SRB |= (1 << UART1_RXEN) | (1 << UART1_TXEN);
        UART1_SRB |= (1 << UART1_RXCIE) | (1 << UART1_TXCIE);
        // Set frame format: 8 N 1
        UART1_SRC  = 0x06;
        pclActive1 = this;
    }

    return 0;
}

//---------------------------------------------------------------------------
uint8_t ATMegaUART::Close(void)
{
    // Disable Rx/Tx + Interrupts
    if (m_u8Identity == 0) {
        UART0_SRB &= ~((1 << UART0_RXEN) | (1 << UART0_TXEN));
        UART0_SRB &= ~((1 << UART0_TXCIE) | (1 << UART0_RXCIE));
    } else {
        UART1_SRB &= ~((1 << UART1_RXEN) | (1 << UART1_TXEN));
        UART1_SRB &= ~((1 << UART1_TXCIE) | (1 << UART1_RXCIE));
    }
    return 0;
}

//---------------------------------------------------------------------------
uint16_t ATMegaUART::Control(uint16_t u16CmdId_, void* pvIn_, uint16_t u16SizeIn_, void* pvOut_, uint16_t u16SizeOut_)
{
    switch (static_cast<UartOpcode_t>(u16CmdId_)) {
        case UART_OPCODE_SET_BAUDRATE: {
            uint32_t u32BaudRate = *((uint32_t*)pvIn_);
            m_u32BaudRate        = u32BaudRate;
            SetBaud();
        } break;
        case UART_OPCODE_SET_BUFFERS: {
            m_pu8RxBuffer = (uint8_t*)pvIn_;
            m_pu8TxBuffer = (uint8_t*)pvOut_;
            m_u8RxSize    = u16SizeIn_;
            m_u8TxSize    = u16SizeOut_;
        } break;       
        case UART_OPCODE_SET_RX_ENABLE: {
            if (m_u8Identity == 0) {
                UART0_SRB |= (1 << UART0_RXEN);
            } else {
                UART1_SRB |= (1 << UART1_RXEN);
            }
        } break;
        case UART_OPCODE_SET_RX_DISABLE: {
            if (m_u8Identity == 0) {
                UART0_SRB &= ~(1 << UART0_RXEN);
            } else {
                UART1_SRB &= ~(1 << UART1_RXEN);
            }
        } break;
        case UART_OPCODE_SET_IDENTITY: {
            m_u8Identity = *((uint8_t*)pvIn_);
        } break;
        default: break;
    }
    return 0;
}

//---------------------------------------------------------------------------
uint16_t ATMegaUART::Read(uint16_t u16SizeIn_, uint8_t* pvData_)
{
    // Read a string of characters of length N.  Return the number of bytes
    // actually read.  If less than the 1 length, this indicates that
    // the buffer is full and that the app needs to wait.

    uint16_t i       = 0;
    uint16_t u16Read = 0;
    bool     bExit   = 0;
    uint8_t* pu8Data = (uint8_t*)pvData_;

    for (i = 0; i < u16SizeIn_; i++) {
        // If Tail != Head, there's data in the buffer.
        CS_ENTER();
        if (m_u8RxTail != m_u8RxHead) {
            // We have room to add the byte, so add it.
            pu8Data[i] = m_pu8RxBuffer[m_u8RxTail];

            // Update the buffer head pointer.
            m_u8RxTail++;
            if (m_u8RxTail >= m_u8RxSize) {
                m_u8RxTail = 0;
            }
            u16Read++;
        } else {
            // Can't do anything else - the buffer is empty
            bExit = 1;
        }
        CS_EXIT();

        // If we have to bail because the buffer is empty, do it now.
        if (bExit == 1) {
            break;
        }
    }
    return u16Read;
}

//---------------------------------------------------------------------------
uint16_t ATMegaUART::Write(uint16_t u16SizeOut_, uint8_t* pvData_)
{
    // Write a string of characters of length N.  Return the number of bytes
    // actually written.  If less than the 1 length, this indicates that
    // the buffer is full and that the app needs to wait.
    uint16_t i          = 0;
    uint16_t u16Written = 0;
    uint8_t  u8Next;
    bool     bActivate = 0;
    bool     bExit     = 0;
    uint8_t* pu8Data   = (uint8_t*)pvData_;

    // If the head = tail, we need to start sending data out the data ourselves.
    if (m_u8TxHead == m_u8TxTail) {
        bActivate = 1;
    }

    for (i = 0; i < u16SizeOut_; i++) {
        CS_ENTER();
        // Check that head != tail (we have room)
        u8Next = (m_u8TxHead + 1);
        if (u8Next >= m_u8TxSize) {
            u8Next = 0;
        }

        if (u8Next != m_u8TxTail) {
            // We have room to add the byte, so add it.
            m_pu8TxBuffer[m_u8TxHead] = pu8Data[i];

            // Update the buffer head pointer.
            m_u8TxHead = u8Next;
            u16Written++;
        } else {
            // Can't do anything - buffer is full
            bExit = 1;
        }
        CS_EXIT();

        // bail if the buffer is full
        if (bExit == 1) {
            break;
        }
    }

    // Activate the transmission if we're currently idle
    if (bActivate == 1) {
        // We know we're idle if we get here.
        CS_ENTER();
        if (m_u8Identity == 0) {
            if (UART0_SRA & (1 << UDRE0)) {
                StartTx();
            }
        } else {
            if (UART1_SRA & (1 << UDRE0)) {
                StartTx();
            }
        }

        CS_EXIT();
    }

    return u16Written;
}

//---------------------------------------------------------------------------
void ATMegaUART::StartTx(void)
{
    // Send the tail byte out.
    uint8_t u8Next;

    CS_ENTER();

    // Send the byte at the tail index
    if (m_u8Identity == 0) {
        UART0_UDR = m_pu8TxBuffer[m_u8TxTail];
    } else {
        UART1_UDR = m_pu8TxBuffer[m_u8TxTail];
    }

    // Update the tail index
    u8Next = (m_u8TxTail + 1);
    if (u8Next >= m_u8TxSize) {
        u8Next = 0;
    }
    m_u8TxTail = u8Next;

    CS_EXIT();
}

//---------------------------------------------------------------------------
void ATMegaUART::RxISR()
{
    uint8_t u8Temp;
    uint8_t u8Next;

    // Read the byte from the data buffer register
    if (m_u8Identity == 0) {
        u8Temp = UART0_UDR;
    } else {
        u8Temp = UART1_UDR;
    }

    // Check that head != tail (we have room)
    u8Next = (m_u8RxHead + 1);
    if (u8Next >= m_u8RxSize) {
        u8Next = 0;
    }

    // Always add the byte to the buffer (but flag an error if it's full...)
    m_pu8RxBuffer[m_u8RxHead] = u8Temp;

    // Update the buffer head pointer.
    m_u8RxHead = u8Next;

    // If the buffer's full, discard the oldest byte in the buffer and flag an error
    if (u8Next == m_u8RxTail) {
        // Update the buffer tail pointer
        m_u8RxTail = (m_u8RxTail + 1);
        if (m_u8RxTail >= m_u8RxSize) {
            m_u8RxTail = 0;
        }

        // Flag an error - the buffer is full
        m_bRxOverflow = 1;
    }
}

//---------------------------------------------------------------------------
ISR(UART0_RX_ISR)
{
    pclActive0->RxISR();
}
ISR(UART1_RX_ISR)
{
    pclActive1->RxISR();
}

//---------------------------------------------------------------------------
void ATMegaUART::TxISR()
{
    // If the head != tail, there's something to send.
    if (m_u8TxHead != m_u8TxTail) {
        StartTx();
    }
}

//---------------------------------------------------------------------------
ISR(UART0_TX_ISR)
{
    pclActive0->TxISR();
}
ISR(UART1_TX_ISR)
{
    pclActive1->TxISR();
}
