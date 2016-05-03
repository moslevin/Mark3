/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012-2016 Funkenstein Software Consulting, all rights reserved.
See license.txt for more information
===========================================================================*/
/*!
    \file "system_heap_config.h"
    
    \brief System heap configuration - defines the block sizes and counts 
           used to fulfill system/service allocations.
*/
#ifndef __SYSTEM_HEAP_CONFIG_H__
#define __SYSTEM_HEAP_CONFIG_H__

#include "kerneltypes.h"

//---------------------------------------------------------------------------
/*!
    Set this to "1" if you want the system heap to be built as part of this
    library.
*/
#define USE_SYSTEM_HEAP        (1)

//---------------------------------------------------------------------------
/*!
    Define the number of heap block sizes that we want to have attached to
    our system heap.
*/
#define HEAP_NUM_SIZES        (3)

//---------------------------------------------------------------------------
/*!
    Define the block sizes for each of the fixed-size blocks that will be 
    managed by our heaps.  Must be defined in incrementing order.
*/
#define HEAP_BLOCK_SIZE_1        ((uint16_t) 8)
#define HEAP_BLOCK_SIZE_2        ((uint16_t) 16)
#define HEAP_BLOCK_SIZE_3        ((uint16_t) 24)
#define HEAP_BLOCK_SIZE_4        ((uint16_t) 32)
#define HEAP_BLOCK_SIZE_5        ((uint16_t) 48)
#define HEAP_BLOCK_SIZE_6        ((uint16_t) 64)
#define HEAP_BLOCK_SIZE_7        ((uint16_t) 96)
#define HEAP_BLOCK_SIZE_8        ((uint16_t) 128)
#define HEAP_BLOCK_SIZE_9        ((uint16_t) 192)
#define HEAP_BLOCK_SIZE_10        ((uint16_t) 256)

//---------------------------------------------------------------------------
/*!
    Define the number of blocks in each bin, tailored for a particular 
    application
*/
#define HEAP_BLOCK_COUNT_1        ((uint16_t) 4)
#define HEAP_BLOCK_COUNT_2        ((uint16_t) 4)
#define HEAP_BLOCK_COUNT_3        ((uint16_t) 2)
#define HEAP_BLOCK_COUNT_4        ((uint16_t) 2)
#define HEAP_BLOCK_COUNT_5        ((uint16_t) 2)
#define HEAP_BLOCK_COUNT_6        ((uint16_t) 2)
#define HEAP_BLOCK_COUNT_7        ((uint16_t) 1)
#define HEAP_BLOCK_COUNT_8        ((uint16_t) 1)
#define HEAP_BLOCK_COUNT_9        ((uint16_t) 1)
#define HEAP_BLOCK_COUNT_10        ((uint16_t) 1)

#endif

