/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012-2016 Funkenstein Software Consulting, all rights reserved.
See license.txt for more information
=========================================================================== */
/*!

    \file   scheduler.h    

    \brief  Thread scheduler function declarations
    
    This scheduler implements a very flexible type of scheduling, which has
    become the defacto industry standard when it comes to real-time operating
    systems.  This scheduling mechanism is referred to as priority round-
    robin.
    
    From the name, there are two concepts involved here:
        
    1)  Priority scheduling:
    
    Threads are each assigned a priority, and the thread with the highest 
    priority which is ready to run gets to execute.  

    2)  Round-robin scheduling:
    
    Where there are multiple ready threads at the highest-priority level,
    each thread in that group gets to share time, ensuring that progress is
    made.
    
    The scheduler uses an array of ThreadList objects to provide the necessary
    housekeeping required to keep track of threads at the various priorities.
    As s result, the scheduler contains one ThreadList per priority, with an
    additional list to manage the storage of threads which are in the 
    "stopped" state (either have been stopped, or have not been started yet).
   
 */

#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#include "kerneltypes.h"
#include "thread.h"
#include "threadport.h"
#include "priomap.h"

extern volatile Thread *g_pclNext;
extern Thread *g_pclCurrent;

//---------------------------------------------------------------------------
/*!
 *  Priority-based round-robin Thread scheduling, using ThreadLists for
 *  housekeeping.
 */
class Scheduler
{
public:
    /*!
     *  \brief Init
     *
     *  Intiailize the scheduler, must be called before use.
     */
    static void Init();
    
    /*!
     *  \brief Schedule
     *
     *  Run the scheduler, determines the next thread to run based on the 
     *  current state of the threads.  Note that the next-thread chosen
     *  from this function is only valid while in a critical section.
     */
    static void Schedule();
    
    /*!
     *  \brief Add
     *
     *  Add a thread to the scheduler at its current priority level.
     *  
     *  \param pclThread_ Pointer to the thread to add to the scheduler
     */
    static void Add(Thread *pclThread_);
    
    /*!
     *  \brief Remove
     *
     *  Remove a thread from the scheduler at its current priority level.
     *  
     *  \param pclThread_ Pointer to the thread to be removed from the
     *         scheduler
     */
    static void Remove(Thread *pclThread_);
    
    /*!
     *  \brief SetScheduler
     *
     *  Set the active state of the scheduler.  When the scheduler is 
     *  disabled, the *next thread* is never set; the currently
     *  running thread will run forever until the scheduler is enabled
     *  again.  Care must be taken to ensure that we don't end up 
     *  trying to block while the scheduler is disabled, otherwise the
     *  system ends up in an unusable state.
     *  
     *  \param bEnable_ true to enable, false to disable the scheduler 
     */
    static bool SetScheduler(bool bEnable_);

    /*!
     *  \brief GetCurrentThread
     *
     *  Return the pointer to the currently-running thread.
     *  
     *  \return Pointer to the currently-running thread
     */
    static Thread *GetCurrentThread(){ return g_pclCurrent; }
    
    /*!
     *  \brief GetNextThread
     *
     *  Return the pointer to the thread that should run next, according
     *  to the last run of the scheduler.
     *  
     *  \return Pointer to the next-running thread
     */
    static volatile Thread *GetNextThread(){ return g_pclNext; }
    
    /*!
     *  \brief GetThreadList
     *
     *  Return the pointer to the active list of threads that are at the 
     *  given priority level in the scheduler.
     *  
     *  \param uXPriority_ Priority level of the threadlist
     *  
     *  \return Pointer to the ThreadList for the given priority level
     */
    static ThreadList *GetThreadList(PRIO_TYPE uXPriority_){ return &m_aclPriorities[uXPriority_]; }
    
    /*!
     *  \brief GetStopList
     *
     *  Return the pointer to the list of threads that are in the 
     *  scheduler's stopped state.
     *  
     *  \return Pointer to the ThreadList containing the stopped threads
     */
    static ThreadList *GetStopList(){ return &m_clStopList; }
    
    /*!
     *  \brief IsEnabled
     *
     *  Return the current state of the scheduler - whether or not scheudling
     *  is enabled or disabled.
     *  
     *  \return true - scheduler enabled, false - disabled
     */
    static bool IsEnabled(){ return m_bEnabled; }

    /*!
     *  \brief QueueScheduler
     *
     *  Tell the kernel to perform a scheduling operation as soon as the
     *  scheduler is re-enabled.
     */
    static void QueueScheduler() { m_bQueuedSchedule = true; }

private:
    //! Scheduler's state - enabled or disabled
    static bool m_bEnabled;

    //! Variable representing whether or not there's a queued scheduler operation
    static bool m_bQueuedSchedule;

    //! ThreadList for all stopped threads
    static ThreadList m_clStopList;
    
    //! ThreadLists for all threads at all priorities
    static ThreadList m_aclPriorities[KERNEL_NUM_PRIORITIES];
    
    //! Priority bitmap lookup structure, 1-bit per thread priority.
    static PriorityMap m_clPrioMap;
};
#endif

