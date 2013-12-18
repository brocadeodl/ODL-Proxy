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
 *   Author : Adarsh
 **************************************************************************/

#include "ClientInterface/L2sys/L2sysClearMessage.h"
#include "ClientInterface/L2sys/L2sysMessageDef.h"

namespace DcmNs
{

    L2sysClearMessage::L2sysClearMessage ()
        : DcmManagementInterfaceMessage (L2SYS_CLEAR_MAC),
        m_vlanid    (0),
        m_type   (0),
        m_cmdCode    (0),
        m_bridge    (0),
        m_ifindex    (0),
        m_interface_type    (0),
        m_port_channel_id (0)
    {
    }

    L2sysClearMessage::L2sysClearMessage (const string &ifName, const string &Mac, const UI32 &vlan_id, const UI32 &type, const UI32 &cmdCode,  const UI32 &bridge,
            const UI32 &ifindex, const UI32 &interface_type, const UI32 &port_channel_id)
        : DcmManagementInterfaceMessage (L2SYS_CLEAR_MAC),
        m_ifName    (ifName),
        m_Mac    	(Mac),
        m_vlanid    (vlan_id),
        m_type   (type),
        m_cmdCode   (cmdCode),
        m_bridge   (bridge),
        m_ifindex  (ifindex),
        m_interface_type (interface_type),
        m_port_channel_id (port_channel_id)

    {
    }

    L2sysClearMessage::~L2sysClearMessage ()
    {
    }

    void  L2sysClearMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeUI32(&m_bridge,"bridge"));
        addSerializableAttribute (new AttributeUI32(&m_vlanid,"vlan_id"));
        addSerializableAttribute (new AttributeUI32(&m_type,"type"));
        addSerializableAttribute (new AttributeUI32(&m_ifindex,"ifindex"));
        addSerializableAttribute (new AttributeString(&m_Mac,"Mac"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeUI32(&m_interface_type,"interfaceType"));
        addSerializableAttribute (new AttributeUI32(&m_port_channel_id,"portChannelId"));
    }

    void  L2sysClearMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  L2sysClearMessage::getIfName() const
    {
        return (m_ifName);
    }
    
    void  L2sysClearMessage::setMac(const string &Mac)
    {
        m_Mac  =  Mac;
    }

    string  L2sysClearMessage::getMac() const
    {
        return (m_Mac);
    }
    
	void  L2sysClearMessage::setVlanid(const UI32 &vlan_id)
    {
        m_vlanid  =  vlan_id;
    }

    UI32  L2sysClearMessage::getVlanid() const
    {
        return (m_vlanid);
    }

    void  L2sysClearMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  L2sysClearMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  L2sysClearMessage::setType(const UI32 &Type)
    {
        m_type  =  Type;
    }

    UI32  L2sysClearMessage::getType() const
    {
        return (m_type);
    }

    void  L2sysClearMessage::setBridge(const UI32 &bridgeId)
    {
        m_bridge  =  bridgeId;
    }

    UI32  L2sysClearMessage::getBridge() const
    {
        return (m_bridge);
    }

    void  L2sysClearMessage::setIfindex(const UI32 &ifindex)
    {
        m_ifindex  =  ifindex;
    }

    UI32  L2sysClearMessage::getIfindex() const
    {
        return (m_ifindex);
    }

   void  L2sysClearMessage::setInterfaceType(const UI32 &interface_type)
    {
        m_interface_type  =  interface_type;
    }

    UI32  L2sysClearMessage::getInterfaceType() const
    {
        return (m_interface_type);
    }

    void  L2sysClearMessage::setPortChannelId(const UI32 &port_channel_id)
    {
        m_port_channel_id = port_channel_id;
    }

    UI32  L2sysClearMessage::getPortChannelId() const
    {
        return (m_port_channel_id);
    }


    void L2sysClearMessage::getDuplicateInterfaceOutputBuffer (UI32 bufferNum,
													UI32 &size, void *&pBuffer)
    {
            pBuffer   =   findBuffer(bufferNum, size);
    }
    
    const void *L2sysClearMessage::getCStructureForInputs ()
    {
        L2sysClear_InputMsg_t *pInput = new L2sysClear_InputMsg_t;

        bzero(pInput,sizeof(L2sysClear_InputMsg_t));
        strncpy (pInput->if_name, m_ifName.c_str (), 64);
        strncpy (pInput->Mac, m_Mac.c_str (), 16);
        pInput->vlan_id      = m_vlanid;
        pInput->cmdCode     = m_cmdCode;
        pInput->bridge     = m_bridge;
        pInput->type     = m_type;
        pInput->ifindex = m_ifindex;
        pInput->interface_type = m_interface_type;
        pInput->port_channel_id = m_port_channel_id;

        return (pInput);
    }

	void L2sysClearMessage::loadOutputsFromCStructure (const void 
														*pOutputCStructure)
	{
        l2sys_show_msg_t *msg = ((l2sys_show_msg_t *)pOutputCStructure);

		if (msg == 0) {
		  return;
		}
        return;
	}
}
