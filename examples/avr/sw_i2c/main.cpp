/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012 - 2017 Funkenstein Software Consulting, all rights reserved.
See license.txt for more information
===========================================================================*/
/*
 * Software I2C Demo - This example shows how the software I2C library can
 * be configured for a target board to write-to and read-from an 24C256
 * EEPROM connected to arbitrary GPIOs on the CPU.
 */

#include "sw_i2c.h"
#include "drvUARTplus.h"
#include "memutil.h"

#include "mark3.h"

#include <avr/io.h>
#include <avr/interrupt.h>

namespace {
using namespace Mark3;

//---------------------------------------------------------------------------
// Software i2c driver port configuration
// SCL - Port C0
#define SCL_DIR     DDRC
#define SCL_OUT     PORTC
#define SCL_BIT     0x01
#define SCL_PIN     PINC

//---------------------------------------------------------------------------
// SDA - Port C1
#define SDA_DIR     DDRC
#define SDA_OUT     PORTC
#define SDA_BIT     0x02
#define SDA_PIN     PINC


//---------------------------------------------------------------------------
// I2C class extending the basic protocol driver for use with the configured
// GPIOs.
class TestI2C : public SoftwareI2C
{
private:    
    virtual void BitDelay(void)
    {
        Thread::USleep(GetDelayTimeUsec());
    }

    virtual void SetSCL(i2c_level_t eLevel_)
    {
        if (eLevel_) {
            // Hi-Z input
            SCL_DIR &= ~SCL_BIT;
            SCL_OUT |= SCL_BIT;
        } else {
            // Output Low
            SCL_DIR |= SCL_BIT;
            SCL_OUT &= ~SCL_BIT;
        }
    }

    virtual void SetSDA(i2c_level_t eLevel_)
    {
        if (eLevel_) {
            // Hi-Z input
            SDA_DIR &= ~SDA_BIT;
            SDA_OUT |= SDA_BIT;
        } else {
            // Output low
            SDA_DIR |= SDA_BIT;
            SDA_OUT &= ~SDA_BIT;
        }
    }
    virtual void GetSCL(i2c_level_t* peLevel_)
    {        
        uint8_t u8Pin = SCL_PIN;
        if ((u8Pin & SCL_BIT) == SCL_BIT) {
            *peLevel_ = I2C_HIGH;
        } else {
            *peLevel_ = I2C_LOW;
        }
    }

    virtual void GetSDA(i2c_level_t* peLevel_)
    {
        uint8_t u8Pin = SDA_PIN;
        if ((u8Pin & SDA_BIT) == SDA_BIT) {
            *peLevel_ = I2C_HIGH;
        } else {
            *peLevel_ = I2C_LOW;
        }
    }
};

//---------------------------------------------------------------------------
// I2C driver instance
TestI2C s_clI2C;

//---------------------------------------------------------------------------
// UART object + buffer data
ATMegaUARTPlus s_clUart;
uint8_t s_u8RxBuffer[32];
uint8_t s_u8TxBuffer[32];

//---------------------------------------------------------------------------
// Threads + Stacks
Thread s_clMainTask;
Thread s_clIdleTask;
K_WORD s_aucMainStack[512];
K_WORD s_aucIdleStack[512];

//---------------------------------------------------------------------------
// Task using the EEPROM driver
void MainTask(void* unused)
{
    (void)unused;

    // Initialize the UART driver - configure the port, baudrate, and buffers,
    // and ensure UART is running in blocking IO mode.
    s_clUart.Init();
    s_clUart.SetBuffers(s_u8RxBuffer, 32, s_u8TxBuffer, 32);
    s_clUart.SetBaudRate(57600);
    s_clUart.SetPortIdentity(0);
    s_clUart.SetPortBlocking(true);
    s_clUart.Open();

    // Initialize the I2C driver - run it slow, allowing it to run in the background
    // efficiently without
    s_clI2C.Init(500);

    // Log current status over the UART
    s_clUart.Write(7, (uint8_t*)"Begin\r\n");
    uint8_t counter = 0;

    while(1) {
        const char* err_msg = "err\r\n";

        // Print out the current counter value
        counter++;
        char szTmp[4] = {0};

        MemUtil::DecimalToString(counter, szTmp);
        s_clUart.Write(4, (uint8_t*)szTmp);
        s_clUart.Write(1, (uint8_t*)"=");

        bool bAck;
        s_clI2C.SendStart();
        // Write device address 0x50
        i2c_return_t rc = s_clI2C.WriteAddress(0x50, false, &bAck);

        if ((rc != I2C_OK) || (!bAck)) {
            s_clUart.Write(5, (uint8_t*)err_msg);
            s_clI2C.SendStop();
            continue;
        }

        // Write page address 0:0
        s_clI2C.WriteByte(0, &bAck);
        rc = s_clI2C.WriteByte(0, &bAck);

        if ((rc != I2C_OK) || (!bAck)) {
            s_clUart.Write(5, (uint8_t*)err_msg);
            s_clI2C.SendStop();
            continue;
        }

        // Write a counter to the EEPROM @ address 0.
        rc = s_clI2C.WriteByte(counter, &bAck);

        if ((rc != I2C_OK) || (!bAck)) {
            s_clUart.Write(5, (uint8_t*)err_msg);
            s_clI2C.SendStop();
            continue;
        }

        s_clI2C.SendStop();

        // Read the data back
        s_clI2C.SendStart();

        // Write device address 0x50
        rc = s_clI2C.WriteAddress(0x50, false, &bAck);

        if ((rc != I2C_OK) || (!bAck)) {
            s_clUart.Write(5, (uint8_t*)err_msg);
            s_clI2C.SendStop();
            continue;
        }

        // Write page address 0:0
        s_clI2C.WriteByte(0, &bAck);
        rc = s_clI2C.WriteByte(0, &bAck);

        if ((rc != I2C_OK) || (!bAck)) {
            s_clUart.Write(5, (uint8_t*)err_msg);
            s_clI2C.SendStop();
            continue;
        }

        // Send a start condition
        s_clI2C.SendStart();

        // Read address 0x50
        rc = s_clI2C.WriteAddress(0x50, true, &bAck);

        if ((rc != I2C_OK) || (!bAck)) {
            s_clUart.Write(5, (uint8_t*)err_msg);
            s_clI2C.SendStop();
            continue;
        }

        // Read a byte of data
        uint8_t verify = 0xFF;
        rc = s_clI2C.ReadByte(&verify, false);
        if (rc != I2C_OK) {
            s_clUart.Write(5, (uint8_t*)err_msg);
            s_clI2C.SendStop();
            continue;
        }

        // Done reading - send stop signal
        s_clI2C.SendStop();

        // Verify that what we written was read back OK
        MemUtil::DecimalToString(verify, szTmp);
        s_clUart.Write(4, (uint8_t*)szTmp);
        s_clUart.Write(1, (uint8_t*) " ");
        if (verify == counter) {
            // Success!!
            s_clUart.Write(4, (uint8_t*)":)\r\n");
        } else {
            // Fail!!
            s_clUart.Write(4, (uint8_t*)":(\r\n");
        }
    }
}
static void IdleTask(void* unused)
{
    while (1) {
        // Do nothing at idle..
    }
}
} // anonymous namespace

using namespace Mark3;
int main(void)
{
    // Init the kernel
    Kernel::Init();

    // Start the main + idle threads
    s_clMainTask.Init(s_aucMainStack, sizeof(s_aucMainStack), 1, MainTask, 0);
    s_clIdleTask.Init(s_aucIdleStack, sizeof(s_aucIdleStack), 0, IdleTask, 0);

    s_clMainTask.Start();
    s_clIdleTask.Start();

    // Start the kernel - never returns
    Kernel::Start();
}
