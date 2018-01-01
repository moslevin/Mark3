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

    \file   process.h

    \brief  Processes using Mark3 Kernel + Arena Heaps
*/

#pragma once

#include "mark3.h"
#include "arena.h"

namespace Mark3
{

//---------------------------------------------------------------------------
// Enumeration of all kernel objects and types that can be tracked within a
// Mark3 Process.
typedef enum {
	POTYPE_THREAD,	
	POTYPE_TRACKED_ALLOC,
	POTYPE_STACK = POTYPE_TRACKED_ALLOC,
#if KERNEL_USE_SEMAPHORE	
	POTYPE_SEMAPHORE,
#endif
#if KERNEL_USE_MUTEX	
	POTYPE_MUTEX,
#endif
#if KERNEL_USE_TIMERS
	POTYPE_TIMER,
#endif
#if KERNEL_USE_EVENTFLAG
	POTYPE_EVENTFLAG,
#endif
#if KERNEL_USE_NOTIFY
	POTYPE_NOTIFICATION,
#endif
#if KERNEL_USE_MAILBOX	
	POTYPE_MAILBOX,
#endif
#if KERNEL_USE_MESSAGE
	POTYPE_MESSAGE_QUEUE,
	POTYPE_MESSAGE,
#endif
#if KERNEL_USE_EXTENDED_CONTEXT
	POTYPE_TLS,
#endif	
	//--
	POTYPE_COUNT
} ProcessObjectType_t;

//---------------------------------------------------------------------------
#if KERNEL_USE_EXTENDED_CONTEXT
class Process;
/*!
 * \brief The ProcessTLS class
 * Thread Local Storage object - allocated and assigned to Threads created within
 * a process.
 */
class ProcessTLS
{
	public:
	void* operator new(size_t sz, void* pv) { return (ProcessTLS*)pv; };
	~ProcessTLS() {}
	
	void SetProcess(Process* pclProcess_) { m_pclProcess = pclProcess_; }
	Process* GetProcess() { return m_pclProcess; }
	
	void SetError(uint8_t u8Error_) { m_u8Error = u8Error_; }
	uint8_t GetError() { return m_u8Error; }
	
private:
	Process* m_pclProcess;
	uint8_t  m_u8Error;
};
#endif

//---------------------------------------------------------------------------
/*!
 * \brief The ProcessObject class
 * Container class used to enable tracking of kernel objects and data within
 * a process.
 */
class ProcessObject : public LinkListNode {
public:
	void* operator new(size_t sz, void* pv) { return (ProcessObject*)pv; };

	void Init(ProcessObjectType_t eType_, void* pvObjData_);
	void DeInit();

    /*!
     * \brief GetObject
     * \return Pointer to the object held within the ProcessObject container.
     */
	void* GetObject() { return m_pclObjectData; }

    /*!
     * \brief GetType
     * \return Return the type of data held within the ProcessObject container.
     */
	ProcessObjectType_t GetType() { return m_eType; }
		
    /*!
     * \brief ObjectSize
     * Returns the size of a specified kernel object type
     *
     * \param eType_ Type of kernel object
     * \return size of the kernel object in bytes.
     */
	static K_ADDR ObjectSize(ProcessObjectType_t eType_);

    /*!
     * \brief GetObjectOffset
     * Return the offset to the object's kernel-object data relative to the
     * object's pointer.
     *
     * \return Offset of the allocated kernel-object (in bytes).
     */
	static K_ADDR GetObjectOffset();	
private:

    // Functions to call destructors for the various tracked object types

	void DestroyThread_i();	
#if KERNEL_USE_SEMAPHORE
	void DestroySemaphore_i();
#endif
#if KERNEL_USE_MUTEX
	void DestroyMutex_i();
#endif	
#if KERNEL_USE_EVENTFLAG
	void DestroyEventFlag_i();
#endif		
#if KERNEL_USE_TIMERS
	void DestroyTimer_i();
#endif
#if KERNEL_USE_NOTIFY
	void DestroyNotify_i();
#endif
#if KERNEL_USE_MAILBOX
	void DestroyMailbox_i();
#endif		
#if KERNEL_USE_MESSAGE
	void DestroyMessage_i();
	void DestroyMessageQueue_i();
#endif	
#if KERNEL_USE_EXTENDED_CONTEXT
	void DestroyProcessTLS_i();
#endif	
	
    void* m_pclObjectData;          //!< Pointer to the associated kernel object's memory
    ProcessObjectType_t m_eType;    //!< Kernel object's type
};

//---------------------------------------------------------------------------
/*!
 * \brief The Process class
 *
 * Mark3 Processes provide a container for related Threads, Timers, kernel
 * objects and data.  Conceptually, processes are the same as in more fully
 * featured operating systems, with the exception that all Mark3 processes
 * share a single physical address space, with processes using either shared
 * or dedicated heaps for allocation.
 *
 * As with other operating systems, when a process container is destroyed
 * or goes out of scope, the Threads/Timers/Kernel objects associated with it
 * are torn down in a sane manner, preserving the rest of the system and
 * enabling the previously-held memory to be re-used.  However, fatal
 * exceptions within a single process will still generally bring the whole
 * system down.
 *
 * Multiple instances of a processes can coexist simultaneously so long as
 * their code does not rely on singleton global data.
 *
 */
class Process
{
public:	
	Process();	
    ~Process() { Destroy(); }

	void* operator new(size_t sz, void* pv) { return (Process*)pv; };	

    /*!
     * \brief SetHeap
     * Set the pointer to the Arena heap to be used by the process.  The
     * heap may be dedicated to the process, or shared between other processes
     * in the system.  The advantage of using a dedicate heap for the process
     * is memory separation (reduce potential to bring down the whole system
     * when a single process corrupts its heap), and reduced potential for
     * catastrophic system-wide fragmentation.
     * \param pclArena_ Pointer to the heap to use within the process.
     */
	void SetHeap(Arena* pclArena_) { m_pclHeap = pclArena_; }
	
    /*!
     * \brief CreateThread
     * Allocate and construct a new Thread object for use within the process.
     * \return Pointer to newly created object, or 0 on allocation failure.
     */
	Thread* CreateThread();

    /*!
     * \brief DestroyThread
     * Destroy and deallocate a Thread from within the process.
     * \param pclThread_ Pointer to the object to destroy.
     */
	void DestroyThread(Thread* pclThread_);
	
    /*!
     * \brief CreateStack
     * Allocate and create a new thread stack for use by a Thread within the process.
     * \param kaSize_ Size of stack (in bytes) to allocate.
     * \return Pointer to newly allocated stack, or 0 on allocation failure.
     */
	K_WORD* CreateStack(K_ADDR kaSize_);

    /*!
     * \brief DestroyProcessStack
     * Destroy and deallocate a previously-allocated thread Stack from within the process.
     * \param pwStack_ Pointer to the object to destroy.
     */
	void DestroyProcessStack(K_WORD* pwStack_);
	
    /*!
     * \brief Allocate
     * Allocate a blob of memory for use within the process.  This blob will be tracked and
     * automatically freed on destruction of the process.
     * \param kaSize_ Size of the blob (in bytes) to allocate
     * \return Pointer to the newly allocated blob, or 0 on allocation failure.
     */
	K_WORD* Allocate(K_ADDR kaSize_);

    /*!
     * \brief Free
     * Deallocate a previously-allocated blob from within the process.  This does not destroy
     * the object, so the object's destructor must be manually invoked if not corresponding to
     * a simple data type.
     * \param pvObject_ Pointer to the object to destroy.
     */
	void Free(void* pvObject_);
	
#if KERNEL_USE_SEMAPHORE
    /*!
     * \brief CreateSemaphore
     * Allocate and create a new Mutex object for use within the process.
     * \return Pointer to newly created object, or 0 on allocation failure.
     */
	Semaphore* CreateSemaphore();

    /*!
     * \brief DestroySemaphore
     * Destroy and deallocate a previously-allocated Semaphore object from within the process.
     * \param pclSemaphore_ Pointer to the object to destroy.
     */
	void DestroySemaphore(Semaphore* pclSemaphore_);
#endif
#if KERNEL_USE_MUTEX
    /*!
     * \brief CreateMutex
     * Allocate and create a new Mutex object for use within the process.
     * \return Pointer to newly created object, or 0 on allocation failure.
     */
	Mutex* CreateMutex();

    /*!
     * \brief DestroyMutex
     * Destroy and deallocate a previously-allocated Mutex object from within the process.
     * \param pclMutex_ Pointer to the object to destroy.
     */
	void DestroyMutex(Mutex* pclMutex_);
#endif
#if KERNEL_USE_EVENTFLAG
    /*!
     * \brief CreateEventFlag
     * Allocate and create a new EventFlag object for use within the process.
     * \return Pointer to newly created object, or 0 on allocation failure.
     */
	EventFlag* CreateEventFlag();

    /*!
     * \brief DestroyEventFlag
     * Destroy and deallocate a previously-allocated EventFlag object from within the process.
     * \param pclEventFlag_ Pointer to the object to destroy.
     */
	void DestroyEventFlag(EventFlag* pclEventFlag_);	
#endif	
#if KERNEL_USE_TIMERS
    /*!
     * \brief CreateTimer
     * Allocate and create a new Timer object for use within the process.
     * \return Pointer to newly created object, or 0 on allocation failure.
     */
	Timer* CreateTimer();

    /*!
     * \brief DestroyTimer
     * Destroy and deallocate a previously-allocated Timer object from within the process.
     * \param pclTimer_ Pointer to the object to destroy.
     */
	void DestroyTimer(Timer* pclTimer_);	
#endif
#if KERNEL_USE_NOTIFY
    /*!
     * \brief CreateNotify
     * Allocate and construct a new Thread object for use within the process.
     * \return Pointer to newly created object, or 0 on allocation failure.
     */
	Notify* CreateNotify();	

    /*!
     * \brief DestroyNotify
     * Destroy and deallocate a previously-allocated Notify object from within the process.
     * \param pclNotify_ Pointer to the object to destroy.
     */
	void DestroyNotify(Notify* pclNotify_);
#endif
#if KERNEL_USE_MAILBOX
    /*!
     * \brief CreateMailbox
     * Allocate and construct a new Mailbox object for use within the process.
     * \return Pointer to newly created object, or 0 on allocation failure.
     */
	Mailbox* CreateMailbox();		

    /*!
     * \brief DestroyMailbox
     * Destroy and deallocate a previously-allocated Mailbox object from within the process.
     * \param pclMailbox_ Pointer to the object to destroy.
     */
	void DestroyMailbox(Mailbox* pclMailbox_);	
#endif	
#if KERNEL_USE_MESSAGE
    /*!
     * \brief CreateMessage
     * Allocate and construct a new Message object for use within the process.
     * \return Pointer to newly created object, or 0 on allocation failure.
     */
	Message* CreateMessage();

    /*!
     * \brief CreateMessageQueue
     * Allocate and construct a new MessageQueue object for use within the process.
     * \return Pointer to newly created object, or 0 on allocation failure.
     */
	MessageQueue* CreateMessageQueue();

    /*!
     * \brief DestroyMessage
     * Destroy and deallocate a previously-allocated Message object from within the process.
     * \param pclMessage_ Pointer to the object to destroy.
     */
	void DestroyMessage(Message* pclMessage_);

    /*!
     * \brief DestroyMessageQueue
     * Destroy and deallocate a previously-allocated MessageQueue object from within the process.
     * \param pclMessageQ_ Pointer to the object to destroy.
     */
	void DestroyMessageQueue(MessageQueue* pclMessageQ_);
#endif
#if KERNEL_USE_EXTENDED_CONTEXT

    /*!
     * \brief CreateProcessTLS
     * Allocate and construct a new TLS object for use within the process.
     * \return Pointer to newly created object, or 0 on allocation failure.
     */
	ProcessTLS* CreateProcessTLS();

    /*!
     * \brief DestroyProcessTLS
     * Destroy and deallocate a previously-allocated TLS object from within the process.
     *
     * \param pclProcessTLS_ Pointer to the TLS object to destroy.
     */
	void DestroyProcessTLS(ProcessTLS* pclProcessTLS_);	

    /*!
     * \brief CurrentProcess
     * Return a pointer to the currently-running process.
     *
     * \return Pointer to the currently-running process, or NULL if thread
     *         is not running from with a process context.
     */
	static Process* CurrentProcess();	
#endif
	
private:
    /*!
     * \brief AllocateProcessObject
     * Allocate and construct a specific type of object from the process heap.
     *
     * \param eType_ Type of process object to allocate
     * \return Pointer to a newly-created process object on succes, or null on
     *         error.
     */
    ProcessObject* AllocateProcessObject(ProcessObjectType_t eType_);

    /*!
     * \brief DestroyProcessObject
     * Terdown a single object tracked in the process.  This invokes the object
     * destructor before freeing its data from the process heap.
     *
     * \param pclObject_    Object to destroy and deallocate
     * \param pclOwner_     List containing the object.
     */
	void DestroyProcessObject(ProcessObject* pclObject_, DoubleLinkList* pclOwner_);

    /*!
     * \brief Destroy
     * Teardown all process timers, then threads, then all of the remaining process
     * objects and tracked allocations.
     */
    void Destroy();
	
    Arena* m_pclHeap;                       //!< Pointer to the heap used by the process
    DoubleLinkList m_clTimerList;           //!< List of all timers in the process
#if KERNEL_USE_TIMERS	
    DoubleLinkList m_clThreadList;          //!< List of all threads in the process
#endif	
    DoubleLinkList m_clProcessObjects;      //!< List of all tracked allocations for the process
};
} //namespace Mark3
