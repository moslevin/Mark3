#include <avr/io.h>

#include "mark3.h"
#include "rtc.h"
#include "bsp_rtc.h"

static RTC s_clRTC;
static on_rtc_tick_t s_pfOnRTCTick;
static volatile uint32_t s_u32TicksToAdd = 0;

void bsp_rtc_set_on_rtc_tick(on_rtc_tick_t pfOnTick_)
{
    s_pfOnRTCTick = pfOnTick_;
}

static void bsp_rtc_hw_init(void)
{
    // Set up an RTC that expires every second.
    TCCR2A = 0;	 // Normal mode -- count to top @ 0xFF
    TCNT2 = 0;

    // Enable overflow interrupt
    TIFR2 = 0;
    TIMSK2 = (1 << TOIE2);

    // Enable asynchronous clock from 32kHz source
    ASSR = (1 << AS2);

    // 1-second expiry (32kHz / 128) = 256 (rollover)
    TCCR2B = (1 << CS22) | (1 << CS20);
}

void bsp_rtc_init(void)
{
    bsp_rtc_hw_init();

    s_clRTC.Init(1); // 1 tick per second
    calendar_t cal = {0};
    cal.u8Day = 27;
    cal.eMonth = MONTH_JUNE;
    cal.u16Year = 2016;
    cal.u8Hour = 20;
    cal.u8Minute = 50;
    cal.u8Second = 0;

    s_clRTC.SetDateTime(&cal);
}

bool bsp_rtc_set_datetime(const calendar_t* cal)
{
    bool rc = false;
    CS_ENTER();
    rc = s_clRTC.SetDateTime(cal);
    CS_EXIT();
    return rc;
}

bool bsp_rtc_get_datetime(calendar_t* cal)
{
    bool rc = false;
    CS_ENTER();
    rc = s_clRTC.GetDateTime(cal);
    CS_EXIT();
    return rc;
}

bool bsp_rtc_get_uptime(uint32_t* pu32Seconds_, uint32_t* pu32Ticks_)
{
    bool rc = false;
    CS_ENTER();
    rc = s_clRTC.GetUptime(pu32Seconds_, pu32Ticks_);
    CS_EXIT();
    return rc;
}

const char* bsp_rtc_get_month_name(void)
{
    const char* rc;
    bool bState = Scheduler::SetScheduler(false);
    rc = s_clRTC.GetMonthName();
    Scheduler::SetScheduler(bState);
    return rc;
}

const char* bsp_rtc_get_day_of_week(void)
{
    const char* rc;
    bool bState = Scheduler::SetScheduler(false);
    rc = s_clRTC.GetDayOfWeek();
    Scheduler::SetScheduler(bState);
    return rc;
}

void bsp_rtc_kick(void)
{
    uint32_t u32Ticks;
    CS_ENTER();
    u32Ticks = s_u32TicksToAdd;
    s_u32TicksToAdd = 0;
    CS_EXIT();

    if (u32Ticks) {
        s_clRTC.AddTime(u32Ticks);
    }
}

ISR(TIMER2_OVF_vect)
{
    s_u32TicksToAdd += 1; // 1-second resolution...
    if (s_pfOnRTCTick)
    {
        s_pfOnRTCTick();
    }
}
