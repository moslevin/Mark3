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
        m_ucRecharge = 0;
    }

    void Start( K_USHORT usX_ )
    {
        // If there's a lazer in play, then destroy it.
        if (m_ucRecharge)
        {
            return;
        }

        if (m_bActive)
        {
            return;
        }

        m_usX100 = usX_ * 100;
        m_usY100 = LAZER_START_Y * 100;

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
        m_ucRecharge = LAZER_RECHARGE;
        m_usX100 = 0;
        m_usY100 = 0;
    }

    K_USHORT GetX() { return ((m_usX100 + 50)/100); }

    K_USHORT GetY() { return ((m_usY100 + 50)/100); }

    void Animate()
    {
        if (m_ucRecharge)
        {
            m_ucRecharge--;
            return;
        }
        if (!m_bActive)
        {
            return;
        }

        Clear();

        m_usY100 -= LAZER_SPEED100;

        Draw();
    }

    K_BOOL IsActive() { return m_bActive; }
private:

    void Clear()
    {
        DrawLine_t stLine;
        if (!m_bActive)
        {
            return;
        }
        stLine.usX1 = (( m_usX100 + 50 ) / 100 - 7);
        stLine.usX2 = stLine.usX1;
        stLine.usY1 = ( m_usY100 + 50 ) / 100;
        stLine.usY2 = stLine.usY1 + LAZER_HEIGHT - 1;
        stLine.uColor = COLOR_BLACK;
        pclDisplay->Line( &stLine );

        stLine.usX1 += 13;
        stLine.usX2 += 13;
        pclDisplay->Line( &stLine );
    }

    void Draw()
    {
        DrawLine_t stLine;

        stLine.usX1 = (( m_usX100 + 50 ) / 100 - 7);
        stLine.usX2 = stLine.usX1;
        stLine.usY1 = (m_usY100 + 50) / 100;
        stLine.usY2 = stLine.usY1 + LAZER_HEIGHT - 1;
        stLine.uColor = COLOR_RED;
        pclDisplay->Line( &stLine );

        stLine.usX1 += 13;
        stLine.usX2 += 13;
        pclDisplay->Line( &stLine );
    }

    K_BOOL   m_bActive;

    K_UCHAR  m_ucRecharge;
    K_USHORT m_usX100;
    K_USHORT m_usY100;

    K_USHORT m_usVel100;
};

#endif
