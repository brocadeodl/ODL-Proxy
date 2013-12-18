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
 *   Author : pbalacha                                                     *
 **************************************************************************/

#ifndef RMONCLIENTCREATERMONALARMMESSAGE_H
#define RMONCLIENTCREATERMONALARMMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class RmonClientCreateRmonAlarmMessage : public DcmManagementInterfaceMessage 
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            RmonClientCreateRmonAlarmMessage ();
            RmonClientCreateRmonAlarmMessage (const SI32 &alarmIndex,const string &snmpOid,const UI32 &interval,const UI32 &sampleType,const UI32 &risingThreshold,const SI32 &risingEventId,const UI32 &fallingThreshold,const SI32 &fallingEventId,const string &owner);
            virtual    ~RmonClientCreateRmonAlarmMessage ();
            void setAlarmIndex(const SI32 &alarmIndex);
            SI32 getAlarmIndex()  const;
            void setSnmpOid(const string &snmpOid);
            string getSnmpOid()  const;
            void setInterval(const UI32 &interval);
            UI32 getInterval()  const;
			void setSampleType(const UI32 &sampleType);
            UI32 getSampleType()  const;
            void setRisingThreshold(const UI32 &risingThreshold);
            UI32 getRisingThreshold()  const;
            void setRisingEventId(const SI32 &risingEventId);
            SI32 getRisingEventId()  const;
            void setFallingThreshold(const UI32 &fallingThreshold);
            UI32 getFallingThreshold()  const;
            void setFallingEventId(const SI32 &fallingEventId);
            SI32 getFallingEventId()  const;
            void setOwner(const string &owner);
            string getOwner()  const;

			virtual const void *getCStructureForInputs    ();
            virtual void  loadOutputsFromCStructure (const void *pOutputCStructure);

// Now the data members

        private:
        protected:
        public:
            SI32  m_alarmIndex;
            string  m_snmpOid;
            UI32  m_interval;
            UI32 m_sampleType;
            UI32  m_risingThreshold;
            SI32  m_risingEventId;
            UI32  m_fallingThreshold;
            SI32  m_fallingEventId;
            string  m_owner;
    };
}
#endif                                            //RMONCLIENTCREATERMONALARMMESSAGE_H
