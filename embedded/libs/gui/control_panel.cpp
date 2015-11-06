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
    \file control_panel.cpp
    \brief GUI Panel Control Implementation
*/

#include "gui.h"
#include "kerneltypes.h"
#include "draw.h"
#include "graphics.h"
#include "control_panel.h"

//---------------------------------------------------------------------------
void PanelControl::Draw()
{
    GUI_DEBUG_PRINT( "PanelControl::Draw()\n");
    GraphicsDriver *pclDriver = GetParentWindow()->GetDriver();
    DrawRectangle_t stRectangle;    
    uint16_t u16X, u16Y;
    
    GetControlOffset(&u16X, &u16Y);
    
    stRectangle.u16Top = GetTop() + u16Y;
    stRectangle.u16Bottom = stRectangle.u16Top + GetHeight() -1;
    stRectangle.u16Left = GetLeft() + u16X;
    stRectangle.u16Right = stRectangle.u16Left + GetWidth() -1;
    stRectangle.bFill = true;
    stRectangle.u32ineColor = m_uColor;
    stRectangle.uFillColor = m_uColor;
    
    pclDriver->Rectangle(&stRectangle);
}
