/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012 - 2019 m0slevin, all rights reserved.
See license.txt for more information
=========================================================================== */
/**

    @file   timer.h

    @brief  Timer object declarations
 */

#pragma once
#include "kerneltypes.h"
#include "mark3cfg.h"

#include "ll.h"

namespace Mark3
{
class Thread;

//---------------------------------------------------------------------------
static constexpr auto uMaxTimerTicks     = uint32_t { 0x7FFFFFFF }; //!< Maximum value to set
static constexpr auto uTimerTicksInvalid = uint32_t { 0 };
static constexpr auto uTimerFlagOneShot  = uint8_t { 0x01 }; //!< Timer is one-shot
static constexpr auto uTimerFlagActive   = uint8_t { 0x02 }; //!< Timer is currently active
static constexpr auto uTimerFlagCallback = uint8_t { 0x04 }; //!< Timer is pending a callback
static constexpr auto uTimerFlagExpired  = uint8_t { 0x08 }; //!< Timer is actually expired.

//---------------------------------------------------------------------------
/**
 * This type defines the callback function type for timer events.  Since these
 * are called from an interrupt context, they do not operate from within a
 * thread or object context directly -- as a result, the context must be
 * manually passed into the calls.
 *
 * pclOwner_ is a pointer to the thread that owns the timer
 * pvData_ is a pointer to some data or object that needs to know about the
 *         timer's expiry from within the timer interrupt context.
 */
using TimerCallback = void (*)(Thread* pclOwner_, void* pvData_);

//---------------------------------------------------------------------------
class TimerList;
class TimerScheduler;
class Quantum;

//---------------------------------------------------------------------------
/**
 * @brief The Timer Class.
 * This class provides kernel-managed timers, used to provide high-precision
 * delays.  Functionality is useful to both user-code, and is used extensively
 * within the kernel and its blocking objects to implement round-robin
 * scheduling, thread sleep, and timeouts.  Provides one-shot and periodic
 * timers for use by application code.  This object relies on a target-defined
 * hardware timer implementation, which is multiplexed by the kernel's timer
 * scheduler.
 */
class Timer : public TypedLinkListNode<Timer>
{
public:
    void* operator new(size_t sz, void* pv) { return reinterpret_cast<Timer*>(pv); }
    ~Timer() {}

    /**
     *  @brief Timer
     *  Default Constructor - Do nothing.  Allow the init call to perform
     *  the necessary object initialization prior to use.
     */
    Timer();

    /**
     *  @brief Init
     * Re-initialize the Timer to default values.
     */
    void Init();

    /**
     *  @brief Start
     *  Start a timer using default ownership, using repeats as an option, and
     *  millisecond resolution.
     *
     *  @param bRepeat_ 0 - timer is one-shot.  1 - timer is repeating.
     *  @param u32IntervalMs_ - Interval of the timer in miliseconds
     *  @param pfCallback_ - Function to call on timer expiry
     *  @param pvData_ - Data to pass into the callback function
     */
    void Start(bool bRepeat_, uint32_t u32IntervalMs_, TimerCallback pfCallback_, void* pvData_);

    /**
     * @brief Start
     * Start or restart a timer using parameters previously configured via
     * calls to Start(<with args>), or via the a-la-carte parameter setter
     * methods.  This is especially useful for retriggering one-shot timers
     * that have previously expired, using the timer's previous configuration.
     */
    void Start();

    /**
     *  @brief Stop
     *  Stop a timer already in progress.   Has no effect on timers that have
     *  already been stopped.
     */
    void Stop();

private:
    friend class TimerList;

    /**
     * @brief SetInitialized
     */
    void SetInitialized() { m_u8Initialized = m_uTimerInitCookie; }

    /**
     * @brief IsInitialized
     * @return true if initialized, false if not initialized.
     */
    bool IsInitialized(void) { return (m_u8Initialized == m_uTimerInitCookie); }

    static inline uint32_t SecondsToTicks(uint32_t x) { return (x) * 1000; }
    static inline uint32_t MSecondsToTicks(uint32_t x) { return (x); }
    static inline uint32_t USecondsToTicks(uint32_t x) { return ((x + 999) / 1000); }

    static constexpr auto m_uTimerInvalidCookie = uint8_t { 0x3C };
    static constexpr auto m_uTimerInitCookie    = uint8_t { 0xC3 };

    //! Cookie used to determine whether or not the timer is initialized
    uint8_t m_u8Initialized;

    //! Flags for the timer, defining if the timer is one-shot or repeated
    uint8_t m_u8Flags;

    //! Pointer to the callback function
    TimerCallback m_pfCallback;

    //! Interval of the timer in timer ticks
    uint32_t m_u32Interval;

    //! Time remaining on the timer
    uint32_t m_u32TimeLeft;

    //! Pointer to the owner thread
    Thread* m_pclOwner;

    //! Pointer to the callback data
    void* m_pvData;
};
} // namespace Mark3
