
#include "kerneltypes.h"
#include "graphics.h"
#include "draw.h"
#include "tiles.h"
#include "brscore.h"

//---------------------------------------------------------------------------
void BrScore::Reset(void)
{
    m_ulScore = 0;
    m_bUpdate = true;
}

//---------------------------------------------------------------------------
void BrScore::AddScore(K_ULONG ulScore_)
{
    m_ulScore += ulScore_;
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
    stText.usLeft = SCORE_X;
    stText.usTop = 0;

    pclDisplay->Text(&stText);

    DrawRectangle_t stRect;
    stRect.usTop = 8;
    stRect.usBottom = 15;
    stRect.usLeft = SCORE_X;
    stRect.usRight = stRect.usLeft + 56;
    stRect.bFill = true;
    stRect.uFillColor = COLOR_BLACK;
    stRect.uLineColor = COLOR_BLACK;
    pclDisplay->Rectangle(&stRect);

    K_CHAR acStr[11];
    MemUtil::DecimalToString(m_ulScore, acStr);

    stText.usTop = 8;
    stText.uColor = COLOR_WHITE;
    stText.pcString = acStr;

    pclDisplay->Text(&stText);

    m_bUpdate = false;
}

//---------------------------------------------------------------------------
K_ULONG BrScore::GetScore(void)
{
    return m_ulScore;
}
