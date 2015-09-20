#ifndef __HIGHSCORE_H__
#define __HIGHSCORE_H__

#include "kerneltypes.h"
#include "graphics.h"
#include "draw.h"
#include "tiles.h"

#include "brcommon.h"

#define NUM_HIGH_SCORES			(5)
//---------------------------------------------------------------------------
typedef struct
{
    union
    {
        struct
        {
            uint32_t u32Score;
            char acName[4];
        };
        uint8_t aucRawData[8];
    };

} HighScore_t;

//---------------------------------------------------------------------------
class HighScore
{
public:
    void CheckInit(void);

    void ReadScore(uint8_t u8Rank_, HighScore_t *pstScore_);

    void WriteScore(uint8_t u8Rank_, HighScore_t *pstScore_);

    bool IsHighScore(uint32_t u32Score_);

    void AddNewScore(HighScore_t *pstNew_);

private:
};

#endif
