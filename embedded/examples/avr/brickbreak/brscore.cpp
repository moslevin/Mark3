
#include "kerneltypes.h"
#include "graphics.h"
#include "draw.h"
#include "tiles.h"
#include "brscore.h"

//---------------------------------------------------------------------------
void BrScore::Reset(void)
{
    m_u32Score = 0;
    m_bUpdate = true;
}

//---------------------------------------------------------------------------
void BrScore::AddScore(uint32_t u32Score_)
{
    m_u32Score += u32Score_;
    m_bUpdate = true;
}

//---------------------------------------------------------------------------
void BrScore::Draw(void)
{
    if (!m_bUpdate)
    {
        return;
    }
    DrawText_t stText;
    stText.pcString = "SCORE";
    stText.pstFont = &fntPrint_Char_21_6_False_False_False_;
    stText.uColor = COLOR_RED;
    stText.u16Left = SCORE_X;
    stText.u16Top = 0;

    pclDisplay->Text(&stText);

    DrawRectangle_t stRect;
    stRect.u16Top = 8;
    stRect.u16Bottom = 15;
    stRect.u16Left = SCORE_X;
    stRect.u16Right = stRect.u16Left + 56;
    stRect.bFill = true;
    stRect.uFillColor = COLOR_BLACK;
    stRect.u32ineColor = COLOR_BLACK;
    pclDisplay->Rectangle(&stRect);

    char acStr[11];
    MemUtil::DecimalToString(m_u32Score, acStr);

    stText.u16Top = 8;
    stText.uColor = COLOR_WHITE;
    stText.pcString = acStr;

    pclDisplay->Text(&stText);

    m_bUpdate = false;
}

//---------------------------------------------------------------------------
uint32_t BrScore::GetScore(void)
{
    return m_u32Score;
}
