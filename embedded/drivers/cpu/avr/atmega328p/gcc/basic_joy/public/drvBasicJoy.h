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
    \file drvBasicJoy.h
    
    \brief Generic digital joystick driver for AVR - one input per key.
    
    This implementation uses the Driver->Graphics Driver class heirarchy in
    order to provide consisency with other drivers in the system, and 
    allow for other joystick drivers to be swapped out without impacting the
    user code.
*/

#ifndef __DRV_BASICJOY_H_
#define __DRV_BASICJOY_H_

#include "kerneltypes.h"
#include "driver.h"
#include "joystick.h"

//---------------------------------------------------------------------------
// Module configuration
//---------------------------------------------------------------------------

// Port used for inputs
#define JOY_PORT           PORTC
#define JOY_PIN            PINC
#define JOY_DDR            DDRC

// IOs used for inputs, from the port above
#define JOY_LEFT			(0x01)
#define JOY_DOWN			(0x02)
#define JOY_RIGHT			(0x04)
#define JOY_UP				(0x10)
#define JOY_FIRE			(0x08)

/*!
    Class implementing a playstation joystick driver, using the Mark3 Joystick
    Driver API.
*/
class BasicJoystick : public JoystickDriver
{
public:
    virtual void Init();
    virtual K_UCHAR Open();
    virtual K_USHORT Control( K_USHORT usEvent_,
                                void *pvDataIn_, 
                                K_USHORT usSizeIn_, 
                                void *pvDataOut_, 
                                K_USHORT usSizeOut_ );
    virtual K_UCHAR Close() { return 0; }
    virtual K_USHORT Read( K_USHORT usBytes_, K_UCHAR *pucData_ ) { return usBytes_; }
    virtual K_USHORT Write( K_USHORT usBytes_, K_UCHAR *pucData_ ) { return usBytes_; }
    
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

