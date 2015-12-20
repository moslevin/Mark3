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
=========================================================================== */
/*!

    \file   timer.h

    \brief  Timer object declarations
 */

#ifndef __TIMER_H__
#define __TIMER_H__

#include "kerneltypes.h"
#include "mark3cfg.h"

#include "ll.h"

#if KERNEL_USE_TIMERS
class Thread;

//---------------------------------------------------------------------------
#define TIMERLIST_FLAG_ONE_SHOT         (0x01)    //!< Timer is one-shot
#define TIMERLIST_FLAG_ACTIVE           (0x02)    //!< Timer is currently active
#define TIMERLIST_FLAG_CALLBACK         (0x04)    //!< Timer is pending a callback
#define TIMERLIST_FLAG_EXPIRED          (0x08)    //!< Timer is actually expired.

//---------------------------------------------------------------------------
#define MAX_TIMER_TICKS                 (0x7FFFFFFF)    //!< Maximum value to set

//---------------------------------------------------------------------------
#if KERNEL_TIMERS_TICKLESS

//---------------------------------------------------------------------------
/*
    Ugly macros to support a wide resolution of delays.
    Given a 16-bit timer @ 16MHz & 256 cycle prescaler, this gives u16...
    Max time, SECONDS_TO_TICKS:  68719s
    Max time, MSECONDS_TO_TICKS: 6871.9s
    Max time, useCONDS_TO_TICKS: 6.8719s

    ...With a 16us tick resolution.

    Depending on the system frequency and timer resolution, you may want to
    customize these values to suit your system more appropriately.
 */
//---------------------------------------------------------------------------
#define SECONDS_TO_TICKS(x)             ((((uint32_t)x) * TIMER_FREQ))
#define MSECONDS_TO_TICKS(x)            ((((((uint32_t)x) * (TIMER_FREQ/100)) + 5) / 10))
#define useCONDS_TO_TICKS(x)            ((((((uint32_t)x) * TIMER_FREQ) + 50000) / 1000000))

//---------------------------------------------------------------------------
#define MIN_TICKS                        (3)    //!< The minimum tick value to set
//---------------------------------------------------------------------------

#else

//---------------------------------------------------------------------------
// add time because we don't know how far in an epoch we are when a call is made.
#define SECONDS_TO_TICKS(x)             (((uint32_t)(x) * 1000) + 1)
#define MSECONDS_TO_TICKS(x)            ((uint32_t)(x + 1))
#define useCONDS_TO_TICKS(x)            (((uint32_t)(x + 999)) / 1000)

//---------------------------------------------------------------------------
#define MIN_TICKS                       (1)    //!< The minimum tick value to set
//---------------------------------------------------------------------------

#endif // KERNEL_TIMERS_TICKLESS

//---------------------------------------------------------------------------
/*!
 * This type defines the callback function type for timer events.  Since these
 * are called from an interrupt context, they do not operate from within a
 * thread or object context directly -- as a result, the context must be
 * manually passed into the calls.
 *
 * pclOwner_ is a pointer to the thread that owns the timer
 * pvData_ is a pointer to some data or object that needs to know about the
 *         timer's expiry from within the timer interrupt context.
  */
typedef void (*TimerCallback_t)(Thread *pclOwner_, void *pvData_);

//---------------------------------------------------------------------------
class TimerList;
class TimerScheduler;
class Quantum;
/*!
 *  Timer - an event-driven execution context based on a specified time
 *  interval.  This inherits from a LinkListNode for ease of management by
 *  a global TimerList object.
 */
class Timer : public LinkListNode
{
public:
    /*!
     *  \brief Timer
     *
     *  Default Constructor - zero-initializes all internal data.
     */
    Timer() { }

    /*!
     *  \brief Init
     *
     * Re-initialize the Timer to default values.
     */
    void Init() { ClearNode(); m_u32Interval = 0; m_u32TimerTolerance = 0; m_u32TimeLeft = 0; m_u8Flags = 0; }

    /*!
     *  \brief Start
     *
     *  Start a timer using default ownership, using repeats as an option, and
     *  millisecond resolution.
     *
     *  \param bRepeat_ 0 - timer is one-shot.  1 - timer is repeating.
     *  \param u32IntervalMs_ - Interval of the timer in miliseconds
     *  \param pfCallback_ - Function to call on timer expiry
     *  \param pvData_ - Data to pass into the callback function
     */
    void Start( bool bRepeat_, uint32_t u32IntervalMs_, TimerCallback_t pfCallback_, void *pvData_ );

    /*!
     *  \brief Start
     *
     *  Start a timer using default ownership, using repeats as an option, and
     *  millisecond resolution.
     *
     *  \param bRepeat_ 0 - timer is one-shot.  1 - timer is repeating.
     *  \param u32IntervalMs_ - Interval of the timer in miliseconds
     *  \param u32ToleranceMs_ - Allow the timer expiry to be delayed by an additional maximum time, in
     *                         order to have as many timers expire at the same time as possible.
     *  \param pfCallback_ - Function to call on timer expiry
     *  \param pvData_ - Data to pass into the callback function
     */
    void Start( bool bRepeat_, uint32_t u32IntervalMs_, uint32_t u32ToleranceMs_, TimerCallback_t pfCallback_, void *pvData_ );

    /*!
     *  \brief Stop
     *
     *  Stop a timer already in progress.   Has no effect on timers that have
     *  already been stopped.
     */
    void Stop();

    /*!
     *  \brief SetFlags
     *
     * Set the timer's flags based on the bits in the u8Flags_ argument
     *
     * \param u8Flags_ Flags to assign to the timer object.
     *             TIMERLIST_FLAG_ONE_SHOT for a one-shot timer,
     *             0 for a continuous timer.
     */
    void SetFlags (uint8_t u8Flags_) { m_u8Flags = u8Flags_; }

    /*!
     *  \brief SetCallback
     *
     *  Define the callback function to be executed on expiry of the timer
     *
     *  \param pfCallback_ Pointer to the callback function to call
     */
    void SetCallback( TimerCallback_t pfCallback_){ m_pfCallback = pfCallback_; }

    /*!
     *  \brief SetData
     *
     *  Define a pointer to be sent to the timer callbcak on timer expiry
     *
     *  \param pvData_ Pointer to data to pass as argument into the callback
     */
    void SetData( void *pvData_ ){ m_pvData = pvData_; }

    /*!
     *  \brief SetOwner
     *
     *  Set the owner-thread of this timer object (all timers must be owned by
     *  a thread).
     *
     *  \param pclOwner_ Owner thread of this timer object
     */
    void SetOwner( Thread *pclOwner_){ m_pclOwner = pclOwner_; }

    /*!
     *  \brief SetIntervalTicks
     *
     *  Set the timer expiry in system-ticks (platform specific!)
     *
     *  \param u32Ticks_ Time in ticks
     */
    void SetIntervalTicks(uint32_t u32Ticks_);

    /*!
     *  \brief SetIntervalSeconds
     *
     *  Set the timer expiry interval in seconds (platform agnostic)
     *
     *  \param u32Seconds_ Time in seconds
     */
    void SetIntervalSeconds(uint32_t u32Seconds_);

    /*!
     * \brief GetInterval
     *
     * \return
     */
    uint32_t GetInterval()	{ return m_u32Interval; }

    /*!
     *  \brief SetIntervalMSeconds
     *
     *  Set the timer expiry interval in milliseconds (platform agnostic)
     *
     *  \param u32MSeconds_ Time in milliseconds
     */
    void SetIntervalMSeconds(uint32_t u32MSeconds_);

    /*!
     *  \brief SetIntervalUSeconds
     *
     *  Set the timer expiry interval in microseconds (platform agnostic)
     *
     *  \param u32USeconds_ Time in microseconds
     */
    void SetIntervalUSeconds(uint32_t u32USeconds_);

    /*!
     *  \brief SetTolerance
     *
     *  Set the timer's maximum tolerance in order to synchronize timer
     *  processing with other timers in the system.
     *
     *  \param u32Ticks_ Maximum tolerance in ticks
     */
    void SetTolerance(uint32_t u32Ticks_);

private:

    friend class TimerList;

    //! Flags for the timer, defining if the timer is one-shot or repeated
    uint8_t m_u8Flags;

    //! Pointer to the callback function
    TimerCallback_t m_pfCallback;

    //! Interval of the timer in timer ticks
    uint32_t m_u32Interval;

    //! Time remaining on the timer
    uint32_t m_u32TimeLeft;

    //! Maximum tolerance (usedd for timer harmonization)
    uint32_t m_u32TimerTolerance;

    //! Pointer to the owner thread
    Thread  *m_pclOwner;

    //! Pointer to the callback data
    void    *m_pvData;
};

#endif // KERNEL_USE_TIMERS

#endif
