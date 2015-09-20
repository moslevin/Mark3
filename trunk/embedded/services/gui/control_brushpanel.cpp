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
    \file control_brushpanel.cpp
    \brief GUI Panel Control Implementation with a brushed-metal finish.
*/

#include "gui.h"
#include "kerneltypes.h"
#include "draw.h"
#include "graphics.h"
#include "control_brushpanel.h"
#include "rand_lfsr.h"

//---------------------------------------------------------------------------
#define GREYSCALE_MIN               (32)
#define GREYSCALE_MAX               (96)

//---------------------------------------------------------------------------
void BrushPanelControl::Draw()
{
    uint16_t u16PixelVal256;
    uint8_t u8Red, u8Green, u8Blue;

     GraphicsDriver *pclDriver = GetParentWindow()->GetDriver();
    DrawLine_t stLine;

    stLine.u16Y1 = GetTop();
    stLine.u16Y2 = GetTop() + GetHeight() - 1;

    for (uint16_t i = GetLeft(); i < ((GetLeft() + GetWidth() + 1) >> 1); i++)
    {
        stLine.u16X1 = i;
        stLine.u16X2 = i;
        u16PixelVal256 = (i * 256) / ((GetLeft() + GetWidth() + 1) >> 1);
        u16PixelVal256 = GREYSCALE_MIN +
                        (u16PixelVal256 / ( 256 / (GREYSCALE_MAX - GREYSCALE_MIN)));

        u8Red = (uint8_t)(u16PixelVal256 / (256 / MAX_RED));
        u8Blue = (uint8_t)(u16PixelVal256 / (256 / MAX_BLUE));
        u8Green = (uint8_t)(u16PixelVal256 / (256 / MAX_GREEN));

        stLine.uColor = RGB_COLOR(u8Red, u8Blue, u8Green);

        pclDriver->Line(&stLine);
    }

    for (uint16_t i = ((GetLeft() + GetWidth())>> 1); i < GetLeft() + GetWidth(); i++)
    {
        stLine.u16X1 = i;
        stLine.u16X2 = i;
        u16PixelVal256 = ( ((GetLeft() + GetWidth()) - i) * 256) / ((GetLeft() + GetWidth() + 1) >> 1);
        u16PixelVal256 = GREYSCALE_MIN +
                        (u16PixelVal256 / ( 256 / (GREYSCALE_MAX - GREYSCALE_MIN)));

        u8Red = (uint8_t)(u16PixelVal256 / (256 / MAX_RED));
        u8Blue = (uint8_t)(u16PixelVal256 / (256 / MAX_BLUE));
        u8Green = (uint8_t)(u16PixelVal256 / (256 / MAX_GREEN));

        stLine.uColor = RGB_COLOR(u8Red, u8Blue, u8Green);

        pclDriver->Line(&stLine);
    }
}
