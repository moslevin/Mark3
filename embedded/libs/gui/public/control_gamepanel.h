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
    \file control_gamepanel.h
    \brief GUI Game Panel Control

    A game panel is a blank UI element whose dimensions define the 
    dimensions of a gameplay surface.  The element triggers a draw() call
    on every tick event (which can be used to kick a game's state machine).
    The control also responds to joystick events, which can then be
    used to control the game.
*/

#ifndef __CONTROL_GAMEPANEL_H__
#define __CONTROL_GAMEPANEL_H__

#include "gui.h"
#include "kerneltypes.h"
#include "draw.h"

class GamePanelControl : public GuiControl
{
public:
    virtual void Init() { SetAcceptFocus(false); m_stJoy.Current.u16RawData = 0; m_stJoy.Previous.u16RawData = 0;}
    virtual void Draw();
    virtual GuiReturn_t ProcessEvent( GuiEvent_t *pstEvent_ );
    virtual void Activate( bool bActivate_ ) {}

private:
    JoystickEvent_t m_stJoy;
};

#endif

