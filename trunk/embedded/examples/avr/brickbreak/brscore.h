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

    void AddScore(uint32_t u32Score_);

    void Draw(void);

    uint32_t GetScore(void);

private:
    uint32_t m_u32Score;
    bool	m_bUpdate;
};

#endif
