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

    \file   kernelswi.cpp

    \brief  Kernel software interrupt implementation for Linux
*/


#include "kerneltypes.h"
#include "thread.h"
#include "threadport.h"
#include "kernelswi.h"

#include <stdio.h>
#include <stdlib.h>

// Pretty hokey...

//---------------------------------------------------------------------------
static volatile bool g_bIsEnabled = false;
static volatile bool g_bIsPending = false;

//---------------------------------------------------------------------------
void KernelSWI::Config(void)
{
    g_bIsEnabled = false;
    g_bIsPending = false;
}

//---------------------------------------------------------------------------
void KernelSWI::Start(void)
{
    g_bIsEnabled = true;
    g_bIsPending = false;
}

//---------------------------------------------------------------------------
void KernelSWI::Stop(void)
{    
    g_bIsEnabled = false;
}

//---------------------------------------------------------------------------
void KernelSWI::Clear(void)
{
    g_bIsPending = false;
}

//---------------------------------------------------------------------------
void KernelSWI::Trigger(void)
{
    if (!g_bIsEnabled)
    {
        fprintf(stderr, "SWI Not enabled\n");
        return;
    }

    if (g_bIsPending)
    {
        fprintf(stderr, "SWI Pending!\n");
        return;
    }
    g_bIsPending = true;    // We have a pending timer interrupt

    u16IntFlags |= FLAG_SWI;

    if( bIntEnabled ) {               
        // Only return back after
        g_bIsPending = false;

        // if interrupts are enabled, block thread
        ThreadPort::SaveContext();
    } else {
        // block thread when we re-enable CS
        bSwitchOnCS = true;
        g_bIsPending = false;
    }
}

//---------------------------------------------------------------------------
uint8_t KernelSWI::DI()
{
    uint8_t u8Ret = static_cast<uint8_t>(g_bIsEnabled);
    g_bIsEnabled = false;
    return u8Ret;
}

//---------------------------------------------------------------------------
void KernelSWI::RI(uint8_t u8RI_)
{
    if (u8RI_)
    {
        g_bIsEnabled = true;
    }
    else
    {
        g_bIsEnabled = false;
    }
}
