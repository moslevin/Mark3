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
    \file draw.h
    \brief Raster graphics APIs
    Description:
        Implements basic drawing functionality.  This forms a hardware abstraction
        layer which requires a backend for rendering.
*/
//---------------------------------------------------------------------------

#ifndef __DRAW_H_
#define __DRAW_H_

#include "kerneltypes.h"
#include "font.h"
#include "colorspace.h"

//---------------------------------------------------------------------------
// Event definitions for 2D hardware accelerated graphics functions
typedef enum
{
    //--[Mandatory for a display driver]-------------------------------------
    DISPLAY_EVENT_SET_PIXEL = 0x00,    
    DISPLAY_EVENT_GET_PIXEL,
	
    //--[Optional if supported in hardware]----------------------------------
    DISPLAY_EVENT_CLEAR,
    DISPLAY_EVENT_LINE,
    DISPLAY_EVENT_RECTANGLE,
    DISPLAY_EVENT_CIRCLE,
    DISPLAY_EVENT_ELLIPSE,
    DISPLAY_EVENT_BITMAP,
    DISPLAY_EVENT_STAMP,
    DISPLAY_EVENT_TEXT,    
    DISPLAY_EVENT_MOVE,
	DISPLAY_EVENT_POLY
} DisplayEvent_t;

//---------------------------------------------------------------------------
/*!
    \brief Defines a pixel
*/
typedef struct
{
    uint16_t u16X;       //!< X coordinate of the pixel
    uint16_t u16Y;       //!< Y coordinate of the pixel
    COLOR uColor;       //!< Color of the pixel
} DrawPoint_t;

//---------------------------------------------------------------------------
/*!
    \brief Defines a simple line
*/
typedef struct
{
    uint16_t u16X1;      //!< Starting X coordinate
    uint16_t u16X2;      //!< Ending X coordinate
    uint16_t u16Y1;      //!< Starting Y Coordinate
    uint16_t u16Y2;      //!< Ending Y coordinate
    COLOR uColor;       //!< Color of the pixel
} DrawLine_t;
//---------------------------------------------------------------------------
/*!
    \brief Defines a rectangle
*/
typedef struct
{
    uint16_t u16Left;    //!< Leftmost pixel of the rectangle
    uint16_t u16Top;     //!< Topmost pixel of the rectangle
    uint16_t u16Right;   //!< Rightmost pixel of the rectangle
    uint16_t u16Bottom;  //!< Bottom pixel of the rectangle
    COLOR u32ineColor;   //!< Color of the line
    bool bFill;       //!< Whether or not to floodfill the interior
    COLOR uFillColor;   //!< Color of the interior of the rectangle
} DrawRectangle_t;
//---------------------------------------------------------------------------
/*!
    \brief Defines a circle
*/
typedef struct
{
    uint16_t u16X;       //!< Center X pixel
    uint16_t u16Y;       //!< Center Y pixel
    uint16_t u16Radius;  //!< Radius in pixels
    COLOR u32ineColor;   //!< Color of the circle perimeter
    bool bFill;       //!< Whether or not to fill the interior of the circle
    COLOR uFillColor;   //!< Fill color for the circle
} DrawCircle_t;
//---------------------------------------------------------------------------
/*!
    \brief Defines a ellipse
*/
typedef struct
{
    uint16_t u16X;       //!< Center X pixel
    uint16_t u16Y;       //!< Center Y pixel
    uint16_t u16Height;  //!< Height of the ellipse
    uint16_t u16Width;   //!< Width of the ellipse
    COLOR uColor;       //!< Color of the ellipse perimeter
} DrawEllipse_t;
//---------------------------------------------------------------------------
/*!
    \brief Defines a bitmap
*/
typedef struct
{
    uint16_t u16X;       //!< Leftmost pixel
    uint16_t u16Y;       //!< Uppermost pixel
    uint16_t u16Width;   //!< Width of the bitmap in pixels
    uint16_t u16Height;  //!< Height of the bitmap in pixels
    uint8_t u8BPP;      //!< Bits-per-pixel
    uint8_t *pu8Data;   //!< Pixel data pointer
} DrawBitmap_t;
//---------------------------------------------------------------------------
/*!
    \brief Defines a 1-bit 2D bitmap of arbitrary resolution
*/
typedef struct
{
    uint16_t u16X;       //!< Leftmost pixel
    uint16_t u16Y;       //!< Uppermost pixel
    uint16_t u16Width;   //!< Width of the stamp
    uint16_t u16Height;  //!< Height of the stamp
    COLOR uColor;       //!< Color of the stamp
    uint8_t *pu8Data;   //!< Pointer to the stamp data
} DrawStamp_t;    // monochrome stamp, bitpacked 8bpp

//---------------------------------------------------------------------------
/*!
    \brief Defines a bitmap-rendered string
*/
typedef struct
{
    uint16_t u16Left;           //!< Leftmost pixel of the text
    uint16_t u16Top;           //!< Uppermost pixel of the text
    COLOR uColor;           //!< Color of the text
    Font_t *pstFont;  //!< Pointer to the font used to render the text
    const char *pcString;     //!< ASCII String to render
} DrawText_t;

//---------------------------------------------------------------------------
#define TEXTFX_FLAG_OPAQUE_BG   (0x01)  //!< use an opaque BG
#define TEXTFX_FLAG_ROTATE      (0x02)  //!< Apply text rotation
#define TEXTFX_FLAG_SCALE_X     (0x04)  //!< Scale the text horizontally
#define TEXTFX_FLAG_SCALE_Y     (0x08)  //!< Scale the text vertically

//---------------------------------------------------------------------------
typedef struct
{
    uint8_t u8Flags;        //!< Text effects applied
    COLOR uBGColor;         //!< Background color for opaque backgrounds
    uint16_t u16RotateDeg;   //!< Rotation in degrees
    uint16_t u16ScaleX100;   //!< Scaling factor, fixed point modulo 100
    uint16_t u16ScaleY100;   //!< Scaling factor, fixed point modulo 100
} TextFX_t;

//---------------------------------------------------------------------------
/*!
    \brief Defines the active window - establishes boundaries for drawing on
    the current display.  Only pixels drawn inside the surface boundaries are
    rendered to the output
*/
typedef struct
{
    uint16_t u16Left;        //!< Left boundary
    uint16_t u16Right;       //!< Right boundary
    uint16_t u16Top;         //!< Upper boundary
    uint16_t u16Bottom;      //!< Bottom boundary
} DrawWindow_t;

//---------------------------------------------------------------------------
/*!
    \brief Simple 2D copy/paste.  Moves a bitmap specified by the given source
    coordinates on-surface to the destination coordinates.
*/
typedef struct
{
    uint16_t u16SrcX;        //!< Source X pixel (leftmost)
    uint16_t u16SrcY;        //!< Source Y pixel (topmost)
    uint16_t u16DstX;        //!< Destination X pixel (leftmost)
    uint16_t u16DstY;        //!< Destination Y pixel (topmost)
    uint16_t u16CopyHeight;  //!< Number of rows to copy 
    uint16_t u16CopyWidth;   //!< Number of columns to copy
} DrawMove_t;

//---------------------------------------------------------------------------
/*!
    \brief Specifies a single 2D point.  When used in arrays, this provides
    a way to draw vector paths, which form the basis of the polygon data
    structures.
*/
typedef struct
{
    uint16_t u16X;
    uint16_t u16Y;
} DrawVector_t;

//---------------------------------------------------------------------------
/*!
    \brief Defines the structure of an arbitrary polygon.  Can be used to 
    specify the 
*/
typedef struct
{
    uint16_t     u16NumPoints;   //!< Number of points in the polygon
    COLOR        uColor;        //!< Color to use for lines/fill
    bool       bFill;         //!< Display as wireframe or filled
    DrawVector_t *pstVector;    //!< Vector points making the polygon
} DrawPoly_t;

#endif //__DRAW_H_
