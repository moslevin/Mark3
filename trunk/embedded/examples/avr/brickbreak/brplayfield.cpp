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
const uint16_t au16LevelData[][ LEVEL_NUM_ROWS ] LEVEL_MEM =
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
void BrPlayField::LoadLevel( uint16_t u16Level_ )
{
    uint16_t u16LevelIdx = u16Level_ % NUM_LEVELS;
    const uint16_t *pu16Data = &(au16LevelData[u16LevelIdx][0]);

    m_bHitCieling = false;

    m_u16BricksLeft = 0;
    m_u8Bonusimer = 0;
    m_u16RandValue = 1;
    m_u8Col = 0;
    m_u8Row = 0;

    for (uint8_t i = 0; i < LEVEL_NUM_ROWS; i++)
    {
        m_clLevelData.au16Data[i] = READ_LEVEL_MEM(pu16Data++);

        uint16_t j = 1;
        while (j < 0x1000)
        {
            if (m_clLevelData.au16Data[i] & j)
            {
                m_u16BricksLeft++;
            }
            j <<= 1;
        }
    }
    m_clLevelData.eLevelType = aeLevelTypes[ u16LevelIdx ];
}

//---------------------------------------------------------------------------
void BrPlayField::Draw(void)
{
    DrawRectangle_t stRect;

    stRect.bFill = true;
    stRect.uFillColor = COLOR_BLUE;
    stRect.u32ineColor = COLOR_BLUE;
    stRect.u16Left = 0;
    stRect.u16Right = PLAYFIELD_BORDER_WIDTH - 1;
    stRect.u16Top = HUD_HEIGHT;
    stRect.u16Bottom = PADDLE_YPOS;

    pclDisplay->Rectangle(&stRect);

    stRect.u16Left = PLAYFIELD_RIGHT_BORDER;
    stRect.u16Right = PLAYFIELD_RIGHT_BORDER + PLAYFIELD_BORDER_WIDTH - 1;

    pclDisplay->Rectangle(&stRect);

    stRect.u16Top = HUD_HEIGHT;
    stRect.u16Bottom = HUD_HEIGHT + PLAYFIELD_BORDER_HEIGHT;
    stRect.u16Left = 0;
    stRect.u16Right = SCREEN_WIDTH - 1;

    pclDisplay->Rectangle(&stRect);

    stRect.u16Top = HUD_HEIGHT + PLAYFIELD_BORDER_HEIGHT;
    stRect.u16Bottom = stRect.u16Top + (LEVEL_NUM_ROWS * BRICK_HEIGHT) + PLAYFIELD_BRICK_MARGIN;
    stRect.u16Left = PLAYFIELD_LEFT_BORDER;
    stRect.u16Right = PLAYFIELD_RIGHT_BORDER-1;
    stRect.uFillColor = COLOR_BLACK;
    stRect.u32ineColor = COLOR_BLACK;
    stRect.bFill = true;

    pclDisplay->Rectangle(&stRect);

    for (uint8_t i = 0; i < LEVEL_NUM_ROWS; i++)
    {
        for (uint8_t j = 0; j < LEVEL_NUM_COLS; j++)
        {
            if (m_clLevelData.au16Data[i] & (1 << (uint16_t)j))
            {
                DrawBrick( i, j, 0 );
            }
        }
    }
}

//---------------------------------------------------------------------------
void BrPlayField::ClearBrick_i(uint8_t u8Row_, uint8_t u8Col_)
{
    DrawRectangle_t stRect;
    stRect.u16Top = (PLAYFIELD_BRICK_MARGIN + HUD_HEIGHT + PLAYFIELD_BORDER_HEIGHT) + (u8Row_ * BRICK_HEIGHT);
    stRect.u16Bottom = stRect.u16Top + BRICK_HEIGHT - 1;
    stRect.u16Left = PLAYFIELD_BORDER_WIDTH + (u8Col_ * BRICK_WIDTH);
    stRect.u16Right = stRect.u16Left + BRICK_WIDTH - 1;
    stRect.bFill = true;
    stRect.uFillColor = COLOR_BLACK;
    stRect.u32ineColor = COLOR_BLACK;
    pclDisplay->Rectangle(&stRect);

    if ((m_u8Bonusow != 0) && (m_u8Bonusol != 0))
    {
        if ((u8Row_ == (m_u8Bonusow - 1)) && (u8Col_ == (m_u8Bonusol - 1)))
        {
            m_u8Bonusol = 0;
            m_u8Bonusow = 0;
        }
    }
}

//---------------------------------------------------------------------------
uint8_t BrPlayField::ClearBrick(uint16_t u16X_, uint16_t u16Y_)
{
    //!! Must ensure the "CheckBrick" test has passed before calling.

    // Convert X/Y to row/column
    uint8_t u8Col = (u16X_ - PLAYFIELD_BORDER_WIDTH) / BRICK_WIDTH;
    uint8_t u8Row = (u16Y_ - (PLAYFIELD_BRICK_MARGIN + HUD_HEIGHT + PLAYFIELD_BORDER_HEIGHT)) / BRICK_HEIGHT;

    uint8_t u8Ret = 0;

    if (u16Y_ == ((PLAYFIELD_BRICK_MARGIN + HUD_HEIGHT + PLAYFIELD_BORDER_HEIGHT) + (u8Row * BRICK_HEIGHT)))
    {
        u8Ret |= BRICK_HIT_TOP;
    }
    else if (u16Y_ == ((PLAYFIELD_BRICK_MARGIN + HUD_HEIGHT + PLAYFIELD_BORDER_HEIGHT - 1) + ((u8Row+1) * BRICK_HEIGHT)))
    {
        u8Ret |= BRICK_HIT_BOTTOM;
    }
    else if (u16X_ == (PLAYFIELD_BORDER_WIDTH + (u8Col * BRICK_WIDTH)))
    {
        u8Ret |= BRICK_HIT_LEFT;
    }
    else if (u16X_ == ((PLAYFIELD_BORDER_WIDTH-1) + ((u8Col+1) * BRICK_WIDTH)))
    {
        u8Ret |= BRICK_HIT_RIGHT;
    }

    // Hit is in the middle of a brick?  Assume a vertical hit.
    if (0 == u8Ret)
    {
        u8Ret = BRICK_HIT_BOTTOM;
    }
    // Check for the presence of a brick at the specified row/col
    if (m_clLevelData.au16Data[u8Row] & (1 << (uint16_t)u8Col))
    {
        m_clLevelData.au16Data[u8Row] &= ~(1 << (uint16_t)u8Col);
        m_u16BricksLeft--;

        if (m_u8Bonusow && m_u8Bonusol)
        {
            if ((u8Row == (m_u8Bonusow - 1)) && (u8Col == (m_u8Bonusol - 1)))
            {
                u8Ret |= BRICK_HIT_SPECIAL;                
            }
        }
        ClearBrick_i(u8Row, u8Col);
    }

    return u8Ret;
}

//---------------------------------------------------------------------------
bool BrPlayField::CheckBrickExists(uint16_t u16X_, uint16_t u16Y_)
{
    uint8_t u8Row;
    uint8_t u8Col;

    // Check if we're vertically out of bounds...
    if (u16Y_ < (PLAYFIELD_BRICK_MARGIN + HUD_HEIGHT + PLAYFIELD_BORDER_HEIGHT))
    {
        return false;
    }
    else if (u16Y_ >= ((PLAYFIELD_BRICK_MARGIN + HUD_HEIGHT + PLAYFIELD_BORDER_HEIGHT) + (LEVEL_NUM_ROWS * BRICK_HEIGHT)))
    {
        return false;
    }

    // Convert X/Y to row/column
    u8Col = (u16X_ - PLAYFIELD_BORDER_WIDTH) / BRICK_WIDTH;
    u8Row = (u16Y_ - (PLAYFIELD_BRICK_MARGIN + HUD_HEIGHT + PLAYFIELD_BORDER_HEIGHT)) / BRICK_HEIGHT;

    if (u8Row >= LEVEL_NUM_ROWS)
    {
        return false;
    }

    // Check for the presence of a brick at the specified row/col
    if (m_clLevelData.au16Data[u8Row] & (1 << (uint16_t)u8Col))
    {
        return true;
    }
    return false;
}

//---------------------------------------------------------------------------
bool BrPlayField::IsPlayFieldEmpty(void)
{
    if (0 == m_u16BricksLeft)
    {
        return true;
    }
    return false;
}

//---------------------------------------------------------------------------
void BrPlayField::LevelAction( uint8_t u8Flags_ )
{
    if (u8Flags_ == LEVEL_ACTION_CIELING_HIT)
    {
        m_bHitCieling = true;
    }

    if (m_u8Bonusimer)
    {
        m_u8Bonusimer--;

        if (!m_u8Bonusimer)
        {
            ClearBonustring();
        }
    }

    // Special-item tick.
    SpecialTick();

    switch (m_clLevelData.eLevelType)
    {
        case BR_LEVEL_ROULETTE:
            if ((u8Flags_ == LEVEL_ACTION_PADDLE_HIT) ||
                (u8Flags_ == LEVEL_ACTION_WALL_HIT) ||
                (u8Flags_ == LEVEL_ACTION_CIELING_HIT))
            {
                uint8_t u8MaxCount = LEVEL_NUM_COLS;
                bool bDone = false;
                while (u8MaxCount--)
                {
                    uint8_t u8Row2 = (m_u8Row + 1) % LEVEL_NUM_ROWS;
                    uint8_t u8Col2 = m_u8Col;
                    if (u8Row2 == 0)
                    {
                        u8Col2 = (u8Col2 + 1) % LEVEL_NUM_COLS;
                    }

                    bDone = SwapBrick(m_u8Row, m_u8Col, u8Row2, u8Col2);

                    m_u8Row = u8Row2;
                    m_u8Col = u8Col2;

                    if (bDone)
                    {
                        m_u8Col = (u8Col2 + 1) % LEVEL_NUM_COLS;
                        break;
                    }
                }
            }
            break;
        case BR_LEVEL_NORMAL:
        if (u8Flags_ == LEVEL_ACTION_STEP)
        {
            // Implement an algorithm to determine when to enable
            // and disable bonus bricks.
            Tick();
        }
        else if (u8Flags_ == LEVEL_ACTION_PADDLE_HIT)
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

    ShowBonustring( eBonus );
    m_u16SpecialTime = 30 * 60;
	m_eSpecial = eBonus;

    return eBonus;
}

//---------------------------------------------------------------------------
void BrPlayField::ShowBonustring( Special_t eBonus_ )
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
    stText.u16Top = 0;
    stText.u16Left = 46;
    stText.uColor = COLOR_RED;
    pclDisplay->Text(&stText);

    m_u8Bonusimer = 120;
}

//---------------------------------------------------------------------------
void BrPlayField::ClearBonustring()
{
    DrawRectangle_t stRect;
    stRect.u16Left = 46;
    stRect.u16Right = stRect.u16Left + 36;
    stRect.u16Top = 0;
    stRect.u16Bottom = 8;
    stRect.u32ineColor = COLOR_BLACK;
    stRect.uFillColor = COLOR_BLACK;
    stRect.bFill = true;

    pclDisplay->Rectangle(&stRect);
}

//---------------------------------------------------------------------------
uint16_t BrPlayField::GetRandomData()
{
    m_u16RandValue = (9 * m_u16RandValue + 3);
    return m_u16RandValue;
}

//---------------------------------------------------------------------------
void BrPlayField::Tick()
{    
    uint8_t u8Row = GetRandomData() >> 1;
    uint8_t u8Col = GetRandomData() >> 1;

    m_u16TickCount++;

    if ((m_u16TickCount & 0x01FF) == 0x000F)
    {
        SetBonus(0,0);        
    }
    else if ((m_u16TickCount & 0x00FF) == 0x000F)
    {
        uint8_t u8Retries = 5;
        while (u8Retries--)
        {
            if (m_clLevelData.au16Data[u8Row % LEVEL_NUM_ROWS] & (1 << (uint16_t)(u8Col % LEVEL_NUM_COLS)))
            {
                SetBonus( (u8Row % LEVEL_NUM_ROWS) + 1,
                          (u8Col% LEVEL_NUM_COLS) + 1 );
                break;
            }

            u8Row = GetRandomData() >> 1;
            u8Col = GetRandomData() >> 1;
        }        
    }
}

//---------------------------------------------------------------------------
bool BrPlayField::SwapBrick(uint8_t u8Row1_, uint8_t u8Col1_, uint8_t u8Row2_, uint8_t u8Col2_)
{
    bool bExist1 = false;
    bool bExist2 = false;

    // Don't move bonus blocks.
    if ( ((u8Row1_ == m_u8Bonusow) && (u8Col1_ == m_u8Bonusol)) ||
         ((u8Row2_ == m_u8Bonusow) && (u8Col2_ == m_u8Bonusol)) )
    {
        return false;
    }

    if (m_clLevelData.au16Data[u8Row1_] & (1 << (uint16_t)u8Col1_))
    {
        bExist1 = true;
    }
    if (m_clLevelData.au16Data[u8Row2_] & (1 << (uint16_t)u8Col2_))
    {
        bExist2 = true;
    }

    if (bExist1 == true && bExist2 == false)
    {
        ClearBrick_i(u8Row1_, u8Col1_);
        DrawBrick(u8Row2_, u8Col2_, 0);
        m_clLevelData.au16Data[u8Row1_] &= ~(1 << (uint16_t)u8Col1_);
        m_clLevelData.au16Data[u8Row2_] |= (1 << (uint16_t)u8Col2_);
        return true;
    }
    else if (bExist1 == false && bExist2 == true)
    {
        ClearBrick_i(u8Row2_, u8Col2_);
        DrawBrick(u8Row1_, u8Col1_, 0);
        m_clLevelData.au16Data[u8Row2_] &= ~(1 << (uint16_t)u8Col2_);
        m_clLevelData.au16Data[u8Row1_] |= (1 << (uint16_t)u8Col1_);
        return true;
    }

    return false;
}

//---------------------------------------------------------------------------
void BrPlayField::SetBonus( uint8_t u8Row_, uint8_t u8Col_ )
{
    if (!u8Row_ || !u8Col_)
    {
        if (m_clLevelData.au16Data[m_u8Bonusow - 1] & (1 << (uint16_t)(m_u8Bonusol - 1)))
        {
            DrawBrick(m_u8Bonusow - 1, m_u8Bonusol - 1, 0);
        }
        m_u8Bonusow = 0;
        m_u8Bonusol = 0;
        return;
    }

    m_u8Bonusow = u8Row_;
    m_u8Bonusol = u8Col_;

    DrawBrick( m_u8Bonusow - 1, m_u8Bonusol - 1, 1);

    if (!(m_clLevelData.au16Data[m_u8Bonusow - 1] & (1 << (uint16_t)(m_u8Bonusol - 1))))
    {
        m_clLevelData.au16Data[m_u8Bonusow - 1] |= (1 << (uint16_t)(m_u8Bonusol - 1));
        m_u16BricksLeft++;
    }
}

//---------------------------------------------------------------------------
void BrPlayField::DrawBrick(uint8_t u8Row_, uint8_t u8Col_, uint8_t u8Mode_)
{
    DrawRectangle_t stRect;
    COLOR uColor;

    // Main body...
    stRect.u16Top = HUD_HEIGHT + PLAYFIELD_BRICK_MARGIN + PLAYFIELD_BORDER_HEIGHT + (u8Row_ * BRICK_HEIGHT);
    stRect.u16Bottom = stRect.u16Top + (BRICK_HEIGHT - 1);
    stRect.u16Left = PLAYFIELD_BORDER_WIDTH + (u8Col_ * BRICK_WIDTH);
    stRect.u16Right = stRect.u16Left + (BRICK_WIDTH - 1);
    stRect.bFill = true;

    if (!u8Mode_)
    {
        switch (u8Row_)
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
    stRect.u32ineColor = uColor;

    pclDisplay->Rectangle(&stRect);

    DrawLine_t stLine;

    // 3D-shadowing...
    // - Top
    if (!u8Mode_)
    {
        switch (u8Row_)
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
    stLine.u16X1 = PLAYFIELD_BORDER_WIDTH + (u8Col_ * BRICK_WIDTH);
    stLine.u16X2 = stLine.u16X1 + (BRICK_WIDTH - 1);
    stLine.u16Y1 = HUD_HEIGHT + PLAYFIELD_BRICK_MARGIN + PLAYFIELD_BORDER_HEIGHT + (u8Row_ * BRICK_HEIGHT);
    stLine.u16Y2 = stLine.u16Y1;
    pclDisplay->Line(&stLine);

    // -Left
    stLine.u16X2 = stLine.u16X1;
    stLine.u16Y2 = stLine.u16Y1 + (BRICK_HEIGHT - 1);
    pclDisplay->Line(&stLine);

    // - OUter-bottom
    if (!u8Mode_)
    {
        switch (u8Row_)
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
    stLine.u16X1 = PLAYFIELD_BORDER_WIDTH + (u8Col_ * BRICK_WIDTH) + 1;
    stLine.u16X2 = stLine.u16X1 + (BRICK_WIDTH - 2);
    stLine.u16Y1 = HUD_HEIGHT + PLAYFIELD_BRICK_MARGIN + PLAYFIELD_BORDER_HEIGHT + ((u8Row_+1) * BRICK_HEIGHT) - 1;
    stLine.u16Y2 = stLine.u16Y1;
    pclDisplay->Line(&stLine);

    // Outer-right
    stLine.u16X1 = stLine.u16X2;
    stLine.u16Y1 = HUD_HEIGHT + PLAYFIELD_BRICK_MARGIN + PLAYFIELD_BORDER_HEIGHT + (u8Row_ * BRICK_HEIGHT) + 1;
    stLine.u16Y2 = stLine.u16Y1 + BRICK_HEIGHT - 2;
    pclDisplay->Line(&stLine);
}
