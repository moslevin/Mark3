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
    \file control_progress.cpp
    \brief GUI Progress Bar Control

    A simple progress bar control using lines and rectangles to display
    the status of an operation from initialization to completion
*/
#include "gui.h"
#include "kerneltypes.h"
#include "draw.h"
#include "control_progress.h"
#include "gui_palette.h"

//---------------------------------------------------------------------------
void ProgressControl::Init()
{
    m_uBackColor = SECONDARY_1_25;
    m_uBorderColor = SECONDARY_1_100;
    m_uProgressColor = SECONDARY_1_75;
    SetAcceptFocus(false);
}

//---------------------------------------------------------------------------
void ProgressControl::Draw()
{
    GraphicsDriver *pclDriver = GetParentWindow()->GetDriver();
    DrawRectangle_t stRect;
    DrawLine_t stLine;

    uint16_t u16X, u16Y;
    uint16_t u16ProgressWidth;

    GetControlOffset(&u16X, &u16Y);

    // Draw the outside of the progress bar region
    stLine.uColor = m_uBorderColor;
    stLine.u16X1 = u16X + GetLeft() + 1;
    stLine.u16X2 = u16X + GetLeft() + GetWidth() - 2;
    stLine.u16Y1 = u16Y + GetTop();
    stLine.u16Y2 = u16Y + GetTop();
    pclDriver->Line(&stLine);

    stLine.u16Y1 = u16Y + GetTop() + GetHeight() - 1;
    stLine.u16Y2 = u16Y + GetTop() + GetHeight() - 1;
    pclDriver->Line(&stLine);

    stLine.u16Y1 = u16Y + GetTop() + 1;
    stLine.u16Y2 = u16Y + GetTop() + GetHeight() - 2;
    stLine.u16X1 = u16X + GetLeft();
    stLine.u16X2 = u16X + GetLeft();
    pclDriver->Line(&stLine);

    stLine.u16X1 = u16X + GetLeft() + GetWidth() - 1;
    stLine.u16X2 = u16X + GetLeft() + GetWidth() - 1;
    pclDriver->Line(&stLine);

    // Draw the "completed" portion
    u16ProgressWidth = (uint16_t)( ( ( ( (uint32_t)m_u8Progress) * (GetWidth()-2) ) + 50 ) / 100);
    stRect.u16Top = u16Y + GetTop() + 1;
    stRect.u16Bottom = u16Y + GetTop() + GetHeight() - 2;
    stRect.u16Left = u16X + GetLeft() + 1;
    stRect.u16Right = stRect.u16Left + u16ProgressWidth - 1;
    stRect.bFill = true;
    stRect.u32ineColor = m_uProgressColor;
    stRect.uFillColor = m_uProgressColor;
    pclDriver->Rectangle(&stRect);

    // Draw the "incomplete" portion
    stRect.u16Left = stRect.u16Right + 1;
    stRect.u16Right = u16X + GetLeft() + GetWidth() - 2;
    stRect.bFill = true;
    stRect.u32ineColor = m_uBackColor;
    stRect.uFillColor = m_uBackColor;
    pclDriver->Rectangle(&stRect);

}

//---------------------------------------------------------------------------
void ProgressControl::SetProgress( uint8_t u8Progress_ )
{
    m_u8Progress = u8Progress_;
    if (m_u8Progress > 100)
    {
        m_u8Progress = 100;
    }
    SetStale();
}

//---------------------------------------------------------------------------
GuiReturn_t ProgressControl::ProcessEvent( GuiEvent_t *pstEvent_)
{
    return GUI_EVENT_OK;
}
