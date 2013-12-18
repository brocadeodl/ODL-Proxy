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

#ifndef TIMEZONETOOLKIT
#define TIMEZONETOOLKIT

#include "Framework/Types/Types.h"
#include "Framework/Types/Date.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"
#include <time.h>

using namespace std;

namespace WaveNs
{

class TimeZoneToolKit
{
    private:
    protected:
    public:
        static void initialize                  ();
        static SI8  getTimeZoneHours            ();
        static void setTimeZoneHours            (const SI8 timeZoneHours);
        static SI8  getTimeZoneMinutes          ();
        static void setTimeZoneMinutes          (const SI8 timeZoneMinutes);
        static char getPostiveNegativeOffset    ();
        static void setPostiveNegativeOffset    (const char postiveNegativeOffset);
        static char getZoneSeperator            ();
        static void setZoneSeperator            (const char zoneSeperator);

       // Now the data members

    private :

        static SI8         s_TimeZoneHours;
        static SI8         s_TimeZoneMinutes;
        static char        s_positiveNegativeOffset;
        static char        s_zoneSeperator;

    protected :
    public :

};

}

#endif // TIMEZONETOOLKIT
