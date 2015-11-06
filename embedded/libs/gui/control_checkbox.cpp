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
    \file control_checkbox.cpp
    \brief Checkbox Control

    A binary On/Off switch control
*/

#include "gui.h"
#include "kerneltypes.h"
#include "draw.h"
#include "font.h"
#include "control_checkbox.h"
#include "gui_palette.h"

//---------------------------------------------------------------------------
#define TEXT_X_OFFSET        (13)

//---------------------------------------------------------------------------
static const uint8_t aucBox[] =
{ 0xE7,
  0x81,
  0x81,
  0x81,
  0x81,
  0x81,
  0x81,
  0xE7 };

//---------------------------------------------------------------------------
static const uint8_t aucCheck[] =
{ 0,
  0,
  0x3C,
  0x3C,
  0x3C,
  0x3C,
  0,
  0 };

//---------------------------------------------------------------------------
void CheckBoxControl::Init()
{
    SetAcceptFocus(true);
    m_uBackColor = PRIMARY_0;
    m_uBoxColor = PRIMARY_50;
    m_uFontColor = PRIMARY_100;
    m_pfCheckCallback = 0;
}

//---------------------------------------------------------------------------
void CheckBoxControl::Draw()
{
    GraphicsDriver *pclDriver = GetParentWindow()->GetDriver();
    uint16_t u16X, u16Y;
    uint16_t u16TextWidth;

    GetControlOffset(&u16X, &u16Y);

    // Draw the box, (and check, if necessary)
    {
        DrawRectangle_t stRect;

        if (GetParentWindow()->IsInFocus(this))
        {
            stRect.u32ineColor = m_uBoxColor;
        }
        else
        {
            stRect.u32ineColor = m_uBackColor;
        }

        stRect.uFillColor = m_uBackColor;
        stRect.u16Top = u16Y + GetTop();
        stRect.u16Left = u16X + GetLeft();
        stRect.u16Right = stRect.u16Left + GetWidth() - 1;
        stRect.u16Bottom = stRect.u16Top + GetHeight() - 1;
        stRect.bFill = true;
        pclDriver->Rectangle(&stRect);
    }

    {
        DrawStamp_t stStamp;
        stStamp.uColor = m_uBoxColor;
        stStamp.u16Y = u16Y + GetTop() + ((GetHeight() - 5) >> 1) - 1;
        stStamp.u16X = u16X + GetLeft() + 2;
        stStamp.u16Width = 8;
        stStamp.u16Height = 8;
        stStamp.pu8Data = (uint8_t*)aucBox;
        pclDriver->Stamp(&stStamp);

        if (m_bChecked)
        {
            stStamp.uColor = m_uBoxColor;
        }
        else
        {
            stStamp.uColor = m_uBackColor;
        }
        stStamp.pu8Data = (uint8_t*)aucCheck;
        pclDriver->Stamp(&stStamp);
    }

    // Draw the caption
    {
        DrawText_t stText;
        stText.u16Left = u16X + GetLeft() + TEXT_X_OFFSET;
        stText.u16Top = u16Y + GetTop();
        stText.uColor = m_uFontColor;
        stText.pstFont = m_pstFont;
        stText.pcString = m_szCaption;

        u16TextWidth = pclDriver->TextWidth(&stText);
        pclDriver->Text(&stText);
    }
}

//---------------------------------------------------------------------------
GuiReturn_t CheckBoxControl::ProcessEvent( GuiEvent_t *pstEvent_ )
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
                if (m_bChecked == true)
                {
                    m_bChecked = false;
                }
                else
                {
                    m_bChecked = true;
                }

                if (m_pfCheckCallback)
                {
                    m_pfCheckCallback(m_bChecked);
                }
                SetStale();
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
                    m_bChecked = true;
                }
                else
                {
                    m_bChecked = false;
                }

                if (m_pfCheckCallback)
                {
                    m_pfCheckCallback(m_bChecked);
                }
                SetStale();
            }
        }
            break;
        case EVENT_TYPE_MOUSE:
        {
            // Is this control currently in the "active"/pressed state?
            if (m_bChecked)
            {
                // Check to see if the movement is out-of-bounds based on the coordinates.
                // If so, de-activate the control
                if (pstEvent_->stMouse.bLeftState)
                {
                    if ((pstEvent_->stMouse.u16X >= GetLeft() + u16XOffset) &&
                        (pstEvent_->stMouse.u16X < GetLeft() + u16XOffset + GetWidth()-1) &&
                        (pstEvent_->stMouse.u16Y >= GetTop() + u16YOffset) &&
                        (pstEvent_->stMouse.u16Y < GetTop() + u16YOffset + GetHeight() - 1))
                    {
                        m_bChecked = false;
                        SetStale();
                        if (m_pfCheckCallback)
                        {
                            m_pfCheckCallback(m_bChecked);
                        }
                    }
                }
            }
            else if (!m_bChecked)
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
                        m_bChecked = true;
                        SetStale();
                        if (m_pfCheckCallback)
                        {
                            m_pfCheckCallback(m_bChecked);
                        }
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
