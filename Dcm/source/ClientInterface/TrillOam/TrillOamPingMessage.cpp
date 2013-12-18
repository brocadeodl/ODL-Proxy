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
 *   Author : Manju Mahajan                                                *
 **************************************************************************/

#include "ClientInterface/TrillOam/TrillOamPingMessage.h"
#include "TrillOam/Common/TrillOamPluginTypes.h"
#include "Framework/Utils/TraceUtils.h"


namespace DcmNs
{

    TrillOamPingMessage::TrillOamPingMessage ()
        : DcmManagementInterfaceMessage (TRILLOAMPING)
    {
    }

    TrillOamPingMessage::TrillOamPingMessage (const MacAddress &srcMac,const MacAddress &dstMac, const UI32 &vlan,
			const UI32 &cmdCode) : DcmManagementInterfaceMessage (TRILLOAMPING),
        m_srcMac    (srcMac),
        m_dstMac    (dstMac),
        m_vlan    (vlan),
        m_cmdCode    (cmdCode)
    {
    }

    TrillOamPingMessage::~TrillOamPingMessage ()
    {
    }

    void  TrillOamPingMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeMacAddress(&m_srcMac,"srcMac"));
        addSerializableAttribute (new AttributeMacAddress(&m_dstMac,"dstMac"));
        addSerializableAttribute (new AttributeUI32(&m_vlan,"vlan"));
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
    }


    void  TrillOamPingMessage::setSrcMac(const MacAddress &srcMac)
    {
        m_srcMac  =  srcMac;
    }

    MacAddress  TrillOamPingMessage::getSrcMac() const
    {
        return (m_srcMac);
    }

    void  TrillOamPingMessage::setDstMac(const MacAddress &dstMac)
    {
        m_dstMac  =  dstMac;
    }

    MacAddress  TrillOamPingMessage::getDstMac() const
    {
        return (m_dstMac);
    }


    void  TrillOamPingMessage::setVlan(const UI32 &vlan)
    {
        m_vlan  =  vlan;
    }

    UI32  TrillOamPingMessage::getVlan() const
    {
        return (m_vlan);
    }

	
    void  TrillOamPingMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  TrillOamPingMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

	const void * TrillOamPingMessage::getCStructureForInputs()
	{
		trilloam_msg_ping   *msg_p = new trilloam_msg_ping;
                memset(msg_p->src_mac, '\0', TRILLOAM_MAX_MAC_STR + 1);
                memset(msg_p->dst_mac, '\0', TRILLOAM_MAX_MAC_STR + 1);
		string mac = m_srcMac.toString();
		trace (TRACE_LEVEL_INFO, string ("MAC_ADDRESS : ") + mac);
		strncpy((char *)msg_p->src_mac, mac.c_str(), TRILLOAM_MAX_MAC_STR);
		mac = m_dstMac.toString();
		strncpy((char *)msg_p->dst_mac, mac.c_str(), TRILLOAM_MAX_MAC_STR);
		msg_p->vlanid = m_vlan;
		return msg_p;
	}

}
