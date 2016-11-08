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

#ifndef __M3SHELL_H__
#define __M3SHELL_H__

#include "mark3.h"

#include "command_dispatch.h"
#include "command_handler.h"
#include "terminal_io.h"
#include "linebuffer.h"

//---------------------------------------------------------------------------
class M3Shell {
public:
    void AddCommand(CommandHandler* pclHandler_);

    void SetDefaultHandler(CommandHandler* pclHandler_);

    void SetPrompt(const char* szPrompt_);

    void Init();

    void InputLoop();

    void SetDriver(Driver* pclDriver_);
private:

    CommandDispatcher m_clDispatcher;

    const char*       m_szPrompt;
    TerminalIO        m_clTerminal;
    LineBuffer        m_clLineBuffer;
};

#endif // __M3SHELL_H__
