/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]------------------------------------------------

Copyright (c) 2012-2015 Funkenstein Software Consulting, all rights reserved.
See license.txt for more information
===========================================================================*/
/*!
    \file gui_palette.h
    \brief Default colors used by the control set.
*/

#ifndef __GUI_PALETTE_H__
#define __GUI_PALETTE_H__

#include "colorspace.h"

//---------------------------------------------------------------------------
#define WEB_COLOR_TO_RGB_COLOR(val) \
    RGB_COLOR( ((val >> 16) & 0xFF) / (((255 + (MAX_RED >> 1)) / MAX_RED) ), ((val >> 8) & 0xFF) / (((255 + (MAX_GREEN >> 1)) / MAX_GREEN) ), (val & 0xFF) / (((255 + (MAX_BLUE >> 1)) / MAX_BLUE)) )

//---------------------------------------------------------------------------
#define PRIMARY_100     WEB_COLOR_TO_RGB_COLOR(0xFFFEE5)
#define PRIMARY_75      WEB_COLOR_TO_RGB_COLOR(0xF6F476)
#define PRIMARY_50      WEB_COLOR_TO_RGB_COLOR(0xBBB927)
#define PRIMARY_25      WEB_COLOR_TO_RGB_COLOR(0x757200)
#define PRIMARY_0       WEB_COLOR_TO_RGB_COLOR(0x0A0A00)

//---------------------------------------------------------------------------
#define SECONDARY_1_100 WEB_COLOR_TO_RGB_COLOR(0xF5FDE3)
#define SECONDARY_1_75  WEB_COLOR_TO_RGB_COLOR(0xC3E86F)
#define SECONDARY_1_50  WEB_COLOR_TO_RGB_COLOR(0x86B025)
#define SECONDARY_1_25  WEB_COLOR_TO_RGB_COLOR(0x4D6E00)
#define SECONDARY_1_0   WEB_COLOR_TO_RGB_COLOR(0x070A00)

//---------------------------------------------------------------------------
#define SECONDARY_2_100 WEB_COLOR_TO_RGB_COLOR(0xF6DDED)
#define SECONDARY_2_75  WEB_COLOR_TO_RGB_COLOR(0xC05C99)
#define SECONDARY_2_50  WEB_COLOR_TO_RGB_COLOR(0x921667)
#define SECONDARY_2_25  WEB_COLOR_TO_RGB_COLOR(0x5B0039)
#define SECONDARY_2_0   WEB_COLOR_TO_RGB_COLOR(0x080005)

//---------------------------------------------------------------------------
#define COMPLEMENT_100  WEB_COLOR_TO_RGB_COLOR(0xE8DAF2)
#define COMPLEMENT_75   WEB_COLOR_TO_RGB_COLOR(0x8454A5)
#define COMPLEMENT_50   WEB_COLOR_TO_RGB_COLOR(0x58207E)
#define COMPLEMENT_25   WEB_COLOR_TO_RGB_COLOR(0x30044E)
#define COMPLEMENT_0    WEB_COLOR_TO_RGB_COLOR(0x040007)

//---------------------------------------------------------------------------


#endif
