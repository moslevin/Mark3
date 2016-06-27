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
    \file rtc.cpp
    \brief Real-time clock class for general time-keeping and system uptime. 
*/

#include "rtc.h"

//---------------------------------------------------------------------------
static const uint8_t s_au8DaysPerMonth[] = 
{
    31, //jan
    28, //feb
    31, //march
    30, //april
    31, //may
    30, //june
    31, //july
    31, //august
    30, //september
    31, //october
    30, //november
    31    
};

//---------------------------------------------------------------------------
static const char* s_szMonthNames[] = 
{
    "January",
    "February",
    "March",
    "April",
    "May",
    "June",
    "July",
    "August",
    "September",
    "October",
    "November",
    "December"
};

//---------------------------------------------------------------------------
static const char* s_szDayNames[] = 
{
    "Sunday",
    "Monday",
    "Tuesday",
    "Wednesday",
    "Thursday",
    "Friday",
    "Saturday"
};

//---------------------------------------------------------------------------
void RTC::Init(uint32_t u32TicksPerSecond_)
{
    m_stCalendar.u8Day = 0;
    m_stCalendar.eMonth = MONTH_JANUARY;
    m_stCalendar.u16Year = 2016;
    m_stCalendar.u8Hour = 0;
    m_stCalendar.u8Minute = 0;
    m_stCalendar.u8Second = 0;
    m_stCalendar.u32Ticks = 0;

    m_u32TicksPerSecond = u32TicksPerSecond_;
    m_u32Seconds = 0;
}

//---------------------------------------------------------------------------
bool RTC::GetDateTime(calendar_t* pstCal_)
{
    if (!pstCal_)
    {
        return false;
    }
    *pstCal_ = m_stCalendar;
    pstCal_->u8Day++;
}

//---------------------------------------------------------------------------
bool RTC::SetDateTime(const calendar_t* pstCal_)
{
    if (!pstCal_) 
    {
        return false;
    }

    // Month validation
    if (pstCal_->eMonth >= MONTHS_PER_YEAR)
    {
        return false;
    }

    // Leap day validation check
    if ((pstCal_->eMonth == MONTH_FEBRUARY) &&
        (pstCal_->u8Day > 29) &&
        YearContainsLeapDay(pstCal_->u16Year))
    {
        return false;
    }
    // Day-of-the-month validation check
    else if ((pstCal_->u8Day) == 0 || (pstCal_->u8Day > s_au8DaysPerMonth[pstCal_->eMonth])) 
    {
        return false;
    }

    // Hour of the day validation
    if (pstCal_->u8Hour >= HOURS_PER_DAY)
    {
        return false;
    }

    // Minutes per hour validation
    if (pstCal_->u8Minute >= MINUTES_PER_HOUR)
    {
        return false;
    }

    // Seconds per minute validation
    if (pstCal_->u8Second >= SECONDS_PER_MINUTE)
    {
        return false;
    }

    // Microsecond validation
    if (pstCal_->u32Ticks >= m_u32TicksPerSecond)
    {
        return false;
    }

    // Checks out - copy in new data.
    m_stCalendar = *pstCal_;
    m_stCalendar.u8Day--;
    return true;
}

//---------------------------------------------------------------------------
void RTC::AddTime(uint32_t u32Ticks_)
{
    m_stCalendar.u32Ticks += u32Ticks_;
 
    while (m_stCalendar.u32Ticks >= m_u32TicksPerSecond)
    {
        m_u32Seconds++;
        m_stCalendar.u32Ticks -= m_u32TicksPerSecond;
        m_stCalendar.u8Second++;
        if (m_stCalendar.u8Second != SECONDS_PER_MINUTE)
        {
            break;
        }

        m_stCalendar.u8Second = 0;
        m_stCalendar.u8Minute++;
        if (m_stCalendar.u8Minute != MINUTES_PER_HOUR)
        {
            break;
        }

        m_stCalendar.u8Minute = 0;
        m_stCalendar.u8Hour++;
        if (m_stCalendar.u8Hour != HOURS_PER_DAY)
        {
            break;
        }
        
        m_stCalendar.u8Hour = 0;
        m_stCalendar.u8Day++;

        if ((m_stCalendar.eMonth == MONTH_FEBRUARY) && YearContainsLeapDay(m_stCalendar.u16Year))
        {            
            if (m_stCalendar.u8Day != 29)
            {
                break;
            }
        }
        else
        {
            if (m_stCalendar.u8Day != s_au8DaysPerMonth[m_stCalendar.eMonth])
            {
                break;
            }
        }
        
        m_stCalendar.u8Day = 0;
        m_stCalendar.eMonth = (month_t)((int)m_stCalendar.eMonth + 1);
        if (m_stCalendar.eMonth != MONTHS_PER_YEAR)
        {
            break;
        }

        m_stCalendar.eMonth = MONTH_JANUARY;
        m_stCalendar.u16Year++;
    }
}

//---------------------------------------------------------------------------
const char* RTC::GetMonthName()
{
    return s_szMonthNames[m_stCalendar.eMonth];
}

//---------------------------------------------------------------------------
const char* RTC::GetDayOfWeek()
{
    ComputeDayOfWeek();
    return s_szDayNames[m_stCalendar.eDayOfWeek];
}

//---------------------------------------------------------------------------
bool RTC::GetUptime(uint32_t* pu32Seconds_, uint32_t* pu32Ticks_)
{
    if (!pu32Seconds_ || !pu32Ticks_)
    {
        return false;
    }
    *pu32Seconds_ = m_u32Seconds;
    *pu32Ticks_ = m_stCalendar.u32Ticks;
}

//---------------------------------------------------------------------------
bool RTC::YearContainsLeapDay(uint16_t u16Year_)
{
    if ((u16Year_ % 100) == 0) 
    {
        if ((u16Year_ % 400) == 0)
        {
            return true;
        }
        return false;
    }
    else if ((u16Year_ & 3) == 0)
    {
        return true;
    }
    return false;
}

//---------------------------------------------------------------------------
void RTC::ComputeDayOfWeek()
{
    uint32_t u32Days;
    u32Days = (uint32_t)(m_stCalendar.u16Year - YEAR_CALENDAR_STARTS) * 365;
    for (uint16_t u16Year = YEAR_CALENDAR_STARTS; u16Year < m_stCalendar.u16Year; u16Year+=4) 
    {
        if (YearContainsLeapDay(u16Year)) 
        {
            u32Days++;
        }
    }
    u32Days --;

    for (month_t eMonth = MONTH_JANUARY; eMonth < m_stCalendar.eMonth; eMonth = (month_t)((int)eMonth + 1)) 
    {
        if (YearContainsLeapDay(m_stCalendar.u16Year) && (eMonth == MONTH_FEBRUARY)) 
        {
            u32Days += 29;
        }
        else
        {
            u32Days += s_au8DaysPerMonth[eMonth];
        }
    }
    u32Days += m_stCalendar.u8Day + 1;
    u32Days %= (uint32_t)(DAYS_PER_WEEK);
    
    m_stCalendar.eDayOfWeek = (day_t)(u32Days);
}

