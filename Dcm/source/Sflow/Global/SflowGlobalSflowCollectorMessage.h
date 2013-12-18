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

/*
 *  SflowGlobalSflowCollectorMessage.h

 *  Copyright (C) 2005-2012 Brocade Communications Systems, Inc.
 *  All rights reserved. 

 *  Created on: Aug 14, 2012
 *  Author: sunniram
 */

#ifndef SFLOWGLOBALSFLOWCOLLECTORMESSAGE_H_
#define SFLOWGLOBALSFLOWCOLLECTORMESSAGE_H_

#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "Framework/Types/IpVxAddress.h"

using namespace WaveNs;

namespace DcmNs
{

    class SflowGlobalSflowCollectorMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();

        public:
            SflowGlobalSflowCollectorMessage ();
            SflowGlobalSflowCollectorMessage (const IpVxAddress &collector,const UI32 &portNumber,const UI32 &opCode);
            virtual    ~SflowGlobalSflowCollectorMessage ();

            void setCollectorIP(const IpVxAddress &collector);
            IpVxAddress getCollectorIP()  const;

            void setCollectorPort(const UI32 &portNumber);
            UI32 getCollectorPort()  const;

            void setCollectorAddressType(const UI32 &type);
            UI32 getCollectorAddressType()  const;

            void setOpCode(const UI32 &opCode);
            UI32 getOpCode()  const;


        private:
        protected:
        public:
            IpVxAddress  m_collector;
            UI32  m_portNumber;
            UI32  m_collectorAddressType;
            UI32  m_opCode;
    };
}

#endif /* SFLOWGLOBALSFLOWCOLLECTORMESSAGE_H_ */
