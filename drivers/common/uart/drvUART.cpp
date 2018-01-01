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
    @brief UART abstraction interface
*/
#include "drvUART.h"


namespace Mark3
{
//---------------------------------------------------------------------------
bool UartDriver::SetBaudRate(uint32_t u32BaudRate_)
{
    return 0 == Control(UART_OPCODE_SET_BAUDRATE, &u32BaudRate_, 0, 0, 0);
}

//---------------------------------------------------------------------------
bool UartDriver::SetBuffers(uint8_t* pu8RxBuffer_, uint32_t u32RxBufferSize_,
                uint8_t* pu8TxBuffer_, uint32_t u32TxBufferSize_)
{
    return 0 == Control(UART_OPCODE_SET_BUFFERS, pu8RxBuffer_, u32RxBufferSize_, pu8TxBuffer_, u32TxBufferSize_);
}                    
                
//---------------------------------------------------------------------------
bool UartDriver::EnableRx(bool bEnable_)
{
    if (bEnable_) {
        return 0 == Control(UART_OPCODE_SET_RX_ENABLE, 0, 0 ,0 ,0);
    }
    return 0 == Control(UART_OPCODE_SET_RX_DISABLE, 0, 0, 0, 0);
}

//---------------------------------------------------------------------------
bool UartDriver::SetPortIdentity(uint8_t u8PortIdentity_)
{
    return 0 == Control(UART_OPCODE_SET_IDENTITY, &u8PortIdentity_, 0, 0, 0);
}

//---------------------------------------------------------------------------
bool UartDriver::SetPortBlocking(bool bBlocking_)
{
    if (bBlocking_) {
        return 0 == Control(UART_OPCODE_SET_BLOCKING, 0, 0, 0, 0);
    }
    return 0 == Control(UART_OPCODE_SET_NONBLOCKING, 0, 0, 0, 0);
}
} //namespace Mark3
