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
    \file graphics_flavr.h
    
    \brief Graphics driver implementation using flAVR for co-simulation
*/

#include "driver.h"
#include "draw.h"
#include "graphics.h"
#include "driver.h"

#ifndef __GRAPHICS_FLAVR_H__
#define __GRAPHICS_FLAVR_H__

//---------------------------------------------------------
class GraphicsFlavr : public GraphicsDriver
{
public:
//---------------------------------------------------------
    virtual void Init() { m_u16Res16X = 128; m_u16Res16Y = 160; }
    virtual uint8_t Open()
        { return 0; }
    virtual uint8_t Close()
        { return 0; }
    virtual uint16_t Read( uint16_t u16Bytes_, uint8_t *pu8Data_ )
        { return u16Bytes_; }
    virtual uint16_t Write( uint16_t u16Bytes_, uint8_t *pu8Data_ )
        { return u16Bytes_; }
    virtual uint16_t Control( uint16_t u16Event_, void *pvDataIn_, uint16_t u16SizeIn_, void *pvDataOut_, uint16_t u16SizeOut_ )
        { return 0; }        
        
//---------------------------------------------------------
    virtual void DrawPixel(DrawPoint_t *pstPoint_);

//---------------------------------------------------------
/*
    Raster operations defined using per-pixel rendering.
    Can be overridden in inheriting classes.
*/
//    virtual void ClearScreen();
      virtual void Point(DrawPoint_t *pstPoint_) { DrawPixel(pstPoint_); }
//    virtual void Line(DrawLine_t *pstLine_);
//    virtual void Rectangle(DrawRectangle_t *pstRectangle_);

//    virtual void Circle(DrawCircle_t *pstCircle_);
//    virtual void Ellipse(DrawEllipse_t *pstEllipse_) {}

//    virtual void Bitmap(DrawBitmap_t *pstBitmap_);
//    virtual void Stamp(DrawStamp_t *pstStamp_);

//    virtual void Move(DrawMove_t *pstMove_ ) {}
//    virtual void TriangleWire(DrawPoly_t *pstPoly_);
//    virtual void TriangleFill(DrawPoly_t *pstPoly_) {}
//    virtual void Polygon(DrawPoly_t *pstPoly_);

//    virtual void Text(DrawText_t *pstText_);
//    virtual uint16_t TextWidth(DrawText_t *pstText_);
      void Flip(void);
};

#endif // __GRAPHICS_FLAVR_H__

