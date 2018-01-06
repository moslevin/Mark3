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

#include "m3shell.h"

#include <stdint.h>
namespace Mark3 {
//---------------------------------------------------------------------------
void M3Shell::AddCommand(CommandHandler* pclHandler_)
{
    m_clDispatcher.AddCommand(pclHandler_);
}

//---------------------------------------------------------------------------
void M3Shell::SetPrompt(const char* szPrompt_)
{
    m_szPrompt = szPrompt_;
}

//---------------------------------------------------------------------------
void M3Shell::SetDefaultHandler(CommandHandler* pclHandler_)
{
    m_clDispatcher.SetDefaultHandler(pclHandler_);
}

//---------------------------------------------------------------------------
void M3Shell::SetDriver(Driver* pclDriver_)
{
    m_clTerminal.SetDriver(pclDriver_);
}

//---------------------------------------------------------------------------
void M3Shell::Init()
{
    SetPrompt("Mark3 $ ");
}

//---------------------------------------------------------------------------
void M3Shell::InputLoop()
{
    m_clLineBuffer.Clear();

    m_clTerminal.SetForeColor(TerminalColor::Green);
    m_clTerminal.Bold();
    m_clTerminal.WriteString(m_szPrompt);
    m_clTerminal.SetForeColor(TerminalColor::White);
    m_clTerminal.ResetAttributes();

    auto done = false;
    while (!done) {
        KeyVal_t stKey;
        if (m_clTerminal.ReadLoop()) {
            m_clTerminal.GetLastKey(&stKey);
            if (stKey.bEscaped) {
                if (stKey.eKey == TerminalKey::Backspace) {
                    if (m_clLineBuffer.Backspace()) {
                        m_clTerminal.Backspace();

                        const char* szSrc = m_clLineBuffer.GetBuffer();
                        uint8_t u8LastIndex = m_clLineBuffer.GetLastIndex();
                        uint8_t u8Cursor = m_clLineBuffer.GetCursorIndex();

                        for (uint8_t i = u8Cursor; i <= u8LastIndex; i++) {
                            m_clTerminal.WriteByte(szSrc[i]);
                        }

                        m_clTerminal.WriteByte(' ');
                        m_clTerminal.CursorLeft(u8LastIndex - u8Cursor + 1);
                    }
                } else if (stKey.eKey == TerminalKey::Left) {
                    if (m_clLineBuffer.CursorLeft()) {
                        m_clTerminal.CursorLeft(1);
                    }
                } else if (stKey.eKey == TerminalKey::Right) {
                    if (m_clLineBuffer.CursorRight()) {
                        m_clTerminal.CursorRight(1);
                    }
                } else if (stKey.eKey == TerminalKey::Home) {
                    uint8_t u8Shift = m_clLineBuffer.CursorHome();
                    if (u8Shift != 0u) {
                        m_clTerminal.CursorLeft(u8Shift);
                    }
                } else if (stKey.eKey == TerminalKey::End) {
                    uint8_t u8Shift = m_clLineBuffer.CursorEnd();
                    if (u8Shift != 0u) {
                        m_clTerminal.CursorRight(u8Shift);
                    }
                } else if (stKey.eKey == TerminalKey::Delete) {
                    m_clLineBuffer.Delete();
                    m_clTerminal.ClearFromCursor();

                    const char* szSrc = m_clLineBuffer.GetBuffer();
                    uint8_t u8LastIndex = m_clLineBuffer.GetLastIndex();
                    uint8_t u8Cursor = m_clLineBuffer.GetCursorIndex();

                    for (uint8_t i = u8Cursor; i <= u8LastIndex; i++) {
                        m_clTerminal.WriteByte(szSrc[i]);
                    }

                    m_clTerminal.WriteByte(' ');
                    m_clTerminal.CursorLeft(u8LastIndex - u8Cursor + 1);

                } else if (stKey.eKey == TerminalKey::Carriage_Return) {
                    m_clTerminal.WriteString("\r\n");
                    done = true;
                } else if (stKey.eKey == TerminalKey::Linefeed) {
                    m_clTerminal.WriteByte('\0');
                    done = true;
                }
            } else {
                m_clLineBuffer.WriteCharacter(stKey.cChar);
                m_clTerminal.WriteByte(stKey.cChar);

                m_clTerminal.ClearFromCursor();

                const char* szSrc = m_clLineBuffer.GetBuffer();
                uint8_t u8LastIndex = m_clLineBuffer.GetLastIndex();
                uint8_t u8Cursor = m_clLineBuffer.GetCursorIndex();

                if (u8Cursor != u8LastIndex) {
                    for (uint8_t i = u8Cursor; i <= u8LastIndex; i++) {
                        m_clTerminal.WriteByte(szSrc[i]);
                    }

                    m_clTerminal.CursorLeft(u8LastIndex - u8Cursor);
                }

                if (stKey.cChar == '\n') {
                    done = true;
                }
            }
        }
    }
    m_clDispatcher.Execute(m_clLineBuffer.GetBuffer());
    m_clTerminal.WriteString("\r\n");
}
} //namespace Mark3
