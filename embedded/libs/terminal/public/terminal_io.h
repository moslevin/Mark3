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

#ifndef __TERMINAL_IO_H__
#define __TERMINAL_IO_H__

#include "mark3.h"

#include "terminal_in.h"
#include "terminal_out.h"

//---------------------------------------------------------------------------
class TerminalIO : public TerminalIn, public TerminalOut
{
public:
    TerminalIO();

    void SetDriver(Driver* pclDriver_) { m_pclDriver = pclDriver_; }

    bool WriteByte(uint8_t u8Byte_);

    bool ReadByte(uint8_t *u8Byte_);
private:

    Driver* m_pclDriver;
};

#endif // __TERMINAL_IO_H__
