#include "kerneltypes.h"
#include "graphics.h"
#include "draw.h"
#include "tiles.h"
#include "brlives.h"

//---------------------------------------------------------------------------
void BrLives::Reset(void)
{
    m_bUpdate = true;
    m_ucLives = 5;
}

//---------------------------------------------------------------------------

K_BOOL BrLives::Decrement(void)
{
    m_bUpdate = true;
    m_ucLives--;
    if (!m_ucLives)
    {
        return false;
    }
    return true;
}


//---------------------------------------------------------------------------
void BrLives::Increment(void)
{
    m_bUpdate = true;
    m_ucLives++;
}


//---------------------------------------------------------------------------
void BrLives::Draw(void)
{
    if (!m_bUpdate)
    {
        return;
    }

    DrawText_t stText;
    stText.pcString = "1UP";
    stText.pstFont = &fntPrint_Char_21_6_False_False_False_;
    stText.uColor = COLOR_RED;
    stText.usLeft = LIVES_X;
    stText.usTop = 0;

    pclDisplay->Text(&stText);

    DrawRectangle_t stRect;
    stRect.usTop = 8;
    stRect.usBottom = 15;
    stRect.usLeft = LIVES_X;
    stRect.usRight = stRect.usLeft + 23;
    stRect.bFill = true;
    stRect.uFillColor = COLOR_BLACK;
    stRect.uLineColor = COLOR_BLACK;
    pclDisplay->Rectangle(&stRect);

    K_CHAR acStr[4];

    MemUtil::DecimalToString(m_ucLives, acStr);

    stText.usTop = 8;
    stText.uColor = COLOR_WHITE;
    stText.pcString = acStr;

    pclDisplay->Text(&stText);

    m_bUpdate = false;
}

//---------------------------------------------------------------------------
K_UCHAR BrLives::GetLives(void)
{
    return m_ucLives;
}

