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
 *   Copyright (C) 2010 Brocade Communications Systems, Inc.          	   *
 *   All rights reserved.                                                  *
 *   Author : jusitnh                                                      *
 **************************************************************************/

#ifndef NsmGlobalPoIpMessage_H
#define NsmGlobalPoIpMessage_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

#include "Nsm/Global/NsmGlobalTypes.h"
#include "ClientInterface/Nsm/NsmUtils.h"

using namespace WaveNs;

namespace DcmNs
{
    class NsmGlobalPoIpMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            NsmGlobalPoIpMessage ();
            virtual  ~NsmGlobalPoIpMessage ();

            void setOpCode(const UI32 &op);
            UI32 getOpCode() const;
            void setCmdCode(const UI32 &cmdCode);
            UI32 getCmdCode()  const;

            void setRbridge(const string &name);
            string getRbridge() const;
            void setIfType(const UI32 &type);
            UI32 getIfType() const;
            void setIfId(const UI32 &if_id);
            UI32 getIfId() const;
            void setIfName(const string &if_name);
            string getIfName() const;

            void setIpv4Address(const string &ip_address);
            string getIpv4Address() const;
            void setSecond(const bool &sec);
            bool getSecond() const;
            void setSwitchport(const bool &swp);
            bool getSwitchport() const;

            void setMtu(const UI32 &mtu);
            UI32 getMtu() const;
            void setDirectedBroadcast(const bool &flag);
            bool getDirectedBroadcast() const;
            void setProxyArp(const bool &flag);
            bool getProxyArp() const;
            void setArpAgingTimeout(const UI32 &timeout);
            UI32 getArpAgingTimeout() const;

        private:
        protected:
        public:
            UI32 m_cmdCode;
            UI32 m_opCode;
              
            string m_rbridge;
            UI32 m_ifType;
            UI32  m_ifId;
            string  m_ifName;

            string m_ipv4Addr;
            bool m_secondary;
            bool m_switchport;


            UI32  m_mtu;
            bool  m_directed_broadcast;
            bool  m_proxy_arp;
            UI32  m_arp_aging_timeout;

    };
}
#endif  //NsmGlobalPoIpMessage_H
