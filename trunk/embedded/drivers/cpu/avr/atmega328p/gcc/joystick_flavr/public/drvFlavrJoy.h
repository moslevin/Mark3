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

