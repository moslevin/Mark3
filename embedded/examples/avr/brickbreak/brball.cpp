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
static uint8_t aucBallTile[] =
{
    0x14, //0110 = 00 01 01 00 = 0x14
    0x79, //1321 = 01 11 10 01 = 0x79
    0x69, //1221 = 01 10 10 01 = 0x69
    0x14  //0110 = 00 01 01 00 = 0x14
};

//---------------------------------------------------------------------------
void BrBall::SetPosition( uint16_t u16X_, uint16_t u16Y_ )
{
    m_u16LastX = m_s16X100 / 100;
    m_u16LastY = m_s16Y100 / 100;

    m_s16X100 = u16X_ * 100;
    m_s16Y100 = u16Y_ * 100;
}

//---------------------------------------------------------------------------
void BrBall::SetVelocity( int16_t s16VelX100_, int16_t s16VelY100_ )
{
    m_s16VelX100 = s16VelX100_;
    m_s16VelY100 = s16VelY100_;
}

//---------------------------------------------------------------------------
void BrBall::ToggleDirX(void)
{
    m_s16VelX100 *= -1;
}

//---------------------------------------------------------------------------
void BrBall::ToggleDirY(void)
{
    m_s16VelY100 *= -1;
}

//---------------------------------------------------------------------------
void BrBall::Update(void)
{
    // Compute number of pixel updates to take place this frame
    uint16_t u16XUpdates = (ABS(m_s16VelX100) + 50) / 100;
    uint16_t u16YUpdates = (ABS(m_s16VelY100) + 50) / 100;

    if (u16XUpdates > u16YUpdates)
    {
        m_u16PixelUpdates = u16XUpdates;
    }
    else
    {
        m_u16PixelUpdates = u16YUpdates;
    }
    m_u16UpdatesLeft = m_u16PixelUpdates;
}

//---------------------------------------------------------------------------
bool BrBall::MoveNextPixel(uint16_t *pu16X_, uint16_t *pu16Y_)
{
    if (!m_u16UpdatesLeft)
    {
        *pu16X_ = (uint16_t)(m_s16X100 / 100);
        *pu16Y_ = (uint16_t)(m_s16Y100 / 100);
        return false;
    }

    m_s16X100 +=  m_s16VelX100 / (int16_t)m_u16PixelUpdates;
    m_s16Y100 +=  m_s16VelY100 / (int16_t)m_u16PixelUpdates;

    m_u16UpdatesLeft--;

    *pu16X_ = (uint16_t)(m_s16X100 / 100);
    *pu16Y_ = (uint16_t)(m_s16Y100 / 100);
    return true;
}

//---------------------------------------------------------------------------
void BrBall::SetX( uint16_t u16X_ )
{
    m_s16X100 = (int16_t)(u16X_ * 100);
}

//---------------------------------------------------------------------------
void BrBall::SetY( uint16_t u16Y_ )
{
    m_s16Y100 = (int16_t)(u16Y_ * 100);
}

//---------------------------------------------------------------------------
int16_t BrBall::GetVelX()
{
    return m_s16VelX100;
}

//---------------------------------------------------------------------------
int16_t BrBall::GetVelY()
{
    return m_s16VelY100;
}

//---------------------------------------------------------------------------
void BrBall::Clear(void)
{
    DrawRectangle_t stRect;
    stRect.bFill = true;

    // Undraw previous rectangle
    stRect.uFillColor = COLOR_BLACK;
    stRect.u32ineColor = COLOR_BLACK;

    stRect.u16Top = m_u16LastY;
    stRect.u16Bottom = stRect.u16Top + BALL_SIZE - 1;
    stRect.u16Left = m_u16LastX;
    stRect.u16Right = stRect.u16Left + BALL_SIZE - 1;
    if (stRect.u16Left < PLAYFIELD_BORDER_WIDTH)
    {
        stRect.u16Left = PLAYFIELD_BORDER_WIDTH;
    }
    if (stRect.u16Right >= (SCREEN_WIDTH - PLAYFIELD_BORDER_WIDTH))
    {
        stRect.u16Right = SCREEN_WIDTH - PLAYFIELD_BORDER_WIDTH - 1;
    }
    if (stRect.u16Top <= PLAYFIELD_BORDER_WIDTH + HUD_HEIGHT)
    {
        stRect.u16Top = PLAYFIELD_BORDER_WIDTH + HUD_HEIGHT + 1;
    }

    pclDisplay->Rectangle(&stRect);
}

//---------------------------------------------------------------------------
void BrBall::Draw(void)
{
    TileDef_t stTile;
    stTile.m_eFormat = TILE_2BPP;
    stTile.m_pu8Data = aucBallTile;
    stTile.m_puPalette = auBallPalette;
    stTile.m_u8Width = 4;
    stTile.m_u8Height = 4;

    clMyTile.LoadTile(&stTile);
    clMyTile.Render(pclDisplay, m_s16X100/100, m_s16Y100/100 );

    m_u16LastX = m_s16X100 / 100;
    m_u16LastY = m_s16Y100 / 100;
}
