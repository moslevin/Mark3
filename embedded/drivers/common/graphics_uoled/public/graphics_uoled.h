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
    \file graphics_uoled.h
    
    \brief Graphics driver implementation on 4D systems uOLED displays
    
*/

#include "driver.h"
#include "draw.h"
#include "graphics.h"
#include "driver.h"

#ifndef __GRAPHICS_UOLED_H__
#define __GRAPHICS_UOLED_H__

//---------------------------------------------------------------------------
typedef struct
{
    struct
    {
        union
        {
            uint16_t u16Data;
            uint8_t u8Data;
        };
    };
    uint8_t u8Len;
} DataVector_t;


/*!
    Graphics driver implementation based on the generic GraphicsDriver class
*/
class GraphicsUOLED : public GraphicsDriver
{
public:
//---------------------------------------------------------
    virtual void Init() {}
    virtual uint8_t Open() { return 0; }
    virtual uint8_t Close() { return 0; }
    virtual uint16_t Read( uint16_t u16Bytes_, uint8_t *pu8Data_ )
        { return 0; }
    virtual uint16_t Write( uint16_t u16Bytes_, uint8_t *pu8Data_ )
        { return 0; }
    virtual uint16_t Control( uint16_t u16Event_, void *pvDataIn_, uint16_t u16SizeIn_, void *pvDataOut_, uint16_t u16SizeOut_ )
        { return 0; }        
        
//---------------------------------------------------------
    virtual void DrawPixel(DrawPoint_t *pstPoint_) { Point(pstPoint_); }

//---------------------------------------------------------
/*
    Raster operations defined using per-pixel rendering.
    Can be overridden in inheriting classes.
*/
    virtual void ClearScreen();
    virtual void Point(DrawPoint_t *pstPoint_);
    virtual void Line(DrawLine_t *pstLine_);
    virtual void Rectangle(DrawRectangle_t *pstRectangle_);
    virtual void Circle(DrawCircle_t *pstCircle_);
    virtual void Ellipse(DrawEllipse_t *pstEllipse_) {}
    virtual void Bitmap(DrawBitmap_t *pstBitmap_) {}
    virtual void Stamp(DrawStamp_t *pstStamp_) {}
    virtual void Move(DrawMove_t *pstMove_ ) {}
    virtual void TriangleWire(DrawPoly_t *pstPoly_);
    virtual void TriangleFill(DrawPoly_t *pstPoly_) {}
    virtual void Polygon(DrawPoly_t *pstPoly_);

    virtual void Text(DrawText_t *pstText_);
    virtual uint16_t TextWidth(DrawText_t *pstText_);
    void SetDriver( Driver *pclDriver_ ) { m_pclDriver = pclDriver_; }
private:


    void WriteByte( uint8_t u8Byte_ );
    void WriteWord( uint16_t u16Word_ );
    void WriteVector( DataVector_t *pstVector_, uint8_t u8Count_);

    uint8_t ReadByte( void );
    uint8_t WaitAck( void );

    void MoveCursor(uint16_t u16X_, uint16_t u16Y_);
    void MoveOrigin( uint16_t u16X_, uint16_t u16Y_ );

    void SetOutlineColor( COLOR uColor_ );
    void SetFontFGColor(COLOR uColor_);
    void SetFontBGColor(COLOR uColor_);
    void SetTextOpacity(bool bOpaque_);

    Driver *m_pclDriver;
    
    COLOR m_uTextColor;
};

#endif
