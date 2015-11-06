/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2014 Funkenstein Software Consulting, all rights reserved.
See license.txt for more information
===========================================================================*/
#include "tiles.h"
#include "bitstream.h"
#include "mark3cfg.h"
#include "kerneltypes.h"
#include "colordepth.h"
#include "colorspace.h"
#include "graphics.h"
#include "draw.h"

//---------------------------------------------------------------------------
void Tile_8x8::LoadTile (TileDef_t *pstTileDef_)
{
    BitStreamer clStream;

    m_u8Width = pstTileDef_->m_u8Width;
    m_u8Height = pstTileDef_->m_u8Height;

    // Tile size = ((width of the tile in pixels * bits-per-pixel encoded) / 8) rounded to the next byte * number of rows.
    uint16_t u16TileSize = (((uint16_t)pstTileDef_->m_u8Width * ((uint16_t)pstTileDef_->m_eFormat + 1)) + 7)  >> 3;
    COLOR *puBufPtr = m_auTileBuffer;

    u16TileSize *= (uint16_t)(pstTileDef_->m_u8Height);

    clStream.Init( pstTileDef_->m_pu8Data, u16TileSize );
    for (uint8_t i = 0; i < pstTileDef_->m_u8Height; i++)
    {
        for (uint8_t j = 0; j < pstTileDef_->m_u8Width; j++)
        {
            uint8_t u8ColorIndex = clStream.ReadBits((uint8_t)pstTileDef_->m_eFormat + 1);
            *puBufPtr++ = pstTileDef_->m_puPalette[u8ColorIndex];
        }
        // Each row starts with a fresh byte
        clStream.AdvanceByte();
    }
}

//---------------------------------------------------------------------------
void Tile_8x8::Render(GraphicsDriver *pclDriver_, uint16_t u16X_, uint16_t u16Y_)
{
    DrawBitmap_t stBitmap;

    // Create a bitmap object that references our object's tile buffer
    stBitmap.pu8Data = (uint8_t*)m_auTileBuffer;
    stBitmap.u8BPP = 32;
    stBitmap.u16Height = m_u8Height;
    stBitmap.u16Width = m_u8Width;
    stBitmap.u16X = u16X_;
    stBitmap.u16Y = u16Y_;

    // use the supplied driver to render the tile as a bitmap
    pclDriver_->Bitmap(&stBitmap);
}
