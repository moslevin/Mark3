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
namespace Mark3 {
//---------------------------------------------------------------------------
static const uint8_t s_au8DaysPerMonth[] = { 31, // jan
                                             28, // feb
                                             31, // march
                                             30, // april
                                             31, // may
                                             30, // june
                                             31, // july
                                             31, // august
                                             30, // september
                                             31, // october
                                             30, // november
                                             31 };

//---------------------------------------------------------------------------
static const char* s_szMonthNames[] = { "January", "February", "March",     "April",   "May",      "June",
                                        "July",    "August",   "September", "October", "November", "December" };

//---------------------------------------------------------------------------
static const char* s_szDayNames[] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };

//---------------------------------------------------------------------------
void RTC::Init(uint32_t u32TicksPerSecond_)
{
    m_stCalendar.u8Day    = 0;
    m_stCalendar.eMonth   = Month::January;
    m_stCalendar.u16Year  = 2016;
    m_stCalendar.u8Hour   = 0;
    m_stCalendar.u8Minute = 0;
    m_stCalendar.u8Second = 0;
    m_stCalendar.u32Ticks = 0;

    m_u32TicksPerSecond = u32TicksPerSecond_;
    m_u32Seconds        = 0;
}

//---------------------------------------------------------------------------
bool RTC::GetDateTime(calendar_t* pstCal_)
{
    if (pstCal_ == 0) {
        return false;
    }
    *pstCal_ = m_stCalendar;
    pstCal_->u8Day++;
    return true;
}

//---------------------------------------------------------------------------
uint8_t RTC::SetDateTime(const calendar_t* pstCal_)
{
    if (pstCal_ == 0) {
        return 1;
    }

    // Month validation
    if (pstCal_->eMonth >= Month::Months_Per_Year) {
        return 2;
    }

    // Leap day validation check
    if ((pstCal_->eMonth == Month::February) && (pstCal_->u8Day > 29) && YearContainsLeapDay(pstCal_->u16Year)) {
        return 3;
    }
    // Day-of-the-month validation check
    if ((pstCal_->u8Day) == 0 || (pstCal_->u8Day > s_au8DaysPerMonth[static_cast<uint8_t>(pstCal_->eMonth)])) {
        return 4;
    }

    // Hour of the day validation
    if (pstCal_->u8Hour >= HOURS_PER_DAY) {
        return 5;
    }

    // Minutes per hour validation
    if (pstCal_->u8Minute >= MINUTES_PER_HOUR) {
        return 6;
    }

    // Seconds per minute validation
    if (pstCal_->u8Second >= SECONDS_PER_MINUTE) {
        return 7;
    }

    // Microsecond validation
    if (pstCal_->u32Ticks >= m_u32TicksPerSecond) {
        return 8;
    }

    // Checks out - copy in new data.
    m_stCalendar = *pstCal_;
    m_stCalendar.u8Day--;
    return 0;
}

//---------------------------------------------------------------------------
void RTC::AddTime(uint32_t u32Ticks_)
{
    m_stCalendar.u32Ticks += u32Ticks_;

    while (m_stCalendar.u32Ticks >= m_u32TicksPerSecond) {
        m_u32Seconds++;
        m_stCalendar.u32Ticks -= m_u32TicksPerSecond;
        m_stCalendar.u8Second++;
        if (m_stCalendar.u8Second != SECONDS_PER_MINUTE) {
            break;
        }

        m_stCalendar.u8Second = 0;
        m_stCalendar.u8Minute++;
        if (m_stCalendar.u8Minute != MINUTES_PER_HOUR) {
            break;
        }

        m_stCalendar.u8Minute = 0;
        m_stCalendar.u8Hour++;
        if (m_stCalendar.u8Hour != HOURS_PER_DAY) {
            break;
        }

        m_stCalendar.u8Hour = 0;
        m_stCalendar.u8Day++;

        if ((m_stCalendar.eMonth == Month::February) && YearContainsLeapDay(m_stCalendar.u16Year)) {
            if (m_stCalendar.u8Day != 29) {
                break;
            }
        } else {
            if (m_stCalendar.u8Day != s_au8DaysPerMonth[static_cast<uint8_t>(m_stCalendar.eMonth)]) {
                break;
            }
        }

        m_stCalendar.u8Day  = 0;
        int next_month = static_cast<int>(m_stCalendar.eMonth) + 1;
        m_stCalendar.eMonth = static_cast<Month>(next_month);
        if (m_stCalendar.eMonth != Month::Months_Per_Year) {
            break;
        }

        m_stCalendar.eMonth = Month::January;
        m_stCalendar.u16Year++;
    }
}

//---------------------------------------------------------------------------
const char* RTC::GetMonthName()
{
    return s_szMonthNames[static_cast<uint8_t>(m_stCalendar.eMonth)];
}

//---------------------------------------------------------------------------
const char* RTC::GetDayOfWeek()
{
    ComputeDayOfWeek();
    return s_szDayNames[static_cast<uint8_t>(m_stCalendar.eDayOfWeek)];
}

//---------------------------------------------------------------------------
bool RTC::GetUptime(uint32_t* pu32Seconds_, uint32_t* pu32Ticks_)
{
    if ((pu32Seconds_ == 0) || (pu32Ticks_ == 0)) {
        return false;
    }
    *pu32Seconds_ = m_u32Seconds;
    *pu32Ticks_   = m_stCalendar.u32Ticks;
    return true;
}

//---------------------------------------------------------------------------
bool RTC::YearContainsLeapDay(uint16_t u16Year_)
{
    if ((u16Year_ % 100) == 0) {
        return (u16Year_ % 400) == 0;
    } return (u16Year_ & 3) == 0;
}

//---------------------------------------------------------------------------
void RTC::ComputeDayOfWeek()
{
    auto u32Days = static_cast<uint32_t>((m_stCalendar.u16Year - YEAR_CALENDAR_STARTS) * 365);
    for (uint16_t u16Year = YEAR_CALENDAR_STARTS; u16Year < m_stCalendar.u16Year; u16Year += 4) {
        if (YearContainsLeapDay(u16Year)) {
            u32Days++;
        }
    }
    u32Days--;

    for (int iMonth = static_cast<int>(Month::January);
         iMonth < static_cast<int>(m_stCalendar.eMonth); iMonth++) {

        if (YearContainsLeapDay(m_stCalendar.u16Year) && (iMonth == static_cast<int>(Month::February))) {
            u32Days += 29;
        } else {
            u32Days += s_au8DaysPerMonth[iMonth];
        }
    }
    u32Days += m_stCalendar.u8Day + 1;
    u32Days %= static_cast<uint32_t>(Day::Days_Per_Week);

    m_stCalendar.eDayOfWeek = static_cast<Day>(u32Days);
}
} //namespace Mark3
