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

#ifndef __LINEBUFFER_H__
#define __LINEBUFFER_H__

#include <stdint.h>

#define LINEBUFFER_LENGTH           (80)

//---------------------------------------------------------------------------
class LineBuffer {
public:
    void Clear();

    bool WriteCharacter(char cByte_);

    bool Backspace();

    uint8_t Delete();

    bool CursorLeft();

    bool CursorRight();

    uint8_t CursorHome();

    uint8_t CursorEnd();

    uint8_t GetCursorIndex() { return m_u8CursorIndex; }
    uint8_t GetLastIndex() { return m_u8LastByteIndex; }
    const char* GetBuffer() { return m_cLineBuffer; }

private:
    char m_cLineBuffer[LINEBUFFER_LENGTH];
    uint8_t m_u8LastByteIndex;
    uint8_t m_u8CursorIndex;
};

#endif //__LINEBUFFER_H__
