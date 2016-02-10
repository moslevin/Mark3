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
    \file   autoalloc.h

    \brief  Automatic memory allocation for kernel objects.
*/

#ifndef __AUTO_ALLOC_H__
#define __AUTO_ALLOC_H__

#include <stdint.h>
#include <stdbool.h>
#include "mark3cfg.h"

#if KERNEL_USE_AUTO_ALLOC
// Forward declaration of kernel objects that can be auotomatically allocated.

#if KERNEL_USE_EVENTFLAG
class EventFlag;
#endif

#if KERNEL_USE_MAILBOX
class Mailbox;
#endif

#if KERNEL_USE_MESSAGE
class Message;
class MessageQueue;
#endif

#if KERNEL_USE_MUTEX
class Mutex;
#endif

#if KERNEL_USE_NOTIFY
class Notify;
#endif

#if KERNEL_USE_SEMAPHORE
class Semaphore;
#endif

class Thread;

#if KERNEL_USE_TIMERS
class Timer;
#endif

class AutoAlloc
{
public:
    /*!
     * \brief Init
     *
     * Initialize the AutoAllocator before use.  Called by Kernel::Init().
     *
     */
    static void Init(void);

    /*!
     * \brief Allocate
     *
     * Allocate a block of memory from the AutoAllocator's heap.
     * Note: If the system is unable to satisfy the request due to
     * heap exhaustion, the result is a Kernel::Panic().
     *
     * \param u16Size_ Size (in bytes) of memory requested
     * \return pointer to a data blob allocated
     */
    static void *Allocate( uint16_t u16Size_ );

#if KERNEL_USE_SEMAPHORE
    static Semaphore *NewSemaphore(void);
#endif

#if KERNEL_USE_MUTEX
    static Mutex *NewMutex(void);
#endif

#if KERNEL_USE_EVENTFLAG
    static EventFlag *NewEventFlag(void);
#endif

#if KERNEL_USE_MESSAGE
    static Message *NewMessage(void);
    static MessageQueue* NewMessageQueue(void);
#endif

#if KERNEL_USE_NOTIFY
    static Notify *NewNotify(void);
#endif

#if KERNEL_USE_MAILBOX
    static Mailbox *NewMailbox(void);
#endif

    static Thread *NewThread(void);

#if KERNEL_USE_TIMERS
    static Timer *NewTimer(void);
#endif

private:
    static uint8_t m_au8AutoHeap[ AUTO_ALLOC_SIZE ];    // Heap memory
    static K_ADDR  m_aHeapTop;                          // Top of the heap
};
#endif

#endif
