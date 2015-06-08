#include "kerneltypes.h"
#include "graphics.h"
#include "draw.h"
#include "tiles.h"

#include "brpaddle.h"

//---------------------------------------------------------------------------
static K_UCHAR aucPaddleTileL[] =
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
static K_UCHAR aucPaddleTileR[] =
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
void BrPaddle::Init( K_UCHAR ucMode_ )
{
    if (ucMode_ & PADDLE_MODE_NORMAL)
    {
        m_ucWidth = PADDLE_WIDTH;
    }
    else if (ucMode_ & PADDLE_MODE_LONG)
    {
        m_ucWidth = PADDLE_WIDTH_LONG;
    }
    else
    {
        m_ucWidth = PADDLE_WIDTH;
    }

    m_ucMode = ucMode_;
}

//---------------------------------------------------------------------------
void BrPaddle::SetPosition(K_USHORT usX_)
{
    m_sX = usX_;
    m_sLastX = m_sX;
    m_bUpdate = true;
}

//---------------------------------------------------------------------------
void BrPaddle::MoveLeft(void)
{
    if (m_sVel > 0)
    {
        m_sVel = -2;
    }
    else if (m_sVel > (-1 * PADDLE_MAX_VELOCITY) )
    {
        m_sVel--;
    }

    m_sX += m_sVel;
    if (m_sX < PLAYFIELD_BORDER_WIDTH)
    {
        m_sX = PLAYFIELD_BORDER_WIDTH;
    }
}

//---------------------------------------------------------------------------
void BrPaddle::MoveRight(void)
{
    if (m_sVel < 0)
    {
        m_sVel = 2;
    }
    else if (m_sVel < PADDLE_MAX_VELOCITY)
    {
        m_sVel++;
    }

    m_sX += m_sVel;
    if (m_sX >= (PLAYFIELD_RIGHT_BORDER - m_ucWidth))
    {
        m_sX = PLAYFIELD_RIGHT_BORDER - m_ucWidth;
    }
}

//---------------------------------------------------------------------------
void BrPaddle::SetVelocity( K_SHORT sVel_ )
{
    m_sVel = sVel_;
    m_sX += m_sVel;
}

//---------------------------------------------------------------------------
void BrPaddle::Decel()
{
    if (m_sVel >= 1)
    {
        m_sVel--;
    }
    else if (m_sVel <= -1)
    {
        m_sVel++;
    }
    else
    {
        m_sVel = 0;
    }
    m_sX += m_sVel;
    if (m_sX >= (PLAYFIELD_RIGHT_BORDER - m_ucWidth))
    {
        m_sX = PLAYFIELD_RIGHT_BORDER - m_ucWidth;
    }
    if (m_sX < PLAYFIELD_BORDER_WIDTH)
    {
        m_sX = PLAYFIELD_BORDER_WIDTH;
    }
}

//---------------------------------------------------------------------------
void BrPaddle::Update(void)
{
    if (m_sX != m_sLastX)
    {
        m_bUpdate = true;
    }
}

//---------------------------------------------------------------------------
void BrPaddle::Clear(void)
{

    DrawRectangle_t stRect;

    stRect.bFill = true;
    stRect.usTop = PADDLE_YPOS;
    stRect.usBottom = PADDLE_YPOS + PADDLE_HEIGHT -1;
    stRect.usLeft = m_sX;
    stRect.usRight = m_sX + m_ucWidth;

    stRect.uLineColor = COLOR_BLACK;
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

    stLine.usX1 = m_sX + 4;
    stLine.usX2 = m_sX + m_ucWidth -4;
    for (K_UCHAR i = 0; i < PADDLE_HEIGHT; i++)
    {
        if (m_ucMode & PADDLE_MODE_LASER)
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

        stLine.usY1 = PADDLE_YPOS + i;
        stLine.usY2 = stLine.usY1;
        pclDisplay->Line(&stLine);
    }

    TileDef_t stTile;
    stTile.m_eFormat = TILE_2BPP;
    stTile.m_pucData = aucPaddleTileR;
    stTile.m_puPalette = auPaddlePalette;
    stTile.m_ucHeight = 6;
    stTile.m_ucWidth = 4;
    clMyTile.LoadTile(&stTile);
    clMyTile.Render(pclDisplay, m_sX, PADDLE_YPOS);

    stTile.m_pucData = aucPaddleTileL;
    clMyTile.LoadTile(&stTile);
    clMyTile.Render(pclDisplay, m_sX + m_ucWidth - 4, PADDLE_YPOS);

    DrawRectangle_t stRect;

    stRect.bFill = true;
    stRect.usTop = PADDLE_YPOS;
    stRect.usBottom = PADDLE_YPOS + PADDLE_HEIGHT -1;

    stRect.uLineColor = COLOR_BLACK;
    stRect.uFillColor = COLOR_BLACK;

    if (m_sVel > 0)
    {
        if (m_sX - m_sVel < 0)
        {
            stRect.usLeft = 0;
            stRect.usRight = m_sX - 1;
        }
        else
        {
            stRect.usLeft = m_sX - m_sVel;
            stRect.usRight = m_sX - 1;
        }
        pclDisplay->Rectangle(&stRect);

    }

    if (m_sVel < 0)
    {
        if (m_sX + m_ucWidth - m_sVel >= SCREEN_WIDTH)
        {
            stRect.usLeft = m_sX + m_ucWidth;
            stRect.usRight = SCREEN_WIDTH - 1;
        }
        else
        {
            stRect.usLeft = m_sX + m_ucWidth;
            stRect.usRight = (K_USHORT)(((K_SHORT)stRect.usLeft) - (m_sVel+1));
        }
        pclDisplay->Rectangle(&stRect);
    }

    m_sLastX = m_sX;
    m_bUpdate = false;
}
