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

#include "Framework/Utils/TraceUtils.h"
#include "ClientInterface/Eld/EldShowMessage.h"
#include "ClientInterface/Eld/eld_dcm_def.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/Nsm/NsmUtils.h"

namespace DcmNs
{

    EldShowMessage::EldShowMessage ()
        : DcmManagementInterfaceMessage (MSG_ELD_SHOW_INTERFACE_VLAN)
    {
		m_cmdcode = 0;
		m_rbridge = 0;
		m_vlanid = 0;
		m_mappedid = 0;
    }

    EldShowMessage::~EldShowMessage ()
    {
    }


    void  EldShowMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdcode,"cmdcode"));
        addSerializableAttribute (new AttributeUI32(&m_rbridge,"rbridge"));
        addSerializableAttribute (new AttributeUI32(&m_vlanid,"vlan_id"));
        addSerializableAttribute (new AttributeString(&m_interface,"interface"));
    	addSerializableAttribute (new AttributeUI32(&m_mappedid,"mappedid")); 
    }

    void  EldShowMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  EldShowMessage::getIfName() const
    {
        return (m_ifName);
    }
    
    void  EldShowMessage::setInterface(const string &interface)
    {
        m_interface  =  interface;
    }

    string  EldShowMessage::getInterface() const
    {
        return (m_interface);
    }
    
    void  EldShowMessage::setMac(const string &Mac)
    {
        m_Mac  =  Mac;
    }

    string  EldShowMessage::getMac() const
    {
        return (m_Mac);
    }
    

    UI32  EldShowMessage::getVlanid() const
    {
        return (m_vlanid);
    }

	 void  EldShowMessage::setVlanid(const UI32 &vlan_id)
    {
        m_vlanid  =  vlan_id;
    }

    UI32  EldShowMessage::getMappedId() const
    {
        return (m_mappedid);
    }
	void  EldShowMessage::setMappedId(const UI32 &mappedid)
    {
        m_mappedid  =  mappedid;
    }	

    void  EldShowMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdcode  =  cmdCode;
    }

    UI32  EldShowMessage::getCmdCode() const
    {
        return (m_cmdcode);
    }

    void  EldShowMessage::setRbridge(const UI32 &rbridge_id)
    {
        m_rbridge  =  rbridge_id;
    }

    UI32  EldShowMessage::getRbridge() const
    {
        return (m_rbridge);
    }

    void  EldShowMessage::setType(const UI32 &Type)
    {
        m_type  =  Type;
    }

    UI32  EldShowMessage::getType() const
    {
        return (m_type);
    }

    void  EldShowMessage::setBridge(const UI32 &bridgeId)
    {
        m_bridge  =  bridgeId;
    }

    UI32  EldShowMessage::getBridge() const
    {
        return (m_bridge);
    }

    void  EldShowMessage::setIfindex(const UI32 &ifindex)
    {
        m_ifindex  =  ifindex;
    }

    UI32  EldShowMessage::getIfindex() const
    {
        return (m_ifindex);
    }

    void  EldShowMessage::setInterfaceType(const UI32 &interface_type)
    {
        m_interface_type  =  interface_type;
    }

    UI32  EldShowMessage::getInterfaceType() const
    {
        return (m_interface_type);
    }

    void  EldShowMessage::setPortChannelId(const UI32 &port_channel_id)
    {
        m_port_channel_id = port_channel_id;
    }

    UI32  EldShowMessage::getPortChannelId() const
    {
        return (m_port_channel_id);
    }

    void EldShowMessage::getDuplicateInterfaceOutputBuffer (UI32 bufferNum,
													UI32 &size, void *&pBuffer)
    {
            pBuffer   =   findBuffer(bufferNum, size);
    }
    
    const void *EldShowMessage::getCStructureForInputs ()
    {
		eldshow_interface_inputmsg_t *eld_show = (eldshow_interface_inputmsg_t *)malloc (sizeof (eldshow_interface_inputmsg_t));

		eld_show->cmdcode     = m_cmdcode;
		eld_show->rbridge_id     = m_rbridge;
        strncpy (eld_show->if_name, m_interface.c_str(), 64);
        eld_show->vlan_id      = m_vlanid;

        return (eld_show);
    }

	void EldShowMessage::loadOutputsFromCStructure (const void 
														*pOutputCStructure)
	{
		eld_show_msg_t *msg = ((eld_show_msg_t *)pOutputCStructure);

        if(msg == NULL)
			return;
	eld_mgmt_buffer_num_t  buff_num = {{0}};
	buff_num.buff.mapped_id = m_mappedid;
	buff_num.buff.tag = MSG_ELD_SHOW_INTERFACE_VLAN;
	addBuffer (buff_num.num, msg->size, pOutputCStructure, false);
	}


}
