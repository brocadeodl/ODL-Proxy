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
 *   Author : mmohan                                                     *
 **************************************************************************/

#include "Framework/Core/Wave.h"
#include "Framework/Messaging/Local/PrismMessage.h"
#include "ClientInterface/Lldp/LldpClientProfileConfSetMessage.h"
#include "ClientInterface/Lldp/lldp_dcm_def.h"
#include "ClientInterface/DceWaveClientMessage.h"


namespace DcmNs
{

    LldpClientProfileConfSetMessage::LldpClientProfileConfSetMessage ()
        : DcmManagementInterfaceMessage (MSG_LLDP_PROFILE_GLOBAL_CONF)
    {
    }

    LldpClientProfileConfSetMessage::LldpClientProfileConfSetMessage (const UI32 &mgmtFlags,const UI32 &cmdcode,const UI32 &msgType,const string &ProfileName,const string &description,const UI32 &hello,const UI8 &mode,const UI32 &multiplier,const bool &dcbxFcoeAppTlv,const bool &dcbxFcoeLogicalLinkTlv,const bool &dcbxIscsiAppTlv,const bool &dcbxTlv,const bool &dot1xTlv,const bool &dot3xTlv,const bool &mgmtAddrTlv,const bool &portDescrTlv,const bool &sysCapTlv,const bool &sysDescrTlv,const bool &sysNameTlv)
        : DcmManagementInterfaceMessage (MSG_LLDP_PROFILE_GLOBAL_CONF),
        m_mgmtFlags    (mgmtFlags),
        m_cmdcode    (cmdcode),
        m_msgType    (msgType),
        m_ProfileName    (ProfileName),
        m_description    (description),
        m_hello    (hello),
        m_mode    (mode),
        m_multiplier    (multiplier),
        m_dcbxFcoeAppTlv    (dcbxFcoeAppTlv),
        m_dcbxFcoeLogicalLinkTlv    (dcbxFcoeLogicalLinkTlv),
        m_dcbxIscsiAppTlv    (dcbxIscsiAppTlv),
		m_dcbxTlv    (dcbxTlv),
        m_dot1xTlv    (dot1xTlv),
        m_dot3xTlv    (dot3xTlv),
        m_mgmtAddrTlv    (mgmtAddrTlv),
        m_portDescrTlv    (portDescrTlv),
        m_sysCapTlv    (sysCapTlv),
        m_sysDescrTlv    (sysDescrTlv),
        m_sysNameTlv    (sysNameTlv)
    {
    }

    LldpClientProfileConfSetMessage::~LldpClientProfileConfSetMessage ()
    {
    }

    void  LldpClientProfileConfSetMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_mgmtFlags,"mgmtFlags"));
        addSerializableAttribute (new AttributeUI32(&m_cmdcode,"cmdcode"));
        addSerializableAttribute (new AttributeUI32(&m_msgType,"msgType"));
        addSerializableAttribute (new AttributeString(&m_ProfileName,"ProfileName"));
        addSerializableAttribute (new AttributeString(&m_description,"description"));
        addSerializableAttribute (new AttributeUI32(&m_hello,"hello"));
        addSerializableAttribute (new AttributeUI32(&m_mode,"mode"));
        addSerializableAttribute (new AttributeUI32(&m_multiplier,"multiplier"));
        addSerializableAttribute (new AttributeBool(&m_dcbxFcoeAppTlv,"dcbxFcoeAppTlv"));
        addSerializableAttribute (new AttributeBool(&m_dcbxFcoeLogicalLinkTlv,"dcbxFcoeLogicalLinkTlv"));
		addSerializableAttribute (new AttributeBool(&m_dcbxIscsiAppTlv,"dcbxIscsiAppTlv"));
        addSerializableAttribute (new AttributeBool(&m_dcbxTlv,"dcbxTlv"));
        addSerializableAttribute (new AttributeBool(&m_dot1xTlv,"dot1xTlv"));
        addSerializableAttribute (new AttributeBool(&m_dot3xTlv,"dot3xTlv"));
        addSerializableAttribute (new AttributeBool(&m_mgmtAddrTlv,"mgmtAddrTlv"));
        addSerializableAttribute (new AttributeBool(&m_portDescrTlv,"portDescrTlv"));
        addSerializableAttribute (new AttributeBool(&m_sysCapTlv,"sysCapTlv"));
        addSerializableAttribute (new AttributeBool(&m_sysDescrTlv,"sysDescrTlv"));
        addSerializableAttribute (new AttributeBool(&m_sysNameTlv,"sysNameTlv"));
    }

    void  LldpClientProfileConfSetMessage::setMgmtFlags(const UI32 &mgmtFlags)
    {
        m_mgmtFlags  =  mgmtFlags;
    }

    UI32  LldpClientProfileConfSetMessage::getMgmtFlags() const
    {
        return (m_mgmtFlags);
    }

    void  LldpClientProfileConfSetMessage::setCmdcode(const UI32 &cmdcode)
    {
        m_cmdcode  =  cmdcode;
    }

    UI32  LldpClientProfileConfSetMessage::getCmdcode() const
    {
        return (m_cmdcode);
    }

    void  LldpClientProfileConfSetMessage::setMsgType(const UI32 &msgType)
    {
        m_msgType  =  msgType;
    }

    UI32  LldpClientProfileConfSetMessage::getMsgType() const
    {
        return (m_msgType);
    }

    void  LldpClientProfileConfSetMessage::setProfileName(const string &ProfileName)
    {
        m_ProfileName  =  ProfileName;
    }

    string  LldpClientProfileConfSetMessage::getProfileName() const
    {
        return (m_ProfileName);
    }

    void  LldpClientProfileConfSetMessage::setDescription(const string &description)
    {
        m_description  =  description;
    }

    string  LldpClientProfileConfSetMessage::getDescription() const
    {
        return (m_description);
    }

    void  LldpClientProfileConfSetMessage::setHello(const UI32 &hello)
    {
        m_hello  =  hello;
    }

    UI32  LldpClientProfileConfSetMessage::getHello() const
    {
        return (m_hello);
    }

    void  LldpClientProfileConfSetMessage::setMode(const UI32 &mode)
    {
        m_mode  =  mode;
    }

    UI32  LldpClientProfileConfSetMessage::getMode() const
    {
        return (m_mode);
    }

    void  LldpClientProfileConfSetMessage::setMultiplier(const UI32 &multiplier)
    {
        m_multiplier  =  multiplier;
    }

    UI32  LldpClientProfileConfSetMessage::getMultiplier() const
    {
        return (m_multiplier);
    }

    void  LldpClientProfileConfSetMessage::setDcbxFcoeAppTlv(const bool &dcbxFcoeAppTlv)
    {
        m_dcbxFcoeAppTlv  =  dcbxFcoeAppTlv;
    }

    bool  LldpClientProfileConfSetMessage::getDcbxFcoeAppTlv() const
    {
        return (m_dcbxFcoeAppTlv);
    }

    void  LldpClientProfileConfSetMessage::setDcbxFcoeLogicalLinkTlv(const bool &dcbxFcoeLogicalLinkTlv)
    {
        m_dcbxFcoeLogicalLinkTlv  =  dcbxFcoeLogicalLinkTlv;
    }

    bool  LldpClientProfileConfSetMessage::getDcbxFcoeLogicalLinkTlv() const
    {
        return (m_dcbxFcoeLogicalLinkTlv);
    }

	void  LldpClientProfileConfSetMessage::setDcbxIscsiAppTlv(const bool &dcbxIscsiAppTlv)
	{
		m_dcbxIscsiAppTlv  =  dcbxIscsiAppTlv;
	}

	bool  LldpClientProfileConfSetMessage::getDcbxIscsiAppTlv() const
	{
		return (m_dcbxIscsiAppTlv);
	}

    void  LldpClientProfileConfSetMessage::setDcbxTlv(const bool &dcbxTlv)
    {
        m_dcbxTlv  =  dcbxTlv;
    }

    bool  LldpClientProfileConfSetMessage::getDcbxTlv() const
    {
        return (m_dcbxTlv);
    }

    void  LldpClientProfileConfSetMessage::setDot1xTlv(const bool &dot1xTlv)
    {
        m_dot1xTlv  =  dot1xTlv;
    }

    bool  LldpClientProfileConfSetMessage::getDot1xTlv() const
    {
        return (m_dot1xTlv);
    }

    void  LldpClientProfileConfSetMessage::setDot3xTlv(const bool &dot3xTlv)
    {
        m_dot3xTlv  =  dot3xTlv;
    }

    bool  LldpClientProfileConfSetMessage::getDot3xTlv() const
    {
        return (m_dot3xTlv);
    }

    void  LldpClientProfileConfSetMessage::setMgmtAddrTlv(const bool &mgmtAddrTlv)
    {
        m_mgmtAddrTlv  =  mgmtAddrTlv;
    }

    bool  LldpClientProfileConfSetMessage::getMgmtAddrTlv() const
    {
        return (m_mgmtAddrTlv);
    }

    void  LldpClientProfileConfSetMessage::setPortDescrTlv(const bool &portDescrTlv)
    {
        m_portDescrTlv  =  portDescrTlv;
    }

    bool  LldpClientProfileConfSetMessage::getPortDescrTlv() const
    {
        return (m_portDescrTlv);
    }

    void  LldpClientProfileConfSetMessage::setSysCapTlv(const bool &sysCapTlv)
    {
        m_sysCapTlv  =  sysCapTlv;
    }

    bool  LldpClientProfileConfSetMessage::getSysCapTlv() const
    {
        return (m_sysCapTlv);
    }

    void  LldpClientProfileConfSetMessage::setSysDescrTlv(const bool &sysDescrTlv)
    {
        m_sysDescrTlv  =  sysDescrTlv;
    }

    bool  LldpClientProfileConfSetMessage::getSysDescrTlv() const
    {
        return (m_sysDescrTlv);
    }

    void  LldpClientProfileConfSetMessage::setSysNameTlv(const bool &sysNameTlv)
    {
        m_sysNameTlv  =  sysNameTlv;
    }

    bool  LldpClientProfileConfSetMessage::getSysNameTlv() const
    {
        return (m_sysNameTlv);
    }
	
	const void *LldpClientProfileConfSetMessage::getCStructureForInputs ()
    {
        lldp_mgmt_conf_req_t *req_p = (lldp_mgmt_conf_req_t*)calloc(1, sizeof(lldp_mgmt_conf_req_t));
		
		/* Read the header from received message and populate 
		 * sending message
		 */
 		req_p->mgmt_flags	= m_mgmtFlags;
        req_p->cmd_code		= m_cmdcode;
        req_p->msg_type	= m_msgType;

        /* Read the data from received message and populate 
		 * sending message
		 */
		if (IS_CMDCODE_SET(m_cmdcode, LLDP_GLOBAL_CONF_PROFILE)) 
			strncpy(req_p->entry.profile_conf.profile_name, m_ProfileName.c_str (), 64); 
			req_p->entry.profile_conf.profile_name[63] = '\0';
				
        if (IS_CMDCODE_SET(m_cmdcode, LLDP_GLOBAL_PROFILE_CONF_DESC))
			strncpy(req_p->entry.profile_conf.description, m_description.c_str (), 64); 
			req_p->entry.profile_conf.description[63] = '\0';
        if (IS_CMDCODE_SET(m_cmdcode, LLDP_GLOBAL_PROFILE_CONF_HELLO))
            req_p->entry.profile_conf.hello = m_hello;
        if (IS_CMDCODE_SET(m_cmdcode, LLDP_GLOBAL_PROFILE_CONF_MODE))
            req_p->entry.profile_conf.mode = m_mode;
        if (IS_CMDCODE_SET(m_cmdcode, LLDP_GLOBAL_PROFILE_CONF_MULTIPLIER))
            req_p->entry.profile_conf.multiplier	= m_multiplier;
        if (IS_CMDCODE_SET(m_cmdcode, LLDP_GLOBAL_PROFILE_CONF_DCBXFCOEAPPTLV))
            req_p->entry.profile_conf.dcbx_fcoeapp_tlv = m_dcbxFcoeAppTlv;
        if (IS_CMDCODE_SET(m_cmdcode, LLDP_GLOBAL_PROFILE_CONF_DCBXFCOELOGICALLINKTLV))
            req_p->entry.profile_conf.dcbx_fcoelogicallink_tlv = m_dcbxFcoeLogicalLinkTlv;
		if (IS_CMDCODE_SET(m_cmdcode, LLDP_GLOBAL_PROFILE_CONF_DCBXISCSIAPPTLV))
			req_p->entry.profile_conf.dcbx_iscsiapp_tlv = m_dcbxIscsiAppTlv;
        if (IS_CMDCODE_SET(m_cmdcode, LLDP_GLOBAL_PROFILE_CONF_DCBXTLV))
            req_p->entry.profile_conf.dcbx_tlv = m_dcbxTlv;
        if (IS_CMDCODE_SET(m_cmdcode, LLDP_GLOBAL_PROFILE_CONF_DOT1XTLV))
            req_p->entry.profile_conf.dot1x_tlv	= m_dot1xTlv;
        if (IS_CMDCODE_SET(m_cmdcode, LLDP_GLOBAL_PROFILE_CONF_DOT3XTLV))
            req_p->entry.profile_conf.dot3x_tlv	= m_dot3xTlv ;
        if (IS_CMDCODE_SET(m_cmdcode, LLDP_GLOBAL_PROFILE_CONF_MGMTADDRTLV))
            req_p->entry.profile_conf.mgmtaddr_tlv = m_mgmtAddrTlv ;
        if (IS_CMDCODE_SET(m_cmdcode, LLDP_GLOBAL_PROFILE_CONF_PORTDESCRTLV))
            req_p->entry.profile_conf.portdescr_tlv = m_portDescrTlv ;
        if (IS_CMDCODE_SET(m_cmdcode, LLDP_GLOBAL_PROFILE_CONF_SYSCAPTLV))
            req_p->entry.profile_conf.syscap_tlv = m_sysCapTlv;
        if (IS_CMDCODE_SET(m_cmdcode, LLDP_GLOBAL_PROFILE_CONF_SYSDESCRTLV))
            req_p->entry.profile_conf.sysdescr_tlv = m_sysDescrTlv;
        if (IS_CMDCODE_SET(m_cmdcode, LLDP_GLOBAL_PROFILE_CONF_SYSNAMETLV))
            req_p->entry.profile_conf.sysname_tlv = m_sysNameTlv ;

        return (req_p);
    }

	void LldpClientProfileConfSetMessage::loadOutputsFromCStructure (
                                                const void *pOutputCStructure)
	{
	}

}
