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
    \file   autoalloc.h

    \brief  Automatic memory allocation for kernel objects.
*/

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "mark3cfg.h"

namespace Mark3
{
//---------------------------------------------------------------------------
// Define function pointer types used for interfacing with an external heap.
//---------------------------------------------------------------------------
enum class AutoAllocType : uint8_t {
//-- Kernel object types
    EventFlag,
    MailBox,
    Message,
    MessagePool,
    MessageQueue,
    Mutex,
    Notify,
    Semaphore,
    Thread,
    Timer,
    ConditionVariable,
    ReaderWriterLock,
//-- Allow for users to define their own object types beginning with AutoAllocType_t::User
    User,
//--
    Raw = 0xFF
};

//---------------------------------------------------------------------------
using AutoAllocAllocator_t = void* (*)(AutoAllocType eType_, size_t sSize_);
using AutoAllocFree_t = void* (*)(AutoAllocType eType_, void* pvObj_);

//---------------------------------------------------------------------------
// Forward declaration of kernel objects that can be auotomatically allocated.
class EventFlag;
class Mailbox;
class Message;
class MessagePool;
class MessageQueue;
class Mutex;
class Notify;
class Semaphore;
class Thread;
class Timer;
class ReaderWriterLock;
class ConditionVariable;

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
     * \brief SetAllocatorFunctions
     * Set the functions used by this class to allocate/free memory used in the kernel.
     * \param pfAllocator_ Function to allocate an object based on its type and/or size
     * \param pfFree_ Function to free a previously-allocated object
     */
    static void SetAllocatorFunctions(AutoAllocAllocator_t pfAllocator_, AutoAllocFree_t pfFree_)
        {m_pfAllocator = pfAllocator_; m_pfFree = pfFree_;}

    /*!
     * \brief NewSemaphore
     * Allocate and create a new Mutex object for use within the system.
     * \return Pointer to newly created object, or 0 on allocation failure.
     */
    static Semaphore* NewSemaphore();

    /*!
     * \brief DestroySemaphore
     * Destroy and deallocate a previously-allocated Semaphore object from within the system.
     * \param pclSemaphore_ Pointer to the object to destroy.
     */
    static void DestroySemaphore(Semaphore* pclSemaphore_);

    /*!
     * \brief NewMutex
     * Allocate and create a new Mutex object for use within the system.
     * \return Pointer to newly created object, or 0 on allocation failure.
     */
    static Mutex* NewMutex();

    /*!
     * \brief DestroyMutex
     * Destroy and deallocate a previously-allocated Mutex object from within the system.
     * \param pclMutex_ Pointer to the object to destroy.
     */
    static void DestroyMutex(Mutex* pclMutex_);
    /*!
     * \brief NewEventFlag
     * Allocate and create a new EventFlag object for use within the system.
     * \return Pointer to newly created object, or 0 on allocation failure.
     */
    static EventFlag* NewEventFlag();

    /*!
     * \brief DestroyEventFlag
     * Destroy and deallocate a previously-allocated EventFlag object from within the system.
     * \param pclEventFlag_ Pointer to the object to destroy.
     */
    static void DestroyEventFlag(EventFlag* pclEventFlag_);

    /*!
     * \brief NewTimer
     * Allocate and create a new Timer object for use within the system.
     * \return Pointer to newly created object, or 0 on allocation failure.
     */
    static Timer* NewTimer();

    /*!
     * \brief DestroyTimer
     * Destroy and deallocate a previously-allocated Timer object from within the system.
     * \param pclTimer_ Pointer to the object to destroy.
     */
    static void DestroyTimer(Timer* pclTimer_);

    /*!
     * \brief NewNotify
     * Allocate and construct a new Thread object for use within the system.
     * \return Pointer to newly created object, or 0 on allocation failure.
     */
    static Notify* NewNotify();

    /*!
     * \brief DestroyNotify
     * Destroy and deallocate a previously-allocated Notify object from within the system.
     * \param pclNotify_ Pointer to the object to destroy.
     */
    static void DestroyNotify(Notify* pclNotify_);

    /*!
     * \brief NewMailbox
     * Allocate and construct a new Mailbox object for use within the system.
     * \return Pointer to newly created object, or 0 on allocation failure.
     */
    static Mailbox* NewMailbox();

    /*!
     * \brief DestroyMailbox
     * Destroy and deallocate a previously-allocated Mailbox object from within the system.
     * \param pclMailbox_ Pointer to the object to destroy.
     */
    static void DestroyMailbox(Mailbox* pclMailbox_);

    /*!
     * \brief NewMessage
     * Allocate and construct a new Message object for use within the system.
     * \return Pointer to newly created object, or 0 on allocation failure.
     */
    static Message* NewMessage();

    /*!
     * \brief NewMessagePool
     * Allocate and construct a new Messagepool object for use within the system
     * \return Pointer to newly created object, or nullptr on allocation failure.
     */
    static MessagePool* NewMessagePool();

    /*!
     * \brief NewMessageQueue
     * Allocate and construct a new MessageQueue object for use within the system.
     * \return Pointer to newly created object, or 0 on allocation failure.
     */
    static MessageQueue* NewMessageQueue();

    /*!
     * \brief DestroyMessage
     * Destroy and deallocate a previously-allocated Message object from within the system.
     * \param pclMessage_ Pointer to the object to destroy.
     */
    static void DestroyMessage(Message* pclMessage_);

    /*!
     * \brief DestroyMessagePool
     * Destroy and deallocate a previously-allocated MessagePool object from within the system.
     * \param pclMessagePool_ Pointer to message pool to be deallocated
     */
    static void DestroyMessagePool(MessagePool *pclMessagePool_);

    /*!
     * \brief DestroyMessageQueue
     * Destroy and deallocate a previously-allocated MessageQueue object from within the system.
     * \param pclMessageQ_ Pointer to the object to destroy.
     */
    static void DestroyMessageQueue(MessageQueue* pclMessageQ_);

    /*!
     * \brief NewConditionVariable
     * Create and initialize a new condition variable from within the system
     * \return pointer to a new ConditionVariable object, or NULL on failure
     */
    static ConditionVariable* NewConditionVariable();

    /*!
     * \brief DestroyConditionVariable
     * Destroy and deallocate a previously-allocated condition variable object
     * \param pclConditionVariable_ Pointer to the object to destroy
     */
    static void DestroyConditionVariable(ConditionVariable* pclConditionVariable_);

    /*!
     * \brief NewReaderWriterLock
     * Allocate and initialize a new ReaderWriterLock object from within the system
     * \return Pointer to a new ReaderWriterLock object, or NULL on failure
     */
    static ReaderWriterLock* NewReaderWriterLock();

    /*!
     * \brief DestroyReaderWriterLock
     * Destroy and deallocate a previously-allocated ReaderWriterLock object
     * \param pclReaderWriterLock_ Pointer to the object to destroy
     */
    static void DestroyReaderWriterLock(ReaderWriterLock* pclReaderWriterLock_);

    /*!
     * \brief NewThread
     * Allocate and construct a new Thread object for use within the system
     * \return Pointer to newly created object, or 0 on allocation failure.
     */
    static Thread* NewThread();

    /*!
     * \brief DestroyThread
     * Destroy and deallocate a Thread from within the process.
     * \param pclThread_ Pointer to the object to destroy.
     */
    static void DestroyThread(Thread* pclThread_);

    /*!
     * \brief NewUserTypeAllocation
     * Attempt to allocate a user-defined object type from the heap.
     * \param eUserType_ User defined object type, interpreted by the allocator function
     * \return pointer to a newly-created object, or NULL on error.
     */
    static void* NewUserTypeAllocation(uint8_t eUserType_);

    /*!
     * \brief DestroyUserTypeAllocation
     * Free a previously allocated user-defined object.
     * \param pvObj_ Pointer to previously-allocated object, allocated through NewUserTypeAllocation  ()
     * \param eUserType_ User defined object type, interpreted by the allocator function
     */
    static void DestroyUserTypeAllocation(uint8_t eUserType_, void* pvObj_);

    /*!
     * \brief NewRawData
     * Attempt to allocate a blob of raw data from the heap
     * \param sSize_ Size of the data blob (in bytes)
     * \return pointer to newly-allocated blob of data, or NULL on error.
     */
    static void* NewRawData(size_t sSize_);

    /*!
     * \brief DestroyRawData
     * Free a previously allocated blob of data allocated via NewRawData()
     * \param pvData_ pointer to previously-created data object
     */
    static void DestroyRawData(void* pvData_);

private:

    static void* Allocate(AutoAllocType eType_, size_t sSize_);
    static void Free(AutoAllocType eType_, void* pvObj_);

    static AutoAllocAllocator_t m_pfAllocator;    //!< Function used to allocate objects
    static AutoAllocFree_t      m_pfFree;         //!< Funciton used to free objects
};
} //namespace Mark3
