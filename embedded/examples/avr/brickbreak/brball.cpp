#include "kerneltypes.h"
#include "graphics.h"
#include "draw.h"
#include "tiles.h"

#include "brball.h"

//---------------------------------------------------------------------------
static COLOR auBallPalette[4] =
{
    COLOR_BLACK,
    COLOR_GREY50,
    COLOR_GREY75,
    COLOR_WHITE
};

//---------------------------------------------------------------------------
static K_UCHAR aucBallTile[] =
{
    0x14, //0110 = 00 01 01 00 = 0x14
    0x79, //1321 = 01 11 10 01 = 0x79
    0x69, //1221 = 01 10 10 01 = 0x69
    0x14  //0110 = 00 01 01 00 = 0x14
};

//---------------------------------------------------------------------------
void BrBall::SetPosition( K_USHORT usX_, K_USHORT usY_ )
{
    m_usLastX = m_sX100 / 100;
    m_usLastY = m_sY100 / 100;

    m_sX100 = usX_ * 100;
    m_sY100 = usY_ * 100;
}

//---------------------------------------------------------------------------
void BrBall::SetVelocity( K_SHORT sVelX100_, K_SHORT sVelY100_ )
{
    m_sVelX100 = sVelX100_;
    m_sVelY100 = sVelY100_;
}

//---------------------------------------------------------------------------
void BrBall::ToggleDirX(void)
{
    m_sVelX100 *= -1;
}

//---------------------------------------------------------------------------
void BrBall::ToggleDirY(void)
{
    m_sVelY100 *= -1;
}

//---------------------------------------------------------------------------
void BrBall::Update(void)
{
    // Compute number of pixel updates to take place this frame
    K_USHORT usXUpdates = (ABS(m_sVelX100) + 50) / 100;
    K_USHORT usYUpdates = (ABS(m_sVelY100) + 50) / 100;

    if (usXUpdates > usYUpdates)
    {
        m_usPixelUpdates = usXUpdates;
    }
    else
    {
        m_usPixelUpdates = usYUpdates;
    }
    m_usUpdatesLeft = m_usPixelUpdates;
}

//---------------------------------------------------------------------------
K_BOOL BrBall::MoveNextPixel(K_USHORT *pusX_, K_USHORT *pusY_)
{
    if (!m_usUpdatesLeft)
    {
        *pusX_ = (K_USHORT)(m_sX100 / 100);
        *pusY_ = (K_USHORT)(m_sY100 / 100);
        return false;
    }

    m_sX100 +=  m_sVelX100 / (K_SHORT)m_usPixelUpdates;
    m_sY100 +=  m_sVelY100 / (K_SHORT)m_usPixelUpdates;

    m_usUpdatesLeft--;

    *pusX_ = (K_USHORT)(m_sX100 / 100);
    *pusY_ = (K_USHORT)(m_sY100 / 100);
    return true;
}

//---------------------------------------------------------------------------
void BrBall::SetX( K_USHORT usX_ )
{
    m_sX100 = (K_SHORT)(usX_ * 100);
}

//---------------------------------------------------------------------------
void BrBall::SetY( K_USHORT usY_ )
{
    m_sY100 = (K_SHORT)(usY_ * 100);
}

//---------------------------------------------------------------------------
K_SHORT BrBall::GetVelX()
{
    return m_sVelX100;
}

//---------------------------------------------------------------------------
K_SHORT BrBall::GetVelY()
{
    return m_sVelY100;
}

//---------------------------------------------------------------------------
void BrBall::Clear(void)
{
    DrawRectangle_t stRect;
    stRect.bFill = true;

    // Undraw previous rectangle
    stRect.uFillColor = COLOR_BLACK;
    stRect.uLineColor = COLOR_BLACK;

    stRect.usTop = m_usLastY;
    stRect.usBottom = stRect.usTop + BALL_SIZE - 1;
    stRect.usLeft = m_usLastX;
    stRect.usRight = stRect.usLeft + BALL_SIZE - 1;
    if (stRect.usLeft < PLAYFIELD_BORDER_WIDTH)
    {
        stRect.usLeft = PLAYFIELD_BORDER_WIDTH;
    }
    if (stRect.usRight >= (SCREEN_WIDTH - PLAYFIELD_BORDER_WIDTH))
    {
        stRect.usRight = SCREEN_WIDTH - PLAYFIELD_BORDER_WIDTH - 1;
    }
    if (stRect.usTop <= PLAYFIELD_BORDER_WIDTH + HUD_HEIGHT)
    {
        stRect.usTop = PLAYFIELD_BORDER_WIDTH + HUD_HEIGHT + 1;
    }

    pclDisplay->Rectangle(&stRect);
}

//---------------------------------------------------------------------------
void BrBall::Draw(void)
{
    TileDef_t stTile;
    stTile.m_eFormat = TILE_2BPP;
    stTile.m_pucData = aucBallTile;
    stTile.m_puPalette = auBallPalette;
    stTile.m_ucWidth = 4;
    stTile.m_ucHeight = 4;

    clMyTile.LoadTile(&stTile);
    clMyTile.Render(pclDisplay, m_sX100/100, m_sY100/100 );

    m_usLastX = m_sX100 / 100;
    m_usLastY = m_sY100 / 100;
}
