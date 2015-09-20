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
    \file drvBasicJoy.cpp
    
    \brief Generic joystick driver using one GPIO per input.
*/

#include "kerneltypes.h"
#include "driver.h"
#include "joystick.h"
#include "drvBasicJoy.h"

#include <avr/io.h>

//---------------------------------------------------------------------------
void BasicJoystick::Init()
{
} 

//---------------------------------------------------------------------------
uint8_t BasicJoystick::Open()
{
	JOY_DDR  &= ~JOY_LEFT;
	JOY_DDR &= ~JOY_RIGHT;
	JOY_DDR &= ~JOY_DOWN;
	JOY_DDR &= ~JOY_UP;
	JOY_DDR &= ~JOY_FIRE;
	
	JOY_PORT |= JOY_LEFT;
	JOY_PORT |= JOY_RIGHT;
	JOY_PORT |= JOY_UP;
	JOY_PORT |= JOY_DOWN;
	JOY_PORT |= JOY_FIRE;
	
    return 0;
}

//---------------------------------------------------------------------------
uint16_t BasicJoystick::Control( uint16_t u16Event_, void *pvIn_, uint16_t u16SizeIn_,
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
void BasicJoystick::ScanInternal()
{
    if (JOY_PIN & JOY_LEFT)
	{
		m_stCurrentReport.bLeft = false;
	}
	else
	{
		m_stCurrentReport.bLeft = true;
	}
	
	if (JOY_PIN & JOY_RIGHT)
	{
		m_stCurrentReport.bRight = false;
	}
	else
	{
		m_stCurrentReport.bRight = true;
	}
	
	if (JOY_PIN & JOY_UP)
	{
		m_stCurrentReport.bUp = false;
	}
	else
	{
		m_stCurrentReport.bUp = true;
	}
	
	if (JOY_PIN & JOY_DOWN)
	{
		m_stCurrentReport.bDown = false;
	}
	else
	{
		m_stCurrentReport.bDown = true;
	}
	
	if (JOY_PIN & JOY_FIRE)
	{
		m_stCurrentReport.bButton1 = false;
	}
	else
	{
		m_stCurrentReport.bButton1 = true;
	}
	
	
}
