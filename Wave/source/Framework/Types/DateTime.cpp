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

#include "Framework/Types/DateTime.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/TimeZoneToolKit.h"
#include <time.h>

//extern char *tzname[2]; // specifies IST etc
extern long timezone; // specifies offset from UTC in secs
//extern int daylight; // specifies if daylight savings is in effect

namespace WaveNs
{

DateTime::DateTime ()
    : m_Micro(0),
      m_dateSeperator ('-'),
      m_timeSeperator (':'),
      m_microSeperator('.'),
      m_betweenDateTime('T')
{
    time_t defaultTime  = {0};
    struct tm *timeInfo = NULL;
    struct tm result    = {0};

    time(&defaultTime); 
    timeInfo = localtime_r (&defaultTime, &result);

    m_Month = timeInfo->tm_mon + 1;
    m_Day  = timeInfo->tm_mday;
    m_Year  = timeInfo->tm_year + 1900;

    m_Hour = timeInfo->tm_hour;
    m_Min  = timeInfo->tm_min;
    m_Sec  = timeInfo->tm_sec;

    m_TimezoneHours = TimeZoneToolKit::getTimeZoneHours ();
    m_TimezoneMinutes =  TimeZoneToolKit::getTimeZoneMinutes ();
    m_positiveNegativeOffset = TimeZoneToolKit::getPostiveNegativeOffset ();
    m_zoneSeperator = TimeZoneToolKit::getZoneSeperator ();
    
}

DateTime::DateTime (const string &datetimeInStringFormat)
{
    fromString (datetimeInStringFormat);
}

DateTime::DateTime (const time_t &datetimeInTimeT)
    : m_Micro(0),
      m_dateSeperator ('-'),
      m_timeSeperator (':'),
      m_microSeperator('.'),
      m_betweenDateTime('T')
{
    fromTimeT (datetimeInTimeT);
}

DateTime::DateTime (const DateTime &datetime)
{
    m_Month             = datetime.m_Month;
    m_Day               = datetime.m_Day;
    m_Year              = datetime.m_Year;

    m_Hour              = datetime.m_Hour;
    m_Min               = datetime.m_Min;
    m_Sec               = datetime.m_Sec;
    m_Micro             = datetime.m_Micro;

    m_TimezoneHours     = datetime.m_TimezoneHours;
    m_TimezoneMinutes   = datetime.m_TimezoneMinutes;

    m_dateSeperator     = datetime.m_dateSeperator;
    m_timeSeperator     = datetime.m_timeSeperator;
    m_microSeperator    = datetime.m_microSeperator;
    m_zoneSeperator     = datetime.m_zoneSeperator;

    m_positiveNegativeOffset = datetime.m_positiveNegativeOffset;
    m_betweenDateTime = datetime.m_betweenDateTime;
}

DateTime::~DateTime ()
{
    m_Month = 0;
    m_Day   = 0;
    m_Year  = 0;

    m_Hour  = 0;
    m_Min   = 0;
    m_Sec   = 0;
    m_Micro = 0;

    m_TimezoneHours = 0;
    m_TimezoneMinutes = 0;
}

string DateTime::toString () const
{
    string datetimeInStringFormat;
    char buffer[64] = {0};

    // YYYY-MM-DD HH:MM:SS.Micro<+|->TimezoneHours:TimezoneMinutes
    //sprintf(buffer,"%04u%c%02u%c%02u%c%02u%c%02u%c%02u%c%06u%c%02u%c%02u", m_Year, m_dateSeperator, m_Month, m_dateSeperator, m_Day, m_betweenDateTime, m_Hour, m_timeSeperator, m_Min, m_timeSeperator, m_Sec, m_microSeperator, m_Micro, m_positiveNegativeOffset, m_TimezoneHours, m_zoneSeperator, m_TimezoneMinutes);
    //sprintf(buffer,"%04u%c%02u%c%02u %02u%c%02u%c%02u%c%02d%c%02d", m_Year, m_dateSeperator, m_Month, m_dateSeperator, m_Day, m_Hour, m_timeSeperator, m_Min, m_timeSeperator, m_Sec, m_positiveNegativeOffset, m_TimezoneHours, m_zoneSeperator, m_TimezoneMinutes);
    snprintf(buffer, 64, "%04u%c%02u%c%02u %02u%c%02u%c%02u", m_Year, m_dateSeperator, m_Month, m_dateSeperator, m_Day, m_Hour, m_timeSeperator, m_Min, m_timeSeperator, m_Sec);

    datetimeInStringFormat = buffer;

    //trace (TRACE_LEVEL_INFO, string("DateTime::toString: ") + datetimeInStringFormat);
    //printf("year:%d, dsep1:%c, month:%d, dsep2:%c, day:%d\n", m_Year, m_dateSeperator, m_Month, m_dateSeperator, m_Day);
    //printf("hour:%d, tsep1:%c, min:%d, tsep2:%c, sec:%d, msep:%c, micro:%d\n", m_Hour, m_timeSeperator, m_Min, m_timeSeperator, m_Sec, m_microSeperator, m_Micro);
    //printf("pnoffset:%c, tzhrs:%d, zsep:%c, tzmins:%d\n", m_positiveNegativeOffset, m_TimezoneHours, m_zoneSeperator, m_TimezoneMinutes);

    return (datetimeInStringFormat);
}

void DateTime::fromString(const string &datetimeInStringFormat)
{
    UI32 year, month, day, hour, min, sec;
    //SI32 tzhrs, tzmins;
    //UI32 micro;
    char dsep1, dsep2, tsep1, tsep2;
    //char zsep, pnoffset;
    //char msep, betweendt;

    //sscanf(datetimeInStringFormat.c_str(), "%04u%c%02u%c%02u%c%02u%c%02u%c%02u%c%06u%c%02u%c%02u", &year, &dsep1, &month, &dsep2, &day, &betweendt, &hour, &tsep1, &min, &tsep2, &sec, &msep, &micro, &pnoffset, &tzhrs, &zsep, &tzmins);
    //sscanf(datetimeInStringFormat.c_str(), "%04u%c%02u%c%02u %02u%c%02u%c%02u%c%02d%c%02d", &year, &dsep1, &month, &dsep2, &day, &hour, &tsep1, &min, &tsep2, &sec, &pnoffset, &tzhrs, &zsep, &tzmins);
    sscanf(datetimeInStringFormat.c_str(), "%04u%c%02u%c%02u %02u%c%02u%c%02u", &year, &dsep1, &month, &dsep2, &day, &hour, &tsep1, &min, &tsep2, &sec);

    //trace (TRACE_LEVEL_INFO, string("DateTime::fromString: arg: ") + datetimeInStringFormat);
    //printf("year:%d, dsep1:%c, month:%d, dsep2:%c, day:%d\n", year, dsep1, month, dsep2, day);
    //printf("hour:%d, tsep1:%c, min:%d, tsep2:%c, sec:%d, msep:%c, micro:%d\n", hour, tsep1, min, tsep2, sec, msep, micro);
    //printf("pnoffset:%c, tzhrs:%d, zsep:%c, tzmins:%d\n", pnoffset, tzhrs, zsep, tzmins);

    m_Month = (UI8)month;
    m_Day   = (UI8)day;
    m_Year  = (SI16)year;

    m_Hour  = (UI8)hour;
    m_Min   = (UI8)min;
    m_Sec   = (UI8)sec;
    //m_Micro = (UI32)micro;

    //m_TimezoneHours = tzhrs;
    //m_TimezoneMinutes = tzmins;

    m_dateSeperator = dsep1;
    m_timeSeperator = tsep1;
    //m_microSeperator = msep;
    //m_zoneSeperator = zsep;

    //m_positiveNegativeOffset = pnoffset;
    //m_betweenDateTime = betweendt;
}


ResourceId DateTime::loadFromPlainString(const string &datetimeInStringFormat)
{
    UI32 year, month, day, hour, min, sec;
    char dsep1, dsep2, tsep1, tsep2;
    ResourceId  status = WAVE_MESSAGE_ERROR;

    if ( 10 == sscanf(datetimeInStringFormat.c_str(), "%04u%c%02u%c%02u %02u%c%02u%c%02u", &year, &dsep1, &month, &dsep2, &day, &hour, &tsep1, &min, &tsep2, &sec))
    {
        m_Month = (UI8)month;
        m_Day   = (UI8)day;
        m_Year  = (SI16)year;
      
        m_Hour  = (UI8)hour;
        m_Min   = (UI8)min;
        m_Sec   = (UI8)sec;
      
        m_dateSeperator = dsep1;
        m_timeSeperator = tsep1;
        
        status = WAVE_MESSAGE_SUCCESS;
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, string("DateTime::loadFromPlainString : Improper format of date it should be in this format YYYY-MM-DD <space> HH:MM:SS"));
    }

    return status;

}

void DateTime::fromTimeT(const time_t &datetimeInTimeT)
{
    struct tm *timeInfo = NULL;
    struct tm result    = {0};

    timeInfo = localtime_r (&datetimeInTimeT, &result);

    m_Month = timeInfo->tm_mon + 1;
    m_Day  = timeInfo->tm_mday;
    m_Year  = timeInfo->tm_year + 1900;

    m_Hour = timeInfo->tm_hour;
    m_Min  = timeInfo->tm_min;
    m_Sec  = timeInfo->tm_sec;

    m_TimezoneHours = TimeZoneToolKit::getTimeZoneHours ();
    m_TimezoneMinutes =  TimeZoneToolKit::getTimeZoneMinutes ();
    m_positiveNegativeOffset = TimeZoneToolKit::getPostiveNegativeOffset ();
    m_zoneSeperator = TimeZoneToolKit::getZoneSeperator ();
    
}
		
bool DateTime::operator == (const DateTime &datetime) const
{
    if ((m_Year != datetime.m_Year) || (m_Day != datetime.m_Day) || (m_Month != datetime.m_Month) || (m_dateSeperator != datetime.m_dateSeperator))
    {
        return (false);
    }

    if ((m_Hour != datetime.m_Hour) || (m_Min != datetime.m_Min) || (m_Sec != datetime.m_Sec) || (m_timeSeperator != datetime.m_timeSeperator))
    {
        return (false);
    }

    return (true);
}

bool DateTime::operator != (const DateTime &datetime) const
{
    return (! (datetime == (*this)));
}

DateTime &DateTime::operator = (const DateTime &datetime)
{
    m_Year = datetime.m_Year;
    m_Day = datetime.m_Day;
    m_Month = datetime.m_Month;

    m_Hour = datetime.m_Hour;
    m_Min = datetime.m_Min;
    m_Sec = datetime.m_Sec;
    m_Micro = datetime.m_Micro;

    m_TimezoneHours = datetime.m_TimezoneHours;
    m_TimezoneMinutes = datetime.m_TimezoneMinutes;

    m_dateSeperator = datetime.m_dateSeperator;
    m_timeSeperator = datetime.m_timeSeperator;
    m_microSeperator = datetime.m_microSeperator;
    m_zoneSeperator = datetime.m_zoneSeperator;

    m_positiveNegativeOffset = datetime.m_positiveNegativeOffset;
    m_betweenDateTime = datetime.m_betweenDateTime;

    return (*this);
}

UI8 DateTime::getMonth(void) const
{
    return(m_Month);
}

UI8 DateTime::getDay(void) const
{
    return(m_Day);
}

SI16 DateTime::getYear(void) const
{
    return(m_Year);
}

UI8 DateTime::getHour(void) const
{
    return(m_Hour);
}

UI8 DateTime::getMin(void) const
{
    return(m_Min);
}

UI8 DateTime::getSec(void) const
{
    return(m_Sec);
}

UI32 DateTime::getMicro(void) const
{
    return(m_Micro);
}

SI8 DateTime::getTimezoneHours(void) const
{
    return(m_TimezoneHours);
}

SI8 DateTime::getTimezoneMinutes(void) const
{
    return(m_TimezoneMinutes);
}

void DateTime::setAllValues(SI16 year, UI8 month, UI8 day, UI8 hour, UI8 min, UI8 sec, UI32 micro, SI8 timezone, SI8 timezoneminutes)
{
    m_Year = year;
    m_Month = month;
    m_Day = day;
    m_Hour = hour;
    m_Min = min;
    m_Sec = sec;
    m_Micro = micro;
    m_TimezoneHours = timezone;
    m_TimezoneMinutes = timezoneminutes;
}

UI64 DateTime::getUI64Representation ()
{
    UI64 timeStamp = 0;

    struct tm configurationTime = {0};

    configurationTime.tm_sec  = (SI32) m_Sec;
    configurationTime.tm_min  = (SI32) m_Min;
    configurationTime.tm_hour = (SI32) m_Hour;
    configurationTime.tm_mday = (SI32) m_Day;
    configurationTime.tm_mon  = ((SI32) m_Month) - 1;
    configurationTime.tm_year = ((SI32) m_Year) - 1900;  // passing 'years since 1900' as expected by mktime.

    SI32 configurationTimeZoneOffset    = (60 * 60 * ((SI32) m_TimezoneHours)) + (60 * ((SI32) m_TimezoneMinutes));
    configurationTime.tm_sec           -= configurationTimeZoneOffset;
    configurationTime.tm_sec           += (60 * 60 * ((SI32) TimeZoneToolKit::getTimeZoneHours ())) + (60 * ((SI32) TimeZoneToolKit::getTimeZoneMinutes ()));

    time_t configurationTimeSeconds     = mktime (&configurationTime);

    timeStamp = (UI64) configurationTimeSeconds;

    return timeStamp;
}

string DateTime::getPlainString () const
{
    char buffer[64];

    snprintf (buffer, 64, "%04u%c%02u%c%02uT%02u%c%02u%c%02u", m_Year, m_dateSeperator, m_Month, m_dateSeperator, m_Day, m_Hour, m_timeSeperator, m_Min, m_timeSeperator, m_Sec);

    string datetimeInStringFormat = buffer;

    return datetimeInStringFormat;
}

}
