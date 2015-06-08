#ifndef __BRLEVEL_H__
#define __BRLEVEL_H__

#include "kerneltypes.h"
#include "graphics.h"
#include "draw.h"
#include "tiles.h"
#include "brcommon.h"

//---------------------------------------------------------------------------
typedef enum
{
    BR_LEVEL_NORMAL,
    BR_LEVEL_BREAKTHROUGH,
    BR_LEVEL_ROULETTE,
} BrLevelType_t;

//---------------------------------------------------------------------------
typedef struct
{
    K_CHAR *acLevelName;
    K_USHORT ausData[LEVEL_NUM_ROWS];
    BrLevelType_t eLevelType;
} BrLevelData_t;

//---------------------------------------------------------------------------
class BrLevel
{
public:
    void Reset(void);

    void NextLevel(void);

    void Draw(void);

    K_UCHAR GetLevel(void);

private:
    K_UCHAR	m_ucLevel;
    K_BOOL	m_bUpdate;
};

#endif
