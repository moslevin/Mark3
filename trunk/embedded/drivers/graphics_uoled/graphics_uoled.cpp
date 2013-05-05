#include "kerneltypes.h"
#include "graphics_uoled.h"
#include "uoled_defines.h"
#include "thread.h"

//---------------------------------------------------------------------------
void GraphicsUOLED::WriteByte( K_UCHAR ucByte_ )
{
    while (!m_pclDriver->Write(1, &ucByte_)) { /* do nothing */ }
}

//---------------------------------------------------------------------------
void GraphicsUOLED::WriteWord( K_USHORT usWord_ )
{
    K_UCHAR ucByte = (K_UCHAR)(usWord_ >> 8);
    while (!m_pclDriver->Write(1, &ucByte)) { /* do nothing */ }

    ucByte = (K_UCHAR)usWord_;
    while (!m_pclDriver->Write(1, &ucByte)) { /* do nothing */ }
}

//---------------------------------------------------------------------------
K_UCHAR GraphicsUOLED::ReadByte( void )
{
    K_UCHAR ucResponse;
    while (!m_pclDriver->Read(1, &ucResponse)) { /* do nothing */ }

    return ucResponse;
}

//---------------------------------------------------------------------------
K_UCHAR GraphicsUOLED::WaitAck( void )
{
    K_USHORT usTimeout = SERIAL_TIMEOUT_MAX;
    K_UCHAR ucResponse = 0;
    
    while (usTimeout--)
    {
        if(m_pclDriver->Read(1, &ucResponse))
        {
            break;
        }
        Thread::USleep(1000);   //100us sleep
    }

    return ucResponse;
}


//---------------------------------------------------------------------------
void GraphicsUOLED::WriteVector( DataVector_t *pstVector_, K_UCHAR ucCount_)
{
    K_UCHAR ucIndex = 0;
    while (ucIndex < ucCount_)
    {
        if (1 == pstVector_->ucLen)
        {
            WriteByte(pstVector_->usData);
        }
        else if (2 == pstVector_->ucLen)
        {
            WriteWord(pstVector_->usData);
        }
        ucIndex++;
        pstVector_++;
    }
}

//---------------------------------------------------------------------------
#define COMMAND_HEADER  \
{ \
    K_UCHAR ucRetry = COMMAND_RETRY_COUNT;  \
    while (ucRetry--) \
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

    astVector[0].usData = GFX_PUT_PIXEL;
    astVector[0].ucLen = 2;
    astVector[1].usData = pstPoint_->usX;
    astVector[1].ucLen = 2;
    astVector[2].usData = pstPoint_->usY;
    astVector[2].ucLen = 2;
    astVector[3].usData = pstPoint_->uColor;
    astVector[3].ucLen = 2;

    WriteVector(astVector, 4);

    COMMAND_FOOTER
}

//---------------------------------------------------------------------------
void GraphicsUOLED::Line(DrawLine_t *pstLine_)
{
    COMMAND_HEADER
    
    DataVector_t astVector[6];

    astVector[0].usData = GFX_DRAW_LINE;
    astVector[0].ucLen = 2;
    astVector[1].usData = pstLine_->usX1;
    astVector[1].ucLen = 2;
    astVector[2].usData = pstLine_->usY1;
    astVector[2].ucLen = 2;
    astVector[3].usData = pstLine_->usX2;
    astVector[3].ucLen = 2;
    astVector[4].usData = pstLine_->usY2;
    astVector[4].ucLen = 2;
    astVector[5].usData = pstLine_->uColor;
    astVector[5].ucLen = 2;

    WriteVector(astVector, 6);

    COMMAND_FOOTER
}

//---------------------------------------------------------------------------
void GraphicsUOLED::SetOutlineColor( COLOR uColor_ )
{
    COMMAND_HEADER

    DataVector_t astVector[2];

    astVector[0].usData = GFX_SET_OUTLINE_COLOR;
    astVector[1].usData = uColor_;

    WriteVector(astVector, 2);

    COMMAND_FOOTER
}

//---------------------------------------------------------------------------
void GraphicsUOLED::Circle(DrawCircle_t *pstCircle_)
{
    if (pstCircle_->bFill)
    {
        SetOutlineColor(pstCircle_->uLineColor);
    }    

    COMMAND_HEADER
    
    DataVector_t astVector[5];

    if (pstCircle_->bFill)
    {
        astVector[0].usData = GFX_DRAW_CIRCLE_FILLED;
    }
    else
    {
        astVector[0].usData = GFX_DRAW_CIRCLE;
    }
    astVector[0].ucLen = 2;

    astVector[1].usData = pstCircle_->usX;
    astVector[1].ucLen = 2;
    astVector[2].usData = pstCircle_->usY;
    astVector[2].ucLen = 2;
    astVector[3].usData = pstCircle_->usRadius;
    astVector[3].ucLen = 2;
    
    if (pstCircle_->bFill)
    {
        astVector[4].usData = pstCircle_->uFillColor;
    }
    else
    {
        astVector[4].usData = pstCircle_->uLineColor;
    }
    astVector[4].ucLen = 2;

    WriteVector(astVector, 5);

    COMMAND_FOOTER
}

//---------------------------------------------------------------------------
void GraphicsUOLED::Rectangle(DrawRectangle_t *pstRectangle_)
{
    if (pstRectangle_->bFill)
    {
        SetOutlineColor(pstRectangle_->uLineColor);
    }    

    COMMAND_HEADER
    
    DataVector_t astVector[6];

    if (pstRectangle_->bFill)
    {
        astVector[0].usData = GFX_DRAW_RECTANGLE_FILLED;
    }
    else
    {
        astVector[0].usData = GFX_DRAW_RECTANGLE;
    }
    astVector[0].ucLen = 2;

    astVector[1].usData = pstRectangle_->usLeft;
    astVector[1].ucLen = 2;
    astVector[2].usData = pstRectangle_->usTop;
    astVector[2].ucLen = 2;
    astVector[3].usData = pstRectangle_->usRight;
    astVector[3].ucLen = 2;
    astVector[4].usData = pstRectangle_->usBottom;
    astVector[4].ucLen = 2;

    if (pstRectangle_->bFill)
    {
        astVector[5].usData = pstRectangle_->uFillColor;
    }
    else
    {
        astVector[5].usData = pstRectangle_->uLineColor;
    }
    astVector[5].ucLen = 2;

    WriteVector(astVector, 6);

    COMMAND_FOOTER
}

//---------------------------------------------------------------------------
void GraphicsUOLED::MoveCursor(K_USHORT usX_, K_USHORT usY_)
{
    COMMAND_HEADER
    DataVector_t astVector[3];

    astVector[0].usData = TEXT_MOVE_CURSOR;
    astVector[0].ucLen = 2;
    astVector[1].usData = (usX_ + 4) >> 3;
    astVector[1].ucLen = 2;
    astVector[2].usData = (usY_ + 4) >> 3;
    astVector[2].ucLen = 2;

    WriteVector(astVector, 3);
    COMMAND_FOOTER

}

//---------------------------------------------------------------------------
void GraphicsUOLED::Text(DrawText_t *pstText_)
{
    MoveCursor(pstText_->usLeft, pstText_->usTop);

    COMMAND_HEADER
    const K_CHAR *pcCursor = pstText_->pcString;

    WriteWord(TEXT_PUT_STRING);
    while (*pcCursor)
    {
        WriteByte(*pcCursor++);
    }
    WriteByte(0);
    COMMAND_FOOTER
}

//---------------------------------------------------------------------------
K_USHORT GraphicsUOLED::TextWidth(DrawText_t *pstText_)
{
    K_USHORT usRetVal = 0;
    const K_CHAR *pcCursor = pstText_->pcString;

    while (*pcCursor)
    {
        usRetVal += 8;
    }
    return usRetVal;
}
