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
#include "ClientInterface/L3sys/L3ssShowMessage.h"
#include "ClientInterface/L3sys/L3ssMessageDef.h"
#include "Framework/Attributes/AttributeIpV4AddressNetworkMask.h"

namespace DcmNs
{

    L3ssShowMessage::L3ssShowMessage ()
        : DcmManagementInterfaceMessage (L3SSSHOW)
    {
    }

    L3ssShowMessage::~L3ssShowMessage ()
    {
    }

    void  L3ssShowMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
		addSerializableAttribute (new AttributeIpV4Address(&m_ip_addr, "ip_addr"));
		addSerializableAttribute (new AttributeIpV4AddressNetworkMask(&m_ip_addr_mask, "ip_addr_mask"));
        addSerializableAttribute (new AttributeString(&m_vrf_name, "vrf_name"));
    }


    void  L3ssShowMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  L3ssShowMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }
    void  L3ssShowMessage::setIp_addr(const IpV4Address &ip_addr)
    {
        m_ip_addr  =  ip_addr;
    }

    IpV4Address  L3ssShowMessage::getIp_addr() const
    {
        return (m_ip_addr);
    }

    void  L3ssShowMessage::setIp_addr_mask(const IpV4AddressNetworkMask &ip_addr_mask)
    {
        m_ip_addr_mask  =  ip_addr_mask;
    }

	IpV4AddressNetworkMask  L3ssShowMessage::getIp_addr_mask() const
    {
        return (m_ip_addr_mask);
    }

    void L3ssShowMessage::setVrf_name(const string &vrf_name)
    {
        m_vrf_name = vrf_name;
    }

    string L3ssShowMessage::getVrf_name() const
    {
        return m_vrf_name;
    }


    const void *L3ssShowMessage::getCStructureForInputs ()
    {
        l3ss_show_in_msg_t  *msg = new l3ss_show_in_msg_t;
		string str_ipaddr;
        string vrf_name;

		msg->cmd = m_cmdCode;
        strncpy(msg->vrf_name, m_vrf_name.c_str(), 256);
        msg->vrf_name[255] = '\0';

		if((msg->cmd == L3SS_SHOW_FIB_IP) || (msg->cmd == L3SS_SHOW_FIB_NEXTHOP_IP)) {
				str_ipaddr = m_ip_addr.toString();
				strncpy(msg->ip_address, str_ipaddr.c_str(), 15);
				msg->ip_address[15] = '\0';
		} else if ((msg->cmd == L3SS_SHOW_FIB_IP_MASK) || (msg->cmd == L3SS_SHOW_FIB_NEXTHOP_IP_MASK)) {
				str_ipaddr = m_ip_addr_mask.toString();
				strncpy(msg->ip_address_mask, str_ipaddr.c_str(), 18);	
				msg->ip_address_mask[18] = '\0';
		}
        return msg;
    }

    void L3ssShowMessage::getOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer)
    {
            pBuffer   =   findBuffer(bufferNum, size);
    }

	void 
    L3ssShowMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
		l3ss_show_out_msg_t *msg = ((l3ss_show_out_msg_t *)pOutputCStructure);

		if (msg == 0) {
			return;
		}

		int num_records = msg->num_records;
		switch (msg->record_type) {
			case SHOW_IP_FIB_TYPE: {
				addBuffer (L3SSSHOW, (((num_records - 1) > 0 )? (num_records - 1) : 0) * sizeof (route_rec_t) +  sizeof (l3ss_show_out_msg_t), pOutputCStructure, false);
        		break;
			}
            case SHOW_IP_FIB_NH_TYPE: {
                addBuffer (L3SSSHOW, (((num_records - 1) > 0 )? (num_records - 1) : 0) * sizeof (route_rec_t) + sizeof (l3ss_show_out_msg_nh_t), pOutputCStructure, false);
            }
			default :
				break;
		}			

		return;
	}
}
