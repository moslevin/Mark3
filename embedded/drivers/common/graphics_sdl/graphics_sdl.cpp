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
    \file graphics_sdl.h
    \brief SDL Graphics driver implementation
*/

#include "kerneltypes.h"
#include "driver.h"
#include "graphics.h"
#include "graphics_sdl.h"
#include "font.h"

#include <stdint.h>
#include <SDL/SDL.h>
//---------------------------------------------------------------------------
void GraphicsSDL::Init(void)
{
    m_u16Res16X = 128;
    m_u16Res16Y = 128;
    m_u8BPP = 32;
    m_u16Left = 0;
    m_u16Right = 127;
    m_u16Top = 0;
    m_u16Bottom = 127;
}

//---------------------------------------------------------------------------
uint8_t GraphicsSDL::Open()
{
    SDL_Init( SDL_INIT_EVERYTHING );
    m_pstScreen = SDL_SetVideoMode( 128, 128, 32, SDL_SWSURFACE );
    return 0;
}

//---------------------------------------------------------------------------
uint8_t GraphicsSDL::Close()
{
    if (m_pstScreen)
    {
        SDL_FreeSurface(m_pstScreen);
    }
    SDL_Quit();
    return 0;
}

//---------------------------------------------------------------------------
void GraphicsSDL::DrawPixel(DrawPoint_t *pstPoint_)
{
    uint32_t *pixels = (uint32_t*)m_pstScreen->pixels;
    if (((pstPoint_->u16X < m_u16Res16X) && (pstPoint_->u16Y < m_u16Res16Y)) &&
        ((pstPoint_->u16X >= m_u16Left ) && (pstPoint_->u16Y >= m_u16Top)) &&
        ((pstPoint_->u16X <= m_u16Right) && (pstPoint_->u16Y <= m_u16Bottom)))
    {
        pixels[ ((uint32_t)(pstPoint_->u16Y) * m_pstScreen->w) + (uint32_t)(pstPoint_->u16X) ] = (uint32_t)pstPoint_->uColor;
    }
}

//---------------------------------------------------------------------------
void GraphicsSDL::ReadPixel(DrawPoint_t *pstPoint_)
{
    uint32_t *pixels = (uint32_t*)m_pstScreen->pixels;
    pstPoint_->uColor = (COLOR)pixels[ ((uint32_t)(pstPoint_->u16Y) * m_pstScreen->w) + (uint32_t)(pstPoint_->u16X) ];
}

//---------------------------------------------------------------------------
void GraphicsSDL::Flip()
{
    if (m_pstScreen)
    {
        SDL_Flip(m_pstScreen);
    }
}

