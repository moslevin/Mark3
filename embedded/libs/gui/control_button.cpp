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
    \file control_button.cpp
    \brief GUI Button Control Implementation

    Basic pushbutton control with an up/down state.
*/

#include "control_button.h"
#include "gui.h"
#include "gui_palette.h"

void ButtonControl::Init()
{
    m_szCaption = "Button";
    m_pstFont = NULL;
    m_uBGColor = PRIMARY_25;
    m_uActiveColor = PRIMARY_50;
    m_u32ineColor = PRIMARY_75;
    m_uFillColor = PRIMARY_50;
    m_uTextColor = PRIMARY_100;
    m_bState = false;
    m_pfCallback = NULL;
    m_pvCallbackData = NULL;
    SetAcceptFocus(true);
}
//---------------------------------------------------------------------------
void ButtonControl::Draw()
{
    DrawText_t stText;
    DrawLine_t stLine;

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
    stLine.uColor = m_u32ineColor;
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

    // Draw a rectangle before the text if the BG is specified.
    {
        DrawRectangle_t stRect;
        stRect.u16Left = GetLeft() + u16XOffset + 1;
        stRect.u16Right = GetLeft() + GetWidth() + u16XOffset - 2;
        stRect.u16Top = GetTop() + u16YOffset + 1;
        stRect.u16Bottom = GetTop() + GetHeight() + u16YOffset - 2;
        stRect.bFill = true;

        if (m_bState)
        {
            stRect.uFillColor = m_uActiveColor;
        }
        else
        {
            stRect.uFillColor = m_uBGColor;
        }

        if (GetParentWindow()->IsInFocus(this))
        {
            stRect.u32ineColor = m_uFillColor;
        }
        else
        {
            stRect.u32ineColor = m_u32ineColor;
        }

        pclDriver->Rectangle(&stRect);
    }

    // Draw the Text
    stText.pstFont = m_pstFont;
    stText.pcString = m_szCaption;
    stText.uColor = m_uTextColor;
    u16HalfWidth = pclDriver->TextWidth(&stText);
    u16HalfWidth >>= 1;
    stText.u16Left = GetLeft() + (GetWidth()>>1) - u16HalfWidth + u16XOffset;
    stText.u16Top = GetTop() + u16YOffset;
    pclDriver->Text(&stText);
}

//---------------------------------------------------------------------------
GuiReturn_t ButtonControl::ProcessEvent( GuiEvent_t *pstEvent_ )
{
    uint16_t u16XOffset, u16YOffset;

    GetControlOffset(&u16XOffset, &u16YOffset);

    GUI_DEBUG_PRINT("ButtonControl::ProcessEvent\n");

    switch (pstEvent_->u8EventType)
    {
        case EVENT_TYPE_JOYSTICK:
        {
            // If this is a space bar or an enter key, behave like a mouse click.
            if( pstEvent_->stJoystick.Current.bButton1 && (!pstEvent_->stJoystick.Previous.bButton1))
            {
                m_bState = true;
                if (m_pfCallback)
                {
                    m_pfCallback(m_pvCallbackData);
                }

                SetStale();
            }
            else
            {
                if (m_bState == true)
                {
                    m_bState = false;
                    SetStale();
                }
            }
        }
        break;
        case EVENT_TYPE_KEYBOARD:
        {
            // If this is a space bar or an enter key, behave like a mouse click.
            if ((KEYCODE_SPACE == pstEvent_->stKey.u8KeyCode) ||
                (KEYCODE_RETURN == pstEvent_->stKey.u8KeyCode))
            {
                if (pstEvent_->stKey.bKeyState)
                {
                    m_bState = true;
                }
                else
                {
                    m_bState = false;
                    if (m_pfCallback)
                    {
                        m_pfCallback(m_pvCallbackData);
                    }
                }
                SetStale();
            }
        }
            break;
        case EVENT_TYPE_MOUSE:
        {
            // Is this control currently in the "active"/pressed state?
            if (m_bState)
            {
                // Check to see if the movement is out-of-bounds based on the coordinates.
                // If so, de-activate the control
                if (pstEvent_->stMouse.bLeftState)
                {
                    if ((pstEvent_->stMouse.u16X < GetLeft() + u16XOffset) ||
                        (pstEvent_->stMouse.u16X >= GetLeft() + u16XOffset + GetWidth()-1) ||
                        (pstEvent_->stMouse.u16Y < GetTop() + u16YOffset) ||
                        (pstEvent_->stMouse.u16Y >= GetTop() + u16YOffset + GetHeight() - 1))
                    {
                        m_bState = false;
                        SetStale();
                    }
                }
                // left button state is now up, and the control was previously active.
                // Run the event callback for the mouse, and go from there.
                else
                {
                    if ((pstEvent_->stMouse.u16X >= GetLeft() + u16XOffset) &&
                        (pstEvent_->stMouse.u16X < GetLeft() + u16XOffset + GetWidth()-1) &&
                        (pstEvent_->stMouse.u16Y >= GetTop() + u16YOffset) &&
                        (pstEvent_->stMouse.u16Y < GetTop() + u16YOffset + GetHeight() - 1))
                    {
                        m_bState = false;
                        SetStale();
                        if (m_pfCallback)
                        {
                            m_pfCallback(m_pvCallbackData);
                        }
                    }
                }
            }
            else if (!m_bState)
            {
                // If we registered a down-click in the bounding box, set the state of the
                // control to activated.
                if (pstEvent_->stMouse.bLeftState)
                {
                    if ((pstEvent_->stMouse.u16X >= GetLeft() + u16XOffset) &&
                        (pstEvent_->stMouse.u16X < GetLeft() + u16XOffset + GetWidth()-1) &&
                        (pstEvent_->stMouse.u16Y >= GetTop() + u16YOffset) &&
                        (pstEvent_->stMouse.u16Y < GetTop() + u16YOffset + GetHeight() - 1))
                    {
                        m_bState = true;
                        SetStale();
                    }
                }
            }

            if (!IsInFocus())
            {
                GetParentWindow()->SetFocus(this);
                SetStale();
            }

        }
            break;
    }
    return GUI_EVENT_OK;
}

//---------------------------------------------------------------------------
void ButtonControl::Activate( bool bActivate_ )
{
    // When we de-activate the control, simply disarm the control and force
    // a redraw
    if (!bActivate_)
    {
        m_bState = false;
    } 
    SetStale();
}
