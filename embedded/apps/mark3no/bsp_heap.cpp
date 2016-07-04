#include "bsp_heap.h"
#include "mark3.h"
#include "arena.h"

#include <stdint.h>

static Arena clHeap;
static uint8_t au8HeapBlob[2048];

void bsp_heap_init(void)
{
    K_ADDR auSizes[9] = { 16, 24, 32, 48, 64, 96, 128, 256, 512 };
    clHeap.Init((void*)au8HeapBlob, 2048, auSizes, 9);
}

void* bsp_heap_alloc(uint32_t u32Size_)
{
    return clHeap.Allocate((K_ADDR)u32Size_);
}

void bsp_heap_free(void* pvData_)
{
    clHeap.Free(pvData_);
}
