#ifndef __BSP_UART_H__
#define __BSP_UART_H__

#include "mark3.h"
#include "drvUARTplus.h"

extern ATMegaUARTPlus clUART0;
extern ATMegaUARTPlus clUART1;

void bsp_uarts_init(void);

#endif
