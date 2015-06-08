#ifndef __BRPLAYFIELD_H__
#define __BRPLAYFIELD_H__

#include "kerneltypes.h"
#include "graphics.h"
#include "draw.h"
#include "tiles.h"
#include "brlevel.h"
#include "brcommon.h"
//---------------------------------------------------------------------------
#define BRICK_HIT_TOP		(0x01)
#define BRICK_HIT_BOTTOM    (0x02)
#define BRICK_HIT_LEFT      (0x04)
#define BRICK_HIT_RIGHT     (0x08)

#define BRICK_HIT_SPECIAL   (0x10)

//---------------------------------------------------------------------------
typedef enum
{
    SPECIAL_BONUS,
    SPECIAL_LONG,
    SPECIAL_SLOW,
    SPECIAL_1UP,
    SPECIAL_CATCH,
    SPECIAL_SKIP,
    SPECIAL_FIREPOWER,
    SPECIAL_NONE,
//----
    SPECIAL_COUNT
} Special_t;

//---------------------------------------------------------------------------
#define LEVEL_ACTION_PADDLE_HIT     (1)
#define LEVEL_ACTION_STEP           (2)
#define LEVEL_ACTION_WALL_HIT       (3)
#define LEVEL_ACTION_CIELING_HIT    (4)

//---------------------------------------------------------------------------
class BrPlayField
{
public:
    void LoadLevel( K_USHORT usLevel_ );

    void Draw(void);

    K_UCHAR ClearBrick(K_USHORT usX_, K_USHORT usY_);

    K_BOOL CheckBrickExists(K_USHORT usX_, K_USHORT usY_);

    K_BOOL IsPlayFieldEmpty(void);

    BrLevelType_t GetType() { return m_clLevelData.eLevelType; }

    void LevelAction( K_UCHAR ucFlags_ );

    void SetBonus( K_UCHAR ucRow_, K_UCHAR ucCol_ );

    void Tick();

    K_USHORT GetRandomData();

    K_BOOL HasHitCieling() { return m_bHitCieling; }

    Special_t SelectBonus(void);

    Special_t GetCurrentSpecial(void)
    {
        if (m_usSpecialTime)
        {           
            return m_eSpecial;
        }
        return SPECIAL_NONE;
    }

    void ResetBonus(void)
    {
        m_usSpecialTime = 0;
        m_eSpecial = SPECIAL_NONE;
    }

private:
    void SpecialTick(void)
    {
        if (m_usSpecialTime)
        {
            m_usSpecialTime--;
        } else {
            m_eSpecial = SPECIAL_NONE;
        }
    }
    K_BOOL SwapBrick(K_UCHAR ucRow1_, K_UCHAR ucCol1_, K_UCHAR ucRow2_, K_UCHAR ucCol2_ );

    void DrawBrick(K_UCHAR ucRow_, K_UCHAR ucCol_, K_UCHAR ucMode_);

    void ClearBrick_i(K_UCHAR ucRow_, K_UCHAR ucCol_);

    void ShowBonusString( Special_t eBonus_ );

    void ClearBonusString();

    BrLevelData_t m_clLevelData;

    K_USHORT m_usBricksLeft;

    K_USHORT m_usRandValue;
    K_USHORT m_usTickCount;

    K_UCHAR m_ucBonusTimer;

    K_UCHAR m_ucCol;
    K_UCHAR m_ucRow;

    K_UCHAR m_ucBonusCol;
    K_UCHAR m_ucBonusRow;

    K_USHORT m_usSpecialTime;
    Special_t m_eSpecial;

    K_BOOL m_bHitCieling;
};


#endif
