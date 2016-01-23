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
    \file control_panel.cpp
    \brief GUI Panel Control Implementation
*/

#include "gui.h"
#include "kerneltypes.h"
#include "draw.h"
#include "graphics.h"
#include "control_label.h"
#include "gui_palette.h"

//---------------------------------------------------------------------------
void LabelControl::Init()
{
    m_uBackColor = COLOR_BLACK;
    m_uFontColor = COLOR_WHITE;
    m_bTransparent = false;
    m_pstFont = NULL;
    m_pcCaption = "";
    SetAcceptFocus(false);
}

//---------------------------------------------------------------------------
void LabelControl::Draw()
{
    GUI_DEBUG_PRINT( "LabelControl::Draw()\n");
    GraphicsDriver *pclDriver = GetParentWindow()->GetDriver();

    DrawWindow_t stWindow;

    uint16_t u16X, u16Y;

    GetControlOffset(&u16X, &u16Y);

    DrawText_t stText;
    DrawRectangle_t stRect;
    uint16_t u16TextWidth;

    stText.u16Top = GetTop() + u16Y;
    stText.u16Left = GetLeft() + u16X;
    stText.uColor = m_uFontColor;
    stText.pcString = m_pcCaption;
    stText.pstFont = m_pstFont;
    u16TextWidth = pclDriver->TextWidth(&stText);

    if (false == m_bTransparent)
    {

        stRect.u16Left = GetLeft() + u16X;
        stRect.u16Right = stRect.u16Left + GetWidth() - 1;
        stRect.u16Top = GetTop() + u16Y;
        stRect.u16Bottom = stRect.u16Top + GetHeight() - 1;
        stRect.u32ineColor = m_uBackColor;
        stRect.uFillColor = m_uBackColor;
        stRect.bFill = true;
        pclDriver->Rectangle(&stRect);
    }

    stWindow.u16Left = stText.u16Left;
    stWindow.u16Right = stText.u16Left+ GetWidth() - 1;
    stWindow.u16Top = stText.u16Top;
    stWindow.u16Bottom = stText.u16Top + GetHeight() - 1;

    if (!stText.pstFont)
    {
        return;
    }

    pclDriver->SetWindow(&stWindow);
    pclDriver->Text(&stText);
    pclDriver->ClearWindow();
}
