#ifndef __BRPADDLE_H__
#define __BRPADDLE_H__

#include "kerneltypes.h"
#include "graphics.h"
#include "draw.h"
#include "tiles.h"

#include "brcommon.h"

//---------------------------------------------------------------------------
#define PADDLE_MODE_NORMAL      (0x01)
#define PADDLE_MODE_LONG        (0x02)
#define PADDLE_MODE_LASER       (0x04)

//---------------------------------------------------------------------------
class BrPaddle
{
public:
	BrPaddle() { m_u8Mode = PADDLE_MODE_NORMAL; m_u8Width = PADDLE_WIDTH; }
    void Init( uint8_t u8Mode_ );

    void SetPosition(uint16_t u16X_);
    void MoveLeft(void);

    void MoveRight(void);
    void SetVelocity( int16_t s16Vel_ );
    int16_t GetVelocity() { return m_s16Vel; }
    void Decel(void);
    void Update(void);

    void Draw(void);
    void Clear(void);
    uint8_t GetMode(void)   { return m_u8Mode; }

    uint16_t GetLeft(void) { return (uint16_t)m_s16X; }
    uint16_t GetRight(void) { return (uint16_t)m_s16X + m_u8Width - 1; }

    uint8_t GetWidth() { return m_u8Width; }
private:
    int16_t m_s16X;
    int16_t m_sLastX;
    int16_t m_s16Vel;
    bool	m_bUpdate;

    uint8_t m_u8Mode;
    uint8_t m_u8Width;
};

#endif
