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
    \file slip_hid.h
    
    \brief HID device abstraction over FunkenSlip channel    
*/

#ifndef __SLIP_HID_H__
#define __SLIP_HID_H__

#include "kerneltypes.h"
#include "driver.h"

//---------------------------------------------------------------------------
typedef enum
{
    CMD_SET_SURFACE,    // Set the event surface to attach to (output)
    CMD_SET_SLIPMUX,    // Set the slip mux object to attach to (input)
} SlipDriverCommands_t;

//---------------------------------------------------------------------------
class SlipHid : public Driver 
{
public:
    virtual void Init() {}
    
    virtual uint8_t Open() 
        { return 0; }
    
    virtual uint8_t Close() 
        { return 0; }
    
    virtual uint16_t Read( uint16_t u16Bytes_, uint8_t *pu8Data_) 
        { return u16Bytes_; }
    
    virtual uint16_t Write( uint16_t u16Bytes_, uint8_t *pu8Data_) 
        { return u16Bytes_; }
    
    virtual uint16_t Control( uint16_t u16Event_, void *pvDataIn_,
                     uint16_t u16SizeIn_, void *pvDataOut_, uint16_t u16SizeOut_ );
private:
};


#endif
