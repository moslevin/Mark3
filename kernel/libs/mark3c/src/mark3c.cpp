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
===========================================================================*/
/*!

    \file   mark3c.h

    \brief  Implementation of C-language wrappers for the Mark3 kernel.
*/

#include "mark3c.h"
#include "mark3.h"

using namespace Mark3;

extern "C" {
void __cxa_pure_virtual(void){}
}
//---------------------------------------------------------------------------
// Kernel Memory managment APIs
#if KERNEL_USE_AUTO_ALLOC
//---------------------------------------------------------------------------
void* Alloc_Memory(uint16_t u16Size_)
{
    return AutoAlloc::NewRawData(u16Size_);
}
//---------------------------------------------------------------------------
void Free_Memory(void* pvObject_)
{
    AutoAlloc::DestroyRawData(pvObject_);
}

#if KERNEL_USE_SEMAPHORE
//---------------------------------------------------------------------------
Semaphore_t Alloc_Semaphore(void)
{
    return (Semaphore_t)AutoAlloc::NewSemaphore();
}

//---------------------------------------------------------------------------
void Free_Semaphore(Semaphore_t handle)
{
    AutoAlloc::DestroySemaphore((Semaphore*)handle);
}

#endif
#if KERNEL_USE_MUTEX
//---------------------------------------------------------------------------
Mutex_t Alloc_Mutex(void)
{
    return (Mutex_t)AutoAlloc::NewMutex();
}

//---------------------------------------------------------------------------
void Free_Mutex(Mutex_t handle)
{
    AutoAlloc::DestroyMutex((Mutex*)handle);
}

#endif
#if KERNEL_USE_EVENTFLAG
//---------------------------------------------------------------------------
EventFlag_t Alloc_EventFlag(void)
{
    return (EventFlag_t)AutoAlloc::NewEventFlag();
}

//---------------------------------------------------------------------------
void Free_EventFlag(EventFlag_t handle)
{
    AutoAlloc::DestroyEventFlag((EventFlag*)handle);
}

#endif
#if KERNEL_USE_MESSAGE
//---------------------------------------------------------------------------
Message_t Alloc_Message(void)
{
    return (Message_t)AutoAlloc::NewMessage();
}

//---------------------------------------------------------------------------
void Free_Message(Message_t handle)
{
    AutoAlloc::DestroyMessage((Message*)handle);
}

//---------------------------------------------------------------------------
MessageQueue_t Alloc_MessageQueue(void)
{
    return (MessageQueue_t)AutoAlloc::NewMessageQueue();
}

//---------------------------------------------------------------------------
void Free_MessageQueue(MessageQueue_t handle)
{
    AutoAlloc::DestroyMessageQueue((MessageQueue*)handle);
}

//---------------------------------------------------------------------------
MessagePool_t Alloc_MessagePool(void)
{
    return (MessagePool_t)AutoAlloc::NewMessagePool();
}

//---------------------------------------------------------------------------
void Free_MessagePool(MessagePool_t handle)
{
    AutoAlloc::DestroyMessagePool((MessagePool*)handle);
}

#endif
#if KERNEL_USE_NOTIFY
//---------------------------------------------------------------------------
Notify_t Alloc_Notify(void)
{
    return (Notify_t)AutoAlloc::NewNotify();
}

//---------------------------------------------------------------------------
void Free_Notify(Notify_t handle)
{
    AutoAlloc::DestroyNotify((Notify*)handle);
}

#endif
#if KERNEL_USE_MAILBOX
//---------------------------------------------------------------------------
Mailbox_t Alloc_Mailbox(void)
{
    return (Mailbox_t)AutoAlloc::NewMailbox();
}
//---------------------------------------------------------------------------
void Free_Mailbox(Mailbox_t handle)
{
    AutoAlloc::DestroyMailbox((Mailbox*)handle);
}
#endif
#if KERNEL_USE_CONDVAR
//---------------------------------------------------------------------------
ConditionVariable_t Alloc_ConditionVariable(void)
{
    return (ConditionVariable_t)AutoAlloc::NewConditionVariable();
}
//---------------------------------------------------------------------------
void Free_ConditionVariable(ConditionVariable_t handle)
{
    AutoAlloc::DestroyConditionVariable((ConditionVariable*)handle);
}

#endif
#if KERNEL_USE_READERWRITER
//---------------------------------------------------------------------------
ReaderWriterLock_t Alloc_ReaderWriterLock(void)
{
    return (ReaderWriterLock_t)AutoAlloc::NewReaderWriterLock();
}
//---------------------------------------------------------------------------
void Free_ReaderWriterLock(ReaderWriterLock_t handle)
{
    AutoAlloc::DestroyReaderWriterLock((ReaderWriterLock*)handle);
}

#endif
//---------------------------------------------------------------------------
Thread_t Alloc_Thread(void)
{
    return (Thread_t)AutoAlloc::NewThread();
}
//---------------------------------------------------------------------------
void Free_Thread(Thread_t handle)
{
    AutoAlloc::DestroyThread((Thread*)handle);
}

#if KERNEL_USE_TIMERS
//---------------------------------------------------------------------------
Timer_t Alloc_Timer(void)
{
    return (Thread_t)AutoAlloc::NewTimer();
}
//---------------------------------------------------------------------------
void Free_Timer(Timer_t handle)
{
    AutoAlloc::DestroyTimer((Timer*)handle);
}

#endif
#endif

//---------------------------------------------------------------------------
// Kernel APIs
//---------------------------------------------------------------------------
void Kernel_Init(void)
{
    Kernel::Init();
}

//---------------------------------------------------------------------------
void Kernel_Start(void)
{
    Kernel::Start();
}

//---------------------------------------------------------------------------
bool Kernel_IsStarted(void)
{
    return Kernel::IsStarted();
}

//---------------------------------------------------------------------------
void Kernel_SetPanic(PanicFunc pfPanic_)
{
    Kernel::SetPanic(pfPanic_);
}

//---------------------------------------------------------------------------
bool Kernel_IsPanic(void)
{
    return Kernel::IsPanic();
}

//---------------------------------------------------------------------------
void Kernel_Panic(uint16_t u16Cause_)
{
    Kernel::Panic(u16Cause_);
}

#if KERNEL_USE_IDLE_FUNC
//---------------------------------------------------------------------------
void Kernel_SetIdleFunc(IdleFunc pfIdle_)
{
    Kernel::SetIdleFunc(pfIdle_);
}
#endif

#if KERNEL_USE_THREAD_CALLOUTS
//---------------------------------------------------------------------------
void Kernel_SetThreadCreateCallout(thread_create_callout_t pfCreate_)
{
    Kernel::SetThreadCreateCallout((ThreadCreateCallout)pfCreate_);
}

//---------------------------------------------------------------------------
void Kernel_SetThreadExitCallout(thread_exit_callout_t pfExit_)
{
    Kernel::SetThreadExitCallout((ThreadExitCallout)pfExit_);
}

//---------------------------------------------------------------------------
void Kernel_SetThreadContextSwitchCallout(thread_context_callout_t pfContext_)
{
    Kernel::SetThreadContextSwitchCallout((ThreadContextCallout)pfContext_);
}

//---------------------------------------------------------------------------
thread_create_callout_t Kernel_GetThreadCreateCallout(void)
{
    return (thread_create_callout_t)Kernel::GetThreadCreateCallout();
}

//---------------------------------------------------------------------------
thread_exit_callout_t Kernel_GetThreadExitCallout(void)
{
    return (thread_exit_callout_t)Kernel::GetThreadExitCallout();
}

//---------------------------------------------------------------------------
thread_context_callout_t Kernel_GetThreadContextSwitchCallout(void)
{
    return (thread_context_callout_t)Kernel::GetThreadContextSwitchCallout();
}

#endif

#if KERNEL_USE_STACK_GUARD
//---------------------------------------------------------------------------
void Kernel_SetStackGuardThreshold(uint16_t u16Threshold_)
{
    Kernel::SetStackGuardThreshold(u16Threshold_);
}

//---------------------------------------------------------------------------
uint16_t Kernel_GetStackGuardThreshold(void)
{
    return Kernel::GetStackGuardThreshold();
}

#endif

//---------------------------------------------------------------------------
// Scheduler APIs
void Scheduler_Enable(bool bEnable_)
{
    Scheduler::SetScheduler(bEnable_);
}

//---------------------------------------------------------------------------
bool Scheduler_IsEnabled(void)
{
    return Scheduler::IsEnabled();
}

//---------------------------------------------------------------------------
Thread_t Scheduler_GetCurrentThread(void)
{
    return (Thread_t)Scheduler::GetCurrentThread();
}

//---------------------------------------------------------------------------
// Thread APIs
//---------------------------------------------------------------------------

void Thread_Init(Thread_t      handle,
                 K_WORD*       pwStack_,
                 uint16_t      u16StackSize_,
                 PORT_PRIO_TYPE     uXPriority_,
                 ThreadEntryFunc pfEntryPoint_,
                 void*         pvArg_)
{
    Thread* pclThread = new ((void*)handle) Thread();
    pclThread->Init(pwStack_, u16StackSize_, uXPriority_, pfEntryPoint_, pvArg_);
}

//---------------------------------------------------------------------------
void Thread_Start(Thread_t handle)
{
    Thread* pclThread = (Thread*)handle;
    pclThread->Start();
}

//---------------------------------------------------------------------------
void Thread_Stop(Thread_t handle)
{
    Thread* pclThread = (Thread*)handle;
    pclThread->Stop();
}
#if KERNEL_USE_THREADNAME
//---------------------------------------------------------------------------
void Thread_SetName(Thread_t handle, const char* szName_)
{
    Thread* pclThread = (Thread*)handle;
    pclThread->SetName(szName_);
}
//---------------------------------------------------------------------------
const char* Thread_GetName(Thread_t handle)
{
    Thread* pclThread = (Thread*)handle;
    return pclThread->GetName();
}
#endif

//---------------------------------------------------------------------------
PORT_PRIO_TYPE Thread_GetPriority(Thread_t handle)
{
    Thread* pclThread = (Thread*)handle;
    return pclThread->GetPriority();
}
//---------------------------------------------------------------------------
PORT_PRIO_TYPE Thread_GetCurPriority(Thread_t handle)
{
    Thread* pclThread = (Thread*)handle;
    return pclThread->GetCurPriority();
}
#if KERNEL_USE_QUANTUM
//---------------------------------------------------------------------------
void Thread_SetQuantum(Thread_t handle, uint16_t u16Quantum_)
{
    Thread* pclThread = (Thread*)handle;
    pclThread->SetQuantum(u16Quantum_);
}
//---------------------------------------------------------------------------
uint16_t Thread_GetQuantum(Thread_t handle)
{
    Thread* pclThread = (Thread*)handle;
    return pclThread->GetQuantum();
}
#endif

//---------------------------------------------------------------------------
void Thread_SetPriority(Thread_t handle, PORT_PRIO_TYPE uXPriority_)
{
    Thread* pclThread = (Thread*)handle;
    pclThread->SetPriority(uXPriority_);
}

#if KERNEL_USE_DYNAMIC_THREADS
//---------------------------------------------------------------------------
void Thread_Exit(Thread_t handle)
{
    Thread* pclThread = (Thread*)handle;
    pclThread->Exit();
}
#endif

#if KERNEL_USE_SLEEP
//---------------------------------------------------------------------------
void Thread_Sleep(uint32_t u32TimeMs_)
{
    Thread::Sleep(u32TimeMs_);
}

//---------------------------------------------------------------------------
void Thread_USleep(uint32_t u32TimeUs_)
{
    Thread::USleep(u32TimeUs_);
}
#endif

#if KERNEL_USE_EXTENDED_CONTEXT
//---------------------------------------------------------------------------
void* Thread_GetExtendedContext(Thread_t handle)
{
    Thread* pclThread = (Thread*)handle;
    return pclThread->GetExtendedContext();
}

//---------------------------------------------------------------------------
void Thread_SetExtendedContext(Thread_t handle, void* pvData_)
{
    Thread* pclThread = (Thread*)handle;
    pclThread->SetExtendedContext(pvData_);
}

#endif

//---------------------------------------------------------------------------
void Thread_Yield(void)
{
    Thread::Yield();
}
//---------------------------------------------------------------------------
void Thread_SetID(Thread_t handle, uint8_t u8ID_)
{
    Thread* pclThread = (Thread*)handle;
    pclThread->SetID(u8ID_);
}
//---------------------------------------------------------------------------
uint8_t Thread_GetID(Thread_t handle)
{
    Thread* pclThread = (Thread*)handle;
    return pclThread->GetID();
}
//---------------------------------------------------------------------------
uint16_t Thread_GetStackSlack(Thread_t handle)
{
    Thread* pclThread = (Thread*)handle;
    return pclThread->GetStackSlack();
}
//---------------------------------------------------------------------------
thread_state_t Thread_GetState(Thread_t handle)
{
    Thread* pclThread = (Thread*)handle;
    return (thread_state_t)pclThread->GetState();
}
//---------------------------------------------------------------------------
// Timer APIs
//---------------------------------------------------------------------------
#if KERNEL_USE_TIMERS

//---------------------------------------------------------------------------
void Timer_Init(Timer_t handle)
{
    Timer* pclTimer = new ((void*)handle) Timer();
    pclTimer->Init();
}

//---------------------------------------------------------------------------
void Timer_Start(Timer_t          handle,
                 bool             bRepeat_,
                 uint32_t         u32IntervalMs_,
                 uint32_t         u32ToleranceMs_,
                 timer_callback_t pfCallback_,
                 void*            pvData_)
{
    Timer* pclTimer = (Timer*)handle;
    pclTimer->Start(bRepeat_, u32IntervalMs_, u32ToleranceMs_, (TimerCallback)pfCallback_, pvData_);
}

//---------------------------------------------------------------------------
void Timer_Stop(Timer_t handle)
{
    Timer* pclTimer = (Timer*)handle;
    pclTimer->Stop();
}

//---------------------------------------------------------------------------
void Timer_Restart(Timer_t handle)
{
    Timer* pclTimer = (Timer*)handle;
    pclTimer->Start();
}

#endif

//---------------------------------------------------------------------------
// Semaphore APIs
//---------------------------------------------------------------------------
#if KERNEL_USE_SEMAPHORE
void Semaphore_Init(Semaphore_t handle, uint16_t u16InitVal_, uint16_t u16MaxVal_)
{
    Semaphore* pclSemaphore = new ((void*)handle) Semaphore();
    pclSemaphore->Init(u16InitVal_, u16MaxVal_);
}

//---------------------------------------------------------------------------
void Semaphore_Post(Semaphore_t handle)
{
    Semaphore* pclSemaphore = (Semaphore*)handle;
    pclSemaphore->Post();
}

//---------------------------------------------------------------------------
void Semaphore_Pend(Semaphore_t handle)
{
    Semaphore* pclSemaphore = (Semaphore*)handle;
    pclSemaphore->Pend();
}

#if KERNEL_USE_TIMEOUTS
//---------------------------------------------------------------------------
bool Semaphore_TimedPend(Semaphore_t handle, uint32_t u32WaitTimeMS_)
{
    Semaphore* pclSemaphore = (Semaphore*)handle;
    return pclSemaphore->Pend(u32WaitTimeMS_);
}
#endif
#endif

//---------------------------------------------------------------------------
// Mutex APIs
#if KERNEL_USE_MUTEX
//---------------------------------------------------------------------------
void Mutex_Init(Mutex_t handle)
{
    Mutex* pclMutex = new ((void*)handle) Mutex();
    pclMutex->Init();
}

//---------------------------------------------------------------------------
void Mutex_Claim(Mutex_t handle)
{
    Mutex* pclMutex = (Mutex*)handle;
    pclMutex->Claim();
}

//---------------------------------------------------------------------------
void Mutex_Release(Mutex_t handle)
{
    Mutex* pclMutex = (Mutex*)handle;
    pclMutex->Release();
}

#if KERNEL_USE_TIMEOUTS
//---------------------------------------------------------------------------
bool Mutex_TimedClaim(Mutex_t handle, uint32_t u32WaitTimeMS_)
{
    Mutex* pclMutex = (Mutex*)handle;
    return pclMutex->Claim(u32WaitTimeMS_);
}

#endif
#endif

//---------------------------------------------------------------------------
// EventFlag APIs
//---------------------------------------------------------------------------
#if KERNEL_USE_EVENTFLAG
void EventFlag_Init(EventFlag_t handle)
{
    EventFlag* pclFlag = new ((void*)handle) EventFlag();
    pclFlag->Init();
}

//---------------------------------------------------------------------------
uint16_t EventFlag_Wait(EventFlag_t handle, uint16_t u16Mask_, event_flag_operation_t eMode_)
{
    EventFlag* pclFlag = (EventFlag*)handle;
    return pclFlag->Wait(u16Mask_, (EventFlagOperation)eMode_);
}

#if KERNEL_USE_TIMEOUTS
//---------------------------------------------------------------------------
uint16_t EventFlag_TimedWait(EventFlag_t handle, uint16_t u16Mask_, event_flag_operation_t eMode_, uint32_t u32TimeMS_)
{
    EventFlag* pclFlag = (EventFlag*)handle;
    return pclFlag->Wait(u16Mask_, (EventFlagOperation)eMode_, u32TimeMS_);
}
#endif

//---------------------------------------------------------------------------
void EventFlag_Set(EventFlag_t handle, uint16_t u16Mask_)
{
    EventFlag* pclFlag = (EventFlag*)handle;
    pclFlag->Set(u16Mask_);
}

//---------------------------------------------------------------------------
void EventFlag_Clear(EventFlag_t handle, uint16_t u16Mask_)
{
    EventFlag* pclFlag = (EventFlag*)handle;
    pclFlag->Clear(u16Mask_);
}

//---------------------------------------------------------------------------
uint16_t EventFlag_GetMask(EventFlag_t handle)
{
    EventFlag* pclFlag = (EventFlag*)handle;
    return pclFlag->GetMask();
}

#endif

//---------------------------------------------------------------------------
// Notification APIs
//---------------------------------------------------------------------------
#if KERNEL_USE_NOTIFY
void Notify_Init(Notify_t handle)
{
    Notify* pclNotify = new ((void*)handle) Notify();
    pclNotify->Init();
}

//---------------------------------------------------------------------------
void Notify_Signal(Notify_t handle)
{
    Notify* pclNotify = (Notify*)handle;
    pclNotify->Signal();
}

//---------------------------------------------------------------------------
void Notify_Wait(Notify_t handle, bool* pbFlag_)
{
    Notify* pclNotify = (Notify*)handle;
    pclNotify->Wait(pbFlag_);
}

#if KERNEL_USE_TIMEOUTS
//---------------------------------------------------------------------------
bool Notify_TimedWait(Notify_t handle, uint32_t u32WaitTimeMS_, bool* pbFlag_)
{
    Notify* pclNotify = (Notify*)handle;
    return pclNotify->Wait(u32WaitTimeMS_, pbFlag_);
}

#endif
#endif

//---------------------------------------------------------------------------
// Atomic Functions
//---------------------------------------------------------------------------
#if KERNEL_USE_ATOMIC
uint8_t Atomic_Set8(uint8_t* pu8Source_, uint8_t u8Val_)
{
    return Atomic::Set(pu8Source_, u8Val_);
}

//---------------------------------------------------------------------------
uint16_t Atomic_Set16(uint16_t* pu16Source_, uint16_t u16Val_)
{
    return Atomic::Set(pu16Source_, u16Val_);
}

//---------------------------------------------------------------------------
uint32_t Atomic_Set32(uint32_t* pu32Source_, uint32_t u32Val_)
{
    return Atomic::Set(pu32Source_, u32Val_);
}

//---------------------------------------------------------------------------
uint8_t Atomic_Add8(uint8_t* pu8Source_, uint8_t u8Val_)
{
    return Atomic::Add(pu8Source_, u8Val_);
}

//---------------------------------------------------------------------------
uint16_t Atomic_Add16(uint16_t* pu16Source_, uint16_t u16Val_)
{
    return Atomic::Add(pu16Source_, u16Val_);
}

//---------------------------------------------------------------------------
uint32_t Atomic_Add32(uint32_t* pu32Source_, uint32_t u32Val_)
{
    return Atomic::Add(pu32Source_, u32Val_);
}

//---------------------------------------------------------------------------
uint8_t Atomic_Sub8(uint8_t* pu8Source_, uint8_t u8Val_)
{
    return Atomic::Sub(pu8Source_, u8Val_);
}

//---------------------------------------------------------------------------
uint16_t Atomic_Sub16(uint16_t* pu16Source_, uint16_t u16Val_)
{
    return Atomic::Sub(pu16Source_, u16Val_);
}

//---------------------------------------------------------------------------
uint32_t Atomic_Sub32(uint32_t* pu32Source_, uint32_t u32Val_)
{
    return Atomic::Sub(pu32Source_, u32Val_);
}

//---------------------------------------------------------------------------
bool Atomic_TestAndSet(bool* pbLock)
{
    return Atomic::TestAndSet(pbLock);
}

#endif

//---------------------------------------------------------------------------
// Message/Message Queue APIs
//---------------------------------------------------------------------------
#if KERNEL_USE_MESSAGE
void Message_Init(Message_t handle)
{
    Message* pclMessage = new ((void*)handle) Message();
    return pclMessage->Init();
}

//---------------------------------------------------------------------------
void Message_SetData(Message_t handle, void* pvData_)
{
    Message* pclMessage = (Message*)handle;
    pclMessage->SetData(pvData_);
}

//---------------------------------------------------------------------------
void* Message_GetData(Message_t handle)
{
    Message* pclMessage = (Message*)handle;
    return pclMessage->GetData();
}

//---------------------------------------------------------------------------
void Message_SetCode(Message_t handle, uint16_t u16Code_)
{
    Message* pclMessage = (Message*)handle;
    pclMessage->SetCode(u16Code_);
}

//---------------------------------------------------------------------------
uint16_t Message_GetCode(Message_t handle)
{
    Message* pclMessage = (Message*)handle;
    return pclMessage->GetCode();
}

//---------------------------------------------------------------------------
void MessageQueue_Init(MessageQueue_t handle)
{
    MessageQueue* pclMsgQ = new ((void*)handle) MessageQueue();
    pclMsgQ->Init();
}

//---------------------------------------------------------------------------
Message_t MessageQueue_Receive(MessageQueue_t handle)
{
    MessageQueue* pclMsgQ = (MessageQueue*)handle;
    return pclMsgQ->Receive();
}

//---------------------------------------------------------------------------
void MessagePool_Init(MessagePool_t handle)
{
    MessagePool* pclMsgPool = new ((void*)handle) MessagePool();
    pclMsgPool->Init();
}

//---------------------------------------------------------------------------
void MessagePool_Push(MessagePool_t handle, Message_t msg)
{
    MessagePool* pclMsgPool = (MessagePool*)handle;
    pclMsgPool->Push((Message*)msg);
}

//---------------------------------------------------------------------------
Message_t MessagePool_Pop(MessagePool_t handle)
{
    MessagePool* pclMsgPool = (MessagePool*)handle;
    return (Message_t)pclMsgPool->Pop();
}
#if KERNEL_USE_TIMEOUTS
//---------------------------------------------------------------------------
Message_t MessageQueue_TimedReceive(MessageQueue_t handle, uint32_t u32TimeWaitMS_)
{
    MessageQueue* pclMsgQ = (MessageQueue*)handle;
    return (Message_t)pclMsgQ->Receive(u32TimeWaitMS_);
}

#endif
//---------------------------------------------------------------------------
void MessageQueue_Send(MessageQueue_t handle, Message_t hMessage_)
{
    MessageQueue* pclMsgQ = (MessageQueue*)handle;
    pclMsgQ->Send((Message*)hMessage_);
}

//---------------------------------------------------------------------------
uint16_t MessageQueue_GetCount(MessageQueue_t handle)
{
    MessageQueue* pclMsgQ = (MessageQueue*)handle;
    return pclMsgQ->GetCount();
}

#endif

//---------------------------------------------------------------------------
// Mailbox APIs
//---------------------------------------------------------------------------
#if KERNEL_USE_MAILBOX
void Mailbox_Init(Mailbox_t handle, void* pvBuffer_, uint16_t u16BufferSize_, uint16_t u16ElementSize_)
{
    Mailbox* pclMBox = new ((void*)handle) Mailbox();
    pclMBox->Init(pvBuffer_, u16BufferSize_, u16ElementSize_);
}

//---------------------------------------------------------------------------
bool Mailbox_Send(Mailbox_t handle, void* pvData_)
{
    Mailbox* pclMBox = (Mailbox*)handle;
    return pclMBox->Send(pvData_);
}

//---------------------------------------------------------------------------
bool Mailbox_SendTail(Mailbox_t handle, void* pvData_)
{
    Mailbox* pclMBox = (Mailbox*)handle;
    return pclMBox->SendTail(pvData_);
}

#if KERNEL_USE_TIMEOUTS
//---------------------------------------------------------------------------
bool Mailbox_TimedSend(Mailbox_t handle, void* pvData_, uint32_t u32TimeoutMS_)
{
    Mailbox* pclMBox = (Mailbox*)handle;
    return pclMBox->Send(pvData_, u32TimeoutMS_);
}

//---------------------------------------------------------------------------
bool Mailbox_TimedSendTail(Mailbox_t handle, void* pvData_, uint32_t u32TimeoutMS_)
{
    Mailbox* pclMBox = (Mailbox*)handle;
    return pclMBox->SendTail(pvData_, u32TimeoutMS_);
}
#endif

//---------------------------------------------------------------------------
void Mailbox_Receive(Mailbox_t handle, void* pvData_)
{
    Mailbox* pclMBox = (Mailbox*)handle;
    pclMBox->Receive(pvData_);
}

//---------------------------------------------------------------------------
void Mailbox_ReceiveTail(Mailbox_t handle, void* pvData_)
{
    Mailbox* pclMBox = (Mailbox*)handle;
    pclMBox->ReceiveTail(pvData_);
}

#if KERNEL_USE_TIMEOUTS
//---------------------------------------------------------------------------
bool Mailbox_TimedReceive(Mailbox_t handle, void* pvData_, uint32_t u32TimeoutMS_)
{
    Mailbox* pclMBox = (Mailbox*)handle;
    return pclMBox->Receive(pvData_, u32TimeoutMS_);
}

//---------------------------------------------------------------------------
bool Mailbox_TimedReceiveTail(Mailbox_t handle, void* pvData_, uint32_t u32TimeoutMS_)
{
    Mailbox* pclMBox = (Mailbox*)handle;
    return pclMBox->ReceiveTail(pvData_, u32TimeoutMS_);
}

//---------------------------------------------------------------------------
uint16_t Mailbox_GetFreeSlots(Mailbox_t handle)
{
    Mailbox* pclMBox = (Mailbox*)handle;
    return pclMBox->GetFreeSlots();
}

//---------------------------------------------------------------------------
bool Mailbox_IsFull(Mailbox_t handle)
{
    Mailbox* pclMBox = (Mailbox*)handle;
    return pclMBox->IsFull();
}

//---------------------------------------------------------------------------
bool Mailbox_IsEmpty(Mailbox_t handle)
{
    Mailbox* pclMBox = (Mailbox*)handle;
    return pclMBox->IsEmpty();
}
#endif
#endif

//---------------------------------------------------------------------------
// Condition Variables
#if KERNEL_USE_CONDVAR
//---------------------------------------------------------------------------
void ConditionVariable_Init(ConditionVariable_t handle)
{
    ConditionVariable* pclCondvar = new ((void*)handle) ConditionVariable();
    pclCondvar->Init();
}
//---------------------------------------------------------------------------
void ConditionVariable_Wait(ConditionVariable_t handle, Mutex_t hMutex_)
{
    ConditionVariable* pclCondvar = (ConditionVariable*)handle;
    Mutex* pclMutex = (Mutex*)hMutex_;
    pclCondvar->Wait(pclMutex);
}
//---------------------------------------------------------------------------
void ConditionVariable_Signal(ConditionVariable_t handle)
{
    ConditionVariable* pclCondvar = (ConditionVariable*)handle;
    pclCondvar->Signal();
}
//---------------------------------------------------------------------------
void ConditionVariable_Broadcast(ConditionVariable_t handle)
{
    ConditionVariable* pclCondvar = (ConditionVariable*)handle;
    pclCondvar->Broadcast();
}
#if KERNEL_USE_TIMEOUTS
//---------------------------------------------------------------------------
bool ConditionVariable_TimedWait(ConditionVariable_t handle, Mutex_t hMutex_, uint32_t u32WaitTimeMS_)
{
    ConditionVariable* pclCondvar = (ConditionVariable*)handle;
    Mutex* pclMutex = (Mutex*)hMutex_;
    return pclCondvar->Wait(pclMutex, u32WaitTimeMS_);
}
#endif
#endif

//---------------------------------------------------------------------------
// Reader-writer locks
#if KERNEL_USE_READERWRITER
//---------------------------------------------------------------------------
void ReaderWriterLock_Init(ReaderWriterLock_t handle)
{
    ReaderWriterLock* pclReaderWriter = new ((void*)handle) ReaderWriterLock();
    pclReaderWriter->Init();
}

//---------------------------------------------------------------------------
void ReaderWriterLock_AcquireReader(ReaderWriterLock_t handle)
{
    ReaderWriterLock* pclReaderWriter = (ReaderWriterLock*)handle;
    pclReaderWriter->AcquireReader();
}

//---------------------------------------------------------------------------
void ReaderWriterLock_ReleaseReader(ReaderWriterLock_t handle)
{
    ReaderWriterLock* pclReaderWriter = (ReaderWriterLock*)handle;
    pclReaderWriter->ReleaseReader();
}

//---------------------------------------------------------------------------
void ReaderWriterLock_AcquireWriter(ReaderWriterLock_t handle)
{
    ReaderWriterLock* pclReaderWriter = (ReaderWriterLock*)handle;
    pclReaderWriter->AcquireWriter();
}

//---------------------------------------------------------------------------
void ReaderWriterLock_ReleaseWriter(ReaderWriterLock_t handle)
{
    ReaderWriterLock* pclReaderWriter = (ReaderWriterLock*)handle;
    pclReaderWriter->ReleaseWriter();
}

#if KERNEL_USE_TIMEOUTS
//---------------------------------------------------------------------------
bool ReaderWriterLock_TimedAcquireWriter(ReaderWriterLock_t handle, uint32_t u32TimeoutMs_)
{
    ReaderWriterLock* pclReaderWriter = (ReaderWriterLock*)handle;
    return pclReaderWriter->AcquireWriter(u32TimeoutMs_);
}

//---------------------------------------------------------------------------
bool ReaderWriterLock_TimedAcquireReader(ReaderWriterLock_t handle, uint32_t u32TimeoutMs_)
{
    ReaderWriterLock* pclReaderWriter = (ReaderWriterLock*)handle;
    return pclReaderWriter->AcquireReader(u32TimeoutMs_);
}
#endif
#endif

#if KERNEL_AWARE_SIMULATION
//---------------------------------------------------------------------------
void KernelAware_ProfileInit(const char* szStr_)
{
    KernelAware::ProfileInit(szStr_);
}

//---------------------------------------------------------------------------
void KernelAware_ProfileStart(void)
{
    KernelAware::ProfileStart();
}

//---------------------------------------------------------------------------
void KernelAware_ProfileStop(void)
{
    KernelAware::ProfileStop();
}

//---------------------------------------------------------------------------
void KernelAware_ProfileReport(void)
{
    KernelAware::ProfileReport();
}

//---------------------------------------------------------------------------
void KernelAware_ExitSimulator(void)
{
    KernelAware::ExitSimulator();
}

//---------------------------------------------------------------------------
void KernelAware_Print(const char* szStr_)
{
    KernelAware::Print(szStr_);
}

//---------------------------------------------------------------------------
void KernelAware_Trace(uint16_t u16File_, uint16_t u16Line_)
{
    KernelAware::Trace(u16File_, u16Line_);
}

//---------------------------------------------------------------------------
void KernelAware_Trace1(uint16_t u16File_, uint16_t u16Line_, uint16_t u16Arg1_)
{
    KernelAware::Trace(u16File_, u16Line_, u16Arg1_);
}

//---------------------------------------------------------------------------
void KernelAware_Trace2(uint16_t u16File_, uint16_t u16Line_, uint16_t u16Arg1_, uint16_t u16Arg2_)
{
    KernelAware::Trace(u16File_, u16Line_, u16Arg1_, u16Arg2_);
}

//---------------------------------------------------------------------------
bool KernelAware_IsSimulatorAware(void)
{
    return KernelAware::IsSimulatorAware();
}

#endif
