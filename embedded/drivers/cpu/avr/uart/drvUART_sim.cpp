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

    \brief  Atmega328p serial port driver
*/

#include "kerneltypes.h"
#include "drvUART.h"
#include "driver.h"
#include "thread.h"
#include "threadport.h"
#include "kerneltimer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//---------------------------------------------------------------------------
static ATMegaUART *pclActive;	// Pointer to the active object

//---------------------------------------------------------------------------
void ATMegaUART::SetBaud(void)
{    
}

//---------------------------------------------------------------------------
void ATMegaUART::Init(void)
{    
}

//---------------------------------------------------------------------------
uint8_t ATMegaUART::Open()
{
	return 0;
}

//---------------------------------------------------------------------------
uint8_t ATMegaUART::Close(void)
{
    return 0;
}

//---------------------------------------------------------------------------
uint16_t ATMegaUART::Control( uint16_t u16CmdId_, void *pvIn_, uint16_t u16SizeIn_, void *pvOut_, uint16_t u16SizeOut_)
{
    switch ((CMD_UART)u16CmdId_)
    {
        case CMD_SET_BAUDRATE:
        {
        }
            break;
        case CMD_SET_BUFFERS:
        {
        }            
            break;		
        case CMD_SET_RX_ESCAPE:
        {
        }
            break;
        case CMD_SET_RX_CALLBACK:
        {
        }
            break;
        case CMD_SET_RX_ECHO:
        {
        }
            break;
		case CMD_SET_RX_ENABLE:
		{
		}
			break;
		case CMD_SET_RX_DISABLE:
		{
		}
			break;						
        default:
            break;
    }
    return 0;
}

//---------------------------------------------------------------------------
uint16_t ATMegaUART::Read( uint16_t u16SizeIn_, uint8_t *pvData_ )
{
    return fread( pvData_, 1, u16SizeIn_, stderr );
}

//---------------------------------------------------------------------------
uint16_t ATMegaUART::Write(uint16_t u16SizeOut_, uint8_t *pvData_)
{
    return fwrite( pvData_, 1, u16SizeOut_, stderr );
}

//---------------------------------------------------------------------------
void ATMegaUART::StartTx(void)
{
    // stub
}

//---------------------------------------------------------------------------
void ATMegaUART::RxISR()
{
    // stub
}

//---------------------------------------------------------------------------
void ATMegaUART::TxISR()
{
    // Stub
}
