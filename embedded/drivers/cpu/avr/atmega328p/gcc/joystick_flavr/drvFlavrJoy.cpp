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


volatile K_UCHAR g_ucFlavrJoy = 0;
volatile K_UCHAR g_ucFlavrJoyUp = 0;
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
K_UCHAR FlavrJoystick::Open()
{
	
    return 0;
}

//---------------------------------------------------------------------------
K_USHORT FlavrJoystick::Control( K_USHORT usEvent_, void *pvIn_, K_USHORT usSizeIn_,
                                                  void *pvOut_, K_USHORT usSizeOut_ )
{    
    switch(usEvent_)
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
	K_UCHAR ucTmp;

	g_ucFlavrJoyUp = 1; //!! This command triggers the kernel-aware simulator to update g_ucFlavrJoy.
	ucTmp = g_ucFlavrJoy;

	if (ucTmp & FLAVR_JOY_LEFT)
	{
		m_stCurrentReport.bLeft = true;
	}
	else
	{
		m_stCurrentReport.bLeft = false;
	}
	
	if (ucTmp & FLAVR_JOY_RIGHT)
	{
		m_stCurrentReport.bRight = true;
	}
	else
	{
		m_stCurrentReport.bRight = false;
	}
	
	if (ucTmp & FLAVR_JOY_UP)
	{
		m_stCurrentReport.bUp = true;
	}
	else
	{
		m_stCurrentReport.bUp = false;
	}
	
	if (ucTmp & FLAVR_JOY_DOWN)
	{
		m_stCurrentReport.bDown = true;
	}
	else
	{
		m_stCurrentReport.bDown = false;
	}
	
	if (ucTmp & FLAVR_JOY_FIRE)
	{
		m_stCurrentReport.bButton1 = true;
	}
	else
	{
		m_stCurrentReport.bButton1 = false;
	}
		
}
