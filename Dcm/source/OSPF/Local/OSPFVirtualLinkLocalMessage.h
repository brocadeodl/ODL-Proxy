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
 **************************************************************************/

#ifndef OSPFVIRTUALLINKLOCALMESSAGE_H
#define OSPFVIRTUALLINKLOCALMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "Framework/Types/IpV4Address.h"
#include "ClientInterface/OSPF/OSPFMessageDef.h"

using namespace WaveNs;

namespace DcmNs
{
    //class OSPFVirtualLinkLocalMessage : public ManagementInterfaceMessage
    class OSPFVirtualLinkLocalMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            OSPFVirtualLinkLocalMessage ();
            virtual    ~OSPFVirtualLinkLocalMessage ();

            void setAreaId(const string &id);
            string getAreaId()  const;
            void setOpCode(const unsigned int &opCode);
            unsigned int getOpCode()  const;
            void setCmdCode(const unsigned int &cmdCode);
            unsigned int getCmdCode()  const;
            void setVirtualLinkRouterId(const IpV4Address &virtualLinkRouterId);
            IpV4Address getVirtualLinkRouterId()  const;
            void setHelloInterval(const UI32 &HelloInterval);
            UI32 getHelloInterval()  const;
            void setRouterDeadInterval(const UI32 &RouterDeadInterval);
            UI32 getRouterDeadInterval()  const;
            void setRetransmitInterval(const UI32 &RetransmitInterval);
            UI32 getRetransmitInterval()  const;
            void setTransDelay(const UI32 &TransDelay);
            UI32 getTransDelay()  const;
            void setAuthType(const OSPFEncrypType &AuthType);
            OSPFEncrypType getAuthType()  const;
            void setEncryptionType(const OSPFEncrypType &EncryptionType);
            OSPFEncrypType getEncryptionType()  const;
            void setAuthKey(const string &AuthKey);
            string getAuthKey()  const;
            void setMD5KeyActWaitTime(const UI32 &MD5KeyActWaitTime);
            UI32 getMD5KeyActWaitTime()  const;
            void setMD5KeyId(const UI8 &MD5KeyId);
            UI32 getMD5KeyId()  const;
            void setMD5Key(const string &MD5Key);
            string getMD5Key()  const;

			void loadOutputsFromCStructure (const void *pOutputCStructure);

// Now the data members

        private:
            IpV4Address  m_virtualLinkRouterId;
            string m_areaId;
            UI32 m_opCode;
            UI32 m_cmdCode;
            UI32  m_HelloInterval;
            UI32  m_RouterDeadInterval;
            UI32  m_RetransmitInterval;
            UI32  m_TransDelay;
            OSPFEncrypType  m_AuthType;
            OSPFEncrypType  m_EncryptionType;
            string  m_AuthKey;
            UI32  m_MD5KeyActWaitTime;
            UI32  m_MD5KeyId;
            string  m_MD5Key;
        protected:
        public:
    };
}
#endif // OSPFVIRTUALLINKLOCALMESSAGE_H

