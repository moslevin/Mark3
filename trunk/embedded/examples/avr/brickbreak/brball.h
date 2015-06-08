#ifndef __BRBALL_H__
#define __BRBALL_H__

#include "kerneltypes.h"
#include "graphics.h"
#include "draw.h"
#include "tiles.h"
#include "brcommon.h"

//---------------------------------------------------------------------------
class BrBall
{
public:
    void SetPosition( K_USHORT usX_, K_USHORT usY_ );

    void SetVelocity( K_SHORT sVelX100_, K_SHORT sVelY100_ );

    void ToggleDirX(void);

    void ToggleDirY(void);

    void Update(void);

    K_BOOL MoveNextPixel(K_USHORT *pusX_, K_USHORT *pusY_);

    void SetX( K_USHORT usX_ );

    void SetY( K_USHORT usY_ );

    K_SHORT GetVelX(void);

    K_SHORT GetVelY(void);

    void Clear(void);

    void Draw(void);

    K_SHORT GetX(void) { return (m_sX100 + 50) / 100; }
    K_SHORT GetY(void) { return (m_sY100 + 50) / 100; }

private:
    K_SHORT m_sX100;
    K_SHORT m_sY100;

    K_SHORT m_sVelX100;
    K_SHORT m_sVelY100;

    K_USHORT m_usPixelUpdates;
    K_USHORT m_usUpdatesLeft;

    K_USHORT m_usLastX;
    K_USHORT m_usLastY;
};


#endif

