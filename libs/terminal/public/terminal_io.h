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
    \file terminal_io.h

    \brief Complete terminal IO class for implementing interactive console
           applications.
 */
#pragma once

#include "mark3.h"
#include "driver.h"

#include "terminal_in.h"
#include "terminal_out.h"
namespace Mark3
{

//---------------------------------------------------------------------------
/*!
 * \brief The TerminalIO class
 *
 * Class implementing terminal input and output functionality.
 */
class TerminalIO : public TerminalIn, public TerminalOut
{
public:
    TerminalIO();

    /*!
     * \brief SetDriver
     *
     * Set the Driver object representing the communication channel for the
     * terminal.  Typically a UART driver of some sort.
     *
     * \param pclDriver_ Pointer to the driver on which the terminal operates.
     */
    void SetDriver(Driver* pclDriver_) { m_pclDriver = pclDriver_; }

    /*!
     * \brief WriteByte
     *
     * Write a raw byte of data to the terminal interface
     *
     * \param u8Byte_ Byte of data to write
     * \return true - data was written, false - data could not be written.
     */
    bool WriteByte(uint8_t u8Byte_);

    /*!
     * \brief ReadByte
     *
     * Read a byte of data from the terminal interface
     *
     * \param u8Byte_ Byte of data read
     * \return true - data successfully read, false otherwise
     */
    bool ReadByte(uint8_t *u8Byte_);
private:

    Driver* m_pclDriver;
};
} //namespace Mark3
