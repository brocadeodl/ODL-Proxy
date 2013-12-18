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

#ifndef QOSCREATEQOSQUEUEMULTICASTSCHEDULERSPMESSAGE_H
#define QOSCREATEQOSQUEUEMULTICASTSCHEDULERSPMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class QosCreateQosQueueMulticastSchedulerSPMessage : public PrismMessage 
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            QosCreateQosQueueMulticastSchedulerSPMessage ();
            QosCreateQosQueueMulticastSchedulerSPMessage (const SI32 &dwrrTrafficClass0,const SI32 &dwrrTrafficClass1,const SI32 &dwrrTrafficClass2,const SI32 &dwrrTrafficClass3,const SI32 &dwrrTrafficClass4,const SI32 &dwrrTrafficClass5,const SI32 &dwrrTrafficClass6,const SI32 &dwrrTrafficClass7);
            virtual    ~QosCreateQosQueueMulticastSchedulerSPMessage ();
            void setDwrrTrafficClass0(const SI32 &dwrrTrafficClass0);
            SI32 getDwrrTrafficClass0()  const;
            void setDwrrTrafficClass1(const SI32 &dwrrTrafficClass1);
            SI32 getDwrrTrafficClass1()  const;
            void setDwrrTrafficClass2(const SI32 &dwrrTrafficClass2);
            SI32 getDwrrTrafficClass2()  const;
            void setDwrrTrafficClass3(const SI32 &dwrrTrafficClass3);
            SI32 getDwrrTrafficClass3()  const;
            void setDwrrTrafficClass4(const SI32 &dwrrTrafficClass4);
            SI32 getDwrrTrafficClass4()  const;
            void setDwrrTrafficClass5(const SI32 &dwrrTrafficClass5);
            SI32 getDwrrTrafficClass5()  const;
            void setDwrrTrafficClass6(const SI32 &dwrrTrafficClass6);
            SI32 getDwrrTrafficClass6()  const;
            void setDwrrTrafficClass7(const SI32 &dwrrTrafficClass7);
            SI32 getDwrrTrafficClass7()  const;

// Now the data members

        private:
        protected:
        public:
            SI32  m_dwrrTrafficClass0;
            SI32  m_dwrrTrafficClass1;
            SI32  m_dwrrTrafficClass2;
            SI32  m_dwrrTrafficClass3;
            SI32  m_dwrrTrafficClass4;
            SI32  m_dwrrTrafficClass5;
            SI32  m_dwrrTrafficClass6;
            SI32  m_dwrrTrafficClass7;
    };
}
#endif                                            //QOSCREATEQOSQUEUEMULTICASTSCHEDULERSPMESSAGE_H
