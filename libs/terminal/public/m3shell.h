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
    \file m3shell.h

    \brief Implements an interactive command shell using an arbitrary Driver.
 */


#pragma once

#include "mark3.h"

#include "command_dispatch.h"
#include "command_handler.h"
#include "terminal_io.h"
#include "linebuffer.h"

namespace Mark3
{

//---------------------------------------------------------------------------
/*!
 * \brief The M3Shell class
 */
class M3Shell {
public:
    /*!
     * \brief AddCommand
     *
     * Add a new command that can be executed within this shell.  The object
     * must persist for the lifespan of the shell object.
     *
     * \param pclHandler_ Pointer to the command handler to add to this shell
     */
    void AddCommand(CommandHandler* pclHandler_);

    /*!
     * \brief SetDefaultHandler
     *
     * Set the command handler to execute when an invalid or unknown command is passed
     * to the dispatcher.  Command must also have been previously added via the
     * AddCommand method.
     *
     * \param pclHandler_ Pointer ot the command handler to set as the default.
     */
    void SetDefaultHandler(CommandHandler* pclHandler_);

    /*!
     * \brief SetPrompt
     *
     * Set the string to be used as the terminal prompt at the beginning of each line
     * of input.
     *
     * \param szPrompt_ prompt string
     */
    void SetPrompt(const char* szPrompt_);

    /*!
     * \brief Init
     *
     * Initialize the shell.  Must be called prior to use.
     *
     */
    void Init();

    /*!
     * \brief InputLoop
     *
     * Handle one line of command-input at a time.
     *
     */
    void InputLoop();

    /*!
     * \brief SetDriver
     *
     * Set the kernel driver on which the shell gets its command input, and to which the
     * shell writes its text output.  Typically a UART or terminal-like interface.
     *
     * \param pclDriver_ Pointer to the driver to use with this shell.
     */
    void SetDriver(Driver* pclDriver_);
private:

    CommandDispatcher m_clDispatcher;

    const char*       m_szPrompt;
    TerminalIO        m_clTerminal;
    LineBuffer        m_clLineBuffer;
};
} //namespace Mark3
