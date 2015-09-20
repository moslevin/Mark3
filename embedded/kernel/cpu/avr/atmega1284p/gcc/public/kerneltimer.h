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

    \file   kerneltimer.h    

    \brief  Kernel Timer Class declaration
*/

#include "kerneltypes.h"

#ifndef __KERNELTIMER_H_
#define __KERNELTIMER_H_

//---------------------------------------------------------------------------
#define SYSTEM_FREQ        ((uint32_t)16000000)
#define TIMER_FREQ        ((uint32_t)(SYSTEM_FREQ / 256)) // Timer ticks per second...

//---------------------------------------------------------------------------
/*!
    Hardware timer interface, used by all scheduling/timer subsystems.
*/
class KernelTimer
{
public:
    /*!
        \fn void Config(void)
        
        Initializes the kernel timer before use
    */
    static void Config(void);
    
    /*!
        \fn void Start(void)
        
        Starts the kernel time (must be configured first)
    */
    static void Start(void);
    
    /*!
        \fn void Stop(void)
        
        Shut down the kernel timer, used when no timers are scheduled
    */
    static void Stop(void);
    
    /*!
        \fn uint8_t DI(void)
        
        Disable the kernel timer's expiry interrupt
    */
    static uint8_t DI(void);
    
    /*!
        \fn void RI(bool bEnable_)
        
        Retstore the state of the kernel timer's expiry interrupt.
        
        \param bEnable_ 1 enable, 0 disable
    */
    static void RI(bool bEnable_);
    
    /*!
        \fn void EI(void)
        
        Enable the kernel timer's expiry interrupt
    */
    static void EI(void);

    /*!
        \fn uint32_t SubtractExpiry(uint32_t u32Interval_)
        
        Subtract the specified number of ticks from the timer's 
        expiry count register.  Returns the new expiry value stored in 
        the register.
        
        \param u32Interval_ Time (in HW-specific) ticks to subtract
        \return Value in ticks stored in the timer's expiry register
    */
    static uint32_t SubtractExpiry(uint32_t u32Interval_);
    
    /*!
        \fn uint32_t TimeToExpiry(void)
        
        Returns the number of ticks remaining before the next timer 
        expiry.
        
        \param Time before next expiry in platform-specific ticks
    */
    static uint32_t TimeToExpiry(void);
    
    /*!
        \fn uint32_t SetExpiry(uint32_t u32Interval_)
        
        Resets the kernel timer's expiry interval to the specified value
        
        \param u32Interval_ Desired interval in ticks to set the timer for
        \return Actual number of ticks set (may be less than desired)        
    */
    static uint32_t SetExpiry(uint32_t u32Interval_);
    
    /*!
        \fn uint32_t GetOvertime(void)
        
        Return the number of ticks that have elapsed since the last
        expiry.
        
        \return Number of ticks that have elapsed after last timer expiration
    */
    static uint32_t GetOvertime(void);
    
    /*!
        \fn void ClearExpiry(void)
        
        Clear the hardware timer expiry register
    */
    static void ClearExpiry(void);
    
private:
    /*!
        \fn uint16_t Read(void)
        
        Safely read the current value in the timer register
        
        \return Value held in the timer register
    */
    static uint16_t Read(void);
    
};

#endif //__KERNELTIMER_H_
