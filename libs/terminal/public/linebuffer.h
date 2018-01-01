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
    \file linebuffer.h

    \brief Command-line input buffer class
 */

#pragma once

#include <stdint.h>

#define LINEBUFFER_LENGTH           (80)    // Bytes of text to store in the object

namespace Mark3
{

//---------------------------------------------------------------------------
/*!
 * \brief The LineBuffer class
 *
 * String buffer used for implementing a line of text input, as entered from a
 * terminal.
 *
 */
class LineBuffer {
public:
    /*!
     * \brief Clear
     *
     * Reset the contents of the buffer
     *
     */
    void Clear();

    /*!
     * \brief WriteCharacter
     *
     * Append a character of data to the line buffer at its present cursor location
     *
     * \param cByte_ character to add to the buffer
     * \return true - character successfully added, false - buffer full
     */
    bool WriteCharacter(char cByte_);

    /*!
     * \brief Backspace
     *
     * Remove the character preceding the line buffer's present cursor location
     *
     * \return true - character removed, false if at beginning of line or buffer empty
     */
    bool Backspace();

    /*!
     * \brief Delete
     *
     * Remove the character after the line buffer's present cursor location
     *
     * \return number of characters that need to be redrawn from the cursor's location
     */
    uint8_t Delete();

    /*!
     * \brief CursorLeft
     *
     * Move the cursor to the left by one character
     *
     * \return true - cursor moved, false if at beginning of line
     */
    bool CursorLeft();

    /*!
     * \brief CursorRight
     *
     * Move the cursor to the right by one character
     *
     * \return true - cursor moved, false if at beginning of line
     */
    bool CursorRight();

    /*!
     * \brief CursorHome
     *
     * Move the cursor to the beginning of the line.
     *
     * \return Number of characters that the cursor moved to the left
     */
    uint8_t CursorHome();

    /*!
     * \brief CursorEnd
     *
     * Move the cursor to the end of the line.
     *
     * \return Number of characters that the cursor moved to the right
     */
    uint8_t CursorEnd();

    /*!
     * \brief GetCursorIndex
     *
     * Return the current cursor index.
     *
     * \return current cursor index.
     */
    uint8_t GetCursorIndex() { return m_u8CursorIndex; }

    /*!
     * \brief GetLastIndex
     *
     * Return the index of the last character on the line.
     *
     * \return index of the last character on the line
     */
    uint8_t GetLastIndex() { return m_u8LastByteIndex; }

    /*!
     * \brief GetBuffer
     *
     * Return a pointer to the object's raw text buffer.  Useful for
     * executing a command on an end-of-line condition.
     *
     * \return pointer to a null-terminated string.
     */
    const char* GetBuffer() { return m_cLineBuffer; }

private:
    char m_cLineBuffer[LINEBUFFER_LENGTH];
    uint8_t m_u8LastByteIndex;
    uint8_t m_u8CursorIndex;
};
} //namespace Mark3
