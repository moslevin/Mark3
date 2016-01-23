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
	\file graphics.cpp
	\brief Generic graphics driver implementation
*/

#include "kerneltypes.h"
#include "graphics.h"
#include "draw.h"
#include "driver.h"
#include "colorspace.h"
#include "font.h"

#include "kernelaware.h"

#include <stdio.h>

//---------------------------------------------------------------------------
void GraphicsDriver::ClearScreen()
{
	DrawPoint_t stPoint;
	stPoint.uColor = COLOR_BLACK;

    for (stPoint.u16X = 0; stPoint.u16X < m_u16Res16X; stPoint.u16X++)
	{
        for (stPoint.u16Y = 0; stPoint.u16Y < m_u16Res16Y; stPoint.u16Y++)
		{
			// Pixel Write
			DrawPixel(&stPoint);
		}
	}
}
//---------------------------------------------------------------------------
void GraphicsDriver::Point(DrawPoint_t *pstPoint_)
{
    DrawPixel(pstPoint_);
}

//---------------------------------------------------------------------------
void GraphicsDriver::Line(DrawLine_t *pstLine_)
{
	// Bresenham Line drawing algorithm, adapted from:
	// www.cs.unc.edu/~mcmillan/comp136/Lecture6/Lines.html

	DrawPoint_t stPoint;
    int16_t s16X1 = (int16_t)pstLine_->u16X1;
    int16_t s16X2 = (int16_t)pstLine_->u16X2;
    int16_t s16Y1 = (int16_t)pstLine_->u16Y1;
    int16_t s16Y2 = (int16_t)pstLine_->u16Y2;
    int16_t s16DeltaY = s16Y2 - s16Y1;
    int16_t s16DeltaX = s16X2 - s16X1;
	char cStepx, cStepy;
	stPoint.uColor = pstLine_->uColor;

    if (s16DeltaY < 0)
	{
        s16DeltaY = -s16DeltaY;
		cStepy = -1;
	}
	else
	{
		cStepy = 1;
	}

    if (s16DeltaX < 0)
	{
        s16DeltaX = -s16DeltaX;
		cStepx = -1;
	}
	else
	{
		cStepx = 1;
	}

	// Scale by a factor of 2 in each direction
    s16DeltaY <<= 1;
    s16DeltaX <<= 1;

    stPoint.u16X = s16X1;
    stPoint.u16Y = s16Y1;
	DrawPixel(&stPoint);

    if (s16DeltaX > s16DeltaY)
	{
        int16_t s16Fraction = s16DeltaY - (s16DeltaX >> 1);

        while (s16X1 != s16X2)
		{
            if (s16Fraction >= 0)
			{
                s16Y1 += cStepy;
                s16Fraction -= s16DeltaX;
			}
            s16X1 += cStepx;
            s16Fraction += s16DeltaY;

            stPoint.u16X = s16X1;
            stPoint.u16Y = s16Y1;
			DrawPixel(&stPoint);
		}
	}
	else
	{
        int16_t s16Fraction = s16DeltaX - (s16DeltaY >> 1);
        while (s16Y1 != s16Y2)
		{
            if (s16Fraction >= 0)
			{
                s16X1 += cStepx;
                s16Fraction -= s16DeltaY;
			}
            s16Y1 += cStepy;
            s16Fraction += s16DeltaX;

            stPoint.u16X = s16X1;
            stPoint.u16Y = s16Y1;
			DrawPixel(&stPoint);
		}
	}
}

//---------------------------------------------------------------------------
void GraphicsDriver::Rectangle(DrawRectangle_t *pstRectangle_)
{
	DrawPoint_t stPoint;

	// if drawing a background fill color (optional)
	if (pstRectangle_->bFill == true)
	{
		stPoint.uColor = pstRectangle_->uFillColor;
		for (stPoint.u16X = pstRectangle_->u16Left; stPoint.u16X <= pstRectangle_->u16Right; stPoint.u16X++)
		{
			for (stPoint.u16Y = pstRectangle_->u16Top; stPoint.u16Y <= pstRectangle_->u16Bottom; stPoint.u16Y++)
			{
				DrawPixel(&stPoint);
			}
		}
	}

	// Draw four orthogonal lines...
	stPoint.uColor = pstRectangle_->u32ineColor;
	stPoint.u16Y = pstRectangle_->u16Top;
	for (stPoint.u16X = pstRectangle_->u16Left; stPoint.u16X <= pstRectangle_->u16Right; stPoint.u16X++)
	{
		DrawPixel(&stPoint);
	}

	stPoint.u16Y = pstRectangle_->u16Bottom;
	for (stPoint.u16X = pstRectangle_->u16Left; stPoint.u16X <= pstRectangle_->u16Right; stPoint.u16X++)
	{
		DrawPixel(&stPoint);
	}

	stPoint.u16X = pstRectangle_->u16Left;
	for (stPoint.u16Y = pstRectangle_->u16Top; stPoint.u16Y <= pstRectangle_->u16Bottom; stPoint.u16Y++)
	{
		DrawPixel(&stPoint);
	}

	stPoint.u16X = pstRectangle_->u16Right;
	for (stPoint.u16Y = pstRectangle_->u16Top; stPoint.u16Y <= pstRectangle_->u16Bottom; stPoint.u16Y++)
	{
		DrawPixel(&stPoint);
	}
}

//---------------------------------------------------------------------------
void GraphicsDriver::Circle(DrawCircle_t *pstCircle_)
{
	DrawPoint_t stPoint;
    int16_t s16X;
    int16_t s16Y;
	uint32_t u32RadSquare;

	uint32_t u32XSquare;
	uint32_t u32YSquare;

	// Get the radius squared value...
    u32RadSquare = (uint32_t)pstCircle_->u16Radius;
    u32RadSquare *= u32RadSquare;

	// Look at the upper-right quarter of the circle
    for (s16X = 0; s16X <= (int16_t)pstCircle_->u16Radius; s16X++)
	{
        u32XSquare = (uint32_t)s16X;
        u32XSquare *= u32XSquare;
        for (s16Y = 0; s16Y <= (int16_t)pstCircle_->u16Radius; s16Y++)
		{
            u32YSquare = (uint32_t)s16Y;
            u32YSquare *= u32YSquare;

			// if filled...
			if (pstCircle_->bFill == true)
			{
				stPoint.uColor = pstCircle_->uFillColor;
				if (u32XSquare + u32YSquare <= u32RadSquare)
				{
					// Draw the fill color at the appropriate locations (quadrature...)
                    stPoint.u16X = pstCircle_->u16X + s16X;
                    stPoint.u16Y = pstCircle_->u16Y + s16Y;
					DrawPixel(&stPoint);
                    stPoint.u16X = pstCircle_->u16X - s16X;
                    stPoint.u16Y = pstCircle_->u16Y + s16Y;
					DrawPixel(&stPoint);
                    stPoint.u16X = pstCircle_->u16X + s16X;
                    stPoint.u16Y = pstCircle_->u16Y - s16Y;
					DrawPixel(&stPoint);
                    stPoint.u16X = pstCircle_->u16X - s16X;
                    stPoint.u16Y = pstCircle_->u16Y - s16Y;
					DrawPixel(&stPoint);
				}
			}
			// Check for edge...
			if (
				((u32XSquare + u32YSquare) >= (u32RadSquare-pstCircle_->u16Radius)) &&
				((u32XSquare + u32YSquare) <= (u32RadSquare+pstCircle_->u16Radius))
			   )
			{
				stPoint.uColor = pstCircle_->u32ineColor;

				// Draw the fill color at the appropriate locations (quadrature...)
                stPoint.u16X = pstCircle_->u16X + s16X;
                stPoint.u16Y = pstCircle_->u16Y + s16Y;
				DrawPixel(&stPoint);
                stPoint.u16X = pstCircle_->u16X - s16X;
                stPoint.u16Y = pstCircle_->u16Y + s16Y;
				DrawPixel(&stPoint);
                stPoint.u16X = pstCircle_->u16X + s16X;
                stPoint.u16Y = pstCircle_->u16Y - s16Y;
				DrawPixel(&stPoint);
                stPoint.u16X = pstCircle_->u16X - s16X;
                stPoint.u16Y = pstCircle_->u16Y - s16Y;
				DrawPixel(&stPoint);
			}
		}
	}
}

//---------------------------------------------------------------------------
void GraphicsDriver::Ellipse(DrawEllipse_t *pstEllipse_)
{
	DrawPoint_t stPoint;
    int16_t s16X;
    int16_t s16Y;
	uint32_t u32Radius;
	uint32_t u32HSquare;
	uint32_t u32VSquare;
	uint32_t u32XSquare;
	uint32_t u32YSquare;

    u32HSquare = (uint32_t)pstEllipse_->u16Width;
    u32HSquare *= u32HSquare;

    u32VSquare = (uint32_t)pstEllipse_->u16Height;
    u32VSquare *= u32VSquare;

    u32Radius = u32HSquare * u32VSquare;

    for (s16X = 0; s16X <= (int16_t)pstEllipse_->u16Width; s16X++)
	{
        u32XSquare = (uint32_t)s16X;
        u32XSquare *= u32XSquare;
        u32XSquare *= u32HSquare;

        for (s16Y = 0; s16Y <= (int16_t)pstEllipse_->u16Height; s16Y++)
		{
            u32YSquare = (uint32_t)s16Y;
            u32YSquare *= u32YSquare;
            u32YSquare *= u32VSquare;

			if ((u32XSquare + u32YSquare) <= u32Radius)
			{
				// Draw the fill color at the appropriate locations (quadrature...)
                stPoint.u16X = pstEllipse_->u16X + s16X;
                stPoint.u16Y = pstEllipse_->u16Y + s16Y;
				DrawPixel(&stPoint);
                stPoint.u16X = pstEllipse_->u16X - s16X;
                stPoint.u16Y = pstEllipse_->u16Y + s16Y;
				DrawPixel(&stPoint);
                stPoint.u16X = pstEllipse_->u16X + s16X;
                stPoint.u16Y = pstEllipse_->u16Y - s16Y;
				DrawPixel(&stPoint);
                stPoint.u16X = pstEllipse_->u16X - s16X;
                stPoint.u16Y = pstEllipse_->u16Y - s16Y;
				DrawPixel(&stPoint);
			}
		}
	}
}

//---------------------------------------------------------------------------
void GraphicsDriver::Bitmap(DrawBitmap_t *pstBitmap_)
{
	uint16_t u16Row;
	uint16_t u16Col;

	DrawPoint_t stPoint;
    COLOR *pColor = (COLOR*)(pstBitmap_->pu8Data);
	for (u16Row = pstBitmap_->u16Y; u16Row < (pstBitmap_->u16Y + pstBitmap_->u16Height); u16Row++)
	{
		for (u16Col = pstBitmap_->u16X; u16Col < (pstBitmap_->u16X + pstBitmap_->u16Width); u16Col++)
		{
			stPoint.u16X = u16Col;
			stPoint.u16Y = u16Row;

			// Build the color.
            stPoint.uColor = *pColor++;

			// Draw the point.
			DrawPixel(&stPoint);

        }
	}
}

//---------------------------------------------------------------------------
void GraphicsDriver::Stamp(DrawStamp_t *pstStamp_)
{
	uint16_t u16Row;
	uint16_t u16Col;
	uint16_t u16Shift;
	uint16_t u16Index;
	DrawPoint_t stPoint;

    u16Index = 0;
	for (u16Row = pstStamp_->u16Y; u16Row < (pstStamp_->u16Y + pstStamp_->u16Height); u16Row++)
	{
        u16Shift = 0x80;
		for (u16Col = pstStamp_->u16X; u16Col < (pstStamp_->u16X + pstStamp_->u16Width); u16Col++)
		{
			// If the packed bit in the bitmap is a "1", draw the color.
			if (pstStamp_->pu8Data[u16Index] & u16Shift)
			{
				stPoint.u16X = u16Col;
				stPoint.u16Y = u16Row;
				stPoint.uColor = pstStamp_->uColor;
				DrawPixel(&stPoint);
			}
			// Stamps are opaque, don't fill in the BG

			// Shift to the next bit in the field
            u16Shift >>= 1;

			// Rollover - next bit in the bitmap.
			// This obviously works best for stamps that are multiples of 8x8
			if (u16Shift == 0)
			{
                u16Shift = 0x80;
                u16Index++;
			}
		}
	}
}

//----------------------------------------------------------------------------
void GraphicsDriver::Move( DrawMove_t *pstMove_ )
{
	DrawPoint_t stPoint;
    int32_t s16X;
    int32_t s16Y;
    int32_t s16XInc = 0;
    int32_t s16YInc = 0;

	bool bLeftToRight = false;
	bool bTopToBottom = false;

	if (pstMove_->u16SrcX > pstMove_->u16DstX)
	{
		bLeftToRight = true;
	}
	if (pstMove_->u16SrcY > pstMove_->u16DstY)
	{
		bTopToBottom = true;
	}

	if (bLeftToRight)
	{
        s16XInc++;
	}
	else
	{
        s16XInc--;
		pstMove_->u16SrcX += pstMove_->u16CopyWidth - 1;
		pstMove_->u16DstX += pstMove_->u16CopyWidth - 1;
	}

	if (bTopToBottom)
	{
        s16YInc++;
	}
	else
	{
        s16YInc--;
		pstMove_->u16SrcY += pstMove_->u16CopyHeight - 1;
		pstMove_->u16DstY += pstMove_->u16CopyHeight - 1;
	}

	// Hideously inefficient memory move...
    for (s16X = 0; s16X < pstMove_->u16CopyWidth; s16X++)
	{
        for (s16Y = 0; s16Y < pstMove_->u16CopyHeight; s16Y++)
		{
			// Read from source (value read into the point struct)
            stPoint.u16Y = (uint16_t)((int32_t)pstMove_->u16SrcY + ((int32_t)s16Y * s16YInc));
            stPoint.u16X = (uint16_t)((int32_t)pstMove_->u16SrcX + ((int32_t)s16X * s16XInc));
			ReadPixel(&stPoint);

			// Copy to dest
            stPoint.u16Y = (uint16_t)((int32_t)pstMove_->u16DstY + ((int32_t)s16Y * s16YInc));
            stPoint.u16X = (uint16_t)((int32_t)pstMove_->u16DstX + ((int32_t)s16X * s16XInc));
			DrawPixel(&stPoint);
		}
	}
}

//---------------------------------------------------------------------------
void GraphicsDriver::Text(DrawText_t *pstText_)
{
	uint16_t u16X, u16Y;
	uint16_t u16StartX;
	uint16_t u16StartY;
	uint16_t u16CharOffsetX;
	uint16_t u16CharIndex = 0;
	uint8_t *pu8Data = (uint8_t*)pstText_->pstFont->pu8FontData;
	DrawPoint_t stPoint;

	// set the color for this element.
	stPoint.uColor = pstText_->uColor;

    u16CharOffsetX = 0;

	// Draw every character in the string, one at a time
	while (pstText_->pcString[u16CharIndex] != 0)
	{
		uint16_t u16Offset = 0;
		
		uint8_t u8Width;
		uint8_t u8Height;
		uint8_t u8VOffset;
		uint8_t u8Bitmask;
		
		// Read the glyphs from memory until we arrive at the one we wish to print
		for (u16X = 0; u16X < pstText_->pcString[u16CharIndex]; u16X++)
		{
			// Glyphs are variable-sized for efficiency - to look up a particular 
			// glyph, we must traverse all preceding glyphs in the list
            u8Width  = Font_ReadByte(u16Offset, pu8Data);
            u8Height = Font_ReadByte(u16Offset + 1, pu8Data);

			// Adjust the offset to point to the next glyph
            u16Offset += ((((uint16_t)u8Width + 7) >> 3) * (uint16_t)u8Height)
						+ (sizeof(Glyph_t) - 1);		
		}
	
		// Header information:  glyph size and vertical offset
        u8Width   = Font_ReadByte(u16Offset++, pu8Data);
        u8Height  = Font_ReadByte(u16Offset++, pu8Data);
        u8VOffset = Font_ReadByte(u16Offset++, pu8Data);

        u16StartY = pstText_->u16Top + (uint16_t)u8VOffset;
        u16StartX = pstText_->u16Left;
		
		// Draw the font from left->right, top->bottom
        for (	u16Y = u16StartY;
                u16Y < u16StartY + (uint16_t)u8Height;
                u16Y++ )
		{
			uint8_t u8TempChar = Font_ReadByte(u16Offset, pu8Data);
            u8Bitmask = 0x80;
			
            for (	u16X = u16CharOffsetX + u16StartX;
                    u16X < u16CharOffsetX + u16StartX + (uint16_t)u8Width;
                    u16X++ )
			{				
				if (!u8Bitmask)
				{
                    u8Bitmask = 0x80;
                    u16Offset++;
                    u8TempChar = Font_ReadByte(u16Offset, pu8Data);
				}
				
				if (u8TempChar & u8Bitmask)
				{
					// Update the location
					stPoint.u16X = u16X;
					stPoint.u16Y = u16Y;

					// Draw the point.
					DrawPixel(&stPoint);
				}
				
                u8Bitmask >>= 1;
			}

            u16Offset++;
		}

		// Next character
        u16CharIndex++;
        u16CharOffsetX += (uint16_t)u8Width + 1;
	}
}

//---------------------------------------------------------------------------
void GraphicsDriver::TextFX(DrawText_t *pstText_, TextFX_t *pstFX_ )
{
    uint16_t u16X, u16Y;
    uint16_t u16PartialX = 0;
    uint16_t u16PartialY = 0;
    uint16_t u16StartX;
    uint16_t u16StartY;
    uint16_t u16CharOffsetX;
    uint16_t u16CharIndex = 0;
    uint8_t *pu8Data = (uint8_t*)pstText_->pstFont->pu8FontData;
    DrawPoint_t stPoint;

    // set the color for this element.
    stPoint.uColor = pstText_->uColor;

    u16CharOffsetX = 0;

    // Draw every character in the string, one at a time
    while (pstText_->pcString[u16CharIndex] != 0)
    {
        uint16_t u16Offset = 0;

        uint8_t u8Width;
        uint8_t u8Height;
        uint8_t u8VOffset;
        uint8_t u8Bitmask;

        // Read the glyphs from memory until we arrive at the one we wish to print
        for (u16X = 0; u16X < pstText_->pcString[u16CharIndex]; u16X++)
        {
            // Glyphs are variable-sized for efficiency - to look up a particular
            // glyph, we must traverse all preceding glyphs in the list
            u8Width  = Font_ReadByte(u16Offset, pu8Data);
            u8Height = Font_ReadByte(u16Offset + 1, pu8Data);

            // Adjust the offset to point to the next glyph
            u16Offset += ((((uint16_t)u8Width + 7) >> 3) * (uint16_t)u8Height)
                        + (sizeof(Glyph_t) - 1);
        }

        // Header information:  glyph size and vertical offset
        u8Width   = Font_ReadByte(u16Offset++, pu8Data);
        u8Height  = Font_ReadByte(u16Offset++, pu8Data);
        u8VOffset = Font_ReadByte(u16Offset++, pu8Data);

        u16StartY = pstText_->u16Top + (uint16_t)u8VOffset;
        u16StartX = pstText_->u16Left;

        // Draw the font from left->right, top->bottom
        for (	u16Y = u16StartY;
                u16Y < u16StartY + (uint16_t)u8Height;
                u16Y++ )
        {
            uint8_t u8TempChar = Font_ReadByte(u16Offset, pu8Data);
            u8Bitmask = 0x80;
            u16PartialY = 0;
            u16PartialX = 0;

            uint16_t u16TempPartialX = 0;

            for (	u16X = u16CharOffsetX + u16StartX;
                    u16X < u16CharOffsetX + u16StartX + (uint16_t)u8Width;
                    u16X++ )
            {
                uint16_t u16TempPartialY = 0;
                u16PartialY = 0;
                if (!u8Bitmask)
                {
                    u8Bitmask = 0x80;
                    u16Offset++;
                    u8TempChar = Font_ReadByte(u16Offset, pu8Data);
                }

                if ((u8TempChar & u8Bitmask) || (pstFX_->u8Flags & TEXTFX_FLAG_OPAQUE_BG))
                {
                    // u16X and u16Y represent the untransformed data...
                    // we need u16StartX, u16StartY, u16DeltaX, u16DeltaY to proceed.
                    uint16_t u16DeltaX = (u16X - pstText_->u16Left);
                    uint16_t u16DeltaY = (u16Y - pstText_->u16Top);

                    // Compute "unadjusted" pixels for normal or scaled
                    uint16_t u16RawX, u16RawY;

                    if (pstFX_->u8Flags & TEXTFX_FLAG_SCALE_X)
                    {
                        u16RawX = u16StartX + (((u16DeltaX * pstFX_->u16ScaleX100))/100);
                        u16TempPartialX = pstFX_->u16ScaleX100;
                    }
                    else
                    {
                        u16RawX = u16X;
                        u16TempPartialX = 100;
                    }
                    u16TempPartialX += u16PartialX;

                    if (pstFX_->u8Flags & TEXTFX_FLAG_SCALE_Y)
                    {
                        u16RawY = u16StartY + (((u16DeltaY * pstFX_->u16ScaleY100))/100);
                        u16TempPartialY = pstFX_->u16ScaleY100;
                    }
                    else
                    {
                        u16RawY = u16Y;
                        u16TempPartialY = 100;
                    }
                    u16TempPartialY += u16PartialY;

                    uint16_t u16BLAH = u16TempPartialX;

                    if (!(u8TempChar & u8Bitmask))
                    {
                        stPoint.uColor = pstFX_->uBGColor;
                    }
                    else
                    {
                        stPoint.uColor = pstText_->uColor;
                    }

                    //!ToDo - Add rotation

                    stPoint.u16X = u16RawX;
                    while (u16TempPartialX >= 50)
                    {
                        stPoint.u16Y = u16RawY;
                        u16BLAH = u16TempPartialY;
                        while (u16BLAH >= 50)
                        {
                            DrawPixel(&stPoint);
                            stPoint.u16Y++;
                            if (u16BLAH >= 100)
                            {
                                u16BLAH -= 100;
                            }
                            else
                            {
                                u16BLAH = 0;
                            }
                        }
                        stPoint.u16X++;
                        if (u16TempPartialX >= 100)
                        {
                            u16TempPartialX -= 100;
                        }
                        else
                        {
                            u16TempPartialX = 0;
                        }
                    }

                    u16PartialX = (u16TempPartialX % 100);
                    u16PartialY = (u16TempPartialY % 100);
                }

                u8Bitmask >>= 1;
            }

            u16Offset++;
        }

        // Next character
        u16CharIndex++;
        u16CharOffsetX += (uint16_t)u8Width + 1;
    }
}

//----------------------------------------------------------------------------
uint16_t GraphicsDriver::TextWidth(DrawText_t *pstText_)
{
    uint16_t u16CharOffsetX;
	uint16_t u16CharIndex = 0;
	uint16_t u16X;
	uint8_t *pu8Data = (uint8_t*)pstText_->pstFont->pu8FontData;

    u16CharOffsetX = 0;

	// Draw every character in the string, one at a time
	while (pstText_->pcString[u16CharIndex] != 0)
	{
		uint16_t u16Offset = 0;
		
		uint8_t u8Width;
		uint8_t u8Height;
		
		// Read the glyphs from memory until we arrive at the one we wish to print
		for (u16X = 0; u16X < pstText_->pcString[u16CharIndex]; u16X++)
		{
			// Glyphs are variable-sized for efficiency - to look up a particular 
			// glyph, we must traverse all preceding glyphs in the list
            u8Width  = Font_ReadByte(u16Offset, pu8Data);
            u8Height = Font_ReadByte(u16Offset + 1, pu8Data);

			// Adjust the offset to point to the next glyph
            u16Offset += ((((uint16_t)u8Width + 7) >> 3) * (uint16_t)u8Height)
						+ (sizeof(Glyph_t) - 1);		
		}
	
		// Header information:  glyph size and vertical offset
        u8Width   = Font_ReadByte(u16Offset, pu8Data);
        u16Offset += (sizeof(Glyph_t) - 1);
	
		// Next character
        u16CharIndex++;
        u16CharOffsetX += (uint16_t)u8Width + 1;
	}
	
	return u16CharOffsetX;
}

//----------------------------------------------------------------------------
void GraphicsDriver::TriangleWire(DrawPoly_t *pstPoly_)
{
    DrawLine_t stLine;

    stLine.uColor = pstPoly_->uColor;

    stLine.u16X1 = pstPoly_->pstVector[0].u16X;
    stLine.u16Y1 = pstPoly_->pstVector[0].u16Y;
    stLine.u16X2 = pstPoly_->pstVector[1].u16X;
    stLine.u16Y2 = pstPoly_->pstVector[1].u16Y;
    Line(&stLine);

    stLine.u16X1 = pstPoly_->pstVector[1].u16X;
    stLine.u16Y1 = pstPoly_->pstVector[1].u16Y;
    stLine.u16X2 = pstPoly_->pstVector[2].u16X;
    stLine.u16Y2 = pstPoly_->pstVector[2].u16Y;
    Line(&stLine);

    stLine.u16X1 = pstPoly_->pstVector[2].u16X;
    stLine.u16Y1 = pstPoly_->pstVector[2].u16Y;
    stLine.u16X2 = pstPoly_->pstVector[0].u16X;
    stLine.u16Y2 = pstPoly_->pstVector[0].u16Y;
    Line(&stLine);
}
//----------------------------------------------------------------------------
void GraphicsDriver::TriangleFill(DrawPoly_t *pstPoly_)
{
    // Drawing a raster-filled triangle:
    uint8_t u8MaxEdge = 0;
    uint8_t u8MinEdge1 = 0, u8MinEdge2 = 0;
    int16_t s16Max = 0;
    int16_t s16Temp;

    int16_t s16DeltaX1, s16DeltaX2;
    int16_t s16DeltaY1, s16DeltaY2;
    char cStepX1, cStepX2;
    char cStepY;
    int16_t s16X1, s16X2, s16X3, s16Y1, s16Y2, s16Y3;
    int16_t s16TempX1, s16TempY1, s16TempX2, s16TempY2;
    int16_t s16Fraction1;
    int16_t s16Fraction2;
    int16_t i;
    DrawPoint_t stPoint;

    // Figure out which line segment is the longest
    s16Temp = (int16_t)pstPoly_->pstVector[0].u16Y - (int16_t)pstPoly_->pstVector[1].u16Y;
    if( s16Temp < 0 )    { s16Temp = -s16Temp; }
    if( s16Temp > s16Max ) { s16Max  = s16Temp; u8MaxEdge = 0; u8MinEdge1 = 1; u8MinEdge2 = 2;}

    s16Temp = (int16_t)pstPoly_->pstVector[1].u16Y - (int16_t)pstPoly_->pstVector[2].u16Y;
    if( s16Temp < 0 )    { s16Temp = -s16Temp; }
    if( s16Temp > s16Max ) { s16Max  = s16Temp; u8MaxEdge = 1; u8MinEdge1 = 2; u8MinEdge2 = 0; }

    s16Temp = (int16_t)pstPoly_->pstVector[2].u16Y - (int16_t)pstPoly_->pstVector[0].u16Y;
    if( s16Temp < 0 )    { s16Temp = -s16Temp; }
    if( s16Temp > s16Max ) { s16Max  = s16Temp; u8MaxEdge = 2; u8MinEdge1 = 0; u8MinEdge2 = 1;}

    // Label the vectors and copy into temporary signed buffers
    s16X1 = (int16_t)pstPoly_->pstVector[u8MaxEdge].u16X;
    s16X2 = (int16_t)pstPoly_->pstVector[u8MinEdge1].u16X;
    s16X3 = (int16_t)pstPoly_->pstVector[u8MinEdge2].u16X;

    s16Y1 = (int16_t)pstPoly_->pstVector[u8MaxEdge].u16Y;
    s16Y2 = (int16_t)pstPoly_->pstVector[u8MinEdge1].u16Y;
    s16Y3 = (int16_t)pstPoly_->pstVector[u8MinEdge2].u16Y;

    // Figure out whether or not we're drawing up-down or down-up
    s16DeltaY1 = s16Y1 - s16Y2;
    if (s16DeltaY1 < 0) { cStepY = -1; s16DeltaY1 = -s16DeltaY1; } else { cStepY = 1; }

    s16DeltaX1 = s16X1 - s16X2;
    if (s16DeltaX1 < 0) { cStepX1 = -1; s16DeltaX1 = -s16DeltaX1; } else { cStepX1 = 1; }

    s16DeltaY2 = s16Y1 - s16Y3;
    if (s16DeltaY2 < 0) { cStepY = -1; s16DeltaY2 = -s16DeltaY2; } else { cStepY = 1; }

    s16DeltaX2 = s16X1 - s16X3;
    if (s16DeltaX2 < 0) { cStepX2 = -1; s16DeltaX2 = -s16DeltaX2; } else { cStepX2 = 1; }

    s16DeltaX1 <<=1;
    s16DeltaX2 <<=1;
    s16DeltaY1 <<=1;
    s16DeltaY2 <<=1;

    s16Fraction1 = s16DeltaX1;// - (s16DeltaY1 >> 1);
    s16Fraction2 = s16DeltaX2;// - (s16DeltaY2 >> 1);

    s16TempY1 = s16Y1;
    s16TempY2 = s16Y1;
    s16TempX1 = s16X1;
    s16TempX2 = s16X1;

    stPoint.uColor = pstPoly_->uColor;

    if( s16DeltaY2 != 0 )
    {
        while (s16TempY2 != s16Y3)
        {
            stPoint.u16Y = s16TempY2;
            if( s16TempX1 < s16TempX2 ) {
                for( i = s16TempX1; i <= s16TempX2; i++) {
                    stPoint.u16X = i;
                    Point(&stPoint);
                }
            } else {
                for( i = s16TempX2; i <= s16TempX1; i++ ) {
                    stPoint.u16X = i;
                    Point(&stPoint);
                }
            }

            while (s16Fraction2 >= s16DeltaY2)
            {
                s16TempX2 -= cStepX2;
                s16Fraction2 -= s16DeltaY2;
            }
            s16TempY2 -= cStepY;
            s16Fraction2 += s16DeltaX2;

            while (s16Fraction1 >= s16DeltaY1)
            {
                s16TempX1 -= cStepX1;
                s16Fraction1 -= s16DeltaY1;
            }
            s16TempY1 -= cStepY;
            s16Fraction1 += s16DeltaX1;
        }
    }

    s16DeltaY2 = s16Y3 - s16Y2;
    s16DeltaX2 = s16X3 - s16X2;

    if (s16DeltaX2 < 0) { cStepX2 = -1; s16DeltaX2 = -s16DeltaX2; } else { cStepX2 = 1; }
    if (s16DeltaY2 < 0) { cStepY = -1;  s16DeltaY2 = -s16DeltaY2; } else { cStepY  = 1; }

    s16DeltaX2 <<=1;
    s16DeltaY2 <<=1;

    s16Fraction2 = s16DeltaX2; // - (s16DeltaY2 >> 1);

    s16TempY2 = s16Y3;
    s16TempX2 = s16X3;

    if( s16DeltaY2 != 0)
    {
        while (s16TempY2 != s16Y2)
        {
            stPoint.u16Y = s16TempY2;
            if( s16TempX1 < s16TempX2 ) {
                for( i = s16TempX1; i <= s16TempX2; i++) {
                    stPoint.u16X = i;
                    Point(&stPoint);
                }
            } else {
                for( i = s16TempX2; i <= s16TempX1; i++ ) {
                    stPoint.u16X = i;
                    Point(&stPoint);
                }
            }

            while (s16Fraction2 >= s16DeltaY2)
            {
                s16TempX2 -= cStepX2;
                s16Fraction2 -= s16DeltaY2;
            }
            s16TempY2 -= cStepY;
            s16Fraction2 += s16DeltaX2;

            while (s16Fraction1 >= s16DeltaY1)
            {
                s16TempX1 -= cStepX1;
                s16Fraction1 -= s16DeltaY1;
            }
            s16TempY1 -= cStepY;
            s16Fraction1 += s16DeltaX1;
        }
    }
}

//----------------------------------------------------------------------------
void GraphicsDriver::Polygon(DrawPoly_t *pstPoly_)
{
    uint16_t i,j,k;
    bool bState = false;

    DrawPoly_t   stTempPoly;
    DrawVector_t astTempVec[3];

    if (pstPoly_->u16NumPoints < 3)
    {
        return;
    }

    stTempPoly.uColor = pstPoly_->uColor;
    stTempPoly.bFill = pstPoly_->bFill;
    stTempPoly.pstVector = astTempVec;
    stTempPoly.u16NumPoints = 3;

    astTempVec[0].u16X = pstPoly_->pstVector[0].u16X;
    astTempVec[1].u16X = pstPoly_->pstVector[1].u16X;
    astTempVec[0].u16Y = pstPoly_->pstVector[0].u16Y;
    astTempVec[1].u16Y = pstPoly_->pstVector[1].u16Y;

    j = 2;
    astTempVec[2].u16X = pstPoly_->pstVector[pstPoly_->u16NumPoints - 1].u16X;
    astTempVec[2].u16Y = pstPoly_->pstVector[pstPoly_->u16NumPoints - 1].u16Y;

    k = pstPoly_->u16NumPoints - 2;

    if( pstPoly_->bFill )
    {
        TriangleFill(&stTempPoly);
    }
    else
    {
        TriangleWire(&stTempPoly);
    }

    // Filled polygon/wireframe polygon using triangle decomp.
    for(i = 0; i < pstPoly_->u16NumPoints - 3; i++)
    {
        astTempVec[0].u16X = astTempVec[1].u16X;
        astTempVec[1].u16X = astTempVec[2].u16X;
        astTempVec[0].u16Y = astTempVec[1].u16Y;
        astTempVec[1].u16Y = astTempVec[2].u16Y;

        if( !bState )
        {
            bState = true;
            astTempVec[2].u16X = pstPoly_->pstVector[j].u16X;
            astTempVec[2].u16Y = pstPoly_->pstVector[j].u16Y;
            j++;
        }
        else
        {
            bState = false;
            astTempVec[2].u16X = pstPoly_->pstVector[k].u16X;
            astTempVec[2].u16Y = pstPoly_->pstVector[k].u16Y;
            k--;
        }
        if( pstPoly_->bFill )
        {
            TriangleFill(&stTempPoly);
        }
        else
        {
            TriangleWire(&stTempPoly);
        }
    }
}

//----------------------------------------------------------------------------
void GraphicsDriver::SetWindow(DrawWindow_t *pstWindow_)
{
	if ((pstWindow_->u16Left <= pstWindow_->u16Right) &&
        (pstWindow_->u16Right < m_u16Res16X) &&
        (pstWindow_->u16Left < m_u16Res16X))
	{
		m_u16Left = pstWindow_->u16Left;
		m_u16Right = pstWindow_->u16Right;
	}
    
    if ((pstWindow_->u16Top <= pstWindow_->u16Bottom) &&
		(pstWindow_->u16Top < m_u16Top)  &&
		(pstWindow_->u16Bottom < m_u16Bottom))
	{
	    m_u16Top = pstWindow_->u16Top;    
	    m_u16Bottom = pstWindow_->u16Bottom;
	}
    
}

//----------------------------------------------------------------------------
void GraphicsDriver::ClearWindow()
{
    m_u16Left = 0;
    m_u16Top = 0;
    m_u16Right = m_u16Res16X - 1;
    m_u16Bottom = m_u16Res16Y - 1;
}
