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

    \file   scheduler.cpp

    \brief  Strict-Priority + Round-Robin thread scheduler implementation

*/

#include "kerneltypes.h"
#include "ll.h"
#include "scheduler.h"
#include "thread.h"
#include "threadport.h"
#include "kernel.h"
#include "kerneldebug.h"
//---------------------------------------------------------------------------
#if defined __FILE_ID__
    #undef __FILE_ID__
#endif
#define __FILE_ID__     SCHEDULER_CPP       //!< File ID used in kernel trace calls

//---------------------------------------------------------------------------
volatile Thread *g_pclNext;      //!< Pointer to the currently-chosen next-running thread
Thread *g_pclCurrent;   //!< Pointer to the currently-running thread

//---------------------------------------------------------------------------
bool Scheduler::m_bEnabled;
bool Scheduler::m_bQueuedSchedule;

ThreadList Scheduler::m_clStopList;
ThreadList Scheduler::m_aclPriorities[NUM_PRIORITIES];
uint8_t Scheduler::m_u8PriFlag;

//---------------------------------------------------------------------------
/*!
 * This implements a 4-bit "Count-leading-zeros" operation using a RAM-based
 * lookup table.  It is used to efficiently perform a CLZ operation under the
 * assumption that a native CLZ instruction is unavailable.  This table is
 * further optimized to provide a 0xFF result in the event that the index value
 * is itself zero, allowing u16 to quickly identify whether or not subsequent
 * 4-bit LUT operations are required to complete the scheduling process.
 */
static const uint8_t aucCLZ[16] ={255,0,1,1,2,2,2,2,3,3,3,3,3,3,3,3};

//---------------------------------------------------------------------------
void Scheduler::Init() 
{
    m_u8PriFlag = 0;
    for (int i = 0; i < NUM_PRIORITIES; i++)
    {
        m_aclPriorities[i].SetPriority(i);
        m_aclPriorities[i].SetFlagPointer(&m_u8PriFlag);
    }
    m_bQueuedSchedule = false;
}

//---------------------------------------------------------------------------
void Scheduler::Schedule()
{
    uint8_t u8Pri = 0;
    
    // Figure out what priority level has ready tasks (8 priorities max)
    // To do this, we apply our current active-thread bitmap (m_u8PriFlag)
    // and perform a CLZ on the upper four bits.  If no tasks are found
    // in the higher priority bits, search the lower priority bits.  This
    // also assumes that we always have the idle thread ready-to-run in
    // priority level zero.
    u8Pri = aucCLZ[m_u8PriFlag >> 4 ];
    if (u8Pri == 0xFF)
    {
        u8Pri = aucCLZ[m_u8PriFlag & 0x0F];
    }
    else
    {
        u8Pri += 4;
    }

#if KERNEL_USE_IDLE_FUNC
    if (u8Pri == 0xFF)
    {
        // There aren't any active threads at all - set g_pclNext to IDLE
        g_pclNext = Kernel::GetIdleThread();
    }
    else
#endif
    {
        // Get the thread node at this priority.
        g_pclNext = (Thread*)( m_aclPriorities[u8Pri].GetHead() );
    }
    KERNEL_TRACE_1( STR_SCHEDULE_1, (uint16_t)((Thread*)g_pclNext)->GetID() );

}

//---------------------------------------------------------------------------
void Scheduler::Add(Thread *pclThread_)
{
    m_aclPriorities[pclThread_->GetPriority()].Add(pclThread_);
}

//---------------------------------------------------------------------------
void Scheduler::Remove(Thread *pclThread_)
{
    m_aclPriorities[pclThread_->GetPriority()].Remove(pclThread_);
}

//---------------------------------------------------------------------------
bool Scheduler::SetScheduler(bool bEnable_)
{
    bool bRet ;
    CS_ENTER();
    bRet = m_bEnabled;
    m_bEnabled = bEnable_;
    // If there was a queued scheduler evevent, dequeue and trigger an
    // immediate Yield
    if (m_bEnabled && m_bQueuedSchedule)
    {
        m_bQueuedSchedule = false;
        Thread::Yield();
    }
    CS_EXIT();
    return bRet;
}
