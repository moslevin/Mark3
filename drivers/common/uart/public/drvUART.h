#ifndef __UARTDRIVER_H__
#define __UARTDRIVER_H__

#include "driver.h"
#include "mark3.h"

//---------------------------------------------------------------------------
typedef enum {
    UART_OPCODE_SET_BAUDRATE = 0x80,
    UART_OPCODE_SET_BUFFERS,
    UART_OPCODE_SET_RX_ENABLE,
    UART_OPCODE_SET_RX_DISABLE,
    UART_OPCODE_SET_IDENTITY,
    UART_OPCODE_SET_BLOCKING,
    UART_OPCODE_SET_NONBLOCKING,
} UartOpcode_t;

//---------------------------------------------------------------------------
class UartDriver : public Driver
{
public:
   
   bool SetBaudRate(uint32_t u32BaudRate_);
   
   bool SetBuffers(uint8_t* pu8RxBuffer_, uint32_t u32RxBufferSize_,
                    uint8_t* pu8TxBuffer_, uint32_t u32TxBufferSize_);
                    
   bool EnableRx(bool bEnable_);
   
   bool SetPortIdentity(uint8_t u8PortIdentity_);
   
   bool SetPortBlocking(bool bBlocking_);
   
private:
};


#endif // __UARTDRIVER_H__


