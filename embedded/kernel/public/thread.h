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

    \file   thread.h    

    \brief  Platform independent thread class declarations
    
    Threads are an atomic unit of execution, and each instance of the thread
    class represents an instance of a program running of the processor.
    The Thread is the fundmanetal user-facing object in the kernel - it
    is what makes multiprocessing possible from application code.
    
    In Mark3, threads each have their own context - consisting of a stack, and
    all of the registers required to multiplex a processor between multiple 
    threads.
    
    The Thread class inherits directly from the LinkListNode class to 
    facilitate efficient thread management using Double, or Double-Circular
    linked lists.
    
*/

#ifndef __THREAD_H__
#define __THREAD_H__

#include "kerneltypes.h"
#include "mark3cfg.h"

#include "ll.h"
#include "threadlist.h"
#include "scheduler.h"
#include "threadport.h"
#include "quantum.h"

//---------------------------------------------------------------------------
/*!
    Function pointer type used for thread entrypoint functions
*/
typedef void (*ThreadEntry_t)(void *pvArg_);

//---------------------------------------------------------------------------
/*!
    Enumeration representing the different states a thread can exist in
*/
typedef enum
{
    THREAD_STATE_EXIT = 0,
    THREAD_STATE_READY,
    THREAD_STATE_BLOCKED,
    THREAD_STATE_STOP,
//--
    THREAD_STATES
} ThreadState_t;

//---------------------------------------------------------------------------
/*!
    Object providing fundamental multitasking support in the kernel.
*/
class Thread : public LinkListNode
{
public:    
    /*!
        \fn void Init(uint8_t *paucStack_, 
              uint16_t u16StackSize_,
              uint8_t u8Priority_,              
              ThreadEntry_t pfEntryPoint_,
              void *pvArg_ );
              
        Initialize a thread prior to its use.  Initialized threads are
        placed in the stopped state, and are not scheduled until the 
        thread's start method has been invoked first.
        
        \param paucStack_    Pointer to the stack to use for the thread
        \param u16StackSize_  Size of the stack (in bytes)
        \param u8Priority_   Priority of the thread (0 = idle, 7 = max)
        \param pfEntryPoint_ This is the function that gets called when the
                             thread is started
        \param pvArg_        Pointer to the argument passed into the thread's
                             entrypoint function.
    */
    void Init(K_WORD *paucStack_, 
              uint16_t u16StackSize_,
              uint8_t u8Priority_,
              ThreadEntry_t pfEntryPoint_,
              void *pvArg_ );

    /*!
        \fn void Start()
       
        Start the thread - remove it from the stopped list, add it to the
        scheduler's list of threads (at the thread's set priority), and
        continue along.       
    */              
    void Start();    
    

    /*!
        \fn void Stop()

        Stop a thread that's actively scheduled without destroying its
        stacks.  Stopped threads can be restarted using the Start() API.
    */
    void Stop();

#if KERNEL_USE_THREADNAME    
    /*!
        \fn void SetName(const char *szName_)
        
        Set the name of the thread - this is purely optional, but can be 
        useful when identifying issues that come along when multiple threads
        are at play in a system.
        
        \param szName_ Char string containing the thread name
    */
    void SetName(const char *szName_) { m_szName = szName_; }

    /*!
        \fn const char* GetName()

        \return Pointer to the name of the thread.  If this is not set, 
                will be NULL.
    */
    const char* GetName() { return m_szName; }
#endif
    
    /*!
        \fn ThreadList *GetOwner(void)
        
        Return the ThreadList where the thread belongs when it's in the 
        active/ready state in the scheduler.
        
        \return Pointer to the Thread's owner list
    */
    ThreadList *GetOwner(void) { return m_pclOwner; }
    
    /*!        
        \fn ThreadList *GetCurrent(void)
        
        Return the ThreadList where the thread is currently located
        
        \return Pointer to the thread's current list        
    */
    ThreadList *GetCurrent(void) { return m_pclCurrent; }
    
    /*!
        \fn uint8_t GetPriority(void)
        
        Return the priority of the current thread
        
        \return Priority of the current thread
    */

    uint8_t GetPriority(void) { return m_u8Priority; }

    /*!
        \fn uint8_t GetCurPriority(void)
        
        Return the priority of the current thread
        
        \return Priority of the current thread
    */
    uint8_t GetCurPriority(void) { return m_u8CurPriority; }
    
#if KERNEL_USE_QUANTUM    
    /*!
        \fn void SetQuantum( uint16_t u16Quantum_ )
        
        Set the thread's round-robin execution quantum.
        
        \param u16Quantum_ Thread's execution quantum (in milliseconds)
    */
    void SetQuantum( uint16_t u16Quantum_ ) { m_u16Quantum = u16Quantum_; }

    /*!
        \fn uint16_t GetQuantum(void)
        
        Get the thread's round-robin execution quantum.
        
        \return The thread's quantum
    */
    uint16_t GetQuantum(void) { return m_u16Quantum; }
#endif

    /*!
        \fn void SetCurrent( ThreadList *pclNewList_ )
        
        Set the thread's current to the specified thread list
        
        \param pclNewList_ Pointer to the threadlist to apply thread ownership       
    */
    void SetCurrent( ThreadList *pclNewList_ ) { m_pclCurrent = pclNewList_; }
    
    /*!
        \fn void SetOwner( ThreadList *pclNewList_ )
        
        Set the thread's owner to the specified thread list
        
        \param pclNewList_ Pointer to the threadlist to apply thread ownership       
    */
    void SetOwner( ThreadList *pclNewList_ ) { m_pclOwner = pclNewList_; }
    
    
    /*!
        \fn void SetPriority( uint8_t u8Priority_ )
        
        Set the priority of the Thread (running or otherwise) to a different
        level.  This activity involves re-scheduling, and must be done so 
        with due caution, as it may effect the determinism of the system.
        
        This should *always* be called from within a critical section to
        prevent system issues.
        
        \param u8Priority_ New priority of the thread
    */
    void SetPriority(uint8_t u8Priority_);
    
    /*!
        \fn void InheritPriority(uint8_t u8Priority_)
        
        Allow the thread to run at a different priority level (temporarily)
        for the purpose of avoiding priority inversions.  This should
        only be called from within the implementation of blocking-objects.
        
        \param u8Priority_  New Priority to boost to.        
    */
    void InheritPriority(uint8_t u8Priority_); 
    
#if KERNEL_USE_DYNAMIC_THREADS    
    /*!
        \fn void Exit()
        
        Remove the thread from being scheduled again.  The thread is 
        effectively destroyed when this occurs.  This is extremely 
        useful for cases where a thread encounters an unrecoverable
        error and needs to be restarted, or in the context of systems
        where threads need to be created and destroyed dynamically.
        
        This must not be called on the idle thread.
    */    
    void Exit();
#endif    

#if KERNEL_USE_SLEEP    
    /*!
        \fn void Sleep(uint32_t u32TimeMs_);
        
        Put the thread to sleep for the specified time (in milliseconds).
        Actual time slept may be longer (but not less than) the interval specified.
        
        \param u32TimeMs_ Time to sleep (in ms)
    */
    static void Sleep(uint32_t u32TimeMs_);

    /*!
        \fn void USleep(uint32_t u32TimeUs_);

        Put the thread to sleep for the specified time (in microseconds).
        Actual time slept may be longer (but not less than) the interval specified.

        \param u32TimeUs_ Time to sleep (in microseconds)
    */
    static void USleep(uint32_t u32TimeUs_);
#endif
    
    /*!
        \fn void Yield(void)
        
        Yield the thread - this forces the system to call the scheduler and 
        determine what thread should run next.  This is typically used when
        threads are moved in and out of the scheduler.
    */
    static void Yield(void);
    
    /*!
        \fn void SetID( uint8_t u8ID_ )
        
        Set an 8-bit ID to uniquely identify this thread.
        
        \param u8ID_ 8-bit Thread ID, set by the user
    */
    void SetID( uint8_t u8ID_ ) { m_u8ThreadID = u8ID_; }
    
    /*!
        \fn uint8_t GetID()
        
        Return the 8-bit ID corresponding to this thread.
        
        \return Thread's 8-bit ID, set by the user
    */
    uint8_t GetID() { return m_u8ThreadID; }
    
    
    /*!
        \fn uint16_t GetStackSlack()
        
        Performs a (somewhat lengthy) check on the thread stack to check the
        amount of stack margin (or "slack") remaining on the stack. If you're
        having problems with blowing your stack, you can run this function 
        at points in your code during development to see what operations 
        cause problems.  Also useful during development as a tool to optimally 
        size thread stacks.
        
        \return The amount of slack (unused bytes) on the stack
    */
    uint16_t GetStackSlack();
    
#if KERNEL_USE_EVENTFLAG
    /*!
        \brief GetEventFlagMask returns the thread's current event-flag mask,
              which is used in conjunction with the EventFlag blocking object
              type.

        \return A copy of the thread's event flag mask
     */
    uint16_t GetEventFlagMask() { return m_u16FlagMask; }

    /*!
        \brief SetEventFlagMask Sets the active event flag bitfield mask
        \param u16Mask_
     */
    void SetEventFlagMask(uint16_t u16Mask_) { m_u16FlagMask = u16Mask_; }

    /*!
     * \brief SetEventFlagMode Sets the active event flag operation mode
     * \param eMode_ Event flag operation mode, defines the logical operator
     *               to apply to the event flag.
     */
    void SetEventFlagMode(EventFlagOperation_t eMode_ ) { m_eFlagMode = eMode_; }

    /*!
     * \brief GetEventFlagMode Returns the thread's event flag's operating mode
     * \return The thread's event flag mode.
     */
    EventFlagOperation_t GetEventFlagMode() { return m_eFlagMode; }
#endif

#if KERNEL_USE_TIMEOUTS || KERNEL_USE_SLEEP
	/*!
		Return a pointer to the thread's timer object
	*/
	Timer *GetTimer();
#endif
#if KERNEL_USE_TIMEOUTS

    /*!
     * \brief SetExpired
     *
     * Set the status of the current blocking call on the thread.
     *
     * \param bExpired_ true - call expired, false - call did not expire
     */
	void SetExpired( bool bExpired_ );

    /*!
     * \brief GetExpired
     *
     * Return the status of the most-recent blocking call on the thread.
     * \return true - call expired, false - call did not expire
     */
	bool GetExpired();
#endif

#if KERNEL_USE_IDLE_FUNC
    /*!
     * \brief InitIdle Initialize this Thread object as the Kernel's idle
     *        thread.  There should only be one of these, maximum, in a
     *        given system.
     */
    void InitIdle();
#endif

    /*!
     * \brief GetState Returns the current state of the thread to the
     *        caller.  Can be used to determine whether or not a thread
     *        is ready (or running), stopped, or terminated/exit'd.
     * \return ThreadState_t representing the thread's current state
     */
    ThreadState_t GetState()                { return m_eState; }

    /*!
     * \brief SetState Set the thread's state to a new value.  This
     *        is only to be used by code within the kernel, and is not
     *        indended for use by an end-user.
     *
     * \param eState_ New thread state to set.
     */
    void SetState( ThreadState_t eState_ )  { m_eState = eState_; }

    friend class ThreadPort;
    
private:
    /*!
        \fn void ContextSwitchSWI()
       
        This code is used to trigger the context switch interrupt.  Called 
        whenever the kernel decides that it is necessary to swap out the 
        current thread for the "next" thread.       
    */
    static void ContextSwitchSWI(void);

    /*!
        \fn void SetPriorityBase(uint8_t u8Priority_)
        \param u8Priority_
     */
    void SetPriorityBase(uint8_t u8Priority_);

    //! Pointer to the top of the thread's stack
    K_WORD *m_pwStackTop;
    
    //! Pointer to the thread's stack
    K_WORD *m_pwStack;

    //! Thread ID
    uint8_t m_u8ThreadID;
    
    //! Default priority of the thread
    uint8_t m_u8Priority;     
    
    //! Current priority of the thread (priority inheritence)
    uint8_t m_u8CurPriority;  

    //! Enum indicating the thread's current state
    ThreadState_t m_eState;

#if KERNEL_USE_THREADNAME
    //! Thread name
    const char *m_szName;
#endif

    //! Size of the stack (in bytes)
    uint16_t m_u16StackSize;

    //! Pointer to the thread-list where the thread currently resides
    ThreadList *m_pclCurrent;

    //! Pointer to the thread-list where the thread resides when active
    ThreadList *m_pclOwner;

    //! The entry-point function called when the thread starts
    ThreadEntry_t m_pfEntryPoint;

    //! Pointer to the argument passed into the thread's entrypoint
    void *m_pvArg;

#if KERNEL_USE_QUANTUM
    //! Thread quantum (in milliseconds)
    uint16_t m_u16Quantum;
#endif

#if KERNEL_USE_EVENTFLAG
    //! Event-flag mask
    uint16_t m_u16FlagMask;

    //! Event-flag mode
    EventFlagOperation_t m_eFlagMode;
#endif

#if KERNEL_USE_TIMEOUTS || KERNEL_USE_SLEEP
	//! Timer used for blocking-object timeouts
	Timer	m_clTimer;
#endif
#if KERNEL_USE_TIMEOUTS
    //! Indicate whether or not a blocking-object timeout has occurred
	bool	m_bExpired;
#endif
    
};

#if KERNEL_USE_IDLE_FUNC
//---------------------------------------------------------------------------
/*!
    If the kernel is set up to use an idle function instead of an idle thread,
    we use a placeholder data structure to "simulate" the effect of having an
    idle thread in the system.  When cast to a Thread, this data structure will
    still result in GetPriority() calls being valid, which is all that is
    needed to support the tick-based/tickless times -- while saving a fairly
    decent chunk of RAM on a small micro.

    Note that this struct must have the same memory layout as the Thread class
    up to the last item.
*/
typedef struct
{
    LinkListNode *next;
    LinkListNode *prev;

    //! Pointer to the top of the thread's stack
    K_WORD *m_pwStackTop;

    //! Pointer to the thread's stack
    K_WORD *m_pwStack;

    //! Thread ID
    uint8_t m_u8ThreadID;

    //! Default priority of the thread
    uint8_t m_u8Priority;

    //! Current priority of the thread (priority inheritence)
    uint8_t m_u8CurPriority;

    //! Enum indicating the thread's current state
    ThreadState_t m_eState;

#if KERNEL_USE_THREADNAME
    //! Thread name
    const char *m_szName;
#endif

} FakeThread_t;
#endif

#endif
