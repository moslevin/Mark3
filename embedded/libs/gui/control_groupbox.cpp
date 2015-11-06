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
    \file control_groupbox.cpp
    \brief GUI GroupBox Control Implementation
*/

#include "gui.h"
#include "kerneltypes.h"
#include "draw.h"
#include "graphics.h"
#include "control_groupbox.h"
#include "gui_palette.h"

#define BORDER_OFFSET           (2)
#define TEXT_X_OFFSET           (4)
#define TEXT_Y_OFFSET           (0)
#define HEADER_HEIGHT           (16)

//---------------------------------------------------------------------------
void GroupBoxControl::Init()
{
    m_uHeaderColor = SECONDARY_2_50;
    m_uFontColor = SECONDARY_2_100;
    m_uPanelColor = SECONDARY_2_0;

    SetAcceptFocus(false);
}

//---------------------------------------------------------------------------
void GroupBoxControl::Draw()
{
    GUI_DEBUG_PRINT( "GroupBoxControl::Draw()\n");
    GraphicsDriver *pclDriver = GetParentWindow()->GetDriver();
    uint16_t u16X, u16Y;
    uint16_t u16TextWidth;

    GetControlOffset(&u16X, &u16Y);

    // Draw the background panel, starting with the caption header
    {
        DrawRectangle_t stRectangle;
        stRectangle.bFill = true;
        stRectangle.u16Left = GetLeft() + u16X;
        stRectangle.u16Right = stRectangle.u16Left + GetWidth() -1;

        stRectangle.u16Top = GetTop() + u16Y;
        stRectangle.u16Bottom = stRectangle.u16Top + HEADER_HEIGHT - 1;

        stRectangle.u32ineColor = m_uHeaderColor;
        stRectangle.uFillColor = m_uHeaderColor;

        pclDriver->Rectangle(&stRectangle);

        stRectangle.u16Top = GetTop() + u16Y + HEADER_HEIGHT;
        stRectangle.u16Bottom = stRectangle.u16Top + GetHeight() - HEADER_HEIGHT;

        stRectangle.u32ineColor = m_uPanelColor;
        stRectangle.uFillColor = m_uPanelColor;

        pclDriver->Rectangle(&stRectangle);        
    }

    // Draw the caption
    {
        DrawText_t stText;
        stText.u16Left = GetLeft() + u16X + TEXT_X_OFFSET;
        stText.u16Top = GetTop() + u16Y + TEXT_Y_OFFSET;
        stText.uColor = m_uFontColor;
        stText.pstFont = m_pstFont;
        stText.pcString = m_pcCaption;

        u16TextWidth = pclDriver->TextWidth(&stText);
        pclDriver->Text(&stText);
    }

}
