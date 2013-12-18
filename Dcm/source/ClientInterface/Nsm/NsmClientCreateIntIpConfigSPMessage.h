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
 *   Author : payyappa                                                     *
 **************************************************************************/

#ifndef NSMCLIENTCREATEINTIPCONFIGSPMESSAGE_H
#define NSMCLIENTCREATEINTIPCONFIGSPMESSAGE_H

#include "ClientInterface/Nsm/NsmVlanProvisionMessage.h"
#include "Framework/Types/IpV4AddressNetworkMask.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmClientCreateIntIpConfigSPMessage : public NsmVlanProvisionMessage 
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            NsmClientCreateIntIpConfigSPMessage ();
            NsmClientCreateIntIpConfigSPMessage (const string &ifName,const UI32 &mtu,const string &ip_addr_mask,const UI32 &opcode, const UI32 &intf_type, const bool &sec, const UI32 &m_arp_ageing_timeout, const bool &proxy_arp);
            virtual    ~NsmClientCreateIntIpConfigSPMessage ();
            void setIfName(const string &ifName);
            string getIfName()  const;
            void setIfId(const UI32 &id);
            UI32 getIfId()  const;
            void setMtu(const UI32 &mtu);
            UI32 getMtu()  const;
            void setDirectedBroadcast(const bool &flag);
            bool getDirectedBroadcast() const;
            void setIp_addr_mask(const string &ip_addr_mask);
            string getIp_addr_mask()  const;
            void setOpcode(const UI32 &opcode);
            UI32 getOpcode() const;
			void setIntfType(const UI32 &intf_type);
			UI32  getIntfType() const;
			const void* getCStructureForInputs();
			void setSec(const bool &sec);
			bool getSec() const;
			void setOspfPassive(const bool &ospfpassive);
			bool getOspfPassive() const;
			void setOspfIgnore(const bool &ospfignore);
			bool getOspfIgnore() const;
			UI32  getArp_ageing_timeout() const;
			void  setArp_ageing_timeout(const UI32 &arp_ageing_timeout);
			bool  getProxy_arp() const;
			void  setProxy_arp(const bool &proxy_arp);
            void setStatus(const bool &flag);
            bool getStatus() const;

// Now the data members

        private:
        protected:
        public:
            string m_ifName;
            UI32 m_ifId;
            UI32  m_mtu;
            bool  m_directed_broadcast;
            string  m_ip_addr_mask;
			UI32	m_opcode;
			UI32	m_intf_type;
			bool	m_sec;
			bool    m_ospfPassive;
			bool 	m_ospfIgnore;
			UI32    m_arp_ageing_timeout;
			bool	m_proxy_arp;
            bool    m_switchport;
            bool    m_status;
    };
}
#endif                                            //NSMCREATEINTIPCONFIGSPMESSAGE_H
