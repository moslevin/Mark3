/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2013 - 2017 Funkenstein Software Consulting, all rights reserved.
See license.txt for more information
===========================================================================*/
/*!
    \file graphics_flavr.cpp

    \brief Graphics driver implementation for flAVR co-simulation
*/

#include "graphics_flavr.h"
#include "kerneltypes.h"
#include "thread.h"
#include "colorspace.h"
#include "draw.h"

//!! These need to be in the global namespace for flAVR to pick the names up.
volatile uint8_t      g_u8GfxCommand = 0;
volatile Mark3::DrawPoint_t* g_pclPoint     = 0;

namespace Mark3
{
//---------------------------------------------------------------------------
void GraphicsFlavr::DrawPixel(DrawPoint_t* pstPoint_)
{
    g_pclPoint     = pstPoint_;
    g_u8GfxCommand = 1;
}

//---------------------------------------------------------------------------
void GraphicsFlavr::Flip(void)
{
    g_u8GfxCommand = 2;
}
} //namespace Mark3
