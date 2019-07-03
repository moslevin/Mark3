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

    @file   mark3c.cpp

    @brief  Implementation of C-language wrappers for the Mark3 kernel.
*/

#include "mark3c.h"
#include "mark3.h"

using namespace Mark3;

//---------------------------------------------------------------------------
// Kernel Memory managment APIs
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

//---------------------------------------------------------------------------
Semaphore_t Alloc_Semaphore(void)
{
    return static_cast<Semaphore_t>(AutoAlloc::NewObject<Semaphore, AutoAllocType::Semaphore>());
}

//---------------------------------------------------------------------------
void Free_Semaphore(Semaphore_t handle)
{
    AutoAlloc::DestroyObject<Semaphore, AutoAllocType::Semaphore>(static_cast<Semaphore*>(handle));
}

//---------------------------------------------------------------------------
Mutex_t Alloc_Mutex(void)
{
    return static_cast<Mutex_t>(AutoAlloc::NewObject<Mutex, AutoAllocType::Mutex>());
}

//---------------------------------------------------------------------------
void Free_Mutex(Mutex_t handle)
{
    AutoAlloc::DestroyObject<Mutex, AutoAllocType::Mutex>(static_cast<Mutex*>(handle));
}

//---------------------------------------------------------------------------
EventFlag_t Alloc_EventFlag(void)
{
    return static_cast<EventFlag_t>(AutoAlloc::NewObject<EventFlag, AutoAllocType::EventFlag>());
}

//---------------------------------------------------------------------------
void Free_EventFlag(EventFlag_t handle)
{
    AutoAlloc::DestroyObject<EventFlag, AutoAllocType::EventFlag>(static_cast<EventFlag*>(handle));
}

//---------------------------------------------------------------------------
Message_t Alloc_Message(void)
{
    return static_cast<EventFlag_t>(AutoAlloc::NewObject<Message, AutoAllocType::Message>());
}

//---------------------------------------------------------------------------
void Free_Message(Message_t handle)
{
    AutoAlloc::DestroyObject<Message, AutoAllocType::Message>(static_cast<Message*>(handle));
}

//---------------------------------------------------------------------------
MessageQueue_t Alloc_MessageQueue(void)
{
    return static_cast<MessageQueue_t>(AutoAlloc::NewObject<MessageQueue, AutoAllocType::MessageQueue>());
}

//---------------------------------------------------------------------------
void Free_MessageQueue(MessageQueue_t handle)
{
    AutoAlloc::DestroyObject<MessageQueue, AutoAllocType::MessageQueue>(static_cast<MessageQueue*>(handle));
}

//---------------------------------------------------------------------------
MessagePool_t Alloc_MessagePool(void)
{
    return static_cast<MessagePool_t>(AutoAlloc::NewObject<MessagePool, AutoAllocType::MessagePool>());
}

//---------------------------------------------------------------------------
void Free_MessagePool(MessagePool_t handle)
{
    AutoAlloc::DestroyObject<MessagePool, AutoAllocType::MessagePool>(static_cast<MessagePool*>(handle));
}

//---------------------------------------------------------------------------
Notify_t Alloc_Notify(void)
{
    return static_cast<Notify_t>(AutoAlloc::NewObject<Notify, AutoAllocType::Notify>());
}

//---------------------------------------------------------------------------
void Free_Notify(Notify_t handle)
{
    AutoAlloc::DestroyObject<Notify, AutoAllocType::Notify>(static_cast<Notify*>(handle));
}
//---------------------------------------------------------------------------
Mailbox_t Alloc_Mailbox(void)
{
    return static_cast<Mailbox_t>(AutoAlloc::NewObject<Mailbox, AutoAllocType::MailBox>());
}
//---------------------------------------------------------------------------
void Free_Mailbox(Mailbox_t handle)
{
    AutoAlloc::DestroyObject<Mailbox, AutoAllocType::MailBox>(static_cast<Mailbox*>(handle));
}

//---------------------------------------------------------------------------
ConditionVariable_t Alloc_ConditionVariable(void)
{
    return static_cast<ConditionVariable_t>(AutoAlloc::NewObject<ConditionVariable, AutoAllocType::ConditionVariable>());
}
//---------------------------------------------------------------------------
void Free_ConditionVariable(ConditionVariable_t handle)
{
    AutoAlloc::DestroyObject<ConditionVariable, AutoAllocType::ConditionVariable>(static_cast<ConditionVariable*>(handle));
}

//---------------------------------------------------------------------------
ReaderWriterLock_t Alloc_ReaderWriterLock(void)
{
    return static_cast<ReaderWriterLock_t>(AutoAlloc::NewObject<ReaderWriterLock, AutoAllocType::ReaderWriterLock>());
}
//---------------------------------------------------------------------------
void Free_ReaderWriterLock(ReaderWriterLock_t handle)
{
    AutoAlloc::DestroyObject<ReaderWriterLock, AutoAllocType::ReaderWriterLock>(static_cast<ReaderWriterLock*>(handle));
}

//---------------------------------------------------------------------------
Thread_t Alloc_Thread(void)
{
    return static_cast<Thread_t>(AutoAlloc::NewObject<Thread, AutoAllocType::Thread>());
}
//---------------------------------------------------------------------------
void Free_Thread(Thread_t handle)
{
    AutoAlloc::DestroyObject<Thread, AutoAllocType::Thread>(static_cast<Thread*>(handle));
}

//---------------------------------------------------------------------------
Timer_t Alloc_Timer(void)
{
    return static_cast<Timer_t>(AutoAlloc::NewObject<Timer, AutoAllocType::Timer>());
}
//---------------------------------------------------------------------------
void Free_Timer(Timer_t handle)
{
    AutoAlloc::DestroyObject<Timer, AutoAllocType::Timer>(static_cast<Timer*>(handle));
}

//---------------------------------------------------------------------------
Coroutine_t Alloc_Coroutine(void)
{
    return static_cast<Coroutine_t>(AutoAlloc::NewObject<Coroutine, AutoAllocType::Coroutine>());
}
//---------------------------------------------------------------------------
void Free_Coroutine(Coroutine_t handle)
{
    AutoAlloc::DestroyObject<Coroutine, AutoAllocType::Coroutine>(static_cast<Coroutine*>(handle));
}

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
void Kernel_SetPanic(panic_func_t pfPanic_)
{
    Kernel::SetPanic(static_cast<PanicFunc>(pfPanic_));
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

//---------------------------------------------------------------------------
uint32_t Kernel_GetTicks(void)
{
    return Kernel::GetTicks();
}

#if KERNEL_THREAD_CREATE_CALLOUT
//---------------------------------------------------------------------------
void Kernel_SetThreadCreateCallout(thread_create_callout_t pfCreate_)
{
    Kernel::SetThreadCreateCallout(reinterpret_cast<ThreadCreateCallout>(pfCreate_));
}
#endif // #if KERNEL_THREAD_CREATE_CALLOUT
//---------------------------------------------------------------------------
#if KERNEL_THREAD_EXIT_CALLOUT
void Kernel_SetThreadExitCallout(thread_exit_callout_t pfExit_)
{
    Kernel::SetThreadExitCallout(reinterpret_cast<ThreadExitCallout>(pfExit_));
}
#endif //#if KERNEL_THREAD_EXIT_CALLOUT
#if KERNEL_CONTEXT_SWITCH_CALLOUT
//---------------------------------------------------------------------------
void Kernel_SetThreadContextSwitchCallout(thread_context_callout_t pfContext_)
{
    Kernel::SetThreadContextSwitchCallout(reinterpret_cast<ThreadContextCallout>(pfContext_));
}
#endif // #if KERNEL_CONTEXT_SWITCH_CALLOUT
#if KERNEL_THREAD_CREATE_CALLOUT
//---------------------------------------------------------------------------
thread_create_callout_t Kernel_GetThreadCreateCallout(void)
{
    return reinterpret_cast<thread_create_callout_t>(Kernel::GetThreadCreateCallout());
}
#endif // #if KERNEL_THREAD_CREATE_CALLOUT
#if KERNEL_THREAD_EXIT_CALLOUT
//---------------------------------------------------------------------------
thread_exit_callout_t Kernel_GetThreadExitCallout(void)
{
    return reinterpret_cast<thread_exit_callout_t>(Kernel::GetThreadExitCallout());
}
#endif // #if KERNEL_THREAD_EXIT_CALLOUT
#if KERNEL_CONTEXT_SWITCH_CALLOUT
//---------------------------------------------------------------------------
thread_context_callout_t Kernel_GetThreadContextSwitchCallout(void)
{
    return reinterpret_cast<thread_context_callout_t>(Kernel::GetThreadContextSwitchCallout());
}
#endif // #if KERNEL_CONTEXT_SWITCH_CALLOUT
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

//---------------------------------------------------------------------------
void Kernel_SetDebugPrintFunction(kernel_debug_print_t pfPrintFunction_)
{
    Kernel::SetDebugPrintFunction(pfPrintFunction_);
}

//---------------------------------------------------------------------------
void Kernel_DebugPrint(const char* szString_)
{
    Kernel::DebugPrint(szString_);
}

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
    return static_cast<Thread_t>(Scheduler::GetCurrentThread());
}

//---------------------------------------------------------------------------
// Thread APIs
//---------------------------------------------------------------------------

void Thread_Init(Thread_t        handle,
                 K_WORD*         pwStack_,
                 uint16_t        u16StackSize_,
                 PORT_PRIO_TYPE  uXPriority_,
                 ThreadEntryFunc pfEntryPoint_,
                 void*           pvArg_)
{
    auto* pclThread = new ((void*)handle) Thread();
    pclThread->Init(pwStack_, u16StackSize_, uXPriority_, pfEntryPoint_, pvArg_);
}

//---------------------------------------------------------------------------
void Thread_Start(Thread_t handle)
{
    auto* pclThread = static_cast<Thread*>(handle);
    pclThread->Start();
}

//---------------------------------------------------------------------------
void Thread_Stop(Thread_t handle)
{
    auto* pclThread = static_cast<Thread*>(handle);
    pclThread->Stop();
}
//---------------------------------------------------------------------------
void Thread_SetName(Thread_t handle, const char* szName_)
{
    auto* pclThread = static_cast<Thread*>(handle);
    pclThread->SetName(szName_);
}
//---------------------------------------------------------------------------
const char* Thread_GetName(Thread_t handle)
{
    auto* pclThread = static_cast<Thread*>(handle);
    return pclThread->GetName();
}

//---------------------------------------------------------------------------
PORT_PRIO_TYPE Thread_GetPriority(Thread_t handle)
{
    auto* pclThread = static_cast<Thread*>(handle);
    return pclThread->GetPriority();
}
//---------------------------------------------------------------------------
PORT_PRIO_TYPE Thread_GetCurPriority(Thread_t handle)
{
    auto* pclThread = static_cast<Thread*>(handle);
    return pclThread->GetCurPriority();
}

//---------------------------------------------------------------------------
void Thread_SetQuantum(Thread_t handle, uint16_t u16Quantum_)
{
    auto* pclThread = static_cast<Thread*>(handle);
    pclThread->SetQuantum(u16Quantum_);
}
//---------------------------------------------------------------------------
uint16_t Thread_GetQuantum(Thread_t handle)
{
    auto* pclThread = static_cast<Thread*>(handle);
    return pclThread->GetQuantum();
}

//---------------------------------------------------------------------------
void Thread_SetPriority(Thread_t handle, PORT_PRIO_TYPE uXPriority_)
{
    auto* pclThread = static_cast<Thread*>(handle);
    pclThread->SetPriority(uXPriority_);
}

//---------------------------------------------------------------------------
void Thread_Exit(Thread_t handle)
{
    auto* pclThread = static_cast<Thread*>(handle);
    pclThread->Exit();
}

//---------------------------------------------------------------------------
void Thread_Sleep(uint32_t u32TimeMs_)
{
    Thread::Sleep(u32TimeMs_);
}

#if KERNEL_EXTENDED_CONTEXT
//---------------------------------------------------------------------------
void* Thread_GetExtendedContext(Thread_t handle)
{
    auto* pclThread = static_cast<Thread*>(handle);
    return pclThread->GetExtendedContext();
}

//---------------------------------------------------------------------------
void Thread_SetExtendedContext(Thread_t handle, void* pvData_)
{
    auto* pclThread = static_cast<Thread*>(handle);
    pclThread->SetExtendedContext(pvData_);
}
#endif // #if KERNEL_EXTENDED_CONTEXT

//---------------------------------------------------------------------------
void Thread_Yield(void)
{
    Thread::Yield();
}
//---------------------------------------------------------------------------
void Thread_CoopYield(void)
{
    Thread::CoopYield();
}
//---------------------------------------------------------------------------
void Thread_SetID(Thread_t handle, uint8_t u8ID_)
{
    auto* pclThread = static_cast<Thread*>(handle);
    pclThread->SetID(u8ID_);
}
//---------------------------------------------------------------------------
uint8_t Thread_GetID(Thread_t handle)
{
    auto* pclThread = static_cast<Thread*>(handle);
    return pclThread->GetID();
}
//---------------------------------------------------------------------------
uint16_t Thread_GetStackSlack(Thread_t handle)
{
    auto* pclThread = static_cast<Thread*>(handle);
    return pclThread->GetStackSlack();
}
//---------------------------------------------------------------------------
thread_state_t Thread_GetState(Thread_t handle)
{
    auto* pclThread = static_cast<Thread*>(handle);
    return static_cast<thread_state_t>(pclThread->GetState());
}
//---------------------------------------------------------------------------
// Timer APIs
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void Timer_Init(Timer_t handle)
{
    auto* pclTimer = new ((void*)handle) Timer();
    pclTimer->Init();
}

//---------------------------------------------------------------------------
void Timer_Start(Timer_t handle, bool bRepeat_, uint32_t u32IntervalMs_, timer_callback_t pfCallback_, void* pvData_)
{
    auto* pclTimer = static_cast<Timer*>(handle);
    pclTimer->Start(bRepeat_, u32IntervalMs_, reinterpret_cast<TimerCallback>(pfCallback_), pvData_);
}

//---------------------------------------------------------------------------
void Timer_Stop(Timer_t handle)
{
    auto* pclTimer = static_cast<Timer*>(handle);
    pclTimer->Stop();
}

//---------------------------------------------------------------------------
void Timer_Restart(Timer_t handle)
{
    auto* pclTimer = static_cast<Timer*>(handle);
    pclTimer->Start();
}

//---------------------------------------------------------------------------
// Semaphore APIs
//---------------------------------------------------------------------------
void Semaphore_Init(Semaphore_t handle, uint16_t u16InitVal_, uint16_t u16MaxVal_)
{
    auto* pclSemaphore = new ((void*)handle) Semaphore();
    pclSemaphore->Init(u16InitVal_, u16MaxVal_);
}

//---------------------------------------------------------------------------
void Semaphore_Post(Semaphore_t handle)
{
    auto* pclSemaphore = static_cast<Semaphore*>(handle);
    pclSemaphore->Post();
}

//---------------------------------------------------------------------------
void Semaphore_Pend(Semaphore_t handle)
{
    auto* pclSemaphore = static_cast<Semaphore*>(handle);
    pclSemaphore->Pend();
}

//---------------------------------------------------------------------------
bool Semaphore_TimedPend(Semaphore_t handle, uint32_t u32WaitTimeMS_)
{
    auto* pclSemaphore = static_cast<Semaphore*>(handle);
    return pclSemaphore->Pend(u32WaitTimeMS_);
}

//---------------------------------------------------------------------------
// Mutex APIs
//---------------------------------------------------------------------------
void Mutex_Init(Mutex_t handle)
{
    auto* pclMutex = new ((void*)handle) Mutex();
    pclMutex->Init();
}

//---------------------------------------------------------------------------
void Mutex_Claim(Mutex_t handle)
{
    auto* pclMutex = static_cast<Mutex*>(handle);
    pclMutex->Claim();
}

//---------------------------------------------------------------------------
void Mutex_Release(Mutex_t handle)
{
    auto* pclMutex = static_cast<Mutex*>(handle);
    pclMutex->Release();
}

//---------------------------------------------------------------------------
bool Mutex_TimedClaim(Mutex_t handle, uint32_t u32WaitTimeMS_)
{
    auto* pclMutex = static_cast<Mutex*>(handle);
    return pclMutex->Claim(u32WaitTimeMS_);
}

#if KERNEL_EVENT_FLAGS
//---------------------------------------------------------------------------
// EventFlag APIs
//---------------------------------------------------------------------------
void EventFlag_Init(EventFlag_t handle)
{
    auto* pclFlag = new ((void*)handle) EventFlag();
    pclFlag->Init();
}

//---------------------------------------------------------------------------
uint16_t EventFlag_Wait(EventFlag_t handle, uint16_t u16Mask_, event_flag_operation_t eMode_)
{
    auto* pclFlag = static_cast<EventFlag*>(handle);
    return pclFlag->Wait(u16Mask_, static_cast<EventFlagOperation>(eMode_));
}

//---------------------------------------------------------------------------
uint16_t EventFlag_TimedWait(EventFlag_t handle, uint16_t u16Mask_, event_flag_operation_t eMode_, uint32_t u32TimeMS_)
{
    auto* pclFlag = static_cast<EventFlag*>(handle);
    return pclFlag->Wait(u16Mask_, static_cast<EventFlagOperation>(eMode_), u32TimeMS_);
}

//---------------------------------------------------------------------------
void EventFlag_Set(EventFlag_t handle, uint16_t u16Mask_)
{
    auto* pclFlag = static_cast<EventFlag*>(handle);
    pclFlag->Set(u16Mask_);
}

//---------------------------------------------------------------------------
void EventFlag_Clear(EventFlag_t handle, uint16_t u16Mask_)
{
    auto* pclFlag = static_cast<EventFlag*>(handle);
    pclFlag->Clear(u16Mask_);
}

//---------------------------------------------------------------------------
uint16_t EventFlag_GetMask(EventFlag_t handle)
{
    auto* pclFlag = static_cast<EventFlag*>(handle);
    return pclFlag->GetMask();
}
#endif // #if #if KERNEL_EVENT_FLAGS

//---------------------------------------------------------------------------
// Notification APIs
//---------------------------------------------------------------------------
void Notify_Init(Notify_t handle)
{
    auto* pclNotify = new ((void*)handle) Notify();
    pclNotify->Init();
}

//---------------------------------------------------------------------------
void Notify_Signal(Notify_t handle)
{
    auto* pclNotify = static_cast<Notify*>(handle);
    pclNotify->Signal();
}

//---------------------------------------------------------------------------
void Notify_Wait(Notify_t handle, bool* pbFlag_)
{
    auto* pclNotify = static_cast<Notify*>(handle);
    pclNotify->Wait(pbFlag_);
}

//---------------------------------------------------------------------------
bool Notify_TimedWait(Notify_t handle, uint32_t u32WaitTimeMS_, bool* pbFlag_)
{
    auto* pclNotify = static_cast<Notify*>(handle);
    return pclNotify->Wait(u32WaitTimeMS_, pbFlag_);
}

//---------------------------------------------------------------------------
// Atomic Functions
//---------------------------------------------------------------------------
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

//---------------------------------------------------------------------------
// Message/Message Queue APIs
//---------------------------------------------------------------------------
void Message_Init(Message_t handle)
{
    auto* pclMessage = new ((void*)handle) Message();
    return pclMessage->Init();
}

//---------------------------------------------------------------------------
void Message_SetData(Message_t handle, void* pvData_)
{
    auto* pclMessage = static_cast<Message*>(handle);
    pclMessage->SetData(pvData_);
}

//---------------------------------------------------------------------------
void* Message_GetData(Message_t handle)
{
    auto* pclMessage = static_cast<Message*>(handle);
    return pclMessage->GetData();
}

//---------------------------------------------------------------------------
void Message_SetCode(Message_t handle, uint16_t u16Code_)
{
    auto* pclMessage = static_cast<Message*>(handle);
    pclMessage->SetCode(u16Code_);
}

//---------------------------------------------------------------------------
uint16_t Message_GetCode(Message_t handle)
{
    auto* pclMessage = static_cast<Message*>(handle);
    return pclMessage->GetCode();
}

//---------------------------------------------------------------------------
void MessageQueue_Init(MessageQueue_t handle)
{
    auto* pclMsgQ = new ((void*)handle) MessageQueue();
    pclMsgQ->Init();
}

//---------------------------------------------------------------------------
Message_t MessageQueue_Receive(MessageQueue_t handle)
{
    auto* pclMsgQ = static_cast<MessageQueue*>(handle);
    return pclMsgQ->Receive();
}

//---------------------------------------------------------------------------
void MessagePool_Init(MessagePool_t handle)
{
    auto* pclMsgPool = new ((void*)handle) MessagePool();
    pclMsgPool->Init();
}

//---------------------------------------------------------------------------
void MessagePool_Push(MessagePool_t handle, Message_t msg)
{
    auto* pclMsgPool = static_cast<MessagePool*>(handle);
    pclMsgPool->Push(reinterpret_cast<Message*>(msg));
}

//---------------------------------------------------------------------------
Message_t MessagePool_Pop(MessagePool_t handle)
{
    auto* pclMsgPool = static_cast<MessagePool*>(handle);
    return reinterpret_cast<Message_t>(pclMsgPool->Pop());
}

//---------------------------------------------------------------------------
Message_t MessageQueue_TimedReceive(MessageQueue_t handle, uint32_t u32TimeWaitMS_)
{
    auto* pclMsgQ = static_cast<MessageQueue*>(handle);
    return reinterpret_cast<Message_t>(pclMsgQ->Receive(u32TimeWaitMS_));
}

//---------------------------------------------------------------------------
void MessageQueue_Send(MessageQueue_t handle, Message_t hMessage_)
{
    auto* pclMsgQ = static_cast<MessageQueue*>(handle);
    pclMsgQ->Send(reinterpret_cast<Message*>(hMessage_));
}

//---------------------------------------------------------------------------
uint16_t MessageQueue_GetCount(MessageQueue_t handle)
{
    auto* pclMsgQ = static_cast<MessageQueue*>(handle);
    return pclMsgQ->GetCount();
}

//---------------------------------------------------------------------------
// Mailbox APIs
//---------------------------------------------------------------------------
void Mailbox_Init(Mailbox_t handle, void* pvBuffer_, uint16_t u16BufferSize_, uint16_t u16ElementSize_)
{ 
    auto* pclMBox = new ((void*)handle) Mailbox();
    pclMBox->Init(pvBuffer_, u16BufferSize_, u16ElementSize_);
}

//---------------------------------------------------------------------------
bool Mailbox_Send(Mailbox_t handle, void* pvData_)
{
    auto* pclMBox = static_cast<Mailbox*>(handle);
    return pclMBox->Send(pvData_);
}

//---------------------------------------------------------------------------
bool Mailbox_SendTail(Mailbox_t handle, void* pvData_)
{
    auto* pclMBox = static_cast<Mailbox*>(handle);
    return pclMBox->SendTail(pvData_);
}

//---------------------------------------------------------------------------
bool Mailbox_TimedSend(Mailbox_t handle, void* pvData_, uint32_t u32TimeoutMS_)
{
    Mailbox* pclMBox = (Mailbox*)handle;
    return pclMBox->Send(pvData_, u32TimeoutMS_);
}

//---------------------------------------------------------------------------
bool Mailbox_TimedSendTail(Mailbox_t handle, void* pvData_, uint32_t u32TimeoutMS_)
{
    auto* pclMBox = static_cast<Mailbox*>(handle);
    return pclMBox->SendTail(pvData_, u32TimeoutMS_);
}

//---------------------------------------------------------------------------
void Mailbox_Receive(Mailbox_t handle, void* pvData_)
{
    auto* pclMBox = static_cast<Mailbox*>(handle);
    pclMBox->Receive(pvData_);
}

//---------------------------------------------------------------------------
void Mailbox_ReceiveTail(Mailbox_t handle, void* pvData_)
{
    auto* pclMBox = static_cast<Mailbox*>(handle);
    pclMBox->ReceiveTail(pvData_);
}

//---------------------------------------------------------------------------
bool Mailbox_TimedReceive(Mailbox_t handle, void* pvData_, uint32_t u32TimeoutMS_)
{
    auto* pclMBox = static_cast<Mailbox*>(handle);
    return pclMBox->Receive(pvData_, u32TimeoutMS_);
}

//---------------------------------------------------------------------------
bool Mailbox_TimedReceiveTail(Mailbox_t handle, void* pvData_, uint32_t u32TimeoutMS_)
{
    auto* pclMBox = static_cast<Mailbox*>(handle);
    return pclMBox->ReceiveTail(pvData_, u32TimeoutMS_);
}

//---------------------------------------------------------------------------
uint16_t Mailbox_GetFreeSlots(Mailbox_t handle)
{
    auto* pclMBox = static_cast<Mailbox*>(handle);
    return pclMBox->GetFreeSlots();
}

//---------------------------------------------------------------------------
bool Mailbox_IsFull(Mailbox_t handle)
{
    auto* pclMBox = static_cast<Mailbox*>(handle);
    return pclMBox->IsFull();
}

//---------------------------------------------------------------------------
bool Mailbox_IsEmpty(Mailbox_t handle)
{
    auto* pclMBox = static_cast<Mailbox*>(handle);
    return pclMBox->IsEmpty();
}

//---------------------------------------------------------------------------
// Condition Variables
//---------------------------------------------------------------------------
void ConditionVariable_Init(ConditionVariable_t handle)
{
    auto* pclCondvar = new ((void*)handle) ConditionVariable();
    pclCondvar->Init();
}
//---------------------------------------------------------------------------
void ConditionVariable_Wait(ConditionVariable_t handle, Mutex_t hMutex_)
{    
    auto* pclCondvar = static_cast<ConditionVariable*>(handle);
    auto* pclMutex   = static_cast<Mutex*>(hMutex_);
    pclCondvar->Wait(pclMutex);
}
//---------------------------------------------------------------------------
void ConditionVariable_Signal(ConditionVariable_t handle)
{
    auto* pclCondvar = static_cast<ConditionVariable*>(handle);
    pclCondvar->Signal();
}
//---------------------------------------------------------------------------
void ConditionVariable_Broadcast(ConditionVariable_t handle)
{
    auto* pclCondvar = static_cast<ConditionVariable*>(handle);
    pclCondvar->Broadcast();
}
//---------------------------------------------------------------------------
bool ConditionVariable_TimedWait(ConditionVariable_t handle, Mutex_t hMutex_, uint32_t u32WaitTimeMS_)
{
    auto* pclCondvar = static_cast<ConditionVariable*>(handle);
    auto* pclMutex   = static_cast<Mutex*>(hMutex_);
    return pclCondvar->Wait(pclMutex, u32WaitTimeMS_);
}

//---------------------------------------------------------------------------
// Reader-writer locks
//---------------------------------------------------------------------------
void ReaderWriterLock_Init(ReaderWriterLock_t handle)
{
    auto* pclReaderWriter = new ((void*)handle) ReaderWriterLock();
    pclReaderWriter->Init();
}

//---------------------------------------------------------------------------
void ReaderWriterLock_AcquireReader(ReaderWriterLock_t handle)
{
    auto* pclReaderWriter   = static_cast<ReaderWriterLock*>(handle);
    pclReaderWriter->AcquireReader();
}

//---------------------------------------------------------------------------
void ReaderWriterLock_ReleaseReader(ReaderWriterLock_t handle)
{
    auto* pclReaderWriter   = static_cast<ReaderWriterLock*>(handle);
    pclReaderWriter->ReleaseReader();
}

//---------------------------------------------------------------------------
void ReaderWriterLock_AcquireWriter(ReaderWriterLock_t handle)
{
    auto* pclReaderWriter   = static_cast<ReaderWriterLock*>(handle);
    pclReaderWriter->AcquireWriter();
}

//---------------------------------------------------------------------------
void ReaderWriterLock_ReleaseWriter(ReaderWriterLock_t handle)
{
    auto* pclReaderWriter   = static_cast<ReaderWriterLock*>(handle);
    pclReaderWriter->ReleaseWriter();
}

//---------------------------------------------------------------------------
bool ReaderWriterLock_TimedAcquireWriter(ReaderWriterLock_t handle, uint32_t u32TimeoutMs_)
{
    auto* pclReaderWriter   = static_cast<ReaderWriterLock*>(handle);
    return pclReaderWriter->AcquireWriter(u32TimeoutMs_);
}

//---------------------------------------------------------------------------
bool ReaderWriterLock_TimedAcquireReader(ReaderWriterLock_t handle, uint32_t u32TimeoutMs_)
{
    auto* pclReaderWriter   = static_cast<ReaderWriterLock*>(handle);
    return pclReaderWriter->AcquireReader(u32TimeoutMs_);
}

//---------------------------------------------------------------------------
void Coroutine_Init(Coroutine_t handle, PORT_PRIO_TYPE uPriority_, coroutine_callback_t pfHandler_, void* pvContext_)
{
    auto* pclCoroutine   = static_cast<Coroutine*>(handle);
    return pclCoroutine->Init(uPriority_, reinterpret_cast<CoroutineHandler>(pfHandler_), pvContext_);
}

//---------------------------------------------------------------------------
void Coroutine_Run(Coroutine_t handle)
{
    auto* pclCoroutine   = static_cast<Coroutine*>(handle);
    pclCoroutine->Run();
}

//---------------------------------------------------------------------------
void Coroutine_Activate(Coroutine_t handle)
{
    auto* pclCoroutine   = static_cast<Coroutine*>(handle);
    pclCoroutine->Activate();
}

//---------------------------------------------------------------------------
void Coroutine_SetPriority(Coroutine_t handle, PORT_PRIO_TYPE uPriority_)
{
    auto* pclCoroutine   = static_cast<Coroutine*>(handle);
    pclCoroutine->SetPriority(uPriority_);
}

//---------------------------------------------------------------------------
PORT_PRIO_TYPE Coroutine_GetPriority(Coroutine_t handle)
{
    auto* pclCoroutine   = static_cast<Coroutine*>(handle);
    return pclCoroutine->GetPriority();
}
