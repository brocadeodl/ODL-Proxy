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

#include "Framework/Types/Time.h"
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

Time::Time ()
    : m_Micro(0),
      m_timeSeperator(':'),
      m_microSeperator('.')
{
    time_t defaultTime  = {0};
    struct tm *timeInfo = NULL;
    struct tm result    = {0};

    time(&defaultTime); 
    timeInfo = localtime_r (&defaultTime, &result);

    m_Hour = timeInfo->tm_hour;
    m_Min  = timeInfo->tm_min;
    m_Sec  = timeInfo->tm_sec;

    m_TimezoneHours = TimeZoneToolKit::getTimeZoneHours ();
    m_TimezoneMinutes =  TimeZoneToolKit::getTimeZoneMinutes ();
    m_positiveNegativeOffset = TimeZoneToolKit::getPostiveNegativeOffset ();
    m_zoneSeperator = TimeZoneToolKit::getZoneSeperator ();    
}

Time::Time (const string &timeInStringFormat)
{
    fromString (timeInStringFormat);
}

Time::Time (const struct tm *ptime)
{
    m_Hour = ptime->tm_hour;
    m_Min  = ptime->tm_min;
    m_Sec  = ptime->tm_sec;
    m_Micro = 0;

    m_TimezoneHours = 0;
    m_TimezoneMinutes = 0;

    m_timeSeperator = ':';
    m_zoneSeperator = ':';
    m_microSeperator = '.';

    m_positiveNegativeOffset = '+';
}

Time::Time (const Time &ptime)
{
    m_Hour = ptime.m_Hour;
    m_Min  = ptime.m_Min;
    m_Sec  = ptime.m_Sec;
    m_Micro = ptime.m_Micro;

    m_TimezoneHours = ptime.m_TimezoneHours;
    m_TimezoneMinutes = ptime.m_TimezoneMinutes;

    m_timeSeperator = ptime.m_timeSeperator;
    m_zoneSeperator = ptime.m_zoneSeperator;
    m_microSeperator = ptime.m_microSeperator;

    m_positiveNegativeOffset = ptime.m_positiveNegativeOffset;
}

Time::~Time ()
{
    m_Hour = 0;
    m_Min   = 0;
    m_Sec  = 0;
    m_Micro  = 0;
    m_TimezoneHours = 0;
    m_TimezoneMinutes = 0;
}

string Time::toString () const
{
    string timeInStringFormat;
    char buffer[32] = {0};

    // HH:MM:SS.Micro<+|->TimezoneHours:TimezoneMinutes
    // + or - above is based on timezone offset from UTC
    // e.g. 04:05:06.000789-8:00
    //sprintf(buffer,"%02u%c%02u%c%02u%c%06u%c%02d%c%02d", m_Hour, m_timeSeperator, m_Min, m_timeSeperator, m_Sec, m_microSeperator, m_Micro, m_positiveNegativeOffset, m_TimezoneHours, m_zoneSeperator, m_TimezoneMinutes);
    snprintf(buffer, 32, "%02u%c%02u%c%02u%c%06u", m_Hour, m_timeSeperator, m_Min, m_timeSeperator, m_Sec, m_microSeperator, m_Micro);

    timeInStringFormat = buffer;

    return (timeInStringFormat);
}

void Time::fromString(const string &timeInStringFormat)
{
    vector<string> tokens;
    vector<string> secTokens;
    tokenize (timeInStringFormat.c_str(), tokens, ':');

    m_Hour = strtoul(tokens[0].c_str(), NULL, 10);
    m_Min = strtoul(tokens[1].c_str(), NULL, 10);

    tokenize (tokens[2], secTokens, '.');
    m_Sec = strtoul(secTokens[0].c_str(), NULL, 10);
    if (secTokens.size() > 1)
        m_Micro = strtoul(secTokens[1].c_str(), NULL, 10);
    else
        m_Micro = 0;

    m_timeSeperator = ':';
    m_microSeperator = '.';

}

bool Time::operator == (const Time &time) const
{
    if ((m_Hour != time.m_Hour) || (m_Min != time.m_Min) || (m_Sec != time.m_Sec) || (m_timeSeperator != time.m_timeSeperator) || (m_Micro != time.m_Micro))
    {
        return (false);
    }
    return (true);
}

bool Time::operator != (const Time &time) const
{
    return (! (time == (*this)));
}

Time &Time::operator = (const Time &time)
{
    m_Hour = time.m_Hour;
    m_Min = time.m_Min;
    m_Sec = time.m_Sec;
    m_Micro = time.m_Micro;

    m_TimezoneHours = time.m_TimezoneHours;
    m_TimezoneMinutes = time.m_TimezoneMinutes;

    m_timeSeperator = time.m_timeSeperator;
    m_microSeperator = time.m_microSeperator;
    m_zoneSeperator = time.m_zoneSeperator;

    m_positiveNegativeOffset = time.m_positiveNegativeOffset;

    return (*this);
}

UI8 Time::getHour(void) const
{
    return(m_Hour);
}

UI8 Time::getMin(void) const
{
    return(m_Min);
}

UI8 Time::getSec(void) const
{
    return(m_Sec);
}

UI32 Time::getMicro(void) const
{
    return(m_Micro);
}

SI8 Time::getTimezoneHours(void) const
{
    return(m_TimezoneHours);
}

SI8 Time::getTimezoneMinutes(void) const
{
    return(m_TimezoneMinutes);
}

void Time::setAllValues(UI8 hour, UI8 min, UI8 sec, UI32 micro, SI8 timezone, SI8 timezoneminutes)
{
    m_Hour = hour;
    m_Min = min;
    m_Sec = sec;
    m_Micro = micro;
    m_TimezoneHours = timezone;
    m_TimezoneMinutes = timezoneminutes;
}

string Time::getPlainString () const
{
    char buffer[32];

    snprintf (buffer, 32, "%02u%c%02u%c%02u", m_Hour, m_timeSeperator, m_Min, m_timeSeperator, m_Sec);

    string timeInPlainStringFormat = buffer;

    if (0 != m_Micro)
    {
        snprintf (buffer, 32, "%c%06u", m_microSeperator, m_Micro);

        timeInPlainStringFormat += buffer;
    }

    return timeInPlainStringFormat;
}

}
