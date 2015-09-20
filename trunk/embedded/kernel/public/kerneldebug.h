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
    uint16_t au16Msg__[5]; \
    au16Msg__[0] = 0xACDC;  \
    au16Msg__[1] = __FILE_ID__; \
    au16Msg__[2] = __LINE__; \
    au16Msg__[3] = TraceBuffer::Increment() ; \
    au16Msg__[4] = (uint16_t)(x) ; \
    TraceBuffer::Write(au16Msg__, 5); \
};

//---------------------------------------------------------------------------
#define KERNEL_TRACE_1( x, arg1 ) \
{ 	\
    uint16_t au16Msg__[6]; \
    au16Msg__[0] = 0xACDC;  \
    au16Msg__[1] = __FILE_ID__; \
    au16Msg__[2] = __LINE__; \
    au16Msg__[3] = TraceBuffer::Increment(); \
    au16Msg__[4] = (uint16_t)(x); \
    au16Msg__[5] = arg1; \
    TraceBuffer::Write(au16Msg__, 6); \
}

//---------------------------------------------------------------------------
#define KERNEL_TRACE_2( x, arg1, arg2 ) \
{ 	\
    uint16_t au16Msg__[7]; \
    au16Msg__[0] = 0xACDC;  \
    au16Msg__[1] = __FILE_ID__; \
    au16Msg__[2] = __LINE__; \
    au16Msg__[3] = TraceBuffer::Increment(); \
    au16Msg__[4] = (uint16_t)(x); \
    au16Msg__[5] = arg1; \
    au16Msg__[6] = arg2; \
    TraceBuffer::Write(au16Msg__, 7); \
}

//---------------------------------------------------------------------------
#define KERNEL_ASSERT( x ) \
{		\
	if( ( x ) == false ) \
	{	\
        uint16_t au16Msg__[5];	\
        au16Msg__[0] = 0xACDC;	\
        au16Msg__[1] = __FILE_ID__;	\
        au16Msg__[2] = __LINE__; \
        au16Msg__[3] = TraceBuffer::Increment(); \
        au16Msg__[4] = STR_ASSERT_FAILED;	\
        TraceBuffer::Write(au16Msg__, 5); \
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
