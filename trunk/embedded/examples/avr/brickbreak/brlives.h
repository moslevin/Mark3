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

    K_BOOL Decrement(void);

    void Increment(void);

    void Draw(void);

    K_UCHAR GetLives(void);

private:
    K_UCHAR m_ucLives;
    K_BOOL	m_bUpdate;
};


#endif

