/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2014 Funkenstein Software Consulting, all rights reserved.
See license.txt for more information
===========================================================================*/
/*!
    \file rtc.h
    \brief Real-time clock class for general time-keeping and system uptime. 
*/

#ifndef __RTC_H__
#define __RTC_H__

#include <stdint.h>

//---------------------------------------------------------------------------
#define SECONDS_PER_MINUTE      (60)
#define MINUTES_PER_HOUR        (60)
#define HOURS_PER_DAY           (24)
#define YEAR_CALENDAR_STARTS    (2012)

//---------------------------------------------------------------------------
typedef enum 
{
    MONTH_JANUARY,
    MONTH_FEBRUARY,
    MONTH_MARCH,
    MONTH_APRIL,
    MONTH_MAY,
    MONTH_JUNE,
    MONTH_JULY,
    MONTH_AUGUST,
    MONTH_SEPTEMBER,
    MONTH_OCTOBER,
    MONTH_NEVEMBER,
    MONTH_DECEMBER,
//--
    MONTHS_PER_YEAR
} month_t;

//---------------------------------------------------------------------------
typedef enum
{
    DAY_SUNDAY,
    DAY_MONDAY,
    DAY_TUESDAY,
    DAY_WEDNESDAY,
    DAY_THURSDAY,
    DAY_FRIDAY,
    DAY_SATURDAY,
//--
    DAYS_PER_WEEK
} day_t;

//---------------------------------------------------------------------------
typedef struct {
    uint8_t  u8Day;
    day_t    eDayOfWeek;
    month_t  eMonth;
    uint16_t u16Year;
    uint8_t  u8Hour;
    uint8_t  u8Minute;
    uint8_t  u8Second;
    uint32_t u32Ticks;
} calendar_t;

//---------------------------------------------------------------------------
class RTC 
{
public:
    void Init(uint32_t u32TicksPerSecond_);
    bool SetDateTime(const calendar_t* pstCal_);
    bool GetDateTime(calendar_t* pstCal_);
    bool GetUptime(uint32_t* pu32Seconds_, uint32_t* pu32Ticks_);
    void AddTime(uint32_t u32Ticks_);
    const char* GetMonthName();
    const char* GetDayOfWeek();

private:
    void ComputeDayOfWeek();
    bool YearContainsLeapDay(uint16_t u16Year_);
    
    calendar_t m_stCalendar;
    uint32_t   m_u32TicksPerSecond;
    uint32_t   m_u32Seconds;
};

#endif //__RTC_H__

