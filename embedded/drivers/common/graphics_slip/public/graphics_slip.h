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
=========================================================================== */
/*!
    \file graphics_slip.h
    
    \brief Graphics driver implementation on-top of SLIP serial.
    
    Graphics driver implementation requires a host capable of interpreting
    the FunkenSLIP graphics protocol and able to render the results.
 */

#include "driver.h"
#include "draw.h"
#include "graphics.h"
#include "slip.h"

#ifndef __GRAPHICS_SLIP_H__
#define __GRAPHICS_SLIP_H__

//---------------------------------------------------------------------------
#define STAMP_CACHE_SIZE            (16)

//---------------------------------------------------------------------------
/*!
 *  This data structure is used to provide a quick lookup table for 
 *  stamps, so that they can be cached instead of redrawn pixel-for-pixel on
 *  every single redraw.
 */
typedef struct
{
    uint8_t u8Access;
    uint8_t *pu8Data;
} SlipStampCache_t;
//---------------------------------------------------------------------------

/*!
 *  Graphics driver implementation based on the generic GraphicsDriver class
 */
class GraphicsSlip : public GraphicsDriver
{
public:
//---------------------------------------------------------
    virtual void Init();
    virtual uint8_t Open() { return 0; }
    virtual uint8_t Close() { return 0; }
    virtual uint16_t Read( uint16_t u16Bytes_, uint8_t *pu8Data_ )
        { return 0; }
    virtual uint16_t Write( uint16_t u16Bytes_, uint8_t *pu8Data_ )
        { return 0; }
    virtual uint16_t Control( uint16_t u16Event_, void *pvDataIn_, uint16_t u16SizeIn_, void *pvDataOut_, uint16_t u16SizeOut_ )
        { return 0; }        
        
//---------------------------------------------------------
    virtual void DrawPixel(DrawPoint_t *pstPoint_);

//---------------------------------------------------------
/*
    Raster operations defined using per-pixel rendering.
    Can be overridden in inheriting classes.
 */
    virtual void ClearScreen();
    // virtual void Point(DrawPoint_t *pstPoint_);
    virtual void Line(DrawLine_t *pstLine_);
    virtual void Rectangle(DrawRectangle_t *pstRectangle_);
    virtual void Circle(DrawCircle_t *pstCircle_);
    virtual void Ellipse(DrawEllipse_t *pstEllipse_);
    // virtual void Bitmap(DrawBitmap_t *pstBitmap_);
    virtual void Stamp(DrawStamp_t *pstStamp_);
    // virtual void Move(DrawMove_t *pstMove_ );
    // virtual void TriangleWire(DrawPoly_t *pstPoly_);
    // virtual void TriangleFill(DrawPoly_t *pstPoly_);
    // virtual void Polygon(DrawPoly_t *pstPoly_);

    //virtual void Text(DrawText_t *pstText_);
    //virtual uint16_t TextWidth(DrawText_t *pstText_) {return 0;}
    void SetSlip( Slip* pclSlip_ ) { m_pclSlip = pclSlip_; }
private:
    bool FindStamp(uint8_t *pu8Stamp_);
    void AddStamp(uint8_t *pu8Stamp_);
    void AddIfNecessary(uint8_t *pu8Stamp_, uint8_t u8Size_);
    uint8_t Strlen(char *szData_);
    
    SlipStampCache_t m_astStampCache[ STAMP_CACHE_SIZE ];
    Slip *m_pclSlip;
    uint8_t m_u8StampCacheIndex;
};

#endif
