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
===========================================================================*/
/*!

    \file   drvUART.cpp

    \brief  SAM D20 serial port driver
*/

#include "drvUART.h"

//---------------------------------------------------------------------------
void D20_UART::Init(void)
{    
    m_clUART.SetInterface(SERCOM_IF_3);
    m_clUART.SetTxPad(SERCOM_PAD_2);
    m_clUART.SetRxPad(SERCOM_PAD_3);
    m_clUART.SetMux(SERCOM_MUX_C);

    m_clUART.SetParity(false);
    m_clUART.SetStopBits(1);
    m_clUART.SetBaud(57600);
}

//---------------------------------------------------------------------------
uint8_t D20_UART::Open()
{
    m_clUART.Open();
    return 0;
}

//---------------------------------------------------------------------------
uint8_t D20_UART::Close(void)
{
    return 0;
}

//---------------------------------------------------------------------------
uint16_t D20_UART::Control( uint16_t u16CmdId_, void *pvIn_, uint16_t u16SizeIn_, void *pvOut_, uint16_t u16SizeOut_)
{
    return 0;
}

//---------------------------------------------------------------------------
uint16_t D20_UART::Read( uint16_t u16SizeIn_, uint8_t *pvData_ )
{
    return u16SizeIn_;
}

//---------------------------------------------------------------------------
uint16_t D20_UART::Write(uint16_t u16SizeOut_, uint8_t *pvData_)
{
    for (uint16_t i = 0; i < u16SizeOut_; i++)
    {
        m_clUART.Write(pvData_[i]);
    }
	return u16SizeOut_;
}
