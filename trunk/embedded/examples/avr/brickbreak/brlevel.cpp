
#include "kerneltypes.h"
#include "graphics.h"
#include "draw.h"
#include "tiles.h"

#include "brlevel.h"
//---------------------------------------------------------------------------
void BrLevel::Reset(void)
{
    m_bUpdate = true;
    m_ucLevel = 0;
}

//---------------------------------------------------------------------------
void BrLevel::NextLevel(void)
{
    m_ucLevel++;
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
    stText.usLeft = LEVEL_X;
    stText.usTop = 0;

    pclDisplay->Text(&stText);

    DrawRectangle_t stRect;
    stRect.usTop = 8;
    stRect.usBottom = 15;
    stRect.usLeft = LEVEL_X;
    stRect.usRight = stRect.usLeft + 15;
    stRect.bFill = true;
    stRect.uFillColor = COLOR_BLACK;
    stRect.uLineColor = COLOR_BLACK;
    pclDisplay->Rectangle(&stRect);

    K_CHAR acStr[4];

    MemUtil::DecimalToString((K_UCHAR)(m_ucLevel + 1), acStr);

    stText.usTop = 8;
    stText.uColor = COLOR_WHITE;
    stText.pcString = acStr;

    pclDisplay->Text(&stText);

    m_bUpdate = false;
}

//---------------------------------------------------------------------------
K_UCHAR BrLevel::GetLevel(void)
{
    return m_ucLevel;
}
