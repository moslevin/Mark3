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

#ifndef __COMMAND_DISPATCH_H__
#define __COMMAND_DISPATCH_H__

#include "ll.h"
#include "command_handler.h"

//---------------------------------------------------------------------------
class CommandDispatcher
{
public:
    CommandDispatcher() { m_pclDefault = 0; }

    void AddCommand(CommandHandler* pclHandler_);

    bool Execute(const char* szCommandString_);

    void SetDefaultHandler(CommandHandler* pclHandler_) { m_pclDefault = pclHandler_; }

private:
    int CommandLength(const char* szCommandString_);

    DoubleLinkList  m_clCommandList;
    CommandHandler*  m_pclDefault;
};

#endif //__CMMAND_HANDLER_H__
