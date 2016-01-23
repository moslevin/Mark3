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
    \file "system_heap.cpp"
    
    \brief Global system-heap implementation.  Provides a system-wide 
           malloc/free paradigm allocation scheme.
*/

#include "kerneltypes.h"
#include "system_heap_config.h"
#include "system_heap.h"

#if use_SYSTEM_HEAP

//---------------------------------------------------------------------------
uint8_t    SystemHeap::m_pu8RawHeap[ HEAP_RAW_SIZE ];
HeapConfig SystemHeap::m_pclSystemHeapConfig[ HEAP_NUM_SIZES + 1];
FixedHeap  SystemHeap::m_clSystemHeap;
bool     SystemHeap::m_bInit;

//---------------------------------------------------------------------------
void SystemHeap::Init(void)
{
#if HEAP_NUM_SIZES > 0      
    m_pclSystemHeapConfig[0].m_u16BlockSize  = HEAP_BLOCK_SIZE_1;
    m_pclSystemHeapConfig[0].m_u16BlockCount = HEAP_BLOCK_COUNT_1;
#endif
#if HEAP_NUM_SIZES > 1      
    m_pclSystemHeapConfig[1].m_u16BlockSize  = HEAP_BLOCK_SIZE_2;
    m_pclSystemHeapConfig[1].m_u16BlockCount = HEAP_BLOCK_COUNT_2;
#endif
#if HEAP_NUM_SIZES > 2      
    m_pclSystemHeapConfig[2].m_u16BlockSize  = HEAP_BLOCK_SIZE_3;
    m_pclSystemHeapConfig[2].m_u16BlockCount = HEAP_BLOCK_COUNT_3;
#endif
#if HEAP_NUM_SIZES > 3      
    m_pclSystemHeapConfig[3].m_u16BlockSize  = HEAP_BLOCK_SIZE_4;
    m_pclSystemHeapConfig[3].m_u16BlockCount = HEAP_BLOCK_COUNT_4;
#endif
#if HEAP_NUM_SIZES > 4      
    m_pclSystemHeapConfig[4].m_u16BlockSize  = HEAP_BLOCK_SIZE_5;
    m_pclSystemHeapConfig[4].m_u16BlockCount = HEAP_BLOCK_COUNT_5;
#endif
#if HEAP_NUM_SIZES > 5      
    m_pclSystemHeapConfig[5].m_u16BlockSize  = HEAP_BLOCK_SIZE_6;
    m_pclSystemHeapConfig[5].m_u16BlockCount = HEAP_BLOCK_COUNT_6;
#endif    
#if HEAP_NUM_SIZES > 6      
    m_pclSystemHeapConfig[6].m_u16BlockSize  = HEAP_BLOCK_SIZE_7;
    m_pclSystemHeapConfig[6].m_u16BlockCount = HEAP_BLOCK_COUNT_7;
#endif
#if HEAP_NUM_SIZES > 7      
    m_pclSystemHeapConfig[7].m_u16BlockSize  = HEAP_BLOCK_SIZE_8;
    m_pclSystemHeapConfig[7].m_u16BlockCount = HEAP_BLOCK_COUNT_8;
#endif
#if HEAP_NUM_SIZES > 8      
    m_pclSystemHeapConfig[8].m_u16BlockSize  = HEAP_BLOCK_SIZE_9;
    m_pclSystemHeapConfig[8].m_u16BlockCount = HEAP_BLOCK_COUNT_9;
#endif
#if HEAP_NUM_SIZES > 9      
    m_pclSystemHeapConfig[9].m_u16BlockSize  = HEAP_BLOCK_SIZE_10;
    m_pclSystemHeapConfig[9].m_u16BlockCount = HEAP_BLOCK_COUNT_10;
#endif    
#if HEAP_NUM_SIZES > 10      
    m_pclSystemHeapConfig[10].m_u16BlockSize  = HEAP_BLOCK_SIZE_11;
    m_pclSystemHeapConfig[10].m_u16BlockCount = HEAP_BLOCK_COUNT_11;
#endif    
#if HEAP_NUM_SIZES > 11      
    m_pclSystemHeapConfig[11].m_u16BlockSize  = HEAP_BLOCK_SIZE_12;
    m_pclSystemHeapConfig[11].m_u16BlockCount = HEAP_BLOCK_COUNT_12;
#endif    
#if HEAP_NUM_SIZES > 12      
    m_pclSystemHeapConfig[12].m_u16BlockSize  = HEAP_BLOCK_SIZE_13;
    m_pclSystemHeapConfig[12].m_u16BlockCount = HEAP_BLOCK_COUNT_13;
#endif    
#if HEAP_NUM_SIZES > 13      
    m_pclSystemHeapConfig[13].m_u16BlockSize  = HEAP_BLOCK_SIZE_14;
    m_pclSystemHeapConfig[13].m_u16BlockCount = HEAP_BLOCK_COUNT_14;
#endif    
#if HEAP_NUM_SIZES > 14      
    m_pclSystemHeapConfig[14].m_u16BlockSize  = HEAP_BLOCK_SIZE_15;
    m_pclSystemHeapConfig[14].m_u16BlockCount = HEAP_BLOCK_COUNT_15;
#endif    
#if HEAP_NUM_SIZES > 15      
    m_pclSystemHeapConfig[15].m_u16BlockSize  = HEAP_BLOCK_SIZE_16;
    m_pclSystemHeapConfig[15].m_u16BlockCount = HEAP_BLOCK_COUNT_16;
#endif    
#if HEAP_NUM_SIZES > 16      
    m_pclSystemHeapConfig[16].m_u16BlockSize  = HEAP_BLOCK_SIZE_17;
    m_pclSystemHeapConfig[16].m_u16BlockCount = HEAP_BLOCK_COUNT_17;
#endif    
#if HEAP_NUM_SIZES > 17      
    m_pclSystemHeapConfig[17].m_u16BlockSize  = HEAP_BLOCK_SIZE_18;
    m_pclSystemHeapConfig[17].m_u16BlockCount = HEAP_BLOCK_COUNT_18;
#endif    
#if HEAP_NUM_SIZES > 18      
    m_pclSystemHeapConfig[18].m_u16BlockSize  = HEAP_BLOCK_SIZE_19;
    m_pclSystemHeapConfig[18].m_u16BlockCount = HEAP_BLOCK_COUNT_19;
#endif    
#if HEAP_NUM_SIZES > 19      
    m_pclSystemHeapConfig[19].m_u16BlockSize  = HEAP_BLOCK_SIZE_20;
    m_pclSystemHeapConfig[19].m_u16BlockCount = HEAP_BLOCK_COUNT_20;
#endif    
#if HEAP_NUM_SIZES > 20      
    m_pclSystemHeapConfig[20].m_u16BlockSize  = HEAP_BLOCK_SIZE_21;
    m_pclSystemHeapConfig[20].m_u16BlockCount = HEAP_BLOCK_COUNT_21;
#endif    
        
    m_pclSystemHeapConfig[HEAP_NUM_SIZES].m_u16BlockSize = 0;
    m_pclSystemHeapConfig[HEAP_NUM_SIZES].m_u16BlockCount = 0;
    
    m_clSystemHeap.Create((void*)m_pu8RawHeap, m_pclSystemHeapConfig);
    
    m_bInit = true;
}

//---------------------------------------------------------------------------
void *SystemHeap::Alloc(uint16_t u16Size_)
{
    if (!m_bInit)
    {
        return NULL;
    }
    return m_clSystemHeap.Alloc(u16Size_);
}

//---------------------------------------------------------------------------
void SystemHeap::Free(void* pvBlock_)
{
    if (!m_bInit)
    {
        return;
    }
    m_clSystemHeap.Free(pvBlock_);
}

#endif // use_SYSTEM_HEAP
