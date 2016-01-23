/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012-2016 Funkenstein Software Consulting, all rights reserved.
See license.txt for more information
===========================================================================*/
/*!
    \file joystick.cpp
    
    \brief Joystick driver base-class implementation
*/
#include "kerneltypes.h"
#include "joystick.h"

//---------------------------------------------------------------------------
void JoystickDriver::Scan()
{
    m_stLastReport.u16Digital = m_stCurrentReport.u16Digital;
    m_stLastReport.u16AnalogX1 = m_stCurrentReport.u16AnalogX1;
    m_stLastReport.u16AnalogY1 = m_stCurrentReport.u16AnalogY1;
    m_stLastReport.u16AnalogX2 = m_stCurrentReport.u16AnalogX2;
    m_stLastReport.u16AnalogY2 = m_stCurrentReport.u16AnalogY2;
    Control(JOYSTICK_SCAN, 0, 0, (void*)&m_stCurrentReport, 0);    
}

//---------------------------------------------------------------------------
void JoystickDriver::ReadCurrent( JoystickReport *pstReport_ )
{
    pstReport_->u16Digital = m_stCurrentReport.u16Digital;
    pstReport_->u16AnalogX1 = m_stCurrentReport.u16AnalogX1;
    pstReport_->u16AnalogY1 = m_stCurrentReport.u16AnalogY1;
    pstReport_->u16AnalogX2 = m_stCurrentReport.u16AnalogX2;
    pstReport_->u16AnalogY2 = m_stCurrentReport.u16AnalogY2;
}

//---------------------------------------------------------------------------
void JoystickDriver::ReadPrevious( JoystickReport *pstReport_ )
{
    pstReport_->u16Digital = m_stLastReport.u16Digital;
    pstReport_->u16AnalogX1 = m_stLastReport.u16AnalogX1;
    pstReport_->u16AnalogY1 = m_stLastReport.u16AnalogY1;
    pstReport_->u16AnalogX2 = m_stLastReport.u16AnalogX2;
    pstReport_->u16AnalogY2 = m_stLastReport.u16AnalogY2;
}
