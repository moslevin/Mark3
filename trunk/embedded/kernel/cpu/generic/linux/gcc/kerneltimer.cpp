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

    \file   kerneltimer.cpp

    \brief  Kernel Timer Implementation for Linux
*/

#include "kerneltypes.h"
#include "kerneltimer.h"
#include "thread.h"
#include "threadport.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <signal.h>
#include <errno.h>


volatile bool bEnabled;
volatile bool bStarted;

//---------------------------------------------------------------------------
static void sig_handler( int signal )
{    
    fprintf(stderr, "SigHandler\n");
    if (!(bEnabled && bStarted))
    {
        return;
    }

    if (u16IntFlags & FLAG_TIMER)
    {
        return;
    }

    u16IntFlags |= FLAG_TIMER;

    if (bIntEnabled)
    {
        fprintf(stderr, "Trigger Timer Interrupt\n");
        sem_post(&stKSem);
    }
    else
    {
        fprintf(stderr, "Trigger generic interrupt\n");
        bTriggerInt = true;
    }
}

//---------------------------------------------------------------------------
void KernelTimer::Config(void)
{
    struct itimerval new_val;
    struct sigaction sa;

    fprintf(stderr, "Installed sighandler\n");
    memset( &sa, 0, sizeof(sa) );
    sa.sa_handler = sig_handler;
    sigaction(SIGVTALRM, &sa, NULL);
}

//---------------------------------------------------------------------------
void KernelTimer::Start(void)
{
    bStarted = true;
    bEnabled = true;
    fprintf(stderr, "Timer Start\n");
}

//---------------------------------------------------------------------------
void KernelTimer::Stop(void)
{
    bStarted = false;
    bEnabled = false;
    fprintf(stderr, "Timer Stop\n");
}

//---------------------------------------------------------------------------
uint16_t KernelTimer::Read(void)
{
    return 0;
}

//---------------------------------------------------------------------------
uint32_t KernelTimer::SubtractExpiry(uint32_t u32Interval_)
{
    uint32_t u32TempInt = TimeToExpiry();

    if (u32TempInt >= u32Interval_)
    {
        u32TempInt -= u32Interval_;
    }

    SetExpiry(u32TempInt);
    return u32TempInt;
}

//---------------------------------------------------------------------------
uint32_t KernelTimer::TimeToExpiry(void)
{
    struct itimerval curr_time;
    uint32_t u32Time;
    getitimer(ITIMER_VIRTUAL, &curr_time);

    u32Time = curr_time.it_value.tv_sec * 1000000;
    u32Time += curr_time.it_value.tv_usec;
    return u32Time;
}

//---------------------------------------------------------------------------
uint32_t KernelTimer::GetOvertime(void)
{
    // assume infinitely-fast timer callback processing.  We're using LINUX
    // so our realtime processing is kind of approximate anyway...
    return 0;
}

//---------------------------------------------------------------------------
uint32_t KernelTimer::SetExpiry(uint32_t u32Interval_)
{
    //!! Todo - support for limited timers (to 0x7FFFFFFF)
    struct itimerval new_timer;
    new_timer.it_value.tv_sec = u32Interval_ / 1000000;
    new_timer.it_value.tv_usec = u32Interval_ % 1000000;
    new_timer.it_interval.tv_sec = u32Interval_ / 1000000;
    new_timer.it_interval.tv_usec = u32Interval_ / 1000000;

    fprintf(stderr, "set expiry %d\n", u32Interval_);

    int err = setitimer(ITIMER_VIRTUAL, &new_timer, NULL );
    if (0 != err)
    {
        fprintf(stderr, "Timer error: %d\n", errno);
    }
    return u32Interval_;
}

//---------------------------------------------------------------------------
void KernelTimer::ClearExpiry(void)
{
    struct itimerval new_timer;
    memset(&new_timer, 0, sizeof(new_timer) );

    setitimer(ITIMER_VIRTUAL, &new_timer, NULL);
    fprintf(stderr, "Clear Expiry\n");
}

//---------------------------------------------------------------------------
uint8_t KernelTimer::DI(void)
{    
    KernelTimer::RI(0);
}

//---------------------------------------------------------------------------
void KernelTimer::EI(void)
{
    KernelTimer::RI(1);
}

//---------------------------------------------------------------------------
void KernelTimer::RI(bool bEnable_)
{    
    bEnabled = bEnable_;
}
