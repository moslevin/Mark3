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
    \file terminal_out.h

    \brief Class for manipulating terminal output - text, cursor position,
           colors.
 */
#pragma once

#include <stdint.h>

namespace Mark3
{

//---------------------------------------------------------------------------
enum class TerminalColor : uint8_t {
    Black,
    Red,
    Green,
    Yellow,
    Blue,
    Magenta,
    Cyan,
    White,
    Default
};

//---------------------------------------------------------------------------
/*!
 * \brief The TerminalOut class
 *
 * Class for implementing terminal output/display functionality.  This consists
 * of cursor commands, drawing/erasing functions and text effects such as
 * colors and bold/italic, and underline.
 *
 */
class TerminalOut
{
public:
    // Cursor commands
    /*!
     * \brief CursorUp
     *
     * Move the cursor up a specified number of rows
     *
     * \param u8Rows_ number of rows to move the cursor
     */
    void CursorUp(uint8_t u8Rows_);

    /*!
     * \brief CursorDown
     *
     * Move the cursor down a specified number of rows
     *
     * \param u8Rows_ number of rows to move the cursor
     */
    void CursorDown(uint8_t u8Rows_);

    /*!
     * \brief CursorLeft
     *
     * Move the cursor left a specified number of columns
     *
     * \param u8Columns_ number of columns to move the cursor
     */
    void CursorLeft(uint8_t u8Columns_);

    /*!
     * \brief CursorRight
     *
     * Move the cursor right a specified number of columns
     *
     * \param u8Columns_ number of columns to move the cursor
     */
    void CursorRight(uint8_t u8Columns_);

    /*!
     * \brief SetCursorPos
     *
     * Set the absolute row and column position of the cursor, with the origin
     * (0,0) representing the upper-left corner of the terminal.
     *
     * \param u8X_  Column position
     * \param u8Y_  Row position
     */
    void SetCursorPos(uint8_t u8X_, uint8_t u8Y_);

    /*!
     * \brief CursorHome
     *
     * Move the cursor back to the upper-left, origin position at (0,0)
     *
     */
    void CursorHome(void);

    /*!
     * \brief SaveCursor
     *
     * Save the current cursor position, for future recall
     *
     */
    void SaveCursor(void);

    /*!
     * \brief RestoreCursor
     *
     * Restore the cursor position to the previously stored location
     *
     */
    void RestoreCursor(void);

    /*!
     * \brief Backspace
     *
     * Delete one character of text and move the cursor to the left one character
     *
     */
    void Backspace(void);

    /*!
     * \brief CursorVisible
     *
     * Make the cursor visible
     *
     */
    void CursorVisible(void);

    /*!
     * \brief CursorInvisible
     *
     * Make the cursor invisible
     *
     */
    void CursorInvisible(void);

    // Erasing text
    /*!
     * \brief ClearScreen
     *
     * Clear the entire screen
     *
     */
    void ClearScreen(void);

    /*!
     * \brief ClearLine
     *
     * Clear the entire line that the cursor currently occupies
     *
     */
    void ClearLine(void);

    /*!
     * \brief ClearToCursor
     *
     * Clear from the left of the cursor to the beginning of the line
     */
    void ClearToCursor(void);

    /*!
     * \brief ClearFromCursor
     *
     * Clear from the current cursor to the end of the line
     */
    void ClearFromCursor(void);

    // Text attributes
    /*!
     * \brief ResetAttributes
     *
     * Clear all special text attributes and continue writing normal text
     *
     */
    void ResetAttributes(void);

    /*!
     * \brief Bold
     *
     * Make text bright or bold.  May be represented with heavier fonts and/or
     * brighter colors.
     *
     */
    void Bold(void);

    /*!
     * \brief Dim
     *
     * Make text dimmer or darker.  May use lighter fonts and/or darker colors.
     *
     */
    void Dim(void);

    /*!
     * \brief Underscore
     *
     * Highlight text with an underscore
     *
     */
    void Underscore(void);

    /*!
     * \brief Blink
     *
     * Make text blink
     *
     */
    void Blink(void);

    /*!
     * \brief Reverse
     *
     * Invert the foreground + background text colors
     *
     */
    void Reverse(void);

    /*!
     * \brief Hidden
     *
     * Mark text as hidden
     *
     */
    void Hidden(void);

    // Foreground coloring
    /*!
     * \brief SetForeColor
     *
     * Set the current terminal foreground color.
     *
     * \param eColor_ Color code to set
     */
    void SetForeColor(TerminalColor eColor_);

    // Background coloring
    /*!
     * \brief SetBackColor
     *
     * Set the current terminal background color.
     *
     * \param eColor_ Color code to set
     */
    void SetBackColor(TerminalColor eColor_);

    /*!
     * \brief WriteByte
     *
     * Write an arbitrary byte of data to the terminal
     *
     * \param u8Val_ Data to write
     *
     * \return true on success, false otherwise
     */
    virtual bool WriteByte(uint8_t u8Val_) = 0;

    /*!
     * \brief WriteString
     *
     * Write a null-terminated string to the terminal
     *
     * \param szStr_ String to write to the terminal
     */
    void WriteString(const char* szStr_);

private:

    /*!
     * \brief Escape
     *
     * Write the escape character to the terminal
     */
    void Escape(void);

    /*!
     * \brief WriteVal
     *
     * Write a byte of arbitrary data to the terminal
     *
     * \param u8Val_ Data byte to write
     */
    void WriteVal(uint8_t u8Val_);
};
} //namespace Mark3
