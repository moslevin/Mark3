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

#ifndef __TERMINAL_IN_H__
#define __TERMINAL_IN_H__

#include <stdint.h>

//---------------------------------------------------------------------------
typedef enum {
    TERMINAL_KEY_BREAK = 0,
    TERMINAL_KEY_BELL,
    TERMINAL_KEY_BACKSPACE,
    TERMINAL_KEY_TAB,
    TERMINAL_KEY_VTAB,
    TERMINAL_KEY_LINEFEED,
    TERMINAL_KEY_CARRIAGE_RETURN,
    TERMINAL_KEY_FORMFEED,
    TERMINAL_KEY_F1,
    TERMINAL_KEY_F2,
    TERMINAL_KEY_F3,
    TERMINAL_KEY_F4,
    TERMINAL_KEY_HOME_ROW,
    TERMINAL_KEY_END_ROW,
    TERMINAL_KEY_UP,
    TERMINAL_KEY_DOWN,
    TERMINAL_KEY_RIGHT,
    TERMINAL_KEY_LEFT,
    TERMINAL_KEY_CENTER,
    TERMINAL_KEY_HOME,
    TERMINAL_KEY_INSERT,
    TERMINAL_KEY_DELETE,
    TERMINAL_KEY_END,
    TERMINAL_KEY_PAGE_UP,
    TERMINAL_KEY_PAGE_DOWN,
    TERMINAL_KEY_F5,
    TERMINAL_KEY_F6,
    TERMINAL_KEY_F7,
    TERMINAL_KEY_F8,
    TERMINAL_KEY_F9,
    TERMINAL_KEY_F10,
    TERMINAL_KEY_F11,
    TERMINAL_KEY_F12
} terminal_key_t;

//---------------------------------------------------------------------------
typedef enum {
    terminal_parser_begin = 0,
    terminal_parser_escape_check,
    terminal_parser_escape_type,
    terminal_parser_escape,
    terminal_parser_escape_modified,
    terminal_parser_escape_expect_semi,
    terminal_parser_escape_expect_modifier,
} terminal_parser_state_t;

//---------------------------------------------------------------------------
typedef enum {
    terminal_key_modifier_none = 0,
    terminal_key_modifier_shift = 2,
    terminal_key_modifier_alt = 3,
    terminal_key_modifier_shift_alt = 4,
    terminal_key_modifier_ctrl = 5,
    terminal_key_modifier_shift_ctrl = 6
} terminal_key_modifier_t;

//---------------------------------------------------------------------------
typedef struct {
    union __attribute__((packed)) {
        terminal_key_t eKey;
        char cChar;
        uint8_t u8Raw;
    };
    terminal_key_modifier_t eModifier;
    bool bEscaped;
} KeyVal_t;

//---------------------------------------------------------------------------
class TerminalIn {
public:

    bool ReadLoop();
    void GetLastKey(KeyVal_t* pstKeyVal_);
    void Init() { m_eState = terminal_parser_begin; }

    virtual bool ReadByte(uint8_t* pu8Byte_) = 0;
private:

    bool ParseByte(uint8_t u8Input_);
    bool RunHandler(uint8_t u8Input_);

    void AcceptCharacter(char cChar_);
    void AcceptSpecial(terminal_key_t eKey_);

    bool BeginHandler(char cInput_);
    bool EscapeCheckHandler(char cInput_);
    bool EscapeTypeHandler(char cInput_);
    bool EscapeHandler(char cInput_);
    bool EscapeModifierHandler(char cInput_);
    bool ExpectSemiHandler(char cInput_);

    terminal_parser_state_t m_eState;

    bool    m_bEscaped;
    char    m_cEscapeBegin;
    uint8_t m_au8EscapeSeq[3];
    uint8_t m_u8EscapeIdx;

    uint8_t m_u8Modifier;
    uint8_t m_u8Command;

    KeyVal_t m_uKey;
};

#endif // __TERMINALINPUT_H__
