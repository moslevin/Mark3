#ifndef __BSP_RTC_H__
#define __BSP_RTC_H__

#include <stdint.h>
#include "rtc.h"

typedef void (*on_rtc_tick_t)(void);

void bsp_rtc_init(void);

void bsp_rtc_set_on_rtc_tick(on_rtc_tick_t pfOnTick_);

bool bsp_rtc_set_datetime(const calendar_t* cal);

bool bsp_rtc_get_datetime(calendar_t* cal);

bool bsp_rtc_get_uptime(uint32_t* pu32Seconds_, uint32_t* pu32Ticks_);

const char* bsp_rtc_get_month_name(void);

const char* bsp_rtc_get_day_of_week(void);

void bsp_rtc_kick(void);

#endif
