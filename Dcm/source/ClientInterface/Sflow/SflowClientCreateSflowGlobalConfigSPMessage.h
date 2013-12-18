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
 *   Author : vsatyana                                                     *
 **************************************************************************/

#ifndef SFLOWCLIENTCREATESFLOWGLOBALCONFIGSPMESSAGE_H
#define SFLOWCLIENTCREATESFLOWGLOBALCONFIGSPMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include "Framework/Types/IpVxAddress.h"

using namespace WaveNs;

namespace DcmNs
{

    class SflowClientCreateSflowGlobalConfigSPMessage : public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            SflowClientCreateSflowGlobalConfigSPMessage ();
            SflowClientCreateSflowGlobalConfigSPMessage (const bool &enable,const IpVxAddress &collector,const UI32 &portNumber,const UI32 &pollingInterval,const UI32 &sampleRate,const UI32 &opCode);
            virtual    ~SflowClientCreateSflowGlobalConfigSPMessage ();
            void setEnable(const bool &enable);
            bool getEnable()  const;
            void setCollector(const IpVxAddress &collector);
            IpVxAddress getCollector()  const;
            void setPortNumber(const UI32 &portNumber);
            UI32 getPortNumber()  const;
            void setCollectorAddressType(const UI32 &type);
            UI32 getCollectorAddressType()  const;
            void setPollingInterval(const UI32 &pollingInterval);
            UI32 getPollingInterval()  const;
            void setSampleRate(const UI32 &sampleRate);
            UI32 getSampleRate()  const;
            void setOpCode(const UI32 &opCode);
            UI32 getOpCode()  const;
            virtual const void *getCStructureForInputs();
            virtual void  loadOutputsFromCStructure(const void *pOutputCStructure);


// Now the data members

        private:
        protected:
        public:
            bool  m_enable;
            IpVxAddress  m_collector;
            UI32  m_portNumber;
            UI32  m_collectorAddressType;
            UI32  m_pollingInterval;
            UI32  m_sampleRate;
            UI32  m_opCode;
    };
}
#endif                                            //SFLOWCLIENTCREATESFLOWGLOBALCONFIGSPMESSAGE_H
