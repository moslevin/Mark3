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
    \file kerneldebug.h
	
	\brief Macros and functions used for assertions, kernel traces, etc.
*/

#ifndef __KERNEL_DEBUG_H__
#define __KERNEL_DEBUG_H__

#include "debugtokens.h"
#include "mark3cfg.h"
#include "tracebuffer.h"
#include "kernelaware.h"
#include "paniccodes.h"
#include "kernel.h"
//---------------------------------------------------------------------------
#if (KERNEL_USE_DEBUG && !KERNEL_AWARE_SIMULATION)

//---------------------------------------------------------------------------
#define __FILE_ID__			STR_UNDEFINED        //!< File ID used in kernel trace calls

//---------------------------------------------------------------------------
#define KERNEL_TRACE( x )	\
{ 	\
	K_USHORT ausMsg__[5]; \
	ausMsg__[0] = 0xACDC;  \
	ausMsg__[1] = __FILE_ID__; \
	ausMsg__[2] = __LINE__; \
	ausMsg__[3] = TraceBuffer::Increment() ; \
	ausMsg__[4] = (K_USHORT)(x) ; \
	TraceBuffer::Write(ausMsg__, 5); \
};

//---------------------------------------------------------------------------
#define KERNEL_TRACE_1( x, arg1 ) \
{ 	\
	K_USHORT ausMsg__[6]; \
	ausMsg__[0] = 0xACDC;  \
	ausMsg__[1] = __FILE_ID__; \
	ausMsg__[2] = __LINE__; \
	ausMsg__[3] = TraceBuffer::Increment(); \
	ausMsg__[4] = (K_USHORT)(x); \
	ausMsg__[5] = arg1; \
	TraceBuffer::Write(ausMsg__, 6); \
}

//---------------------------------------------------------------------------
#define KERNEL_TRACE_2( x, arg1, arg2 ) \
{ 	\
	K_USHORT ausMsg__[7]; \
	ausMsg__[0] = 0xACDC;  \
	ausMsg__[1] = __FILE_ID__; \
	ausMsg__[2] = __LINE__; \
	ausMsg__[3] = TraceBuffer::Increment(); \
	ausMsg__[4] = (K_USHORT)(x); \
	ausMsg__[5] = arg1; \
	ausMsg__[6] = arg2; \
	TraceBuffer::Write(ausMsg__, 7); \
}

//---------------------------------------------------------------------------
#define KERNEL_ASSERT( x ) \
{		\
	if( ( x ) == false ) \
	{	\
		K_USHORT ausMsg__[5];	\
		ausMsg__[0] = 0xACDC;	\
		ausMsg__[1] = __FILE_ID__;	\
		ausMsg__[2] = __LINE__; \
		ausMsg__[3] = TraceBuffer::Increment(); \
		ausMsg__[4] = STR_ASSERT_FAILED;	\
		TraceBuffer::Write(ausMsg__, 5); \
        Kernel::Panic(PANIC_ASSERT_FAILED); \
	}	\
}

#elif (KERNEL_USE_DEBUG && KERNEL_AWARE_SIMULATION)
//---------------------------------------------------------------------------
#define __FILE_ID__			STR_UNDEFINED

//---------------------------------------------------------------------------
#define KERNEL_TRACE( x )	\
{ 	\
    KernelAware::Trace( __FILE_ID__, __LINE__, x ); \
};

//---------------------------------------------------------------------------
#define KERNEL_TRACE_1( x, arg1 ) \
{ 	\
    KernelAware::Trace( __FILE_ID__, __LINE__, x, arg1 ); \
}

//---------------------------------------------------------------------------
#define KERNEL_TRACE_2( x, arg1, arg2 ) \
{ 	\
    KernelAware::Trace( __FILE_ID__, __LINE__, x, arg1, arg2 ); \
}

//---------------------------------------------------------------------------
#define KERNEL_ASSERT( x ) \
{		\
    if( ( x ) == false ) \
    {	\
        KernelAware::Trace( __FILE_ID__, __LINE__, STR_ASSERT_FAILED ); \
        Kernel::Panic( PANIC_ASSERT_FAILED ); \
    }	\
}

#else
//---------------------------------------------------------------------------
// Note -- when kernel-debugging is disabled, we still have to define the
// macros to ensure that the expressions compile (albeit, by elimination
// during pre-processing).
//---------------------------------------------------------------------------
#define __FILE_ID__			0           //!< Null ID
//---------------------------------------------------------------------------
#define KERNEL_TRACE( x )               //!< Null Kernel Trace Macro
//---------------------------------------------------------------------------
#define KERNEL_TRACE_1( x, arg1 )       //!< Null Kernel Trace Macro
//---------------------------------------------------------------------------
#define KERNEL_TRACE_2( x, arg1, arg2 ) //!< Null Kernel Trace Macro
//---------------------------------------------------------------------------
#define KERNEL_ASSERT( x )              //!< Null Kernel Assert Macro

#endif // KERNEL_USE_DEBUG

#endif
