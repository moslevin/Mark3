#include "kerneltypes.h"
#include "graphics.h"
#include "draw.h"
#include "tiles.h"
#include "highscore.h"
#include "kernelaware.h"

//!! flAVR doesn't support EEPROM writes currently, so use RAM-based scores.

//---------------------------------------------------------------------------
#ifdef __AVR__
    #include <avr/eeprom.h>
    #define READ_EE_BYTE(x) eeprom_read_byte(x)
    #define READ_EE_BLOCK(x, y, z) eeprom_read_block(x, y, z)
    #define WRITE_EE_BLOCK(x, y, z) eeprom_write_block(x, y, z)
#else
    #include <stdio.h>

    uint8_t aucEEData[64];
    #define READ_EE_BYTE(x) aucEEData[(K_ADDR)x]
    #define READ_EE_BLOCK(x,y,z) MemUtil::CopyMemory(x, (void*)&aucEEData[(K_ADDR)y], z)
    #define WRITE_EE_BLOCK(x,y,z) MemUtil::CopyMemory((void*)&aucEEData[(K_ADDR)y], x, z)
#endif

//---------------------------------------------------------------------------
void HighScore::CheckInit(void)
{
    uint8_t u8Val;

#if defined(__AVR__)
    uint8_t *pu8KeyVal = (uint8_t*)((K_ADDR)(NUM_HIGH_SCORES * sizeof(HighScore_t)));
    u8Val = eeprom_read_byte(pu8KeyVal);
    if (u8Val != 0x69)
    {
        eeprom_write_byte(pu8KeyVal, 0x69);
    }
#else
    u8Val = aucEEData[NUM_HIGH_SCORES * sizeof(HighScore_t)];
    if (u8Val != 0x69)
    {
        aucEEData[NUM_HIGH_SCORES * sizeof(HighScore_t)] = 0x69;
    }
#endif

    if (u8Val == 0x69)
    {
        return;
    }

    HighScore_t stScore;
    stScore.acName[0] = 'M';
    stScore.acName[1] = 'O';
    stScore.acName[2] = 'S';
    stScore.acName[3] = '\0';
    stScore.u32Score = 50000;
    WriteScore(0, &stScore);

    stScore.acName[0] = 'E';
    stScore.acName[1] = 'S';
    stScore.acName[2] = 'S';
    stScore.u32Score = 40000;
    WriteScore(1, &stScore);

    stScore.acName[0] = 'C';
    stScore.acName[1] = 'A';
    stScore.acName[2] = 'S';
    stScore.u32Score = 30000;
    WriteScore(2, &stScore);

    stScore.acName[0] = 'M';
    stScore.acName[1] = 'A';
    stScore.acName[2] = 'S';
    stScore.u32Score = 20000;
    WriteScore(3, &stScore);

    stScore.acName[0] = 'B';
    stScore.acName[1] = 'A';
    stScore.acName[2] = 'S';
    stScore.acName[3] = '\0';
    stScore.u32Score = 10000;
    WriteScore(4, &stScore);

}

//---------------------------------------------------------------------------
void HighScore::ReadScore(uint8_t u8Rank_, HighScore_t *pstScore_)
{

#if defined(__AVR__)
    uint8_t *pu8EEData;
    pu8EEData = (uint8_t*)((K_ADDR)u8Rank_ * sizeof(HighScore_t));
    eeprom_read_block((void*)pstScore_, (const void*)pu8EEData, sizeof(HighScore_t));
#else
    MemUtil::CopyMemory((void*)pstScore_, (const void*)&aucEEData[u8Rank_ * sizeof(HighScore_t)], sizeof(HighScore_t));
#endif
}

//---------------------------------------------------------------------------
void HighScore::WriteScore(uint8_t u8Rank_, HighScore_t *pstScore_)
{
#if defined(__AVR__)
    uint8_t *pu8EEData;
    pu8EEData = (uint8_t*)((K_ADDR)u8Rank_ * sizeof(HighScore_t));
    WRITE_EE_BLOCK((const void*)pstScore_, (void*)pu8EEData, sizeof(HighScore_t));
#else
    MemUtil::CopyMemory((void*)&aucEEData[u8Rank_ * sizeof(HighScore_t)], (const void*)pstScore_, sizeof(HighScore_t));
#endif
}

//---------------------------------------------------------------------------
bool HighScore::IsHighScore(uint32_t u32Score_)
{
    HighScore_t stScore;

    for (uint8_t i = 0; i < NUM_HIGH_SCORES; i++)
    {
        ReadScore(i, &stScore);
        if (u32Score_ > stScore.u32Score)
        {
            return true;
        }
    }
    return false;
}

//---------------------------------------------------------------------------
void HighScore::AddNewScore(HighScore_t *pstNew_)
{
    HighScore_t stScore;
    HighScore_t stMoveScore;

    for (uint8_t i = 0; i < NUM_HIGH_SCORES; i++)
    {
        ReadScore(i, &stScore);
        if (pstNew_->u32Score > stScore.u32Score)
        {            
            for (uint8_t j = NUM_HIGH_SCORES - 1; j > i; j--)
            {
                ReadScore(j - 1, &stMoveScore);
                WriteScore(j, &stMoveScore);
            }
            WriteScore(i, pstNew_);
            break;
        }
    }
}
