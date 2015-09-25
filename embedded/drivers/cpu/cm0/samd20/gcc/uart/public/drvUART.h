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

    \file   drvUART.h

    \brief  SAM D20s serial port driver

*/
#ifndef __DRVUART_H_
#define __DRVUART_H_

#include "kerneltypes.h"
#include "driver.h"
#include "sys_uart.h"

//---------------------------------------------------------------------------
// UART defines - user-configurable for different targets
//---------------------------------------------------------------------------
#define UART_DEFAULT_BAUD       ((uint32_t)57600)

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

class D20_UART;
//---------------------------------------------------------------------------
typedef struct _UartData_t UartData_t;

//---------------------------------------------------------------------------
typedef void (*UART_Rx_Callback_t)( D20_UART *pclUART );

//---------------------------------------------------------------------------
class D20_UART : public Driver
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

private:

    SysUART m_clUART;

};

#endif 
