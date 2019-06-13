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
=========================================================================== */
/**
    @file   autoalloc.h

    @brief  Automatic memory allocation for kernel objects.
*/

#pragma once

#include <stddef.h>
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
using AutoAllocFree_t      = void (*)(AutoAllocType eType_, void* pvObj_);

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

/**
 * @brief The AutoAlloc class.
 * This class provides an object-allocation interface for both kernel objects
 * and user-defined types.  This class supplies callouts for alloc/free that
 * use object-type metadata to determine how objects may be allocated, allowing
 * a user to create custom dynamic memory implementations for specific object
 * types and sizes.  As a result, the user-defined allocators can avoid the
 * kinds of memory fragmentation and exhaustion issues that occur in typical
 * embedded systems in which a single heap is used to satisfy all allocations
 * in the application.
 */
class AutoAlloc
{
public:
    /**
     * @brief Init
     * Initialize the AutoAllocator before use.  Called by Kernel::Init().
     *
     */
    static void Init(void);

    /**
     * @brief SetAllocatorFunctions
     * Set the functions used by this class to allocate/free memory used in the kernel.
     * @param pfAllocator_ Function to allocate an object based on its type and/or size
     * @param pfFree_ Function to free a previously-allocated object
     */
    static void SetAllocatorFunctions(AutoAllocAllocator_t pfAllocator_, AutoAllocFree_t pfFree_);

    /**
     * Template function used to manage the allocation of predefined kernel object types
     */
    template <typename T, AutoAllocType e> static T* NewObject()
    {
        auto* pvObj = Allocate(e, sizeof(T));
        if (pvObj) {
            return new (pvObj) T();
        }
        return 0;
    }

    /**
     * Template function used to manage the destruction and de-allocation of predefined
     * kernel objects
     */
    template <typename T, AutoAllocType e> static void DestroyObject(T* pObj_)
    {
        pObj_->~T();
        Free(e, pObj_);
    }

    /**
     * @brief NewUserTypeAllocation
     * Attempt to allocate a user-defined object type from the heap.
     * @param eUserType_ User defined object type, interpreted by the allocator function
     * @return pointer to a newly-created object, or nullptr on error.
     */
    static void* NewUserTypeAllocation(uint8_t eUserType_);

    /**
     * @brief DestroyUserTypeAllocation
     * Free a previously allocated user-defined object.
     * @param pvObj_ Pointer to previously-allocated object, allocated through NewUserTypeAllocation  ()
     * @param eUserType_ User defined object type, interpreted by the allocator function
     */
    static void DestroyUserTypeAllocation(uint8_t eUserType_, void* pvObj_);

    /**
     * @brief NewRawData
     * Attempt to allocate a blob of raw data from the heap
     * @param sSize_ Size of the data blob (in bytes)
     * @return pointer to newly-allocated blob of data, or nullptr on error.
     */
    static void* NewRawData(size_t sSize_);

    /**
     * @brief DestroyRawData
     * Free a previously allocated blob of data allocated via NewRawData()
     * @param pvData_ pointer to previously-created data object
     */
    static void DestroyRawData(void* pvData_);

private:
    static void* Allocate(AutoAllocType eType_, size_t sSize_);
    static void  Free(AutoAllocType eType_, void* pvObj_);

    static AutoAllocAllocator_t m_pfAllocator; //!< Function used to allocate objects
    static AutoAllocFree_t      m_pfFree;      //!< Funciton used to free objectss
};
} // namespace Mark3
