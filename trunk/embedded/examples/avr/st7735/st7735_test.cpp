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
#include "timerlist.h"
#include "ksemaphore.h"
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

static Semaphore clSem;
static Timer clTimer;

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
void Timer_Callback(Thread *pclOwner_, void *unused_)
{
    clSem.Post();
}

//---------------------------------------------------------------------------
void AppEntry(void)
{        
    clTFT.SetName("/dev/display");     //!< Add the display driver
    clTFT.Init();
    clTFT.Open();

    // Clear the screen
    clTFT.ClearScreen();

    // Demo 1 - Write some text to the display
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
    Thread::Sleep(2000);

    // Demo 2 - Draw an animated ball-and-paddle
    clSem.Init(0, 1);
    clTimer.Init();

    // Create a timer that expires every 16ms (approx 60Hz).  We use this to time our
    // updates to the display
    clTimer.Start( true, 16, 0, Timer_Callback,0 );

    // Variables used in the demo,
    K_USHORT usBallX = 0;
    K_USHORT usBallY = 0;

    bool bBallXDir = false;
    bool bBallYDir = false;

    // Objects used to draw/undraw the ball and paddle
    DrawRectangle_t stBall;
    DrawRectangle_t stPaddle;

    // Seed the ball/paddle original position.
    stBall.bFill = true;
    stBall.usLeft = usBallX;
    stBall.usTop = usBallY;
    stBall.usRight = usBallX+4;
    stBall.usBottom = usBallY+4;
    stBall.uLineColor = COLOR_BLUE;
    stBall.uFillColor = COLOR_BLUE;

    stPaddle.bFill = true;

    if (usBallX < 12)
    {
        stPaddle.usLeft = 0;
    }
    else if (usBallX >= 116)
    {
        stPaddle.usLeft = 104;
    }
    else
    {
        stPaddle.usLeft = usBallX - 12;
    }
    stPaddle.usRight = stPaddle.usLeft + 24;
    stPaddle.uLineColor = COLOR_WHITE;
    stPaddle.uFillColor = COLOR_WHITE;
    stPaddle.usTop = 152;
    stPaddle.usBottom = 155;

    // Draw the initial ball/paddle image
    clTFT.Rectangle(&stBall);
    clTFT.Rectangle(&stPaddle);

    while(1)
    {
        // Wait for our 60Hz timer to post the semaphore.
        clSem.Pend();

        // "Undraw" the previous ball, redraw it as quickly as possible at new location
        stBall.uFillColor = COLOR_BLACK;
        stBall.uLineColor = COLOR_BLACK;
        clTFT.Rectangle(&stBall);

        if (bBallXDir)
        {
            usBallX -= 2;
            if (usBallX == 0)
            {
                bBallXDir = false;
            }
        }
        else
        {
            usBallX += 2;
            if (usBallX >= 120)
            {
                bBallXDir = true;
            }
        }

        if (bBallYDir)
        {
            usBallY -= 2;
            if (usBallY == 0)
            {
                bBallYDir = false;
            }
        }
        else
        {
            usBallY += 2;
            if (usBallY >= 148)
            {
                bBallYDir = true;
            }
        }
        stBall.usLeft = usBallX;
        stBall.usTop = usBallY;
        stBall.usRight = usBallX+4;
        stBall.usBottom = usBallY+4;
        stBall.uLineColor = COLOR_BLUE;
        stBall.uFillColor = COLOR_BLUE;

        clTFT.Rectangle(&stBall);

        // Undraw previous paddle, redraw as quickly as possible at new location
        stPaddle.uLineColor = COLOR_BLACK;
        stPaddle.uFillColor = COLOR_BLACK;
        clTFT.Rectangle(&stPaddle);

        if (usBallX < 12)
        {
            stPaddle.usLeft = 0;
        }
        else if (usBallX >= 116)
        {
            stPaddle.usLeft = 104;
        }
        else
        {
            stPaddle.usLeft = usBallX - 12;
        }
        stPaddle.usRight = stPaddle.usLeft + 24;
        stPaddle.uLineColor = COLOR_WHITE;
        stPaddle.uFillColor = COLOR_WHITE;
        clTFT.Rectangle(&stPaddle);
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

