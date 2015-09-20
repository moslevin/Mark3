#ifndef __BRLIVES_H__
#define __BRLIVES_H__

#include "kerneltypes.h"
#include "graphics.h"
#include "draw.h"
#include "tiles.h"

#include "brcommon.h"
//---------------------------------------------------------------------------
class BrLives
{
public:
    void Reset(void);

    bool Decrement(void);

    void Increment(void);

    void Draw(void);

    uint8_t GetLives(void);

private:
    uint8_t m_u8Lives;
    bool	m_bUpdate;
};


#endif

