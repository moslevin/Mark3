/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012-2015 Funkenstein Software Consulting, all rights reserved.
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
char ShellSupport::RunCommand( CommandLine_t *pstCommand_, const ShellCommand_t *pastShellCommands_ )
{
    uint8_t i = 0;
    uint8_t tmp_len;
    while (pastShellCommands_[i].szCommand)
    {
        tmp_len = MIN(pstCommand_->pstCommand->u8Len, MemUtil::StringLength(pastShellCommands_[i].szCommand));

        if (true == MemUtil::CompareMemory( (const void*)pastShellCommands_[i].szCommand,
                                            (const void*)(pstCommand_->pstCommand->pcToken),
                                            tmp_len ) )
        {
            pastShellCommands_[i].pfHandler( pstCommand_ );
            return 1;
        }
        i++;
    }
    return 0;
}

//---------------------------------------------------------------------------
void ShellSupport::UnescapeToken( Token_t *pstToken_, char *szDest_ )
{
    const char *szSrc = pstToken_->pcToken;
    int i;
    int j = 0;
    for (i = 0; i < pstToken_->u8Len; i++)
    {
        //-- Escape characters
        if ('\\' == szSrc[i])
        {
            i++;
            if (i >= pstToken_->u8Len)
            {
                break;
            }
            switch (szSrc[i])
            {
            case 't':
                szDest_[j++] = '\t';
                break;
            case 'r':
                szDest_[j++] = '\r';
                break;
            case 'n':
                szDest_[j++] = '\n';
                break;
            case ' ':
                szDest_[j++] = ' ';
                break;
            case '\\':
                szDest_[j++] = '\\';
                break;
            case '\"':
                szDest_[j++] = '\"';
                break;
            default:
                break;
            }
        }
        //-- Unescaped quotes
        else if ('\"' == szSrc[i])
        {
            continue;
        }
        //-- Everything else
        else
        {
            szDest_[j++] = szSrc[i];
        }
    }
    //-- Null-terminate the string
    szDest_[j] = '\0';
}

//---------------------------------------------------------------------------
Option_t *ShellSupport::CheckForOption( CommandLine_t *pstCommand_, const char *szOption_ )
{
    char i;
    uint8_t tmp_len;
    for (i = 0; i < pstCommand_->u8NumOptions; i++)
    {
        tmp_len = MIN(MemUtil::StringLength(szOption_), pstCommand_->astOptions[i].pstStart->u8Len);

        if (true == MemUtil::CompareMemory( (const void*)szOption_,
                                   (const void*)(pstCommand_->astOptions[i].pstStart->pcToken),
                                    tmp_len ) )
        {
            return &(pstCommand_->astOptions[i]);
        }
    }
    return 0;
}

//---------------------------------------------------------------------------
char ShellSupport::TokensToCommandLine(Token_t *pastTokens_, uint8_t u8Tokens_, CommandLine_t *pstCommand_)
{
    char count = 0;
    char token = 0;
    char option = 0;
    pstCommand_->u8NumOptions = 0;

    if (!u8Tokens_)
    {
        return -1;
    }

    // Command is a single token...
    pstCommand_->pstCommand = &pastTokens_[0];

    // Parse out options
    token = 1;
    while (token < u8Tokens_ && option < 12)
    {
        pstCommand_->astOptions[option].pstStart = &pastTokens_[token];
        count = 1;
        token++;
        while (token < u8Tokens_ && pastTokens_[token].pcToken[0] != '-')
        {
            token++;
            count++;
        }
        pstCommand_->astOptions[option].u8Count = count;
        option++;
    }

    pstCommand_->u8NumOptions = option;
    pstCommand_->u8TokenCount = u8Tokens_;
    pstCommand_->pastTokenList = pastTokens_;
    return option;
}
