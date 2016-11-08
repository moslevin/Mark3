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

#include "terminal_in.h"

#include <stdint.h>

//---------------------------------------------------------------------------
bool TerminalIn::ReadLoop(void)
{
    uint8_t ch;
    if (ReadByte(&ch)) {
        return ParseByte(ch);
    }
    return false;
}

//---------------------------------------------------------------------------
bool TerminalIn::ParseByte(uint8_t u8Input_)
{
    return RunHandler(u8Input_);
}

//---------------------------------------------------------------------------
void TerminalIn::GetLastKey(KeyVal_t* pstKeyVal_)
{
    *pstKeyVal_ = m_uKey;
}

//---------------------------------------------------------------------------
bool TerminalIn::RunHandler(uint8_t u8Input_)
{
    switch (m_eState) {
    case terminal_parser_begin:                     return BeginHandler(u8Input_);
    case terminal_parser_escape_check:              return EscapeCheckHandler(u8Input_);
    case terminal_parser_escape_type:               return EscapeTypeHandler(u8Input_);
    case terminal_parser_escape:                    return EscapeHandler(u8Input_);
    case terminal_parser_escape_modified:           return EscapeModifierHandler(u8Input_);
    case terminal_parser_escape_expect_semi:        return ExpectSemiHandler(u8Input_);
    }
    return false;
}

//---------------------------------------------------------------------------
void TerminalIn::AcceptSpecial(terminal_key_t eKey_)
{
    m_uKey.eKey = eKey_;
    m_uKey.eModifier = (terminal_key_modifier_t)(m_u8Modifier);
    m_uKey.bEscaped = true;
    m_eState = terminal_parser_begin;
}

//---------------------------------------------------------------------------
void TerminalIn::AcceptCharacter(char cChar_)
{
    m_uKey.cChar = cChar_;
    m_uKey.eModifier = (terminal_key_modifier_t)(m_u8Modifier);
    m_uKey.bEscaped = false;
    m_eState = terminal_parser_begin;
}

//---------------------------------------------------------------------------
bool TerminalIn::BeginHandler(char cInput_)
{
    m_u8Modifier = 0;
    m_u8Command = 0;
    m_u8EscapeIdx = 0;

    // Non-printing ANSI terminal characters
    switch (cInput_) {
    case 0x07:
        AcceptSpecial(TERMINAL_KEY_BELL);
        return true;
    case 0x08:
        AcceptSpecial(TERMINAL_KEY_BACKSPACE);
        return true;
    case 0x09:
        AcceptSpecial(TERMINAL_KEY_TAB);
        return true;
    case 0x0A:
        AcceptSpecial(TERMINAL_KEY_LINEFEED);
        return true;
    case 0x0B:
        AcceptSpecial(TERMINAL_KEY_VTAB);
        return true;
    case 0x0C:
        AcceptSpecial(TERMINAL_KEY_FORMFEED);
        return true;
    case 0x0D:
        AcceptSpecial(TERMINAL_KEY_CARRIAGE_RETURN);
        return true;
    case 0x1B:
        m_eState = terminal_parser_escape_check;
        return false;
    case 0x7F:
        AcceptSpecial(TERMINAL_KEY_BACKSPACE);
        return true;
    }

    AcceptCharacter(cInput_);
    return true;
}

//---------------------------------------------------------------------------
bool TerminalIn::EscapeCheckHandler(char cInput_)
{
    if ((cInput_ == '^') || (cInput_ == '[') || (cInput_ == 'O')) {
        m_eState = terminal_parser_escape_type;
        m_cEscapeBegin = cInput_;
        return false;
    }

    m_eState = terminal_parser_begin;
    return false;
}

//---------------------------------------------------------------------------
bool TerminalIn::EscapeTypeHandler(char cInput_)
{
    if (m_cEscapeBegin == '^') {
        switch (cInput_) {
        case 'C': {
            AcceptSpecial(TERMINAL_KEY_BREAK);
            return true;
        }
        case 'G': {
            AcceptSpecial(TERMINAL_KEY_BELL);
            return true;
        }
        case 'H': {
            AcceptSpecial(TERMINAL_KEY_BACKSPACE);
            return true;
        }
        case 'I': {
            AcceptSpecial(TERMINAL_KEY_TAB);
            return true;
        }
        case '?': {
            AcceptSpecial(TERMINAL_KEY_BACKSPACE);
            return true;
        }
        }
    } else if (m_cEscapeBegin == 'O') {
        switch (cInput_) {
        case '1':
            m_eState = terminal_parser_escape_modified;
            return false;
        case 'P':
            AcceptSpecial(TERMINAL_KEY_F1);
            return true;
        case 'Q':
            AcceptSpecial(TERMINAL_KEY_F2);
            return true;
        case 'R':
            AcceptSpecial(TERMINAL_KEY_F3);
            return true;
        case 'S':
            AcceptSpecial(TERMINAL_KEY_F4);
            return true;
        case 'H':
            AcceptSpecial(TERMINAL_KEY_HOME_ROW);
            return true;
        case 'F':
            AcceptSpecial(TERMINAL_KEY_END_ROW);
            return true;
        }
    }

    switch (cInput_) {
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        m_au8EscapeSeq[0] = cInput_;
        m_u8EscapeIdx = 1;
        m_eState = terminal_parser_escape;
        return false;
    case 'A':
        AcceptSpecial(TERMINAL_KEY_UP);
        return true;
    case 'B':
        AcceptSpecial(TERMINAL_KEY_DOWN);
        return true;
    case 'C':
        AcceptSpecial(TERMINAL_KEY_RIGHT);
        return true;
    case 'D':
        AcceptSpecial(TERMINAL_KEY_LEFT);
        return true;
    case 'E':
        AcceptSpecial(TERMINAL_KEY_CENTER);
        return true;
    }

    m_eState = terminal_parser_begin;
    return false;
}

//---------------------------------------------------------------------------
bool TerminalIn::EscapeHandler(char cInput_)
{
    switch (cInput_) {
    case ';': {
        uint8_t cmd = 0;
        for (uint8_t i = 0; i < m_u8EscapeIdx; i++) {
            cmd = (cmd * 10) + (m_au8EscapeSeq[i] - '0');
        }

        m_u8Command = cmd;
        m_u8EscapeIdx = 0;
        return false;
    }
    case '~': {
        uint8_t cmd = 0;
        for (uint8_t i = 0; i < m_u8EscapeIdx; i++) {
            cmd = (cmd * 10) + (m_au8EscapeSeq[i] - '0');
        }
        if (!m_u8Command) {
            m_u8Command = cmd;
        } else {
            m_u8Modifier = cmd;
        }

        switch (m_u8Command) {
        case 1:
            AcceptSpecial(TERMINAL_KEY_HOME);
            return true;
        case 2:
            AcceptSpecial(TERMINAL_KEY_INSERT);
            return true;
        case 3:
            AcceptSpecial(TERMINAL_KEY_DELETE);
            return true;
        case 4:
            AcceptSpecial(TERMINAL_KEY_END);
            return true;
        case 5:
            AcceptSpecial(TERMINAL_KEY_PAGE_UP);
            return true;
        case 6:
            AcceptSpecial(TERMINAL_KEY_PAGE_DOWN);
            return true;
        case 15:
            AcceptSpecial(TERMINAL_KEY_F5);
            return true;
        case 17:
            AcceptSpecial(TERMINAL_KEY_F6);
            return true;
        case 18:
            AcceptSpecial(TERMINAL_KEY_F7);
            return true;
        case 19:
            AcceptSpecial(TERMINAL_KEY_F8);
            return true;
        case 20:
            AcceptSpecial(TERMINAL_KEY_F9);
            return true;
        case 21:
            AcceptSpecial(TERMINAL_KEY_F10);
            return true;
        case 23:
            AcceptSpecial(TERMINAL_KEY_F11);
            return true;
        case 24:
            AcceptSpecial(TERMINAL_KEY_F12);
            return true;
        default:
            break;
        }

        m_eState = terminal_parser_begin;
        return false;
    }

    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        if (m_u8EscapeIdx > 2) {
            m_eState = terminal_parser_begin;
            return false;
        }
        m_au8EscapeSeq[m_u8EscapeIdx++] = cInput_;
        return false;
    case 'A':
        if (m_u8Command == 1) {
            m_u8Modifier = m_au8EscapeSeq[0] - '0';
            AcceptSpecial(TERMINAL_KEY_UP);
            return true;
        }
        break;
    case 'B':
        if (m_u8Command == 1) {
            m_u8Modifier = m_au8EscapeSeq[0] - '0';
            AcceptSpecial(TERMINAL_KEY_DOWN);
            return true;
        }
        break;
    case 'C':
        if (m_u8Command == 1) {
            m_u8Modifier = m_au8EscapeSeq[0] - '0';
            AcceptSpecial(TERMINAL_KEY_RIGHT);
            return true;
        }
        break;
    case 'D':
        if (m_u8Command == 1) {
            m_u8Modifier = m_au8EscapeSeq[0] - '0';
            AcceptSpecial(TERMINAL_KEY_LEFT);
            return true;
        }
        break;
    case 'E':
        if (m_u8Command == 1) {
            m_u8Modifier = m_au8EscapeSeq[0] - '0';
            AcceptSpecial(TERMINAL_KEY_CENTER);
            return true;
        }
        break;
    case 'H':
        if (m_u8Command == 1) {
            m_u8Modifier = m_au8EscapeSeq[0] - '0';
            AcceptSpecial(TERMINAL_KEY_HOME_ROW);
            return true;
        }
        break;
    case 'F':
        if (m_u8Command == 1) {
            m_u8Modifier = m_au8EscapeSeq[0] - '0';
            AcceptSpecial(TERMINAL_KEY_END_ROW);
            return true;
        }
        break;
    }

    m_eState = terminal_parser_begin;
    return false;
}

//---------------------------------------------------------------------------
bool TerminalIn::EscapeModifierHandler(char cInput_)
{
    if (cInput_ == ';') {
        m_eState = terminal_parser_escape_expect_semi;
        return false;
    }
    m_eState = terminal_parser_begin;
    return false;
}

//---------------------------------------------------------------------------
bool TerminalIn::ExpectSemiHandler(char cInput_)
{
    switch (cInput_) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            m_u8Modifier = cInput_ - '0';
            m_eState = terminal_parser_escape_type;
            return false;
    }

    m_eState = terminal_parser_begin;
    return false;
}
