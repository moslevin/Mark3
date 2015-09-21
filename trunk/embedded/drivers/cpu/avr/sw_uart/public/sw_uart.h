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

    \file   sw_uart.h

    \brief  Software UART Implementation
*/

#ifndef __SW_UART_H__
#define __SW_UART_H__

//---------------------------------------------------------------------------
class SoftwareUART
{
public:
    void Init( uint32_t u32Baud_ );

    void SendByte( uint8_t u8Byte_ );

    static void TimerStop(void);

    uint8_t ReceiveByte(void);

private:

    void TimerStart(void);
   
    void TimerInit(void);

    void RxInit(void);

    uint16_t m_u16Prescale;
    uint8_t m_u8TimerPeriod;
};

#endif
