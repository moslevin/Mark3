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
===========================================================================*/
/*!

    \file   shell_support.cpp

    \brief  Support functions & data structures useful in implementing a
            shell.

*/

#include "kerneltypes.h"
#include "memutil.h"
#include "shell_support.h"

//---------------------------------------------------------------------------
char ShellSupport::RunCommand(CommandLine_t* pstCommand_, const ShellCommand_t* pastShellCommands_)
{
    uint8_t i = 0;
    uint8_t u8TmpLen;
    while (pastShellCommands_[i].szCommand) {
        u8TmpLen = MIN(pstCommand_->pstCommand->u8Len, MemUtil::StringLength(pastShellCommands_[i].szCommand));

        if (true == MemUtil::CompareMemory((const void*)pastShellCommands_[i].szCommand,
                                           (const void*)(pstCommand_->pstCommand->pcToken),
                                           u8TmpLen)) {
            pastShellCommands_[i].pfHandler(pstCommand_);
            return 1;
        }
        i++;
    }
    return 0;
}

//---------------------------------------------------------------------------
void ShellSupport::UnescapeToken(Token_t* pstToken_, char* szDest_)
{
    const char* szSrc = pstToken_->pcToken;
    uint8_t     i;
    uint8_t     j = 0;
    for (i = 0; i < pstToken_->u8Len; i++) {
        //-- Escape characters
        if ('\\' == szSrc[i]) {
            i++;
            if (i >= pstToken_->u8Len) {
                break;
            }
            switch (szSrc[i]) {
                case 't': szDest_[j++]  = '\t'; break;
                case 'r': szDest_[j++]  = '\r'; break;
                case 'n': szDest_[j++]  = '\n'; break;
                case ' ': szDest_[j++]  = ' '; break;
                case '\\': szDest_[j++] = '\\'; break;
                case '\"': szDest_[j++] = '\"'; break;
                default: break;
            }
        }
        //-- Unescaped quotes
        else if ('\"' == szSrc[i]) {
            continue;
        }
        //-- Everything else
        else {
            szDest_[j++] = szSrc[i];
        }
    }
    //-- Null-terminate the string
    szDest_[j] = '\0';
}

//---------------------------------------------------------------------------
Option_t* ShellSupport::CheckForOption(CommandLine_t* pstCommand_, const char* szOption_)
{
    uint8_t i;
    uint8_t u8TmpLen;
    for (i = 0; i < pstCommand_->u8NumOptions; i++) {
        u8TmpLen = MIN(MemUtil::StringLength(szOption_), pstCommand_->astOptions[i].pstStart->u8Len);

        if (true == MemUtil::CompareMemory(
                        (const void*)szOption_, (const void*)(pstCommand_->astOptions[i].pstStart->pcToken), u8TmpLen)) {
            return &(pstCommand_->astOptions[i]);
        }
    }
    return 0;
}

//---------------------------------------------------------------------------
char ShellSupport::TokensToCommandLine(Token_t* pastTokens_, uint8_t u8Tokens_, CommandLine_t* pstCommand_)
{
    pstCommand_->u8NumOptions = 0;
    uint8_t u8Token             = 0;
    uint8_t u8Count             = 0;
    uint8_t u8Option            = 0;

    if (!u8Tokens_) {
        return -1;
    }

    // Command is a single token...
    pstCommand_->pstCommand = &pastTokens_[0];

    // Parse out options
    u8Token = 1;
    while (u8Token < u8Tokens_ && u8Option < 12) {
        pstCommand_->astOptions[u8Option].pstStart = &pastTokens_[u8Token];
        u8Count                                    = 1;
        u8Token++;
        while (u8Token < u8Tokens_ && pastTokens_[u8Token].pcToken[0] != '-') {
            u8Token++;
            u8Count++;
        }
        pstCommand_->astOptions[u8Option].u8Count = u8Count;
        u8Option++;
    }

    pstCommand_->u8NumOptions  = u8Option;
    pstCommand_->u8TokenCount  = u8Tokens_;
    pstCommand_->pastTokenList = pastTokens_;
    return u8Option;
}
