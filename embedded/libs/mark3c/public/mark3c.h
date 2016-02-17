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

    \brief  Header providing C-language wrappers for the Mark3 kernel.
*/

#include "mark3cfg.h"
#include "kerneltypes.h"

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
typedef void* EventFlag_t;
typedef void* Mailbox_t;
typedef void* Message_t;
typedef void* MessageQueue_t;
typedef void* Mutex_t;
typedef void* Notify_t;
typedef void* Semaphore_t;
typedef void* Thread_t;
typedef void* Timer_t;

//---------------------------------------------------------------------------
// ToDo: Use the struct_profiling script to generate these numbers as part
// of the build process.  These are also only valid for AVR right now, and 
// are subject to change based on changes to mark3cfg.h at build-time.
#define THREAD_SIZE         (51)
#define TIMER_SIZE          (23)
#define SEMAPHORE_SIZE      (13)
#define MUTEX_SIZE          (14)
#define MESSAGE_SIZE        (8)
#define MESSAGEQUEUE_SIZE   (19)
#define MAILBOX_SIZE        (38)
#define NOTIFY_SIZE         (9)
#define EVENTFLAG_SIZE      (11)

//---------------------------------------------------------------------------
// Macros for declaring opaque buffers of an appropriate size for the given
// kernel objects

#define TOKEN_1(x,y)    x ## y
#define TOKEN_2(x,y)    TOKEN_1(x,y)

#define DECLARE_THREAD(name)    \
    uint8_t TOKEN_2(__thread_, name)[THREAD_SIZE]; \
    Thread_t name = (Thread_t)TOKEN_2(__thread_, name); \

#define DECLARE_TIMER(name)    \
    uint8_t TOKEN_2(__timer_, name)[TIMER_SIZE]; \
    Timer_t name = (Timer_t)TOKEN_2(__timer_, name); \

#define DECLARE_SEMAPHORE(name)    \
    uint8_t TOKEN_2(__semaphore_, name)[SEMAPHORE_SIZE]; \
    Semaphore_t name = (Semaphore_t)TOKEN_2(__semaphore_, name); \

#define DECLARE_MUTEX(name)    \
    uint8_t TOKEN_2(__mutex_, name)[MUTEX_SIZE]; \
    Mutex_t name = (Mutex_t)TOKEN_2(__mutex_, name); \

#define DECLARE_MESSAGE(name)    \
    uint8_t TOKEN_2(__message_, name)[MESSAGE_SIZE]; \
    Message_t name = (Message_t)TOKEN_2(__message_, name); \

#define DECLARE_MESSAGEQUEUE(name)    \
    uint8_t TOKEN_2(__messagequeue_, name)[MESSAGEQUEUE_SIZE]; \
    MessageQueue_t name = (MessageQueue_t)TOKEN_2(__messagequeue_, name); \

#define DECLARE_MAILBOX(name)    \
    uint8_t TOKEN_2(__mailbox_, name)[MAILBOX_SIZE]; \
    Mailbox_t name = (Mailbox_t)TOKEN_2(__mailbox_, name); \

#define DECLARE_NOTIFY(name)    \
    uint8_t TOKEN_2(__notify_, name)[NOTIFY_SIZE]; \
    Notify_t name = (Notify_t)TOKEN_2(__notify_, name); \

#define DECLARE_EVENTFLAG(name)    \
    uint8_t TOKEN_2(__eventflag_, name)[EVENTFLAG_SIZE]; \
    EventFlag_t name = (EventFlag_t)TOKEN_2(__eventflag_, name); \

//---------------------------------------------------------------------------
// API declarations

//---------------------------------------------------------------------------
// Allocate-once Memory managment APIs
#if defined KERNEL_USE_AUTO_ALLOC
    void *AutoAlloc( uint16_t u16Size_ );
# if KERNEL_USE_SEMAPHORE
    Semaphore_t Alloc_Semaphore(void);
# endif
# if KERNEL_USE_MUTEX
    Mutex_t Alloc_Mutex(void);
# endif
# if KERNEL_USE_EVENTFLAG
    EventFlag_t Alloc_EventFlag(void);
# endif
# if KERNEL_USE_MESSAGE
    Message_t Alloc_Message(void);
    MessageQueue_t Alloc_MessageQueue(void);
# endif
# if KERNEL_USE_NOTIFY
    Notify_t Alloc_Notify(void);
# endif
# if KERNEL_USE_MAILBOX
    Mailbox_t Alloc_Mailbox(void);
# endif
    Thread_t Alloc_Thread(void);
# if KERNEL_USE_TIMERS
    Timer_t Alloc_Timer(void);
# endif
#endif

//---------------------------------------------------------------------------
// Kernel APIs
void Kernel_Init(void);

void Kernel_Start(void);

bool Kernel_IsStarted(void);

void Kernel_SetPanic(panic_func_t pfPanic_);

bool Kernel_IsPanic(void);

void Kernel_Panic(uint16_t u16Cause_);

#if KERNEL_USE_IDLE_FUNC
void Kernel_SetIdleFunc(idle_func_t pfIdle_);
#endif

//---------------------------------------------------------------------------
// Scheduler APIs
void Scheduler_Enable(bool bEnable_);

bool Scheduler_IsEnabled(void);

Thread_t Scheduler_GetCurrentThread(void);

//---------------------------------------------------------------------------
// Thread APIs

void Thread_Init(Thread_t handle,
                 K_WORD *pwStack_,
                 uint16_t u16StackSize_,
                 PRIO_TYPE uXPriority_,
                 ThreadEntry_t pfEntryPoint_,
                 void *pvArg_);

void Thread_Start(Thread_t handle);

void Thread_Stop(Thread_t handle);

#if KERNEL_USE_THREADNAME
void Thread_SetName(Thread_t handle, const char *szName_);
const char* Thread_GetName(Thread_t handle);
#endif

PRIO_TYPE   Thread_GetPriority(Thread_t handle);

PRIO_TYPE   Thread_GetCurPriority(Thread_t handle);

#if KERNEL_USE_QUANTUM
void Thread_SetQuantum(Thread_t handle, uint16_t u16Quantum_);
uint16_t Thread_GetQuantum(Thread_t handle);
#endif

void Thread_SetPriority(Thread_t handle, PRIO_TYPE uXPriority_);

#if KERNEL_USE_DYNAMIC_THREADS
void Thread_Exit(Thread_t handle);
#endif

#if KERNEL_USE_SLEEP
void Thread_Sleep(uint32_t u32TimeMs_);
void Thread_USleep(uint32_t u32TimeUs_);
#endif

void Thread_Yield(void);

void Thread_SetID(Thread_t handle, uint8_t u8ID_);

uint8_t Thread_GetID(Thread_t handle);

uint16_t Thread_GetStackSlack(Thread_t handle);

ThreadState_t Thread_GetState(Thread_t handle);

//---------------------------------------------------------------------------
// Timer APIs
#if KERNEL_USE_TIMERS
typedef void (*TimerCallbackC_t)(Thread_t hOwner_, void *pvData_);

void Timer_Init(Timer_t handle);

void Timer_Start(Timer_t handle, bool bRepeat_, uint32_t u32IntervalMs_, uint32_t u32ToleranceMs_, TimerCallbackC_t pfCallback_, void *pvData_ );

void Timer_Stop(Timer_t handle);

#endif

//---------------------------------------------------------------------------
// Semaphore APIs
#if KERNEL_USE_SEMAPHORE
void Semaphore_Init(Semaphore_t handle, uint16_t u16InitVal_, uint16_t u16MaxVal_);

void Semaphore_Post(Semaphore_t handle);

void Semaphore_Pend(Semaphore_t handle);

# if KERNEL_USE_TIMEOUTS
bool Semaphore_TimedPend(Semaphore_t handle, uint32_t u32WaitTimeMS_);
# endif
#endif

//---------------------------------------------------------------------------
// Mutex APIs
#if KERNEL_USE_MUTEX
void Mutex_Init(Mutex_t handle);

void Mutex_Claim(Mutex_t handle);

void Mutex_Release(Mutex_t handle);

# if KERNEL_USE_TIMEOUTS
bool Mutex_TimedClaim(Mutex_t handle, uint32_t u32WaitTimeMS_);
# endif
#endif

//---------------------------------------------------------------------------
// EventFlag APIs
#if KERNEL_USE_EVENTFLAG
    void EventFlag_Init(EventFlag_t handle);

    uint16_t EventFlag_Wait(EventFlag_t handle, uint16_t u16Mask_, EventFlagOperation_t eMode_);

# if KERNEL_USE_TIMEOUTS
    uint16_t EventFlag_TimedWait(EventFlag_t handle, uint16_t u16Mask_, EventFlagOperation_t eMode_, uint32_t u32TimeMS_);
# endif

    void EventFlag_Set(EventFlag_t handle, uint16_t u16Mask_);

    void EventFlag_Clear(EventFlag_t handle, uint16_t u16Mask_);

    uint16_t EventFlag_GetMask(EventFlag_t handle);
#endif

//---------------------------------------------------------------------------
// Notification APIs
#if KERNEL_USE_NOTIFY
void Notify_Init(Notify_t handle);

void Notify_Signal(Notify_t handle);

void Notify_Wait(Notify_t handle, bool *pbFlag_);

# if KERNEL_USE_TIMEOUTS
bool Notify_TimedWait(Notify_t handle, uint32_t u32WaitTimeMS_, bool *pbFlag_);
# endif
#endif

//---------------------------------------------------------------------------
// Atomic Functions
#if KERNEL_USE_ATOMIC
uint8_t Atomic_Set8( uint8_t *pu8Source_, uint8_t u8Val_ );
uint16_t Atomic_Set16( uint16_t *pu16Source_, uint16_t u16Val_ );
uint32_t Atomic_Set32( uint32_t *pu32Source_, uint32_t u32Val_ );

uint8_t Atomic_Add8( uint8_t *pu8Source_, uint8_t u8Val_ );
uint16_t Atomic_Add16( uint16_t *pu16Source_, uint16_t u16Val_ );
uint32_t Atomic_Add32( uint32_t *pu32Source_, uint32_t u32Val_ );

uint8_t Atomic_Sub8( uint8_t *pu8Source_, uint8_t u8Val_ );
uint16_t Atomic_Sub16( uint16_t *pu16Source_, uint16_t u16Val_ );
uint32_t Atomic_Sub32( uint32_t *pu32Source_, uint32_t u32Val_ );

bool Atomic_TestAndSet( bool *pbLock );
#endif

//---------------------------------------------------------------------------
// Message/Message Queue APIs
#if KERNEL_USE_MESSAGE
    void Message_Init(Message_t handle);
    void Message_SetData(Message_t handle, void *pvData_);
    void* Message_GetData(Message_t handle);
    void Message_SetCode(Message_t handle, uint16_t u16Code_);
    uint16_t Message_GetCode(Message_t handle);

    void GlobalMessagePool_Push(Message_t handle);
    Message_t GlobalMessagePool_Pop(void);

    void MessageQueue_Init(MessageQueue_t handle);
    Message_t MessageQueue_Receive(MessageQueue_t handle);
# if KERNEL_USE_TIMEOUTS
    Message_t MessageQueue_TimedReceive(MessageQueue_t handle, uint32_t u32TimeWaitMS_);
# endif
    void MessageQueue_Send(MessageQueue_t handle, Message_t hMessage_);
    uint16_t MessageQueue_GetCount(void);
#endif

//---------------------------------------------------------------------------
// Mailbox APIs
#if KERNEL_USE_MAILBOX
void Mailbox_Init(Mailbox_t handle, void *pvBuffer_, uint16_t u16BufferSize_, uint16_t u16ElementSize_ );
bool Mailbox_Send(Mailbox_t handle, void *pvData_);
bool Mailbox_SendTail(Mailbox_t handle, void *pvData_);
bool Mailbox_TimedSend(Mailbox_t handle, void *pvData_, uint32_t u32TimeoutMS_);
bool Mailbox_TimedSendTail(Mailbox_t handle, void *pvData_, uint32_t u32TimeoutMS_);
void Mailbox_Receive(Mailbox_t handle, void *pvData_);
void Mailbox_ReceiveTail(Mailbox_t handle, void *pvData_);
# if KERNEL_USE_TIMEOUTS
bool Mailbox_TimedReceive(Mailbox_t handle, void *pvData_, uint32_t u32TimeoutMS_ );
bool Mailbox_TimedReceiveTail(Mailbox_t handle, void *pvData_, uint32_t u32TimeoutMS_ );
uint16_t Mailbox_GetFreeSlots(Mailbox_t handle);
bool Mailbox_IsFull(Mailbox_t handle);
bool Mailbox_IsEmpty(Mailbox_t handle);
# endif
#endif

//---------------------------------------------------------------------------
// Kernel-Aware Simulation APIs
#if KERNEL_AWARE_SIMULATION
void KernelAware_ProfileInit( const char *szStr_ );

void KernelAware_ProfileStart( void );

void KernelAware_ProfileStop( void );

void KernelAware_ProfileReport( void );

void KernelAware_ExitSimulator( void );

void KernelAware_Print( const char *szStr_ );
void KernelAware_Trace( uint16_t u16File_,
                        uint16_t u16Line_);

void KernelAware_Trace1( uint16_t u16File_,
                         uint16_t u16Line_,
                         uint16_t u16Arg1_);

void KernelAware_Trace2( uint16_t u16File_,
                         uint16_t u16Line_,
                         uint16_t u16Arg1_,
                         uint16_t u16Arg2_);

bool KernelAware_IsSimulatorAware(void);
#endif

#if defined(__cplusplus)
}
#endif

#endif // __MARK3C_H__
