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

#include "mark3cfg.h"
#include "tracebuffer.h"
#include "kernelaware.h"
#include "paniccodes.h"
#include "kernel.h"
#include "buffalogger.h"
#include "dbg_file_list.h"

//---------------------------------------------------------------------------
#if (KERNEL_USE_DEBUG && !KERNEL_AWARE_SIMULATION && KERNEL_ENABLE_LOGGING)

//---------------------------------------------------------------------------
#define KERNEL_TRACE( x )	\
{ 	\
    EMIT_DBG_STRING( x ); \
    uint16_t au16Msg__[4]; \
    au16Msg__[0] = 0xACDC;  \
    au16Msg__[1] = DBG_FILE; \
    au16Msg__[2] = __LINE__; \
    au16Msg__[3] = TraceBuffer::Increment() ; \
    TraceBuffer::Write(au16Msg__, 4); \
};

//---------------------------------------------------------------------------
#define KERNEL_TRACE_1( x, arg1 ) \
{ 	\
    EMIT_DBG_STRING( x ); \
    uint16_t au16Msg__[5]; \
    au16Msg__[0] = 0xACDC;  \
    au16Msg__[1] = DBG_FILE; \
    au16Msg__[2] = __LINE__; \
    au16Msg__[3] = TraceBuffer::Increment(); \
    au16Msg__[4] = arg1; \
    TraceBuffer::Write(au16Msg__, 5); \
}

//---------------------------------------------------------------------------
#define KERNEL_TRACE_2( x, arg1, arg2 ) \
{ 	\
    EMIT_DBG_STRING( x ); \
    uint16_t au16Msg__[6]; \
    au16Msg__[0] = 0xACDC;  \
    au16Msg__[1] = DBG_FILE; \
    au16Msg__[2] = __LINE__; \
    au16Msg__[3] = TraceBuffer::Increment(); \
    au16Msg__[4] = arg1; \
    au16Msg__[5] = arg2; \
    TraceBuffer::Write(au16Msg__, 6); \
}

//---------------------------------------------------------------------------
#define KERNEL_ASSERT( x ) \
{		\
	if( ( x ) == false ) \
	{	\
        EMIT_DBG_STRING( "ASSERT FAILED" ); \
        uint16_t au16Msg__[4];	\
        au16Msg__[0] = 0xACDC;	\
        au16Msg__[1] = DBG_FILE;	\
        au16Msg__[2] = __LINE__; \
        au16Msg__[3] = TraceBuffer::Increment(); \
        TraceBuffer::Write(au16Msg__, 4); \
        Kernel::Panic(PANIC_ASSERT_FAILED); \
	}	\
}
#elif (KERNEL_USE_DEBUG && KERNEL_AWARE_SIMULATION && KERNEL_ENABLE_LOGGING)

//---------------------------------------------------------------------------
#define KERNEL_TRACE( x )	\
{ 	\
    EMIT_DBG_STRING( x ); \
    KernelAware::Trace( DBG_FILE, __LINE__ ); \
};

//---------------------------------------------------------------------------
#define KERNEL_TRACE_1( x, arg1 ) \
{ 	\
    EMIT_DBG_STRING( x ); \
    KernelAware::Trace( DBG_FILE, __LINE__, arg1 ); \
}

//---------------------------------------------------------------------------
#define KERNEL_TRACE_2( x, arg1, arg2 ) \
{ 	\
    EMIT_DBG_STRING( x ); \
    KernelAware::Trace( DBG_FILE, __LINE__, arg1, arg2 ); \
}

//---------------------------------------------------------------------------
#define KERNEL_ASSERT( x ) \
{		\
    if( ( x ) == false ) \
    {	\
        EMIT_DBG_STRING( "ASSERT FAILED" ); \
        KernelAware::Trace( DBG_FILE, __LINE__ ); \
        Kernel::Panic( PANIC_ASSERT_FAILED ); \
    }	\
}

#else
//---------------------------------------------------------------------------
// Note -- when kernel-debugging is disabled, we still have to define the
// macros to ensure that the expressions compile (albeit, by elimination
// during pre-processing).
//---------------------------------------------------------------------------
#define KERNEL_TRACE( x )               //!< Null Kernel Trace Macro
//---------------------------------------------------------------------------
#define KERNEL_TRACE_1( x, arg1 )       //!< Null Kernel Trace Macro
//---------------------------------------------------------------------------
#define KERNEL_TRACE_2( x, arg1, arg2 ) //!< Null Kernel Trace Macro
//---------------------------------------------------------------------------
#define KERNEL_ASSERT( x )              //!< Null Kernel Assert Macro

#endif // KERNEL_USE_DEBUG


//---------------------------------------------------------------------------
#if (KERNEL_USE_DEBUG && !KERNEL_AWARE_SIMULATION && KERNEL_ENABLE_USER_LOGGING)

//---------------------------------------------------------------------------
#define USER_TRACE( x )	\
{ 	\
    EMIT_DBG_STRING( x ); \
    uint16_t au16Msg__[4]; \
    au16Msg__[0] = 0xACDC;  \
    au16Msg__[1] = DBG_FILE; \
    au16Msg__[2] = __LINE__; \
    au16Msg__[3] = TraceBuffer::Increment() ; \
    TraceBuffer::Write(au16Msg__, 4); \
};

//---------------------------------------------------------------------------
#define USER_TRACE_1( x, arg1 ) \
{ 	\
    EMIT_DBG_STRING( x ); \
    uint16_t au16Msg__[5]; \
    au16Msg__[0] = 0xACDC;  \
    au16Msg__[1] = DBG_FILE; \
    au16Msg__[2] = __LINE__; \
    au16Msg__[3] = TraceBuffer::Increment(); \
    au16Msg__[4] = arg1; \
    TraceBuffer::Write(au16Msg__, 5); \
}

//---------------------------------------------------------------------------
#define USER_TRACE_2( x, arg1, arg2 ) \
{ 	\
    EMIT_DBG_STRING( x ); \
    uint16_t au16Msg__[6]; \
    au16Msg__[0] = 0xACDC;  \
    au16Msg__[1] = DBG_FILE; \
    au16Msg__[2] = __LINE__; \
    au16Msg__[3] = TraceBuffer::Increment(); \
    au16Msg__[4] = arg1; \
    au16Msg__[5] = arg2; \
    TraceBuffer::Write(au16Msg__, 6); \
}

//---------------------------------------------------------------------------
#define USER_ASSERT( x ) \
{		\
    if( ( x ) == false ) \
    {	\
        EMIT_DBG_STRING( "ASSERT FAILED" ); \
        uint16_t au16Msg__[4];	\
        au16Msg__[0] = 0xACDC;	\
        au16Msg__[1] = DBG_FILE;	\
        au16Msg__[2] = __LINE__; \
        au16Msg__[3] = TraceBuffer::Increment(); \
        TraceBuffer::Write(au16Msg__, 4); \
        Kernel::Panic(PANIC_ASSERT_FAILED); \
    }	\
}
#elif (KERNEL_USE_DEBUG && KERNEL_AWARE_SIMULATION && KERNEL_ENABLE_USER_LOGGING)

//---------------------------------------------------------------------------
#define USER_TRACE( x )	\
{ 	\
    EMIT_DBG_STRING( x ); \
    KernelAware::Trace( DBG_FILE, __LINE__ ); \
};

//---------------------------------------------------------------------------
#define USER_TRACE_1( x, arg1 ) \
{ 	\
    EMIT_DBG_STRING( x ); \
    KernelAware::Trace( DBG_FILE, __LINE__, arg1 ); \
}

//---------------------------------------------------------------------------
#define USER_TRACE_2( x, arg1, arg2 ) \
{ 	\
    EMIT_DBG_STRING( x ); \
    KernelAware::Trace( DBG_FILE, __LINE__, arg1, arg2 ); \
}

//---------------------------------------------------------------------------
#define USER_ASSERT( x ) \
{		\
    if( ( x ) == false ) \
    {	\
        EMIT_DBG_STRING( "ASSERT FAILED" ); \
        KernelAware::Trace( DBG_FILE, __LINE__ ); \
        Kernel::Panic( PANIC_ASSERT_FAILED ); \
    }	\
}

#else
//---------------------------------------------------------------------------
// Note -- when kernel-debugging is disabled, we still have to define the
// macros to ensure that the expressions compile (albeit, by elimination
// during pre-processing).
//---------------------------------------------------------------------------
#define USER_TRACE( x )               //!< Null Kernel Trace Macro
//---------------------------------------------------------------------------
#define USER_TRACE_1( x, arg1 )       //!< Null Kernel Trace Macro
//---------------------------------------------------------------------------
#define USER_TRACE_2( x, arg1, arg2 ) //!< Null Kernel Trace Macro
//---------------------------------------------------------------------------
#define USER_ASSERT( x )              //!< Null Kernel Assert Macro

#endif // KERNEL_USE_DEBUG

#endif
