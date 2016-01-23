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
    \file control_brushpanel.h
    \brief GUI Panel Control With a Brushed-Metal Finish
*/

#ifndef __CONTROL_BRUSHPANEL_H__
#define __CONTROL_BRUSHPANEL_H__

#include "gui.h"
#include "kerneltypes.h"
#include "draw.h"

class BrushPanelControl : public GuiControl
{
public:
    virtual void Init() { SetAcceptFocus(false); }
    virtual void Draw();
    virtual GuiReturn_t ProcessEvent( GuiEvent_t *pstEvent_ ) { return GUI_EVENT_OK; }
    virtual void Activate( bool bActivate_ ) {}

private:

};

#endif
