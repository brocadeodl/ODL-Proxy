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
 *   Author : Mobin Mohan                                                  *
 **************************************************************************/

#include "Framework/Core/Wave.h"
#include "Framework/Messaging/Local/PrismMessage.h"

#include "ClientInterface/Lldp/NsmLldpClientIntfConfSetMessage.h"
#include "ClientInterface/Lldp/lldp_dcm_def.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/InterfaceCommon.h"
#include "Framework/Attributes/AttributeEnum.h"
namespace DcmNs
{

    NsmLldpClientIntfConfSetMessage::NsmLldpClientIntfConfSetMessage ()
        : DcmManagementInterfaceMessage (MSG_LLDP_PHY_INTF_CONF)
    {
		m_lldpCeeType = LLDP_CEE_OFF;
    }

    NsmLldpClientIntfConfSetMessage::NsmLldpClientIntfConfSetMessage (const UI32 &mgmtFlags,const UI32 &cmdcode,const UI32 &msgType,const string &id,const string &ifName,const UI64 &ifIndex,const UI8 &dcbxVersion,const bool &disable,const string &profile,const UI32 &iscsiPriority)
        : DcmManagementInterfaceMessage (MSG_LLDP_PHY_INTF_CONF),
        m_mgmtFlags    (mgmtFlags),
        m_cmdcode    (cmdcode),
        m_msgType    (msgType),
        m_id    (id),
        m_ifName    (ifName),
        m_ifIndex    (ifIndex),
        m_dcbxVersion    (dcbxVersion),
        m_disable    (disable),
        m_profile    (profile),
        m_iscsiPriority    (iscsiPriority)
    {
        m_ifType = IF_TYPE_INVALID;
		m_lldpCeeType = LLDP_CEE_OFF;
    }

    NsmLldpClientIntfConfSetMessage::~NsmLldpClientIntfConfSetMessage ()
    {
    }

    void  NsmLldpClientIntfConfSetMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_mgmtFlags,"mgmtFlags"));
        addSerializableAttribute (new AttributeUI32(&m_cmdcode,"cmdcode"));
        addSerializableAttribute (new AttributeUI32(&m_msgType,"msgType"));
        addSerializableAttribute (new AttributeString(&m_id,"id"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeUI64(&m_ifIndex,"ifIndex"));
        addSerializableAttribute (new AttributeUI8(&m_dcbxVersion,"dcbxVersion"));
        addSerializableAttribute (new AttributeBool(&m_disable,"disable"));
        addSerializableAttribute (new AttributeString(&m_profile,"profile"));
        addSerializableAttribute (new AttributeUI32(&m_iscsiPriority,"iscsiPriority"));
        addSerializableAttribute (new AttributeString(&m_description,"description"));
        addSerializableAttribute (new AttributeUI32(&m_ifType, "ifType"));
		addSerializableAttribute (new AttributeEnum((UI32*)(&m_lldpCeeType),"lldpCeeType"));
    }
    
    void NsmLldpClientIntfConfSetMessage::setLldpCeeStatus(const lldp_cee_on_off_t &lldpCeeType)
    {
        m_lldpCeeType = lldpCeeType;
    }

    lldp_cee_on_off_t NsmLldpClientIntfConfSetMessage::getLldpCeeStatus() const
    {
        return m_lldpCeeType;
    }

    void NsmLldpClientIntfConfSetMessage::setIfType(const UI32 &ifType)
    {
        m_ifType =  ifType;
    }

    UI32 NsmLldpClientIntfConfSetMessage::getIfType() const
    {
        return (m_ifType);
    }
    void  NsmLldpClientIntfConfSetMessage::setMgmtFlags(const UI32 &mgmtFlags)
    {
        m_mgmtFlags  =  mgmtFlags;
    }

    UI32  NsmLldpClientIntfConfSetMessage::getMgmtFlags() const
    {
        return (m_mgmtFlags);
    }

    void  NsmLldpClientIntfConfSetMessage::setCmdcode(const UI32 &cmdcode)
    {
        m_cmdcode  =  cmdcode;
    }

    UI32  NsmLldpClientIntfConfSetMessage::getCmdcode() const
    {
        return (m_cmdcode);
    }

    void  NsmLldpClientIntfConfSetMessage::setMsgType(const UI32 &msgType)
    {
        m_msgType  =  msgType;
    }

    UI32  NsmLldpClientIntfConfSetMessage::getMsgType() const
    {
        return (m_msgType);
    }

    void  NsmLldpClientIntfConfSetMessage::setId(const string &id)
    {
        m_id  =  id;
    }

    string  NsmLldpClientIntfConfSetMessage::getId() const
    {
        return (m_id);
    }

    void  NsmLldpClientIntfConfSetMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  NsmLldpClientIntfConfSetMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  NsmLldpClientIntfConfSetMessage::setIfIndex(const UI64 &ifIndex)
    {
        m_ifIndex  =  ifIndex;
    }

    UI64  NsmLldpClientIntfConfSetMessage::getIfIndex() const
    {
        return (m_ifIndex);
    }

    void  NsmLldpClientIntfConfSetMessage::setDcbxVersion(const UI8 &dcbxVersion)
    {
        m_dcbxVersion  =  dcbxVersion;
    }

    UI8  NsmLldpClientIntfConfSetMessage::getDcbxVersion() const
    {
        return (m_dcbxVersion);
    }

    void  NsmLldpClientIntfConfSetMessage::setDisable(const bool &disable)
    {
        m_disable  =  disable;
    }

    bool  NsmLldpClientIntfConfSetMessage::getDisable() const
    {
        return (m_disable);
    }

    void  NsmLldpClientIntfConfSetMessage::setProfile(const string &profile)
    {
        m_profile  =  profile;
    }

    string  NsmLldpClientIntfConfSetMessage::getProfile() const
    {
        return (m_profile);
    }
    
    void  NsmLldpClientIntfConfSetMessage::setIscsiPriority(const UI32 &iscsiPriority)
    {
        m_iscsiPriority  =  iscsiPriority;
    }

    UI32  NsmLldpClientIntfConfSetMessage::getIscsiPriority() const
    {
        return (m_iscsiPriority);
    }
    
	void  NsmLldpClientIntfConfSetMessage::setDescription(const string &description)
    {
        m_description  =  description;
    }

    string  NsmLldpClientIntfConfSetMessage::getDescription() const
    {
        return (m_description);
    }

	const void *NsmLldpClientIntfConfSetMessage::getCStructureForInputs ()
    {
        lldp_mgmt_conf_req_t *req_p = (lldp_mgmt_conf_req_t*)calloc(1, sizeof(lldp_mgmt_conf_req_t));
        
		/* Read the header from received message and populate 
		 * sending message
		 */
 		req_p->mgmt_flags	= m_mgmtFlags;
        req_p->cmd_code		= m_cmdcode;
        req_p->msg_type	= m_msgType;
		
		strncpy(req_p->entry.intf_conf.id, m_id.c_str (), 63); 
		strncpy(req_p->entry.intf_conf.if_name, m_ifName.c_str (), 63); 
		req_p->entry.intf_conf.if_index = (unsigned int)m_disable;
		req_p->entry.intf_conf.if_type = m_ifType;

        /* Read the data from received message and populate 
		 * sending message
		 */
        if (IS_CMDCODE_SET(m_cmdcode, LLDP_INTF_CONF_DISABLE))
            req_p->entry.intf_conf.disable = m_disable;
        if (IS_CMDCODE_SET(m_cmdcode, LLDP_INTF_CONF_DCBXVERSION))
            req_p->entry.intf_conf.dcbx_version = m_dcbxVersion;
        if (IS_CMDCODE_SET(m_cmdcode, LLDP_INTF_CONF_PROFILE))
			strncpy(req_p->entry.intf_conf.profile_name, m_profile.c_str (), 63); 
        if (IS_CMDCODE_SET(m_cmdcode, LLDP_INTF_CONF_ISCSIPRIORITY))
            req_p->entry.intf_conf.iscsi_priority_bits = (1 << m_iscsiPriority);
        if (IS_CMDCODE_SET(m_cmdcode, LLDP_INTF_CONF_DESCRIPTION))
			strncpy(req_p->entry.intf_conf.description, m_description.c_str (), 63); 
        if (IS_CMDCODE_SET(m_cmdcode, LLDP_INTF_CONF_CEE_ON_OFF))
			req_p->entry.intf_conf.lldp_cee_type = m_lldpCeeType;
			

        return (req_p);
    }

	void NsmLldpClientIntfConfSetMessage::loadOutputsFromCStructure (
                                                const void *pOutputCStructure)
	{
	}

}
