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

    \file   flavr_gfx.cpp

    \brief  Test harness for Funkenstein Software flAVR simulated display.

*/

//---------------------------------------------------------------------------

#include "kerneltypes.h"
#include "kernel.h"
#include "thread.h"
#include "drvUART.h"
#include "graphics.h"
#include "graphics_flavr.h"
#include "draw.h"
#include "colorspace.h"
#include "kernelaware.h"
#include "tahoma_10.h"
#include "font.h"
#include <avr/io.h>
#include <avr/sleep.h>

//---------------------------------------------------------------------------
// Need to have this to handle pure-virtual functions
extern "C" void __cxa_pure_virtual() {}

//---------------------------------------------------------------------------
// Global objects
static Thread AppThread;			//!< Main "application" thread
static Thread IdleThread;			//!< Idle thread - runs when app can't

static GraphicsFlavr clflAVR;       //! flAVR simulated graphics driver object

//---------------------------------------------------------------------------
#define STACK_SIZE_APP		(192)	//!< Size of the main app's stack
#define STACK_SIZE_IDLE		(192)	//!< Size of the idle thread stack

//---------------------------------------------------------------------------
static K_UCHAR aucAppStack[STACK_SIZE_APP];
static K_UCHAR aucIdleStack[STACK_SIZE_IDLE];

//---------------------------------------------------------------------------
static void AppEntry(void);
static void IdleEntry(void);

//---------------------------------------------------------------------------
int main(void)
{		
	Kernel::Init();						//!< MUST be before other kernel ops

	AppThread.Init(	aucAppStack,		//!< Pointer to the stack
					STACK_SIZE_APP,		//!< Size of the stack
					1,					//!< Thread priority
					(ThreadEntry_t)AppEntry,	//!< Entry function
					(void*)&AppThread );//!< Entry function argument

	IdleThread.Init( aucIdleStack,		//!< Pointer to the stack
					 STACK_SIZE_IDLE,	//!< Size of the stack
					 0,					//!< Thread priority		
					 (ThreadEntry_t)IdleEntry,	//!< Entry function
					 NULL );			//!< Entry function argument

	AppThread.Start();					//!< Schedule the threads
	IdleThread.Start();

    clflAVR.SetName("/dev/display");     //!< Add the display driver
    clflAVR.Init();

    DriverList::Add( &clflAVR );

	Kernel::Start();					//!< Start the kernel!
}

//---------------------------------------------------------------------------
void AppEntry(void)
{    	
    GraphicsFlavr *my_gfx = (GraphicsFlavr*)(DriverList::FindByPath("/dev/display" ));

    my_gfx->Open();
    // Dummy code - just write hello world forever...
    my_gfx->ClearScreen();
    {
        DrawWindow_t stWindow;
        stWindow.usBottom = 159;
        stWindow.usTop = 0;
        stWindow.usLeft = 0;
        stWindow.usRight = 127;
        my_gfx->SetWindow(&stWindow);
    }
    Thread::Sleep(10);

    my_gfx->ClearScreen();
    Thread::Sleep(10);

    while(1)
	{
        DrawText_t stText;

        stText.usLeft = 8;
        stText.usTop = 8;
        stText.uColor = COLOR_RED;
        stText.pstFont = &fntTahoma_10_False_False_False_;
        stText.pcString = "Hello World!";
        my_gfx->Text(&stText);

        stText.usLeft = 8;
        stText.usTop = 16;
        stText.uColor = COLOR_RED;
        stText.pstFont = &fntTahoma_10_False_False_False_;
        stText.pcString = "Goodbye World!";
        my_gfx->Text(&stText);
        Thread::Sleep(10);

        my_gfx->Flip();
	}
}

//---------------------------------------------------------------------------
void IdleEntry(void)
{
	while(1)
	{
#if 1
		// LPM code;
		set_sleep_mode(SLEEP_MODE_IDLE);
		cli();
		sleep_enable();
		sei();
		sleep_cpu();
		sleep_disable();
		sei();
#endif
	}
}

