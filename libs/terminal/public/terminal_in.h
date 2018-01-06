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
/*!
    \file terminal_in.h

    \brief Class for parsing terminal input (i.e. keyboard input).  This includes
           special keys and modifiers (shift, ctrl, alt).
 */

#pragma once

#include <stdint.h>

namespace Mark3
{

//---------------------------------------------------------------------------
// Special keys supported by the TerminalIn class
enum class TerminalKey : uint8_t {
    Break = 0,
    Bell,
    Backspace,
    Tab,
    Vtab,
    Linefeed,
    Carriage_Return,
    Formfeed,
    F1,
    F2,
    F3,
    F4,
    Home_Row,
    End_Row,
    Up,
    Down,
    Right,
    Left,
    Center,
    Home,
    Insert,
    Delete,
    End,
    Page_Up,
    Page_Down,
    F5,
    F6,
    F7,
    F8,
    F9,
    F10,
    F11,
    F12
};

//---------------------------------------------------------------------------
// Special modifiers, or modifier combinations, that can be applied to keys
enum class TerminalKeyModifier : uint8_t {
    None = 0,
    Shift = 2,
    Alt = 3,
    Shift_Alt = 4,
    Ctrl = 5,
    Shift_Ctrl = 6
};

//---------------------------------------------------------------------------
// States in the TerminalIn state machine
enum class TerminalParser : uint8_t {
    Begin = 0,
    Check,
    Type,
    Escape,
    Escape_Modified,
    Escape_Expect_Semi,
    Escape_Expect_Modifier,
};

//---------------------------------------------------------------------------
// Struct representing a key value
typedef struct {
    union __attribute__((packed)) {
        TerminalKey eKey;
        char cChar;
        uint8_t u8Raw;
    };
    TerminalKeyModifier eModifier;
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
    void Init() { m_eState = TerminalParser::Begin; }

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
    void AcceptSpecial(TerminalKey eKey_);

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

    TerminalParser m_eState;

    bool    m_bEscaped;
    char    m_cEscapeBegin;
    uint8_t m_au8EscapeSeq[3];
    uint8_t m_u8EscapeIdx;

    uint8_t m_u8Modifier;
    uint8_t m_u8Command;

    KeyVal_t m_uKey;
};
} //namespace Mark3
