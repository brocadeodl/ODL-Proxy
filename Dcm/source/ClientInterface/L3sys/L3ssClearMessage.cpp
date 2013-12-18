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
 **************************************************************************/

#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/L3sys/L3ssClearMessage.h"
#include "ClientInterface/L3sys/L3ssMessageDef.h"
#include "Framework/Attributes/AttributeIpV4AddressNetworkMask.h"

namespace DcmNs
{

    L3ssClearMessage::L3ssClearMessage ()
        : DcmManagementInterfaceMessage (L3SSCLEAR)
    {
    }

    L3ssClearMessage::~L3ssClearMessage ()
    {
    }

    void  L3ssClearMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeIpV4Address(&m_ip_addr, "ip_addr"));
        addSerializableAttribute (new AttributeIpV4AddressNetworkMask(&m_ip_addr_mask, "ip_addr_mask"));
        addSerializableAttribute (new AttributeString(&m_vrf_name, "vrf_name"));
    }


    void  L3ssClearMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  L3ssClearMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  L3ssClearMessage::setIp_addr(const IpV4Address &ip_addr)
    {
        m_ip_addr  =  ip_addr;
    }

    IpV4Address  L3ssClearMessage::getIp_addr() const
    {
        return (m_ip_addr);
    }

    void  L3ssClearMessage::setIp_addr_mask(const IpV4AddressNetworkMask &ip_addr_mask)
    {
        m_ip_addr_mask  =  ip_addr_mask;
    }

    IpV4AddressNetworkMask  L3ssClearMessage::getIp_addr_mask() const
    {
        return (m_ip_addr_mask);
    }

    void L3ssClearMessage::setVrf_name(const string &vrf_name)
    {
        m_vrf_name = vrf_name;
    }

    string L3ssClearMessage::getVrf_name() const
    {
        return (m_vrf_name);
    }

    const void *L3ssClearMessage::getCStructureForInputs ()
    {
        l3ss_clear_in_msg_t  *msg = new l3ss_clear_in_msg_t;
        string str_ipaddr;

        msg->cmd = m_cmdCode;
        strncpy(msg->vrf_name, m_vrf_name.c_str(), 256);
        msg->vrf_name[255] = '\0';


        if(msg->cmd == L3SS_CLEAR_FIB_IP || msg->cmd == L3SS_CLEAR_FIB_ARP_IP) {
                str_ipaddr = m_ip_addr.toString();
                strncpy(msg->ip_address, str_ipaddr.c_str(), 15);
                msg->ip_address[15] = '\0';

        } else if (msg->cmd == L3SS_CLEAR_FIB_IP_MASK) {
                str_ipaddr = m_ip_addr_mask.toString();
                strncpy(msg->ip_address_mask, str_ipaddr.c_str(), 18);
                msg->ip_address_mask[18] = '\0';
        }

        return msg;
    }

  	void L3ssClearMessage::getOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer)
    {
            pBuffer   =   findBuffer(bufferNum, size);
    }
    void
    L3ssClearMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
        l3ss_clear_out_msg_t *msg = ((l3ss_clear_out_msg_t *)pOutputCStructure);

        if (msg == 0) {
            return;
        }

		addBuffer(L3SSCLEAR,
                  sizeof(l3ss_clear_out_msg_t),
                  (unsigned char *)pOutputCStructure,
                  false);

        return;
    }


}


