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
 *   Copyright (C) 2005-2010 Brocade Coreq_pmunications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : mmohan                                                     *
 **************************************************************************/
#include "Framework/Core/Wave.h"
#include "Framework/Messaging/Local/PrismMessage.h"
/*
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Shell/WaveCliShell.h"
*/
#include "ClientInterface/Lldp/LldpClientScalarsConfSetMessage.h"
#include "ClientInterface/Lldp/lldp_dcm_def.h"
#include "ClientInterface/DceWaveClientMessage.h"

namespace DcmNs
{

    LldpClientScalarsConfSetMessage::LldpClientScalarsConfSetMessage ()
        : DcmManagementInterfaceMessage (MSG_LLDP_SCALARS_GLOBAL_CONF)
    {
    }

    LldpClientScalarsConfSetMessage::LldpClientScalarsConfSetMessage (const UI32 &mgmtFlags,const UI32 &cmdcode,const UI32 &msgType,const string &description,const UI32 &hello,const UI32 &mode,const UI32 &multiplier,const bool &dcbxFcoeAppTlv,const bool &dcbxFcoeLogicalLinkTlv,const bool &dcbxIscsiAppTlv,const UI32 &iscsiPriority,const bool &dcbxTlv,const bool &dot1xTlv,const bool &dot3xTlv,const bool &mgmtAddrTlv,const bool &portDescrTlv,const bool &sysCapTlv,const bool &sysDescrTlv,const bool &sysNameTlv,const string &sysName,const string &sysDescr,const bool &disable)
        : DcmManagementInterfaceMessage (MSG_LLDP_SCALARS_GLOBAL_CONF),
        m_mgmtFlags    (mgmtFlags),
        m_cmdcode    (cmdcode),
        m_msgType    (msgType),
        m_description    (description),
        m_hello    (hello),
        m_mode    (mode),
        m_multiplier    (multiplier),
        m_dcbxFcoeAppTlv    (dcbxFcoeAppTlv),
        m_dcbxFcoeLogicalLinkTlv    (dcbxFcoeLogicalLinkTlv),
        m_dcbxIscsiAppTlv    (dcbxIscsiAppTlv),
        m_iscsiPriority    (iscsiPriority),
        m_dcbxTlv    (dcbxTlv),
        m_dot1xTlv    (dot1xTlv),
        m_dot3xTlv    (dot3xTlv),
        m_mgmtAddrTlv    (mgmtAddrTlv),
        m_portDescrTlv    (portDescrTlv),
        m_sysCapTlv    (sysCapTlv),
        m_sysDescrTlv    (sysDescrTlv),
        m_sysNameTlv    (sysNameTlv),
        m_sysName    (sysName),
        m_sysDescr    (sysDescr),
        m_disable    (disable)
    {
    }

    LldpClientScalarsConfSetMessage::~LldpClientScalarsConfSetMessage ()
    {
    }

    void  LldpClientScalarsConfSetMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_mgmtFlags,"mgmtFlags"));
        addSerializableAttribute (new AttributeUI32(&m_cmdcode,"cmdcode"));
        addSerializableAttribute (new AttributeUI32(&m_msgType,"msgType"));
        addSerializableAttribute (new AttributeString(&m_description,"description"));
        addSerializableAttribute (new AttributeUI32(&m_hello,"hello"));
        addSerializableAttribute (new AttributeUI32(&m_mode,"mode"));
        addSerializableAttribute (new AttributeUI32(&m_multiplier,"multiplier"));
        addSerializableAttribute (new AttributeBool(&m_dcbxFcoeAppTlv,"dcbxFcoeAppTlv"));
        addSerializableAttribute (new AttributeBool(&m_dcbxFcoeLogicalLinkTlv,"dcbxFcoeLogicalLinkTlv"));
        addSerializableAttribute (new AttributeBool(&m_dcbxIscsiAppTlv,"dcbxIscsiAppTlv"));
        addSerializableAttribute (new AttributeUI32(&m_iscsiPriority,"iscsiPriority"));
        addSerializableAttribute (new AttributeBool(&m_dcbxTlv,"dcbxTlv"));
        addSerializableAttribute (new AttributeBool(&m_dot1xTlv,"dot1xTlv"));
        addSerializableAttribute (new AttributeBool(&m_dot3xTlv,"dot3xTlv"));
        addSerializableAttribute (new AttributeBool(&m_mgmtAddrTlv,"mgmtAddrTlv"));
        addSerializableAttribute (new AttributeBool(&m_portDescrTlv,"portDescrTlv"));
        addSerializableAttribute (new AttributeBool(&m_sysCapTlv,"sysCapTlv"));
        addSerializableAttribute (new AttributeBool(&m_sysDescrTlv,"sysDescrTlv"));
        addSerializableAttribute (new AttributeBool(&m_sysNameTlv,"sysNameTlv"));
        addSerializableAttribute (new AttributeString(&m_sysName,"sysName"));
        addSerializableAttribute (new AttributeString(&m_sysDescr,"sysDescr"));
        addSerializableAttribute (new AttributeBool(&m_disable,"disable"));
    }

    void  LldpClientScalarsConfSetMessage::setMgmtFlags(const UI32 &mgmtFlags)
    {
        m_mgmtFlags  =  mgmtFlags;
    }

    UI32  LldpClientScalarsConfSetMessage::getMgmtFlags() const
    {
        return (m_mgmtFlags);
    }

    void  LldpClientScalarsConfSetMessage::setCmdcode(const UI32 &cmdcode)
    {
        m_cmdcode  =  cmdcode;
    }

    UI32  LldpClientScalarsConfSetMessage::getCmdcode() const
    {
        return (m_cmdcode);
    }

    void  LldpClientScalarsConfSetMessage::setMsgType(const UI32 &msgType)
    {
        m_msgType  =  msgType;
    }

    UI32  LldpClientScalarsConfSetMessage::getMsgType() const
    {
        return (m_msgType);
    }

    void  LldpClientScalarsConfSetMessage::setDescription(const string &description)
    {
        m_description  =  description;
    }

    string  LldpClientScalarsConfSetMessage::getDescription() const
    {
        return (m_description);
    }

    void  LldpClientScalarsConfSetMessage::setHello(const UI32 &hello)
    {
        m_hello  =  hello;
    }

    UI32  LldpClientScalarsConfSetMessage::getHello() const
    {
        return (m_hello);
    }

    void  LldpClientScalarsConfSetMessage::setMode(const UI32 &mode)
    {
        m_mode  =  mode;
    }

    UI32  LldpClientScalarsConfSetMessage::getMode() const
    {
        return (m_mode);
    }

    void  LldpClientScalarsConfSetMessage::setMultiplier(const UI32 &multiplier)
    {
        m_multiplier  =  multiplier;
    }

    UI32  LldpClientScalarsConfSetMessage::getMultiplier() const
    {
        return (m_multiplier);
    }

    void  LldpClientScalarsConfSetMessage::setDcbxFcoeAppTlv(const bool &dcbxFcoeAppTlv)
    {
        m_dcbxFcoeAppTlv  =  dcbxFcoeAppTlv;
    }

    bool  LldpClientScalarsConfSetMessage::getDcbxFcoeAppTlv() const
    {
        return (m_dcbxFcoeAppTlv);
    }

    void  LldpClientScalarsConfSetMessage::setDcbxFcoeLogicalLinkTlv(const bool &dcbxFcoeLogicalLinkTlv)
    {
        m_dcbxFcoeLogicalLinkTlv  =  dcbxFcoeLogicalLinkTlv;
    }

    bool  LldpClientScalarsConfSetMessage::getDcbxFcoeLogicalLinkTlv() const
    {
        return (m_dcbxFcoeLogicalLinkTlv);
    }

	void  LldpClientScalarsConfSetMessage::setDcbxIscsiAppTlv(const bool &dcbxIscsiAppTlv)
	{
		m_dcbxIscsiAppTlv  =  dcbxIscsiAppTlv;
	}

	bool  LldpClientScalarsConfSetMessage::getDcbxIscsiAppTlv() const
	{
		return (m_dcbxIscsiAppTlv);
	}

    void  LldpClientScalarsConfSetMessage::setIscsiPriority(const UI32 &iscsiPriority)
    {
        m_iscsiPriority  =  iscsiPriority;
    }

    UI32  LldpClientScalarsConfSetMessage::getIscsiPriority() const
    {
        return (m_iscsiPriority);
    }

    void  LldpClientScalarsConfSetMessage::setDcbxTlv(const bool &dcbxTlv)
    {
        m_dcbxTlv  =  dcbxTlv;
    }

    bool  LldpClientScalarsConfSetMessage::getDcbxTlv() const
    {
        return (m_dcbxTlv);
    }

    void  LldpClientScalarsConfSetMessage::setDot1xTlv(const bool &dot1xTlv)
    {
        m_dot1xTlv  =  dot1xTlv;
    }

    bool  LldpClientScalarsConfSetMessage::getDot1xTlv() const
    {
        return (m_dot1xTlv);
    }

    void  LldpClientScalarsConfSetMessage::setDot3xTlv(const bool &dot3xTlv)
    {
        m_dot3xTlv  =  dot3xTlv;
    }

    bool  LldpClientScalarsConfSetMessage::getDot3xTlv() const
    {
        return (m_dot3xTlv);
    }

    void  LldpClientScalarsConfSetMessage::setMgmtAddrTlv(const bool &mgmtAddrTlv)
    {
        m_mgmtAddrTlv  =  mgmtAddrTlv;
    }

    bool  LldpClientScalarsConfSetMessage::getMgmtAddrTlv() const
    {
        return (m_mgmtAddrTlv);
    }

    void  LldpClientScalarsConfSetMessage::setPortDescrTlv(const bool &portDescrTlv)
    {
        m_portDescrTlv  =  portDescrTlv;
    }

    bool  LldpClientScalarsConfSetMessage::getPortDescrTlv() const
    {
        return (m_portDescrTlv);
    }

    void  LldpClientScalarsConfSetMessage::setSysCapTlv(const bool &sysCapTlv)
    {
        m_sysCapTlv  =  sysCapTlv;
    }

    bool  LldpClientScalarsConfSetMessage::getSysCapTlv() const
    {
        return (m_sysCapTlv);
    }

    void  LldpClientScalarsConfSetMessage::setSysDescrTlv(const bool &sysDescrTlv)
    {
        m_sysDescrTlv  =  sysDescrTlv;
    }

    bool  LldpClientScalarsConfSetMessage::getSysDescrTlv() const
    {
        return (m_sysDescrTlv);
    }

    void  LldpClientScalarsConfSetMessage::setSysNameTlv(const bool &sysNameTlv)
    {
        m_sysNameTlv  =  sysNameTlv;
    }

    bool  LldpClientScalarsConfSetMessage::getSysNameTlv() const
    {
        return (m_sysNameTlv);
    }

    void  LldpClientScalarsConfSetMessage::setSysName(const string &sysName)
    {
        m_sysName  =  sysName;
    }

    string  LldpClientScalarsConfSetMessage::getSysName() const
    {
        return (m_sysName);
    }

    void  LldpClientScalarsConfSetMessage::setSysDescr(const string &sysDescr)
    {
        m_sysDescr  =  sysDescr;
    }

    string  LldpClientScalarsConfSetMessage::getSysDescr() const
    {
        return (m_sysDescr);
    }

    void  LldpClientScalarsConfSetMessage::setDisable(const bool &disable)
    {
        m_disable  =  disable;
    }

    bool  LldpClientScalarsConfSetMessage::getDisable() const
    {
        return (m_disable);
    }
	
	const void *LldpClientScalarsConfSetMessage::getCStructureForInputs ()
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
        if (IS_CMDCODE_SET(m_cmdcode, LLDP_GLOBAL_CONF_DESC)) {
			if(m_description.length ())
			{
				strncpy(req_p->entry.global_conf.description, m_description.c_str (), 64); 
			}
			else
			{
				strncpy(req_p->entry.global_conf.description, "", 64); 
		}
			req_p->entry.global_conf.description[63] = '\0';
		}
        if (IS_CMDCODE_SET(m_cmdcode, LLDP_GLOBAL_CONF_HELLO))
            req_p->entry.global_conf.hello = m_hello;
        if (IS_CMDCODE_SET(m_cmdcode, LLDP_GLOBAL_CONF_MODE))
            req_p->entry.global_conf.mode = m_mode;
        if (IS_CMDCODE_SET(m_cmdcode, LLDP_GLOBAL_CONF_MULTIPLIER))
            req_p->entry.global_conf.multiplier	= m_multiplier;
        if (IS_CMDCODE_SET(m_cmdcode, LLDP_GLOBAL_CONF_DCBXFCOEAPPTLV))
            req_p->entry.global_conf.dcbx_fcoeapp_tlv = m_dcbxFcoeAppTlv;
        if (IS_CMDCODE_SET(m_cmdcode, LLDP_GLOBAL_CONF_DCBXFCOELOGICALLINKTLV))
            req_p->entry.global_conf.dcbx_fcoelogicallink_tlv = m_dcbxFcoeLogicalLinkTlv;
		if (IS_CMDCODE_SET(m_cmdcode, LLDP_GLOBAL_CONF_DCBXISCSIAPPTLV))
			req_p->entry.global_conf.dcbx_iscsiapp_tlv = m_dcbxIscsiAppTlv;
        if (IS_CMDCODE_SET(m_cmdcode, LLDP_GLOBAL_CONF_ISCSIPRIORITY))
            req_p->entry.global_conf.iscsi_priority_bits = (1 << m_iscsiPriority);
        if (IS_CMDCODE_SET(m_cmdcode, LLDP_GLOBAL_CONF_DCBXTLV))
            req_p->entry.global_conf.dcbx_tlv = m_dcbxTlv;
        if (IS_CMDCODE_SET(m_cmdcode, LLDP_GLOBAL_CONF_DOT1XTLV))
            req_p->entry.global_conf.dot1x_tlv	= m_dot1xTlv;
        if (IS_CMDCODE_SET(m_cmdcode, LLDP_GLOBAL_CONF_DOT3XTLV))
            req_p->entry.global_conf.dot3x_tlv	= m_dot3xTlv ;
        if (IS_CMDCODE_SET(m_cmdcode, LLDP_GLOBAL_CONF_MGMTADDRTLV))
            req_p->entry.global_conf.mgmtaddr_tlv = m_mgmtAddrTlv ;
        if (IS_CMDCODE_SET(m_cmdcode, LLDP_GLOBAL_CONF_PORTDESCRTLV))
            req_p->entry.global_conf.portdescr_tlv = m_portDescrTlv ;
        if (IS_CMDCODE_SET(m_cmdcode, LLDP_GLOBAL_CONF_SYSCAPTLV))
            req_p->entry.global_conf.syscap_tlv = m_sysCapTlv;
        if (IS_CMDCODE_SET(m_cmdcode, LLDP_GLOBAL_CONF_SYSDESCRTLV))
            req_p->entry.global_conf.sysdescr_tlv = m_sysDescrTlv;
        if (IS_CMDCODE_SET(m_cmdcode, LLDP_GLOBAL_CONF_SYSNAMETLV))
            req_p->entry.global_conf.sysname_tlv = m_sysNameTlv ;
        if (IS_CMDCODE_SET(m_cmdcode, LLDP_GLOBAL_CONF_SYSNAME))
			strncpy(req_p->entry.global_conf.sysname, m_sysName.c_str (), 64); 
			req_p->entry.global_conf.sysname[63] = '\0';
        if (IS_CMDCODE_SET(m_cmdcode, LLDP_GLOBAL_CONF_SYSDESCR))
			strncpy(req_p->entry.global_conf.sysdescr, m_sysDescr.c_str (), 64); 
			req_p->entry.global_conf.sysdescr[63] = '\0';
        if (IS_CMDCODE_SET(m_cmdcode, LLDP_GLOBAL_CONF_DISABLE))
            req_p->entry.global_conf.disable	= m_disable;

        return (req_p);
    }

	void LldpClientScalarsConfSetMessage::loadOutputsFromCStructure (
                                                const void *pOutputCStructure)
	{
	}
}
