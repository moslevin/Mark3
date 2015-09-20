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
    char *acLevelName;
    uint16_t au16Data[LEVEL_NUM_ROWS];
    BrLevelType_t eLevelType;
} BrLevelData_t;

//---------------------------------------------------------------------------
class BrLevel
{
public:
    void Reset(void);

    void NextLevel(void);

    void Draw(void);

    uint8_t GetLevel(void);

private:
    uint8_t	m_u8Level;
    bool	m_bUpdate;
};

#endif
