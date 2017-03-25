/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012 - 2017 Funkenstein Software Consulting, all rights reserved.
See license.txt for more information
===========================================================================*/
/*!

    \file   process.cpp

    \brief  Processes using Mark3 Kernel + Arena Heaps
*/
#include "mark3.h"
#include "process.h"

//---------------------------------------------------------------------------
K_ADDR ProcessObject::ObjectSize(ProcessObjectType_t eType_)
{
	switch (eType_) {
		case POTYPE_THREAD: return sizeof(Thread);
#if KERNEL_USE_SEMAPHORE
		case POTYPE_SEMAPHORE: return sizeof(Semaphore);
#endif
#if KERNEL_USE_MUTEX
		case POTYPE_MUTEX: return sizeof(Mutex);
#endif
#if KERNEL_USE_TIMERS
		case POTYPE_TIMER: return sizeof(Timer);
#endif
#if KERNEL_USE_EVENTFLAG
		case POTYPE_EVENTFLAG: return sizeof(EventFlag);
#endif
#if KERNEL_USE_NOTIFY
		case POTYPE_NOTIFICATION: return sizeof(Notify);
#endif
#if KERNEL_USE_MAILBOX
		case POTYPE_MAILBOX: return sizeof(Mailbox);
#endif
#if KERNEL_USE_MESSAGE
		case POTYPE_MESSAGE_QUEUE: return sizeof(MessageQueue);
		case POTYPE_MESSAGE: return sizeof(Message);
#endif
#if KERNEL_USE_EXTENDED_CONTEXT
		case POTYPE_TLS: return sizeof(ProcessTLS);
#endif
		default: return 0;
	}
}

//---------------------------------------------------------------------------
void ProcessObject::Init(ProcessObjectType_t eType_, void* pvObjData_)
{
	m_eType = eType_;
	m_pclObjectData = pvObjData_;
	switch (eType_) {
		case POTYPE_THREAD: new (pvObjData_) Thread(); break;
#if KERNEL_USE_SEMAPHORE
		case POTYPE_SEMAPHORE: new (pvObjData_) Semaphore(); break;
#endif
#if KERNEL_USE_MUTEX
		case POTYPE_MUTEX: new (pvObjData_) Mutex(); break;
#endif
#if KERNEL_USE_TIMERS
		case POTYPE_TIMER: new (pvObjData_) Timer(); break;
#endif
#if KERNEL_USE_EVENTFLAG
		case POTYPE_EVENTFLAG: new (pvObjData_) EventFlag(); break;
#endif
#if KERNEL_USE_NOTIFY
		case POTYPE_NOTIFICATION: new (pvObjData_) Notify(); break;
#endif
#if KERNEL_USE_MAILBOX
		case POTYPE_MAILBOX: new (pvObjData_) Mailbox(); break;
#endif
#if KERNEL_USE_MESSAGE
		case POTYPE_MESSAGE_QUEUE: new (pvObjData_) MessageQueue(); break;
		case POTYPE_MESSAGE: new (pvObjData_) Message(); break;
#endif
#if KERNEL_USE_EXTENDED_CONTEXT
		case POTYPE_TLS: new (pvObjData_) ProcessTLS(); break;
#endif
		default: break;
	}
}

//---------------------------------------------------------------------------
void ProcessObject::DeInit()
{
	switch (m_eType) {
		case POTYPE_THREAD: DestroyThread_i(); break;
#if KERNEL_USE_SEMAPHORE
		case POTYPE_SEMAPHORE: DestroySemaphore_i(); break;
#endif
#if KERNEL_USE_MUTEX
		case POTYPE_MUTEX: DestroyMutex_i(); break;
#endif
#if KERNEL_USE_TIMERS
		case POTYPE_TIMER: DestroyTimer_i(); break;
#endif
#if KERNEL_USE_EVENTFLAG
		case POTYPE_EVENTFLAG: DestroyEventFlag_i(); break;
#endif
#if KERNEL_USE_NOTIFY
		case POTYPE_NOTIFICATION: DestroyNotify_i(); break;
#endif
#if KERNEL_USE_MAILBOX
		case POTYPE_MAILBOX: DestroyMailbox_i(); break;
#endif
#if KERNEL_USE_MESSAGE
		case POTYPE_MESSAGE_QUEUE: DestroyMessageQueue_i(); break;
		case POTYPE_MESSAGE: DestroyMessage_i(); break;
#endif
#if KERNEL_USE_EXTENDED_CONTEXT
		case POTYPE_TLS: DestroyProcessTLS_i(); break;
#endif
		default: break;
	}
}

//---------------------------------------------------------------------------
K_ADDR ProcessObject::GetObjectOffset()
{
	return sizeof(LinkListNode);
}

//---------------------------------------------------------------------------
void ProcessObject::DestroyThread_i()
{
	Thread* pclThread = static_cast<Thread*>(m_pclObjectData);
	pclThread->Stop();
	pclThread->~Thread();
}

#if KERNEL_USE_SEMAPHORE
//---------------------------------------------------------------------------
void ProcessObject::DestroySemaphore_i()
{
	Semaphore* pclSemaphore = static_cast<Semaphore*>(m_pclObjectData);
	pclSemaphore->~Semaphore();
}
#endif

#if KERNEL_USE_MUTEX
//---------------------------------------------------------------------------
void ProcessObject::DestroyMutex_i()
{
	Mutex* pclSemaphore = static_cast<Mutex*>(m_pclObjectData);
	pclSemaphore->~Mutex();
}
#endif

#if KERNEL_USE_EVENTFLAG
//---------------------------------------------------------------------------
void ProcessObject::DestroyEventFlag_i()
{
	EventFlag* pclEventFlag = static_cast<EventFlag*>(m_pclObjectData);
	pclEventFlag->~EventFlag();
}
#endif

#if KERNEL_USE_TIMERS
//---------------------------------------------------------------------------
void ProcessObject::DestroyTimer_i()
{
	Timer* pclTimer = static_cast<Timer*>(m_pclObjectData);
	pclTimer->Stop();
	pclTimer->~Timer();
}
#endif

#if KERNEL_USE_NOTIFY
//---------------------------------------------------------------------------
void ProcessObject::DestroyNotify_i()
{
	Notify* pclNotify = static_cast<Notify*>(m_pclObjectData);
	pclNotify->~Notify();
}
#endif

#if KERNEL_USE_MAILBOX
//---------------------------------------------------------------------------
void ProcessObject::DestroyMailbox_i()
{
	Mailbox* pclMailbox = static_cast<Mailbox*>(m_pclObjectData);
	pclMailbox->~Mailbox();
}
#endif

#if KERNEL_USE_MESSAGE
//---------------------------------------------------------------------------
void ProcessObject::DestroyMessage_i()
{
	Message* pclMessage = static_cast<Message*>(m_pclObjectData);
	pclMessage->~Message();
}

//---------------------------------------------------------------------------
void ProcessObject::DestroyMessageQueue_i()
{
	MessageQueue* pclMessageQ = static_cast<MessageQueue*>(m_pclObjectData);
	pclMessageQ->~MessageQueue();
}
#endif

#if KERNEL_USE_EXTENDED_CONTEXT
//---------------------------------------------------------------------------
void ProcessObject::DestroyProcessTLS_i()
{
	ProcessTLS* pclProcessTLS = static_cast<ProcessTLS*>(m_pclObjectData);
	pclProcessTLS->~ProcessTLS();
}
#endif

//---------------------------------------------------------------------------
Process::Process()
{
	m_clProcessObjects.Init();
	m_clThreadList.Init();
	m_clTimerList.Init();
}

//---------------------------------------------------------------------------
Thread* Process::CreateThread()
{
	ProcessObject* pclProcessObject = AllocateProcessObject(POTYPE_THREAD);
	if (!pclProcessObject) {
		return 0;
	}
	return static_cast<Thread*>(pclProcessObject->GetObject());
}

//---------------------------------------------------------------------------
void Process::DestroyThread(Thread* pclThread_)
{
	ProcessObject* pclBase = reinterpret_cast<ProcessObject*>((K_ADDR)pclThread_ - ProcessObject::GetObjectOffset());
#if KERNEL_USE_EXTENDED_CONTEXT
	DestroyProcessTLS(static_cast<ProcessTLS*>(pclThread_->GetExtendedContext()));
#endif
	DestroyProcessObject(pclBase, &m_clThreadList);
}

//---------------------------------------------------------------------------
K_WORD* Process::Allocate(K_ADDR kaSize_)
{
	void *pvObj = m_pclHeap->Allocate(sizeof(ProcessObject));
	if (!pvObj) {
		return 0;
	}
	void *pvObjData = m_pclHeap->Allocate(kaSize_);
	if (!pvObjData) {
		m_pclHeap->Free(pvObj);
		return 0;
	}

	ProcessObject *pclNewObj = static_cast<ProcessObject*>(pvObj);
	pclNewObj->Init(POTYPE_TRACKED_ALLOC, pvObjData);

	m_clProcessObjects.Add(pclNewObj);
	return static_cast<K_WORD*>(pvObjData);
}

//---------------------------------------------------------------------------
void Process::Free(void* pvObject_)
{
	ProcessObject* pclBase = reinterpret_cast<ProcessObject*>((K_ADDR)pvObject_ - ProcessObject::GetObjectOffset());
	DestroyProcessObject(pclBase, &m_clProcessObjects);
}

//---------------------------------------------------------------------------
K_WORD* Process::CreateStack(K_ADDR kaSize_)
{
	return static_cast<K_WORD*>(Allocate(kaSize_));
}

//---------------------------------------------------------------------------
void Process::DestroyProcessStack(K_WORD* pwStack_)
{
	Free((void*)pwStack_);
}

#if KERNEL_USE_SEMAPHORE
//---------------------------------------------------------------------------
Semaphore* Process::CreateSemaphore()
{
	ProcessObject* pclProcessObject = AllocateProcessObject(POTYPE_SEMAPHORE);
	if (!pclProcessObject) {
		return 0;
	}
	return static_cast<Semaphore*>(pclProcessObject->GetObject());
}

//---------------------------------------------------------------------------
void Process::DestroySemaphore(Semaphore* pclSemaphore_)
{
	ProcessObject* pclBase = reinterpret_cast<ProcessObject*>((K_ADDR)pclSemaphore_ - ProcessObject::GetObjectOffset());
	DestroyProcessObject(pclBase, &m_clProcessObjects);
}
#endif

#if KERNEL_USE_MUTEX
//---------------------------------------------------------------------------
Mutex* Process::CreateMutex()
{
	ProcessObject* pclProcessObject = AllocateProcessObject(POTYPE_MUTEX);
	if (!pclProcessObject) {
		return 0;
	}
	return static_cast<Mutex*>(pclProcessObject->GetObject());
}

//---------------------------------------------------------------------------
void Process::DestroyMutex(Mutex* pclMutex_)
{
	ProcessObject* pclBase = reinterpret_cast<ProcessObject*>((K_ADDR)pclMutex_ - ProcessObject::GetObjectOffset());
	DestroyProcessObject(pclBase, &m_clProcessObjects);
}
#endif

#if KERNEL_USE_EVENTFLAG
//---------------------------------------------------------------------------
EventFlag* Process::CreateEventFlag()
{
	ProcessObject* pclProcessObject = AllocateProcessObject(POTYPE_EVENTFLAG);
	if (!pclProcessObject) {
		return 0;
	}
	return static_cast<EventFlag*>(pclProcessObject->GetObject());
}

//---------------------------------------------------------------------------
void Process::DestroyEventFlag(EventFlag* pclEventFlag_)
{
	ProcessObject* pclBase = reinterpret_cast<ProcessObject*>((K_ADDR)pclEventFlag_ - ProcessObject::GetObjectOffset());
	DestroyProcessObject(pclBase, &m_clProcessObjects);
}
#endif

#if KERNEL_USE_TIMERS
//---------------------------------------------------------------------------
Timer* Process::CreateTimer()
{
	ProcessObject* pclProcessObject = AllocateProcessObject(POTYPE_TIMER);
	if (!pclProcessObject) {
		return 0;
	}
	return static_cast<Timer*>(pclProcessObject->GetObject());
}

//---------------------------------------------------------------------------
void Process::DestroyTimer(Timer* pclTimer_)
{
	ProcessObject* pclBase = reinterpret_cast<ProcessObject*>((K_ADDR)pclTimer_ - ProcessObject::GetObjectOffset());
	DestroyProcessObject(pclBase, &m_clTimerList);
}
#endif

#if KERNEL_USE_NOTIFY
//---------------------------------------------------------------------------
Notify* Process::CreateNotify()
{
	ProcessObject* pclProcessObject = AllocateProcessObject(POTYPE_NOTIFICATION);
	if (!pclProcessObject) {
		return 0;
	}
	return static_cast<Notify*>(pclProcessObject->GetObject());
}

//---------------------------------------------------------------------------
void Process::DestroyNotify(Notify* pclNotify_)
{
	ProcessObject* pclBase = reinterpret_cast<ProcessObject*>((K_ADDR)pclNotify_ - ProcessObject::GetObjectOffset());
	DestroyProcessObject(pclBase, &m_clProcessObjects);
}
#endif

#if KERNEL_USE_MAILBOX
//---------------------------------------------------------------------------
Mailbox* Process::CreateMailbox()
{
	ProcessObject* pclProcessObject = AllocateProcessObject(POTYPE_MAILBOX);
	if (!pclProcessObject) {
		return 0;
	}
	return static_cast<Mailbox*>(pclProcessObject->GetObject());
}

//---------------------------------------------------------------------------
void Process::DestroyMailbox(Mailbox* pclMailbox_)
{
	ProcessObject* pclBase = reinterpret_cast<ProcessObject*>((K_ADDR)pclMailbox_ - ProcessObject::GetObjectOffset());
	DestroyProcessObject(pclBase, &m_clProcessObjects);
}
#endif

#if KERNEL_USE_MESSAGE
//---------------------------------------------------------------------------
Message* Process::CreateMessage()
{
	ProcessObject* pclProcessObject = AllocateProcessObject(POTYPE_MESSAGE);
	if (!pclProcessObject) {
		return 0;
	}
	return static_cast<Message*>(pclProcessObject->GetObject());
}

//---------------------------------------------------------------------------
MessageQueue* Process::CreateMessageQueue()
{
	ProcessObject* pclProcessObject = AllocateProcessObject(POTYPE_MESSAGE_QUEUE);
	if (!pclProcessObject) {
		return 0;
	}
	return static_cast<MessageQueue*>(pclProcessObject->GetObject());
}

//---------------------------------------------------------------------------
void Process::DestroyMessage(Message* pclMessage_)
{
	ProcessObject* pclBase = reinterpret_cast<ProcessObject*>((K_ADDR)pclMessage_ - ProcessObject::GetObjectOffset());
	DestroyProcessObject(pclBase, &m_clProcessObjects);
}

//---------------------------------------------------------------------------
void Process::DestroyMessageQueue(MessageQueue* pclMessageQ_)
{
	ProcessObject* pclBase = reinterpret_cast<ProcessObject*>((K_ADDR)pclMessageQ_ - ProcessObject::GetObjectOffset());
	DestroyProcessObject(pclBase, &m_clProcessObjects);
}
#endif

#if KERNEL_USE_EXTENDED_CONTEXT
//---------------------------------------------------------------------------
ProcessTLS* Process::CreateProcessTLS()
{
	ProcessObject* pclProcessObject = AllocateProcessObject(POTYPE_TLS);
	if (!pclProcessObject) {
		return 0;
	}
	return static_cast<ProcessTLS*>(pclProcessObject->GetObject());
}

//---------------------------------------------------------------------------
void Process::DestroyProcessTLS(ProcessTLS* pclProcessTLS_)
{
	ProcessObject* pclBase = reinterpret_cast<ProcessObject*>((K_ADDR)pclProcessTLS_ - ProcessObject::GetObjectOffset());
	DestroyProcessObject(pclBase, &m_clProcessObjects);
}

//---------------------------------------------------------------------------
Process* Process::CurrentProcess()
{
	ProcessTLS* pclCurrent = static_cast<ProcessTLS*>(Scheduler::GetCurrentThread()->GetExtendedContext());
    if (!pclCurrent) {
        return 0;
    }
	return pclCurrent->GetProcess();
}
#endif

//---------------------------------------------------------------------------
ProcessObject* Process::AllocateProcessObject(ProcessObjectType_t eType_)
{
	void *pvObj = m_pclHeap->Allocate(sizeof(ProcessObject));
	if (!pvObj) {
		return 0;
	}

	void *pvObjData = m_pclHeap->Allocate(ProcessObject::ObjectSize(eType_));
	if (!pvObjData) {
		m_pclHeap->Free(pvObj);
		return 0;
	}

	ProcessObject* pclNewObj = new (pvObj) ProcessObject();
	pclNewObj->Init(eType_, pvObjData);

#if KERNEL_USE_EXTENDED_CONTEXT
	if (eType_ == POTYPE_THREAD) {
		ProcessTLS* pclNewTLS = CreateProcessTLS();
		if (!pclNewTLS) {
			m_pclHeap->Free(pvObjData);
			m_pclHeap->Free(pvObj);
			return 0;
		}
		pclNewTLS->SetProcess(this);
		Thread* pclNewThread = static_cast<Thread*>(pclNewObj->GetObject());
		pclNewThread->SetExtendedContext(pclNewTLS);
	}
#endif

	if (eType_ == POTYPE_THREAD) {
		m_clThreadList.Add(static_cast<LinkListNode*>(pclNewObj));
	}
#if KERNEL_USE_TIMERS
	else if (eType_ == POTYPE_TIMER) {
		m_clTimerList.Add(static_cast<LinkListNode*>(pclNewObj));
	}
#endif
	else {
		m_clProcessObjects.Add(static_cast<LinkListNode*>(pclNewObj));
	}
	return pclNewObj;
}

//---------------------------------------------------------------------------
void Process::DestroyProcessObject(ProcessObject* pclObject_, DoubleLinkList* pclOwner_)
{
	pclOwner_->Remove(pclObject_);

	pclObject_->DeInit();

	m_pclHeap->Free(pclObject_->GetObject());
	m_pclHeap->Free(pclObject_);
}

//---------------------------------------------------------------------------
void Process::Destroy()
{
#if KERNEL_USE_TIMERS
	DoubleLinkList* pclLists[] = {&m_clTimerList, &m_clThreadList, &m_clProcessObjects};
#else
	DoubleLinkList* pclLists[] = {&m_clThreadList, &m_clProcessObjects};
#endif

	for (K_WORD i = 0; i < sizeof(pclLists)/sizeof(DoubleLinkList*); i++) {
		ProcessObject* pclNext = static_cast<ProcessObject*>(pclLists[i]->GetHead());
		while (pclNext) {
			ProcessObject *pclCurr = pclNext;
			pclNext = static_cast<ProcessObject*>(pclNext->GetNext());

			DestroyProcessObject(pclCurr, pclLists[i]);
		}
	}
}
