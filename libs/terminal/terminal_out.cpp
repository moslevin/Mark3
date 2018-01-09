/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012 - 2017 Funkenstein Software Consulting, all rights reserved.
See license.txt for more information
===========================================================================*/
#include "terminal_out.h"
#include "memutil.h"
#include "driver.h"

#include <stdint.h>
namespace Mark3 {

//---------------------------------------------------------------------------
void TerminalOut::Escape(void)
{
    WriteByte(0x1B);
}

//---------------------------------------------------------------------------
void TerminalOut::WriteString(const char* szStr_)
{
    auto *szSrc = szStr_;
    while (*szSrc != '\0') {
        WriteByte(static_cast<uint8_t>(*szSrc++));
    }
}

//---------------------------------------------------------------------------
void TerminalOut::WriteVal(uint8_t u8Val_)
{
    char tmp[4];
    MemUtil::DecimalToString(u8Val_, tmp);
    auto* src = tmp;
    while (*src != 0) {
        WriteByte(*src);
        src++;
    }
}

//---------------------------------------------------------------------------
void TerminalOut::CursorUp(uint8_t u8Rows_)
{
    Escape();
    WriteByte('[');
    WriteVal(u8Rows_);
    WriteByte('A');
}

//---------------------------------------------------------------------------
void TerminalOut::CursorDown(uint8_t u8Rows_)
{
    Escape();
    WriteByte('[');
    WriteVal(u8Rows_);
    WriteByte('B');
}

//---------------------------------------------------------------------------
void TerminalOut::CursorLeft(uint8_t u8Columns_)
{
    Escape();
    WriteByte('[');
    WriteVal(u8Columns_);
    WriteByte('D');
}

//---------------------------------------------------------------------------
void TerminalOut::CursorRight(uint8_t u8Columns_)
{
    Escape();
    WriteByte('[');
    WriteVal(u8Columns_);
    WriteByte('C');
}

//---------------------------------------------------------------------------
void TerminalOut::SetCursorPos(uint8_t u8X_, uint8_t  /*u8Y_*/)
{
    Escape();
    WriteByte('[');
    WriteVal(u8X_);
    WriteByte(';');
    WriteVal(u8X_);
    WriteByte('f');
}

//---------------------------------------------------------------------------
void TerminalOut::CursorHome(void)
{
    Escape();
    WriteByte('[');
    WriteByte('H');
}

//---------------------------------------------------------------------------
void TerminalOut::SaveCursor(void)
{
    Escape();
    WriteByte('[');
    WriteByte('s');
}

//---------------------------------------------------------------------------
void TerminalOut::RestoreCursor(void)
{
    Escape();
    WriteByte('[');
    WriteByte('u');
}

//---------------------------------------------------------------------------
void TerminalOut::Backspace(void)
{
    WriteByte(0x7F);
    ClearFromCursor();
}

//---------------------------------------------------------------------------
void TerminalOut::CursorVisible(void)
{
    Escape();
    WriteByte('[');
    WriteByte('?');
    WriteVal(25);
    WriteByte('h');
}

//---------------------------------------------------------------------------
void TerminalOut::CursorInvisible(void)
{
    Escape();
    WriteByte('[');
    WriteByte('?');
    WriteVal(25);
    WriteByte('l');
}

//---------------------------------------------------------------------------
// Erasing text
void TerminalOut::ClearScreen(void)
{
    Escape();
    WriteByte('[');
    WriteByte('2');
    WriteByte('J');
    CursorHome();
}

//---------------------------------------------------------------------------
void TerminalOut::ClearLine(void)
{
    Escape();
    WriteByte('[');
    WriteByte('2');
    WriteByte('K');
}

//---------------------------------------------------------------------------
void TerminalOut::ClearToCursor(void)
{
    Escape();
    WriteByte('[');
    WriteByte('1');
    WriteByte('K');
}

//---------------------------------------------------------------------------
void TerminalOut::ClearFromCursor(void)
{
    Escape();
    WriteByte('[');
    WriteByte('K');
}

//---------------------------------------------------------------------------
// Text attributes
void TerminalOut::ResetAttributes(void)
{
    Escape();
    WriteByte('[');
    WriteByte('0');
    WriteByte('m');
}

//---------------------------------------------------------------------------
void TerminalOut::Bold(void)
{
    Escape();
    WriteByte('[');
    WriteByte('1');
    WriteByte('m');
}

//---------------------------------------------------------------------------
void TerminalOut::Dim(void)
{
    Escape();
    WriteByte('[');
    WriteByte('2');
    WriteByte('m');
}

//---------------------------------------------------------------------------
void TerminalOut::Underscore(void)
{
    Escape();
    WriteByte('[');
    WriteByte('4');
    WriteByte('m');
}

//---------------------------------------------------------------------------
void TerminalOut::Blink(void)
{
    Escape();
    WriteByte('[');
    WriteByte('5');
    WriteByte('m');
}

//---------------------------------------------------------------------------
void TerminalOut::Reverse(void)
{
    Escape();
    WriteByte('[');
    WriteByte('7');
    WriteByte('m');
}

//---------------------------------------------------------------------------
void TerminalOut::Hidden(void)
{
    Escape();
    WriteByte('[');
    WriteByte('8');
    WriteByte('m');
}

//---------------------------------------------------------------------------
// Foreground coloring
void TerminalOut::SetForeColor(TerminalColor eColor_)
{
    Escape();
    WriteByte('[');
    WriteByte('3');
    WriteByte('0' + static_cast<uint8_t>(eColor_));
    WriteByte('m');
}

//---------------------------------------------------------------------------
// Background coloring
void TerminalOut::SetBackColor(TerminalColor eColor_)
{
    Escape();
    WriteByte('[');
    WriteByte('4');
    WriteByte('0' + static_cast<uint8_t>(eColor_));
    WriteByte('m');
}
} //namespace Mark3
