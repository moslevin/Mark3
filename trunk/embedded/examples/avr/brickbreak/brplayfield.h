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
    void LoadLevel( uint16_t u16Level_ );

    void Draw(void);

    uint8_t ClearBrick(uint16_t u16X_, uint16_t u16Y_);

    bool CheckBrickExists(uint16_t u16X_, uint16_t u16Y_);

    bool IsPlayFieldEmpty(void);

    BrLevelType_t GetType() { return m_clLevelData.eLevelType; }

    void LevelAction( uint8_t u8Flags_ );

    void SetBonus( uint8_t u8Row_, uint8_t u8Col_ );

    void Tick();

    uint16_t GetRandomData();

    bool HasHitCieling() { return m_bHitCieling; }

    Special_t SelectBonus(void);

    Special_t GetCurrentSpecial(void)
    {
        if (m_u16SpecialTime)
        {           
            return m_eSpecial;
        }
        return SPECIAL_NONE;
    }

    void ResetBonus(void)
    {
        m_u16SpecialTime = 0;
        m_eSpecial = SPECIAL_NONE;
    }

private:
    void SpecialTick(void)
    {
        if (m_u16SpecialTime)
        {
            m_u16SpecialTime--;
        } else {
            m_eSpecial = SPECIAL_NONE;
        }
    }
    bool SwapBrick(uint8_t u8Row1_, uint8_t u8Col1_, uint8_t u8Row2_, uint8_t u8Col2_ );

    void DrawBrick(uint8_t u8Row_, uint8_t u8Col_, uint8_t u8Mode_);

    void ClearBrick_i(uint8_t u8Row_, uint8_t u8Col_);

    void ShowBonustring( Special_t eBonus_ );

    void ClearBonustring();

    BrLevelData_t m_clLevelData;

    uint16_t m_u16BricksLeft;

    uint16_t m_u16RandValue;
    uint16_t m_u16TickCount;

    uint8_t m_u8Bonusimer;

    uint8_t m_u8Col;
    uint8_t m_u8Row;

    uint8_t m_u8Bonusol;
    uint8_t m_u8Bonusow;

    uint16_t m_u16SpecialTime;
    Special_t m_eSpecial;

    bool m_bHitCieling;
};


#endif
