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
    void SetPosition( uint16_t u16X_, uint16_t u16Y_ );

    void SetVelocity( int16_t s16VelX100_, int16_t s16VelY100_ );

    void ToggleDirX(void);

    void ToggleDirY(void);

    void Update(void);

    bool MoveNextPixel(uint16_t *pu16X_, uint16_t *pu16Y_);

    void SetX( uint16_t u16X_ );

    void SetY( uint16_t u16Y_ );

    int16_t GetVelX(void);

    int16_t GetVelY(void);

    void Clear(void);

    void Draw(void);

    int16_t GetX(void) { return (m_s16X100 + 50) / 100; }
    int16_t GetY(void) { return (m_s16Y100 + 50) / 100; }

private:
    int16_t m_s16X100;
    int16_t m_s16Y100;

    int16_t m_s16VelX100;
    int16_t m_s16VelY100;

    uint16_t m_u16PixelUpdates;
    uint16_t m_u16UpdatesLeft;

    uint16_t m_u16LastX;
    uint16_t m_u16LastY;
};


#endif

