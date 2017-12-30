#include <avr/io.h>

#include "mark3.h"
#include "drvUARTplus.h"
#include "bsp_uarts.h"

ATMegaUARTPlus     clUART0;
static uint8_t au8RX0[32];
static uint8_t au8TX0[32];

ATMegaUARTPlus     clUART1;
static uint8_t au8RX1[32];
static uint8_t au8TX1[32];

void bsp_uarts_init(void)
{
    clUART0.Init();
    clUART0.SetName("/dev/tty0");
    uint8_t u8Identity = 0;
    clUART0.Control(UART_OPCODE_SET_IDENTITY, &u8Identity, 0, 0, 0);
    uint32_t u32Baud = 57600;
    clUART0.Control(UART_OPCODE_SET_BAUDRATE, &u32Baud, 0, 0, 0);
    clUART0.Control(UART_OPCODE_SET_BUFFERS, au8RX0, 32, au8TX0, 32);
    clUART0.Open();

    // Initialize UART1
    clUART1.Init();
    clUART1.SetName("/dev/tty1");
    u8Identity = 1;
    clUART1.Control(UART_OPCODE_SET_IDENTITY, &u8Identity, 0, 0, 0);
    clUART1.Control(UART_OPCODE_SET_BAUDRATE, &u32Baud, 0, 0, 0);
    clUART1.Control(UART_OPCODE_SET_BUFFERS, au8RX1, 32, au8TX1, 32);
    clUART1.Open();

    DriverList::GetInstance()->Add(&clUART0);
    DriverList::GetInstance()->Add(&clUART1);
}
