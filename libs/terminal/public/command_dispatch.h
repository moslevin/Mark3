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
    \file command_dispatch.h

    \brief Class to compare text against a list of commands, executing the
           command that matches the one from the list, or a sane default on
           incorrect input.
 */

#pragma once

#include "ll.h"
#include "command_handler.h"

namespace Mark3
{

//---------------------------------------------------------------------------
/*!
 * \brief The CommandDispatcher class
 *
 * Implements a list of command-handlers that can be dynamically initialized
 * by the user.
 *
 */
class CommandDispatcher
{
public:
    CommandDispatcher() { m_pclDefault = 0; }

    /*!
     * \brief AddCommand
     *
     * Add a new command to the dispatcher.  The object must persist for the
     * lifespan of the Dispatch object.
     *
     * \param pclHandler_ Pointer to the command handler to add to this dispatcher
     */
    void AddCommand(CommandHandler* pclHandler_);

    /*!
     * \brief Execute
     *
     * Commands from the list are executed by comparing the list of known
     * command strings against user input.  The first space-delimited token is used
     * to determine which command to run (on a match), with the remaining string
     * data passed to the command's Execute() method as argument data.
     *
     * If no valid command is found for a corresponding command string, then
     * the default command handler is executed, which may provide the user
     * with further assistance, or simply present an error message.
     *
     * \param szCommandString_  Command to execute, provided by the user.
     * \return true on success, false on failure to match the input against
     *         any commands linked to this dispatcher
     */
    bool Execute(const char* szCommandString_);

    /*!
     * \brief SetDefaultHandler
     *
     * Set the command handler to execute when an invalid or unknown command is passed
     * to the dispatcher.  Command must also have been previously added via the
     * AddCommand method.
     *
     * \param pclHandler_ Pointer ot the command handler to set as the default.
     */
    void SetDefaultHandler(CommandHandler* pclHandler_) { m_pclDefault = pclHandler_; }

private:
    /*!
     * \brief CommandLength
     *
     * Return the length of the first token within the command string
     *
     * \param szCommandString_ User-provided command string
     * \return length of the command in bytes
     */
    int CommandLength(const char* szCommandString_);

    DoubleLinkList  m_clCommandList;
    CommandHandler*  m_pclDefault;
};
} //namespace Mark3
