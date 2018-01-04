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
using namespace Mark3;

//---------------------------------------------------------------------------
// Need to have this to handle pure-virtual functions
extern "C" void __cxa_pure_virtual()
{
}

namespace {
//---------------------------------------------------------------------------
// Global objects
Thread AppThread;  //!< Main "application" thread
Thread IdleThread; //!< Idle thread - runs when app can't

GraphicsFlavr clTFT_Sim; //!< TFT Display driver
// static GraphicsST7735 clTFT;

FlavrJoystick clJoy_Sim;
// static BasicJoystick clJoy;

SoundDriver clSound;

Semaphore clSem;
Timer     clTimer;

//---------------------------------------------------------------------------
uint8_t aucAppStack[PORT_KERNEL_DEFAULT_STACK_SIZE * 2];

//---------------------------------------------------------------------------
Breakout clBreakout;

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
    pclDisplay  = &clTFT_Sim;

    clSound.SetName("/dev/sound");
    clSound.Init();
    clSound.Open();

    auto lTimerCallback = [](Thread* /*pclOwner_*/, void* /*unused_*/) {
        clSem.Post();
    };

    clTimer.Start(true, 33, 0, lTimerCallback, 0);

    // Clear the screen
    pclDisplay->ClearScreen();
    clBreakout.TransitionToState(GAME_STATE_TITLE);

    while (1) {
        // Wait for 60Hz timer
        clSem.Pend();

        // Run the game logic for this frame.
        clBreakout.GameTick();

        if (KernelAware::IsSimulatorAware()) {
            clTFT_Sim.Flip();
        }
    }
}

//---------------------------------------------------------------------------
void IdleEntry(void)
{
#if !KERNEL_USE_IDLE_FUNC
    while (1) {
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
} // anonymous namespace

JoystickDriver* pclJoystick = &clJoy_Sim;
GraphicsDriver* pclDisplay = &clTFT_Sim;
SoundDriver*    pclSound = &clSound;
//---------------------------------------------------------------------------
int main(void)
{
    Kernel::Init(); //!< MUST be before other kernel ops

    AppThread.Init(aucAppStack,             //!< Pointer to the stack
                   sizeof(aucAppStack),          //!< Size of the stack
                   1,                       //!< Thread priority
                   (ThreadEntryFunc)AppEntry, //!< Entry function
                   (void*)&AppThread);      //!< Entry function argument

    AppThread.Start(); //!< Schedule the threads

#if KERNEL_USE_IDLE_FUNC
    Kernel::SetIdleFunc(IdleEntry);
#else
    IdleThread.Init(aucIdleStack,             //!< Pointer to the stack
                    sizeof(aucIdleStack),          //!< Size of the stack
                    0,                        //!< Thread priority
                    (ThreadEntry_t)IdleEntry, //!< Entry function
                    NULL);                    //!< Entry function argument

    IdleThread.Start();
#endif

    Kernel::Start(); //!< Start the kernel!
}
