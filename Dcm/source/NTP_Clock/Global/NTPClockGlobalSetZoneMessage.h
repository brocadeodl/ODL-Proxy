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
 *   Author : noku                                                     *
 **************************************************************************/

#ifndef NTPCLOCKGLOBALSETZONEMESSAGE_H
#define NTPCLOCKGLOBALSETZONEMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class NTPClockGlobalSetZoneMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            NTPClockGlobalSetZoneMessage ();
            NTPClockGlobalSetZoneMessage (const UI32 &Timezone);
            virtual    ~NTPClockGlobalSetZoneMessage ();
			string convertZoneEnumToStr(const UI32 &Timezone);
            void setTimezone(const UI32 &Timezone);
            UI32 getTimezone()  const;
			string getTimeStr() const;

// Now the data members

        private:
        protected:
        public:
            UI32  m_Timezone;
			string m_Timestr;
    };
}
#endif                                            //NTPCLOCKGLOBALSETZONEMESSAGE_H
