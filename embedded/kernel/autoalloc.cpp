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
    \file   autoalloc.cpp

    \brief  Automatic memory allocation for kernel objects.
*/

#include "mark3cfg.h"
#include "mark3.h"
#include "autoalloc.h"
#include "threadport.h"
#include "kernel.h"


#if KERNEL_USE_AUTO_ALLOC

// Align to nearest word boundary
#define ALLOC_ALIGN(x)  ( ((x) + (sizeof(K_ADDR)-1)) & (sizeof(K_ADDR) - 1) )

//---------------------------------------------------------------------------
uint8_t AutoAlloc::m_au8AutoHeap[ AUTO_ALLOC_SIZE ];
K_ADDR  AutoAlloc::m_aHeapTop;

//---------------------------------------------------------------------------
void AutoAlloc::Init(void)
{
    m_aHeapTop = (K_ADDR)(m_au8AutoHeap);
}

//---------------------------------------------------------------------------
void *AutoAlloc::Allocate( uint16_t u16Size_ )
{
    void *pvRet = 0;

    CS_ENTER();
    uint16_t u16AllocSize = ALLOC_ALIGN(u16Size_);
    if ((((K_ADDR)m_aHeapTop - (K_ADDR)&m_au8AutoHeap[0]) + u16AllocSize) < AUTO_ALLOC_SIZE)
    {
        pvRet = (void*)m_aHeapTop;
        m_aHeapTop += u16AllocSize;
    }
    CS_EXIT();

    if (!pvRet)
    {
        Kernel::Panic(PANIC_AUTO_HEAP_EXHAUSTED);
    }

    return pvRet;
}

#if KERNEL_USE_SEMAPHORE
//---------------------------------------------------------------------------
Semaphore *AutoAlloc::NewSemaphore(void)
{
    void *pvObj = Allocate(sizeof(Semaphore));
    if (pvObj)
    {
        return new(pvObj) Semaphore();
    }
    return 0;
}
#endif


#if KERNEL_USE_MUTEX
//---------------------------------------------------------------------------
Mutex *AutoAlloc::NewMutex(void)
{
    void *pvObj = Allocate(sizeof(Mutex));
    if (pvObj)
    {
        return new(pvObj) Mutex();
    }
    return 0;
}
#endif

#if KERNEL_USE_EVENTFLAG
//---------------------------------------------------------------------------
EventFlag *AutoAlloc::NewEventFlag(void)
{
    void *pvObj = Allocate(sizeof(EventFlag));
    if (pvObj)
    {
        return new(pvObj) EventFlag();
    }
    return 0;
}
#endif

#if KERNEL_USE_MESSAGE
//---------------------------------------------------------------------------
Message *AutoAlloc::NewMessage(void)
{
    void *pvObj = Allocate(sizeof(Message));
    if (pvObj)
    {
        return new(pvObj) Message();
    }
    return 0;
}
//---------------------------------------------------------------------------
MessageQueue *AutoAlloc::NewMessageQueue(void)
{
    void *pvObj = Allocate(sizeof(MessageQueue));
    if (pvObj)
    {
        return new(pvObj) MessageQueue();
    }
    return 0;
}

#endif

#if KERNEL_USE_NOTIFY
//---------------------------------------------------------------------------
Notify *AutoAlloc::NewNotify(void)
{
    void *pvObj = Allocate(sizeof(Notify));
    if (pvObj)
    {
        return new(pvObj) Notify();
    }
    return 0;
}
#endif

#if KERNEL_USE_MAILBOX
//---------------------------------------------------------------------------
Mailbox *AutoAlloc::NewMailbox(void)
{
    void *pvObj = Allocate(sizeof(Mailbox));
    if (pvObj)
    {
        return new(pvObj) Mailbox();
    }
    return 0;
}
#endif

//---------------------------------------------------------------------------
Thread *AutoAlloc::NewThread(void)
{
    void *pvObj = Allocate(sizeof(Thread));
    if (pvObj)
    {
        return new(pvObj) Thread();
    }
    return 0;
}

#if KERNEL_USE_TIMERS
//---------------------------------------------------------------------------
Timer *AutoAlloc::NewTimer(void)
{
    void *pvObj = Allocate(sizeof(Timer));
    if (pvObj)
    {
        return new(pvObj) Timer();
    }
    return 0;
}
#endif

#endif
