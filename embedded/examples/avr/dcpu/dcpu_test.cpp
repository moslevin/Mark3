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

    \file   dcpu_test.cpp

    \brief  "Hello World" implemented using the DCPU-16 VM 
*/
//---------------------------------------------------------------------------

#include "kerneltypes.h"
#include "dcpu.h"
#include "kernel.h"
#include "thread.h"
#include "drvUART.h"

#include <avr/io.h>
#include <avr/sleep.h>

//---------------------------------------------------------------------------
// Need to have this to handle pure-virtual functions
extern "C" void __cxa_pure_virtual() {}

//---------------------------------------------------------------------------
// Thread configuration
#define STACK_SIZE_APP      (256)   //!< Size of the main app's stack
#define STACK_SIZE_IDLE     (192)   //!< Size of the idle thread stack

//---------------------------------------------------------------------------
// Serial port configuration
#define UART_SIZE_RX        (32)    //!< UART RX Buffer size
#define UART_SIZE_TX        (64)    //!< UART TX Buffer size

//---------------------------------------------------------------------------
// Configuration for the DCPU VM
#define RAM_SIZE		    (32)    //!< Words of RAM for the VM (don't need much)

//---------------------------------------------------------------------------
// Configuration for the DCPU terminal device plugin
#define TERMINAL_DEVICE_NUMBER      (1)
#define TERMINAL_DEVICE_HWID        (0x00000001)
#define TERMINAL_DEVICE_VID         (0xBADC0FEE)
#define TERMINAL_DEVICE_VERSION     (1)

//---------------------------------------------------------------------------
// Global objects
static Thread AppThread;            //!< Main "application" thread
static Thread IdleThread;           //!< Idle thread - runs when app can't

static ATMegaUART clUART;           //!< UART device driver object
static DCPU clDCPU;                 //!< DCPU emulator object
static DCPUPlugin clTerminalPlugin; //!< DCPU Terminal Plugin object

static K_USHORT ausRAM[RAM_SIZE];   //!< RAM buffer for the DCPU emulator

/*
    This is the hex dump of the program assembled from "hello_world.a"
    We convert it to a K_USHORT array below, and use it as our VM's 
    program ROM.

    00000000  81 7f 1f 00 68 00 65 00  6c 00 6c 00 6f 00 20 00  |....h.e.l.l.o. .|
    00000010  77 00 6f 00 72 00 6c 00  64 00 21 00 0a 00 00 00  |w.o.r.l.d.!.....|
    00000020  21 03 63 7f 01 00 41 00  21 7c 00 00 01 7c 01 00  |!.c...A.!|...|..|
    00000030  40 02 62 7f 01 00 01 64  62 7f 01 00 81 67 01 7c  |@.b....db....g.||
    00000040  02 00 20 7c 10 00 81 7f  21 00                    |.. |....!.|
*/

static K_USHORT ausROM[] = 
{
    0x7f81, 0x001f, 0x0068, 0x0065, 0x006c, 0x006c, 0x006f, 0x0020,
    0x0077, 0x006f, 0x0072, 0x006c, 0x0064, 0x0021, 0x000a, 0x0000,
    0x0321, 0x7f63, 0x0001, 0x0041, 0x7c21, 0x0000, 0x7c01, 0x0001,
    0x0240, 0x7f62, 0x0001, 0x6401, 0x7f62, 0x0001, 0x6781, 0x7c01,
    0x0002, 0x7c20, 0x0010, 0x7f81, 0x0021,                        
};

//---------------------------------------------------------------------------
static K_UCHAR aucAppStack[STACK_SIZE_APP];
static K_UCHAR aucIdleStack[STACK_SIZE_IDLE];

//---------------------------------------------------------------------------
static K_UCHAR aucTxBuffer[UART_SIZE_TX];
static K_UCHAR aucRxBuffer[UART_SIZE_RX];

//---------------------------------------------------------------------------
static void AppEntry(void);
static void IdleEntry(void);

//---------------------------------------------------------------------------
int main(void)
{        
    Kernel::Init();                             //!< MUST be before other kernel ops
    
    AppThread.Init( aucAppStack,                //!< Pointer to the stack
                    STACK_SIZE_APP,             //!< Size of the stack
                    1,                          //!< Thread priority
                    (ThreadEntry_t)AppEntry,    //!< Entry function
                    (void*)&AppThread );        //!< Entry function argument

    IdleThread.Init( aucIdleStack,      //!< Pointer to the stack
                     STACK_SIZE_IDLE,   //!< Size of the stack
                     0,                 //!< Thread priority        
                     (ThreadEntry_t)IdleEntry,    //!< Entry function
                     NULL );            //!< Entry function argument
    
    AppThread.Start();                  //!< Schedule the threads
    IdleThread.Start();
    
    clUART.SetName("/dev/tty");         //!< Add the serial driver
    clUART.Init();    
    
    DriverList::Add( &clUART );
    
    Kernel::Start();                    //!< Start the kernel!
}

//---------------------------------------------------------------------------
static void PrintByte(const K_CHAR cChar_)
{
    while( 1 != clUART.Write( 1, (K_UCHAR*)&cChar_ ) ) { /* Do nothing */ }       
}

//---------------------------------------------------------------------------
static void Terminal_Callback( DCPU *pclVM_ )
{
    DCPU_Registers *pstRegisters = pclVM_->GetRegisters();
    const K_USHORT *pusString;
  
    // Terminal operation is stored in register "B"
    switch (pstRegisters->B)
    {
        case 0:	// Print from ROM
        {
            pusString = (const K_USHORT*)(&ausROM[pstRegisters->C]);
            while (*pusString)
            {
                PrintByte( (K_CHAR)((*pusString) & 0xFF) );
                pusString++;
            }
        }
        break;

        case 1: // Print from RAM
        {
            pusString = (const K_USHORT*)(&ausRAM[pstRegisters->C]);
            while (*pusString)
            {
                PrintByte( (K_CHAR)((*pusString) & 0xFF) );
                pusString++;
            }
        }
        break;

        default:
            break;
    }
}

//---------------------------------------------------------------------------
void AppEntry(void)
{
    // Initialize the serial port
    Driver *pclDriver = DriverList::FindByPath("/dev/tty");
    {
        K_ULONG ulBaud = 57600;
        pclDriver->Control(CMD_SET_BAUDRATE, (void*)&ulBaud, 0, 0, 0);
    }

    pclDriver->Control(CMD_SET_BUFFERS, 
                       (void*)aucRxBuffer, UART_SIZE_RX, 
                       (void*)aucTxBuffer, UART_SIZE_TX);
    pclDriver->Open();

    // Initialize the DCPU virtual terminal plugin
    clTerminalPlugin.Init( TERMINAL_DEVICE_NUMBER,
                            TERMINAL_DEVICE_HWID,
                            TERMINAL_DEVICE_VID,
                            TERMINAL_DEVICE_VERSION,
                            Terminal_Callback );

    // Initialize the DCPU emulator
    clDCPU.Init(ausRAM, RAM_SIZE, ausROM, sizeof(ausROM)/sizeof(K_USHORT));
    clDCPU.AddPlugin(&clTerminalPlugin);

    // Run VM opcodes in an endless loop
    while(1)
    {
        clDCPU.RunOpcode();  
    }
}

//---------------------------------------------------------------------------
void IdleEntry(void)
{
    while(1)
    {
        // LPM code;
        set_sleep_mode(SLEEP_MODE_IDLE);
        cli();
        sleep_enable();
        sei();
        sleep_cpu();
        sleep_disable();
        sei();
    }
}

