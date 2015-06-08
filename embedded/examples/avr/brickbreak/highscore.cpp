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

    K_UCHAR aucEEData[64];
    #define READ_EE_BYTE(x) aucEEData[(K_ADDR)x]
    #define READ_EE_BLOCK(x,y,z) MemUtil::CopyMemory(x, (void*)&aucEEData[(K_ADDR)y], z)
    #define WRITE_EE_BLOCK(x,y,z) MemUtil::CopyMemory((void*)&aucEEData[(K_ADDR)y], x, z)
#endif

//---------------------------------------------------------------------------
void HighScore::CheckInit(void)
{
    K_UCHAR ucVal;

#if defined(__AVR__)
    K_UCHAR *pucKeyVal = (K_UCHAR*)((K_ADDR)(NUM_HIGH_SCORES * sizeof(HighScore_t)));
    ucVal = eeprom_read_byte(pucKeyVal);
    if (ucVal != 0x69)
    {
        eeprom_write_byte(pucKeyVal, 0x69);
    }
#else
    ucVal = aucEEData[NUM_HIGH_SCORES * sizeof(HighScore_t)];
    if (ucVal != 0x69)
    {
        aucEEData[NUM_HIGH_SCORES * sizeof(HighScore_t)] = 0x69;
    }
#endif

    if (ucVal == 0x69)
    {
        return;
    }

    HighScore_t stScore;
    stScore.acName[0] = 'M';
    stScore.acName[1] = 'O';
    stScore.acName[2] = 'S';
    stScore.acName[3] = '\0';
    stScore.ulScore = 50000;
    WriteScore(0, &stScore);

    stScore.acName[0] = 'E';
    stScore.acName[1] = 'S';
    stScore.acName[2] = 'S';
    stScore.ulScore = 40000;
    WriteScore(1, &stScore);

    stScore.acName[0] = 'C';
    stScore.acName[1] = 'A';
    stScore.acName[2] = 'S';
    stScore.ulScore = 30000;
    WriteScore(2, &stScore);

    stScore.acName[0] = 'M';
    stScore.acName[1] = 'A';
    stScore.acName[2] = 'S';
    stScore.ulScore = 20000;
    WriteScore(3, &stScore);

    stScore.acName[0] = 'B';
    stScore.acName[1] = 'A';
    stScore.acName[2] = 'S';
    stScore.acName[3] = '\0';
    stScore.ulScore = 10000;
    WriteScore(4, &stScore);

}

//---------------------------------------------------------------------------
void HighScore::ReadScore(K_UCHAR ucRank_, HighScore_t *pstScore_)
{

#if defined(__AVR__)
    K_UCHAR *pucEEData;
    pucEEData = (K_UCHAR*)((K_ADDR)ucRank_ * sizeof(HighScore_t));
    eeprom_read_block((void*)pstScore_, (const void*)pucEEData, sizeof(HighScore_t));
#else
    MemUtil::CopyMemory((void*)pstScore_, (const void*)&aucEEData[ucRank_ * sizeof(HighScore_t)], sizeof(HighScore_t));
#endif
}

//---------------------------------------------------------------------------
void HighScore::WriteScore(K_UCHAR ucRank_, HighScore_t *pstScore_)
{
#if defined(__AVR__)
    K_UCHAR *pucEEData;
    pucEEData = (K_UCHAR*)((K_ADDR)ucRank_ * sizeof(HighScore_t));
    WRITE_EE_BLOCK((const void*)pstScore_, (void*)pucEEData, sizeof(HighScore_t));
#else
    MemUtil::CopyMemory((void*)&aucEEData[ucRank_ * sizeof(HighScore_t)], (const void*)pstScore_, sizeof(HighScore_t));
#endif
}

//---------------------------------------------------------------------------
K_BOOL HighScore::IsHighScore(K_ULONG ulScore_)
{
    HighScore_t stScore;

    for (K_UCHAR i = 0; i < NUM_HIGH_SCORES; i++)
    {
        ReadScore(i, &stScore);
        if (ulScore_ > stScore.ulScore)
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

    for (K_UCHAR i = 0; i < NUM_HIGH_SCORES; i++)
    {
        ReadScore(i, &stScore);
        if (pstNew_->ulScore > stScore.ulScore)
        {            
            for (K_UCHAR j = NUM_HIGH_SCORES - 1; j > i; j--)
            {
                ReadScore(j - 1, &stMoveScore);
                WriteScore(j, &stMoveScore);
            }
            WriteScore(i, pstNew_);
            break;
        }
    }
}
