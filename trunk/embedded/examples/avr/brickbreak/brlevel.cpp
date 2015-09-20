
#include "kerneltypes.h"
#include "graphics.h"
#include "draw.h"
#include "tiles.h"

#include "brlevel.h"
//---------------------------------------------------------------------------
void BrLevel::Reset(void)
{
    m_bUpdate = true;
    m_u8Level = 0;
}

//---------------------------------------------------------------------------
void BrLevel::NextLevel(void)
{
    m_u8Level++;
    m_bUpdate = true;
}

//-----------------------------------H----------------------------------------
void BrLevel::Draw(void)
{
    if (!m_bUpdate)
    {
        return;
    }
    DrawText_t stText;
    stText.pcString = "LV";
    stText.pstFont = &fntPrint_Char_21_6_False_False_False_;
    stText.uColor = COLOR_RED;
    stText.u16Left = LEVEL_X;
    stText.u16Top = 0;

    pclDisplay->Text(&stText);

    DrawRectangle_t stRect;
    stRect.u16Top = 8;
    stRect.u16Bottom = 15;
    stRect.u16Left = LEVEL_X;
    stRect.u16Right = stRect.u16Left + 15;
    stRect.bFill = true;
    stRect.uFillColor = COLOR_BLACK;
    stRect.u32ineColor = COLOR_BLACK;
    pclDisplay->Rectangle(&stRect);

    char acStr[4];

    MemUtil::DecimalToString((uint8_t)(m_u8Level + 1), acStr);

    stText.u16Top = 8;
    stText.uColor = COLOR_WHITE;
    stText.pcString = acStr;

    pclDisplay->Text(&stText);

    m_bUpdate = false;
}

//---------------------------------------------------------------------------
uint8_t BrLevel::GetLevel(void)
{
    return m_u8Level;
}
