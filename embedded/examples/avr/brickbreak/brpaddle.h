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
	BrPaddle() { m_ucMode = PADDLE_MODE_NORMAL; m_ucWidth = PADDLE_WIDTH; }
    void Init( K_UCHAR ucMode_ );

    void SetPosition(K_USHORT usX_);
    void MoveLeft(void);

    void MoveRight(void);
    void SetVelocity( K_SHORT sVel_ );
    K_SHORT GetVelocity() { return m_sVel; }
    void Decel(void);
    void Update(void);

    void Draw(void);
    void Clear(void);
    K_UCHAR GetMode(void)   { return m_ucMode; }

    K_USHORT GetLeft(void) { return (K_USHORT)m_sX; }
    K_USHORT GetRight(void) { return (K_USHORT)m_sX + m_ucWidth - 1; }

    K_UCHAR GetWidth() { return m_ucWidth; }
private:
    K_SHORT m_sX;
    K_SHORT m_sLastX;
    K_SHORT m_sVel;
    K_BOOL	m_bUpdate;

    K_UCHAR m_ucMode;
    K_UCHAR m_ucWidth;
};

#endif
