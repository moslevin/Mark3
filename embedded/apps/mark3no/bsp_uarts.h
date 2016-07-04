#ifndef __BSP_UART_H__
#define __BSP_UART_H__

#include "mark3.h"
#include "drvUART.h"

extern ATMegaUART clUART0;
extern ATMegaUART clUART1;

void bsp_uarts_init(void);

#endif
