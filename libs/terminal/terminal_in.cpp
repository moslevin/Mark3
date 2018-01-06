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

#include "terminal_in.h"
#include "driver.h"

#include <stdint.h>
namespace Mark3 {
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
    case TerminalParser::Begin:                     return BeginHandler(u8Input_);
    case TerminalParser::Check:              return EscapeCheckHandler(u8Input_);
    case TerminalParser::Type:               return EscapeTypeHandler(u8Input_);
    case TerminalParser::Escape:                    return EscapeHandler(u8Input_);
    case TerminalParser::Escape_Modified:           return EscapeModifierHandler(u8Input_);
    case TerminalParser::Escape_Expect_Semi:        return ExpectSemiHandler(u8Input_);
    case TerminalParser::Escape_Expect_Modifier:    return false;
    }
    return false;
}

//---------------------------------------------------------------------------
void TerminalIn::AcceptSpecial(TerminalKey eKey_)
{
    m_uKey.eKey = eKey_;
    m_uKey.eModifier = static_cast<TerminalKeyModifier>(m_u8Modifier);
    m_uKey.bEscaped = true;
    m_eState = TerminalParser::Begin;
}

//---------------------------------------------------------------------------
void TerminalIn::AcceptCharacter(char cChar_)
{
    m_uKey.cChar = cChar_;
    m_uKey.eModifier = static_cast<TerminalKeyModifier>(m_u8Modifier);
    m_uKey.bEscaped = false;
    m_eState = TerminalParser::Begin;
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
        AcceptSpecial(TerminalKey::Bell);
        return true;
    case 0x08:
        AcceptSpecial(TerminalKey::Backspace);
        return true;
    case 0x09:
        AcceptSpecial(TerminalKey::Tab);
        return true;
    case 0x0A:
        AcceptSpecial(TerminalKey::Linefeed);
        return true;
    case 0x0B:
        AcceptSpecial(TerminalKey::Vtab);
        return true;
    case 0x0C:
        AcceptSpecial(TerminalKey::Formfeed);
        return true;
    case 0x0D:
        AcceptSpecial(TerminalKey::Carriage_Return);
        return true;
    case 0x1B:
        m_eState = TerminalParser::Check;
        return false;
    case 0x7F:
        AcceptSpecial(TerminalKey::Backspace);
        return true;
    }

    AcceptCharacter(cInput_);
    return true;
}

//---------------------------------------------------------------------------
bool TerminalIn::EscapeCheckHandler(char cInput_)
{
    if ((cInput_ == '^') || (cInput_ == '[') || (cInput_ == 'O')) {
        m_eState = TerminalParser::Type;
        m_cEscapeBegin = cInput_;
        return false;
    }

    m_eState = TerminalParser::Begin;
    return false;
}

//---------------------------------------------------------------------------
bool TerminalIn::EscapeTypeHandler(char cInput_)
{
    if (m_cEscapeBegin == '^') {
        switch (cInput_) {
        case 'C': {
            AcceptSpecial(TerminalKey::Break);
            return true;
        }
        case 'G': {
            AcceptSpecial(TerminalKey::Bell);
            return true;
        }
        case 'H': {
            AcceptSpecial(TerminalKey::Backspace);
            return true;
        }
        case 'I': {
            AcceptSpecial(TerminalKey::Tab);
            return true;
        }
        case '?': {
            AcceptSpecial(TerminalKey::Backspace);
            return true;
        }
        }
    } else if (m_cEscapeBegin == 'O') {
        switch (cInput_) {
        case '1':
            m_eState = TerminalParser::Escape_Modified;
            return false;
        case 'P':
            AcceptSpecial(TerminalKey::F1);
            return true;
        case 'Q':
            AcceptSpecial(TerminalKey::F2);
            return true;
        case 'R':
            AcceptSpecial(TerminalKey::F3);
            return true;
        case 'S':
            AcceptSpecial(TerminalKey::F4);
            return true;
        case 'H':
            AcceptSpecial(TerminalKey::Home_Row);
            return true;
        case 'F':
            AcceptSpecial(TerminalKey::End_Row);
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
        m_eState = TerminalParser::Escape;
        return false;
    case 'A':
        AcceptSpecial(TerminalKey::Up);
        return true;
    case 'B':
        AcceptSpecial(TerminalKey::Down);
        return true;
    case 'C':
        AcceptSpecial(TerminalKey::Right);
        return true;
    case 'D':
        AcceptSpecial(TerminalKey::Left);
        return true;
    case 'E':
        AcceptSpecial(TerminalKey::Center);
        return true;
    }

    m_eState = TerminalParser::Begin;
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
        if (m_u8Command == 0u) {
            m_u8Command = cmd;
        } else {
            m_u8Modifier = cmd;
        }

        switch (m_u8Command) {
        case 1:
            AcceptSpecial(TerminalKey::Home);
            return true;
        case 2:
            AcceptSpecial(TerminalKey::Insert);
            return true;
        case 3:
            AcceptSpecial(TerminalKey::Delete);
            return true;
        case 4:
            AcceptSpecial(TerminalKey::End);
            return true;
        case 5:
            AcceptSpecial(TerminalKey::Page_Up);
            return true;
        case 6:
            AcceptSpecial(TerminalKey::Page_Down);
            return true;
        case 15:
            AcceptSpecial(TerminalKey::F5);
            return true;
        case 17:
            AcceptSpecial(TerminalKey::F6);
            return true;
        case 18:
            AcceptSpecial(TerminalKey::F7);
            return true;
        case 19:
            AcceptSpecial(TerminalKey::F8);
            return true;
        case 20:
            AcceptSpecial(TerminalKey::F9);
            return true;
        case 21:
            AcceptSpecial(TerminalKey::F10);
            return true;
        case 23:
            AcceptSpecial(TerminalKey::F11);
            return true;
        case 24:
            AcceptSpecial(TerminalKey::F12);
            return true;
        default:
            break;
        }

        m_eState = TerminalParser::Begin;
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
            m_eState = TerminalParser::Begin;
            return false;
        }
        m_au8EscapeSeq[m_u8EscapeIdx++] = cInput_;
        return false;
    case 'A':
        if (m_u8Command == 1) {
            m_u8Modifier = m_au8EscapeSeq[0] - '0';
            AcceptSpecial(TerminalKey::Up);
            return true;
        }
        break;
    case 'B':
        if (m_u8Command == 1) {
            m_u8Modifier = m_au8EscapeSeq[0] - '0';
            AcceptSpecial(TerminalKey::Down);
            return true;
        }
        break;
    case 'C':
        if (m_u8Command == 1) {
            m_u8Modifier = m_au8EscapeSeq[0] - '0';
            AcceptSpecial(TerminalKey::Right);
            return true;
        }
        break;
    case 'D':
        if (m_u8Command == 1) {
            m_u8Modifier = m_au8EscapeSeq[0] - '0';
            AcceptSpecial(TerminalKey::Left);
            return true;
        }
        break;
    case 'E':
        if (m_u8Command == 1) {
            m_u8Modifier = m_au8EscapeSeq[0] - '0';
            AcceptSpecial(TerminalKey::Center);
            return true;
        }
        break;
    case 'H':
        if (m_u8Command == 1) {
            m_u8Modifier = m_au8EscapeSeq[0] - '0';
            AcceptSpecial(TerminalKey::Home_Row);
            return true;
        }
        break;
    case 'F':
        if (m_u8Command == 1) {
            m_u8Modifier = m_au8EscapeSeq[0] - '0';
            AcceptSpecial(TerminalKey::End_Row);
            return true;
        }
        break;
    }

    m_eState = TerminalParser::Begin;
    return false;
}

//---------------------------------------------------------------------------
bool TerminalIn::EscapeModifierHandler(char cInput_)
{
    if (cInput_ == ';') {
        m_eState = TerminalParser::Escape_Expect_Semi;
        return false;
    }
    m_eState = TerminalParser::Begin;
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
            m_eState = TerminalParser::Type;
            return false;
    }

    m_eState = TerminalParser::Begin;
    return false;
}
} //namespace Mark3
