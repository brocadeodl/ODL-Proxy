/* Copyright (c) 2005-2013 Brocade Communications Systems, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/***************************************************************************
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Himanshu Varshney, Amit Agrawal                              *
 ***************************************************************************/

#include "Framework/Types/Date.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/TimeZoneToolKit.h"
#include <time.h>

extern long timezone; // specifies offset from UTC in secs
//extern char *tzname[2]; // specifies IST etc
//extern int daylight; // specifies if daylight savings is in effect

namespace WaveNs
{

Date::Date ()
    : m_seperator ('-')
{
    time_t defaultTime  = {0};
    struct tm *timeInfo = NULL;
    struct tm result    = {0};

    time(&defaultTime); 
    timeInfo = localtime_r (&defaultTime, &result);   

    m_Month = timeInfo->tm_mon + 1;
    m_Day  = timeInfo->tm_mday;
    m_Year  = timeInfo->tm_year + 1900;

    m_TimezoneHours = TimeZoneToolKit::getTimeZoneHours ();
    m_TimezoneMinutes =  TimeZoneToolKit::getTimeZoneMinutes ();
    m_positiveNegativeOffset = TimeZoneToolKit::getPostiveNegativeOffset ();
    m_zoneSeperator = TimeZoneToolKit::getZoneSeperator (); 
    
}

Date::Date (const string &dateInStringFormat)
{
    //trace (TRACE_LEVEL_INFO, string("Date::Date string arg:") + dateInStringFormat);
    fromString (dateInStringFormat);
}

Date::Date (const Date &date)
{
    m_Month             = date.m_Month;
    m_Day               = date.m_Day;
    m_Year              = date.m_Year;
    m_TimezoneHours     = date.m_TimezoneHours;
    m_TimezoneMinutes   = date.m_TimezoneMinutes;
    m_seperator         = date.m_seperator;
    m_positiveNegativeOffset = date.m_positiveNegativeOffset;
    m_zoneSeperator     = date.m_zoneSeperator;
}

Date::~Date ()
{
    m_Month = 0;
    m_Day   = 0;
    m_Year  = 0;
    m_TimezoneHours = 0;
    m_TimezoneMinutes = 0;
}

string Date::toString () const
{
    string dateInStringFormat;
    char buffer[32] = {0};

    // YYYY-MM-DD format as per http://www.postgresql.org/docs/8.4/static/datatype-datetime.html
    // but we are following what WyserEa prints which is 2010-05-21+08:00
    //sprintf(buffer,"%04u%c%02u%c%02u%c%02u%c%02u", m_Year, m_seperator, m_Month, m_seperator, m_Day, m_positiveNegativeOffset, m_TimezoneHours, m_zoneSeperator, m_TimezoneMinutes);
    snprintf(buffer, 32, "%04u%c%02u%c%02u", m_Year, m_seperator, m_Month, m_seperator, m_Day);

    dateInStringFormat = buffer;

    return (dateInStringFormat);
}

void Date::fromString(const string &dateInStringFormat)
{
    UI32    month, day, year;
    //UI32    tzhrs, tzmins;
    char    sep1, sep2;
    //char    pnoffset, zsep;

    // format as displayed by WyserEa is: 2010-05-21+08:00
    //sscanf(dateInStringFormat.c_str(), "%04u%c%02u%c%02u%c%02u%c%02u", &year, &sep1, &month, &sep2, &day, &pnoffset, &tzhrs, &zsep, &tzmins);
    sscanf(dateInStringFormat.c_str(), "%04u%c%02u%c%02u", &year, &sep1, &month, &sep2, &day);

    m_Month = (UI8)month;
    m_Day   = (UI8)day;
    m_Year  = (SI16)year;
    //m_TimezoneHours = (SI8)tzhrs;
    //m_TimezoneMinutes = (SI8)tzmins;

    m_seperator = sep1;
    //m_positiveNegativeOffset = pnoffset;
    //m_zoneSeperator = zsep;

    //trace (TRACE_LEVEL_INFO, string("Date::fromString: string:") + dateInStringFormat + string(" year:") + m_Year + string(" month:") + m_Month + string(" day:") + m_Day + string(" timezone:") + m_TimezoneHours + string(" timezone mins:") + m_TimezoneMinutes);
    //trace (TRACE_LEVEL_INFO, string("Date::fromString: string:") + dateInStringFormat + string(" year:") + m_Year + string(" month:") + m_Month + string(" day:") + m_Day);
}

ResourceId Date::loadFromPlainString(const string &dateInStringFormat)
{
    UI32        month, day, year;
    char        sep1, sep2;
    ResourceId  status = WAVE_MESSAGE_ERROR;

    if (5 == sscanf(dateInStringFormat.c_str(), "%04u%c%02u%c%02u", &year, &sep1, &month, &sep2, &day))
    {
        m_Month = (UI8)month;
        m_Day   = (UI8)day;
        m_Year  = (SI16)year;
        m_seperator = sep1;
        
        status = WAVE_MESSAGE_SUCCESS;
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, string("Date::loadFromPlainString : Improper format of date it should be in this format YYYY-MM-DD"));
    }

    return status;
}

bool Date::operator == (const Date &date) const
{
    if ((m_Year != date.m_Year) || (m_Day != date.m_Day) || (m_Month != date.m_Month) || (m_seperator != date.m_seperator))
    {
        return (false);
    }
    return (true);
}

bool Date::operator != (const Date &date) const
{
    return (! (date == (*this)));
}

Date &Date::operator = (const Date &date)
{
    m_Year = date.m_Year;
    m_Day = date.m_Day;
    m_Month = date.m_Month;
    m_TimezoneHours = date.m_TimezoneHours;
    m_TimezoneMinutes = date.m_TimezoneMinutes;

    m_seperator = date.m_seperator;
    m_positiveNegativeOffset = date.m_positiveNegativeOffset;
    m_zoneSeperator = date.m_zoneSeperator;

    return (*this);
}

SI16 Date::getYear (void) const
{
    return m_Year;
}

UI8 Date::getMonth (void) const
{
    return m_Month;
}

UI8 Date::getDay (void) const
{
    return m_Day;
}

SI8 Date::getTimezoneHours (void) const
{
    return m_TimezoneHours;
}

SI8 Date::getTimezoneMinutes (void) const
{
    return m_TimezoneMinutes;
}

void Date::setAllValues (SI16 year, UI8 month, UI8 day, SI8 timezone, SI8 timezoneminutes)
{
    m_Year = year;
    m_Month = month;
    m_Day = day;
    m_TimezoneHours = timezone;
    m_TimezoneMinutes = timezoneminutes;

    m_seperator = '-';
    m_zoneSeperator = ':';

    if (m_TimezoneHours > 0)
    {
        m_positiveNegativeOffset = '+';
    }
    else if ((m_TimezoneHours == 0) && (m_TimezoneMinutes > 0))
    {
        m_positiveNegativeOffset = '+';
    }
    else
    {
        m_positiveNegativeOffset = '-';
    }
}

string Date::getPlainString () const
{
    char buffer[32];

    snprintf (buffer, 32, "%04u%c%02u%c%02u%c%02u%c%02u", m_Year, m_seperator, m_Month, m_seperator, m_Day, m_positiveNegativeOffset, m_TimezoneHours, m_zoneSeperator, m_TimezoneMinutes);

    string dateInPlainStringFormat = buffer;

    return dateInPlainStringFormat;
}

}
