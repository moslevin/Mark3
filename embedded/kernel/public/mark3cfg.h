/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012-2015 Funkenstein Software Consulting, all rights reserved.
See license.txt for more information
===========================================================================*/
/*!
    \file mark3cfg.h
    
    \brief Mark3 Kernel Configuration
    
    This file is used to configure the kernel for your specific application
    in order to provide the optimal set of features for a given use case.

    Since you only pay the price (code space/RAM) for the features you use,
    you can usually find a sweet spot between features and resource usage
    by picking and choosing features a-la-carte.  This config file is
    written in an "interactive" way, in order to minimize confusion about
    what each option provides, and to make dependencies obvious.   
*/

#ifndef __MARK3CFG_H__
#define __MARK3CFG_H__

/*!
    Define the number of thread priorities that the kernel's scheduler will
    support.  The number of thread priorities is limited only by the memory
    of the host CPU, as a ThreadList object is statically-allocated for each
    thread priority.

    In practice, systems rarely need more than 32 priority levels, with the
    most complex having the capacity for 256.
*/
#define KERNEL_NUM_PRIORITIES            (8)

/*!
    The following options is related to all kernel time-tracking.

    -timers provide a way for events to be periodically triggered in a 
     lightweight manner.  These can be periodic, or one-shot.

    -Thread Quantum (usedd for round-robin scheduling) is dependent on this
     module, as is Thread Sleep functionality.
*/
#define KERNEL_USE_TIMERS                (1)

/*!
    If you've opted to use the kernel timers module, you have an option
    as to which timer implementation to use:  Tick-based or Tick-less.

    Tick-based timers provide a "traditional" RTOS timer implementation
    based on a fixed-frequency timer interrupt.  While this provides
    very accurate, reliable timing, it also means that the CPU is being
    interrupted far more often than may be necessary (as not all timer
    ticks result in "real work" being done).

    Tick-less timers still rely on a hardware timer interrupt, but uses
    a dynamic expiry interval to ensure that the interrupt is only
    called when the next timer expires.  This increases the complexity
    of the timer interrupt handler, but reduces the number and frequency.

    Note that the CPU port (kerneltimer.cpp) must be implemented for the
    particular timer variant desired.
*/
#if KERNEL_USE_TIMERS
    #define KERNEL_TIMERS_TICKLESS       (1)
#endif

/*!
    By default, if you opt to enable kernel timers, you also get timeout-
    enabled versions of the blocking object APIs along with it.  This
    support comes at a small cost to code size, but a slightly larger cost
    to realtime performance - as checking for the use of timers in the
    underlying internal code costs some cycles.

    As a result, the option is given to the user here to manually disable
    these timeout-based APIs if desired by the user for performance and
    code-size reasons.
*/
#if KERNEL_USE_TIMERS
    #define KERNEL_USE_TIMEOUTS          (1)
#else
    #define KERNEL_USE_TIMEOUTS          (0)
#endif

/*!
    Do you want to enable time quanta?  This is useful when you want to have
    tasks in the same priority group share time in a controlled way.  This
    allows equal tasks to use unequal amounts of the CPU, which is a great 
    way to set up CPU budgets per thread in a round-robin scheduling system.
    If enabled, you can specify a number of ticks that serves as the default 
    time period (quantum).  Unless otherwise specified, every thread in a 
    priority will get the default quantum.
*/
#if KERNEL_USE_TIMERS    
    #define KERNEL_USE_QUANTUM           (1)
#else
    #define KERNEL_USE_QUANTUM           (0)
#endif

/*!
    This value defines the default thread quantum when KERNEL_USE_QUANTUM is
    enabled.  The thread quantum value is in milliseconds
*/
#define THREAD_QUANTUM_DEFAULT           (4)

/*!
    This is a simple blocking object, where a thread (or threads) are guaranteed
    to block until an asynchronous event signals the object.
*/
#define KERNEL_USE_NOTIFY                (1)

/*!
    Do you want the ability to use counting/binary semaphores for thread 
    synchronization?  Enabling this features provides fully-blocking semaphores
    and enables all API functions declared in semaphore.h.  If you have to 
    pick one blocking mechanism, this is the one to choose.
*/
#define KERNEL_USE_SEMAPHORE             (1)

/*!
    Do you want the ability to use mutual exclusion semaphores (mutex) for
    resource/block protection?  Enabling this feature provides mutexes, with
    priority inheritence, as declared in mutex.h.
 */
#define KERNEL_USE_MUTEX                 (1)

/*!
    Provides additional event-flag based blocking.  This relies on an
    additional per-thread flag-mask to be allocated, which adds 2 bytes
    to the size of each thread object.
*/
#define KERNEL_USE_EVENTFLAG             (1)

/*!
    Enable inter-thread messaging using message queues.  This is the preferred
    mechanism for IPC for serious multi-threaded communications; generally
    anywhere a semaphore or event-flag is insufficient.
*/
#if KERNEL_USE_SEMAPHORE
    #define KERNEL_USE_MESSAGE           (1)
#else
    #define KERNEL_USE_MESSAGE           (0)
#endif

/*!
    If Messages are enabled, define the size of the default kernel message
    pool.  Messages can be manually added to the message pool, but this
    mechansims is more convenient and automatic.   All message queues
    share their message objects from this global pool to maximize efficiency
    and simplify data management.
*/
#if KERNEL_USE_MESSAGE
    #define GLOBAL_MESSAGE_POOL_SIZE     (8)
#endif

/*!
    Enable inter-thread messaging using mailboxes.  A mailbox manages a blob
    of data provided by the user, that is partitioned into fixed-size blocks
    called envelopes.  The size of an envelope is set by the user when the
    mailbox is initialized.  Any number of threads can read-from and write-to
    the mailbox.  Envelopes can be sent-to or received-from the mailbox at
    the head or tail.  In this way, mailboxes essentially act as a circular
    buffer that can be used as a blocking FIFO or LIFO queue.
*/
#if KERNEL_USE_SEMAPHORE
    #define KERNEL_USE_MAILBOX           (1)
#else
    #define KERNEL_USE_MAILBOX           (0)
#endif

/*!
    Do you want to be able to set threads to sleep for a specified time?
    This enables the Thread::Sleep() API.
*/
#if KERNEL_USE_TIMERS && KERNEL_USE_SEMAPHORE
    #define KERNEL_USE_SLEEP             (1)
#else
    #define KERNEL_USE_SLEEP             (0)
#endif

/*!
    Enabling device drivers provides a posix-like filesystem interface for 
    peripheral device drivers.
*/
#define KERNEL_USE_DRIVER                (1)

/*!
    Provide Thread method to allow the user to set a name for each
    thread in the system.  Adds a const char* pointer to the size
    of the thread object.
*/
#define KERNEL_USE_THREADNAME            (0)

/*!
    Provide extra Thread methods to allow the application to create
    (and more importantly destroy) threads at runtime.  useful for
    designs implementing worker threads, or threads that can be restarted
    after encountering error conditions.
*/
#define KERNEL_USE_DYNAMIC_THREADS       (1)

/*!
    Provides extra classes for profiling the performance of code.  useful
    for debugging and development, but uses an additional hardware timer.
*/
#define KERNEL_USE_PROFILER              (1)

/*!
    Provides extra logic for kernel debugging, and instruments the kernel 
    with extra asserts, and kernel trace functionality.
*/
#define KERNEL_USE_DEBUG                 (1)

#if KERNEL_USE_DEBUG
    /*!
        Set this to 1 to enable very chatty kernel logging.  Since most
        important things in the kernel emit logs, a large log-buffer and
        fast output are required in order to keep up.  This is a pretty
        advanced power-user type feature, so it's disabled by default.
     */
    #define KERNEL_ENABLE_LOGGING        (0)

    /*!
        This enables a set of logging macros similar to the kernel-logging
        macros; however, these can be enabled or disabled independently.
        This allows for user-code to benefit from the built-in kernel
        logging macros without having to account for the super-high-volume
        of logs generated by kernel code.
     */
    #define KERNEL_ENABLE_USER_LOGGING   (1)    //!< 1 to enable logging outside of kernel code
#else
    #define KERNEL_ENABLE_LOGGING        (0)
    #define KERNEL_ENABLE_USER_LOGGING   (0)
#endif


/*!
    Provides support for atomic operations, including addition, subtraction,
    set, and test-and-set.  Add/Sub/Set contain 8, 16, and 32-bit variants.
*/
#define KERNEL_USE_ATOMIC                (0)

/*!
    "Safe unlinking" performs extra checks on data to make sure that there
    are no consistencies when performing operations on linked lists.  This
    goes beyond pointer checks, adding a layer of structural and metadata
    validation to help detect system corruption early.
*/
#define SAFE_UNLINK                      (0)

/*!
    Include support for kernel-aware simulation.  Enabling this feature
    adds advanced profiling, trace, and environment-aware debugging and
    diagnostic functionality when Mark3-based applications are run on the
    flavr AVR simulator.
*/
#define KERNEL_AWARE_SIMULATION          (1)

/*!
    Enabling this feature removes the necessity for the user to dedicate
    a complete thread for idle functionality.  This saves a full thread
    stack, but also requires a bit extra static data.  This also adds a
    slight overhead to the context switch and scheduler, as a special case
    has to be taken into account.
*/
#define KERNEL_USE_IDLE_FUNC             (1)

/*!
    This feature enables an additional set of APIs that allow for objects
    to be created on-the-fly out of a special heap, without having to
    explicitly allocate them (from stack, heap, or static memory). Note
    that auto-alloc memory cannot be reclaimed.
*/
#define KERNEL_USE_AUTO_ALLOC            (0)

#if KERNEL_USE_AUTO_ALLOC
    #define AUTO_ALLOC_SIZE              (512)
#endif

#endif
