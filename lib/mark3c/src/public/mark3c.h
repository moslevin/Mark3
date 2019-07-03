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
===========================================================================*/
/**

    @file   mark3c.h

    @brief  Header providing C-language API bindings for the Mark3 kernel.
*/

#pragma once

#include "mark3cfg.h"
#include "fake_types.h"

#include <stdint.h>
#include <stdbool.h>

#if defined(__cplusplus)
extern "C" {
#endif

//---------------------------------------------------------------------------
// Define a series of handle types to be used in place of the underlying classes
// of Mark3.
#if KERNEL_EVENT_FLAGS
typedef void* EventFlag_t;         //!< EventFlag opaque handle data type
#endif                             // #if KERNEL_EVENT_FLAGS
typedef void* Mailbox_t;           //!< Mailbox opaque handle data type
typedef void* Message_t;           //!< Message opaque handle data type
typedef void* MessagePool_t;       //!< MessagePool opaque handle data type
typedef void* MessageQueue_t;      //!< MessageQueue opaque handle data type
typedef void* Mutex_t;             //!< Mutex opaque handle data type
typedef void* Notify_t;            //!< Notification object opaque handle data type
typedef void* Semaphore_t;         //!< Semaphore opaque handle data type
typedef void* Thread_t;            //!< Thread opaque handle data type
typedef void* Timer_t;             //!< Timer opaque handle data type
typedef void* ConditionVariable_t; //!< Condition Variable opaque handle data type
typedef void* ReaderWriterLock_t;  //!< Reader-writer-lock opaque handle data type
typedef void* Coroutine_t;         //!< Coroutine opaaque handle data type

//---------------------------------------------------------------------------
// Function pointer types used by Kernel APIs
#if KERNEL_THREAD_CREATE_CALLOUT
typedef void (*thread_create_callout_t)(Thread_t hThread_);
#endif // #if KERNEL_THREAD_CREATE_CALLOUT
#if KERNEL_THREAD_EXIT_CALLOUT
typedef void (*thread_exit_callout_t)(Thread_t hThread_);
#endif // #if KERNEL_THREAD_EXIT_CALLOUT
#if KERNEL_CONTEXT_SWITCH_CALLOUT
typedef void (*thread_context_callout_t)(Thread_t hThread_);
#endif // #if KERNEL_CONTEXT_SWITCH_CALLOUT
typedef void (*kernel_debug_print_t)(const char* szString_);
typedef void (*coroutine_callback_t)(Coroutine_t caller, void* pvContext);

//---------------------------------------------------------------------------
// Use the sizes of the structs in fake_types.h to generate opaque object-blobs
// that get instantiated as kernel objects (from the C++ code) later.
#define THREAD_SIZE (sizeof(Fake_Thread))
#define TIMER_SIZE (sizeof(Fake_Timer))
#define SEMAPHORE_SIZE (sizeof(Fake_Semaphore))
#define MUTEX_SIZE (sizeof(Fake_Mutex))
#define MESSAGE_SIZE (sizeof(Fake_Message))
#define MESSAGEQUEUE_SIZE (sizeof(Fake_MessageQueue))
#define MAILBOX_SIZE (sizeof(Fake_Mailbox))
#define NOTIFY_SIZE (sizeof(Fake_Notify))
#if KERNEL_EVENT_FLAGS
#define EVENTFLAG_SIZE (sizeof(Fake_EventFlag))
#endif // #if KERNEL_EVENT_FLAGS
#define MESSAGEPOOL_SIZE (sizeof(Fake_MessagePool))
#define CONDITIONVARIABLE_SIZE (sizeof(Fake_ConditionVariable))
#define READERWRITERLOCK_SIZE (sizeof(Fake_ReaderWriterLock))
#define COROUTINE_SIZE (sizeof(Fake_Coroutine))
#if KERNEL_EVENT_FLAGS
//---------------------------------------------------------------------------
/**
 * Define the various types of modifications that can be performed to
 * an event flag object.
 */
typedef enum {
    EVENT_FLAG_ALL_SET,        //!< Block until all bits in the specified bitmask are set
    EVENT_FLAG_ANY_SET,        //!< Block until any bits in the specified bitmask are set
    EVENT_FLAG_ALL_CLEAR,      //!< Block until all bits in the specified bitmask are cleared
    EVENT_FlAG_ANY_CLEAR,      //!< Block until any bits in the specified bitmask are cleared
    EVENT_FLAG_PENDING_UNBLOCK //!< Special code.  Not used by user
} event_flag_operation_t;
#endif // #if KERNEL_EVENT_FLAGS

//---------------------------------------------------------------------------
/**
 * Define the various states that a thread can be in
 */
typedef enum {
    THREAD_STATE_EXIT = 0, ///!< Thread has terminated via exit path
    THREAD_STATE_READY,    ///!< Thread is ready to run
    THREAD_STATE_BLOCKED,  ///!< Thread is blocked on a blocking call
    THREAD_STATE_STOP,     ///!< Thread has been manually stopped
    THREAD_STATE_INVALID   ///!< Invalid thread state
} thread_state_t;

//---------------------------------------------------------------------------
// Macros for declaring opaque buffers of an appropriate size for the given
// kernel objects
#define TOKEN_1(x, y) x##y
#define TOKEN_2(x, y) TOKEN_1(x, y)

// Ensure that opaque buffers are sized to the nearest word - which is
// a platform-dependent value.
#define WORD_ROUND(x) (((x) + (sizeof(K_WORD) - 1)) / sizeof(K_WORD))

#define DECLARE_THREAD(name)                                                                                           \
    K_WORD   TOKEN_2(__thread_, name)[WORD_ROUND(THREAD_SIZE)];                                                        \
    Thread_t name = (Thread_t)TOKEN_2(__thread_, name);

#define DECLARE_TIMER(name)                                                                                            \
    K_WORD  TOKEN_2(__timer_, name)[WORD_ROUND(TIMER_SIZE)];                                                           \
    Timer_t name = (Timer_t)TOKEN_2(__timer_, name);

#define DECLARE_SEMAPHORE(name)                                                                                        \
    K_WORD      TOKEN_2(__semaphore_, name)[WORD_ROUND(SEMAPHORE_SIZE)];                                               \
    Semaphore_t name = (Semaphore_t)TOKEN_2(__semaphore_, name);

#define DECLARE_MUTEX(name)                                                                                            \
    K_WORD  TOKEN_2(__mutex_, name)[WORD_ROUND(MUTEX_SIZE)];                                                           \
    Mutex_t name = (Mutex_t)TOKEN_2(__mutex_, name);

#define DECLARE_MESSAGE(name)                                                                                          \
    K_WORD    TOKEN_2(__message_, name)[WORD_ROUND(MESSAGE_SIZE)];                                                     \
    Message_t name = (Message_t)TOKEN_2(__message_, name);

#define DECLARE_MESSAGEPOOL(name)                                                                                      \
    K_WORD        TOKEN_2(__messagepool_, name)[WORD_ROUND(MESSAGEPOOL_SIZE)];                                         \
    MessagePool_t name = (MessagePool_t)TOKEN_2(__messagepool_, name);

#define DECLARE_MESSAGEQUEUE(name)                                                                                     \
    K_WORD         TOKEN_2(__messagequeue_, name)[WORD_ROUND(MESSAGEQUEUE_SIZE)];                                      \
    MessageQueue_t name = (MessageQueue_t)TOKEN_2(__messagequeue_, name);

#define DECLARE_MAILBOX(name)                                                                                          \
    K_WORD    TOKEN_2(__mailbox_, name)[WORD_ROUND(MAILBOX_SIZE)];                                                     \
    Mailbox_t name = (Mailbox_t)TOKEN_2(__mailbox_, name);

#define DECLARE_NOTIFY(name)                                                                                           \
    K_WORD   TOKEN_2(__notify_, name)[WORD_ROUND(NOTIFY_SIZE)];                                                        \
    Notify_t name = (Notify_t)TOKEN_2(__notify_, name);

#if KERNEL_EVENT_FLAGS
#define DECLARE_EVENTFLAG(name)                                                                                        \
    K_WORD      TOKEN_2(__eventflag_, name)[WORD_ROUND(EVENTFLAG_SIZE)];                                               \
    EventFlag_t name = (EventFlag_t)TOKEN_2(__eventflag_, name);
#endif // #if KERNEL_EVENT_FLAGS

#define DECLARE_CONDITIONVARIABLE(name)                                                                                \
    K_WORD              TOKEN_2(__condvar_, name)[WORD_ROUND(EVENTFLAG_SIZE)];                                         \
    ConditionVariable_t name = (ConditionVariable_t)TOKEN_2(__condvar_, name);

#define DECLARE_READERWRITERLOCK(name)                                                                                 \
    K_WORD             TOKEN_2(__readerwriterlock_, name)[WORD_ROUND(EVENTFLAG_SIZE)];                                 \
    ReaderWriterLock_t name = (ReaderWriterLock_t)TOKEN_2(__readerwriterlock_, name);

#define DECLARE_COROUTINE(name) \
    K_WORD             TOKEN_2(__coroutine_, name)[WORD_ROUND(COROUTINE_SIZE)]; \
    Coroutine_t name = (Coroutine_t)TOKEN_2(__coroutine_, name);

//---------------------------------------------------------------------------
// Allocate-once Memory managment APIs
/**
 * @brief Alloc_Memory
 * @sa void* AutoAlloc::NewRawData(size_t sSize_)
 * @param eSize_ Size in bytes to allocate from the one-time-allocate heap
 * @return Pointer to an allocated blob of memory, or nullptr if heap exhausted
 */
void* Alloc_Memory(size_t eSize_);

/**
 * @brief Free_Memory
 * @param pvObject_ Pointer to previously allocated block of memory
 */
void Free_Memory(void* pvObject_);

/**
 * @brief Alloc_Semaphore
 * @sa Semaphore* AutoAlloc::NewSemaphore()
 * @return Handle to an allocated object, or nullptr if heap exhausted
 */
Semaphore_t Alloc_Semaphore(void);
void        Free_Semaphore(Semaphore_t handle);

/**
 * @brief Alloc_Mutex
 * @sa Mutex* AutoAlloc::NewMutex()
 * @return Handle to an allocated object, or nullptr if heap exhausted
 */
Mutex_t Alloc_Mutex(void);
void    Free_Mutex(Mutex_t handle);

#if KERNEL_EVENT_FLAGS
/**
 * @brief Alloc_EventFlag
 * @sa EventFlag* AutoAlloc::NewEventFlag()
 * @return Handle to an allocated object, or nullptr if heap exhausted
 */
EventFlag_t Alloc_EventFlag(void);
void        Free_EventFlag(EventFlag_t handle);
#endif // #if KERNEL_EVENT_FLAGS

/**
 * @brief Alloc_Message
 * @sa AutoAlloc::NewMessage()
 * @return Handle to an allocated object, or nullptr if heap exhausted
 */
Message_t Alloc_Message(void);
void      Free_Message(Message_t handle);

/**
 * @brief Alloc_MessageQueue
 * @sa MesageQueue* AutoAlloc::NewMessageQueue()
 * @return Handle to an allocated object, or nullptr if heap exhausted
 */
MessageQueue_t Alloc_MessageQueue(void);
void           Free_MessageQueue(MessageQueue_t handle);

MessagePool_t Alloc_MessagePool(void);
void          Free_MessagePool(MessagePool_t handle);

/**
 * @brief Alloc_Notify
 * @sa Notify* AutoAlloc::NewNotify()
 * @return Handle to an allocated object, or nullptr if heap exhausted
 */
Notify_t Alloc_Notify(void);
void     Free_Notify(Notify_t handle);

/**
 * @brief Alloc_Mailbox
 * @sa Mailbox* AutoAlloc::NewMailbox()
 * @return Handle to an allocated object, or nullptr if heap exhausted
 */
Mailbox_t Alloc_Mailbox(void);
void      Free_Mailbox(Mailbox_t handle);

/**
 * @brief Alloc_Thread
 * @sa Thread* AutoAlloc::NewThread()
 * @return Handle to an allocated object, or nullptr if heap exhausted
 */
Thread_t Alloc_Thread(void);
void     Free_Thread(Thread_t handle);

/**
 * @brief Alloc_Timer
 * @sa Timer* AutoAlloc::NewTimer()
 * @return Handle to an allocated object, or nullptr if heap exhausted
 */
Timer_t Alloc_Timer(void);
void    Free_Timer(Timer_t handle);

//---------------------------------------------------------------------------
// Kernel APIs
/**
 * @brief Kernel_Init
 * @sa void Kernel::Init()
 */
void Kernel_Init(void);
/**
 * @brief Kernel_Start
 * @sa void Kernel::Start()
 */
void Kernel_Start(void);
/**
 * @brief Kernel_IsStarted
 * @sa bool Kernel::IsStarted()
 * @return Whether or not the kernel has started - true = running, false =
 *         not started
 */
bool Kernel_IsStarted(void);

typedef void (*panic_func_t)(uint16_t u16PanicCode_);
/**
 * @brief Kernel_SetPanic
 * @sa void Kernel::SetPanic(PanicFunc_t pfPanic_)
 * @param pfPanic_ Panic function pointer
 */
void Kernel_SetPanic(panic_func_t pfPanic_);
/**
 * @brief Kernel_IsPanic
 * @sa bool Kernel::IsPanic()
 * @return Whether or not the kernel is in a panic state
 */
bool Kernel_IsPanic(void);
/**
 * @brief Kernel_Panic
 * @sa void Kernel::Panic(uint16_t u16Cause_)
 * @param u16Cause_ Reason for the kernel panic
 */
void Kernel_Panic(uint16_t u16Cause_);

#if KERNEL_THREAD_CREATE_CALLOUT
/**
 * @brief Kernel_SetThreadCreateCallout
 * @sa Kernel::SetThreadCreateCallout
 * @param pfCreate_ Function to calll on thread creation
 */
void Kernel_SetThreadCreateCallout(thread_create_callout_t pfCreate_);
#endif // #if KERNEL_THREAD_CREATE_CALLOUT

#if KERNEL_THREAD_EXIT_CALLOUT
/**
 * @brief Kernel_SetThreadExitCallout
 * @sa Kernel::SetThreadExitCallout
 * @param pfExit_ Function to call on thread exit
 */
void Kernel_SetThreadExitCallout(thread_exit_callout_t pfExit_);
#endif // #if KERNEL_THREAD_EXIT_CALLOUT

#if KERNEL_CONTEXT_SWITCH_CALLOUT
/**
 * @brief Kernel_SetThreadContextSwitchCallout
 * @sa Kernel::SetThreadContextSwitchCallout
 * @param pfContext_ Function to call prior to each context switch
 */
void Kernel_SetThreadContextSwitchCallout(thread_context_callout_t pfContext_);
#endif // #if KERNEL_CONTEXT_SWITCH_CALLOUT

#if KERNEL_THREAD_CREATE_CALLOUT
/**
 * @brief Kernel_GetThreadCreateCallout
 * @sa Kernel::GetThreadCreateCallout
 * @return Current function called on each thread creation
 */
thread_create_callout_t Kernel_GetThreadCreateCallout(void);
#endif // #if KERNEL_THREAD_CREATE_CALLOUT

#if KERNEL_THREAD_EXIT_CALLOUT
/**
 * @brief Kernel_GetThreadExitCallout
 * @sa Kernel::GetThreadExitCallout
 * @return Current function called on each thread exit
 */
thread_exit_callout_t Kernel_GetThreadExitCallout(void);
#endif // #if KERNEL_THREAD_EXIT_CALLOUT

#if KERNEL_CONTEXT_SWITCH_CALLOUT
/**
 * @brief Kernel_GetThreadContextSwitchCallout
 * @sa Kernel::GetThreadContextSwitchCallout
 * @return Current function called on each context switch
 */
thread_context_callout_t Kernel_GetThreadContextSwitchCallout(void);
#endif // #if KERNEL_CONTEXT_SWITCH_CALLOUT

#if KERNEL_STACK_CHECK
/**
 * @brief Kernel_SetStackGuardThreshold
 * @sa Kernel::SetStackGuardThreshold
 * @param u16Threshold_ Minimum number of guard-words for each thread stack.
 *        Violation results in kernel panic.
 */
void Kernel_SetStackGuardThreshold(uint16_t u16Threshold_);

/**
 * @brief Kernel_GetStackGuardThreshold
 * @sa Kernel::GetStackGuardThreshold
 * @return Current kernel stack-guard threshold
 */
uint16_t Kernel_GetStackGuardThreshold(void);
#endif // #if KERNEL_STACK_CHECK

/**
 * @brief Kernel_GetTicks
 * @sa Kernel::GetTicks()
 * @return Number of kernel ticks that have elapsed since boot
 */
uint32_t Kernel_GetTicks(void);

//---------------------------------------------------------------------------
// Scheduler APIs
/**
 * @brief Scheduler_Enable
 * @sa void Scheduler::SetScheduler(bool bEnable_)
 * @param bEnable_ true to enable, false to disable the scheduler
 */
void Scheduler_Enable(bool bEnable_);
/**
 * @brief Scheduler_IsEnabled
 * @sa bool Scheduler::IsEnabled()
 * @return true - scheduler enabled, false - disabled
 */
bool Scheduler_IsEnabled(void);
/**
 * @brief Scheduler_GetCurrentThread
 * @sa Thread* Scheduler::GetCurrentThread()
 * @return Handle of the currently-running thread
 */
Thread_t Scheduler_GetCurrentThread(void);

typedef void (*thread_entry_func_t)(void* pvArg_);
//---------------------------------------------------------------------------
// Thread APIs
/**
 * @brief Thread_Init
 * @sa void Thread::Init(K_WORD *pwStack_, uint16_t u16StackSize_, PORT_PRIO_TYPE uXPriority_, ThreadEntry_t
 * pfEntryPoint_, void *pvArg_)
 * @param handle        Handle of the thread to initialize
 * @param pwStack_      Pointer to the stack to use for the thread
 * @param u16StackSize_  Size of the stack (in bytes)
 * @param uXPriority_   Priority of the thread (0 = idle, 7 = max)
 * @param pfEntryPoint_ This is the function that gets called when the
 *                      thread is started
 * @param pvArg_        Pointer to the argument passed into the thread's
 *                      entrypoint function.
 */
void Thread_Init(Thread_t            handle,
                 K_WORD*             pwStack_,
                 uint16_t            u16StackSize_,
                 PORT_PRIO_TYPE      uXPriority_,
                 thread_entry_func_t pfEntryPoint_,
                 void*               pvArg_);
/**
 * @brief Thread_Start
 * @sa void Thread::Start()
 * @param handle Handle of the thread to start
 */
void Thread_Start(Thread_t handle);
/**
 * @brief Thread_Stop
 * @sa void Thread::Stop()
 * @param handle Handle of the thread to stop
 */
void Thread_Stop(Thread_t handle);

#if KERNEL_NAMED_THREADS
/**
 * @brief Thread_SetName
 * @sa void Thread::SetName(const char *szName_)
 * @param handle Handle of the thread
 * @param szName_ Name to set for the thread
 */
void Thread_SetName(Thread_t handle, const char* szName_);
/**
 * @brief Thread_GetName
 * @sa const char* Thread::GetName()
 * @param handle Handle of the thread
 * @return Thread current name, or nullptr if no name specified
 */
const char* Thread_GetName(Thread_t handle);
#endif // #if KERNEL_NAMED_THREADS

/**
 * @brief Thread_GetPriority
 * @sa PORT_PRIO_TYPE Thread::GetPriority()
 * @param handle Handle of the thread
 * @return Current priority of the thread not considering priority inheritence
 */
PORT_PRIO_TYPE Thread_GetPriority(Thread_t handle);
/**
 * @brief Thread_GetCurPriority
 * @sa PORT_PRIO_TYPE Thread::GetCurPriority()
 * @param handle Handle of the thread
 * @return Current priority of the thread considering priority inheritence
 */
PORT_PRIO_TYPE Thread_GetCurPriority(Thread_t handle);

#if KERNEL_ROUND_ROBIN
/**
 * @brief Thread_SetQuantum
 * @sa void Thread::SetQuentum(uint16_t u16Quantum_)
 * @param handle Handle of the thread
 * @param u16Quantum_ Time (in ticks) to set for the thread execution quantum
 */
void Thread_SetQuantum(Thread_t handle, uint16_t u16Quantum_);
/**
 * @brief Thread_GetQuantum
 * @sa uint16_t Thread::GetQuantum()
 * @param handle Handle of the thread
 * @return Thread's current execution quantum
 */
uint16_t Thread_GetQuantum(Thread_t handle);
#endif // #if KERNEL_ROUND_ROBIN

/**
 * @brief Thread_SetPriority
 * @sa void Thread::SetPriority(PORT_PRIO_TYPE uXPriority_)
 * @param handle Handle of the thread
 * @param uXPriority_ New priority level
 */
void Thread_SetPriority(Thread_t handle, PORT_PRIO_TYPE uXPriority_);
/**
 * @brief Thread_Exit
 * @sa void Thread::Exit()
 * @param handle Handle of the thread
 */
void Thread_Exit(Thread_t handle);
/**
 * @brief Thread_Sleep
 * @sa void Thread::Sleep(uint32_t u32TimeMs_)
 * @param u32TimeMs_ Time in ms to block the thread for
 */
void Thread_Sleep(uint32_t u32TimeMs_);

#if KERNEL_EXTENDED_CONTEXT
/**
 * @brief Thread_GetExtendedContext
 * @sa Thread::GetExtendedContext()
 * @param handle Handle of the thread
 * @return Pointer to the Thread's extended context
 */
void* Thread_GetExtendedContext(Thread_t handle);

/**
 * @brief Thread_SetExtendedContext
 * @sa Thread::SetExtendedContext(void* pvData_)
 * @param handle Handle of the Thread
 * @param pvData_ Pointer to Object to assign to Thread's extended context
 */
void Thread_SetExtendedContext(Thread_t handle, void* pvData_);
#endif // #if KERNEL_EXTENDED_CONTEXT

/**
 * @brief Thread_Yield
 * @sa void Thread::Yield()
 */
void Thread_Yield(void);

/**
 * @brief Thread_CoopYield
 * @sa void Thread::CoopYield()
 */
void Thread_CoopYield(void);

/**
 * @brief Thread_SetID
 * @sa void Thread::SetID(uint8_t u8ID_)
 * @param handle Handle of the thread
 * @param u8ID_ ID To assign to the thread
 */
void Thread_SetID(Thread_t handle, uint8_t u8ID_);
/**
 * @brief Thread_GetID
 * @sa uint8_t Thread::GetID()
 * @param handle Handle of the thread
 * @return Return ID assigned to the thread
 */
uint8_t Thread_GetID(Thread_t handle);

#if KERNEL_STACK_CHECK
/**
 * @brief Thread_GetStackSlack
 * @sa uint16_t Thread::GetStackSlack()
 * @param handle Handle of the thread
 * @return Return the amount of unused stack on the given thread
 */
uint16_t Thread_GetStackSlack(Thread_t handle);
#endif // #if KERNEL_STACK_CHECK

/**
 * @brief Thread_GetState
 * @sa ThreadState Thread::GetState()
 * @param handle Handle of the thread
 * @return The thread's current execution state
 */
thread_state_t Thread_GetState(Thread_t handle);

//---------------------------------------------------------------------------
// Timer APIs
typedef void (*timer_callback_t)(Thread_t hOwner_, void* pvData_);
/**
 * @brief Timer_Init
 * @sa void Timer::Init()
 * @param handle Handle of the timer
 */
void Timer_Init(Timer_t handle);
/**
 * @brief Timer_Start
 * @sa void Timer::Start(bool bRepeat_, uint32_t u32IntervalMs_, uint32_t u32ToleranceMs_, TimerCallbackC_t pfCallback_,
 * void *pvData_ )
 * @param handle Handle of the timer
 * @param bRepeat_          Restart the timer continuously on expiry
 * @param u32IntervalMs_    Time in ms to expiry
 * @param pfCallback_       Callback to run on timer expiry
 * @param pvData_           Data to pass to the callback on expiry
 */
void Timer_Start(Timer_t handle, bool bRepeat_, uint32_t u32IntervalMs_, timer_callback_t pfCallback_, void* pvData_);

/**
 * @brief Timer_Restart
 * @sa void Timer::Start()
 * @param handle Handle of the timer to restart.
 */
void Timer_Restart(Timer_t handle);

/**
 * @brief Timer_Stop
 * @sa void Timer::Stop()
 * @param handle Handle of the timer
 */
void Timer_Stop(Timer_t handle);

//---------------------------------------------------------------------------
// Semaphore APIs
/**
 * @brief Semaphore_Init
 * @sa void Semaphore::Init(uint16_t u16InitVal_, uint16_t u16MaxVal_)
 * @param handle Handle of the semaphore
 * @param u16InitVal_   Initial value of the semaphore
 * @param u16MaxVal_    Maximum value that can be held for a semaphore
 */
void Semaphore_Init(Semaphore_t handle, uint16_t u16InitVal_, uint16_t u16MaxVal_);
/**
 * @brief Semaphore_Post
 * @sa void Semaphore::Post()
 * @param handle Handle of the semaphore
 */
void Semaphore_Post(Semaphore_t handle);
/**
 * @brief Semaphore_Pend
 * @sa void Semaphore::Pend()
 * @param handle Handle of the semaphore
 */
void Semaphore_Pend(Semaphore_t handle);
/**
 * @brief Semaphore_TimedPend
 * @sa bool Semaphore::Pend(uint32_t u32WaitTimeMS_)
 * @param handle Handle of the semaphore
 * @param u32WaitTimeMS_ Time in ms to wait
 * @return true if semaphore was acquired, false on timeout
 */
bool Semaphore_TimedPend(Semaphore_t handle, uint32_t u32WaitTimeMS_);

//---------------------------------------------------------------------------
// Mutex APIs
/**
 * @brief Mutex_Init
 * @sa void Mutex::Init()
 * @param handle Handle of the mutex
 */
void Mutex_Init(Mutex_t handle);
/**
 * @brief Mutex_Claim
 * @sa void Mutex::Claim()
 * @param handle Handle of the mutex
 */
void Mutex_Claim(Mutex_t handle);
/**
 * @brief Mutex_Release
 * @sa void Mutex::Release()
 * @param handle Handle of the mutex
 */
void Mutex_Release(Mutex_t handle);
/**
 * @brief Mutex_TimedClaim
 * @sa bool Mutex::Claim(uint32_t u32WaitTimeMS_)
 * @param handle Handle of the mutex
 * @param u32WaitTimeMS_ Time to wait before aborting
 * @return true if mutex was claimed, false on timeout
 */
bool Mutex_TimedClaim(Mutex_t handle, uint32_t u32WaitTimeMS_);

#if KERNEL_EVENT_FLAGS
//---------------------------------------------------------------------------
// EventFlag APIs
/**
 * @brief EventFlag_Init
 * @sa void EventFlag::Init()
 * @param handle Handle of the event flag object
 */
void EventFlag_Init(EventFlag_t handle);
/**
 * @brief EventFlag_Wait
 * @sa uint16_t EventFlag::Wait(uint16_t u16Mask_, event_flag_operation_t eMode_)
 * @param handle Handle of the event flag object
 * @param u16Mask_ condition flags to wait for
 * @param eMode_   Specify conditions under which the thread will be unblocked
 * @return bitfield contained in the eventflag on unblock
 */
uint16_t EventFlag_Wait(EventFlag_t handle, uint16_t u16Mask_, event_flag_operation_t eMode_);
/**
 * @brief EventFlag_TimedWait
 * @sa uint16_t EventFlag::Wait(uint16_t u16Mask_, event_flag_operation_t eMode_, uint32_t u32TimeMS_)
 * @param handle Handle of the event flag object
 * @param u16Mask_  condition flags to wait for
 * @param eMode_    Specify conditions under which the thread will be unblocked
 * @param u32TimeMS_ Time in ms to wait before aborting the operation
 * @return bitfield contained in the eventflag on unblock, or 0 on expiry.
 */
uint16_t EventFlag_TimedWait(EventFlag_t handle, uint16_t u16Mask_, event_flag_operation_t eMode_, uint32_t u32TimeMS_);
/**
 * @brief EventFlag_Set
 * @sa void EventFlag::Set(uint16_t u16Mask_)
 * @param handle Handle of the event flag object
 * @param u16Mask_ Bits to set in the eventflag's internal condition register
 */
void EventFlag_Set(EventFlag_t handle, uint16_t u16Mask_);
/**
 * @brief EventFlag_Clear
 * @sa void EventFlag::Clear(uint16_t u16Mask_)
 * @param handle Handle of the event flag object
 * @param u16Mask_ Bits to clear in the eventflag's internal condition regster
 */
void EventFlag_Clear(EventFlag_t handle, uint16_t u16Mask_);
/**
 * @brief EventFlag_GetMask
 * @sa void EventFlag::GetMask()
 * @param handle Handle of the event flag object
 * @return Return the current bitmask
 */
uint16_t EventFlag_GetMask(EventFlag_t handle);
#endif // #if KERNEL_EVENT_FLAGS

//---------------------------------------------------------------------------
// Notification APIs
/**
 * @brief Notify_Init
 * @sa void Notify::Init()
 * @param handle Handle of the notification object
 */
void Notify_Init(Notify_t handle);
/**
 * @brief Notify_Signal
 * @sa void Notify::Signal()
 * @param handle Handle of the notification object
 */
void Notify_Signal(Notify_t handle);
/**
 * @brief Notify_Wait
 * @sa void Notify::Wait(bool *pbFlag_)
 * @param handle Handle of the notification object
 * @param pbFlag_ Flag to set to true on notification
 */
void Notify_Wait(Notify_t handle, bool* pbFlag_);
/**
 * @brief Notify_TimedWait
 * @sa bool Notify::Wait(uint32_t u32WaitTimeMS_, bool *pbFlag_)
 * @param handle Handle of the notification object
 * @param u32WaitTimeMS_ Maximum time to wait for notification in ms
 * @param pbFlag_  Flag to set to true on notification
 * @return true on unblock, false on timeout
 */
bool Notify_TimedWait(Notify_t handle, uint32_t u32WaitTimeMS_, bool* pbFlag_);

//---------------------------------------------------------------------------
// Atomic Functions
/**
 * @brief Atomic_Set8
 * @sa uint8_t Atomic::Set(uint8_t *pu8Source_, uint8_t u8Val_)
 * @param pu8Source_ Pointer to a variable to set the value of
 * @param u8Val_ New value to set in the variable
 * @return Previously-set value
 */
uint8_t Atomic_Set8(uint8_t* pu8Source_, uint8_t u8Val_);
/**
 * @brief Atomic_Set16
 * @sa uint16_t Atomic::Set(uint16_t *pu16Source_, uint16_t u16Val_)
 * @param pu16Source_ Pointer to a variable to set the value of
 * @param u16Val_ New value to set in the variable
 * @return Previously-set value
 */
uint16_t Atomic_Set16(uint16_t* pu16Source_, uint16_t u16Val_);
/**
 * @brief Atomic_Set32
 * @sa uint32_t Atomic::Set(uint32_t *pu32Source_, uint32_t u32Val_)
 * @param pu32Source_ Pointer to a variable to set the value of
 * @param u32Val_ New value to set in the variable
 * @return Previously-set value
 */
uint32_t Atomic_Set32(uint32_t* pu32Source_, uint32_t u32Val_);
/**
 * @brief Atomic_Add8
 * @sa uint8_t Atomic::Add(uint8_t *pu8Source_, uint8_t u8Val_)
 * @param pu8Source_ Pointer to a variable
 * @param u8Val_ Value to add to the variable
 * @return Previously-held value in pu8Source_
 */
uint8_t Atomic_Add8(uint8_t* pu8Source_, uint8_t u8Val_);
/**
 * @brief Atomic_Add16
 * @sa uint16_t Atomic::Add(uint16_t *pu16Source_, uint16_t u16Val_)
 * @param pu16Source_ Pointer to a variable
 * @param u16Val_ Value to add to the variable
 * @return Previously-held value in pu16Source_
 */
uint16_t Atomic_Add16(uint16_t* pu16Source_, uint16_t u16Val_);
/**
 * @brief Atomic_Add32
 * @sa uint32_t Atomic::Add(uint32_t *pu32Source_, uint32_t u32Val_)
 * @param pu32Source_ Pointer to a variable
 * @param u32Val_ Value to add to the variable
 * @return Previously-held value in pu32Source_
 */
uint32_t Atomic_Add32(uint32_t* pu32Source_, uint32_t u32Val_);
/**
 * @brief Atomic_Sub8
 * @sa uint8_t Atomic::Sub(uint8_t *pu8Source_, uint8_t u8Val_)
 * @param pu8Source_ Pointer to a variable
 * @param u8Val_ Value to subtract from the variable
 * @return Previously-held value in pu8Source_
 */
uint8_t Atomic_Sub8(uint8_t* pu8Source_, uint8_t u8Val_);
/**
 * @brief Atomic_Sub16
 * @sa uint16_t Atomic::Sub(uint16_t *pu16Source_, uint16_t u16Val_)
 * @param pu16Source_ Pointer to a variable
 * @param u16Val_ Value to subtract from the variable
 * @return Previously-held value in pu16Source_
 */
uint16_t Atomic_Sub16(uint16_t* pu16Source_, uint16_t u16Val_);
/**
 * @brief Atomic_Sub32
 * @sa uint32_t Atomic::Sub(uint32_t *pu32Source_, uint32_t u32Val_)
 * @param pu32Source_ Pointer to a variable
 * @param u32Val_ Value to subtract from the variable
 * @return Previously-held value in pu32Source_
 */
uint32_t Atomic_Sub32(uint32_t* pu32Source_, uint32_t u32Val_);
/**
 * @brief Atomic_TestAndSet
 * @sa bool Atomic::TestAndSet(bool *pbLock)
 * @param pbLock Pointer to a value to test against.  This will always
 *        be set to "true" at the end of a call to TestAndSet.
 *
 * @return true - Lock value was "true" on entry, false - Lock was set
 */
bool Atomic_TestAndSet(bool* pbLock);

//---------------------------------------------------------------------------
// Message/Message Queue APIs
/**
 * @brief Message_Init
 * @sa void Message::Init()
 * @param handle Handle of the message object
 */
void Message_Init(Message_t handle);
/**
 * @brief Message_SetData
 * @sa void Message::SetData(void *pvData_)
 * @param handle Handle of the message object
 * @param pvData_ Pointer to the data object to send in the message
 */
void Message_SetData(Message_t handle, void* pvData_);
/**
 * @brief Message_GetData
 * @sa void* Message::GetData()
 * @param handle Handle of the message object
 * @return Pointer to the data set in the message object
 */
void* Message_GetData(Message_t handle);
/**
 * @brief Message_SetCode
 * @sa void Message::SetCode(uint16_t u16Code_)
 * @param handle Handle of the message object
 * @param u16Code_ Data code to set in the object
 */
void Message_SetCode(Message_t handle, uint16_t u16Code_);
/**
 * @brief Message_GetCode
 * @sa uint16_t Message::GetCode()
 * @param handle Handle of the message object
 * @return user code set in the object
 */
uint16_t Message_GetCode(Message_t handle);
/**
 * @brief MessageQueue_Init
 * @sa void MessageQueue::Init()
 * @param handle Handle to the message queue to initialize
 */
void MessageQueue_Init(MessageQueue_t handle);
/**
 * @brief MessageQueue_Receive
 * @sa Message_t MessageQueue::Receive()
 * @param handle Handle of the message queue object
 * @return Pointer to a message object at the head of the queue
 */
Message_t MessageQueue_Receive(MessageQueue_t handle);
/**
 * @brief MessageQueue_TimedReceive
 * @sa Message_t MessageQueue::TimedReceive(uint32_t u32TimeWaitMS_)
 * @param handle Handle of the message queue object
 * @param u32TimeWaitMS_ The amount of time in ms to wait for a
 *        message before timing out and unblocking the waiting thread.
 *
 * @return Pointer to a message object at the head of the queue or
 *         nullptr on timeout.
 */
Message_t MessageQueue_TimedReceive(MessageQueue_t handle, uint32_t u32TimeWaitMS_);

/**
 * @brief MessageQueue_Send
 * @sa void MessageQueue::Send(Message *pclMessage_)
 * @param handle Handle of the message queue object
 * @param hMessage_ Handle to the message to send to the given queue
 */
void MessageQueue_Send(MessageQueue_t handle, Message_t hMessage_);

/**
 * @brief MessageQueue_GetCount
 * @sa uint16_t MessageQueue::GetCount()
 * @return Count of pending messages in the queue.
 */
uint16_t MessageQueue_GetCount(MessageQueue_t handle);

/**
 * @brief MessagePool_Init
 * @sa void MessagePool::Init()
 * @param handle Handle of the message pool object
 */
void MessagePool_Init(MessagePool_t handle);

/**
 * @brief MessagePool_Push
 * @sa void MessagePool::Push(Message* pclMessage_)
 * @param handle  Handle of the message pool object
 * @param msg Message object to return back to the pool
 */
void MessagePool_Push(MessagePool_t handle, Message_t msg);

/**
 * @brief MessagePool_Pop
 * @sa Message* MessagePool::Pop()
 * @param handle  Handle of the message pool object
 * @return Handle to a Message object, or nullptr on allocation error
 */
Message_t MessagePool_Pop(MessagePool_t handle);

//---------------------------------------------------------------------------
// Mailbox APIs

/**
 * @brief Mailbox_Init
 * @sa void Mailbox::Init(void *pvBuffer_, uint16_t u16BufferSize_, uint16_t u16ElementSize_ )
 * @param handle Handle of the mailbox object
 * @param pvBuffer_         Pointer to the static buffer to use for the mailbox
 * @param u16BufferSize_    Size of the mailbox buffer, in bytes
 * @param u16ElementSize_   Size of each envelope, in bytes
 */
void Mailbox_Init(Mailbox_t handle, void* pvBuffer_, uint16_t u16BufferSize_, uint16_t u16ElementSize_);

/**
 * @brief Mailbox_Send
 * @sa bool Mailbox::Send(void *pvData_)
 * @param handle Handle of the mailbox object
 * @param pvData_           Pointer to the data object to send to the mailbox.
 * @return                  true - envelope was delivered, false - mailbox is full.
 */
bool Mailbox_Send(Mailbox_t handle, void* pvData_);

/**
 * @brief Mailbox_SendTail
 * @sa bool Mailbox::SendTail(void *pvData_)
 * @param handle Handle of the mailbox object
 * @param pvData_           Pointer to the data object to send to the mailbox.
 * @return                  true - envelope was delivered, false - mailbox is full.
 */
bool Mailbox_SendTail(Mailbox_t handle, void* pvData_);

/**
 * @brief Mailbox_TimedSend
 * @sa bool Mailbox::Send(void *pvData_, uint32_t u32TimeoutMS_)
 * @param handle Handle of the mailbox object
 * @param pvData_           Pointer to the data object to send to the mailbox.
 * @param u32TimeoutMS_     Maximum time to wait for a free transmit slot
 * @return                  true - envelope was delivered, false - mailbox is full.
 */
bool Mailbox_TimedSend(Mailbox_t handle, void* pvData_, uint32_t u32TimeoutMS_);

/**
 * @brief Mailbox_TimedSendTail
 * @sa bool Mailbox::Send(void *pvData_, uint32_t u32TimeoutMS_)
 * @param handle Handle of the mailbox object
 * @param pvData_           Pointer to the data object to send to the mailbox.
 * @param u32TimeoutMS_     Maximum time to wait for a free transmit slot
 * @return                  true - envelope was delivered, false - mailbox is full.
 */
bool Mailbox_TimedSendTail(Mailbox_t handle, void* pvData_, uint32_t u32TimeoutMS_);

/**
 * @brief Mailbox_Receive
 * @sa void Mailbox::Receive(void *pvData_)
 * @param handle Handle of the mailbox object
 * @param pvData_ Pointer to a buffer that will have the envelope's contents
 *                copied into upon delivery.
 */
void Mailbox_Receive(Mailbox_t handle, void* pvData_);

/**
 * @brief Mailbox_ReceiveTail
 * @sa void Mailbox::ReceiveTail(void *pvData_)
 * @param handle Handle of the mailbox object
 * @param pvData_ Pointer to a buffer that will have the envelope's contents
 *                copied into upon delivery.
 */
void Mailbox_ReceiveTail(Mailbox_t handle, void* pvData_);

/**
 * @brief Mailbox_TimedReceive
 * @sa bool Mailbox::Receive(void *pvData_, uint32_t u32TimeoutMS_ )
 * @param handle Handle of the mailbox object
 * @param pvData_ Pointer to a buffer that will have the envelope's contents
 *                copied into upon delivery.
 * @param u32TimeoutMS_ Maximum time to wait for delivery.
 * @return true - envelope was delivered, false - delivery timed out.
 */
bool Mailbox_TimedReceive(Mailbox_t handle, void* pvData_, uint32_t u32TimeoutMS_);

/**
 * @brief Mailbox_TimedReceiveTail
 * @sa bool Mailbox::ReceiveTail(void *pvData_, uint32_t u32TimeoutMS_ )
 * @param handle Handle of the mailbox object
 * @param pvData_ Pointer to a buffer that will have the envelope's contents
 *                copied into upon delivery.
 * @param u32TimeoutMS_ Maximum time to wait for delivery.
 * @return true - envelope was delivered, false - delivery timed out.
 */
bool Mailbox_TimedReceiveTail(Mailbox_t handle, void* pvData_, uint32_t u32TimeoutMS_);

/**
 * @brief Mailbox_GetFreeSlots
 * @sa uint16_t Mailbox::GetFreeSlots()
 * @param handle Handle of the mailbox object
 * @return Number of free slots in the mailbox
 */
uint16_t Mailbox_GetFreeSlots(Mailbox_t handle);

/**
 * @brief Mailbox_IsFull
 * @sa bool Mailbox::IsFull()
 * @param handle Handle of the mailbox object
 * @return true if the mailbox is full, false otherwise
 */
bool Mailbox_IsFull(Mailbox_t handle);

/**
 * @brief Mailbox_IsEmpty
 * @sa bool Mailbox::IsEmpty()
 * @param handle Handle of the mailbox object
 * @return true if the mailbox is empty, false otherwise
 */
bool Mailbox_IsEmpty(Mailbox_t handle);

//---------------------------------------------------------------------------
// Condition Variables
/**
 * @brief ConditionVariable_Init
 * @sa void ConditionVariable::Init()
 * @param handle Handle of the condition variable object
 */
void ConditionVariable_Init(ConditionVariable_t handle);

/**
 * @brief ConditionVariable_Wait
 * @sa void ConditionVariable::Wait(Mutex* pclMutex_)
 * @param handle Handle of the condition variable object
 * @param hMutex_ Handle of the mutex to lock on acquisition of the condition variable
 */
void ConditionVariable_Wait(ConditionVariable_t handle, Mutex_t hMutex_);

/**
 * @brief ConditionVariable_Signal
 * @sa void ConditionVariable::Signal()
 * @param handle Handle of the condition variable object
 */
void ConditionVariable_Signal(ConditionVariable_t handle);

/**
 * @brief ConditionVariable_Broadcast
 * @sa void ConditionVariable::Broadcast()
 * @param handle Handle of the condition variable object
 */
void ConditionVariable_Broadcast(ConditionVariable_t handle);

/**
 * @brief ConditionVariable_TimedWait
 * @sa bool ConditionVariable::Wait(Mutex* pclMutex_, uint32_t u32WaitTimeMS_)
 * @param handle Handle of the condition variable object
 * @param hMutex_ Handle of the mutex to lock on acquisition of the condition variable
 * @param u32WaitTimeMS_ Maximum time to wait for object
 * @return true on success, false on timeout
 */
bool ConditionVariable_TimedWait(ConditionVariable_t handle, Mutex_t hMutex_, uint32_t u32WaitTimeMS_);

//---------------------------------------------------------------------------
// Reader-writer locks
/**
 * @brief ReaderWriterLock_Init
 * @sa void ReaderWriterLock::Init()
 * @param handle Handle of the reader-writer object
 */
void ReaderWriterLock_Init(ReaderWriterLock_t handle);

/**
 * @brief ReaderWriterLock_AcquireReader
 * @sa void ReaderWriterLock::AcquireReader()
 * @param handle Handle of the reader-writer object
 */
void ReaderWriterLock_AcquireReader(ReaderWriterLock_t handle);

/**
 * @brief ReaderWriterLock_ReleaseReader
 * @sa void ReaderWriterLock::ReleaseReader()
 * @param handle Handle of the reader-writer object
 */
void ReaderWriterLock_ReleaseReader(ReaderWriterLock_t handle);

/**
 * @brief ReaderWriterLock_AcquireWriter
 * @sa void ReaderWriterLock::AcquireWriter()
 * @param handle Handle of the reader-writer object
 */
void ReaderWriterLock_AcquireWriter(ReaderWriterLock_t handle);

/**
 * @brief ReaderWriterLock_ReleaseWriter
 * @sa void ReaderWriterLock::ReleaseWriter()
 * @param handle Handle of the reader-writer object
 */
void ReaderWriterLock_ReleaseWriter(ReaderWriterLock_t handle);

/**
 * @brief ReaderWriterLock_TimedAcquireWriter
 * @sa bool ReaderWriterLock::AcquireWriter(uint32_t u32TimeoutMs_)
 * @param handle Handle of the reader-writer object
 * @param u32TimeoutMs_ Maximum time to wait for the writer lock before bailing
 * @return true on success, false on timeout
 */
bool ReaderWriterLock_TimedAcquireWriter(ReaderWriterLock_t handle, uint32_t u32TimeoutMs_);

/**
 * @brief ReaderWriterLock_TimedAcquireReader
 * @sa bool ReaderWriterLock::AcquireReader(uint32_t u32TimeoutMs_)
 * @param handle Handle of the reader-writer object
 * @param u32TimeoutMs_  Maximum time to wait for the reader lock before bailing
 * @return true on success, false on timeout
 */
bool ReaderWriterLock_TimedAcquireReader(ReaderWriterLock_t handle, uint32_t u32TimeoutMs_);

/**
 * @brief Kernel_SetDebugPrintFunction
 * @sa void Kernel::SetDebugPrintFunction()
 * @param pfPrintFunction_ Function to use to print debug information from the kernel
 */
void Kernel_SetDebugPrintFunction(kernel_debug_print_t pfPrintFunction_);

/**
 * @brief KernelDebug_DebugPrint
 * @sa void DebugPrint(const char* szString_)
 * @param szString_ String to print to debug interface
 */
void Kernel_DebugPrint(const char* szString_);

#if defined(__cplusplus)
}
#endif
