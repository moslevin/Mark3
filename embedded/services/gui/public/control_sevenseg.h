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
    \file control_progress.h
    \brief GUI Progress Bar Control

    A simple progress bar control using lines and rectangles to display
    the status of an operation from initialization to completion
*/

#ifndef __CONTROL_SEVENSEG_H__
#define __CONTROL_SEVENSEG_H__

#include "gui.h"
#include "kerneltypes.h"
#include "draw.h"
#include "font.h"

class SevenSegControl : public GuiControl
{
public:
    virtual void Init();

    virtual void Draw();
    virtual GuiReturn_t ProcessEvent( GuiEvent_t *pstEvent_ );
    virtual void Activate( K_BOOL bActivate_ ) {}

    void SetValue( K_USHORT usValue_ )  {m_usValue = usValue_; SetStale(); }
    void SetColon( bool kSet_ )         {m_bColon = kSet_; SetStale();  }
    K_USHORT GetValue(void) { return m_usValue; }
private:

    K_USHORT m_usValue;
    bool     m_bColon;
};

#endif
