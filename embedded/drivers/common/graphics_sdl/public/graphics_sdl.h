/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012-2016 Funkenstein Software Consulting, all rights reserved.
See license.txt for more information
===========================================================================*/
/*!
    \file graphics_sdl.h
    
    \brief Graphics driver implementation using libSDL
    
    Graphics driver implementation sitting on top of the cross-platform
    SDL graphics library.
*/

#include "driver.h"
#include "draw.h"
#include "graphics.h"
#include <SDL/SDL.h>

#ifndef __GRAPHICS_SDL_H__
#define __GRAPHICS_SDL_H__

//---------------------------------------------------------------------------
/*!
 *  Graphics driver implementation based on the generic GraphicsDriver class
 */
class GraphicsSDL : public GraphicsDriver
{
public:
//---------------------------------------------------------
    virtual void Init();
    virtual uint8_t Open();
    virtual uint8_t Close();
    virtual uint16_t Read( uint16_t u16Bytes_, uint8_t *pu8Data_ )
        { return 0; }
    virtual uint16_t Write( uint16_t u16Bytes_, uint8_t *pu8Data_ )
        { return 0; }
    virtual uint16_t Control( uint16_t u16Event_, void *pvDataIn_, uint16_t u16SizeIn_, void *pvDataOut_, uint16_t u16SizeOut_ )
        { return 0; }        
        
//---------------------------------------------------------
    virtual void DrawPixel(DrawPoint_t *pstPoint_);
    virtual void ReadPixel(DrawPoint_t *pstPoint_);
//---------------------------------------------------------
/*
    Raster operations defined using per-pixel rendering.
    Can be overridden in inheriting classes.
*/
    // virtual void ClearScreen();
    // virtual void Point(DrawPoint_t *pstPoint_);
    // virtual void Line(DrawLine_t *pstLine_);
    // virtual void Rectangle(DrawRectangle_t *pstRectangle_);
    // virtual void Circle(DrawCircle_t *pstCircle_);
    // virtual void Ellipse(DrawEllipse_t *pstEllipse_);
    // virtual void Bitmap(DrawBitmap_t *pstBitmap_);
    // virtual void Stamp(DrawStamp_t *pstStamp_);
    // virtual void Move(DrawMove_t *pstMove_ );
    // virtual void TriangleWire(DrawPoly_t *pstPoly_);
    // virtual void TriangleFill(DrawPoly_t *pstPoly_);
    // virtual void Polygon(DrawPoly_t *pstPoly_);

    // virtual void Text(DrawText_t *pstText_);
    // virtual uint16_t TextWidth(DrawText_t *pstText_) {return 0;}

    void Flip();
    
private:
    SDL_Surface *m_pstScreen;
};

#endif
