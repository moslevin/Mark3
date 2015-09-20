/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012 Funkenstein Software Consulting, all rights reserved.
See license.txt for more information
===========================================================================*/
/*!
    \file drvFlavrJoy.cpp
    
    \brief Co-simulated joystick input, using flAVR as a host.

*/

#include "kerneltypes.h"
#include "driver.h"
#include "joystick.h"
#include "drvFlavrJoy.h"


volatile uint8_t g_u8FlavrJoy = 0;
volatile uint8_t g_u8FlavrJoyUp = 0;
//---------------------------------------------------------------------------
#define FLAVR_JOY_UP		0x01
#define FLAVR_JOY_DOWN		0x02
#define FLAVR_JOY_LEFT		0x04
#define FLAVR_JOY_RIGHT		0x08
#define FLAVR_JOY_FIRE		0x10

//---------------------------------------------------------------------------
void FlavrJoystick::Init()
{
} 

//---------------------------------------------------------------------------
uint8_t FlavrJoystick::Open()
{
	
    return 0;
}

//---------------------------------------------------------------------------
uint16_t FlavrJoystick::Control( uint16_t u16Event_, void *pvIn_, uint16_t u16SizeIn_,
                                                  void *pvOut_, uint16_t u16SizeOut_ )
{    
    switch(u16Event_)
    {
        // Main event - poll joypad.
        case JOYSTICK_SCAN:
        {
            ScanInternal();
        }
            break;
        default:
            break;
    }
    return 0;
}

//---------------------------------------------------------------------------
void FlavrJoystick::ScanInternal()
{
	uint8_t u8Tmp;

	g_u8FlavrJoyUp = 1; //!! This command triggers the kernel-aware simulator to update g_u8FlavrJoy.
	u8Tmp = g_u8FlavrJoy;

	if (u8Tmp & FLAVR_JOY_LEFT)
	{
		m_stCurrentReport.bLeft = true;
	}
	else
	{
		m_stCurrentReport.bLeft = false;
	}
	
	if (u8Tmp & FLAVR_JOY_RIGHT)
	{
		m_stCurrentReport.bRight = true;
	}
	else
	{
		m_stCurrentReport.bRight = false;
	}
	
	if (u8Tmp & FLAVR_JOY_UP)
	{
		m_stCurrentReport.bUp = true;
	}
	else
	{
		m_stCurrentReport.bUp = false;
	}
	
	if (u8Tmp & FLAVR_JOY_DOWN)
	{
		m_stCurrentReport.bDown = true;
	}
	else
	{
		m_stCurrentReport.bDown = false;
	}
	
	if (u8Tmp & FLAVR_JOY_FIRE)
	{
		m_stCurrentReport.bButton1 = true;
	}
	else
	{
		m_stCurrentReport.bButton1 = false;
	}
		
}
