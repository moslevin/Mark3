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

#include "command_dispatch.h"
#include "command_handler.h"
#include "memutil.h"
namespace Mark3 {
//---------------------------------------------------------------------------
void CommandDispatcher::AddCommand(CommandHandler* pclHandler_)
{
    m_clCommandList.Add(pclHandler_);
}

//---------------------------------------------------------------------------
bool CommandDispatcher::Execute(const char* szCommandString_)
{
    auto iCommandLength = CommandLength(szCommandString_);
    auto* szArgs = (szCommandString_ + iCommandLength);
    if (iCommandLength == 0) {
        return true;
    }

    auto* pclCurr = static_cast<CommandHandler*>(m_clCommandList.GetHead());
    while (pclCurr != 0) {
       auto* szCommandName = pclCurr->Name();

        if ( (MemUtil::CompareStrings(szCommandString_, szCommandName, iCommandLength)) &&
             (CommandLength(szCommandName) == CommandLength(szCommandString_)) ) {
            pclCurr->Execute(szArgs);
            return true;
        }
        pclCurr = static_cast<CommandHandler*>(pclCurr->GetNext());
    }
    if (m_pclDefault != 0) {
        m_pclDefault->Execute(szCommandString_);
    }
    return false;
}

//---------------------------------------------------------------------------
int CommandDispatcher::CommandLength(const char* szCommandString_)
{
    int iRc = 0;
    while ((szCommandString_[iRc] != '\0') &&
           (szCommandString_[iRc] != ' ')) {
        iRc++;
    }
    return iRc;
}
} //namespace Mark3
