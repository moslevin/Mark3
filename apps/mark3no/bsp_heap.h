#ifndef __BSP_HEAP_H__
#define __BSP_HEAP_H__

#include <stdint.h>

void bsp_heap_init(void);

void* bsp_heap_alloc(uint32_t u32Size_);

void bsp_heap_free(void* pvData_);

#endif
