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
 *   Author : pahuja                                                     *
 **************************************************************************/

#ifndef SNMPSERVERINTSNMPCONFIGCHANGEHOSTINTMESSAGE_H
#define SNMPSERVERINTSNMPCONFIGCHANGEHOSTINTMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"
#include "SnmpServer/Global/enumdatatype.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "Framework/Types/HostUC.h"


using namespace WaveNs;

namespace DcmNs
{

    class SnmpServerIntSnmpConfigChangeHostIntMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            SnmpServerIntSnmpConfigChangeHostIntMessage ();
            SnmpServerIntSnmpConfigChangeHostIntMessage (const HostUC &ip,const Version &version,const UI32 &udpPort,const string &community,const SevLevel &sevLevel);
            virtual    ~SnmpServerIntSnmpConfigChangeHostIntMessage ();
			void setHostUC(const HostUC &ip);
            HostUC getHostUC()  const;
            void setVersion(const Version &version);
            Version getVersion()  const;
            void setUdpPort(const UI32 &udpPort);
            UI32 getUdpPort()  const;
            void setCommunity(const string &community);
            string getCommunity()  const;
            void setSevLevel(const SevLevel &sevLevel);
            SevLevel getSevLevel()  const;

// Now the data members

        private:
        protected:
        public:
            HostUC m_ip;
            Version  m_version;
            UI32  m_udpPort;
            string  m_community;
            SevLevel m_sevLevel;
    };
}
#endif                                            //SNMPSERVERINTSNMPCONFIGCHANGEHOSTINTMESSAGE_H
