/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012-2016 Funkenstein Software Consulting, all rights reserved.
See license.txt for more information
===========================================================================*/
#include "terminal.h"
#include "memutil.h"

#include <stdint.h>

void Terminal::Escape(void)
{
    uint16_t u16Written = 0;
    uint8_t u8Val = 0x1B;
    while (!u16Written) 
    {
        u16Written = m_pclDriver->Write(1, &u8Val);
    }
}

void Terminal::PrintChar(uint8_t u8Val_)
{
    uint16_t u16Written = 0;
    while (!u16Written) 
    {
        u16Written = m_pclDriver->Write(1, &u8Val_);
    }
}

void Terminal::PrintVal(uint8_t u8Val_)
{
    char tmp[4];
    MemUtil::DecimalToString(u8Val_, tmp);
    char *src = tmp;
    while (*src) 
    {
        uint16_t u16Written = 0;
        while (!u16Written) 
        {
            u16Written = m_pclDriver->Write(1,(uint8_t*)src);
        }
        src++;
    }
}

void Terminal::CursorUp(uint8_t u8Rows_)
{
    Escape();
    PrintChar('[');
    PrintVal(u8Rows_);
    PrintChar('A');
}

void Terminal::CursorDown(uint8_t u8Rows_)
{
    Escape();
    PrintChar('[');
    PrintVal(u8Rows_);
    PrintChar('B');
}

void Terminal::CursorLeft(uint8_t u8Columns_)
{
    Escape();
    PrintChar('[');
    PrintVal(u8Columns_);
    PrintChar('C');
}

void Terminal::CursorRight(uint8_t u8Columns_)
{
    Escape();
    PrintChar('[');
    PrintVal(u8Columns_);
    PrintChar('D');
}

void Terminal::SetCursorPos(uint8_t u8X_, uint8_t u8Y_)
{
    Escape();
    PrintChar('[');
    PrintVal(u8X_);
    PrintChar(';');
    PrintVal(u8X_);
    PrintChar('f');    
}

void Terminal::CursorHome(void)
{
    Escape();
    PrintChar('[');
    PrintChar('H');
}

void Terminal::SaveCursor(void)
{
    Escape();
    PrintChar('[');
    PrintChar('s');
}

void Terminal::RestoreCursor(void)
{
    Escape();
    PrintChar('[');
    PrintChar('u');
}

void Terminal::Backspace(void)
{
    PrintChar('\b');
    ClearFromCursor();
}

void Terminal::CursorVisible(void)
{
    Escape();
    PrintChar('[');
    PrintChar('?');
    PrintVal(25);
    PrintChar('h');
}

void Terminal::CursorInvisible(void)
{
    Escape();
    PrintChar('[');
    PrintChar('?');
    PrintVal(25);
    PrintChar('l');
}

// Erasing text
void Terminal::ClearScreen(void)
{
    Escape();
    PrintChar('[');
    PrintChar('2');
    PrintChar('J');
    CursorHome();
}

void Terminal::ClearLine(void)
{
    Escape();
    PrintChar('[');
    PrintChar('2');
    PrintChar('K');
}

void Terminal::ClearToCursor(void)
{
    Escape();
    PrintChar('[');
    PrintChar('1');
    PrintChar('K');
}

void Terminal::ClearFromCursor(void)
{
    Escape();
    PrintChar('[');
    PrintChar('K');
}

// Text attributes
void Terminal::ResetAttributes(void)
{
    Escape();
    PrintChar('[');
    PrintChar('0');
    PrintChar('m');
}

void Terminal::Bold(void)
{
    Escape();
    PrintChar('[');
    PrintChar('1');
    PrintChar('m');
}

void Terminal::Dim(void)
{
    Escape();
    PrintChar('[');
    PrintChar('2');
    PrintChar('m');
}

void Terminal::Underscore(void)
{
    Escape();
    PrintChar('[');
    PrintChar('4');
    PrintChar('m');
}

void Terminal::Blink(void)
{
    Escape();
    PrintChar('[');
    PrintChar('5');
    PrintChar('m');
}

void Terminal::Reverse(void)
{
    Escape();
    PrintChar('[');
    PrintChar('7');
    PrintChar('m');
}

void Terminal::Hidden(void)
{
    Escape();
    PrintChar('[');
    PrintChar('8');
    PrintChar('m');
}

// Foreground coloring
void Terminal::SetForeColor(terminal_color_t eColor_)
{
    Escape();
    PrintChar('[');
    PrintChar('3');
    PrintChar('0' + eColor_);
    PrintChar('m');
}

// Background coloring
void Terminal::SetBackColor(terminal_color_t eColor_)
{
    Escape();
    PrintChar('[');
    PrintChar('4');
    PrintChar('0' + eColor_);
    PrintChar('m');
}

