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
  \file  sys_uart.h
  \brief Basic UART class definition for Atmel SAM-D20 SERCOM interface
 */

#ifndef __SYS_UART_H__
#define __SYS_UART_H__

#include <samd20.h>

#include "sys_clocks.h"
#include "sys_port.h"
#include "kerneltypes.h"
#include "sys_sercom.h"

//---------------------------------------------------------------------------
class SysUART
{
public:
    SysUART();

    SysUART(SERCOM_Interface_t eInterface_);

    void SetInterface(SERCOM_Interface_t eInterface_)
        { m_eInterface = eInterface_; }

    void SetMux(SERCOM_Mux_t eMux_)
        {  m_eMux = eMux_; }

    void SetTxPad(SERCOM_Pad_t ePad_)
        { m_eTxPad = ePad_; }

    void SetRxPad(SERCOM_Pad_t ePad_)
        { m_eRxPad = ePad_; }

    void SetBaud(uint32_t u32Baud_)
        { m_u32Baud = u32Baud_; }

    void SetParity(bool bParity_)
        { m_bParity = bParity_; }

    void SetStopBits(uint8_t u8StopBits_ )
        { m_u8StopBits = u8StopBits_; }

    void SetClockGen(ClockGenerator_t eClockGen_)
        { m_eClockGen = eClockGen_; }

    void SetBaudClock(uint32_t u32ClockHz_)
        { m_u32ClockFreqHz = u32ClockHz_; }

    void Open(void);

    bool Write(char cChar_);

private:

    void SetupClocks(void);

    void SetupPin(const SERCOM_Lookup_t *pstLUT_);

    uint32_t CalculateBaud(void);

    void SetupRegisters(void);

    void SetupPort(void);

    SercomUsart *GetPort(void);

    void WriteSync();

    SERCOM_Interface_t  m_eInterface;
    SERCOM_Mux_t        m_eMux;
    SERCOM_Pad_t        m_eTxPad;
    SERCOM_Pad_t        m_eRxPad;

    ClockGenerator_t    m_eClockGen;
    uint32_t             m_u32ClockFreqHz;
    uint32_t             m_u32Baud;
    bool                m_bParity;
    uint8_t             m_u8StopBits;

};


#endif
