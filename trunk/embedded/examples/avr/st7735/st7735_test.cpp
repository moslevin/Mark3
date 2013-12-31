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

    \file   uoled_test.cpp

    \brief  Test harness for ST7735-based display driver.

*/

//---------------------------------------------------------------------------

#include "kerneltypes.h"
#include "kernel.h"
#include "thread.h"
#include "graphics_st7735.h"
#include "tahoma_10.h"
#include "draw.h"
#include "colorspace.h"
#include "font.h"
#include <avr/io.h>
#include <avr/sleep.h>

//---------------------------------------------------------------------------
// Need to have this to handle pure-virtual functions
extern "C" void __cxa_pure_virtual() {}

//---------------------------------------------------------------------------
// Global objects
static Thread AppThread;            //!< Main "application" thread
static Thread IdleThread;            //!< Idle thread - runs when app can't

static GraphicsST7735 clTFT;         //!< TFT Display driver

//---------------------------------------------------------------------------
#define STACK_SIZE_APP        (320)    //!< Size of the main app's stack
#define STACK_SIZE_IDLE        (320)    //!< Size of the idle thread stack

//---------------------------------------------------------------------------
static K_UCHAR aucAppStack[STACK_SIZE_APP];
static K_UCHAR aucIdleStack[STACK_SIZE_IDLE];

//---------------------------------------------------------------------------
static void AppEntry(void);
static void IdleEntry(void);

//---------------------------------------------------------------------------
int main(void)
{        
    Kernel::Init();                        //!< MUST be before other kernel ops
    
    AppThread.Init(    aucAppStack,        //!< Pointer to the stack
                    STACK_SIZE_APP,        //!< Size of the stack
                    1,                    //!< Thread priority
                    (ThreadEntry_t)AppEntry,    //!< Entry function
                    (void*)&AppThread );//!< Entry function argument

    IdleThread.Init( aucIdleStack,        //!< Pointer to the stack
                     STACK_SIZE_IDLE,    //!< Size of the stack
                     0,                    //!< Thread priority        
                     (ThreadEntry_t)IdleEntry,    //!< Entry function
                     NULL );            //!< Entry function argument
    
    AppThread.Start();                    //!< Schedule the threads
    IdleThread.Start();
         
    Kernel::Start();                    //!< Start the kernel!
}

//---------------------------------------------------------------------------
void AppEntry(void)
{        
    clTFT.SetName("/dev/display");     //!< Add the display driver
    clTFT.Init();
    clTFT.Open();

    // Dummy code - just write hello world forever...
    clTFT.ClearScreen();

    DrawText_t stText;

    stText.usLeft = 8;
    stText.usTop = 8;
    stText.uColor = COLOR_RED;
    stText.pstFont = &fntTahoma_10_False_False_False_;
    stText.pcString = "Hello World!";
    clTFT.Text(&stText);

    stText.usLeft = 8;
    stText.usTop = 22;
    stText.uColor = COLOR_RED;
    stText.pstFont = &fntTahoma_10_False_False_False_;
    stText.pcString = "Goodbye World!";
    clTFT.Text(&stText);
    Thread::Sleep(10);

    K_USHORT usXOff = 0;
    K_USHORT usYOff = 0;

    while(1)
    {

        for (usXOff = 0; usXOff <= 120; usXOff+= 8)
        {
            for (usYOff = 0; usYOff <= 152; usYOff += 8)
            {
                DrawRectangle_t stRect;

                stRect.bFill = true;
                stRect.usLeft = usXOff;
                stRect.usTop = usYOff;
                stRect.usRight = usXOff+7;
                stRect.usBottom = usYOff+7;
                stRect.uLineColor = COLOR_BLUE;
                stRect.uFillColor = COLOR_BLUE;
                clTFT.Rectangle(&stRect);
            }
        }
        clTFT.ClearScreen();
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

