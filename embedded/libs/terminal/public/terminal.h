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
=========================================================================== */

#ifndef __TERMINAL_H__
#define __TERMIANL_H__

#include "mark3cfg.h"
#include "mark3.h"
#include <stdint.h>

typedef enum {
    TERMINAL_COLOR_BLACK,
    TERMINAL_COLOR_RED,
    TERMINAL_COLOR_GREEN,
    TERMINAL_COLOR_YELLOW,
    TERMINAL_COLOR_BLUE,
    TERMINAL_COLOR_MAGENTA,
    TERMINAL_COLOR_CYAN,
    TERMINAL_COLOR_WHITE,
    TERMINAL_COLOR_DEFAULT
} terminal_color_t;

class Terminal
{
public:
    void SetDriver(Driver* pclDriver_) { m_pclDriver = pclDriver_; }
    
    // Cursor commands
    void CursorUp(uint8_t u8Rows_);
    void CursorDown(uint8_t u8Rows_);
    void CursorLeft(uint8_t u8Columns_);
    void CursorRight(uint8_t u8Columns_);

    void SetCursorPos(uint8_t u8X_, uint8_t u8Y_);
    void CursorHome(void);
    void SaveCursor(void);
    void RestoreCursor(void);
    void Backspace(void);
    void CursorVisible(void);
    void CursorInvisible(void);

    // Erasing text
    void ClearScreen(void);
    void ClearLine(void);
    void ClearToCursor(void);
    void ClearFromCursor(void);

    // Text attributes
    void ResetAttributes(void);
    void Bold(void);
    void Dim(void);
    void Underscore(void);
    void Blink(void);
    void Reverse(void);
    void Hidden(void);

    // Foreground coloring
    void SetForeColor(terminal_color_t eColor_);

    // Background coloring
    void SetBackColor(terminal_color_t eColor_);

private:
    void Escape(void);
    void PrintChar(uint8_t u8Val_);
    void PrintVal(uint8_t u8Val_);

    Driver* m_pclDriver;
};


#endif //__TERMINAL_H__

