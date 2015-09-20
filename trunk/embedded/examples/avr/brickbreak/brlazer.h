#ifndef __BRLAZER_H__
#define __BRLAZER_H__

#include "kerneltypes.h"
#include "draw.h"
#include "brcommon.h"

//---------------------------------------------------------------------------
#define LAZER_HEIGHT            ( 16 )
#define LAZER_WIDTH             ( 1 )
#define LAZER_SPEED100          ( 700 )
#define LAZER_START_Y           ( PADDLE_YPOS - LAZER_HEIGHT )
#define LAZER_RECHARGE          ( 10 )

//---------------------------------------------------------------------------
class BrLazer
{
public:

    BrLazer()
    {
        m_bActive = false;
        m_u8Recharge = 0;
    }

    void Start( uint16_t u16X_ )
    {
        // If there's a lazer in play, then destroy it.
        if (m_u8Recharge)
        {
            return;
        }

        if (m_bActive)
        {
            return;
        }

        m_u16X100 = u16X_ * 100;
        m_u16Y100 = LAZER_START_Y * 100;

        m_bActive = true;
        Animate();
    }

    void Destroy()
    {
        if (!m_bActive)
        {
            return;
        }
        Clear();
        m_bActive = false;
        m_u8Recharge = LAZER_RECHARGE;
        m_u16X100 = 0;
        m_u16Y100 = 0;
    }

    uint16_t GetX() { return ((m_u16X100 + 50)/100); }

    uint16_t GetY() { return ((m_u16Y100 + 50)/100); }

    void Animate()
    {
        if (m_u8Recharge)
        {
            m_u8Recharge--;
            return;
        }
        if (!m_bActive)
        {
            return;
        }

        Clear();

        m_u16Y100 -= LAZER_SPEED100;

        Draw();
    }

    bool IsActive() { return m_bActive; }
private:

    void Clear()
    {
        DrawLine_t stLine;
        if (!m_bActive)
        {
            return;
        }
        stLine.u16X1 = (( m_u16X100 + 50 ) / 100 - 7);
        stLine.u16X2 = stLine.u16X1;
        stLine.u16Y1 = ( m_u16Y100 + 50 ) / 100;
        stLine.u16Y2 = stLine.u16Y1 + LAZER_HEIGHT - 1;
        stLine.uColor = COLOR_BLACK;
        pclDisplay->Line( &stLine );

        stLine.u16X1 += 13;
        stLine.u16X2 += 13;
        pclDisplay->Line( &stLine );
    }

    void Draw()
    {
        DrawLine_t stLine;

        stLine.u16X1 = (( m_u16X100 + 50 ) / 100 - 7);
        stLine.u16X2 = stLine.u16X1;
        stLine.u16Y1 = (m_u16Y100 + 50) / 100;
        stLine.u16Y2 = stLine.u16Y1 + LAZER_HEIGHT - 1;
        stLine.uColor = COLOR_RED;
        pclDisplay->Line( &stLine );

        stLine.u16X1 += 13;
        stLine.u16X2 += 13;
        pclDisplay->Line( &stLine );
    }

    bool   m_bActive;

    uint8_t  m_u8Recharge;
    uint16_t m_u16X100;
    uint16_t m_u16Y100;

    uint16_t m_u16Vel100;
};

#endif
