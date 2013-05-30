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

    \file   timerlist.h    

    \brief  Timer list and timer-scheduling declarations
    
    These classes implements a linked list of timer objects attached to the 
	global kernel timer.  Unlike other kernels which use a fully-synchronous
	"tick-based" timing mechanism, where the OS timing facilities are based on 
	a fixed-frequency timer (which causes regular timer interrupts), Mark3
	uses a "tickless" timer implementation, which only triggers interrupts when
	absolutely required.  This is much more efficient in most cases - timer 
	interrupts occur less frequently, allowing the kernel to stay in sleep 
	much longer than it would otherwise.
*/

#ifndef __TIMERLIST_H__
#define __TIMERLIST_H__

#include "kerneltypes.h"
#include "mark3cfg.h"

#include "ll.h"
#include "thread.h"

#if KERNEL_USE_TIMERS

//---------------------------------------------------------------------------
#define TIMERLIST_FLAG_ONE_SHOT         (0x01)	//!< Timer is one-shot
#define TIMERLIST_FLAG_ACTIVE           (0x02)	//!< Timer is currently active
#define TIMERLIST_FLAG_CALLBACK         (0x04)	//!< Timer is pending a callback
#define TIMERLIST_FLAG_EXPIRED          (0x08)	//!< Timer is actually expired.

//---------------------------------------------------------------------------
#define MAX_TIMER_TICKS                 (0x7FFFFFFF)	//!< Maximum value to set

//---------------------------------------------------------------------------
/*
	Ugly macros to support a wide resolution of delays.
	Given a 16-bit timer @ 16MHz & 256 cycle prescaler, this gives us...
	Max time, SECONDS_TO_TICKS:  68719s
	Max time, MSECONDS_TO_TICKS: 6871.9s
	Max time, USECONDS_TO_TICKS: 6.8719s
	 With a 16us tick resolution.
*/
//---------------------------------------------------------------------------
#define SECONDS_TO_TICKS(x)				((((K_ULONG)x) * TIMER_FREQ))
#define MSECONDS_TO_TICKS(x)			((((((K_ULONG)x) * (TIMER_FREQ/100)) + 5) / 10))
#define USECONDS_TO_TICKS(x)			((((((K_ULONG)x) * TIMER_FREQ) + 50000) / 1000000))

//---------------------------------------------------------------------------
#define MIN_TICKS						(3)	//!< The minimum tick value to set
//---------------------------------------------------------------------------
typedef void (*TimerCallback_t)(Thread *pclOwner_, void *pvData_);

//---------------------------------------------------------------------------
class TimerList;
class TimerScheduler;
class Quantum;
/*!
	Timer - an event-driven execution context based on a specified time 
	interval.  This inherits from a LinkListNode for ease of management by
	a global TimerList object.
*/
class Timer : public LinkListNode
{
public:
	/*!
		Default Constructor - zero-initializes all internal data.
	*/
	Timer(){ m_ulInterval = 0; m_ulTimeLeft = 0; m_ucFlags = 0; }
	
    /*!
        Start a timer using default ownership, using repeats as an option, and 
        millisecond resolution.
    */
	void Start( K_UCHAR bRepeat_, K_ULONG ulIntervalMs_, TimerCallback_t pfCallback_, void *pvData_ );
	
    /*!
        Stop a timer already in progress.   Has no effect on timers that have
		already been stopped.
    */
	void Stop();
    
	/*!
        \fn void SetFlags (K_UCHAR ucFlags_)
        
		Set the timer's flags based on the bits in the ucFlags_ argument
        
        \param ucFlags_ Flags to assign to the timer object.
                    TIMERLIST_FLAG_ONE_SHOT for a one-shot timer,
                    0 for a continuous timer.
	*/
	void SetFlags (K_UCHAR ucFlags_) { m_ucFlags = ucFlags_; };
		
	/*!
        \fn void SetCallback( TimerCallback_t pfCallback_)
        
		Define the callback function to be executed on expiry of the timer
        
        \param pfCallback_ Pointer to the callback function to call
	*/
    void SetCallback( TimerCallback_t pfCallback_){ m_pfCallback = pfCallback_; }
		
	/*!
        \fn void SetData( void *pvData_ )
        
		Define a pointer to be sent to the timer callbcak on timer expiry
        
        \param pvData_ Pointer to data to pass as argument into the callback
	*/
    void SetData( void *pvData_ ){ m_pvData = pvData_; }
		
	/*!
        \fn void SetOwner( Thread *pclOwner_)
        
		Set the owner-thread of this timer object (all timers must be owned by
		a thread).
        
        \param pclOwner_ Owner thread of this timer object
	*/
    void SetOwner( Thread *pclOwner_){ m_pclOwner = pclOwner_; }
		
	/*!
        \fn void SetIntervalTicks(K_ULONG ulTicks_)
        
		Set the timer expiry in system-ticks (platform specific!)
        
        \param ulTicks_ Time in ticks
	*/
    void SetIntervalTicks(K_ULONG ulTicks_);
	
	/*!
        \fn void SetIntervalSeconds(K_ULONG ulSeconds_);
        
		Set the timer expiry interval in seconds (platform agnostic)
        
        \param ulSeconds_ Time in seconds
	*/
    void SetIntervalSeconds(K_ULONG ulSeconds_);
    
	/*!
        \fn void SetIntervalMSeconds(K_ULONG ulMSeconds_)
        
		Set the timer expiry interval in milliseconds (platform agnostic)
        
        \param ulMSeconds_ Time in milliseconds
	*/
	void SetIntervalMSeconds(K_ULONG ulMSeconds_);
    
	/*!
        \fn void SetIntervalUSeconds(K_ULONG ulUSeconds_)
        
		Set the timer expiry interval in microseconds (platform agnostic)
        
        \param ulUSeconds_ Time in microseconds
	*/
	void SetIntervalUSeconds(K_ULONG ulUSeconds_);
	
private:
    
	friend class TimerList;
    
    //! Flags for the timer, defining if the timer is one-shot or repeated
    K_UCHAR m_ucFlags;

    //! Pointer to the callback function
    TimerCallback_t m_pfCallback;
    
    //! Interval of the timer in timer ticks
    K_ULONG m_ulInterval;
    
    //! Time remaining on the timer
    K_ULONG m_ulTimeLeft;

    //! Pointer to the owner thread
    Thread  *m_pclOwner;
    
    //! Pointer to the callback data
    void    *m_pvData;
};

//---------------------------------------------------------------------------
/*!
	TimerList class - a doubly-linked-list of timer objects.
*/
class TimerList : public DoubleLinkList
{
public:
	/*! 
		\fn void Init()
	
        Initialize the TimerList object.  Must be called before 
		using the object.		
	*/
    void Init();
	
	/*!
		\fn void Add(Timer *pclListNode_)
		
		Add a timer to the TimerList.
		
		\param pclListNode_ Pointer to the Timer to Add
	*/
    void Add(Timer *pclListNode_);
	
	/*!
		\fn void Remove(Timer *pclListNode_)
		
		Remove a timer from the TimerList, cancelling its expiry.
		
		\param pclListNode_ Pointer to the Timer to remove
	*/
    void Remove(Timer *pclListNode_);
	
	/*!
		\fn void Process()
        
		Process all timers in the timerlist as a result of the timer expiring.
		This will select a new timer epoch based on the next timer to expire.
	*/
    void Process();
    
private:
	//! The time (in system clock ticks) of the next wakeup event
    K_ULONG m_ulNextWakeup;
	
	//! Whether or not the timer is active
    K_UCHAR m_bTimerActive;    
};

//---------------------------------------------------------------------------
/*!
	"Static" Class used to interface a global TimerList with the rest of the
	kernel.
*/
class TimerScheduler
{
public:
	/*!
		\fn void Init()
        
		Initialize the timer scheduler.  Must be called before any timer, or
		timer-derived functions are used.
	*/
	static void Init() { m_clTimerList.Init(); }
		
	/*!
		\fn void Add(Timer *pclListNode_) 
        
		Add a timer to the timer scheduler.  Adding a timer implicitly starts
		the timer as well.		
		
		\param pclListNode_ Pointer to the timer list node to add
	*/
	static void Add(Timer *pclListNode_) 
		{m_clTimerList.Add(pclListNode_); }
		
	/*!
		\fn void Remove(Timer *pclListNode_)
        
		Remove a timer from the timer scheduler.  May implicitly stop the 
		timer if this is the only active timer scheduled.
		
		\param pclListNode_ Pointer to the timer list node to remove
	*/
	static void Remove(Timer *pclListNode_) 
		{m_clTimerList.Remove(pclListNode_); }
		
	/*!
		\fn void Process()
        
		This function must be called on timer expiry (from the timer's ISR 
		context).  This will result in all timers being updated based on 
		the epoch that just elapsed.  New timer epochs are set based on the 
		next timer to expire.
	*/
	static void Process() {m_clTimerList.Process();}
private:

	//! TimerList object manipulated by the Timer Scheduler
	static TimerList m_clTimerList;
};

#endif // KERNEL_USE_TIMERS

#endif