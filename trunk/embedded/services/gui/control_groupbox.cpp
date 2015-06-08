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
    K_USHORT usX, usY;
    K_USHORT usTextWidth;

    GetControlOffset(&usX, &usY);

    // Draw the background panel, starting with the caption header
    {
        DrawRectangle_t stRectangle;
        stRectangle.bFill = true;
        stRectangle.usLeft = GetLeft() + usX;
        stRectangle.usRight = stRectangle.usLeft + GetWidth() -1;

        stRectangle.usTop = GetTop() + usY;
        stRectangle.usBottom = stRectangle.usTop + HEADER_HEIGHT - 1;

        stRectangle.uLineColor = m_uHeaderColor;
        stRectangle.uFillColor = m_uHeaderColor;

        pclDriver->Rectangle(&stRectangle);

        stRectangle.usTop = GetTop() + usY + HEADER_HEIGHT;
        stRectangle.usBottom = stRectangle.usTop + GetHeight() - HEADER_HEIGHT;

        stRectangle.uLineColor = m_uPanelColor;
        stRectangle.uFillColor = m_uPanelColor;

        pclDriver->Rectangle(&stRectangle);        
    }

    // Draw the caption
    {
        DrawText_t stText;
        stText.usLeft = GetLeft() + usX + TEXT_X_OFFSET;
        stText.usTop = GetTop() + usY + TEXT_Y_OFFSET;
        stText.uColor = m_uFontColor;
        stText.pstFont = m_pstFont;
        stText.pcString = m_pcCaption;

        usTextWidth = pclDriver->TextWidth(&stText);
        pclDriver->Text(&stText);
    }

}
