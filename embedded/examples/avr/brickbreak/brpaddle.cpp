#include "kerneltypes.h"
#include "graphics.h"
#include "draw.h"
#include "tiles.h"

#include "brpaddle.h"

//---------------------------------------------------------------------------
static uint8_t aucPaddleTileL[] =
{
    0x15, // 0111 = 00 01 01 01 = 0x15
    0x6A, // 1222 = 01 10 10 10 = 0x6A
    0x6F, // 1233 = 01 10 11 11 = 0x6F
    0x6F, // 1233 = 01 10 11 11
    0x6A, // 1222
    0x15  // 0111
};

//---------------------------------------------------------------------------
static COLOR auPaddlePalette[4] =
{
    COLOR_BLACK,
    RGB_COLOR(MAX_RED/2, 0, 0),
    RGB_COLOR(3*MAX_RED/4, 0, 0),
    RGB_COLOR(MAX_RED, 0, 0)
};

//---------------------------------------------------------------------------
static uint8_t aucPaddleTileR[] =
{
    0x54,   // 1110 = 01 01 01 00 = 0x54
    0xA9,   // 2221 = 10 10 10 01 = 0xA9
    0xF9,   // 3321
    0xF9,   // 3321
    0xA9,   // 2221
    0x54    // 1110
};

#include <stdio.h>
//---------------------------------------------------------------------------
void BrPaddle::Init( uint8_t u8Mode_ )
{
    if (u8Mode_ & PADDLE_MODE_NORMAL)
    {
        m_u8Width = PADDLE_WIDTH;
    }
    else if (u8Mode_ & PADDLE_MODE_LONG)
    {
        m_u8Width = PADDLE_WIDTH_LONG;
    }
    else
    {
        m_u8Width = PADDLE_WIDTH;
    }

    m_u8Mode = u8Mode_;
}

//---------------------------------------------------------------------------
void BrPaddle::SetPosition(uint16_t u16X_)
{
    m_s16X = u16X_;
    m_sLastX = m_s16X;
    m_bUpdate = true;
}

//---------------------------------------------------------------------------
void BrPaddle::MoveLeft(void)
{
    if (m_s16Vel > 0)
    {
        m_s16Vel = -2;
    }
    else if (m_s16Vel > (-1 * PADDLE_MAX_VELOCITY) )
    {
        m_s16Vel--;
    }

    m_s16X += m_s16Vel;
    if (m_s16X < PLAYFIELD_BORDER_WIDTH)
    {
        m_s16X = PLAYFIELD_BORDER_WIDTH;
    }
}

//---------------------------------------------------------------------------
void BrPaddle::MoveRight(void)
{
    if (m_s16Vel < 0)
    {
        m_s16Vel = 2;
    }
    else if (m_s16Vel < PADDLE_MAX_VELOCITY)
    {
        m_s16Vel++;
    }

    m_s16X += m_s16Vel;
    if (m_s16X >= (PLAYFIELD_RIGHT_BORDER - m_u8Width))
    {
        m_s16X = PLAYFIELD_RIGHT_BORDER - m_u8Width;
    }
}

//---------------------------------------------------------------------------
void BrPaddle::SetVelocity( int16_t s16Vel_ )
{
    m_s16Vel = s16Vel_;
    m_s16X += m_s16Vel;
}

//---------------------------------------------------------------------------
void BrPaddle::Decel()
{
    if (m_s16Vel >= 1)
    {
        m_s16Vel--;
    }
    else if (m_s16Vel <= -1)
    {
        m_s16Vel++;
    }
    else
    {
        m_s16Vel = 0;
    }
    m_s16X += m_s16Vel;
    if (m_s16X >= (PLAYFIELD_RIGHT_BORDER - m_u8Width))
    {
        m_s16X = PLAYFIELD_RIGHT_BORDER - m_u8Width;
    }
    if (m_s16X < PLAYFIELD_BORDER_WIDTH)
    {
        m_s16X = PLAYFIELD_BORDER_WIDTH;
    }
}

//---------------------------------------------------------------------------
void BrPaddle::Update(void)
{
    if (m_s16X != m_sLastX)
    {
        m_bUpdate = true;
    }
}

//---------------------------------------------------------------------------
void BrPaddle::Clear(void)
{

    DrawRectangle_t stRect;

    stRect.bFill = true;
    stRect.u16Top = PADDLE_YPOS;
    stRect.u16Bottom = PADDLE_YPOS + PADDLE_HEIGHT -1;
    stRect.u16Left = m_s16X;
    stRect.u16Right = m_s16X + m_u8Width;

    stRect.u32ineColor = COLOR_BLACK;
    stRect.uFillColor = COLOR_BLACK;

    pclDisplay->Rectangle(&stRect);
    m_bUpdate = true;
}

//---------------------------------------------------------------------------
void BrPaddle::Draw(void)
{
    if (!m_bUpdate)
    {
        return;
    }

    DrawLine_t stLine;

    stLine.u16X1 = m_s16X + 4;
    stLine.u16X2 = m_s16X + m_u8Width -4;
    for (uint8_t i = 0; i < PADDLE_HEIGHT; i++)
    {
        if (m_u8Mode & PADDLE_MODE_LASER)
        {
            switch (i)
            {
                case 0: case 5: stLine.uColor = stLine.uColor = RGB_COLOR(MAX_RED/3,0,MAX_BLUE/3); break;
                case 1: case 4: stLine.uColor = RGB_COLOR(MAX_RED/2,0,MAX_BLUE/2); break;
                case 2: case 3: stLine.uColor = RGB_COLOR(MAX_RED,0,MAX_BLUE);  break;
                default: break;
            }
        }
        else
        {
            switch (i)
            {
                case 0: case 5: stLine.uColor = COLOR_GREY37; break;
                case 1: case 4: stLine.uColor = COLOR_GREY75; break;
                case 2: case 3: stLine.uColor = COLOR_WHITE; break;
                default: break;
            }
        }

        stLine.u16Y1 = PADDLE_YPOS + i;
        stLine.u16Y2 = stLine.u16Y1;
        pclDisplay->Line(&stLine);
    }

    TileDef_t stTile;
    stTile.m_eFormat = TILE_2BPP;
    stTile.m_pu8Data = aucPaddleTileR;
    stTile.m_puPalette = auPaddlePalette;
    stTile.m_u8Height = 6;
    stTile.m_u8Width = 4;
    clMyTile.LoadTile(&stTile);
    clMyTile.Render(pclDisplay, m_s16X, PADDLE_YPOS);

    stTile.m_pu8Data = aucPaddleTileL;
    clMyTile.LoadTile(&stTile);
    clMyTile.Render(pclDisplay, m_s16X + m_u8Width - 4, PADDLE_YPOS);

    DrawRectangle_t stRect;

    stRect.bFill = true;
    stRect.u16Top = PADDLE_YPOS;
    stRect.u16Bottom = PADDLE_YPOS + PADDLE_HEIGHT -1;

    stRect.u32ineColor = COLOR_BLACK;
    stRect.uFillColor = COLOR_BLACK;

    if (m_s16Vel > 0)
    {
        if (m_s16X - m_s16Vel < 0)
        {
            stRect.u16Left = 0;
            stRect.u16Right = m_s16X - 1;
        }
        else
        {
            stRect.u16Left = m_s16X - m_s16Vel;
            stRect.u16Right = m_s16X - 1;
        }
        pclDisplay->Rectangle(&stRect);

    }

    if (m_s16Vel < 0)
    {
        if (m_s16X + m_u8Width - m_s16Vel >= SCREEN_WIDTH)
        {
            stRect.u16Left = m_s16X + m_u8Width;
            stRect.u16Right = SCREEN_WIDTH - 1;
        }
        else
        {
            stRect.u16Left = m_s16X + m_u8Width;
            stRect.u16Right = (uint16_t)(((int16_t)stRect.u16Left) - (m_s16Vel+1));
        }
        pclDisplay->Rectangle(&stRect);
    }

    m_sLastX = m_s16X;
    m_bUpdate = false;
}
