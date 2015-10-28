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
=========================================================================== */
/*!
    \file buffalogger.h

    Super-efficient, super-secure logging routines.  Uses offline processing
    to ensure performance.
*/
#pragma once
#include <stdint.h>

//---------------------------------------------------------------------------
#define STR1(s) #s
#define STR(s) STR1(s)

//---------------------------------------------------------------------------
#define EMIT_DBG_STRING(str) \
do { \
    const static volatile char log_str[] __attribute__((section (".logger"))) __attribute__((unused)) = str; \
    const static volatile uint16_t line_id __attribute__((section (".logger"))) __attribute__((unused)) = __LINE__; \
    const static volatile uint16_t file_id __attribute__((section (".logger"))) __attribute__((unused)) = DBG_FILE; \
    const static volatile uint16_t sync __attribute__((section (".logger"))) __attribute__((unused)) = 0xCAFE; \
} while(0);

