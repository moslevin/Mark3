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
    \file control_sevenseg.cpp
    \brief 7-segment display control

    4-digit seven-segment display control.  used for clocks, stopwatches, etc.
*/

#include "gui.h"
#include "kerneltypes.h"
#include "draw.h"
#include "font.h"
#include "control_sevenseg.h"
#include "kernelaware.h"
#include "gui_palette.h"
//---------------------------------------------------------------------------
/*

    This control implements a seven-segment display using raster primatives
    for the individual segments.

    Segments are mapped clockwise as follows, with segment "7" being in the
    middle.

    [--1---]
    6      2
    [--7---]
    5      3
    [--4---]

    There are 4 digits presented in this control, along with a ":" character

 */

#define SET_SEGMENT_COLOR(data, bit)         ( (((data) & (bit)) == (bit)) ? SECONDARY_1_75 : SECONDARY_1_25 )

#define LCD_SEG_1       (1 << 0)
#define LCD_SEG_2       (1 << 1)
#define LCD_SEG_3       (1 << 2)
#define LCD_SEG_4       (1 << 3)
#define LCD_SEG_5       (1 << 4)
#define LCD_SEG_6       (1 << 5)
#define LCD_SEG_7       (1 << 6)

const static uint8_t aucLCDMap[10] =
{
    /* 0 */ (LCD_SEG_1 | LCD_SEG_2 | LCD_SEG_3 | LCD_SEG_4 | LCD_SEG_5 | LCD_SEG_6) ,
    /* 1 */ (LCD_SEG_2 | LCD_SEG_3),
    /* 2 */ (LCD_SEG_1 | LCD_SEG_2 | LCD_SEG_4 | LCD_SEG_5 | LCD_SEG_7 ),
    /* 3 */ (LCD_SEG_1 | LCD_SEG_2 | LCD_SEG_3 | LCD_SEG_4 | LCD_SEG_7 ),
    /* 4 */ (LCD_SEG_2 | LCD_SEG_3 | LCD_SEG_6 | LCD_SEG_7 ),
    /* 5 */ (LCD_SEG_1 | LCD_SEG_3 | LCD_SEG_4 | LCD_SEG_6 | LCD_SEG_7 ),
    /* 6 */ (LCD_SEG_1 | LCD_SEG_3 | LCD_SEG_4 | LCD_SEG_5 | LCD_SEG_6 | LCD_SEG_7 ),
    /* 7 */ (LCD_SEG_1 | LCD_SEG_2 | LCD_SEG_3 ),
    /* 8 */ (LCD_SEG_1 | LCD_SEG_2 | LCD_SEG_3 | LCD_SEG_4 | LCD_SEG_5 | LCD_SEG_6 | LCD_SEG_7 ),
    /* 9 */ (LCD_SEG_1 | LCD_SEG_2 | LCD_SEG_3 | LCD_SEG_6 | LCD_SEG_7 )
};

//---------------------------------------------------------------------------
 void SevenSegControl::Init()
{
    SetAcceptFocus(false);
}

//---------------------------------------------------------------------------
void SevenSegControl::Draw()
{
    DrawRectangle_t stRect;
    GraphicsDriver *pclGfx = GetParentWindow()->GetDriver();

    uint16_t u16Space = ((5 * GetWidth()) + 50) / 100;
    uint16_t u16Width = (GetWidth() >> 2) - u16Space - (u16Space >> 2);

    uint16_t u16ControlOffsetX;
    uint16_t u16ControlOffsetY;
    GetControlOffset(&u16ControlOffsetX, &u16ControlOffsetY);

    stRect.bFill = true;
    stRect.uFillColor = SET_SEGMENT_COLOR(m_bColon, LCD_SEG_1);
    stRect.u32ineColor = stRect.uFillColor;

    // Draw Colon
    stRect.u16Left = GetLeft() + u16ControlOffsetX + (2 * ( u16Width + u16Space ));
    stRect.u16Right = stRect.u16Left + u16Space - 1;
    stRect.u16Top = GetTop() + u16ControlOffsetY + (GetHeight() >> 3);
    stRect.u16Bottom = GetTop() + u16ControlOffsetY + ((GetHeight() *3 ) >> 3);

    pclGfx->Rectangle(&stRect);

    stRect.u16Top = GetTop() + ((GetHeight() * 5) >> 3);
    stRect.u16Bottom = GetTop() + ((GetHeight() * 7 ) >> 3);

    pclGfx->Rectangle(&stRect);

    uint16_t u16TmpValue = m_u16Value;

    for (uint8_t i = 4; i > 0; i-- )
    {
        uint16_t u16OffX = GetLeft() + ((u16Width + u16Space) * (i-1));
        uint8_t u8SegValue = aucLCDMap[ u16TmpValue % 10 ];

        if (i >= 3)
        {
            u16OffX += 2 * ((5 * GetWidth()) + 50) / 100;
        }

        u16OffX += u16ControlOffsetX;

        // Segment 1 -- Top
        stRect.u16Left = u16OffX + (u16Width >> 3) + 1;
        stRect.u16Right = u16OffX + ((7 * u16Width) >> 3) - 1;
        stRect.u16Top = GetTop() + u16ControlOffsetY;
        stRect.u16Bottom = GetTop() + u16ControlOffsetY + (GetHeight() >> 3);

        stRect.uFillColor = SET_SEGMENT_COLOR( u8SegValue, LCD_SEG_1 );
        stRect.u32ineColor = stRect.uFillColor;

        pclGfx->Rectangle(&stRect);

        // Segment 4 -- Bottom
        stRect.u16Top = GetTop() + u16ControlOffsetY + ((7 * GetHeight()) >> 3);
        stRect.u16Bottom = GetTop() + u16ControlOffsetY + (GetHeight() -1);

        stRect.uFillColor = SET_SEGMENT_COLOR( u8SegValue, LCD_SEG_4 );
        stRect.u32ineColor = stRect.uFillColor;

        pclGfx->Rectangle(&stRect);

        // Segment 6 -- Upper Left Vertical
        stRect.u16Top = GetTop() + u16ControlOffsetY;
        stRect.u16Bottom = GetTop() + u16ControlOffsetY + (GetHeight() >> 1);
        stRect.u16Left = u16OffX;
        stRect.u16Right = u16OffX + (u16Width >> 3);

        stRect.uFillColor = SET_SEGMENT_COLOR( u8SegValue, LCD_SEG_6 );
        stRect.u32ineColor = stRect.uFillColor;

        pclGfx->Rectangle(&stRect);

        // Segment 5 -- Lower Left Vertical
        stRect.u16Top = GetTop() + u16ControlOffsetY + (GetHeight() >> 1);
        stRect.u16Bottom = GetTop() + u16ControlOffsetY + (GetHeight() -1 );

        stRect.uFillColor = SET_SEGMENT_COLOR( u8SegValue, LCD_SEG_5 );
        stRect.u32ineColor = stRect.uFillColor;

        pclGfx->Rectangle(&stRect);

        // Segment 2 -- Upper Right Vertical
        stRect.u16Top = GetTop() + u16ControlOffsetY;
        stRect.u16Bottom = GetTop() + u16ControlOffsetY + (GetHeight() >> 1);
        stRect.u16Left = u16OffX  + ((u16Width * 7) >> 3);
        stRect.u16Right = u16OffX + u16Width-1;

        stRect.uFillColor = SET_SEGMENT_COLOR( u8SegValue, LCD_SEG_2 );
        stRect.u32ineColor = stRect.uFillColor;

        pclGfx->Rectangle(&stRect);

        // Segment 3 -- Lower Right Vertical
        stRect.u16Top = GetTop() + u16ControlOffsetY + (GetHeight() >> 1);
        stRect.u16Bottom = GetTop() + u16ControlOffsetY + (GetHeight() -1 );

        stRect.uFillColor = SET_SEGMENT_COLOR( u8SegValue, LCD_SEG_3 );
        stRect.u32ineColor = stRect.uFillColor;

        pclGfx->Rectangle(&stRect);

        // Segment 7 -- Middle
        stRect.u16Top = GetTop() + u16ControlOffsetY + ((GetHeight() * 7) >> 4);
        stRect.u16Bottom = GetTop() + u16ControlOffsetY + ((GetHeight() * 9) >> 4);
        stRect.u16Left = u16OffX + (u16Width >> 3) + 1;
        stRect.u16Right = u16OffX + ((7 * u16Width) >> 3) - 1;

        stRect.uFillColor = SET_SEGMENT_COLOR( u8SegValue, LCD_SEG_7 );
        stRect.u32ineColor = stRect.uFillColor;

        pclGfx->Rectangle(&stRect);

        u16TmpValue /= 10;
    }
}

//---------------------------------------------------------------------------
GuiReturn_t SevenSegControl::ProcessEvent( GuiEvent_t *pstEvent_ )
{
    return GUI_EVENT_OK;
}
