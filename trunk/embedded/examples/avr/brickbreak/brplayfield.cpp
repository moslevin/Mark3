#include "kerneltypes.h"
#include "graphics.h"
#include "draw.h"
#include "tiles.h"
#include "brplayfield.h"

// Levels are 12x10
#define NUM_LEVELS				(32)

//---------------------------------------------------------------------------
#if defined(__AVR__)
    #include <avr/pgmspace.h>
    #define LEVEL_MEM				PROGMEM
    #define READ_LEVEL_MEM(x)		(pgm_read_dword(x))
#else
    #include <stdio.h>
    #include <stdlib.h>
    #define LEVEL_MEM
    #define READ_LEVEL_MEM(x)		(*x)
#endif

//---------------------------------------------------------------------------
const K_USHORT ausLevelData[][ LEVEL_NUM_ROWS ] LEVEL_MEM =
{
    {
        0xFFF,
        0xFFF,
        0x666,
        0x666,
        0x666,
        0x666,
        0x666,
        0x666,
        0xFFF,
        0xFFF
    },
    {
        0xF81,
        0xF18,
        0x7E,
        0x9FF,
        0xFFF,
        0xFFF,
        0xFE7,
        0xF81,
        0x63C,
        0x7E
    },
    {
        0x1,
        0xFFD,
        0x805,
        0xBF5,
        0xA15,
        0xAF5,
        0xA05,
        0xBFD,
        0x801,
        0xFFF
    },
    {
        0x198,
        0x198,
        0x3FC,
        0x3FC,
        0x7FE,
        0x7FE,
        0x3FC,
        0x3FC,
        0x198,
        0x198
    },
    {
        0xF0,
        0x1F8,
        0x3FC,
        0x36C,
        0x3FC,
        0x7FE,
        0x7FE,
        0x7FE,
        0x108,
        0x294
    },
    {
        0x36C,
        0x76E,
        0xC63,
        0x861,
        0xBFD,
        0xBFD,
        0x861,
        0xC63,
        0x76E,
        0x36C
    },
    {
        0x39C,
        0x7FE,
        0x7FE,
        0x7FE,
        0x7FE,
        0x3FC,
        0x1F8,
        0xF0,
        0x60,
        0x0
    },
    {
        0x0,
        0x7B6,
        0xBB7,
        0xBB6,
        0xBB6,
        0x7B6,
        0x3F6,
        0x36F,
        0x0,
        0xFFF
    },
    {
        0xF6F,
        0xE67,
        0xEF7,
        0xF0,
        0xFFF,
        0xFFF,
        0xF0,
        0xEF7,
        0xE67,
        0xF6F
    },
    {
        0xF0,
        0x3FC,
        0x7FE,
        0x666,
        0x7FE,
        0x7FE,
        0x6F6,
        0x70E,
        0x3FC,
        0xF0
    },
    {
        0xC63,
        0x666,
        0x666,
        0x76E,
        0x30C,
        0xDFB,
        0x30C,
        0x76E,
        0x666,
        0xC63
    },
    {
        0xFFF,
        0xFFF,
        0xFFF,
        0x7,
        0xFF,
        0xFF,
        0x7,
        0xFFF,
        0xFFF,
        0xFFF
    },
    {
        0xFC1,
        0xFE3,
        0x877,
        0x877,
        0xF7F,
        0xF5D,
        0x85D,
        0x85D,
        0xFDD,
        0xFDD
    },
    {
        0xFFF,
        0x0,
        0xFFF,
        0x0,
        0xFFF,
        0x0,
        0xFFF,
        0x0,
        0xFFF,
        0x0
    },
    {
        0xC03,
        0xC63,
        0xD6B,
        0xCF3,
        0xDFB,
        0xCF3,
        0xCF3,
        0xDFB,
        0xD6B,
        0xC63
    },
    {
        0xFFF,
        0x7FF,
        0x383,
        0x1C3,
        0xE3,
        0xC70,
        0xC38,
        0xC1C,
        0xFFE,
        0xFFF
    },
    {
        0xC,
        0x1E,
        0xC,
        0x0,
        0x7FE,
        0xFFF,
        0xFFF,
        0xFFF,
        0xFFF,
        0x7FE,
    },
    {
        0x1C7,
        0x5D7,
        0x1C7,
        0xE38,
        0xEBA,
        0xE38,
        0x1C7,
        0x5D7,
        0x1C7,
        0xFFF
    },
    {
        0x444,
        0x666,
        0x626,
        0xEEE,
        0xFFF,
        0xFFF,
        0x777,
        0x666,
        0x666,
        0x222
    },
    {
        0x3DC,
        0x7FE,
        0xFFF,
        0xF0F,
        0xE07,
        0x6F6,
        0xF0,
        0x60,
        0x0,
        0x0
    },
    {
        0x0EEE,
        0x0777,
        0x0BBB,
        0x0DDD,
        0x0EEE,
        0x0777,
        0x0BBB,
        0x0DDD,
        0x0EEE,
        0x0777
    },
    {
        0x07FE,
        0x03FC,
        0x09F9,
        0x0CF3,
        0x0E67,
        0x0F0F,
        0x0E67,
        0x0CF3,
        0x09F9,
        0x03FC
    },
    {
        0x0FFF,
        0x0999,
        0x0999,
        0x0999,
        0x0FFF,
        0x0666,
        0x0666,
        0x0666,
        0x0FFF,
        0
    },
    {
        0x0F0F,
        0x0F0F,
        0x0F9F,
        0x0F9F,
        0x0FFF,
        0x0FFF,
        0x0F6F,
        0x0F6F,
        0x0F0F,
        0x0F0F
    },
    {
        0x0007,
        0x000F,
        0x001F,
        0x003F,
        0x007F,
        0x00FF,
        0x01FF,
        0x03FF,
        0x07FF,
        0x0FFF
    },
    {
        0x0FFF,
        0x0801,
        0x0BFD,
        0x0FFF,
        0,
        0x0FFF,
        0x0BFD,
        0x0801,
        0x0FFF,
        0
    },
    {
        0x0FFF,
        0x0FFF,
        0x0FFF,
        0x0FFF,
        0x0FFF,
        0x0FFF,
        0x0FFF,
        0x0FFF,
        0x0FFF,
        0x0FFF
    },
    {
        0x0BFD,
        0x0BFD,
        0x0BFD,
        0x0BFD,
        0,
        0,
        0x0BFD,
        0x0BFD,
        0x0BFD,
        0x0BFD
    },
    {
        0x0FFF,
        0x0FFF,
        0x0C03,
        0x0C03,
        0x0CF3,
        0x0CF3,
        0x0C03,
        0x0C03,
        0x0FFF,
        0x0FFF
    },
    {
        0x0AAA,
        0x0555,
        0x0AAA,
        0x0555,
        0x0AAA,
        0x0555,
        0x0AAA,
        0x0555,
        0x0AAA,
        0x0555
    },
    {
        0x0FFF,
        0x0FFF,
        0x0000,
        0x0FFF,
        0x0FFF,
        0x0FFF,
        0x0000,
        0x0FFF,
        0x0FFF
    },
    {
        0x0FFF,
        0x0FFF,
        0x0F0F,
        0x0F0F,
        0x03FC,
        0x03FC,
        0x0F0F,
        0x0FFF,
        0x0FFF
    }
};

static BrLevelType_t aeLevelTypes[ NUM_LEVELS ] =
{
    BR_LEVEL_NORMAL,
    BR_LEVEL_NORMAL,
    BR_LEVEL_BREAKTHROUGH,
    BR_LEVEL_ROULETTE,
    BR_LEVEL_NORMAL,
    BR_LEVEL_NORMAL,
    BR_LEVEL_BREAKTHROUGH,
    BR_LEVEL_NORMAL,
    BR_LEVEL_ROULETTE,
    BR_LEVEL_NORMAL,
    BR_LEVEL_BREAKTHROUGH,
    BR_LEVEL_NORMAL,
    BR_LEVEL_NORMAL,
    BR_LEVEL_ROULETTE,
    BR_LEVEL_BREAKTHROUGH,
    BR_LEVEL_NORMAL,
    BR_LEVEL_ROULETTE,
    BR_LEVEL_NORMAL,
    BR_LEVEL_BREAKTHROUGH,
    BR_LEVEL_ROULETTE,
    BR_LEVEL_NORMAL,
    BR_LEVEL_NORMAL,
    BR_LEVEL_BREAKTHROUGH,
    BR_LEVEL_NORMAL,
    BR_LEVEL_ROULETTE,
    BR_LEVEL_NORMAL,
    BR_LEVEL_BREAKTHROUGH,
    BR_LEVEL_NORMAL,
    BR_LEVEL_NORMAL,
    BR_LEVEL_ROULETTE,
    BR_LEVEL_BREAKTHROUGH,
    BR_LEVEL_NORMAL
};

//---------------------------------------------------------------------------
void BrPlayField::LoadLevel( K_USHORT usLevel_ )
{
    K_USHORT usLevelIdx = usLevel_ % NUM_LEVELS;
    const K_USHORT *pusData = &(ausLevelData[usLevelIdx][0]);

    m_bHitCieling = false;

    m_usBricksLeft = 0;
    m_ucBonusTimer = 0;
    m_usRandValue = 1;
    m_ucCol = 0;
    m_ucRow = 0;

    for (K_UCHAR i = 0; i < LEVEL_NUM_ROWS; i++)
    {
        m_clLevelData.ausData[i] = READ_LEVEL_MEM(pusData++);

        K_USHORT j = 1;
        while (j < 0x1000)
        {
            if (m_clLevelData.ausData[i] & j)
            {
                m_usBricksLeft++;
            }
            j <<= 1;
        }
    }
    m_clLevelData.eLevelType = aeLevelTypes[ usLevelIdx ];
}

//---------------------------------------------------------------------------
void BrPlayField::Draw(void)
{
    DrawRectangle_t stRect;

    stRect.bFill = true;
    stRect.uFillColor = COLOR_BLUE;
    stRect.uLineColor = COLOR_BLUE;
    stRect.usLeft = 0;
    stRect.usRight = PLAYFIELD_BORDER_WIDTH - 1;
    stRect.usTop = HUD_HEIGHT;
    stRect.usBottom = PADDLE_YPOS;

    pclDisplay->Rectangle(&stRect);

    stRect.usLeft = PLAYFIELD_RIGHT_BORDER;
    stRect.usRight = PLAYFIELD_RIGHT_BORDER + PLAYFIELD_BORDER_WIDTH - 1;

    pclDisplay->Rectangle(&stRect);

    stRect.usTop = HUD_HEIGHT;
    stRect.usBottom = HUD_HEIGHT + PLAYFIELD_BORDER_HEIGHT;
    stRect.usLeft = 0;
    stRect.usRight = SCREEN_WIDTH - 1;

    pclDisplay->Rectangle(&stRect);

    stRect.usTop = HUD_HEIGHT + PLAYFIELD_BORDER_HEIGHT;
    stRect.usBottom = stRect.usTop + (LEVEL_NUM_ROWS * BRICK_HEIGHT) + PLAYFIELD_BRICK_MARGIN;
    stRect.usLeft = PLAYFIELD_LEFT_BORDER;
    stRect.usRight = PLAYFIELD_RIGHT_BORDER-1;
    stRect.uFillColor = COLOR_BLACK;
    stRect.uLineColor = COLOR_BLACK;
    stRect.bFill = true;

    pclDisplay->Rectangle(&stRect);

    for (K_UCHAR i = 0; i < LEVEL_NUM_ROWS; i++)
    {
        for (K_UCHAR j = 0; j < LEVEL_NUM_COLS; j++)
        {
            if (m_clLevelData.ausData[i] & (1 << (K_USHORT)j))
            {
                DrawBrick( i, j, 0 );
            }
        }
    }
}

//---------------------------------------------------------------------------
void BrPlayField::ClearBrick_i(K_UCHAR ucRow_, K_UCHAR ucCol_)
{
    DrawRectangle_t stRect;
    stRect.usTop = (PLAYFIELD_BRICK_MARGIN + HUD_HEIGHT + PLAYFIELD_BORDER_HEIGHT) + (ucRow_ * BRICK_HEIGHT);
    stRect.usBottom = stRect.usTop + BRICK_HEIGHT - 1;
    stRect.usLeft = PLAYFIELD_BORDER_WIDTH + (ucCol_ * BRICK_WIDTH);
    stRect.usRight = stRect.usLeft + BRICK_WIDTH - 1;
    stRect.bFill = true;
    stRect.uFillColor = COLOR_BLACK;
    stRect.uLineColor = COLOR_BLACK;
    pclDisplay->Rectangle(&stRect);

    if ((m_ucBonusRow != 0) && (m_ucBonusCol != 0))
    {
        if ((ucRow_ == (m_ucBonusRow - 1)) && (ucCol_ == (m_ucBonusCol - 1)))
        {
            m_ucBonusCol = 0;
            m_ucBonusRow = 0;
        }
    }
}

//---------------------------------------------------------------------------
K_UCHAR BrPlayField::ClearBrick(K_USHORT usX_, K_USHORT usY_)
{
    //!! Must ensure the "CheckBrick" test has passed before calling.

    // Convert X/Y to row/column
    K_UCHAR ucCol = (usX_ - PLAYFIELD_BORDER_WIDTH) / BRICK_WIDTH;
    K_UCHAR ucRow = (usY_ - (PLAYFIELD_BRICK_MARGIN + HUD_HEIGHT + PLAYFIELD_BORDER_HEIGHT)) / BRICK_HEIGHT;

    K_UCHAR ucRet = 0;

    if (usY_ == ((PLAYFIELD_BRICK_MARGIN + HUD_HEIGHT + PLAYFIELD_BORDER_HEIGHT) + (ucRow * BRICK_HEIGHT)))
    {
        ucRet |= BRICK_HIT_TOP;
    }
    else if (usY_ == ((PLAYFIELD_BRICK_MARGIN + HUD_HEIGHT + PLAYFIELD_BORDER_HEIGHT - 1) + ((ucRow+1) * BRICK_HEIGHT)))
    {
        ucRet |= BRICK_HIT_BOTTOM;
    }
    else if (usX_ == (PLAYFIELD_BORDER_WIDTH + (ucCol * BRICK_WIDTH)))
    {
        ucRet |= BRICK_HIT_LEFT;
    }
    else if (usX_ == ((PLAYFIELD_BORDER_WIDTH-1) + ((ucCol+1) * BRICK_WIDTH)))
    {
        ucRet |= BRICK_HIT_RIGHT;
    }

    // Hit is in the middle of a brick?  Assume a vertical hit.
    if (0 == ucRet)
    {
        ucRet = BRICK_HIT_BOTTOM;
    }
    // Check for the presence of a brick at the specified row/col
    if (m_clLevelData.ausData[ucRow] & (1 << (K_USHORT)ucCol))
    {
        m_clLevelData.ausData[ucRow] &= ~(1 << (K_USHORT)ucCol);
        m_usBricksLeft--;

        if (m_ucBonusRow && m_ucBonusCol)
        {
            if ((ucRow == (m_ucBonusRow - 1)) && (ucCol == (m_ucBonusCol - 1)))
            {
                ucRet |= BRICK_HIT_SPECIAL;                
            }
        }
        ClearBrick_i(ucRow, ucCol);
    }

    return ucRet;
}

//---------------------------------------------------------------------------
K_BOOL BrPlayField::CheckBrickExists(K_USHORT usX_, K_USHORT usY_)
{
    K_UCHAR ucRow;
    K_UCHAR ucCol;

    // Check if we're vertically out of bounds...
    if (usY_ < (PLAYFIELD_BRICK_MARGIN + HUD_HEIGHT + PLAYFIELD_BORDER_HEIGHT))
    {
        return false;
    }
    else if (usY_ >= ((PLAYFIELD_BRICK_MARGIN + HUD_HEIGHT + PLAYFIELD_BORDER_HEIGHT) + (LEVEL_NUM_ROWS * BRICK_HEIGHT)))
    {
        return false;
    }

    // Convert X/Y to row/column
    ucCol = (usX_ - PLAYFIELD_BORDER_WIDTH) / BRICK_WIDTH;
    ucRow = (usY_ - (PLAYFIELD_BRICK_MARGIN + HUD_HEIGHT + PLAYFIELD_BORDER_HEIGHT)) / BRICK_HEIGHT;

    if (ucRow >= LEVEL_NUM_ROWS)
    {
        return false;
    }

    // Check for the presence of a brick at the specified row/col
    if (m_clLevelData.ausData[ucRow] & (1 << (K_USHORT)ucCol))
    {
        return true;
    }
    return false;
}

//---------------------------------------------------------------------------
K_BOOL BrPlayField::IsPlayFieldEmpty(void)
{
    if (0 == m_usBricksLeft)
    {
        return true;
    }
    return false;
}

//---------------------------------------------------------------------------
void BrPlayField::LevelAction( K_UCHAR ucFlags_ )
{
    if (ucFlags_ == LEVEL_ACTION_CIELING_HIT)
    {
        m_bHitCieling = true;
    }

    if (m_ucBonusTimer)
    {
        m_ucBonusTimer--;

        if (!m_ucBonusTimer)
        {
            ClearBonusString();
        }
    }

    // Special-item tick.
    SpecialTick();

    switch (m_clLevelData.eLevelType)
    {
        case BR_LEVEL_ROULETTE:
            if ((ucFlags_ == LEVEL_ACTION_PADDLE_HIT) ||
                (ucFlags_ == LEVEL_ACTION_WALL_HIT) ||
                (ucFlags_ == LEVEL_ACTION_CIELING_HIT))
            {
                K_UCHAR ucMaxCount = LEVEL_NUM_COLS;
                K_BOOL bDone = false;
                while (ucMaxCount--)
                {
                    K_UCHAR ucRow2 = (m_ucRow + 1) % LEVEL_NUM_ROWS;
                    K_UCHAR ucCol2 = m_ucCol;
                    if (ucRow2 == 0)
                    {
                        ucCol2 = (ucCol2 + 1) % LEVEL_NUM_COLS;
                    }

                    bDone = SwapBrick(m_ucRow, m_ucCol, ucRow2, ucCol2);

                    m_ucRow = ucRow2;
                    m_ucCol = ucCol2;

                    if (bDone)
                    {
                        m_ucCol = (ucCol2 + 1) % LEVEL_NUM_COLS;
                        break;
                    }
                }
            }
            break;
        case BR_LEVEL_NORMAL:
        if (ucFlags_ == LEVEL_ACTION_STEP)
        {
            // Implement an algorithm to determine when to enable
            // and disable bonus bricks.
            Tick();
        }
        else if (ucFlags_ == LEVEL_ACTION_PADDLE_HIT)
        {
            GetRandomData();
        }
             break;
        deafult:
            break;
    }
}

//---------------------------------------------------------------------------
Special_t BrPlayField::SelectBonus()
{
    Special_t eBonus = (Special_t)(GetRandomData() % (SPECIAL_COUNT - 1));

    ShowBonusString( eBonus );
    m_usSpecialTime = 30 * 60;
	m_eSpecial = eBonus;

    return eBonus;
}

//---------------------------------------------------------------------------
void BrPlayField::ShowBonusString( Special_t eBonus_ )
{
    DrawText_t stText;
    const char *pcBonus;

    stText.pstFont = &fntPrint_Char_21_6_False_False_False_;
    switch (eBonus_)
    {
    case SPECIAL_1UP:       pcBonus = "1UP"; break;
    case SPECIAL_BONUS:     pcBonus = "BONUS"; break;
    case SPECIAL_CATCH:     pcBonus = "CATCH"; break;
    case SPECIAL_FIREPOWER: pcBonus = "LAZER"; break;
    case SPECIAL_LONG:      pcBonus = "LONG"; break;
    case SPECIAL_SKIP:      pcBonus = "SKIP"; break;
    case SPECIAL_SLOW:       pcBonus = "SLOW"; break;
    default:
        pcBonus = "BONUS";
        break;

    }

    stText.pcString = pcBonus;
    stText.usTop = 0;
    stText.usLeft = 46;
    stText.uColor = COLOR_RED;
    pclDisplay->Text(&stText);

    m_ucBonusTimer = 120;
}

//---------------------------------------------------------------------------
void BrPlayField::ClearBonusString()
{
    DrawRectangle_t stRect;
    stRect.usLeft = 46;
    stRect.usRight = stRect.usLeft + 36;
    stRect.usTop = 0;
    stRect.usBottom = 8;
    stRect.uLineColor = COLOR_BLACK;
    stRect.uFillColor = COLOR_BLACK;
    stRect.bFill = true;

    pclDisplay->Rectangle(&stRect);
}

//---------------------------------------------------------------------------
K_USHORT BrPlayField::GetRandomData()
{
    m_usRandValue = (9 * m_usRandValue + 3);
    return m_usRandValue;
}

//---------------------------------------------------------------------------
void BrPlayField::Tick()
{    
    K_UCHAR ucRow = GetRandomData() >> 1;
    K_UCHAR ucCol = GetRandomData() >> 1;

    m_usTickCount++;

    if ((m_usTickCount & 0x01FF) == 0x000F)
    {
        SetBonus(0,0);        
    }
    else if ((m_usTickCount & 0x00FF) == 0x000F)
    {
        K_UCHAR ucRetries = 5;
        while (ucRetries--)
        {
            if (m_clLevelData.ausData[ucRow % LEVEL_NUM_ROWS] & (1 << (K_USHORT)(ucCol % LEVEL_NUM_COLS)))
            {
                SetBonus( (ucRow % LEVEL_NUM_ROWS) + 1,
                          (ucCol% LEVEL_NUM_COLS) + 1 );
                break;
            }

            ucRow = GetRandomData() >> 1;
            ucCol = GetRandomData() >> 1;
        }        
    }
}

//---------------------------------------------------------------------------
K_BOOL BrPlayField::SwapBrick(K_UCHAR ucRow1_, K_UCHAR ucCol1_, K_UCHAR ucRow2_, K_UCHAR ucCol2_)
{
    K_BOOL bExist1 = false;
    K_BOOL bExist2 = false;

    // Don't move bonus blocks.
    if ( ((ucRow1_ == m_ucBonusRow) && (ucCol1_ == m_ucBonusCol)) ||
         ((ucRow2_ == m_ucBonusRow) && (ucCol2_ == m_ucBonusCol)) )
    {
        return false;
    }

    if (m_clLevelData.ausData[ucRow1_] & (1 << (K_USHORT)ucCol1_))
    {
        bExist1 = true;
    }
    if (m_clLevelData.ausData[ucRow2_] & (1 << (K_USHORT)ucCol2_))
    {
        bExist2 = true;
    }

    if (bExist1 == true && bExist2 == false)
    {
        ClearBrick_i(ucRow1_, ucCol1_);
        DrawBrick(ucRow2_, ucCol2_, 0);
        m_clLevelData.ausData[ucRow1_] &= ~(1 << (K_USHORT)ucCol1_);
        m_clLevelData.ausData[ucRow2_] |= (1 << (K_USHORT)ucCol2_);
        return true;
    }
    else if (bExist1 == false && bExist2 == true)
    {
        ClearBrick_i(ucRow2_, ucCol2_);
        DrawBrick(ucRow1_, ucCol1_, 0);
        m_clLevelData.ausData[ucRow2_] &= ~(1 << (K_USHORT)ucCol2_);
        m_clLevelData.ausData[ucRow1_] |= (1 << (K_USHORT)ucCol1_);
        return true;
    }

    return false;
}

//---------------------------------------------------------------------------
void BrPlayField::SetBonus( K_UCHAR ucRow_, K_UCHAR ucCol_ )
{
    if (!ucRow_ || !ucCol_)
    {
        if (m_clLevelData.ausData[m_ucBonusRow - 1] & (1 << (K_USHORT)(m_ucBonusCol - 1)))
        {
            DrawBrick(m_ucBonusRow - 1, m_ucBonusCol - 1, 0);
        }
        m_ucBonusRow = 0;
        m_ucBonusCol = 0;
        return;
    }

    m_ucBonusRow = ucRow_;
    m_ucBonusCol = ucCol_;

    DrawBrick( m_ucBonusRow - 1, m_ucBonusCol - 1, 1);

    if (!(m_clLevelData.ausData[m_ucBonusRow - 1] & (1 << (K_USHORT)(m_ucBonusCol - 1))))
    {
        m_clLevelData.ausData[m_ucBonusRow - 1] |= (1 << (K_USHORT)(m_ucBonusCol - 1));
        m_usBricksLeft++;
    }
}

//---------------------------------------------------------------------------
void BrPlayField::DrawBrick(K_UCHAR ucRow_, K_UCHAR ucCol_, K_UCHAR ucMode_)
{
    DrawRectangle_t stRect;
    COLOR uColor;

    // Main body...
    stRect.usTop = HUD_HEIGHT + PLAYFIELD_BRICK_MARGIN + PLAYFIELD_BORDER_HEIGHT + (ucRow_ * BRICK_HEIGHT);
    stRect.usBottom = stRect.usTop + (BRICK_HEIGHT - 1);
    stRect.usLeft = PLAYFIELD_BORDER_WIDTH + (ucCol_ * BRICK_WIDTH);
    stRect.usRight = stRect.usLeft + (BRICK_WIDTH - 1);
    stRect.bFill = true;

    if (!ucMode_)
    {
        switch (ucRow_)
        {
        case 0: case 1: uColor = RGB_COLOR(0, 4*MAX_GREEN/5, 0); break;
        case 2: case 3: uColor = RGB_COLOR(0, 0, 4*MAX_BLUE/5); break;
        case 4: case 5: uColor = RGB_COLOR(4*MAX_RED/5, 4*MAX_GREEN/5, 0); break;
        case 6: case 7: uColor = RGB_COLOR(4*MAX_RED/5, 2*MAX_GREEN/5, 0); break;
        case 8: case 9: uColor = RGB_COLOR(4*MAX_RED/5, 0, 4*MAX_BLUE/5); break;
        default:
            break;
        }
    }
    else
    {
        uColor = COLOR_GREY75;
    }
    stRect.uFillColor = uColor;
    stRect.uLineColor = uColor;

    pclDisplay->Rectangle(&stRect);

    DrawLine_t stLine;

    // 3D-shadowing...
    // - Top
    if (!ucMode_)
    {
        switch (ucRow_)
        {
        case 0: case 1: uColor = RGB_COLOR(0, MAX_GREEN/2, 0); break;
        case 2: case 3: uColor = RGB_COLOR(0, 0, MAX_BLUE/2); break;
        case 4: case 5: uColor = RGB_COLOR(MAX_RED/2, MAX_GREEN/2, 0); break;
        case 6: case 7: uColor = RGB_COLOR(MAX_RED/2, MAX_GREEN/4, 0); break;
        case 8: case 9: uColor = RGB_COLOR(MAX_RED/2, 0, MAX_BLUE/2); break;
        default:
            break;
        }
    }
    else
    {
        uColor = COLOR_GREY50;
    }

    stLine.uColor = uColor;
    stLine.usX1 = PLAYFIELD_BORDER_WIDTH + (ucCol_ * BRICK_WIDTH);
    stLine.usX2 = stLine.usX1 + (BRICK_WIDTH - 1);
    stLine.usY1 = HUD_HEIGHT + PLAYFIELD_BRICK_MARGIN + PLAYFIELD_BORDER_HEIGHT + (ucRow_ * BRICK_HEIGHT);
    stLine.usY2 = stLine.usY1;
    pclDisplay->Line(&stLine);

    // -Left
    stLine.usX2 = stLine.usX1;
    stLine.usY2 = stLine.usY1 + (BRICK_HEIGHT - 1);
    pclDisplay->Line(&stLine);

    // - OUter-bottom
    if (!ucMode_)
    {
        switch (ucRow_)
        {
        case 0: case 1: uColor = RGB_COLOR(0, MAX_GREEN, 0); break;
        case 2: case 3: uColor = RGB_COLOR(0, 0, MAX_BLUE); break;
        case 4: case 5: uColor = RGB_COLOR(MAX_RED, MAX_GREEN, 0); break;
        case 6: case 7: uColor = RGB_COLOR(MAX_RED, MAX_GREEN/2, 0); break;
        case 8: case 9: uColor = RGB_COLOR(MAX_RED, 0, MAX_BLUE); break;
        default:
            break;
        }
    }
    else
    {
        uColor = COLOR_WHITE;
    }

    stLine.uColor = uColor;
    stLine.usX1 = PLAYFIELD_BORDER_WIDTH + (ucCol_ * BRICK_WIDTH) + 1;
    stLine.usX2 = stLine.usX1 + (BRICK_WIDTH - 2);
    stLine.usY1 = HUD_HEIGHT + PLAYFIELD_BRICK_MARGIN + PLAYFIELD_BORDER_HEIGHT + ((ucRow_+1) * BRICK_HEIGHT) - 1;
    stLine.usY2 = stLine.usY1;
    pclDisplay->Line(&stLine);

    // Outer-right
    stLine.usX1 = stLine.usX2;
    stLine.usY1 = HUD_HEIGHT + PLAYFIELD_BRICK_MARGIN + PLAYFIELD_BORDER_HEIGHT + (ucRow_ * BRICK_HEIGHT) + 1;
    stLine.usY2 = stLine.usY1 + BRICK_HEIGHT - 2;
    pclDisplay->Line(&stLine);
}
