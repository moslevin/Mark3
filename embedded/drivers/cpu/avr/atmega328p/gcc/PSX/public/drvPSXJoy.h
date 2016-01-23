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
/*!
    \file drvPSXJoy.h
    
    \brief Playstation (PSX/PS2) joystick driver for AVR
    
    This implementation uses the Driver->Graphics Driver class heirarchy in
    order to provide consisency with other drivers in the system, and 
    allow for other joystick drivers to be swapped out without impacting the
    user code.
 */

#ifndef __DRV_PSXJOY_IN_H_
#define __DRV_PSXJOY_IN_H_

#include "kerneltypes.h"
#include "driver.h"
#include "joystick.h"

//---------------------------------------------------------------------------
// Module configuration
//---------------------------------------------------------------------------
#define PSX_ACK_BIT        (0x20)        // Bit 5
#define PSX_CLK_BIT        (0x10)        // Bit 4
#define PSX_ATT_BIT        (0x08)        // Bit 3
#define PSX_CMD_BIT        (0x04)        // Bit 2
#define PSX_DATA_BIT       (0x02)        // Bit 1

#define PSX_PORT           PORTC
#define PSX_PIN            PINC
#define PSX_DDR            DDRC

//---------------------------------------------------------------------------
#define MAX_PSX_DATA_BYTES        (6)
#define POLL_SPIN_COUNT           (100)

//---------------------------------------------------------------------------
// Commands sent from PSX to Joypad
#define PSX_CMD_START            (0x01)
#define PSX_CMD_POLL             (0x42)
#define PSX_CMD_IDLE             (0xFF)

// Possible TYPE command responses
#define PSX_TYPE_DIGITAL         (0x41)
#define PSX_TYPE_ANALOG_RED      (0x73)
#define PSX_TYPE_ANALOG_GREEN    (0x53)
#define PSX_TYPE_NEGCON          (0x23)

// Joypad report sizes
#define PSX_SIZE_DIGITAL         (2)
#define PSX_SIZE_ANALOG          (6)

/*!
 *  Class implementing a playstation joystick driver, using the Mark3 Joystick
 *  Driver API.
 */
class PSXJoystick : public JoystickDriver
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
    virtual uint16_t Write( uint16_t u16Bytes_, uint8_t pu8Data_ ) { return u16Bytes_; }
    
private:
    /*!
     *  \brief JoyDelay
     *
     *  Simple delay implementation.  Not instrumented.  This is a
     *  busy-waiting delay.
     */
    void JoyDelay(uint16_t u16Time_);
    
    /*!
     *  \brief ScanInternal
     *
     *  Function that is invoked to scan the joystick from Driver::Control().
     *  This starts the process of querying the joystick interface on the 
     *  hardware ports, and updates the current report structure if successful.
     */
    void ScanInternal();
    
    /*!
     *  \brief CmdByte
     *
     *  Function that scans a single byte from the joystick interface.
     *  
     *  \param u8Cmd_ Command to issue on the interface
     *  \param bWaitAck_ Wait for acknowledgment on the interface if
     *         true, false to not wait.
     */
    uint8_t CmdByte(uint8_t u8Cmd_, bool bWaitAck_);
    
    /*!
     *  \brief Decode
     *
     *  Decode the bytestream read from the joystick interafce, and 
     *  convert it into the regular joystick report format.
     *  
     *  \param u8JoyMode_ Joystick mode byte, read from the HW interface
     *  \param pu8Data_ Pointer to the bytestream containing the joystick
     *                  state information, relative to the joystick mode.
     */
    void Decode(uint8_t u8JoyMode_, uint8_t *pu8Data_);
    
    uint8_t m_u8Type;        //!< Joystick type identifier
    bool m_bConnected;    //!< Whether or not the joystick is connected
    
    uint8_t m_aucRaw[6];    //!< Raw Joystick Data
};

#endif 

