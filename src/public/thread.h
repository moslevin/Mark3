/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012 - 2018 m0slevin, all rights reserved.
See license.txt for more information
=========================================================================== */
/**

    @file   thread.h

    @brief  Platform independent thread class declarations

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

#pragma once

#include "kerneltypes.h"
#include "mark3cfg.h"

#include "ll.h"
#include "threadlist.h"
#include "scheduler.h"
#include "ithreadport.h"
#include "quantum.h"
#include "autoalloc.h"
#include "priomap.h"

namespace Mark3
{
class Thread;

//---------------------------------------------------------------------------
using ThreadCreateCallout  = void (*)(Thread* pclThread_);
using ThreadExitCallout    = void (*)(Thread* pclThread_);
using ThreadContextCallout = void (*)(Thread* pclThread_);

//---------------------------------------------------------------------------
/**
 *  Object providing fundamental multitasking support in the kernel.
 */
class Thread : public TypedLinkListNode<Thread>
{
public:
    void* operator new(size_t sz, void* pv) { return reinterpret_cast<Thread*>(pv); };
    ~Thread();

    Thread() { m_eState = ThreadState::Invalid; }

    bool IsInitialized() { return (m_eState != ThreadState::Invalid); }

    /**
     *  @brief Init
     *
     *  Initialize a thread prior to its use.  Initialized threads are
     *  placed in the stopped state, and are not scheduled until the
     *  thread's start method has been invoked first.
     *
     *  @param pwStack_    Pointer to the stack to use for the thread
     *  @param u16StackSize_  Size of the stack (in bytes)
     *  @param uXPriority_   Priority of the thread (0 = idle, 7 = max)
     *  @param pfEntryPoint_ This is the function that gets called when the
     *                       thread is started
     *  @param pvArg_        Pointer to the argument passed into the thread's
     *                       entrypoint function.
     */
    void Init(K_WORD*         pwStack_,
              uint16_t        u16StackSize_,
              PORT_PRIO_TYPE  uXPriority_,
              ThreadEntryFunc pfEntryPoint_,
              void*           pvArg_);

    /**
     * @brief Init
     *
     * Create and initialize a new thread, using memory from the auto-allocated
     * heap region to supply both the thread object and its stack.  The thread
     * returned can then be started using the Start() method directly.  Note that
     * the memory used to create this thread cannot be reclaimed, and so this API
     * is only suitable for threads that exist for the duration of runtime.
     *
     * @param u16StackSize_  Size of the stack (in bytes)
     * @param uXPriority_    Priority of the thread (0 = idle, 7 = max)
     * @param pfEntryPoint_  This is the function that gets called when the
     *                       thread is started
     * @param pvArg_         Pointer to the argument passed into the thread's
     *                      entrypoint function.
     *
     * @return Pointer to a newly-created thread.
     */
    static Thread*
    Init(uint16_t u16StackSize_, PORT_PRIO_TYPE uXPriority_, ThreadEntryFunc pfEntryPoint_, void* pvArg_);

    /**
     *  @brief Start
     *
     *  Start the thread - remove it from the stopped list, add it to the
     *  scheduler's list of threads (at the thread's set priority), and
     *  continue along.
     */
    void Start();

    /**
     *  @brief Stop
     *
     *  Stop a thread that's actively scheduled without destroying its
     *  stacks.  Stopped threads can be restarted using the Start() API.
     */
    void Stop();

#if KERNEL_NAMED_THREADS
    /**
     *  @brief SetName
     *
     *  Set the name of the thread - this is purely optional, but can be
     *  useful when identifying issues that come along when multiple threads
     *  are at play in a system.
     *
     *  @param szName_ Char string containing the thread name
     */
    void SetName(const char* szName_) { m_szName = szName_; }
    /**
     *  @brief GetName
     *
     *  @return Pointer to the name of the thread.  If this is not set,
     *          will be nullptr.
     */
    const char* GetName() { return m_szName; }
#endif // #if KERNEL_NAMED_THREADS

    /**
     *  @brief GetOwner
     *
     *  Return the ThreadList where the thread belongs when it's in the
     *  active/ready state in the scheduler.
     *
     *  @return Pointer to the Thread's owner list
     */
    ThreadList* GetOwner(void) { return m_pclOwner; }
    /**
     *  @brief GetCurrent
     *
     *  Return the ThreadList where the thread is currently located
     *
     *  @return Pointer to the thread's current list
     */
    inline ThreadList* GetCurrent(void) { return m_pclCurrent; }
    /**
     *  @brief GetPriority
     *
     *  Return the priority of the current thread
     *
     *  @return Priority of the current thread
     */

    PORT_PRIO_TYPE GetPriority(void) { return m_uXPriority; }
    /**
     *  @brief GetCurPriority
     *
     *  Return the priority of the current thread
     *
     *  @return Priority of the current thread
     */
    PORT_PRIO_TYPE GetCurPriority(void) { return m_uXCurPriority; }

#if KERNEL_ROUND_ROBIN
    /**
     *  @brief SetQuantum
     *
     *  Set the thread's round-robin execution quantum.
     *
     *  @param u16Quantum_ Thread's execution quantum (in milliseconds)
     */
    void SetQuantum(uint16_t u16Quantum_) { m_u16Quantum = u16Quantum_; }
    /**
     *  @brief GetQuantum
     *
     *  Get the thread's round-robin execution quantum.
     *
     *  @return The thread's quantum
     */
    uint16_t GetQuantum(void) { return m_u16Quantum; }
#endif // #if KERNEL_ROUND_ROBIN

    /**
     *  @brief SetCurrent
     *
     *  Set the thread's current to the specified thread list
     *
     *  @param pclNewList_ Pointer to the threadlist to apply thread ownership
     */
    void SetCurrent(ThreadList* pclNewList_) { m_pclCurrent = pclNewList_; }
    /**
     *  @brief SetOwner
     *
     *  Set the thread's owner to the specified thread list
     *
     *  @param pclNewList_ Pointer to the threadlist to apply thread ownership
     */
    void SetOwner(ThreadList* pclNewList_) { m_pclOwner = pclNewList_; }
    /**
     *  @brief SetPriority
     *
     *  Set the priority of the Thread (running or otherwise) to a different
     *  level.  This activity involves re-scheduling, and must be done so
     *  with due caution, as it may effect the determinism of the system.
     *
     *  This should *always* be called from within a critical section to
     *  prevent system issues.
     *
     *  @param uXPriority_ New priority of the thread
     */
    void SetPriority(PORT_PRIO_TYPE uXPriority_);

    /**
     *  @brief InheritPriority
     *
     *  Allow the thread to run at a different priority level (temporarily)
     *  for the purpose of avoiding priority inversions.  This should
     *  only be called from within the implementation of blocking-objects.
     *
     *  @param uXPriority_  New Priority to boost to.
     */
    void InheritPriority(PORT_PRIO_TYPE uXPriority_);

    /**
     *  @brief Exit
     *
     *  Remove the thread from being scheduled again.  The thread is
     *  effectively destroyed when this occurs.  This is extremely
     *  useful for cases where a thread encounters an unrecoverable
     *  error and needs to be restarted, or in the context of systems
     *  where threads need to be created and destroyed dynamically.
     *
     *  This must not be called on the idle thread.
     */
    void Exit();

    /**
     *  @brief Sleep
     *
     *  Put the thread to sleep for the specified time (in milliseconds).
     *  Actual time slept may be longer (but not less than) the interval specified.
     *
     *  @param u32TimeMs_ Time to sleep (in ms)
     */
    static void Sleep(uint32_t u32TimeMs_);

    /**
     *  @brief Yield
     *
     *  Yield the thread - this forces the system to call the scheduler and
     *  determine what thread should run next.  This is typically used when
     *  threads are moved in and out of the scheduler.
     */
    static void Yield(void);

    /**
     * @brief CoopYield
     *
     * Cooperative yield - This forces the system to not only call the scheduler,
     * but also move the currently executing thread to the back of the current
     * thread list, allowing other same-priority threads the opportunity to run.
     * This is used primarily for cooperative scheduling between threads in the
     * same priority level.
     */
    static void CoopYield(void);

    /**
     *  @brief SetID
     *
     *  Set an 8-bit ID to uniquely identify this thread.
     *
     *  @param u8ID_ 8-bit Thread ID, set by the user
     */
    void SetID(uint8_t u8ID_) { m_u8ThreadID = u8ID_; }
    /**
     *  @brief GetID
     *
     *  Return the 8-bit ID corresponding to this thread.
     *
     *  @return Thread's 8-bit ID, set by the user
     */
    uint8_t GetID() { return m_u8ThreadID; }

#if KERNEL_STACK_CHECK
    /**
     *  @brief GetStackSlack
     *
     *  Performs a (somewhat lengthy) check on the thread stack to check the
     *  amount of stack margin (or "slack") remaining on the stack. If you're
     *  having problems with blowing your stack, you can run this function
     *  at points in your code during development to see what operations
     *  cause problems.  Also useful during development as a tool to optimally
     *  size thread stacks.
     *
     *  @return The amount of slack (unused bytes) on the stack
     */
    uint16_t GetStackSlack();
#endif // #if KERNEL_STACK_CHECK

#if KERNEL_EVENT_FLAGS
    /**
     *  @brief GetEventFlagMask returns the thread's current event-flag mask,
     *        which is used in conjunction with the EventFlag blocking object
     *        type.
     *
     *  @return A copy of the thread's event flag mask
     */
    uint16_t GetEventFlagMask() { return m_u16FlagMask; }

    /**
     *  @brief SetEventFlagMask Sets the active event flag bitfield mask
     *  @param u16Mask_
     */
    void SetEventFlagMask(uint16_t u16Mask_) { m_u16FlagMask = u16Mask_; }

    /**
     * @brief SetEventFlagMode Sets the active event flag operation mode
     * @param eMode_ Event flag operation mode, defines the logical operator
     *               to apply to the event flag.
     */
    void SetEventFlagMode(EventFlagOperation eMode_) { m_eFlagMode = eMode_; }

    /**
     * @brief GetEventFlagMode Returns the thread's event flag's operating mode
     * @return The thread's event flag mode.
     */
    EventFlagOperation GetEventFlagMode() { return m_eFlagMode; }
#endif // #if KERNEL_EVENT_FLAGS

    /**
     *  Return a pointer to the thread's timer object
     */
    Timer* GetTimer();

    /**
     * @brief SetExpired
     *
     * Set the status of the current blocking call on the thread.
     *
     * @param bExpired_ true - call expired, false - call did not expire
     */
    void SetExpired(bool bExpired_);

    /**
     * @brief GetExpired
     *
     * Return the status of the most-recent blocking call on the thread.
     * @return true - call expired, false - call did not expire
     */
    bool GetExpired();

#if KERNEL_EXTENDED_CONTEXT
    /**
     * @brief GetExtendedContext
     *
     * Return the Thread object's extended-context data pointer.  Used by code
     * implementing a user-defined thread-local storage model.  Pointer exists
     * only for the lifespan of the Thread.
     *
     * @return Thread's extended context data pointer.
     */
    void* GetExtendedContext() { return m_pvExtendedContext; }

    /**
     * @brief SetExtendedContext
     *
     * Assign the Thread object's extended-context data pointer.  Used by code
     * implementing a user-defined thread-local storage model.
     *
     * Object assigned to the context pointer should persist for the duration
     * of the Thread.
     *
     * @param pvData_ Object to assign to the extended data pointer.+
     */
    void SetExtendedContext(void* pvData_) { m_pvExtendedContext = pvData_; }
#endif // #if KERNEL_EXTENDED_CONTEXT

    /**
     * @brief GetState Returns the current state of the thread to the
     *        caller.  Can be used to determine whether or not a thread
     *        is ready (or running), stopped, or terminated/exit'd.
     * @return ThreadState_t representing the thread's current state
     */
    ThreadState GetState() { return m_eState; }
    /**
     * @brief SetState Set the thread's state to a new value.  This
     *        is only to be used by code within the kernel, and is not
     *        indended for use by an end-user.
     *
     * @param eState_ New thread state to set.
     */
    void SetState(ThreadState eState_) { m_eState = eState_; }

    /**
     * @brief GetStack
     * @return Pointer to the blob of memory used as the thread's stack
     */
    K_WORD* GetStack() { return m_pwStack; }

    /**
     * @brief GetStackSize
     * @return Size of the thread's stack in bytes
     */
    uint16_t GetStackSize() { return m_u16StackSize; }

    friend class ThreadPort;

private:
    /**
     *  @brief ContextSwitchSWI
     *
     *  This code is used to trigger the context switch interrupt.  Called
     *  whenever the kernel decides that it is necessary to swap out the
     *  current thread for the "next" thread.
     */
    static void ContextSwitchSWI(void);

    /**
     *  @brief SetPriorityBase
     *
     *  @param uXPriority_
     */
    void SetPriorityBase(PORT_PRIO_TYPE uXPriority_);

    //! Pointer to the top of the thread's stack
    K_WORD* m_pwStackTop;

    //! Pointer to the thread's stack
    K_WORD* m_pwStack;

    //! Thread ID
    uint8_t m_u8ThreadID;

    //! Default priority of the thread
    PORT_PRIO_TYPE m_uXPriority;

    //! Current priority of the thread (priority inheritence)
    PORT_PRIO_TYPE m_uXCurPriority;

    //! Enum indicating the thread's current state
    ThreadState m_eState;

#if KERNEL_EXTENDED_CONTEXT
    //! Pointer provided to a Thread to implement thread-local storage
    void* m_pvExtendedContext;
#endif // #if KERNEL_EXTENDED_CONTEXT

#if KERNEL_NAMED_THREADS
    //! Thread name
    const char* m_szName;
#endif // #if KERNEL_NAMED_THREADS

    //! Size of the stack (in bytes)
    uint16_t m_u16StackSize;

    //! Pointer to the thread-list where the thread currently resides
    ThreadList* m_pclCurrent;

    //! Pointer to the thread-list where the thread resides when active
    ThreadList* m_pclOwner;

    //! The entry-point function called when the thread starts
    ThreadEntryFunc m_pfEntryPoint;

    //! Pointer to the argument passed into the thread's entrypoint
    void* m_pvArg;

#if KERNEL_ROUND_ROBIN
    //! Thread quantum (in milliseconds)
    uint16_t m_u16Quantum;
#endif // #if KERNEL_ROUND_ROBIN

#if KERNEL_EVENT_FLAGS
    //! Event-flag mask
    uint16_t m_u16FlagMask;

    //! Event-flag mode
    EventFlagOperation m_eFlagMode;
#endif // #if KERNEL_EVENT_FLAGS

    //! Timer used for blocking-object timeouts
    Timer m_clTimer;

    //! Indicate whether or not a blocking-object timeout has occurred
    bool m_bExpired;
};

} // namespace Mark3
