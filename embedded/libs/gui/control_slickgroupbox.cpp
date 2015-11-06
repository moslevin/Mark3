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
#include "control_slickgroupbox.h"

#define TEXT_X_OFFSET           (8)
#define TEXT_Y_OFFSET           (0)
#define TEXT_BANNER_HEIGHT      (5)


//---------------------------------------------------------------------------
void SlickGroupBoxControl::Draw()
{
    GUI_DEBUG_PRINT( "GroupBoxControl::Draw()\n");
    GraphicsDriver *pclDriver = GetParentWindow()->GetDriver();
    uint16_t u16X, u16Y;
    uint16_t u16TextWidth;

    GetControlOffset(&u16X, &u16Y);

        // Draw the header rectangle
    {
        DrawRectangle_t stRect;
        stRect.u16Top = GetTop() + u16Y;
        stRect.u16Bottom = stRect.u16Top + GetHeight() - 1;
        stRect.u16Left = GetLeft() + u16X;
        stRect.u16Right = stRect.u16Left + GetWidth() - 1;
        stRect.bFill = true;
        stRect.uFillColor = m_uBGColor;
        stRect.u32ineColor = m_uBGColor;
        pclDriver->Rectangle(&stRect);
    }

    {
        DrawLine_t stLine;

        // Draw the rounded-off rectangle
        stLine.u16X1 = GetLeft() + u16X + 1;
        stLine.u16X2 = stLine.u16X1 + GetWidth() - 3;
        stLine.u16Y1 = GetTop() + u16Y;
        stLine.u16Y2 = stLine.u16Y1;
        stLine.uColor = COLOR_GREY50;
        pclDriver->Line(&stLine);

        stLine.u16Y1 = GetTop() + u16Y + GetHeight() - 1;
        stLine.u16Y2 = stLine.u16Y1;
        pclDriver->Line(&stLine);

        stLine.u16Y1 = GetTop() + u16Y + m_pstFont->u8Size + TEXT_BANNER_HEIGHT;
        stLine.u16Y2 = stLine.u16Y1;
        pclDriver->Line(&stLine);

        // Draw the rounded-off rectangle
        stLine.u16X1 = GetLeft() + u16X ;
        stLine.u16X2 = stLine.u16X1;

        stLine.u16Y1 = GetTop() + u16Y + 1;
        stLine.u16Y2 = stLine.u16Y1 + GetHeight() - 3;
        pclDriver->Line(&stLine);

        // Draw the rounded-off rectangle
        stLine.u16X1 = GetLeft() + u16X + GetWidth() - 1;
        stLine.u16X2 = stLine.u16X1;
        pclDriver->Line(&stLine);

    }

    // Draw the header rectangle
    {
        DrawRectangle_t stRect;
        stRect.u16Top = GetTop() + u16Y + 1;
        stRect.u16Bottom = GetTop() + u16Y + ((m_pstFont->u8Size + TEXT_BANNER_HEIGHT) >> 1);
        stRect.u16Left = GetLeft() + u16X + 1;
        stRect.u16Right = GetLeft() + GetWidth() - 2;
        stRect.bFill = true;
        stRect.uFillColor = RGB_COLOR((uint32_t)(MAX_RED * 0.20),(uint32_t)(MAX_RED * 0.20),(uint32_t)(MAX_BLUE * 0.75) );
        stRect.u32ineColor = stRect.uFillColor;
        pclDriver->Rectangle(&stRect);

        stRect.u16Top = GetTop() + u16Y + ((m_pstFont->u8Size + TEXT_BANNER_HEIGHT) >> 1);
        stRect.u16Bottom = GetTop() + u16Y + (m_pstFont->u8Size + TEXT_BANNER_HEIGHT) - 1;
        stRect.uFillColor = RGB_COLOR((uint32_t)(MAX_RED * 0.10),(uint32_t)(MAX_RED * 0.10),(uint32_t)(MAX_BLUE * 0.65) );
        stRect.u32ineColor = stRect.uFillColor;
        pclDriver->Rectangle(&stRect);

    }

    // Draw the caption
    {
        DrawText_t stText;
        stText.u16Left = u16X + GetLeft() + TEXT_X_OFFSET;
        stText.u16Top = u16Y + GetTop() + TEXT_Y_OFFSET;
        stText.uColor = COLOR_WHITE;
        stText.pstFont = m_pstFont;
        stText.pcString = m_pcCaption;

        u16TextWidth = pclDriver->TextWidth(&stText);
        pclDriver->Text(&stText);
    }


}
