#include "kerneltypes.h"
#include "graphics.h"
#include "draw.h"
#include "tiles.h"
#include "brlives.h"

//---------------------------------------------------------------------------
void BrLives::Reset(void)
{
    m_bUpdate = true;
    m_u8Lives = 5;
}

//---------------------------------------------------------------------------

bool BrLives::Decrement(void)
{
    m_bUpdate = true;
    m_u8Lives--;
    if (!m_u8Lives)
    {
        return false;
    }
    return true;
}


//---------------------------------------------------------------------------
void BrLives::Increment(void)
{
    m_bUpdate = true;
    m_u8Lives++;
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
    stText.u16Left = LIVES_X;
    stText.u16Top = 0;

    pclDisplay->Text(&stText);

    DrawRectangle_t stRect;
    stRect.u16Top = 8;
    stRect.u16Bottom = 15;
    stRect.u16Left = LIVES_X;
    stRect.u16Right = stRect.u16Left + 23;
    stRect.bFill = true;
    stRect.uFillColor = COLOR_BLACK;
    stRect.u32ineColor = COLOR_BLACK;
    pclDisplay->Rectangle(&stRect);

    char acStr[4];

    MemUtil::DecimalToString(m_u8Lives, acStr);

    stText.u16Top = 8;
    stText.uColor = COLOR_WHITE;
    stText.pcString = acStr;

    pclDisplay->Text(&stText);

    m_bUpdate = false;
}

//---------------------------------------------------------------------------
uint8_t BrLives::GetLives(void)
{
    return m_u8Lives;
}

