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
	\file tracebuffer.cpp
	\brief Kernel trace buffer class definition	
*/

#include "kerneltypes.h"
#include "tracebuffer.h"
#include "mark3cfg.h"
#include "writebuf16.h"
#include "kerneldebug.h"

#if KERNEL_USE_DEBUG && !KERNEL_AWARE_SIMULATION
//---------------------------------------------------------------------------
WriteBuffer16 TraceBuffer::m_clBuffer;
volatile uint16_t TraceBuffer::m_u16Index;
uint16_t TraceBuffer::m_au16Buffer[ (TRACE_BUFFER_SIZE/sizeof(uint16_t)) ];

//---------------------------------------------------------------------------
void TraceBuffer::Init()
{
	m_clBuffer.SetBuffers(m_au16Buffer, TRACE_BUFFER_SIZE/sizeof(uint16_t));
	m_u16Index = 0;
}

//---------------------------------------------------------------------------
uint16_t TraceBuffer::Increment()
{
	return m_u16Index++;	
}

//---------------------------------------------------------------------------
void TraceBuffer::Write( uint16_t *pu16Data_, uint16_t u16Size_ )
{
	// Pipe the data directly to the circular buffer
	m_clBuffer.WriteData(pu16Data_, u16Size_);
}

#endif

