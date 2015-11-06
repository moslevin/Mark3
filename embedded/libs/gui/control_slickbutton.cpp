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

#include "control_slickbutton.h"
#include "gui.h"
#include "kernelaware.h"
#include <stdio.h>
#include <stdlib.h>


/* Control looks somthing like this:

   ----------------------------
  |         tone 1             |
  |____________________________|
  |         tone 2             |
  |                            |
   ----------------------------

   Button with rounded corners,
   two-tone.
*/

void SlickButtonControl::Init()
{
    m_szCaption = "Button";
    m_pstFont = NULL;
    m_bState = false;
    m_pfCallback = NULL;
    m_pvCallbackData = NULL;
    m_u8Timeout = 0;
    SetAcceptFocus(true);
}
//---------------------------------------------------------------------------
void SlickButtonControl::Draw()
{
    DrawText_t stText;
    DrawLine_t stLine;
    DrawRectangle_t stRect;

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
    stLine.uColor = COLOR_GREY50;
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

    // Draw the two-tone button

    stRect.u16Top = GetTop() + u16YOffset + 1;
    stRect.u16Bottom = stRect.u16Top + ((GetHeight() - 1) / 2) - 1;
    stRect.u16Left = GetLeft() + u16XOffset + 1;
    stRect.u16Right = stRect.u16Left + GetWidth() - 3;
    stRect.bFill = true;

    if (m_u8Timeout)
    {
        if (m_u8Timeout & 1)
        {
            stRect.u32ineColor = COLOR_WHITE;
        }
        else
        {
            stRect.u32ineColor = COLOR_BLACK;
        }
    }
    else
    {
        if (IsInFocus())
        {
            stRect.u32ineColor = RGB_COLOR((uint32_t)(MAX_RED * 0.10),(uint32_t)(MAX_RED * 0.10),(uint32_t)(MAX_BLUE * 0.65) );
        }
        else
        {
            stRect.u32ineColor = RGB_COLOR( (uint32_t)(MAX_RED * 0.10),(uint32_t)(MAX_GREEN * 0.10),(uint32_t)(MAX_BLUE * 0.10));;
        }
    }
    stRect.uFillColor = stRect.u32ineColor;
    pclDriver->Rectangle(&stRect);

    stRect.u16Top = stRect.u16Bottom + 1;
    stRect.u16Bottom = stRect.u16Top + ((GetHeight() - 1) / 2 ) - 1;

    if (m_u8Timeout)
    {
        if (m_u8Timeout & 1)
        {
            stRect.u32ineColor = COLOR_WHITE;
        }
        else
        {
            stRect.u32ineColor = COLOR_BLACK;
        }
    }
    else
    {
        if (IsInFocus())
        {
            stRect.u32ineColor = RGB_COLOR((uint32_t)(MAX_RED * 0.20),(uint32_t)(MAX_RED * 0.20),(uint32_t)(MAX_BLUE * 0.75) );
        }
        else
        {
            stRect.u32ineColor = RGB_COLOR( (uint32_t)(MAX_RED * 0.20),(uint32_t)(MAX_GREEN * 0.20),(uint32_t)(MAX_BLUE * 0.20));
        }
    }

    stRect.uFillColor = stRect.u32ineColor;
    pclDriver->Rectangle(&stRect);
    // Draw the Text
    stText.pstFont = m_pstFont;
    stText.pcString = m_szCaption;
    stText.uColor = COLOR_GREY75;
    u16HalfWidth = pclDriver->TextWidth(&stText);
    u16HalfWidth >>= 1;
    stText.u16Left = GetLeft() + (GetWidth()>>1) - u16HalfWidth + u16XOffset;
    stText.u16Top = GetTop() + u16YOffset;
    pclDriver->Text(&stText);
}

//---------------------------------------------------------------------------
GuiReturn_t SlickButtonControl::ProcessEvent( GuiEvent_t *pstEvent_ )
{
    uint16_t u16XOffset, u16YOffset;

    GetControlOffset(&u16XOffset, &u16YOffset);

    GUI_DEBUG_PRINT("ButtonControl::ProcessEvent\n");

    switch (pstEvent_->u8EventType)
    {
        case EVENT_TYPE_TIMER:
        {
            if (m_u8Timeout)
            {
                SetStale();
                m_u8Timeout--;
                if (!m_u8Timeout)
                {
                    if (m_pfCallback)
                    {
                        m_pfCallback(m_pvCallbackData);
                    }
                }
            }

            break;
        }
        case EVENT_TYPE_JOYSTICK:
        {
            if (!m_u8Timeout)
            {
                // If this is a space bar or an enter key, behave like a mouse click.
                if( pstEvent_->stJoystick.Current.bButton1 && (!pstEvent_->stJoystick.Previous.bButton1))
                {

                    m_bState = false;
                    m_u8Timeout = 10;
                    // SetAcceptFocus(false);
                    //GetParentWindow()->SetFocus(NULL);
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
                    m_u8Timeout = 10;
                    //SetAcceptFocus(false);
                    //GetParentWindow()->SetFocus(NULL);
                    SetStale();

                }
                SetStale();
            }
        }
            break;
        case EVENT_TYPE_MOUSE:
        {
            if (m_u8Timeout)
            {
                break;
            }
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
                        m_u8Timeout = 10;
                        SetAcceptFocus(false);
                        GetParentWindow()->SetFocus(NULL);
                        SetStale();

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

}

//---------------------------------------------------------------------------
void SlickButtonControl::Activate( bool bActivate_ )
{
    // When we de-activate the control, simply disarm the control and force
    // a redraw
    if (!bActivate_)
    {
        m_bState = false;
    }
    SetStale();
}
