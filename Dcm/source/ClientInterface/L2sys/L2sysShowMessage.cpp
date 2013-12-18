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

#include "Framework/Utils/TraceUtils.h"
#include "ClientInterface/L2sys/L2sysShowMessage.h"
#include "ClientInterface/L2sys/L2sysMessageDef.h"
#include "L2sys/Local/L2sysTypes.h"

namespace DcmNs
{

    L2sysShowMessage::L2sysShowMessage ()
        : DcmManagementInterfaceMessage (L2SYS_SHOW_MAC),
        m_vlanid    (0),
        m_type   (0),
        m_cmdCode   (0),
        m_bridge   (0),
		m_ifindex (0),
		m_interface_type (0),
		m_port_channel_id (0),
		m_poType (0),
		m_mappedid(0),
		m_pageIdx(0),
		m_lastRbridge_id(0),
		m_lastIf_name("0"),
		m_lastMac("0"),
		m_lastType("0"),
		m_lastState("0"),
		m_lastVlan_id(0),
		m_lastAppmFlag(0),
		m_lastIs_last(0),
		m_lastIsprofiled("0"),
		m_isFirstRequest(0)
    {
    }

    L2sysShowMessage::L2sysShowMessage (const string &interface, const string &ifName,
			const string &Mac, const UI32 &vlan_id, const UI32 &type,
			const UI32 &cmdCode,  const UI32 &bridge, const UI32 &ifindex,
			const UI32 &interface_type, const UI32 &port_channel_id,const UI32 &mappedid)
        : DcmManagementInterfaceMessage (L2SYS_SHOW_MAC),
        m_ifName    (ifName),
        m_interface    (interface),
        m_Mac    	(Mac),
        m_vlanid    (vlan_id),
        m_type   (type),
        m_cmdCode   (cmdCode),
        m_bridge   (bridge),
		m_ifindex (ifindex),
		m_interface_type (interface_type),
		m_port_channel_id (port_channel_id),
		m_mappedid(mappedid)
    {
		m_pageIdx = 0;
		m_lastRbridge_id = 0;
		m_lastIf_name = "0";
		m_lastMac = "0";
		m_lastType = "0";
		m_lastState = "0";
		m_lastVlan_id = 0;
		m_lastAppmFlag = 0;
		m_lastIs_last = 0;
		m_lastIsprofiled = "0";
		m_isFirstRequest = 0;
		m_poType = 0;
    }

    L2sysShowMessage::~L2sysShowMessage ()
    {
    }

    void  L2sysShowMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeUI32(&m_bridge,"bridge"));
        addSerializableAttribute (new AttributeUI32(&m_vlanid,"vlan_id"));
        addSerializableAttribute (new AttributeUI32(&m_type,"type"));
        addSerializableAttribute (new AttributeUI32(&m_ifindex,"ifindex"));
        addSerializableAttribute (new AttributeString(&m_Mac,"Mac"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeString(&m_interface,"interface"));
        addSerializableAttribute (new AttributeUI32(&m_interface_type,"interface_type"));
        addSerializableAttribute (new AttributeUI32(&m_port_channel_id,"port_channel_id"));
        addSerializableAttribute (new AttributeUI32(&m_poType,"poType"));
    	addSerializableAttribute (new AttributeUI32(&m_mappedid,"mappedid"));
		addSerializableAttribute (new AttributeSI32(&m_pageIdx,"pageIdx"));

        // add remaining seializable attributes
        addSerializableAttribute (new AttributeUI32(&m_lastRbridge_id, "lastRbridge_id"));
        addSerializableAttribute (new AttributeString(&m_lastIf_name,"lastIf_name"));
        addSerializableAttribute (new AttributeString(&m_lastMac,"lastMac"));
        addSerializableAttribute (new AttributeString(&m_lastType,"lastType"));
        addSerializableAttribute (new AttributeString(&m_lastState,"lastState"));
        addSerializableAttribute (new AttributeUI32(&m_lastVlan_id, "lastVlan_id"));
        addSerializableAttribute (new AttributeUI32(&m_lastAppmFlag, "lastAppmFlag"));
        addSerializableAttribute (new AttributeUI32(&m_lastIs_last, "lastIs_last"));
        addSerializableAttribute (new AttributeString(&m_lastIsprofiled, "lastIsprofiled"));
        addSerializableAttribute (new AttributeUI32(&m_isFirstRequest, "isFirstRequest"));
    }


    void  L2sysShowMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  L2sysShowMessage::getIfName() const
    {
        return (m_ifName);
    }
    
    void  L2sysShowMessage::setInterface(const string &interface)
    {
        m_interface  =  interface;
    }

    string  L2sysShowMessage::getInterface() const
    {
        return (m_interface);
    }
    
    void  L2sysShowMessage::setMac(const string &Mac)
    {
        m_Mac  =  Mac;
    }

    string  L2sysShowMessage::getMac() const
    {
        return (m_Mac);
    }
    

    UI32  L2sysShowMessage::getVlanid() const
    {
        return (m_vlanid);
    }

	 void  L2sysShowMessage::setVlanid(const UI32 &vlan_id)
    {
        m_vlanid  =  vlan_id;
    }

    UI32  L2sysShowMessage::getMappedId() const
    {
        return (m_mappedid);
    }
	void  L2sysShowMessage::setMappedId(const UI32 &mappedid)
    {
        m_mappedid  =  mappedid;
    }	
    void  L2sysShowMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  L2sysShowMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  L2sysShowMessage::setType(const UI32 &Type)
    {
        m_type  =  Type;
    }

    UI32  L2sysShowMessage::getType() const
    {
        return (m_type);
    }

    void  L2sysShowMessage::setBridge(const UI32 &bridgeId)
    {
        m_bridge  =  bridgeId;
    }

    UI32  L2sysShowMessage::getBridge() const
    {
        return (m_bridge);
    }

    void  L2sysShowMessage::setIfindex(const UI32 &ifindex)
    {
        m_ifindex  =  ifindex;
    }

    UI32  L2sysShowMessage::getIfindex() const
    {
        return (m_ifindex);
    }

    void  L2sysShowMessage::setInterfaceType(const UI32 &interface_type)
    {
        m_interface_type  =  interface_type;
    }

    UI32  L2sysShowMessage::getInterfaceType() const
    {
        return (m_interface_type);
    }

    void  L2sysShowMessage::setPortChannelId(const UI32 &port_channel_id)
    {
        m_port_channel_id = port_channel_id;
    }

    UI32  L2sysShowMessage::getPortChannelId() const
    {
        return (m_port_channel_id);
    }

    void  L2sysShowMessage::setPortChannelType(const UI32 &poType)
    {
        m_poType = poType;
    }

    UI32  L2sysShowMessage::getPortChannelType() const
    {
        return (m_poType);
    }

    void  L2sysShowMessage::setPageIdx(const SI32 &pageIdx)
    {
        m_pageIdx = pageIdx;
    }

    SI32  L2sysShowMessage::getPageIdx() const
    {
        return (m_pageIdx);
    }

    void  L2sysShowMessage::setIsFirstRequest(const UI32 &isFirstRequest)
    {
        m_isFirstRequest = isFirstRequest;
    }

    UI32  L2sysShowMessage::getIsFirstRequest() const
    {
        return (m_isFirstRequest);
    }

    void L2sysShowMessage::setLastMacRecord(const l2sys_show_mac_output_msg_t *lastMacRecord)
    {
        m_lastRbridge_id = lastMacRecord->rbridge_id;
        m_lastIf_name = lastMacRecord->if_name;
        m_lastMac = lastMacRecord->Mac;
        m_lastType = lastMacRecord->type;
        m_lastState = lastMacRecord->state;
        m_lastVlan_id = lastMacRecord->vlan_id;
        m_lastAppmFlag = lastMacRecord->appmFlag;
        m_lastIs_last = lastMacRecord->is_last;
        m_lastIsprofiled = lastMacRecord->isprofiled;
    }

    void L2sysShowMessage::getDuplicateInterfaceOutputBuffer (UI32 bufferNum,
													UI32 &size, void *&pBuffer)
    {
            pBuffer   =   findBuffer(bufferNum, size);
    }
    
    const void *L2sysShowMessage::getCStructureForInputs ()
    {
        L2sysShow_InputMsg_t *pInput = new L2sysShow_InputMsg_t;

		memset(pInput, 0, sizeof(L2sysShow_InputMsg_t));
        strncpy (pInput->if_name, m_ifName.c_str (), 64);
        strncpy (pInput->interface, m_interface.c_str (), 64);
        strncpy (pInput->Mac, m_Mac.c_str (), 16);
        pInput->vlan_id      = m_vlanid;
        pInput->cmdCode     = m_cmdCode;
        pInput->bridge     = m_bridge;
        pInput->type     = m_type;
		pInput->ifindex = m_ifindex;
		pInput->interface_type = m_interface_type;
		pInput->port_channel_id = m_port_channel_id;
		pInput->port_channel_type = m_poType;
		pInput->page_idx = m_pageIdx;
        //copy the information of the last Mac Record
        pInput->is_first_request = m_isFirstRequest;
        pInput->last_record.rbridge_id = m_lastRbridge_id;
        strncpy(pInput->last_record.if_name, m_lastIf_name.c_str(), 256);
        strncpy(pInput->last_record.Mac, m_lastMac.c_str(), 16);
        strncpy(pInput->last_record.type, m_lastType.c_str(), 64);
        strncpy(pInput->last_record.state, m_lastState.c_str(), 64);
        pInput->last_record.vlan_id = m_lastVlan_id;
        pInput->last_record.appmFlag = m_lastAppmFlag;
        pInput->last_record.is_last = m_lastIs_last;
        strncpy(pInput->last_record.isprofiled, m_lastIsprofiled.c_str(), 20);
        return (pInput);
    }

	void L2sysShowMessage::loadOutputsFromCStructure (const void 
														*pOutputCStructure)
	{
    	l2sys_show_msg_t *msg = ((l2sys_show_msg_t *)pOutputCStructure);
        if(msg == NULL)
			return;
		l2sys_buffer_num_t  buff_num;
		memset(&buff_num, 0, sizeof(buff_num));	
		int num_records = msg->num_records;
		switch (msg->record_type) {
		  case L2SYSSHOWMAC: 
        		addBuffer (L2SYS_SHOW_MAC, (sizeof(l2sys_show_msg_t)+ 
                            ((num_records - 1) * sizeof (l2sys_show_mac_output_msg_t))), 
                           pOutputCStructure, false);
				break;
		  case L2SYSSHOWRBRIDGEMACTABLE: 
				buff_num.buff.mapped_id = this->getMappedId();
				buff_num.buff.tag = L2SYSSHOWRBRIDGEMAC;
				addBuffer (buff_num.num,
					(sizeof(l2sys_show_msg_t)+ ((num_records - 1) * sizeof (l2sys_show_mac_output_msg_t))),
					pOutputCStructure, false);
				break;
		  case L2SYSSHOWAGINGTIME: 
        		addBuffer (L2SYS_SHOW_AGING_TIME, sizeof(l2sys_show_msg_t), pOutputCStructure, false);
				break;
		  case L2SYSSHOWCOUNT: 
        		addBuffer (L2SYS_SHOW_COUNT, sizeof(l2sys_show_msg_t), pOutputCStructure, false);
				break;
 		  case L2SYSSHOWLINECARD: 
				addBuffer (L2SYS_SHOW_LINECARD, (sizeof(l2sys_show_msg_t)+
						((num_records - 1) * sizeof (l2sys_global_linecard_config_msg_t))),
					pOutputCStructure, false);
				break;
		  case L2SYSSHOWMACPORTPROFILE:
				buff_num.buff.mapped_id = this->getMappedId();
				buff_num.buff.tag = L2SYSSHOWMACPROFILE;
				addBuffer(buff_num.num, (sizeof(l2sys_show_msg_t) +
						((num_records - 1) * sizeof (l2sys_show_mac_output_msg_t))), pOutputCStructure, false); 
				break; 
		  default :
			break;
		}
	}
}
