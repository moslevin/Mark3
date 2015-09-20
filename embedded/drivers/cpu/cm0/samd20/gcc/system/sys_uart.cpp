/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2013 Funkenstein Software Consulting, all rights reserved.
See license.txt for more information
===========================================================================*/
/*!
  \file  sys_uart.cpp
  \brief Basic UART class implementation for Atmel SAM-D20 SERCOM interfac
 */


#include "sys_uart.h"

//---------------------------------------------------------------------------
#define SYSUART_DEFAULT_BAUD            (57600)
#define SYSUART_DEFAULT_CLOCKFREQ       (8000000)

//---------------------------------------------------------------------------
SysUART::SysUART()
{
    m_eInterface    = SERCOM_IF_0;
    m_u32Baud        = SYSUART_DEFAULT_BAUD;
    m_u8StopBits    = 1;
    m_eMux          = SERCOM_MUX_C;
    m_eTxPad        = SERCOM_PAD_2;
    m_eRxPad        = SERCOM_PAD_3;
    m_bParity       = false;
    m_u32ClockFreqHz = SYSUART_DEFAULT_CLOCKFREQ;
    m_eClockGen     = GCLK_1;
}

//---------------------------------------------------------------------------
SysUART::SysUART(SERCOM_Interface_t eInterface_)
{
    SysUART();
    m_eInterface    = eInterface_;
}

//---------------------------------------------------------------------------
void SysUART::Open(void)
{
    // Enable clocks/power-management for port
    SetupClocks();

    // Setup physical connections/muxing
    SetupPort();

    // Setup peripheral registers
    SetupRegisters();
}

//---------------------------------------------------------------------------
bool SysUART::Write(char cChar_)
{
    SercomUsart *pstPort = GetPort();

    if(!(pstPort->INTFLAG.reg & SERCOm_UART_INTFLAG_DRE))
    {
        return false;
    }

    WriteSync();

    pstPort->DATA.reg = (uint16_t)cChar_;

    while (!(pstPort->INTFLAG.reg & SERCOm_UART_INTFLAG_TXC))
    {
        // Wait for data to complete transmission
    }
    return true;
}

//---------------------------------------------------------------------------
void SysUART::SetupClocks(void)
{
    Clock_t eClockID;
    SysClock clClock;

    // Enable the clock in the power-management module...
    PM->APBCMASK.reg |= 1 << (PM_APBCMASK_SERCOM0_Pos + (uint32_t)m_eInterface);

    // Enable the SERCOM Core Clock for the specified interface
    eClockID = (Clock_t)((uint8_t)CLK_SERCOM0_CORE + (uint8_t)m_eInterface);

    // Hook the SERCOM Core Clock to the appropriate generator
    clClock.SetClockID(eClockID);
    clClock.SetGenerator(m_eClockGen);
    clClock.CommitConfig();

    clClock.Enable(true);
}

//---------------------------------------------------------------------------
void SysUART::SetupPin(const SERCOM_Lookup_t *pstLUT_)
{
    SysPort *pclPort;
    PinMux_t eMux;

    if (SYS_PORT_A == pstLUT_->ePort)
    {
        pclPort = &PortA;
    }
    else
    {
        pclPort = &PortB;
    }

    if (SERCOM_MUX_C == m_eMux)
    {
        eMux = PIN_MUX_C;
    }
    else
    {
        eMux = PIN_MUX_D;
    }

    // Set the pins as inputs, pu32led high.
    pclPort->SetDir(pstLUT_->u8Index, false);
    pclPort->SetPinConfig(pstLUT_->u8Index, true, true, true);

    // Set the pinmux values for the port to the right value for the
    // peripheral
    pclPort->SetPortMux(pstLUT_->u8Index, eMux);
}

//---------------------------------------------------------------------------
uint32_t SysUART::CalculateBaud(void)
{
    /* Temporary variables  */
    uint64_t ratio = 0;
    uint64_t scale = 0;
    uint64_t baud_calculated = 0;

    /* Calculate the BAUD value */
    ratio = ((16 * (uint64_t)m_u32Baud) << 32) / m_u32ClockFreqHz;
    scale = ((uint64_t)1 << 32) - ratio;
    baud_calculated = (65536 * scale) >> 32;

    return baud_calculated;
}

//---------------------------------------------------------------------------
void SysUART::SetupRegisters(void)
{
    SercomUsart *pstUART = GetPort();
    uint32_t u32CtrlA = 0;
    uint32_t u32CtrlB = 0;
    uint32_t u32Baud = 0;

    // Register A fields
    // DORD - Data order: 0 = MSB first, 1 = LSB first
    // CPOL - Clock Polarity (N/A for UART)
    // CMODE - 0 = Async
    // FORM[3:0] Frame Format - 0 = UART, 1 = UART w/Parity
    // RXPO[1:0] Receive Data Pin
    // TXPO Tx Data Pinout
    // IBON Buffer Overflow Notification
    // RUNSTDBY Run in Standby
    // MODE - 0 = UART with external clock, 1 = internal
    // ENABLE - 1 = Peripheral enabled
    // SWRST - Software reset

    // Register B fields
    // RXEN - 1 = receive enable
    // TXEN - 1 = transmit enable
    // PMODE - parity mode (0 = even, 1 = odd)
    // SBMODE - # stop bits 0 = 1b, 1 = 2b
    // CHSIZE[2:0] = 0 = 8b, 1 = 9b, 5 = 5b, 6 = 6b, 7 = 7b

    // Default to RX/TX enabled, user-selected parity & stop bits
    // 8-bit character width.
    u32CtrlB |= SERCOm_UART_CTRLB_RXEN | SERCOm_UART_CTRLB_TXEN;
    if (2 == m_u8StopBits)
    {
        u32CtrlB |= SERCOm_UART_CTRLB_SBMODE;
    }

    if (m_bParity)
    {
        u32CtrlA |= (1 << SERCOm_UART_CTRLA_FORM_Pos);
        u32CtrlB |= SERCOm_UART_CTRLB_PMODE;
    }

    u32CtrlA |= SERCOm_UART_CTRLA_DORD; // LSB first!
    u32CtrlA |= 1 << SERCOm_UART_CTRLA_MODE_Pos; // internal clock
    u32CtrlA |= ((uint32_t)m_eRxPad << SERCOm_UART_CTRLA_RXPO_Pos);
    if (SERCOM_PAD_2 == m_eTxPad)
    {
        u32CtrlA |= SERCOm_UART_CTRLA_TXPO;
    }

    WriteSync();
    u32Baud = CalculateBaud();
    pstUART->BAUD.reg = u32Baud;
    WriteSync();
    pstUART->CTRLA.reg = u32CtrlA;
    WriteSync();
    pstUART->CTRLB.reg = u32CtrlB;

    WriteSync();
    u32CtrlA |= SERCOm_UART_CTRLA_ENABLE;
    pstUART->CTRLA.reg = u32CtrlA;
}

//---------------------------------------------------------------------------
void SysUART::SetupPort(void)
{
    uint16_t u16BaseIdx;

    // Figure out (based on config) where in the LUT to look for our pin configuration
    u16BaseIdx =   ((uint16_t)m_eMux * (uint16_t)SERCOM_PAD_COUNT * (uint16_t)SERCOM_IF_COUNT)
                + ((uint16_t)m_eInterface * (uint16_t)SERCOM_PAD_COUNT);

    // Setup TX pin as input
    SetupPin(&g_astLookup[u16BaseIdx + (uint16_t)m_eTxPad]);

    // Setup RX pin as input
    SetupPin(&g_astLookup[u16BaseIdx + (uint16_t)m_eRxPad]);
}

//---------------------------------------------------------------------------
SercomUsart *SysUART::GetPort(void)
{
    SercomUsart *pstPort;
    switch (m_eInterface)
    {
    case SERCOM_IF_0:
        pstPort = (SercomUsart*)SERCOM0;
        break;
    case SERCOM_IF_1:
        pstPort = (SercomUsart*)SERCOM1;
        break;
    case SERCOM_IF_2:
        pstPort = (SercomUsart*)SERCOM2;
        break;
    case SERCOM_IF_3:
        pstPort = (SercomUsart*)SERCOM3;
        break;
    default:
        return (SercomUsart*)SERCOM0;
    }
    return pstPort;
}

//---------------------------------------------------------------------------
void SysUART::WriteSync()
{
    SercomUsart *pstPort = GetPort();
    while (pstPort->STATUS.reg & SERCOm_UART_STATUS_SYNCBUSY)
    {
        /* Do Nothing */
    }
}
