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
            K_ULONG ulScore;
            K_CHAR acName[4];
        };
        K_UCHAR aucRawData[8];
    };

} HighScore_t;

//---------------------------------------------------------------------------
class HighScore
{
public:
    void CheckInit(void);

    void ReadScore(K_UCHAR ucRank_, HighScore_t *pstScore_);

    void WriteScore(K_UCHAR ucRank_, HighScore_t *pstScore_);

    K_BOOL IsHighScore(K_ULONG ulScore_);

    void AddNewScore(HighScore_t *pstNew_);

private:
};

#endif
