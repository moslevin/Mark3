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
    K_USHORT usPixelVal256;
    K_UCHAR ucRed, ucGreen, ucBlue;

     GraphicsDriver *pclDriver = GetParentWindow()->GetDriver();
    DrawLine_t stLine;

    stLine.usY1 = GetTop();
    stLine.usY2 = GetTop() + GetHeight() - 1;

    for (K_USHORT i = GetLeft(); i < ((GetLeft() + GetWidth() + 1) >> 1); i++)
    {
        stLine.usX1 = i;
        stLine.usX2 = i;
        usPixelVal256 = (i * 256) / ((GetLeft() + GetWidth() + 1) >> 1);
        usPixelVal256 = GREYSCALE_MIN +
                        (usPixelVal256 / ( 256 / (GREYSCALE_MAX - GREYSCALE_MIN)));

        ucRed = (K_UCHAR)(usPixelVal256 / (256 / MAX_RED));
        ucBlue = (K_UCHAR)(usPixelVal256 / (256 / MAX_BLUE));
        ucGreen = (K_UCHAR)(usPixelVal256 / (256 / MAX_GREEN));

        stLine.uColor = RGB_COLOR(ucRed, ucBlue, ucGreen);

        pclDriver->Line(&stLine);
    }

    for (K_USHORT i = ((GetLeft() + GetWidth())>> 1); i < GetLeft() + GetWidth(); i++)
    {
        stLine.usX1 = i;
        stLine.usX2 = i;
        usPixelVal256 = ( ((GetLeft() + GetWidth()) - i) * 256) / ((GetLeft() + GetWidth() + 1) >> 1);
        usPixelVal256 = GREYSCALE_MIN +
                        (usPixelVal256 / ( 256 / (GREYSCALE_MAX - GREYSCALE_MIN)));

        ucRed = (K_UCHAR)(usPixelVal256 / (256 / MAX_RED));
        ucBlue = (K_UCHAR)(usPixelVal256 / (256 / MAX_BLUE));
        ucGreen = (K_UCHAR)(usPixelVal256 / (256 / MAX_GREEN));

        stLine.uColor = RGB_COLOR(ucRed, ucBlue, ucGreen);

        pclDriver->Line(&stLine);
    }
}
