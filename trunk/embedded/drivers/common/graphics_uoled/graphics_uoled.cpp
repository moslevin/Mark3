#include "kerneltypes.h"
#include "graphics_uoled.h"
#include "uoled_defines.h"
#include "thread.h"

//---------------------------------------------------------------------------
void GraphicsUOLED::WriteByte( uint8_t u8Byte_ )
{
    while (!m_pclDriver->Write(1, &u8Byte_)) { /* do nothing */ }
}

//---------------------------------------------------------------------------
void GraphicsUOLED::WriteWord( uint16_t u16Word_ )
{
    uint8_t u8Byte = (uint8_t)(u16Word_ >> 8);
    while (!m_pclDriver->Write(1, &u8Byte)) { /* do nothing */ }

    u8Byte = (uint8_t)u16Word_;
    while (!m_pclDriver->Write(1, &u8Byte)) { /* do nothing */ }
}

//---------------------------------------------------------------------------
uint8_t GraphicsUOLED::ReadByte( void )
{
    uint8_t u8Response;
    while (!m_pclDriver->Read(1, &u8Response)) { /* do nothing */ }

    return u8Response;
}

//---------------------------------------------------------------------------
uint8_t GraphicsUOLED::WaitAck( void )
{
    uint16_t u16Timeout = SERIAL_TIMEOUT_MAX;
    uint8_t u8Response = 0;
    
    while (u16Timeout--)
    {
        if(m_pclDriver->Read(1, &u8Response))
        {
            break;
        }
        Thread::USleep(1000);   //100us sleep
    }

    return u8Response;
}


//---------------------------------------------------------------------------
void GraphicsUOLED::WriteVector( DataVector_t *pstVector_, uint8_t u8Count_)
{
    uint8_t u8Index = 0;
    while (u8Index < u8Count_)
    {
        if (1 == pstVector_->u8Len)
        {
            WriteByte(pstVector_->u16Data);
        }
        else if (2 == pstVector_->u8Len)
        {
            WriteWord(pstVector_->u16Data);
        }
        u8Index++;
        pstVector_++;
    }
}

//---------------------------------------------------------------------------
#define COMMAND_HEADER  \
{ \
    uint8_t u8Retry = COMMAND_RETRY_COUNT;  \
    while (u8Retry--) \
    { \

#define COMMAND_FOOTER \
        if (ACK_BYTE == WaitAck()) { break; } \
    } \
}

//---------------------------------------------------------------------------
void GraphicsUOLED::ClearScreen()
{
    COMMAND_HEADER

    WriteWord(GFX_CLEAR_SCREEN);
    
    COMMAND_FOOTER
}

//---------------------------------------------------------------------------
void GraphicsUOLED::Point(DrawPoint_t *pstPoint_)
{
    COMMAND_HEADER
    
    DataVector_t astVector[4];

    astVector[0].u16Data = GFX_PUT_PIXEL;
    astVector[0].u8Len = 2;
    astVector[1].u16Data = pstPoint_->u16X;
    astVector[1].u8Len = 2;
    astVector[2].u16Data = pstPoint_->u16Y;
    astVector[2].u8Len = 2;
    astVector[3].u16Data = pstPoint_->uColor;
    astVector[3].u8Len = 2;

    WriteVector(astVector, 4);

    COMMAND_FOOTER
}

//---------------------------------------------------------------------------
void GraphicsUOLED::Line(DrawLine_t *pstLine_)
{
    COMMAND_HEADER
    
    DataVector_t astVector[6];

    astVector[0].u16Data = GFX_DRAW_LINE;
    astVector[0].u8Len = 2;
    astVector[1].u16Data = pstLine_->u16X1;
    astVector[1].u8Len = 2;
    astVector[2].u16Data = pstLine_->u16Y1;
    astVector[2].u8Len = 2;
    astVector[3].u16Data = pstLine_->u16X2;
    astVector[3].u8Len = 2;
    astVector[4].u16Data = pstLine_->u16Y2;
    astVector[4].u8Len = 2;
    astVector[5].u16Data = pstLine_->uColor;
    astVector[5].u8Len = 2;

    WriteVector(astVector, 6);

    COMMAND_FOOTER
}

//---------------------------------------------------------------------------
void GraphicsUOLED::SetOutlineColor( COLOR uColor_ )
{
    COMMAND_HEADER

    DataVector_t astVector[2];

    astVector[0].u16Data = GFX_SET_OUTLINE_COLOR;
    astVector[1].u16Data = uColor_;

    WriteVector(astVector, 2);

    COMMAND_FOOTER
}

//---------------------------------------------------------------------------
void GraphicsUOLED::Circle(DrawCircle_t *pstCircle_)
{
    if (pstCircle_->bFill)
    {
        SetOutlineColor(pstCircle_->u32ineColor);
    }    

    COMMAND_HEADER
    
    DataVector_t astVector[5];

    if (pstCircle_->bFill)
    {
        astVector[0].u16Data = GFX_DRAW_CIRCLE_FILLED;
    }
    else
    {
        astVector[0].u16Data = GFX_DRAW_CIRCLE;
    }
    astVector[0].u8Len = 2;

    astVector[1].u16Data = pstCircle_->u16X;
    astVector[1].u8Len = 2;
    astVector[2].u16Data = pstCircle_->u16Y;
    astVector[2].u8Len = 2;
    astVector[3].u16Data = pstCircle_->u16Radius;
    astVector[3].u8Len = 2;
    
    if (pstCircle_->bFill)
    {
        astVector[4].u16Data = pstCircle_->uFillColor;
    }
    else
    {
        astVector[4].u16Data = pstCircle_->u32ineColor;
    }
    astVector[4].u8Len = 2;

    WriteVector(astVector, 5);

    COMMAND_FOOTER
}

//---------------------------------------------------------------------------
void GraphicsUOLED::Rectangle(DrawRectangle_t *pstRectangle_)
{
    if (pstRectangle_->bFill)
    {
        SetOutlineColor(pstRectangle_->u32ineColor);
    }    

    COMMAND_HEADER
    
    DataVector_t astVector[6];

    if (pstRectangle_->bFill)
    {
        astVector[0].u16Data = GFX_DRAW_RECTANGLE_FILLED;
    }
    else
    {
        astVector[0].u16Data = GFX_DRAW_RECTANGLE;
    }
    astVector[0].u8Len = 2;

    astVector[1].u16Data = pstRectangle_->u16Left;
    astVector[1].u8Len = 2;
    astVector[2].u16Data = pstRectangle_->u16Top;
    astVector[2].u8Len = 2;
    astVector[3].u16Data = pstRectangle_->u16Right;
    astVector[3].u8Len = 2;
    astVector[4].u16Data = pstRectangle_->u16Bottom;
    astVector[4].u8Len = 2;

    if (pstRectangle_->bFill)
    {
        astVector[5].u16Data = pstRectangle_->uFillColor;
    }
    else
    {
        astVector[5].u16Data = pstRectangle_->u32ineColor;
    }
    astVector[5].u8Len = 2;

    WriteVector(astVector, 6);

    COMMAND_FOOTER
}

//---------------------------------------------------------------------------
void GraphicsUOLED::TriangleWire(DrawPoly_t *pstTriangle_)
{
    COMMAND_HEADER
    DataVector_t astVector[8];

    astVector[0].u16Data = GFX_DRAW_TRIANGLE;
    astVector[0].u8Len = 2;
    astVector[1].u16Data = pstTriangle_->pstVector[0].u16X;
    astVector[1].u8Len = 2;
    astVector[2].u16Data = pstTriangle_->pstVector[0].u16Y;
    astVector[2].u8Len = 2;
    astVector[3].u16Data = pstTriangle_->pstVector[1].u16X;
    astVector[3].u8Len = 2;
    astVector[4].u16Data = pstTriangle_->pstVector[1].u16Y;
    astVector[4].u8Len = 2;
    astVector[5].u16Data = pstTriangle_->pstVector[2].u16X;
    astVector[5].u8Len = 2;
    astVector[6].u16Data = pstTriangle_->pstVector[2].u16Y;
    astVector[6].u8Len = 2;
    astVector[7].u16Data = pstTriangle_->uColor;
    astVector[7].u8Len = 2;
    WriteVector(astVector, 8);

    COMMAND_FOOTER
}

//---------------------------------------------------------------------------
void GraphicsUOLED::Polygon(DrawPoly_t *pstPoly_)
{
    uint8_t i;

    COMMAND_HEADER
    WriteWord(GFX_DRAW_POLYGON);
    WriteWord(pstPoly_->u16NumPoints);
    for (i = 0; i < pstPoly_->u16NumPoints; i++)
    {
        WriteWord(pstPoly_->pstVector[i].u16X);
        WriteWord(pstPoly_->pstVector[i].u16Y);
    }
    WriteWord(pstPoly_->uColor);
    COMMAND_FOOTER
}

//---------------------------------------------------------------------------
void GraphicsUOLED::MoveCursor(uint16_t u16X_, uint16_t u16Y_)
{
    COMMAND_HEADER
    DataVector_t astVector[3];

    astVector[0].u16Data = TEXT_MOVE_CURSOR;
    astVector[0].u8Len = 2;
    astVector[1].u16Data = (u16Y_ + 4) >> 3;
    astVector[1].u8Len = 2;
    astVector[2].u16Data = (u16X_ + 4) >> 3;
    astVector[2].u8Len = 2;
    WriteVector(astVector, 3);
    COMMAND_FOOTER
}
//---------------------------------------------------------------------------
void GraphicsUOLED::MoveOrigin(uint16_t u16X_, uint16_t u16Y_)
{
    COMMAND_HEADER
    DataVector_t astVector[3];

    astVector[0].u16Data = GFX_MOVE_ORIGIN;
    astVector[0].u8Len = 2;
    astVector[1].u16Data = u16X_;
    astVector[1].u8Len = 2;
    astVector[2].u16Data = u16Y_;
    astVector[2].u8Len = 2;
    WriteVector(astVector, 3);
    COMMAND_FOOTER
}

//---------------------------------------------------------------------------
void GraphicsUOLED::Text(DrawText_t *pstText_)
{
    MoveOrigin(pstText_->u16Left, pstText_->u16Top);
    if (m_uTextColor != pstText_->uColor)
    {
        SetFontFGColor(pstText_->uColor);
        m_uTextColor = pstText_->uColor;
    }

    COMMAND_HEADER
    const char *pcCursor = pstText_->pcString;

    WriteWord(TEXT_PUT_STRING);
    while (*pcCursor)
    {
        WriteByte(*pcCursor++);
    }
    WriteByte(0);
    COMMAND_FOOTER

    MoveOrigin(0, 0);
}

//---------------------------------------------------------------------------
uint16_t GraphicsUOLED::TextWidth(DrawText_t *pstText_)
{
    uint16_t u16RetVal = 0;
    const char *pcCursor = pstText_->pcString;

    while (*pcCursor)
    {
        u16RetVal += 8;
    }
    return u16RetVal;
}

//---------------------------------------------------------------------------
void GraphicsUOLED::SetFontFGColor(COLOR uColor_)
{
    COMMAND_HEADER
    WriteWord(TEXT_FG_COLOR);
    WriteWord(uColor_);
    COMMAND_FOOTER
}

//---------------------------------------------------------------------------
void GraphicsUOLED::SetFontBGColor(COLOR uColor_)
{
    COMMAND_HEADER
    WriteWord(TEXT_BG_COLOR);
    WriteWord(uColor_);
    COMMAND_FOOTER
}

//---------------------------------------------------------------------------
void GraphicsUOLED::SetTextOpacity(bool bOpaque_)
{
    COMMAND_HEADER
    WriteWord(TEXT_OPACITY);
    WriteWord((uint16_t)bOpaque_);
    COMMAND_FOOTER
}

//---------------------------------------------------------------------------
