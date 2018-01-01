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
=========================================================================== */

#include "linebuffer.h"

#include <stdint.h>

#include "memutil.h"
namespace Mark3 {
//---------------------------------------------------------------------------
void LineBuffer::Clear()
{
    // Zero buffer
    MemUtil::SetMemory(m_cLineBuffer, 0, sizeof(m_cLineBuffer));
    m_u8LastByteIndex = 0;
    m_u8CursorIndex = 0;
}

//---------------------------------------------------------------------------
bool LineBuffer::WriteCharacter(char cByte_)
{
    // Insert character at cursor
    if (m_u8LastByteIndex >= (sizeof(m_cLineBuffer) -1)) {
        return false;
    }

    // Move all characters to the right of the cursor right one byte
    if (m_u8LastByteIndex != m_u8CursorIndex) {
        uint8_t i = m_u8LastByteIndex + 1;
        while (i != m_u8CursorIndex) {
            m_cLineBuffer[i] = m_cLineBuffer[i - 1];
            i--;
        }
    }

    m_cLineBuffer[m_u8CursorIndex++] = cByte_;
    m_cLineBuffer[++m_u8LastByteIndex] = '\0';

    return true;
}

//---------------------------------------------------------------------------
bool LineBuffer::Backspace()
{
    if (m_u8CursorIndex == 0) {
        return false;
    }

    // Move all characters to the right of the cursor right one byte
    if (m_u8LastByteIndex != m_u8CursorIndex) {
        uint8_t i = m_u8CursorIndex;
        while (i <= m_u8LastByteIndex) {
            m_cLineBuffer[i - 1] = m_cLineBuffer[i];
            i++;
        }
    }

    m_u8CursorIndex--;
    m_cLineBuffer[--m_u8LastByteIndex] = '\0';

    return true;
}

//---------------------------------------------------------------------------
uint8_t LineBuffer::Delete()
{
    if (m_u8CursorIndex == m_u8LastByteIndex) {
        return 0;
    }

    // move all trailing characters left one byte.
    if (m_u8LastByteIndex != m_u8CursorIndex) {
        uint8_t i = m_u8CursorIndex;
        while (i != m_u8LastByteIndex) {
            m_cLineBuffer[i] = m_cLineBuffer[i+1];
            i++;
        }
    }

    m_cLineBuffer[m_u8LastByteIndex--] = '\0';
    if (m_u8LastByteIndex < m_u8CursorIndex) {
        m_u8CursorIndex = m_u8LastByteIndex;
    }
    return (m_u8LastByteIndex - m_u8CursorIndex);
}

//---------------------------------------------------------------------------
bool LineBuffer::CursorLeft()
{
    if (m_u8CursorIndex != 0) {
        m_u8CursorIndex--;
        return true;
    }
    return false;
}

//---------------------------------------------------------------------------
bool LineBuffer::CursorRight()
{
    if (m_u8CursorIndex < m_u8LastByteIndex) {
        m_u8CursorIndex++;
        return true;
    }
    return false;
}

//---------------------------------------------------------------------------
uint8_t LineBuffer::CursorHome()
{
    uint8_t u8Diff = m_u8CursorIndex;
    m_u8CursorIndex = 0;
    return u8Diff;
}

//---------------------------------------------------------------------------
uint8_t LineBuffer::CursorEnd()
{
    uint8_t u8Diff = m_u8LastByteIndex - m_u8CursorIndex;
    m_u8CursorIndex = m_u8LastByteIndex;
    return u8Diff;
}
} //namespace Mark3
