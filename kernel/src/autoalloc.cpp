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
/*!
    \file   autoalloc.cpp

    \brief  Automatic memory allocation for kernel objects.
*/

#include "mark3.h"

#include <stdint.h>

using namespace Mark3;
//---------------------------------------------------------------------------
// Override new() and delete() using functions provided to AutoAlloc
//---------------------------------------------------------------------------
void* operator new(size_t n)
{
    return AutoAlloc::NewRawData(n);
}

//---------------------------------------------------------------------------
void* operator new[](size_t n)
{
    return AutoAlloc::NewRawData(n);
}

//---------------------------------------------------------------------------
void operator delete(void * p)
{
    AutoAlloc::DestroyRawData(p);
}

//---------------------------------------------------------------------------
void operator delete[](void * p)
{
    AutoAlloc::DestroyRawData(p);
}

//---------------------------------------------------------------------------
namespace Mark3
{
AutoAllocAllocator_t AutoAlloc::m_pfAllocator;    //!< Function used to allocate objects
AutoAllocFree_t      AutoAlloc::m_pfFree;         //!< Funciton used to free objects

//---------------------------------------------------------------------------
void AutoAlloc::Init()
{
    m_pfAllocator = NULL;
    m_pfFree = NULL;
}

//---------------------------------------------------------------------------
void* AutoAlloc::Allocate(AutoAllocType eType_, size_t sSize_)
{
    if (!m_pfAllocator) {
        return NULL;
    }
    return m_pfAllocator(eType_, sSize_);
}

//---------------------------------------------------------------------------
void AutoAlloc::Free(AutoAllocType eType_, void* pvObj_)
{
    if (!m_pfFree) {
        return;
    }
    m_pfFree(eType_, pvObj_);
}
//---------------------------------------------------------------------------
Semaphore* AutoAlloc::NewSemaphore(void)
{
    void* pvObj = Allocate(AutoAllocType::Semaphore, sizeof(Semaphore));
    if (pvObj) {
        return new (pvObj) Semaphore();
    }
    return 0;
}
//---------------------------------------------------------------------------
void AutoAlloc::DestroySemaphore(Semaphore *pclSemaphore_)
{
    pclSemaphore_->~Semaphore();
    Free(AutoAllocType::Semaphore, pclSemaphore_);
}
//---------------------------------------------------------------------------
Mutex* AutoAlloc::NewMutex(void)
{
    void* pvObj = Allocate(AutoAllocType::Mutex, sizeof(Mutex));
    if (pvObj) {
        return new (pvObj) Mutex();
    }
    return 0;
}
//---------------------------------------------------------------------------
void AutoAlloc::DestroyMutex(Mutex *pclMutex_)
{
    pclMutex_->~Mutex();
    Free(AutoAllocType::Mutex, pclMutex_);
}
//---------------------------------------------------------------------------
EventFlag* AutoAlloc::NewEventFlag(void)
{
    void* pvObj = Allocate(AutoAllocType::EventFlag, sizeof(EventFlag));
    if (pvObj) {
        return new (pvObj) EventFlag();
    }
    return 0;
}
//---------------------------------------------------------------------------
void AutoAlloc::DestroyEventFlag(EventFlag *pclEventFlag_)
{
    pclEventFlag_->~EventFlag();
    Free(AutoAllocType::EventFlag, pclEventFlag_);
}
//---------------------------------------------------------------------------
Message* AutoAlloc::NewMessage(void)
{
    void* pvObj = Allocate(AutoAllocType::Message, sizeof(Message));
    if (pvObj) {
        return new (pvObj) Message();
    }
    return 0;
}
//---------------------------------------------------------------------------
void AutoAlloc::DestroyMessage(Message *pclMessage_)
{
    pclMessage_->~Message();
    Free(AutoAllocType::Message, pclMessage_);
}
//---------------------------------------------------------------------------
MessagePool* AutoAlloc::NewMessagePool(void)
{
    void* pvObj = Allocate(AutoAllocType::MessagePool, sizeof(MessagePool));
    if (pvObj) {
        return new (pvObj) MessagePool();
    }
    return 0;
}
//---------------------------------------------------------------------------
void AutoAlloc::DestroyMessagePool(MessagePool *pclMessagePool_)
{
    pclMessagePool_->~MessagePool();
    Free(AutoAllocType::MessagePool, pclMessagePool_);
}

//---------------------------------------------------------------------------
MessageQueue* AutoAlloc::NewMessageQueue(void)
{
    void* pvObj = Allocate(AutoAllocType::MessageQueue, sizeof(MessageQueue));
    if (pvObj) {
        return new (pvObj) MessageQueue();
    }
    return 0;
}
//---------------------------------------------------------------------------
void AutoAlloc::DestroyMessageQueue(MessageQueue *pclMessageQ_)
{
    pclMessageQ_->~MessageQueue();
    Free(AutoAllocType::MessageQueue, pclMessageQ_);
}

//---------------------------------------------------------------------------
Notify* AutoAlloc::NewNotify(void)
{
    void* pvObj = Allocate(AutoAllocType::Notify, sizeof(Notify));
    if (pvObj) {
        return new (pvObj) Notify();
    }
    return 0;
}
//---------------------------------------------------------------------------
void AutoAlloc::DestroyNotify(Notify *pclNotify_)
{
    pclNotify_->~Notify();
    Free(AutoAllocType::Notify, pclNotify_);
}

//---------------------------------------------------------------------------
Mailbox* AutoAlloc::NewMailbox(void)
{
    void* pvObj = Allocate(AutoAllocType::MailBox, sizeof(Mailbox));
    if (pvObj) {
        return new (pvObj) Mailbox();
    }
    return 0;
}
//---------------------------------------------------------------------------
void AutoAlloc::DestroyMailbox(Mailbox *pclMailbox_)
{
    pclMailbox_->~Mailbox();
    Free(AutoAllocType::MailBox, pclMailbox_);
}

//---------------------------------------------------------------------------
ConditionVariable* AutoAlloc::NewConditionVariable()
{
    void* pvObj = Allocate(AutoAllocType::ConditionVariable, sizeof(ConditionVariable));
    if (pvObj) {
        return new (pvObj) ConditionVariable();
    }
    return 0;
}
//---------------------------------------------------------------------------
void AutoAlloc::DestroyConditionVariable(ConditionVariable* pclCondvar_)
{
    pclCondvar_->~ConditionVariable();
    Free(AutoAllocType::ConditionVariable, pclCondvar_);
}

//---------------------------------------------------------------------------
ReaderWriterLock* AutoAlloc::NewReaderWriterLock()
{
    void* pvObj = Allocate(AutoAllocType::ReaderWriterLock, sizeof(ReaderWriterLock));
    if (pvObj) {
        return new (pvObj) ReaderWriterLock();
    }
    return 0;
}
//---------------------------------------------------------------------------
void AutoAlloc::DestroyReaderWriterLock(ReaderWriterLock *pclReaderWriterLock_)
{
    pclReaderWriterLock_->~ReaderWriterLock();
    Free(AutoAllocType::ReaderWriterLock, pclReaderWriterLock_);
}

//---------------------------------------------------------------------------
Thread* AutoAlloc::NewThread(void)
{
    void* pvObj = Allocate(AutoAllocType::Thread, sizeof(Thread));
    if (pvObj) {
        return new (pvObj) Thread();
    }
    return 0;
}
//---------------------------------------------------------------------------
void AutoAlloc::DestroyThread(Thread *pclThread_)
{
    pclThread_->~Thread();
    Free(AutoAllocType::Thread, pclThread_);
}

//---------------------------------------------------------------------------
Timer* AutoAlloc::NewTimer(void)
{
    void* pvObj = Allocate(AutoAllocType::Timer, sizeof(Timer));
    if (pvObj) {
        return new (pvObj) Timer();
    }
    return 0;
}
//---------------------------------------------------------------------------
void AutoAlloc::DestroyTimer(Timer *pclTimer_)
{
    pclTimer_->~Timer();
    Free(AutoAllocType::Timer, pclTimer_);
}

//---------------------------------------------------------------------------
void* AutoAlloc::NewUserTypeAllocation(uint8_t eType_)
{
    return Allocate(static_cast<AutoAllocType>(eType_), 0);
}
//---------------------------------------------------------------------------
void AutoAlloc::DestroyUserTypeAllocation(uint8_t eUserType_, void *pvObj_)
{
    Free(AutoAllocType::User, pvObj_);
}
//---------------------------------------------------------------------------
void* AutoAlloc::NewRawData(size_t sSize_)
{
    return Allocate(AutoAllocType::Raw, sSize_);
}
//---------------------------------------------------------------------------
void AutoAlloc::DestroyRawData(void *pvData_)
{
    Free(AutoAllocType::Raw, pvData_);
}
} //namespace Mark3
