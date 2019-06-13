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
    @file   coroutine.h

    @brief  CoRoutine implementation
 */
#pragma once

#include "mark3cfg.h"
#include "ll.h"
#include "priomapl1.h"
#include "priomapl2.h"

namespace Mark3 {

// Priority map type declaration, based on port configuration
#if PORT_COROUTINE_PRIORITIES <= (PORT_PRIO_MAP_WORD_SIZE * 8u)
using CoPrioMap = PriorityMapL1<PORT_PRIO_TYPE, PORT_COROUTINE_PRIORITIES>;
#else
using CoPrioMap = PriorityMapL2<PORT_PRIO_TYPE, PORT_COROUTINE_PRIORITIES>;
#endif

// Forward declarations
class CoList;
class Coroutine;

// CoRoutine functino handler type definition
using CoroutineHandler = void (*)(Coroutine* pclCaller_, void* pvContext_);

/**
 * @brief The Coroutine class
 * implements a lightweight, run-to-completion task that forms
 * the basis for co-operative task scheduling in Mark3.  Coroutines are designed
 * to be run from a singular context, and scheduled as a result of events occurring
 * from threads, timers, interrupt sources, or other co-routines.
 *
 * Co-routines differ from Threads in that they cannot block, and must run to
 * completion before other (potentially higher-priority) co-routines block.
 *
 */
class Coroutine : public TypedLinkListNode<Coroutine>
{
public:

    ~Coroutine();

    /**
     * @brief Init
     * Initialize the coroutine object prior to use.  Must be called before using the
     * other methods in the class.
     *
     * @param uPriority_ The scheduling priority of this coroutine as configured by
     * the port.
     * @param pfHandler_ Function to run when the coroutine is executed
     * @param pvContext_ User-defined value passed into the handler function on
     * execution
     */
    void Init(PORT_PRIO_TYPE uPriority_, CoroutineHandler pfHandler_, void* pvContext_);

    /**
     * @brief Run
     * Clear the co-routine's pending execution flag, and execute the coroutine's handler
     * function.
     */
    void Run();

    /**
     * @brief Activate
     * Tag the co-routine as pending execution.  Has no effect if the co-routine is already
     * pending execution.
     */
    void Activate();

    /**
     * @brief SetPriority
     * Update the scheduling priority of the co-routine.  Can be called from within the
     * co-routine, or from any other context aware of the co-routine object.
     *
     * @param uPriority_ New priority of the co-routine
     */
    void SetPriority(PORT_PRIO_TYPE uPriority_);

    /**
     * @brief GetPriority
     * Retrieve the current scheduling priority of the co-routine.
     *
     * @return current scheduling priority of the co-routine.
     */
    PORT_PRIO_TYPE GetPriority();

private:
    CoList* m_pclOwner;
    CoroutineHandler m_pfHandler;
    void* m_pvContext;
    PORT_PRIO_TYPE m_uPriority;
    bool m_bQueued;
};
} // namespace Mark3
