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

#ifndef NTPCLOCKGLOBALGETCLOCKMESSAGE_H
#define NTPCLOCKGLOBALGETCLOCKMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"
#include "Framework/Attributes/AttributeDateTime.h"

using namespace WaveNs;

namespace DcmNs
{

    class NTPClockGlobalGetClockMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            NTPClockGlobalGetClockMessage ();
            NTPClockGlobalGetClockMessage (const DateTime &ClockDateTime,const UI32 &MappedId, const string &ClockTimeZone);
            NTPClockGlobalGetClockMessage (const vector<DateTime> &ClockDateTime,const vector<UI32> &MappedId, const vector<string> &ClockTimeZone);
            virtual    ~NTPClockGlobalGetClockMessage ();
            void setClockDateTime(const DateTime &ClockDateTime);
            DateTime getClockDateTime()  const;
            void setMappedId(const UI32 &MappedId);
            UI32 getMappedId()  const;
            void setClockTimeZone(const string &ClockTimeZone);
            string getClockTimeZone()  const;

            UI32 getNumVectors() const;
			void clearNumVectors();
            void setVectorClockDateTime(const DateTime &ClockDateTime, const UI32 &MappedId, const string &ClockTimeZone);
            bool getVectorClockDateTime(const UI32 &index, DateTime &ClockDateTime, UI32 &MappedId, string &ClockTimeZone);

// Now the data members

        private:
        protected:
        public:
            DateTime  m_ClockDateTime;
            UI32  m_MappedId;
			string m_ClockTimeZone;
			UI32	m_NumVectors;
            vector<DateTime>  m_vClockDateTime;
            vector<UI32>  m_vMappedId;
            vector<string> m_vClockTimeZone;
    };
}
#endif                                            //NTPCLOCKGLOBALGETCLOCKMESSAGE_H
