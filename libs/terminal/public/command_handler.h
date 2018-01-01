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
    \file command_handler.h

    \brief Text-based command execution
 */

#pragma once

#include "ll.h"

//---------------------------------------------------------------------------
typedef void(*command_action)(const char* szArgs_);

namespace Mark3
{

//---------------------------------------------------------------------------
/*!
 * \brief The CommandHandler class
 *
 * Class used to represent an individual command that can be run from a
 * command-line interface.  Essentially, this is used to map a text command
 * string to a handler function that it represents.
 *
 */
class CommandHandler : public LinkListNode
{
public:
    /*!
     * \brief CommandHandler
     *
     * Constructor which also allows the user to set the name and
     * handler function of the object.
     *
     * \param szName_     Name of the command
     * \param pfHandler_  Function representing this command
     */
    CommandHandler(const char* szName_, command_action pfHandler_);

    CommandHandler();

    /*!
     * \brief Set
     *
     * Set the name and handler function associated with the command
     *
     * \param szName_     Name of the command
     * \param pfHandler_  Function representing this command
     */
    void Set(const char* szName_, command_action pfHandler_);

    /*!
     * \brief Name
     *
     * Return the command's name.
     *
     * \return Pointer to the command's name
     */
    const char* Name();

    /*!
     * \brief Execute
     *
     * Execute the command by calling it's handler function.
     *
     * \param szArgs_ Argument string passed to the command via higher-level
     *        command interface.
     */
    void Execute(const char* szArgs_);

private:
    const char*         m_szCommandName;
    command_action      m_pfHandler;
};
} //namespace Mark3
