/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012 Funkenstein Software Consulting, all rights reserved.
See license.txt for more information
===========================================================================*/
/*!
    \file drvFlavrJoy.h
    
    \brief Co-simulated joystick input, using flAVR as a host.

*/

#ifndef __DRV_FLAVRJOY_H_
#define __DRV_FLAVRJOY_H_

#include "kerneltypes.h"
#include "driver.h"
#include "joystick.h"

//---------------------------------------------------------------------------
// Module configuration
//---------------------------------------------------------------------------

/*!
    Class implementing a playstation joystick driver, using the Mark3 Joystick
    Driver API.
*/
class FlavrJoystick : public JoystickDriver
{
public:
    virtual void Init();
    virtual uint8_t Open();
    virtual uint16_t Control( uint16_t u16Event_,
                                void *pvDataIn_, 
                                uint16_t u16SizeIn_, 
                                void *pvDataOut_, 
                                uint16_t u16SizeOut_ );
    virtual uint8_t Close() { return 0; }
    virtual uint16_t Read( uint16_t u16Bytes_, uint8_t *pu8Data_ ) { return u16Bytes_; }
    virtual uint16_t Write( uint16_t u16Bytes_, uint8_t *pu8Data_ ) { return u16Bytes_; }
    
private:
  
    /*!
        \fn void ScanInternal();
        
        Function that is invoked to scan the joystick from Driver::Control().
        This starts the process of querying the joystick interface on the 
        hardware ports, and updates the current report structure if successful.
    */
    void ScanInternal();
    

};

#endif 

