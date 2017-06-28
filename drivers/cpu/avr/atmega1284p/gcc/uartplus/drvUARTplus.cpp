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
#include "drvUARTplus.h"
#include "driver.h"
#include "thread.h"
#include "notify.h"
#include "streamer.h"
#include "threadport.h"
#include "kerneltimer.h"
#include "kernelaware.h"

#include <avr/io.h>
#include <avr/interrupt.h>

//---------------------------------------------------------------------------
static ATMegaUARTPlus* pclActive0; // Pointer to the active object
static ATMegaUARTPlus* pclActive1; // Pointer to the active object

//---------------------------------------------------------------------------
void ATMegaUARTPlus::SetBaud(uint32_t u32Baud_)
{
    uint16_t u16BaudTemp;
    uint16_t u16PortTemp;

    // Calculate the baud rate from the value in the driver.
    u16BaudTemp = (uint16_t)(((PORT_SYSTEM_FREQ / 16) / u32Baud_) - 1);

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
void ATMegaUARTPlus::Init(void)
{
    SetBaud(UART_DEFAULT_BAUD);

    m_clNotifyIn.Init();
    m_clNotifyOut.Init();

    m_clTimerIn.Init();

    m_bBlocking = true;
    m_bStartTx = true;
}

//---------------------------------------------------------------------------
uint8_t ATMegaUARTPlus::Open()
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
uint8_t ATMegaUARTPlus::Close(void)
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
uint16_t ATMegaUARTPlus::Control(uint16_t u16CmdId_, void* pvIn_, uint16_t u16SizeIn_, void* pvOut_, uint16_t u16SizeOut_)
{
    switch (static_cast<UartOpcode_t>(u16CmdId_)) {
        case UART_OPCODE_SET_BAUDRATE: {
            uint32_t u32BaudRate = *((uint32_t*)pvIn_);
            SetBaud(u32BaudRate);
        } break;
        case UART_OPCODE_SET_BUFFERS: {
            uint8_t* pau8In = (uint8_t*)pvIn_;
            uint8_t* pau8Out = (uint8_t*)pvOut_;
            m_clStreamIn.Init(pau8In, u16SizeIn_);
            m_clStreamOut.Init(pau8Out, u16SizeOut_);
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
        case UART_OPCODE_SET_BLOCKING: {
            m_bBlocking = true;
        } break;
        case UART_OPCODE_SET_NONBLOCKING: {
            m_bBlocking = false;
        } break;
        default: break;
    }
    return 0;
}

//---------------------------------------------------------------------------
uint16_t ATMegaUARTPlus::Read(uint16_t u16SizeIn_, uint8_t* pvData_)
{
    uint8_t* pu8Data = pvData_;
    uint16_t u16Read = 0;
    while (u16SizeIn_) {
        if (m_clStreamIn.Read(pu8Data)) {
            pu8Data++;
            u16SizeIn_--;
            u16Read++;
        } else {
            if (m_bBlocking) {
                m_clNotifyIn.Wait(0);
            } else {
                return u16Read;
            }
        }
    }
    return u16Read;
}

//---------------------------------------------------------------------------
bool ATMegaUARTPlus::StreamOutByte(uint8_t u8In_)
{
    if (!m_clStreamOut.CanWrite()) {
        return false;
    }
    bool bStart = false;
    if (m_clStreamOut.IsEmpty() && m_bStartTx) {
        bStart = true;
        m_bStartTx = false;
    }
    m_clStreamOut.Write(u8In_);
    if (bStart) {
        StartTx();
    }
    return true;
}

//---------------------------------------------------------------------------
uint16_t ATMegaUARTPlus::Write(uint16_t u16SizeOut_, uint8_t* pvData_)
{
    uint8_t* pu8Data = pvData_;
    uint16_t u16Written = 0;
    while (u16SizeOut_) {
        if (StreamOutByte(*pu8Data)) {
            pu8Data++;
            u16SizeOut_--;
            u16Written++;
        } else {
            if (m_bBlocking) {
                m_clNotifyOut.Wait(0);
            } else {
                return u16Written;
            }
        }
    }
    return u16Written;
}

//---------------------------------------------------------------------------
void ATMegaUARTPlus::StartTx()
{
    CS_ENTER();
    uint8_t u8Byte;
    if (m_clStreamOut.Read(&u8Byte)) {
        if (m_u8Identity == 0) {
            UART0_UDR = u8Byte;
        } else {
            UART1_UDR = u8Byte;
        }
    }
    CS_EXIT();
}

//---------------------------------------------------------------------------
void ATMegaUARTPlus::StreamInCallback()
{
    CS_ENTER();
    if (m_clStreamIn.IsEmpty()) {
        m_clTimerIn.Stop();
    }
    CS_EXIT();
    m_clNotifyIn.Signal();
}

//---------------------------------------------------------------------------
static void StreamTimerCallback(Thread* pclOwner_, void* pvData_)
{
    ATMegaUARTPlus *pclThis = (ATMegaUARTPlus*)pvData_;
    pclThis->StreamInCallback();
}

//---------------------------------------------------------------------------
void ATMegaUARTPlus::RxISR()
{
    uint8_t u8Byte;
    if (m_u8Identity == 0) {
        u8Byte = UART0_UDR;
    } else {
        u8Byte = UART1_UDR;
    }

    if (!m_clStreamIn.CanWrite()) {
        return;
    }

    bool bStart = false;
    if (m_clStreamIn.IsEmpty()) {
        bStart = true;
    }
    m_clStreamIn.Write(u8Byte);
    if (bStart) {
        m_clTimerIn.Start(true, 50, StreamTimerCallback, (void*)this);
    }
    if ((u8Byte == 0) || (u8Byte == '\n')) {
        m_clNotifyIn.Signal();
    }
}

//---------------------------------------------------------------------------
void ATMegaUARTPlus::TxISR()
{
    if (!m_clStreamOut.IsEmpty() && !m_bStartTx) {
        StartTx();
    } else {
        m_bStartTx = true;
        m_clNotifyOut.Signal();
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
ISR(UART0_TX_ISR)
{
    pclActive0->TxISR();
}
ISR(UART1_TX_ISR)
{
    pclActive1->TxISR();
}
