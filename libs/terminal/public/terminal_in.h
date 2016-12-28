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
/*!
    \file terminal_in.h

    \brief Class for parsing terminal input (i.e. keyboard input).  This includes
           special keys and modifiers (shift, ctrl, alt).
 */

#ifndef __TERMINAL_IN_H__
#define __TERMINAL_IN_H__

#include <stdint.h>

//---------------------------------------------------------------------------
// Special keys supported by the TerminalIn class
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
} TerminalKey_t;

//---------------------------------------------------------------------------
// Special modifiers, or modifier combinations, that can be applied to keys
typedef enum {
    TERMINAL_KEY_MODIFIER_NONE = 0,
    TERMINAL_KEY_MODIFIER_SHIFT = 2,
    TERMINAL_KEY_MODIFIER_ALT = 3,
    TERMINAL_KEY_MODIFIER_SHIFT_ALT = 4,
    TERMINAL_KEY_MODIFIER_CTRL = 5,
    TERMINAL_KEY_MODIFIER_SHIFT_CTRL = 6
} TerminalKeyModifier_t;

//---------------------------------------------------------------------------
// States in the TerminalIn state machine
typedef enum {
    TERMINAL_PARSER_BEGIN = 0,
    TERMINAL_PARSER_ESCAPE_CHECK,
    TERMINAL_PARSER_ESCAPE_TYPE,
    TERMINAL_PARSER_ESCAPE,
    TERMINAL_PARSER_ESCAPE_MODIFIED,
    TERMINAL_PARSER_ESCAPE_EXPECT_SEMI,
    TERMINAL_PARSER_ESCAPE_EXPECT_MODIFIER,
} TerminalParserState_t;

//---------------------------------------------------------------------------
// Struct representing a key value
typedef struct {
    union __attribute__((packed)) {
        TerminalKey_t eKey;
        char cChar;
        uint8_t u8Raw;
    };
    TerminalKeyModifier_t eModifier;
    bool bEscaped;
} KeyVal_t;

//---------------------------------------------------------------------------
/*!
 * \brief The TerminalIn class
 *
 * Class to interpret ANSI terminal data.
 *
 */
class TerminalIn {
public:

    /*!
     * \brief ReadLoop
     *
     * Read and interpret a single byte of data of terminal input.
     *
     * \return true if a byte of data was successfully read and parsed
     */
    bool ReadLoop();

    /*!
     * \brief GetLastKey
     *
     * Copy the contents of the last matched key value into the provided
     * input struct.
     *
     * \param pstKeyVal_ Struct to contain the last matched key value.
     *
     */
    void GetLastKey(KeyVal_t* pstKeyVal_);

    /*!
     * \brief Init
     *
     * Initialize the object's state machine prior to use.
     */
    void Init() { m_eState = TERMINAL_PARSER_BEGIN; }

    /*!
     * \brief ReadByte
     *
     * Attempt to read a byte of input from the terminal interface.
     *
     * \param pu8Byte_ pointer to the byte of read data.
     * \return true - byte successfully read, false otherwise.
     */
    virtual bool ReadByte(uint8_t* pu8Byte_) = 0;
private:

    /*!
     * \brief ParseByte
     *
     * Pass a byte of data through the terminal input state machine.
     *
     * \param u8Input_ Byte to process
     * \return true - key value was matched with this input byte.  false - further
     *         data bytes are required to complete the current byte sequence.
     */
    bool ParseByte(uint8_t u8Input_);

    /*!
     * \brief RunHandler
     *
     * Pass the byte of data through the current state.
     *
     * \param u8Input_ Byte to process
     * \return true - key value was matched with this input byte.  false - further
     *         data bytes are required to complete the current byte sequence.
     */
    bool RunHandler(uint8_t u8Input_);

    /*!
     * \brief AcceptCharacter
     *
     * Accept input as a printable ASCII character.
     *
     * \param cChar_ character to accept.
     */
    void AcceptCharacter(char cChar_);

    /*!
     * \brief AcceptSpecial
     *
     * Accept input as a non-printable character.  This includes things like
     * tab and linefeed characters, as well as special keys on a typical keyboard
     * (i.e. function keys, home/end/insert/delete).
     *
     * \param eKey_ Struct containing the key value
     */
    void AcceptSpecial(TerminalKey_t eKey_);

    /*!
     * Handler functions for all states implemented within the object's
     * state machine.
     */
    bool BeginHandler(char cInput_);
    bool EscapeCheckHandler(char cInput_);
    bool EscapeTypeHandler(char cInput_);
    bool EscapeHandler(char cInput_);
    bool EscapeModifierHandler(char cInput_);
    bool ExpectSemiHandler(char cInput_);

    TerminalParserState_t m_eState;

    bool    m_bEscaped;
    char    m_cEscapeBegin;
    uint8_t m_au8EscapeSeq[3];
    uint8_t m_u8EscapeIdx;

    uint8_t m_u8Modifier;
    uint8_t m_u8Command;

    KeyVal_t m_uKey;
};

#endif // __TERMINALINPUT_H__
