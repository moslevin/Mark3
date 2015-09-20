/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2013 Funkenstein Software Consulting, all rights reserved.
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

volatile uint8_t g_u8GfxCommand = 0;
volatile DrawPoint_t *g_pclPoint = 0;

//---------------------------------------------------------------------------
void GraphicsFlavr::DrawPixel(DrawPoint_t *pstPoint_)
{
    g_pclPoint = pstPoint_;
    g_u8GfxCommand = 1;
}

//---------------------------------------------------------------------------
void GraphicsFlavr::Flip(void)
{
    g_u8GfxCommand = 2;
}
