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

    \file   threadport.cpp   

    \brief  ARM Cortex-M0 Multithreading

*/

#include "kerneltypes.h"
#include "mark3cfg.h"
#include "thread.h"
#include "threadport.h"
#include "kernelswi.h"
#include "kerneltimer.h"
#include "timerlist.h"
#include "quantum.h"

//---------------------------------------------------------------------------
Thread *g_pstCurrentThread;

//---------------------------------------------------------------------------
static void ThreadPort_StartFirstThread( void ) __attribute__ (( naked ));
void ThreadPort_SVCHandler( void ) __attribute__ (( naked ));
void ThreadPort_PendSVHandler( void ) __attribute__ (( naked ));

//---------------------------------------------------------------------------
void ThreadPort::InitStack(Thread *pclThread_)
{
	K_ULONG *pulStack;
	K_ULONG *pulTemp;
	K_ULONG ulAddr;
	K_USHORT i;

	// Get the entrypoint for the thread
	ulAddr = (K_ULONG)(pclThread_->m_pfEntryPoint);

	// Get the top-of-stack pointer for the thread
	pulStack = (K_ULONG*)pclThread_->m_paucStackTop;

	// Initialize the stack to all FF's to aid in stack depth checking
	pulTemp = (K_ULONG*)pclThread_->m_paucStack;
	for (i = 0; i < pclThread_->m_usStackSize / sizeof(K_ULONG); i++)
	{
		pulTemp[i] = 0xFFFFFFFF;
	}

	// Add a word offset for returning from interrupts (which we simulate when start
	// a thread)
	PUSH_TO_STACK(pulStack, 0);				// Interrupt Padding
	PUSH_TO_STACK(pulStack, 0x01000000);	// XSPR
	PUSH_TO_STACK(pulStack, ulAddr);		// PC
	PUSH_TO_STACK(pulStack, 0x13);			// R13 -> R1
	PUSH_TO_STACK(pulStack, 0x12);
	PUSH_TO_STACK(pulStack, 0x3);
    PUSH_TO_STACK(pulStack, 0x2);
	PUSH_TO_STACK(pulStack, 0x1);
	PUSH_TO_STACK(pulStack, (K_ULONG)pclThread_->m_pvArg);	// R0 = argument
	PUSH_TO_STACK(pulStack, 0x11);			// R11 -> R4
	PUSH_TO_STACK(pulStack, 0x10);
	PUSH_TO_STACK(pulStack, 0x09);
	PUSH_TO_STACK(pulStack, 0x08);
	PUSH_TO_STACK(pulStack, 0x07);
	PUSH_TO_STACK(pulStack, 0x06);
	PUSH_TO_STACK(pulStack, 0x05);
	PUSH_TO_STACK(pulStack, 0x04);

	pclThread_->m_paucStackTop = (K_ADDR*)pulStack;
}

//---------------------------------------------------------------------------
static void Thread_Switch(void)
{
    g_pstCurrent = g_pstNext;
}

//---------------------------------------------------------------------------
void ThreadPort::StartThreads()
{
    KernelSWI::Config();             // configure the task switch SWI
    KernelTimer::Config();           // configure the kernel timer

    Scheduler::SetScheduler(1);      // enable the scheduler
    Scheduler::Schedule();           // run the scheduler - determine the first thread to run

    Thread_Switch();                 // Set the next scheduled thread to the current thread

    KernelTimer::Start();            // enable the kernel timer
    KernelSWI::Start();              // enable the task switch SWI

    ThreadPort_StartFirstThread();	 // Jump to the first thread (does not return)
}

//---------------------------------------------------------------------------
void ThreadPort_StartFirstThread( void )
{
    //!ToDo
}

//---------------------------------------------------------------------------
void ThreadPort_SVCHandler( void )
{
    //!ToDo
}

//---------------------------------------------------------------------------
void ThreadPort_PendSVHandler( void )
{
    //!ToDo
}

#endif
