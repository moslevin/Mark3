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

#ifndef __COMMAND_HANDLER_H__
#define __COMMAND_HANDLER_H__

#include "ll.h"

//---------------------------------------------------------------------------
typedef void(*command_action)(const char* szArgs_);

//---------------------------------------------------------------------------
class CommandHandler : public LinkListNode
{
public:
    CommandHandler(const char* szName_, command_action pfHandler_);

    CommandHandler();

    void Set(const char* szName_, command_action pfHandler_);

    const char* Name();

    void Execute(const char* szArgs_);

private:
    const char*         m_szCommandName;
    command_action      m_pfHandler;
};

#endif // __COMMAND_HANDLER_H__
