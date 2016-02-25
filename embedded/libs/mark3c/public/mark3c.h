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
===========================================================================*/
/*!

    \file   mark3c.h

    \brief  Header providing C-language API bindings for the Mark3 kernel.

    \section MARK3C Mark3C - C-language API bindings for the Mark3 Kernel.

    The C-language Mark3C APIs map directly to their Mark3 counterparts using
    a simple set of conventions, documented below.  As a result, explicit API
    documentation for Mark3C is not necessary, as the functions map 1-1 to their
    C++ counterparts.

    \subsection MARK3CAPI API Conventions

    1) Static Methods:

    <ClassName>::<MethodName>()   Becomes    <ClassName>_<MethodName>()
    i.e. Kernel::Start()          Becomes    Kernel_Start()

    2) Kernel Object Methods:

    In short, any class instance is represented using an object handle, and is
    always passed into the relevant APIs as the first argument.  Further, any
    method that returns a pointer to an object in the C++ implementation now
    returns a handle to that object.

    <Object>.<MethodName>(<args>) Becomes    <ClassName>_<MethoodName>(<ObjectHandle>, <args>)

    i.e. clAppThread.Start()      Becomes    Thread_Start(hAppThread)

    3) Overloaded Methods:

    a) Methods overloaded with a Timeout parameter:

    <Object>.<MethodName>(<args>) Becomes    <ClassName>_Timed<MethodName>(<ObjectHandle>, <args>)

    i.e. clSemaphore.Wait(1000)   Becomes    Semaphore_Wait(hSemaphore, 1000)

    b) Methods overloaded based on number of arguments:

    <Object>.<MethodName>()                   Becomes     <ClassName>_<MethodName>(<ObjectHandle>)
    <Object>.<MethodName>(<arg1>)             Becomes     <ClassName>_<MethodName>1(<ObjectHandle>, <arg1>)
    <Object>.<MethodName>(<arg1>, <arg2>)     Becomes     <ClassName>_<MethodName>2(<ObjectHandle>, <arg1>, <arg2>)

    <ClassName>::<MethodName>()               Becomes     <ClassName>_<MethodName>(<ObjectHandle>)
    <ClassName>::<MethodName>(<arg1>)         Becomes     <ClassName>_<MethodName>1(<ObjectHandle>, <arg1>)
    <ClassName>::<MethodName>(<arg1>, <arg2>) Becomes     <ClassName>_<MethodName>2(<ObjectHandle>, <arg1>, <arg2>)

    c) Methods overloaded base on parameter types:

    <Object>.<MethodName>(<arg type_a>)       Becomes     <ClassName>_<MethodName><type_a>(<ObjectHandle>, <arg type a>)
    <Object>.<MethodName>(<arg type_b>)       Becomes     <ClassName>_<MethodName><type_b>(<ObjectHandle>, <arg type b>)
    <ClassName>::<MethodName>(<arg type_a>)   Becomes     <ClassName>_<MethodName><type_a>(<arg type a>)
    <ClassName>::<MethodName>(<arg type_b>)   Becomes     <ClassName>_<MethodName><type_b>(<arg type b>)

    d) Allocate-once memory allocation APIs

    AutoAlloc::New<ObjectName>                Becomes      Alloc_<ObjectName>
    AutoAlloc::Allocate(uint16_t u16Size_)    Becomes      AutoAlloc(uint16_t u16Size_)

    \subsection MARK3CALLOC Allocating Objects

    Aside from the API name translations, the object allocation scheme is the
    major different between Mark3C and Mark3.  Instead of instantiating objects
    of the various kernel types, kernel objects must be declared using Declaration
    macros, which serve the purpose of reserving memory for the kernel object, and
    provide an opaque handle to that object memory.  This is the case for
    statically-allocated objects, and objects allocated on the stack.

    Example: Declaring a thread

    \verbatim

    #include "mark3c.h"

    // Statically-allocated
    DECLARE_THREAD(hMyThread1);
    ...

    // On stack
    int main(void)
    {
        DECLARE_THREAD(hMyThread2);
        ...
    }

    \endverbatim


    Where:

        hMyThread1 - is a handle to a statically-allocated thread
        hMyThread2 - is a handle to a thread allocated from the main stack.

    Alternatively, the AutoAlloc APIs can be used to dynamically allocate objects,
    as demonstrated in the following example.

    \verbatim

    void Allocate_Example(void)
    {
        Thread_t hMyThread = AutoAlloc_Thread();

        Thread_Init(hMyThread, awMyStack, sizeof(awMyStack), 1, MyFunction, 0);
    }

    \endverbatim

    Note that the relevant kernel-object Init() function *must* be called prior to using
    any kernel object, whether or not they have been allocated statically, or dynamically.

*/

#include "mark3cfg.h"
#include "kerneltypes.h"
#include "fake_types.h"

#include <stdint.h>
#include <stdbool.h>

#ifndef __MARK3C_H__
#define __MARK3C_H__

#if defined(__cplusplus)
extern "C" {
#endif

//---------------------------------------------------------------------------
// Define a series of handle types to be used in place of the underlying classes
// of Mark3.
typedef void* EventFlag_t;      //!< EventFlag opaque handle data type
typedef void* Mailbox_t;        //!< Mailbox opaque handle data type
typedef void* Message_t;        //!< Message opaque handle data type
typedef void* MessageQueue_t;   //!< MessageQueue opaque handle data type
typedef void* Mutex_t;          //!< Mutex opaque handle data type
typedef void* Notify_t;         //!< Notification object opaque handle data type
typedef void* Semaphore_t;      //!< Semaphore opaque handle data type
typedef void* Thread_t;         //!< Thread opaque handle data type
typedef void* Timer_t;          //!< Timer opaque handle data type

//---------------------------------------------------------------------------
// Use the sizes of the structs in fake_types.h to generate opaque object-blobs
// that get instantiated as kernel objects (from the C++ code) later.
#define THREAD_SIZE         (sizeof(Fake_Thread))
#define TIMER_SIZE          (sizeof(Fake_Timer))
#define SEMAPHORE_SIZE      (sizeof(Fake_Semaphore))
#define MUTEX_SIZE          (sizeof(Fake_Mutex))
#define MESSAGE_SIZE        (sizeof(Fake_Message))
#define MESSAGEQUEUE_SIZE   (sizeof(Fake_MessageQueue))
#define MAILBOX_SIZE        (sizeof(Fake_Mailbox))
#define NOTIFY_SIZE         (sizeof(Fake_Notify))
#define EVENTFLAG_SIZE      (sizeof(Fake_EventFlag))

//---------------------------------------------------------------------------
// Macros for declaring opaque buffers of an appropriate size for the given
// kernel objects
#define TOKEN_1(x,y)    x ## y
#define TOKEN_2(x,y)    TOKEN_1(x,y)

// Ensure that opaque buffers are sized to the nearest word - which is
// a platform-dependent value.
#define WORD_ROUND(x)   (((x) + (sizeof(K_WORD) - 1)) / sizeof(K_WORD))

#define DECLARE_THREAD(name)    \
    K_WORD TOKEN_2(__thread_, name)[WORD_ROUND(THREAD_SIZE)]; \
    Thread_t name = (Thread_t)TOKEN_2(__thread_, name); \

#define DECLARE_TIMER(name)    \
    K_WORD TOKEN_2(__timer_, name)[WORD_ROUND(TIMER_SIZE)]; \
    Timer_t name = (Timer_t)TOKEN_2(__timer_, name); \

#define DECLARE_SEMAPHORE(name)    \
    K_WORD TOKEN_2(__semaphore_, name)[WORD_ROUND(SEMAPHORE_SIZE)]; \
    Semaphore_t name = (Semaphore_t)TOKEN_2(__semaphore_, name); \

#define DECLARE_MUTEX(name)    \
    K_WORD TOKEN_2(__mutex_, name)[WORD_ROUND(MUTEX_SIZE)]; \
    Mutex_t name = (Mutex_t)TOKEN_2(__mutex_, name); \

#define DECLARE_MESSAGE(name)    \
    K_WORD TOKEN_2(__message_, name)[WORD_ROUND(MESSAGE_SIZE)]; \
    Message_t name = (Message_t)TOKEN_2(__message_, name); \

#define DECLARE_MESSAGEQUEUE(name)    \
    K_WORD TOKEN_2(__messagequeue_, name)[WORD_ROUND(MESSAGEQUEUE_SIZE)]; \
    MessageQueue_t name = (MessageQueue_t)TOKEN_2(__messagequeue_, name); \

#define DECLARE_MAILBOX(name)    \
    K_WORD TOKEN_2(__mailbox_, name)[WORD_ROUND(MAILBOX_SIZE)]; \
    Mailbox_t name = (Mailbox_t)TOKEN_2(__mailbox_, name); \

#define DECLARE_NOTIFY(name)    \
    K_WORD TOKEN_2(__notify_, name)[WORD_ROUND(NOTIFY_SIZE)]; \
    Notify_t name = (Notify_t)TOKEN_2(__notify_, name); \

#define DECLARE_EVENTFLAG(name)    \
    K_WORD TOKEN_2(__eventflag_, name)[WORD_ROUND(EVENTFLAG_SIZE)]; \
    EventFlag_t name = (EventFlag_t)TOKEN_2(__eventflag_, name); \

//---------------------------------------------------------------------------
// Allocate-once Memory managment APIs
#if defined KERNEL_USE_AUTO_ALLOC
/*!
 * \brief AutoAlloc
 * \sa void* AutoAlloc::Allocate(uint16_t u16Size_)
 * \param u16Size_
 * \return
 */
void *AutoAlloc( uint16_t u16Size_ );
# if KERNEL_USE_SEMAPHORE
/*!
 * \brief Alloc_Semaphore
 * \sa Semaphore* AutoAlloc::NewSemaphore()
 * \return
 */
Semaphore_t Alloc_Semaphore(void);
# endif
# if KERNEL_USE_MUTEX
/*!
 * \brief Alloc_Mutex
 * \sa Mutex* AutoAlloc::NewMutex()
 * \return
 */
Mutex_t Alloc_Mutex(void);
# endif
# if KERNEL_USE_EVENTFLAG
/*!
 * \brief Alloc_EventFlag
 * \sa EventFlag* AutoAlloc::NewEventFlag()
 * \return
 */
EventFlag_t Alloc_EventFlag(void);
# endif
# if KERNEL_USE_MESSAGE
/*!
 * \brief Alloc_Message
 * \sa AutoAlloc::NewMessage()
 * \return
 */
Message_t Alloc_Message(void);
/*!
 * \brief Alloc_MessageQueue
 * \sa MesageQueue* AutoAlloc::NewMessageQueue()
 * \return
 */
MessageQueue_t Alloc_MessageQueue(void);
# endif
# if KERNEL_USE_NOTIFY
/*!
 * \brief Alloc_Notify
 * \sa Notify* AutoAlloc::NewNotify()
 * \return
 */
Notify_t Alloc_Notify(void);
# endif
# if KERNEL_USE_MAILBOX
/*!
 * \brief Alloc_Mailbox
 * \sa Mailbox* AutoAlloc::NewMailbox()
 * \return
 */
Mailbox_t Alloc_Mailbox(void);
# endif
/*!
 * \brief Alloc_Thread
 * \sa Thread* AutoAlloc::NewThread()
 * \return
 */
Thread_t Alloc_Thread(void);
# if KERNEL_USE_TIMERS
/*!
 * \brief Alloc_Timer
 * \sa Timer* AutoAlloc::NewTimer()
 * \return
 */
Timer_t Alloc_Timer(void);
# endif
#endif

//---------------------------------------------------------------------------
// Kernel APIs
/*!
 * \brief Kernel_Init
 * \sa void Kernel::Init()
 */
void Kernel_Init(void);
/*!
 * \brief Kernel_Start
 * \sa void Kernel::Start()
 */
void Kernel_Start(void);
/*!
 * \brief Kernel_IsStarted
 * \sa bool Kernel::IsStarted()
 * \return Whether or not the kernel has started - true = running, false =
 *         not started
 */
bool Kernel_IsStarted(void);
/*!
 * \brief Kernel_SetPanic
 * \sa void Kernel::SetPanic(panic_func_t pfPanic_)
 * \param pfPanic_ Panic function pointer
 */
void Kernel_SetPanic(panic_func_t pfPanic_);
/*!
 * \brief Kernel_IsPanic
 * \sa bool Kernel::IsPanic()
 * \return Whether or not the kernel is in a panic state
 */
bool Kernel_IsPanic(void);
/*!
 * \brief Kernel_Panic
 * \sa void Kernel::Panic(uint16_t u16Cause_)
 * \param u16Cause_ Reason for the kernel panic
 */
void Kernel_Panic(uint16_t u16Cause_);
#if KERNEL_USE_IDLE_FUNC
/*!
 * \brief Kernel_SetIdleFunc
 * \sa void Kernel::SetIdleFunc(idle_func_t pfIdle_)
 * \param pfIdle_ Pointer to the idle function
 */
void Kernel_SetIdleFunc(idle_func_t pfIdle_);
#endif

//---------------------------------------------------------------------------
// Scheduler APIs
/*!
 * \brief Scheduler_Enable
 * \sa void Scheduler::SetScheduler(bool bEnable_)
 * \param bEnable_true to enable, false to disable the scheduler
 */
void Scheduler_Enable(bool bEnable_);
/*!
 * \brief Scheduler_IsEnabled
 * \sa bool Scheduler::IsEnabled()
 * \return true - scheduler enabled, false - disabled
 */
bool Scheduler_IsEnabled(void);
/*!
 * \brief Scheduler_GetCurrentThread
 * \sa Thread* Scheduler::GetCurrentThread()
 * \return Handle of the currently-running thread
 */
Thread_t Scheduler_GetCurrentThread(void);

//---------------------------------------------------------------------------
// Thread APIs
/*!
 * \brief Thread_Init
 * \sa void Thread::Init(K_WORD *pwStack_, uint16_t u16StackSize_, PRIO_TYPE uXPriority_, ThreadEntry_t pfEntryPoint_, void *pvArg_)
 * \param handle        Handle of the thread to initialize
 * \param pwStack_      Pointer to the stack to use for the thread
 * \param u16StackSize_  Size of the stack (in bytes)
 * \param uXPriority_   Priority of the thread (0 = idle, 7 = max)
 * \param pfEntryPoint_ This is the function that gets called when the
 *                      thread is started
 * \param pvArg_        Pointer to the argument passed into the thread's
 *                      entrypoint function.
 */
void Thread_Init(Thread_t handle,
                 K_WORD *pwStack_,
                 uint16_t u16StackSize_,
                 PRIO_TYPE uXPriority_,
                 ThreadEntry_t pfEntryPoint_,
                 void *pvArg_);
/*!
 * \brief Thread_Start
 * \sa void Thread::Start()
 * \param handle Handle of the thread to start
 */
void Thread_Start(Thread_t handle);
/*!
 * \brief Thread_Stop
 * \sa void Thread::Stop()
 * \param handle Handle of the thread to stop
 */
void Thread_Stop(Thread_t handle);
#if KERNEL_USE_THREADNAME
/*!
 * \brief Thread_SetName
 * \sa void Thread::SetName(const char *szName_)
 * \param handle Handle of the thread
 * \param szName_
 */
void Thread_SetName(Thread_t handle, const char *szName_);
/*!
 * \brief Thread_GetName
 * \sa const char* Thread::GetName()
 * \param handle Handle of the thread
 * \return
 */
const char* Thread_GetName(Thread_t handle);
#endif
/*!
 * \brief Thread_GetPriority
 * \sa PRIO_TYPE Thread::GetPriority()
 * \param handle Handle of the thread
 * \return
 */
PRIO_TYPE   Thread_GetPriority(Thread_t handle);
/*!
 * \brief Thread_GetCurPriority
 * \sa PRIO_TYPE Thread::GetCurPriority()
 * \param handle Handle of the thread
 * \return
 */
PRIO_TYPE   Thread_GetCurPriority(Thread_t handle);
#if KERNEL_USE_QUANTUM
/*!
 * \brief Thread_SetQuantum
 * \sa void Thread::SetQuentum(uint16_t u16Quantum_)
 * \param handle Handle of the thread
 * \param u16Quantum_
 */
void Thread_SetQuantum(Thread_t handle, uint16_t u16Quantum_);
/*!
 * \brief Thread_GetQuantum
 * \sa uint16_t Thread::GetQuantum()
 * \param handle Handle of the thread
 * \return
 */
uint16_t Thread_GetQuantum(Thread_t handle);
#endif
/*!
 * \brief Thread_SetPriority
 * \sa void Thread::SetPriority(PRIO_TYPE uXPriority_)
 * \param handle Handle of the thread
 * \param uXPriority_
 */
void Thread_SetPriority(Thread_t handle, PRIO_TYPE uXPriority_);
#if KERNEL_USE_DYNAMIC_THREADS
/*!
 * \brief Thread_Exit
 * \sa void Thread::Exit()
 * \param handle Handle of the thread
 */
void Thread_Exit(Thread_t handle);
#endif
#if KERNEL_USE_SLEEP
/*!
 * \brief Thread_Sleep
 * \sa void Thread::Sleep(uint32_t u32TimeMs_)
 * \param u32TimeMs_
 */
void Thread_Sleep(uint32_t u32TimeMs_);
/*!
 * \brief Thread_USleep
 * \sa void Thread::USleep(uint32_t u32TimeUs_)
 * \param u32TimeUs_
 */
void Thread_USleep(uint32_t u32TimeUs_);
#endif
/*!
 * \brief Thread_Yield
 * \sa void Thread::Yield()
 */
void Thread_Yield(void);
/*!
 * \brief Thread_SetID
 * \sa void Thread::SetID(uint8_t u8ID_)
 * \param handle Handle of the thread
 * \param u8ID_
 */
void Thread_SetID(Thread_t handle, uint8_t u8ID_);
/*!
 * \brief Thread_GetID
 * \sa uint8_t Thread::GetID()
 * \param handle Handle of the thread
 * \return
 */
uint8_t Thread_GetID(Thread_t handle);
/*!
 * \brief Thread_GetStackSlack
 * \sa uint16_t Thread::GetStackSlack()
 * \param handle Handle of the thread
 * \return
 */
uint16_t Thread_GetStackSlack(Thread_t handle);
/*!
 * \brief Thread_GetState
 * \sa ThreadState_t Thread::GetState()
 * \param handle Handle of the thread
 * \return
 */
ThreadState_t Thread_GetState(Thread_t handle);

//---------------------------------------------------------------------------
// Timer APIs
#if KERNEL_USE_TIMERS
typedef void (*TimerCallbackC_t)(Thread_t hOwner_, void *pvData_);
/*!
 * \brief Timer_Init
 * \sa void Timer::Init()
 * \param handle Handle of the timer
 */
void Timer_Init(Timer_t handle);
/*!
 * \brief Timer_Start
 * \sa void Timer::Start(bool bRepeat_, uint32_t u32IntervalMs_, uint32_t u32ToleranceMs_, TimerCallbackC_t pfCallback_, void *pvData_ )
 * \param handle Handle of the timer
 * \param bRepeat_
 * \param u32IntervalMs_
 * \param u32ToleranceMs_
 * \param pfCallback_
 * \param pvData_
 */
void Timer_Start(Timer_t handle, bool bRepeat_, uint32_t u32IntervalMs_, uint32_t u32ToleranceMs_, TimerCallbackC_t pfCallback_, void *pvData_ );
/*!
 * \brief Timer_Stop
 * \sa void Timer::Stop()
 * \param handle Handle of the timer
 */
void Timer_Stop(Timer_t handle);
#endif

//---------------------------------------------------------------------------
// Semaphore APIs
#if KERNEL_USE_SEMAPHORE
/*!
 * \brief Semaphore_Init
 * \sa void Semaphore::Init(uint16_t u16InitVal_, uint16_t u16MaxVal_)
 * \param handle Handle of the semaphore
 * \param u16InitVal_
 * \param u16MaxVal_
 */
void Semaphore_Init(Semaphore_t handle, uint16_t u16InitVal_, uint16_t u16MaxVal_);
/*!
 * \brief Semaphore_Post
 * \sa void Semaphore::Post()
 * \param handle Handle of the semaphore
 */
void Semaphore_Post(Semaphore_t handle);
/*!
 * \brief Semaphore_Pend
 * \sa void Semaphore::Pend()
 * \param handle Handle of the semaphore
 */
void Semaphore_Pend(Semaphore_t handle);
# if KERNEL_USE_TIMEOUTS
/*!
 * \brief Semaphore_TimedPend
 * \sa bool Semaphore::Pend(uint32_t u32WaitTimeMS_)
 * \param handle Handle of the semaphore
 * \param u32WaitTimeMS_
 * \return
 */
bool Semaphore_TimedPend(Semaphore_t handle, uint32_t u32WaitTimeMS_);
# endif
#endif

//---------------------------------------------------------------------------
// Mutex APIs
#if KERNEL_USE_MUTEX
/*!
 * \brief Mutex_Init
 * \sa void Mutex::Init()
 * \param handle Handle of the mutex
 */
void Mutex_Init(Mutex_t handle);
/*!
 * \brief Mutex_Claim
 * \sa void Mutex::Claim()
 * \param handle Handle of the mutex
 */
void Mutex_Claim(Mutex_t handle);
/*!
 * \brief Mutex_Release
 * \sa void Mutex::Release()
 * \param handle Handle of the mutex
 */
void Mutex_Release(Mutex_t handle);
# if KERNEL_USE_TIMEOUTS
/*!
 * \brief Mutex_TimedClaim
 * \sa bool Mutex::Claim(uint32_t u32WaitTimeMS_)
 * \param handle Handle of the mutex
 * \param u32WaitTimeMS_
 * \return
 */
bool Mutex_TimedClaim(Mutex_t handle, uint32_t u32WaitTimeMS_);
# endif
#endif

//---------------------------------------------------------------------------
// EventFlag APIs
#if KERNEL_USE_EVENTFLAG
/*!
 * \brief EventFlag_Init
 * \sa void EventFlag::Init()
 * \param handle Handle of the event flag object
 */
void EventFlag_Init(EventFlag_t handle);
/*!
 * \brief EventFlag_Wait
 * \sa uint16_t EventFlag::Wait(uint16_t u16Mask_, EventFlagOperation_t eMode_)
 * \param handle Handle of the event flag object
 * \param u16Mask_
 * \param eMode_
 * \return
 */
uint16_t EventFlag_Wait(EventFlag_t handle, uint16_t u16Mask_, EventFlagOperation_t eMode_);
# if KERNEL_USE_TIMEOUTS
/*!
 * \brief EventFlag_TimedWait
 * \sa uint16_t EventFlag::Wait(uint16_t u16Mask_, EventFlagOperation_t eMode_, uint32_t u32TimeMS_)
 * \param handle Handle of the event flag object
 * \param u16Mask_
 * \param eMode_
 * \param u32TimeMS_
 * \return
 */
uint16_t EventFlag_TimedWait(EventFlag_t handle, uint16_t u16Mask_, EventFlagOperation_t eMode_, uint32_t u32TimeMS_);
# endif
/*!
 * \brief EventFlag_Set
 * \sa void EventFlag::Set(uint16_t u16Mask_)
 * \param handle Handle of the event flag object
 * \param u16Mask_
 */
void EventFlag_Set(EventFlag_t handle, uint16_t u16Mask_);
/*!
 * \brief EventFlag_Clear
 * \sa void EventFlag::Clear(uint16_t u16Mask_)
 * \param handle Handle of the event flag object
 * \param u16Mask_
 */
void EventFlag_Clear(EventFlag_t handle, uint16_t u16Mask_);
/*!
 * \brief EventFlag_GetMask
 * \sa void EventFlag::GetMask()
 * \param handle Handle of the event flag object
 * \return
 */
uint16_t EventFlag_GetMask(EventFlag_t handle);
#endif

//---------------------------------------------------------------------------
// Notification APIs
#if KERNEL_USE_NOTIFY
/*!
 * \brief Notify_Init
 * \sa void Notify::Init()
 * \param handle Handle of the notification object
 */
void Notify_Init(Notify_t handle);
/*!
 * \brief Notify_Signal
 * \sa void Notify::Signal()
 * \param handle Handle of the notification object
 */
void Notify_Signal(Notify_t handle);
/*!
 * \brief Notify_Wait
 * \sa void Notify::Wait(bool *pbFlag_)
 * \param handle Handle of the notification object
 * \param pbFlag_
 */
void Notify_Wait(Notify_t handle, bool *pbFlag_);
# if KERNEL_USE_TIMEOUTS
/*!
 * \brief Notify_TimedWait
 * \sa bool Notify::Wait(uint32_t u32WaitTimeMS_, bool *pbFlag_)
 * \param handle Handle of the notification object
 * \param u32WaitTimeMS_
 * \param pbFlag_
 * \return
 */
bool Notify_TimedWait(Notify_t handle, uint32_t u32WaitTimeMS_, bool *pbFlag_);
# endif
#endif

//---------------------------------------------------------------------------
// Atomic Functions
#if KERNEL_USE_ATOMIC
/*!
 * \brief Atomic_Set8
 * \sa uint8_t Atomic::Set(uint8_t *pu8Source_, uint8_t u8Val_)
 * \param pu8Source_
 * \param u8Val_
 * \return
 */
uint8_t Atomic_Set8( uint8_t *pu8Source_, uint8_t u8Val_ );
/*!
 * \brief Atomic_Set16
 * \sa uint16_t Atomic::Set(uint16_t *pu16Source_, uint16_t u16Val_)
 * \param pu16Source_
 * \param u16Val_
 * \return
 */
uint16_t Atomic_Set16( uint16_t *pu16Source_, uint16_t u16Val_ );
/*!
 * \brief Atomic_Set32
 * \sa uint32_t Atomic::Set(uint32_t *pu32Source_, uint32_t u32Val_)
 * \param pu32Source_
 * \param u32Val_
 * \return
 */
uint32_t Atomic_Set32( uint32_t *pu32Source_, uint32_t u32Val_ );
/*!
 * \brief Atomic_Add8
 * \sa uint8_t Atomic::Add(uint8_t *pu8Source_, uint8_t u8Val_)
 * \param pu8Source_
 * \param u8Val_
 * \return
 */
uint8_t Atomic_Add8( uint8_t *pu8Source_, uint8_t u8Val_ );
/*!
 * \brief Atomic_Add16
 * \sa uint16_t Atomic::Add(uint16_t *pu16Source_, uint16_t u16Val_)
 * \param pu16Source_
 * \param u16Val_
 * \return
 */
uint16_t Atomic_Add16( uint16_t *pu16Source_, uint16_t u16Val_ );
/*!
 * \brief Atomic_Add32
 * \sa uint32_t Atomic::Add(uint32_t *pu32Source_, uint32_t u32Val_)
 * \param pu32Source_
 * \param u32Val_
 * \return
 */
uint32_t Atomic_Add32( uint32_t *pu32Source_, uint32_t u32Val_ );
/*!
 * \brief Atomic_Sub8
 * \sa uint8_t Atomic::Sub(uint8_t *pu8Source_, uint8_t u8Val_)
 * \param pu8Source_
 * \param u8Val_
 * \return
 */
uint8_t Atomic_Sub8( uint8_t *pu8Source_, uint8_t u8Val_ );
/*!
 * \brief Atomic_Sub16
 * \sa uint16_t Atomic::Sub(uint16_t *pu16Source_, uint16_t u16Val_)
 * \param pu16Source_
 * \param u16Val_
 * \return
 */
uint16_t Atomic_Sub16( uint16_t *pu16Source_, uint16_t u16Val_ );
/*!
 * \brief Atomic_Sub32
 * \sa uint32_t Atomic::Sub(uint32_t *pu32Source_, uint32_t u32Val_)
 * \param pu32Source_
 * \param u32Val_
 * \return
 */
uint32_t Atomic_Sub32( uint32_t *pu32Source_, uint32_t u32Val_ );
/*!
 * \brief Atomic_TestAndSet
 * \sa bool Atomic::TestAndSet(bool *pbLock)
 * \param pbLock
 * \return
 */
bool Atomic_TestAndSet( bool *pbLock );
#endif

//---------------------------------------------------------------------------
// Message/Message Queue APIs
#if KERNEL_USE_MESSAGE
/*!
 * \brief Message_Init
 * \sa void Message::Init()
 * \param handle Handle of the message object
 */
void Message_Init(Message_t handle);
/*!
 * \brief Message_SetData
 * \sa void Message::SetData(void *pvData_)
 * \param handle Handle of the message object
 * \param pvData_
 */
void Message_SetData(Message_t handle, void *pvData_);
/*!
 * \brief Message_GetData
 * \sa void* Message::GetData()
 * \param handle Handle of the message object
 * \return
 */
void* Message_GetData(Message_t handle);
/*!
 * \brief Message_SetCode
 * \sa void Message::SetCode(uint16_t u16Code_)
 * \param handle Handle of the message object
 * \param u16Code_
 */
void Message_SetCode(Message_t handle, uint16_t u16Code_);
/*!
 * \brief Message_GetCode
 * \sa uint16_t Message::GetCode()
 * \param handle Handle of the message object
 * \return
 */
uint16_t Message_GetCode(Message_t handle);
/*!
 * \brief GlobalMessagePool_Push
 * \sa void GlobalMessagePool::Push()
 * \param handle Handle of the message object
 */
void GlobalMessagePool_Push(Message_t handle);
/*!
 * \brief GlobalMessagePool_Pop
 * \sa Message_t GlobalMessagePool::Pop()
 * \return
 */
Message_t GlobalMessagePool_Pop(void);
/*!
 * \brief MessageQueue_Init
 * \sa void MessageQueue::Init()
 * \param handle
 */
void MessageQueue_Init(MessageQueue_t handle);
/*!
 * \brief MessageQueue_Receive
 * \sa Message_t MessageQueue::Receive()
 * \param handle Handle of the message queue object
 * \return
 */
Message_t MessageQueue_Receive(MessageQueue_t handle);
# if KERNEL_USE_TIMEOUTS
/*!
 * \brief MessageQueue_TimedReceive
 * \sa Message_t MessageQueue::TimedReceive(uint32_t u32TimeWaitMS_)
 * \param handle Handle of the message queue object
 * \param u32TimeWaitMS_
 * \return
 */
Message_t MessageQueue_TimedReceive(MessageQueue_t handle, uint32_t u32TimeWaitMS_);
# endif

/*!
 * \brief MessageQueue_Send
 * \sa void MessageQueue::Send(Message *pclMessage_)
 * \param handle Handle of the message queue object
 * \param hMessage_
 */
void MessageQueue_Send(MessageQueue_t handle, Message_t hMessage_);

/*!
 * \brief MessageQueue_GetCount
 * \sa uint16_t MessageQueue::GetCount()
 * \return
 */
uint16_t MessageQueue_GetCount(void);
#endif

//---------------------------------------------------------------------------
// Mailbox APIs
#if KERNEL_USE_MAILBOX

/*!
 * \brief Mailbox_Init
 * \sa void Mailbox::Init(void *pvBuffer_, uint16_t u16BufferSize_, uint16_t u16ElementSize_ )
 * \param handle Handle of the mailbox object
 * \param pvBuffer_
 * \param u16BufferSize_
 * \param u16ElementSize_
 */
void Mailbox_Init(Mailbox_t handle, void *pvBuffer_, uint16_t u16BufferSize_, uint16_t u16ElementSize_ );

/*!
 * \brief Mailbox_Send
 * \sa bool Mailbox::Send(void *pvData_)
 * \param handle Handle of the mailbox object
 * \param pvData_
 * \return
 */
bool Mailbox_Send(Mailbox_t handle, void *pvData_);

/*!
 * \brief Mailbox_SendTail
 * \sa bool Mailbox::SendTail(void *pvData_)
 * \param handle Handle of the mailbox object
 * \param pvData_
 * \return
 */
bool Mailbox_SendTail(Mailbox_t handle, void *pvData_);

/*!
 * \brief Mailbox_TimedSend
 * \sa bool Mailbox::Send(void *pvData_, uint32_t u32TimeoutMS_)
 * \param handle Handle of the mailbox object
 * \param pvData_
 * \param u32TimeoutMS_
 * \return
 */
bool Mailbox_TimedSend(Mailbox_t handle, void *pvData_, uint32_t u32TimeoutMS_);

/*!
 * \brief Mailbox_TimedSendTail
 * \sa bool Mailbox::Send(void *pvData_, uint32_t u32TimeoutMS_)
 * \param handle Handle of the mailbox object
 * \param pvData_
 * \param u32TimeoutMS_
 * \return
 */
bool Mailbox_TimedSendTail(Mailbox_t handle, void *pvData_, uint32_t u32TimeoutMS_);

/*!
 * \brief Mailbox_Receive
 * \sa void Mailbox::Receive(void *pvData_)
 * \param handle Handle of the mailbox object
 * \param pvData_
 */
void Mailbox_Receive(Mailbox_t handle, void *pvData_);

/*!
 * \brief Mailbox_ReceiveTail
 * \sa void Mailbox::ReceiveTail(void *pvData_)
 * \param handle Handle of the mailbox object
 * \param pvData_
 */
void Mailbox_ReceiveTail(Mailbox_t handle, void *pvData_);
# if KERNEL_USE_TIMEOUTS

/*!
 * \brief Mailbox_TimedReceive
 * \sa bool Mailbox::Receive(void *pvData_, uint32_t u32TimeoutMS_ )
 * \param handle Handle of the mailbox object
 * \param pvData_
 * \param u32TimeoutMS_
 * \return
 */
bool Mailbox_TimedReceive(Mailbox_t handle, void *pvData_, uint32_t u32TimeoutMS_ );

/*!
 * \brief Mailbox_TimedReceiveTail
 * \sa bool Mailbox::ReceiveTail(void *pvData_, uint32_t u32TimeoutMS_ )
 * \param handle Handle of the mailbox object
 * \param pvData_
 * \param u32TimeoutMS_
 * \return
 */
bool Mailbox_TimedReceiveTail(Mailbox_t handle, void *pvData_, uint32_t u32TimeoutMS_ );

/*!
 * \brief Mailbox_GetFreeSlots
 * \sa uint16_t Mailbox::GetFreeSlots()
 * \param handle Handle of the mailbox object
 * \return
 */
uint16_t Mailbox_GetFreeSlots(Mailbox_t handle);

/*!
 * \brief Mailbox_IsFull
 * \sa bool Mailbox::IsFull()
 * \param handle Handle of the mailbox object
 * \return
 */
bool Mailbox_IsFull(Mailbox_t handle);

/*!
 * \brief Mailbox_IsEmpty
 * \sa bool Mailbox::IsEmpty()
 * \param handle Handle of the mailbox object
 * \return
 */
bool Mailbox_IsEmpty(Mailbox_t handle);
# endif
#endif

//---------------------------------------------------------------------------
// Kernel-Aware Simulation APIs
#if KERNEL_AWARE_SIMULATION

/*!
 * \brief KernelAware_ProfileInit
 * \sa void KernelAware::ProfileInit(const char *szStr_);
 *
 * \param szStr_
 */
void KernelAware_ProfileInit( const char *szStr_ );

/*!
 * \brief KernelAware_ProfileStart
 * \sa void KernelAware::ProfileStart()
 */
void KernelAware_ProfileStart( void );

/*!
 * \brief KernelAware_ProfileStop
 * \sa void KernelAware::ProfileStop()
 */
void KernelAware_ProfileStop( void );

/*!
 * \brief KernelAware_ProfileReport
 * \sa void KernelAware::ProfileReport()
 */
void KernelAware_ProfileReport( void );

/*!
 * \brief KernelAware_ExitSimulator
 * \sa void KernelAware::ExitSimulator()
 *
 */
void KernelAware_ExitSimulator( void );

/*!
 * \brief KernelAware_Print
 * \sa void KernelAware::Print(const char *szStr_)
 * \param szStr_
 */
void KernelAware_Print( const char *szStr_ );

/*!
 * \brief KernelAware_Trace
 * \sa void KernelAware::Trace(uint16_t u16File_, uint16_t u16Line_);
 *
 * \param u16File_
 * \param u16Line_
 */
void KernelAware_Trace( uint16_t u16File_,
                        uint16_t u16Line_);

/*!
 * \brief KernelAware_Trace1
 * \sa void KernelAware::Trace(uint16_t u16File_, uint16_t u16Line_, uint16_t u16Arg1_);
 * \param u16File_
 * \param u16Line_
 * \param u16Arg1_
 */
void KernelAware_Trace1( uint16_t u16File_,
                         uint16_t u16Line_,
                         uint16_t u16Arg1_);
/*!
 * \brief KernelAware_Trace2
 * \sa void KernelAware::Trace(uint16_t u16File_, uint16_t u16Line_, uint16_t u16Arg1_, uint16_t u16Arg2_);
 * \param u16File_
 * \param u16Line_
 * \param u16Arg1_
 * \param u16Arg2_
 */
void KernelAware_Trace2( uint16_t u16File_,
                         uint16_t u16Line_,
                         uint16_t u16Arg1_,
                         uint16_t u16Arg2_);
/*!
 * \brief KernelAware_IsSimulatorAware
 *
 * \sa void Kernel::IsSimulatorAware()
 *
 * \return
 */
bool KernelAware_IsSimulatorAware(void);
#endif

#if defined(__cplusplus)
}
#endif

#endif // __MARK3C_H__
