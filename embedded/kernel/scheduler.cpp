/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012 Funkenstein Software Consulting, all rights reserved.
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
#include "kernel_debug.h"
//---------------------------------------------------------------------------
#if defined __FILE_ID__
	#undef __FILE_ID__
#endif
#define __FILE_ID__ 	SCHEDULER_CPP

//---------------------------------------------------------------------------
Thread *g_pstNext;
Thread *g_pstCurrent;

//---------------------------------------------------------------------------
K_UCHAR Scheduler::m_bEnabled;
ThreadList Scheduler::m_clStopList;
ThreadList Scheduler::m_aclPriorities[NUM_PRIORITIES];
K_UCHAR Scheduler::m_ucPriFlag;

K_UCHAR g_ucFlag;
//---------------------------------------------------------------------------
static const K_UCHAR aucCLZ[16] ={-1,0,1,1,2,2,2,2,3,3,3,3,3,3,3,3};

//---------------------------------------------------------------------------
void Scheduler::Init() 
{
    m_ucPriFlag = 0;
    for (int i = 0; i < NUM_PRIORITIES; i++)
    {
        m_aclPriorities[i].SetPriority(i);
        m_aclPriorities[i].SetFlagPointer(&m_ucPriFlag);
    }
	g_ucFlag = m_ucPriFlag;
}

//---------------------------------------------------------------------------
void Scheduler::Schedule()
{
    K_UCHAR ucPri = 0;
    
    // Figure out what priority level has ready tasks (8 priorities max)
    ucPri = aucCLZ[m_ucPriFlag >> 4 ];
    if (ucPri == 0xFF) { ucPri = aucCLZ[m_ucPriFlag & 0x0F]; }
    else { ucPri += 4; }
    
    // Get the thread node at this priority.
    g_pstNext = (Thread*)( m_aclPriorities[ucPri].GetHead() );
	g_ucFlag = m_ucPriFlag;
	
	KERNEL_TRACE_1( STR_SCHEDULE_1, (K_USHORT)g_pstNext->GetID() );
}

//---------------------------------------------------------------------------
void Scheduler::Add(Thread *pclThread_)
{
    m_aclPriorities[pclThread_->GetPriority()].Add(pclThread_);
	g_ucFlag = m_ucPriFlag;
}

//---------------------------------------------------------------------------
void Scheduler::Remove(Thread *pclThread_)
{
    m_aclPriorities[pclThread_->GetPriority()].Remove(pclThread_);
	g_ucFlag = m_ucPriFlag;
}
