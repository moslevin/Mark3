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
    \file control_label.h
    \brief GUI Label Control

    A label control is a static text eliment, specified by a font, a color,
    and a string to overlay at a given location.
*/

#ifndef __CONTROL_LABEL_H__
#define __CONTROL_LABEL_H__

#include "gui.h"
#include "kerneltypes.h"
#include "draw.h"
#include "font.h"

class LabelControl : public GuiControl
{
public:
    virtual void Init();
    virtual void Draw();
    virtual GuiReturn_t ProcessEvent( GuiEvent_t *pstEvent_ ) { return GUI_EVENT_OK; }
    virtual void Activate( bool bActivate_ ) {}

    void SetBackColor( COLOR eColor_ )             { m_uBackColor = eColor_; }
    void SetFontColor( COLOR eColor_ )             { m_uFontColor = eColor_; }
    void SetFont( Font_t *pstFont_ )             { m_pstFont = pstFont_; }
    void SetCaption( const char *pcData_ )     { m_pcCaption = pcData_; }
    void SetTransparent( bool bTransparent_ )    { m_bTransparent = bTransparent_; }
private:
    Font_t *m_pstFont;
    const char *m_pcCaption;
    COLOR m_uBackColor;
    COLOR m_uFontColor;
    bool m_bTransparent;
};

#endif

