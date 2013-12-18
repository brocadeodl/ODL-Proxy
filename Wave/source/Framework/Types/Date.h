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

#ifndef DATE_H
#define DATE_H

#include <string>
#include "Framework/Types/Types.h"

namespace WaveNs
{

class Date
{

    private:
    protected:
    public:
                        Date                 ();
                        Date                 (const string &dateInStringFormat);
                        Date                 (const Date &date);
                       ~Date                 ();
                                            
        string          toString             ()                                     const;
        void            fromString           (const string &dateInStringFormat);
        ResourceId      loadFromPlainString  (const string &dateInStringFormat);

        bool            operator ==          (const Date &date)                     const;
        bool            operator !=          (const Date &date)                     const;
        Date            &operator =          (const Date &date);
                       
        SI16            getYear              ()                                     const;
        UI8             getMonth             ()                                     const;
        UI8             getDay               ()                                     const;
        SI8             getTimezoneHours     ()                                     const;
        SI8             getTimezoneMinutes   ()                                     const;
        void            setAllValues         (SI16 year, UI8 month, UI8 day, SI8 timezone, SI8 timezoneminutes);
        string          getPlainString       ()                                     const;

    // Now the data members
    private:
        UI8         m_Month;
        UI8         m_Day;
        SI16        m_Year;
        SI8         m_TimezoneHours;
        SI8         m_TimezoneMinutes;
        char        m_seperator;
        char        m_positiveNegativeOffset;
        char        m_zoneSeperator;
    protected:
    public:
};

}

#endif //DATE_H
