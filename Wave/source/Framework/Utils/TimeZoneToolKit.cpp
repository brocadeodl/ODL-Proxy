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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Anand Kumar Subramanian                                      *
 ***************************************************************************/

#include "Framework/Utils/TimeZoneToolKit.h"

extern long timezone; // specifies offset from UTC in secs

namespace WaveNs
{

SI8         TimeZoneToolKit::s_TimeZoneHours = 0;
SI8         TimeZoneToolKit::s_TimeZoneMinutes = 0;
char        TimeZoneToolKit::s_positiveNegativeOffset = '+';
char        TimeZoneToolKit::s_zoneSeperator = ':' ;

void TimeZoneToolKit::initialize ()
{
    tzset(); // get time zone details in the extern variables above

    s_TimeZoneHours = (timezone / 3600);
    s_TimeZoneMinutes = ((timezone % 3600) / 60);

    if (s_TimeZoneHours > 0)
    {
        s_positiveNegativeOffset = '+';
    }
    else if ((s_TimeZoneHours == 0) && (s_TimeZoneMinutes > 0))
    {
        s_positiveNegativeOffset = '+';
    }
    else
    {
        s_positiveNegativeOffset = '-';
    }

}

SI8  TimeZoneToolKit::getTimeZoneHours () 
{
    return s_TimeZoneHours;
}

void TimeZoneToolKit::setTimeZoneHours (const SI8 timeZoneHours)
{
    s_TimeZoneHours = timeZoneHours;
}

SI8  TimeZoneToolKit::getTimeZoneMinutes ()
{
    return s_TimeZoneMinutes;
}

void TimeZoneToolKit::setTimeZoneMinutes (const SI8 timeZoneMinutes)
{
    s_TimeZoneMinutes = timeZoneMinutes;
}

char TimeZoneToolKit::getPostiveNegativeOffset ()
{
    return s_positiveNegativeOffset;
}

void TimeZoneToolKit::setPostiveNegativeOffset (const char postiveNegativeOffset)
{
    s_positiveNegativeOffset = postiveNegativeOffset;
}

char TimeZoneToolKit::getZoneSeperator ()
{
    return s_zoneSeperator;    
}

void TimeZoneToolKit::setZoneSeperator (const char zoneSeperator)
{
    s_zoneSeperator = zoneSeperator;
}

}
