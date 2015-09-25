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
    \file drvPSXjoy.cpp
    
    \brief Playstation (PSX/PS2) Joystick driver implementaiton for AVR
*/

#include "kerneltypes.h"
#include "driver.h"
#include "joystick.h"
#include "drvPSXJoy.h"

#include <avr/io.h>

//---------------------------------------------------------------------------
void PSXJoystick::Init()
{
    m_u8Type = 0;
    m_bConnected = false;
}

//---------------------------------------------------------------------------
uint8_t PSXJoystick::Open()
{
    PSX_DDR  &= ~PSX_DATA_BIT;  // We receive data (act like PSX)    
    PSX_PORT |= PSX_DATA_BIT;   // Set internal pu32lup

    PSX_PORT |= PSX_CLK_BIT;    // Clock output (we control?)
    PSX_DDR  |= PSX_CLK_BIT;    // Clock output high
    
    PSX_DDR  |= PSX_CMD_BIT;    // We send commands (act like PSX)
    PSX_PORT |= PSX_CMD_BIT;    // Hold line high
        
    PSX_DDR  |= PSX_ATT_BIT;    // Attention bit (master ctrld)
    PSX_PORT |= PSX_ATT_BIT;    // Attention bit high until CS

    PSX_PORT |= PSX_ACK_BIT;    // Set internal pu32lup    
    PSX_DDR  &= ~PSX_ACK_BIT;   // Ack controlled by ctrlr    
    
    return 0;
}

//---------------------------------------------------------------------------
uint16_t PSXJoystick::Control( uint16_t u16Event_, void *pvIn_, uint16_t u16SizeIn_,
                                                  void *pvOut_, uint16_t u16SizeOut_ )
{    
    switch(u16Event_)
    {
        // Main event - poll joypad.
        case JOYSTICK_SCAN:
        {
            ScanInternal();
        }
            break;
        default:
            break;
    }
    return 0;
}

//---------------------------------------------------------------------------
void PSXJoystick::JoyDelay(uint16_t u16Time_)
{
    volatile uint16_t u16Time = 0;
    while (u16Time < (u16Time_ << 4))
    {
        u16Time++;
    }
}

//---------------------------------------------------------------------------
void PSXJoystick::ScanInternal()
{
    uint8_t u8BytesLeft;
    uint8_t u8JoyMode;
    uint8_t u8JoyIndex;
    
    uint8_t aucData[MAX_PSX_DATA_BYTES];
    
    // Signal attention - time to read out data
    PSX_PORT |= PSX_CMD_BIT;
    PSX_PORT |= PSX_CLK_BIT;
    PSX_PORT &= ~PSX_ATT_BIT;    
    
    JoyDelay(2);    
    
    // Send request for data
    CmdByte(PSX_CMD_START, true); 
        
    // Get the joypad mode
    u8JoyMode = CmdByte(PSX_CMD_POLL, false);
    m_aucRaw[0] = u8JoyMode;

    // Idle - Joypad ready to talk...
    m_aucRaw[1] = CmdByte(PSX_CMD_IDLE, false);

    // # bytes remaining based on Joypad mode:
    switch (u8JoyMode)
    {
        case PSX_TYPE_DIGITAL:    // Digital Mode
        {
            u8BytesLeft = PSX_SIZE_DIGITAL;
        }
            break;
        case PSX_TYPE_ANALOG_GREEN:    // Analog Green Mode
        {
            u8BytesLeft = PSX_SIZE_ANALOG;
        }
            break;
        case PSX_TYPE_ANALOG_RED:    // Analog Red Mode
        {
            u8BytesLeft = PSX_SIZE_ANALOG;
        }
            break;
        default:
        {
            // Disconnect.
            u8BytesLeft = 0;
            m_bConnected = false;
        }
    }
    
    // Array index for the raw PSX joypad input
    u8JoyIndex = 0;
    
    // Read the rest of the bytes.
    while (u8BytesLeft && m_bConnected)
    {
        if (u8BytesLeft != 1)
        {
            aucData[u8JoyIndex] = CmdByte(PSX_CMD_IDLE, false);
            m_aucRaw[3] = aucData[u8JoyIndex];
        }
        else
        {
            aucData[u8JoyIndex] = CmdByte(PSX_CMD_IDLE, true);
            m_aucRaw[4] = aucData[u8JoyIndex];
        }
        
        // Update indexes.
        u8BytesLeft--;
        u8JoyIndex++;
    }
    
    // Set attention high.
    PSX_PORT |= PSX_ATT_BIT;
    if (m_bConnected)
    {
        // Decode the joypad data if it's connected.
        Decode(u8JoyMode, aucData);
    }
}

//---------------------------------------------------------------------------
uint8_t PSXJoystick::CmdByte(uint8_t u8Cmd_, bool bWaitAck_)
{
    uint8_t u8ReturnVal = 0;
    uint8_t u8BitMask = 0x01;
    uint8_t u8Spin = 0;
    
    // Wait for ack to go high...
    while ((PSX_PORT & PSX_ACK_BIT) == 0)
    {
        u8Spin++;
        JoyDelay(1);
        if (u8Spin > POLL_SPIN_COUNT)
        {
            m_bConnected = false;
            return 0;
        }        
    }

    m_bConnected = true;
    JoyDelay(2);
    
    while (u8BitMask)
    {
        if ((u8Cmd_ & u8BitMask) != 0)
        {
            // Clock command out.
            PSX_PORT |= PSX_CMD_BIT;
        }
        else
        {
            PSX_PORT &= ~PSX_CMD_BIT;
        }

        // data clocked in on falling edge
        PSX_PORT &= ~PSX_CLK_BIT;    
        JoyDelay(5);

        // Read the input data pin.
        if ((PSX_PIN & PSX_DATA_BIT) != 0)
        {
            u8ReturnVal    |= u8BitMask;
        }
        
        // data high phase
        PSX_PORT |= PSX_CLK_BIT;    
        JoyDelay(5);
        
        // Next bit
        u8BitMask <<= 1;
    }

    // If this current byte requires acknowledgement
    if (bWaitAck_)
    {
        uint8_t u8Counter = 0;
        
        // Retry loop for waiting on ACK
        while(u8Counter < POLL_SPIN_COUNT)
        {
            // Poll for ACK...
            if ((PSX_PIN & PSX_ACK_BIT) == 0)
            {                
                break;
            }            
            u8Counter++;
            JoyDelay(1);
        }    
    }

    return u8ReturnVal;
}
//---------------------------------------------------------------------------
void PSXJoystick::Decode(uint8_t u8JoyMode_, uint8_t *pu8Data_)
{
    // Common to all supported modes...
    m_stCurrentReport.bSelect   = ((pu8Data_[0] & 0x01) == 0);
    m_stCurrentReport.bButton10 = ((pu8Data_[0] & 0x02) == 0);    //L3
    m_stCurrentReport.bButton9  = ((pu8Data_[0] & 0x04) == 0);    //R3
    m_stCurrentReport.bStart    = ((pu8Data_[0] & 0x08) == 0);
    m_stCurrentReport.bUp       = ((pu8Data_[0] & 0x10) == 0);
    m_stCurrentReport.bRight    = ((pu8Data_[0] & 0x20) == 0);
    m_stCurrentReport.bDown     = ((pu8Data_[0] & 0x40) == 0);
    m_stCurrentReport.bLeft     = ((pu8Data_[0] & 0x80) == 0);
    
    // Decode the message data into the appropriate joypad report format
    switch (u8JoyMode_)
    {
        case PSX_TYPE_DIGITAL:    // Digital Mode
        {            
            m_stCurrentReport.bButton6  = ((pu8Data_[1] & 0x01) == 0);  //L1
            m_stCurrentReport.bButton5  = ((pu8Data_[1] & 0x02) == 0);  //R1
            m_stCurrentReport.bButton8  = ((pu8Data_[1] & 0x04) == 0);  //L2
            m_stCurrentReport.bButton7  = ((pu8Data_[1] & 0x08) == 0);  //R2
            m_stCurrentReport.bButton1  = ((pu8Data_[1] & 0x10) == 0);  //Triangle
            m_stCurrentReport.bButton2  = ((pu8Data_[1] & 0x20) == 0);  //O
            m_stCurrentReport.bButton3  = ((pu8Data_[1] & 0x40) == 0);  //X
            m_stCurrentReport.bButton4  = ((pu8Data_[1] & 0x80) == 0);  //[]
        }
            break;

        case PSX_TYPE_ANALOG_GREEN:    // Analog Green Mode
        {           
            m_stCurrentReport.bButton8 = ((pu8Data_[1] & 0x01) == 0);
            m_stCurrentReport.bButton7 = ((pu8Data_[1] & 0x02) == 0);
            m_stCurrentReport.bButton4 = ((pu8Data_[1] & 0x04) == 0);
            m_stCurrentReport.bButton1 = ((pu8Data_[1] & 0x08) == 0);
            m_stCurrentReport.bButton5 = ((pu8Data_[1] & 0x10) == 0);
            m_stCurrentReport.bButton2 = ((pu8Data_[1] & 0x20) == 0);
            m_stCurrentReport.bButton3 = ((pu8Data_[1] & 0x40) == 0);
            m_stCurrentReport.bButton7 = ((pu8Data_[1] & 0x80) == 0);

            m_stCurrentReport.u16AnalogX2 = pu8Data_[2];
            m_stCurrentReport.u16AnalogY2 = pu8Data_[3];
            m_stCurrentReport.u16AnalogX1 = pu8Data_[4];
            m_stCurrentReport.u16AnalogY1 = pu8Data_[5];
        }
            break;
            
        case PSX_TYPE_ANALOG_RED:    // Analog Red Mode
        {
            m_stCurrentReport.bButton6  = ((pu8Data_[1] & 0x01) == 0);  //L1
            m_stCurrentReport.bButton5  = ((pu8Data_[1] & 0x02) == 0);  //R1
            m_stCurrentReport.bButton8  = ((pu8Data_[1] & 0x04) == 0);  //L2
            m_stCurrentReport.bButton7  = ((pu8Data_[1] & 0x08) == 0);  //R2
            m_stCurrentReport.bButton1  = ((pu8Data_[1] & 0x10) == 0);  //Triangle
            m_stCurrentReport.bButton2  = ((pu8Data_[1] & 0x20) == 0);  //O
            m_stCurrentReport.bButton3  = ((pu8Data_[1] & 0x40) == 0);  //X
            m_stCurrentReport.bButton4  = ((pu8Data_[1] & 0x80) == 0);  //[]
        
            m_stCurrentReport.u16AnalogX2 = pu8Data_[2];
            m_stCurrentReport.u16AnalogY2 = pu8Data_[3];
            m_stCurrentReport.u16AnalogX1 = pu8Data_[4];
            m_stCurrentReport.u16AnalogY1 = pu8Data_[5];
        }
            break;
        default:
            break;
    }
}

