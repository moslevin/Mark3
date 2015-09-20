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
#include "mark3.h"

#include "graphics_flavr.h"
#include "graphics_st7735.h"
#include "font_8.h"
#include "draw.h"
#include "colorspace.h"
#include "font.h"
#include "tiles.h"
#include "breakout.h"
#include "drvFlavrJoy.h"
#include "drvBasicJoy.h"
#include "drvSound.h"

#include <avr/io.h>
#include <avr/sleep.h>

//---------------------------------------------------------------------------
// Need to have this to handle pure-virtual functions
extern "C" void __cxa_pure_virtual() {}

//---------------------------------------------------------------------------
// Global objects
static Thread AppThread;            //!< Main "application" thread
static Thread IdleThread;            //!< Idle thread - runs when app can't

static GraphicsFlavr clTFT_Sim;         //!< TFT Display driver
//static GraphicsST7735 clTFT;
GraphicsDriver *pclDisplay = &clTFT_Sim;

static FlavrJoystick clJoy_Sim;
//static BasicJoystick clJoy;

JoystickDriver *pclJoystick = &clJoy_Sim;

static SoundDriver clSound;
SoundDriver *pclSound = &clSound;

static Semaphore clSem;
static Timer clTimer;

static SongNote_t astNotes[] =
{
	{ TONE(C, 4), NOTE_QUARTER, 32 },
	{ TONE(D, 4), NOTE_HALF, 32 },
	{ TONE(E, 4), NOTE_QUARTER, 16 },
	{ TONE(F, 4), NOTE_HALF, 16 },
	{ 0,0,0 }
};

static Song_t stSong = 
{
	128,
	astNotes
};

//---------------------------------------------------------------------------
#define STACK_SIZE_APP        (320)    //!< Size of the main app's stack
#define STACK_SIZE_IDLE        (320)    //!< Size of the idle thread stack

//---------------------------------------------------------------------------
static uint8_t aucAppStack[STACK_SIZE_APP];
static uint8_t aucIdleStack[STACK_SIZE_IDLE];

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

    AppThread.Start();                    //!< Schedule the threads

#if KERNEL_USE_IDLE_FUNC
    Kernel::SetIdleFunc( IdleEntry );
#else
    IdleThread.Init( aucIdleStack,        //!< Pointer to the stack
                     STACK_SIZE_IDLE,    //!< Size of the stack
                     0,                    //!< Thread priority        
                     (ThreadEntry_t)IdleEntry,    //!< Entry function
                     NULL );            //!< Entry function argument
    
    IdleThread.Start();
#endif

    Kernel::Start();                    //!< Start the kernel!
}

//---------------------------------------------------------------------------
void Timer_Callback(Thread *pclOwner_, void *unused_)
{
    clSem.Post();
}

static Breakout clBreakout;

//---------------------------------------------------------------------------
void AppEntry(void)
{        
	// Set up system timing... 60Hz event loop
	
	clSem.Init(0, 1);
	clTimer.Init();

    // Init graphics
    clTFT_Sim.SetName("/dev/display");
    clTFT_Sim.Init();
    clTFT_Sim.Open();

    // Init Joysticks
    clJoy_Sim.SetName("/dev/joy");
    clJoy_Sim.Init();
    clJoy_Sim.Open();

    pclJoystick = &clJoy_Sim;
    pclDisplay = &clTFT_Sim;

	clSound.SetName("/dev/sound");
	clSound.Init();
	clSound.Open();

    clTimer.Start( true, 33, 0, Timer_Callback,0 );

    // Clear the screen
    pclDisplay->ClearScreen();
    clBreakout.TransitionToState(GAME_STATE_TITLE);
	
	while (1)
	{
		// Wait for 60Hz timer
		clSem.Pend();
		
		// Run the game logic for this frame.
		clBreakout.GameTick();

        if (KernelAware::IsSimulatorAware())
		{
        		clTFT_Sim.Flip();
		}
	}	
}

//---------------------------------------------------------------------------
void IdleEntry(void)
{
#if !KERNEL_USE_IDLE_FUNC
    while(1)
    {
#endif
        // LPM code;
        set_sleep_mode(SLEEP_MODE_IDLE);
        cli();
        sleep_enable();
        sei();
        sleep_cpu();
        sleep_disable();
        sei();
#if !KERNEL_USE_IDLE_FUNC
    }
#endif
}

