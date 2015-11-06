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
    \file control_notification.cpp
    \brief Notification pop-up control

    A pop-up control that can be used to present the user with information
    about system state changes, events, etc.
*/

#include "control_notification.h"
#include "kerneltypes.h"

//---------------------------------------------------------------------------
void NotificationControl::Draw()
{
    if (!m_bVisible)
    {
        return;
    }

    DrawRectangle_t stRect;
    DrawLine_t stLine;
    DrawText_t stText;

    GraphicsDriver *pclDriver = GetParentWindow()->GetDriver();

    uint16_t u16XOffset = 0;
    uint16_t u16HalfWidth = 0;
    uint16_t u16YOffset = 0;

    // Get the location of the control relative to elements higher in the heirarchy
    GetControlOffset(&u16XOffset, &u16YOffset);

    // Draw the rounded-off rectangle
    stLine.u16X1 = GetLeft() + u16XOffset + 1;
    stLine.u16X2 = stLine.u16X1 + GetWidth() - 3;
    stLine.u16Y1 = GetTop() + u16YOffset;
    stLine.u16Y2 = stLine.u16Y1;
    stLine.uColor = COLOR_WHITE;
    pclDriver->Line(&stLine);

    stLine.u16Y1 = GetTop() + u16YOffset + GetHeight() - 1;
    stLine.u16Y2 = stLine.u16Y1;
    pclDriver->Line(&stLine);

    // Draw the rounded-off rectangle
    stLine.u16X1 = GetLeft() + u16XOffset ;
    stLine.u16X2 = stLine.u16X1;

    stLine.u16Y1 = GetTop() + u16YOffset + 1;
    stLine.u16Y2 = stLine.u16Y1 + GetHeight() - 3;
    pclDriver->Line(&stLine);

    // Draw the rounded-off rectangle
    stLine.u16X1 = GetLeft() + u16XOffset + GetWidth() - 1;
    stLine.u16X2 = stLine.u16X1;
    pclDriver->Line(&stLine);

    stRect.u16Top = GetTop() + u16YOffset + 1;
    stRect.u16Bottom = stRect.u16Top + GetHeight() - 3;
    stRect.u16Left = GetLeft() + u16XOffset + 1;
    stRect.u16Right = stRect.u16Left + GetWidth() - 3;
    stRect.bFill = true;
    stRect.uFillColor = COLOR_BLACK;
    stRect.u32ineColor = COLOR_BLACK;
    pclDriver->Rectangle(&stRect);

    // Draw the Text
    stText.pstFont = m_pstFont;
    stText.pcString = m_szCaption;
    stText.uColor = COLOR_WHITE;
    u16HalfWidth = pclDriver->TextWidth(&stText);
    u16HalfWidth >>= 1;
    stText.u16Left = GetLeft() + (GetWidth()>>1) - u16HalfWidth + u16XOffset;
    stText.u16Top = GetTop() + u16YOffset;
    pclDriver->Text(&stText);
}

//---------------------------------------------------------------------------
GuiReturn_t NotificationControl::ProcessEvent( GuiEvent_t *pstEvent_ )
{

    switch (pstEvent_->u8EventType)
    {
        case EVENT_TYPE_TIMER:
        {
            if (m_bTrigger && m_u16Timeout)
            {
                m_u16Timeout--;

                if (!m_u16Timeout)
                {
                    m_bVisible = false;
                    m_bTrigger = false;
                    SetStale();

                    uint16_t u16X, u16Y;
                    GetControlOffset(&u16X, &u16Y);

                    GetParentWindow()->InvalidateRegion( GetLeft() + u16X, GetTop() + u16Y, GetWidth(), GetHeight());
                }
            }

            break;
        }
        default:
            break;
    }
}
