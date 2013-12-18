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

#ifndef QOSCREATEQOSRCVQUEUEMULTICASTSETTINGSPMESSAGE_H
#define QOSCREATEQOSRCVQUEUEMULTICASTSETTINGSPMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class QosCreateQosRcvQueueMulticastSettingSPMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            QosCreateQosRcvQueueMulticastSettingSPMessage ();
            QosCreateQosRcvQueueMulticastSettingSPMessage (const SI32 &rateLimit,const SI32 &burst,const SI32 &thresholdTrafficClass0,const SI32 &thresholdTrafficClass1,const SI32 &thresholdTrafficClass2,const SI32 &thresholdTrafficClass3,const SI32 &thresholdTrafficClass4,const SI32 &thresholdTrafficClass5,const SI32 &thresholdTrafficClass6,const SI32 &thresholdTrafficClass7,const SI32 &opCode);
            virtual    ~QosCreateQosRcvQueueMulticastSettingSPMessage ();
            void setRateLimit(const SI32 &rateLimit);
            SI32 getRateLimit()  const;
            void setBurst(const SI32 &burst);
            SI32 getBurst()  const;
            void setThresholdTrafficClass0(const SI32 &thresholdTrafficClass0);
            SI32 getThresholdTrafficClass0()  const;
            void setThresholdTrafficClass1(const SI32 &thresholdTrafficClass1);
            SI32 getThresholdTrafficClass1()  const;
            void setThresholdTrafficClass2(const SI32 &thresholdTrafficClass2);
            SI32 getThresholdTrafficClass2()  const;
            void setThresholdTrafficClass3(const SI32 &thresholdTrafficClass3);
            SI32 getThresholdTrafficClass3()  const;
            void setThresholdTrafficClass4(const SI32 &thresholdTrafficClass4);
            SI32 getThresholdTrafficClass4()  const;
            void setThresholdTrafficClass5(const SI32 &thresholdTrafficClass5);
            SI32 getThresholdTrafficClass5()  const;
            void setThresholdTrafficClass6(const SI32 &thresholdTrafficClass6);
            SI32 getThresholdTrafficClass6()  const;
            void setThresholdTrafficClass7(const SI32 &thresholdTrafficClass7);
            SI32 getThresholdTrafficClass7()  const;
            void setOpCode(const SI32 &opCode);
            SI32 getOpCode()  const;

// Now the data members

        private:
        protected:
        public:
            SI32  m_rateLimit;
            SI32  m_burst;
            SI32  m_thresholdTrafficClass0;
            SI32  m_thresholdTrafficClass1;
            SI32  m_thresholdTrafficClass2;
            SI32  m_thresholdTrafficClass3;
            SI32  m_thresholdTrafficClass4;
            SI32  m_thresholdTrafficClass5;
            SI32  m_thresholdTrafficClass6;
            SI32  m_thresholdTrafficClass7;
            SI32  m_opCode;
    };
}
#endif                                            //QOSCREATEQOSRCVQUEUEMULTICASTSETTINGSPMESSAGE_H
