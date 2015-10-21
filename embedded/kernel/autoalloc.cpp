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
    \file   autoalloc.cpp

    \brief  Automatic memory allocation for kernel objects.
*/

#include "mark3cfg.h"
#include "autoalloc.h"
#include "threadport.h"
#include "kernel.h"

#if KERNEL_USE_AUTO_ALLOC

// Align to nearest word boundary
#define ALLOC_ALIGN(x)  ( ((x) + (sizeof(K_ADDR)-1)) & (sizeof(K_ADDR) - 1) )

//---------------------------------------------------------------------------
uint8_t AutoAlloc::m_au8AutoHeap[ AUTO_ALLOC_SIZE ];
K_ADDR  AutoAlloc::m_aHeapTop;

//---------------------------------------------------------------------------
void AutoAlloc::Init(void)
{
    m_aHeapTop = (K_ADDR)(m_au8AutoHeap);
}

//---------------------------------------------------------------------------
void *AutoAlloc::Allocate( uint16_t u16Size_ )
{
    void *pvRet = 0;

    CS_ENTER();
    uint16_t u16AllocSize = ALLOC_ALIGN(u16Size_);
    if ((((K_ADDR)m_aHeapTop - (K_ADDR)&m_au8AutoHeap[0]) + u16AllocSize) < AUTO_ALLOC_SIZE)
    {
        pvRet = (void*)m_aHeapTop;
        m_aHeapTop += u16AllocSize;
    }
    CS_EXIT();

    if (!pvRet)
    {
        Kernel::Panic(PANIC_AUTO_HEAP_EXHUSTED);
    }

    return pvRet;
}

#endif
