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
    \file slip_hid.cpp
    
    \brief HID device abstraction over FunkenSlip channel    
*/
#include "kerneltypes.h"
#include "driver.h"
#include "gui.h"
#include "message.h"
#include "system_heap.h"
#include "slip.h"
#include "slip_mux.h"
#include "slip_hid.h"
#include "driver.h"

//---------------------------------------------------------------------------
typedef enum
{
    SLIP_HID_MOUSE,
    SLIP_HID_KEYBOARD,
    SLIP_HID_JOYSTICK
} SlipHidEvents_t;

//---------------------------------------------------------------------------
static GuiEventSurface *g_pclSurface;

//---------------------------------------------------------------------------
void SlipHidHandler(Driver *pclDriver_, uint8_t u8Channel_, uint8_t *pu8Data_, uint16_t u16Len_ )
{    
    uint8_t u8EventType = pu8Data_[0];
    GuiEvent_t stGuiEvent;
    bool bSend = true;
    GuiEvent_t *pstEvent = (GuiEvent_t*)&pu8Data_[1];

    if (!pu8Data_ || !u16Len_)
    {
        return;
    }
    
    stGuiEvent.u8EventType = pstEvent->u8EventType;
    stGuiEvent.u8TargetID = pstEvent->u8TargetID;

    switch (u8EventType)
    {
        case SLIP_HID_MOUSE:
        {
            MouseEvent_t *pstMouse = (MouseEvent_t*)&pu8Data_[3];            
            
            stGuiEvent.stMouse.u8Flags = pstMouse->u8Flags;
            stGuiEvent.stMouse.u16X = pstMouse->u16X;
            stGuiEvent.stMouse.u16Y = pstMouse->u16Y;
        }            
            break;
        case SLIP_HID_JOYSTICK:
        {
            JoystickEvent_t *pstJoystick = (JoystickEvent_t*)&pu8Data_[3];            
            stGuiEvent.stJoystick.Current.u16RawData = pstJoystick->Current.u16RawData;
            stGuiEvent.stJoystick.Previous.u16RawData = pstJoystick->Previous.u16RawData;
        }        
            break;
        case SLIP_HID_KEYBOARD:
        {
            KeyEvent_t *pstKeyboard = (KeyEvent_t*)&pu8Data_[3];            
            stGuiEvent.stKey.u8Flags = pstKeyboard->u8Flags;
            stGuiEvent.stKey.u8KeyCode = pstKeyboard->u8KeyCode;
        }        
            break;
        default:
            bSend = false;
        break;
    }
    
    if (bSend)
    {
        g_pclSurface->SendEvent(&stGuiEvent);
    }    
}

//---------------------------------------------------------------------------
uint16_t SlipHid::Control( uint16_t u16Event_, void *pvDataIn_,
                            uint16_t u16SizeIn_, void *pvDataOut_, uint16_t u16SizeOut_ )
{
    switch (u16Event_)    
    {
        case CMD_SET_SLIPMUX:
        {
            SlipMux *pclSlipMux = (SlipMux *)pvDataIn_;
            pclSlipMux->InstallHandler(SLIP_CHANNEL_HID, SlipHidHandler);
        }
            break;
        case CMD_SET_SURFACE:
        {
            g_pclSurface = (GuiEventSurface *)pvDataIn_;
        }
        default:
            return 0;
    }
    return 0;
}    
