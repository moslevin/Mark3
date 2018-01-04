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
/*!

    \file   kernel++.cpp

    \brief  Test harness

*/

//---------------------------------------------------------------------------

#include "kerneltypes.h"
#include "kernel.h"
#include "thread.h"
#include "drvATMegaUART.h"
#include "slip.h"

#include "font.h"
#include "arial_10.h"

#include <avr/io.h>
#include <avr/sleep.h>

//---------------------------------------------------------------------------
// Need to have this to handle pure-virtual functions
extern "C" void __cxa_pure_virtual()
{
}

namespace {
using namespace Mark3;
//---------------------------------------------------------------------------
// Global objects
Thread AppThread;  //!< Main "application" thread
Thread IdleThread; //!< Idle thread - runs when app can't

ATMegaUART clUART; //!< UART device driver object

//---------------------------------------------------------------------------
#define UART_SIZE_RX (32) //!< UART RX Buffer size
#define UART_SIZE_TX (64) //!< UART TX Buffer size

//---------------------------------------------------------------------------
uint8_t aucAppStack[PORT_KERNEL_DEFAULT_STACK_SIZE];
uint8_t aucIdleStack[PORT_KERNEL_DEFAULT_STACK_SIZE];

//---------------------------------------------------------------------------
uint8_t aucTxBuffer[UART_SIZE_TX];
uint8_t aucRxBuffer[UART_SIZE_RX];

void Font_PrintGlyph(Font_t* pstFont_, uint8_t u8Glyph_)
{
    uint8_t i, j;
    uint8_t u8Mask;

    uint8_t u8Width;
    uint8_t u8Height;

    uint8_t u8TempByte;

    uint16_t u16Offset = 0;

    // If this font uses a subset of printable characters,
    // remove the offset before doing any calculations/pointer math
    u8Glyph_ -= pstFont_->u8StartChar;

    // Read the glyphs from memory until we arrive at the one we wish to print
    for (i = 0; i < u8Glyph_; i++) {
        // Glyphs are variable-sized for efficiency - to look up a particular
        // glyph, we must traverse all preceding glyphs in the list
        u8Width  = Font_ReadByte(u16Offset, pstFont_->pu8FontData);
        u8Height = Font_ReadByte(u16Offset + 1, pstFont_->pu8FontData);

        // Adjust the offset to point to the next glyph
        u16Offset += ((((uint16_t)u8Width + 7) >> 3) * (uint16_t)u8Height) + (sizeof(Glyph_t) - 1);
    }

    // Echo the character out to terminal, using an X for a lit pixel
    // and a space for a vacant pixel

    u8Width  = Font_ReadByte(u16Offset, pstFont_->pu8FontData);
    u8Height = Font_ReadByte(u16Offset + 1, pstFont_->pu8FontData);

    u16Offset += sizeof(Glyph_t) - 1;

    u8TempByte = Font_ReadByte(u16Offset, pstFont_->pu8FontData);
    for (i = 0; i < u8Height; i++) {
        u8Mask = 0x80;
        for (j = 0; j < u8Width; j++) {
            if (!u8Mask) {
                u8Mask = 0x80;
                u16Offset++;
                u8TempByte = Font_ReadByte(u16Offset, pstFont_->pu8FontData);
            }

            if (u8TempByte & u8Mask) {
                while (0 == clUART.Write(1, (uint8_t*)"X")) {
                }
            } else {
                while (0 == clUART.Write(1, (uint8_t*)" ")) {
                }
            }

            u8Mask >>= 1;
        }
        while (0 == clUART.Write(1, (uint8_t*)"\n")) {
        }
        u16Offset++;
        u8TempByte = Font_ReadByte(u16Offset, pstFont_->pu8FontData);
    }
    while (0 == clUART.Write(1, (uint8_t*)"\n")) {
    }
}

//---------------------------------------------------------------------------
void AppEntry(void)
{
    // Initialize the serial mux
    clUART.SetBuffers(aucRxBuffer, UART_SIZE_RX, aucTxBuffer, UART_SIZE_TX);
    clUART.Open();

    // Dummy code - just write hello world forever...
    while (1) {
        Font_PrintGlyph(&fntArial_10_False_False_False_, 'M');
        Font_PrintGlyph(&fntArial_10_False_False_False_, 'a');
        Font_PrintGlyph(&fntArial_10_False_False_False_, 'r');
        Font_PrintGlyph(&fntArial_10_False_False_False_, 'k');
        Font_PrintGlyph(&fntArial_10_False_False_False_, '3');
        Thread::Sleep(1000);
    }
}

//---------------------------------------------------------------------------
void IdleEntry(void)
{
    while (1) {
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
} // anonymous namespace

using namespace Mark3;
//---------------------------------------------------------------------------
int main(void)
{
    Kernel::Init(); //!< MUST be before other kernel ops

    AppThread.Init(aucAppStack,             //!< Pointer to the stack
                   sizeof(aucAppStack),          //!< Size of the stack
                   1,                       //!< Thread priority
                   (ThreadEntryFunc)AppEntry, //!< Entry function
                   (void*)&AppThread);      //!< Entry function argument

    IdleThread.Init(aucIdleStack,             //!< Pointer to the stack
                    sizeof(aucIdleStack),          //!< Size of the stack
                    0,                        //!< Thread priority
                    (ThreadEntryFunc)IdleEntry, //!< Entry function
                    NULL);                    //!< Entry function argument

    AppThread.Start(); //!< Schedule the threads
    IdleThread.Start();

    clUART.SetName("/dev/tty"); //!< Add the serial driver
    clUART.Init();

    DriverList::Add(&clUART);

    Kernel::Start(); //!< Start the kernel!
}
