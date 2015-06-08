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

    4-digit seven-segment display control.  Used for clocks, stopwatches, etc.
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

const static K_UCHAR aucLCDMap[10] =
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

    K_USHORT usSpace = ((5 * GetWidth()) + 50) / 100;
    K_USHORT usWidth = (GetWidth() >> 2) - usSpace - (usSpace >> 2);

    K_USHORT usControlOffsetX;
    K_USHORT usControlOffsetY;
    GetControlOffset(&usControlOffsetX, &usControlOffsetY);

    stRect.bFill = true;
    stRect.uFillColor = SET_SEGMENT_COLOR(m_bColon, LCD_SEG_1);
    stRect.uLineColor = stRect.uFillColor;

    // Draw Colon
    stRect.usLeft = GetLeft() + usControlOffsetX + (2 * ( usWidth + usSpace ));
    stRect.usRight = stRect.usLeft + usSpace - 1;
    stRect.usTop = GetTop() + usControlOffsetY + (GetHeight() >> 3);
    stRect.usBottom = GetTop() + usControlOffsetY + ((GetHeight() *3 ) >> 3);

    pclGfx->Rectangle(&stRect);

    stRect.usTop = GetTop() + ((GetHeight() * 5) >> 3);
    stRect.usBottom = GetTop() + ((GetHeight() * 7 ) >> 3);

    pclGfx->Rectangle(&stRect);

    K_USHORT usTmpValue = m_usValue;

    for (K_UCHAR i = 4; i > 0; i-- )
    {
        K_USHORT usOffX = GetLeft() + ((usWidth + usSpace) * (i-1));
        K_UCHAR ucSegValue = aucLCDMap[ usTmpValue % 10 ];

        if (i >= 3)
        {
            usOffX += 2 * ((5 * GetWidth()) + 50) / 100;
        }

        usOffX += usControlOffsetX;

        // Segment 1 -- Top
        stRect.usLeft = usOffX + (usWidth >> 3) + 1;
        stRect.usRight = usOffX + ((7 * usWidth) >> 3) - 1;
        stRect.usTop = GetTop() + usControlOffsetY;
        stRect.usBottom = GetTop() + usControlOffsetY + (GetHeight() >> 3);

        stRect.uFillColor = SET_SEGMENT_COLOR( ucSegValue, LCD_SEG_1 );
        stRect.uLineColor = stRect.uFillColor;

        pclGfx->Rectangle(&stRect);

        // Segment 4 -- Bottom
        stRect.usTop = GetTop() + usControlOffsetY + ((7 * GetHeight()) >> 3);
        stRect.usBottom = GetTop() + usControlOffsetY + (GetHeight() -1);

        stRect.uFillColor = SET_SEGMENT_COLOR( ucSegValue, LCD_SEG_4 );
        stRect.uLineColor = stRect.uFillColor;

        pclGfx->Rectangle(&stRect);

        // Segment 6 -- Upper Left Vertical
        stRect.usTop = GetTop() + usControlOffsetY;
        stRect.usBottom = GetTop() + usControlOffsetY + (GetHeight() >> 1);
        stRect.usLeft = usOffX;
        stRect.usRight = usOffX + (usWidth >> 3);

        stRect.uFillColor = SET_SEGMENT_COLOR( ucSegValue, LCD_SEG_6 );
        stRect.uLineColor = stRect.uFillColor;

        pclGfx->Rectangle(&stRect);

        // Segment 5 -- Lower Left Vertical
        stRect.usTop = GetTop() + usControlOffsetY + (GetHeight() >> 1);
        stRect.usBottom = GetTop() + usControlOffsetY + (GetHeight() -1 );

        stRect.uFillColor = SET_SEGMENT_COLOR( ucSegValue, LCD_SEG_5 );
        stRect.uLineColor = stRect.uFillColor;

        pclGfx->Rectangle(&stRect);

        // Segment 2 -- Upper Right Vertical
        stRect.usTop = GetTop() + usControlOffsetY;
        stRect.usBottom = GetTop() + usControlOffsetY + (GetHeight() >> 1);
        stRect.usLeft = usOffX  + ((usWidth * 7) >> 3);
        stRect.usRight = usOffX + usWidth-1;

        stRect.uFillColor = SET_SEGMENT_COLOR( ucSegValue, LCD_SEG_2 );
        stRect.uLineColor = stRect.uFillColor;

        pclGfx->Rectangle(&stRect);

        // Segment 3 -- Lower Right Vertical
        stRect.usTop = GetTop() + usControlOffsetY + (GetHeight() >> 1);
        stRect.usBottom = GetTop() + usControlOffsetY + (GetHeight() -1 );

        stRect.uFillColor = SET_SEGMENT_COLOR( ucSegValue, LCD_SEG_3 );
        stRect.uLineColor = stRect.uFillColor;

        pclGfx->Rectangle(&stRect);

        // Segment 7 -- Middle
        stRect.usTop = GetTop() + usControlOffsetY + ((GetHeight() * 7) >> 4);
        stRect.usBottom = GetTop() + usControlOffsetY + ((GetHeight() * 9) >> 4);
        stRect.usLeft = usOffX + (usWidth >> 3) + 1;
        stRect.usRight = usOffX + ((7 * usWidth) >> 3) - 1;

        stRect.uFillColor = SET_SEGMENT_COLOR( ucSegValue, LCD_SEG_7 );
        stRect.uLineColor = stRect.uFillColor;

        pclGfx->Rectangle(&stRect);

        usTmpValue /= 10;
    }
}

//---------------------------------------------------------------------------
GuiReturn_t SevenSegControl::ProcessEvent( GuiEvent_t *pstEvent_ )
{
    return GUI_EVENT_OK;
}
