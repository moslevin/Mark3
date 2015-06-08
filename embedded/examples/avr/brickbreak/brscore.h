#ifndef __BRSCORE_H__
#define __BRSCORE_H__

#include "kerneltypes.h"
#include "graphics.h"
#include "draw.h"
#include "tiles.h"

#include "brcommon.h"

//---------------------------------------------------------------------------
class BrScore
{
public:
    void Reset(void);

    void AddScore(K_ULONG ulScore_);

    void Draw(void);

    K_ULONG GetScore(void);

private:
    K_ULONG m_ulScore;
    K_BOOL	m_bUpdate;
};

#endif
