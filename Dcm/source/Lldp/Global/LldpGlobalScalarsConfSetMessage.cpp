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

#include "Lldp/Global/LldpGlobalScalarsConfSetMessage.h"
#include "Lldp/Global/LldpGlobalObjectManager.h"
#include "Lldp/Global/LldpGlobalTypes.h"
#include "Utils/DcmMemUtil.h"
#include "Framework/Utils/TraceUtils.h"
namespace DcmNs
{

    LldpGlobalScalarsConfSetMessage::LldpGlobalScalarsConfSetMessage ()
        : ManagementInterfaceMessage (LldpGlobalObjectManager::getClassName (),LLDPGLOBALSCALARSCONFSET)
    {
        trace (TRACE_LEVEL_INFO,"LldpGlobalScalarsConfSetMessage::Constructor"
                                          "Entering..");
       	DcmMemUtil::memStatUpdate (DCM_MEM_TYPE_LLDP_GLOBALSCALARSCONFSETMESSAGE,sizeof(LldpGlobalScalarsConfSetMessage),DCMMEMSTATINCREMENT);

    }

    LldpGlobalScalarsConfSetMessage::LldpGlobalScalarsConfSetMessage (const UI32 &mgmtFlags,const UI32 &cmdcode,const UI32 &msgType,const string &description,const UI32 &hello,const UI32 &mode,const UI32 &multiplier,const bool &dcbxFcoeAppTlv,const bool &dcbxFcoeLogicalLinkTlv,const bool &dcbxIscsiAppTlv,const UI32 &iscsiPriority,const bool &dcbxTlv,const bool &dot1xTlv,const bool &dot3xTlv,const bool &mgmtAddrTlv,const bool &portDescrTlv,const bool &sysCapTlv,const bool &sysDescrTlv,const bool &sysNameTlv,const string &sysName,const string &sysDescr,const bool &disable)
        : ManagementInterfaceMessage (LldpGlobalObjectManager::getClassName (),LLDPGLOBALSCALARSCONFSET),
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
		trace (TRACE_LEVEL_INFO,"LldpGlobalScalarsConfSetMessage::Constructor"
			                                          "Entering..");
		DcmMemUtil::memStatUpdate (DCM_MEM_TYPE_LLDP_GLOBALSCALARSCONFSETMESSAGE,sizeof(LldpGlobalScalarsConfSetMessage),DCMMEMSTATINCREMENT);
    }

    LldpGlobalScalarsConfSetMessage::~LldpGlobalScalarsConfSetMessage ()
    {
		trace (TRACE_LEVEL_INFO,"LldpGlobalScalarsConfSetMessage::Destructor"
                                          "Entering..");
		DcmMemUtil::memStatUpdate (DCM_MEM_TYPE_LLDP_GLOBALSCALARSCONFSETMESSAGE,sizeof(LldpGlobalScalarsConfSetMessage),DCMMEMSTATDECREMENT);
    }

    void  LldpGlobalScalarsConfSetMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
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

    void  LldpGlobalScalarsConfSetMessage::setMgmtFlags(const UI32 &mgmtFlags)
    {
        m_mgmtFlags  =  mgmtFlags;
    }

    UI32  LldpGlobalScalarsConfSetMessage::getMgmtFlags() const
    {
        return (m_mgmtFlags);
    }

    void  LldpGlobalScalarsConfSetMessage::setCmdcode(const UI32 &cmdcode)
    {
        m_cmdcode  =  cmdcode;
    }

    UI32  LldpGlobalScalarsConfSetMessage::getCmdcode() const
    {
        return (m_cmdcode);
    }

    void  LldpGlobalScalarsConfSetMessage::setMsgType(const UI32 &msgType)
    {
        m_msgType  =  msgType;
    }

    UI32  LldpGlobalScalarsConfSetMessage::getMsgType() const
    {
        return (m_msgType);
    }

    void  LldpGlobalScalarsConfSetMessage::setDescription(const string &description)
    {
        m_description  =  description;
    }

    string  LldpGlobalScalarsConfSetMessage::getDescription() const
    {
        return (m_description);
    }

    void  LldpGlobalScalarsConfSetMessage::setHello(const UI32 &hello)
    {
        m_hello  =  hello;
    }

    UI32  LldpGlobalScalarsConfSetMessage::getHello() const
    {
        return (m_hello);
    }

    void  LldpGlobalScalarsConfSetMessage::setMode(const UI32 &mode)
    {
        m_mode  =  mode;
    }

    UI32  LldpGlobalScalarsConfSetMessage::getMode() const
    {
        return (m_mode);
    }

    void  LldpGlobalScalarsConfSetMessage::setMultiplier(const UI32 &multiplier)
    {
        m_multiplier  =  multiplier;
    }

    UI32  LldpGlobalScalarsConfSetMessage::getMultiplier() const
    {
        return (m_multiplier);
    }

    void  LldpGlobalScalarsConfSetMessage::setDcbxFcoeAppTlv(const bool &dcbxFcoeAppTlv)
    {
        m_dcbxFcoeAppTlv  =  dcbxFcoeAppTlv;
    }

    bool  LldpGlobalScalarsConfSetMessage::getDcbxFcoeAppTlv() const
    {
        return (m_dcbxFcoeAppTlv);
    }

    void  LldpGlobalScalarsConfSetMessage::setDcbxFcoeLogicalLinkTlv(const bool &dcbxFcoeLogicalLinkTlv)
    {
        m_dcbxFcoeLogicalLinkTlv  =  dcbxFcoeLogicalLinkTlv;
    }

    bool  LldpGlobalScalarsConfSetMessage::getDcbxFcoeLogicalLinkTlv() const
    {
        return (m_dcbxFcoeLogicalLinkTlv);
    }

	void  LldpGlobalScalarsConfSetMessage::setDcbxIscsiAppTlv(const bool &dcbxIscsiAppTlv)
	{
		m_dcbxIscsiAppTlv  =  dcbxIscsiAppTlv;
	}

	bool  LldpGlobalScalarsConfSetMessage::getDcbxIscsiAppTlv() const
	{
		return (m_dcbxIscsiAppTlv);
	}

    void  LldpGlobalScalarsConfSetMessage::setIscsiPriority(const UI32 &iscsiPriority)
    {
        m_iscsiPriority  =  iscsiPriority;
    }

    UI32  LldpGlobalScalarsConfSetMessage::getIscsiPriority() const
    {
        return (m_iscsiPriority);
    }

    void  LldpGlobalScalarsConfSetMessage::setDcbxTlv(const bool &dcbxTlv)
    {
        m_dcbxTlv  =  dcbxTlv;
    }

    bool  LldpGlobalScalarsConfSetMessage::getDcbxTlv() const
    {
        return (m_dcbxTlv);
    }

    void  LldpGlobalScalarsConfSetMessage::setDot1xTlv(const bool &dot1xTlv)
    {
        m_dot1xTlv  =  dot1xTlv;
    }

    bool  LldpGlobalScalarsConfSetMessage::getDot1xTlv() const
    {
        return (m_dot1xTlv);
    }

    void  LldpGlobalScalarsConfSetMessage::setDot3xTlv(const bool &dot3xTlv)
    {
        m_dot3xTlv  =  dot3xTlv;
    }

    bool  LldpGlobalScalarsConfSetMessage::getDot3xTlv() const
    {
        return (m_dot3xTlv);
    }

    void  LldpGlobalScalarsConfSetMessage::setMgmtAddrTlv(const bool &mgmtAddrTlv)
    {
        m_mgmtAddrTlv  =  mgmtAddrTlv;
    }

    bool  LldpGlobalScalarsConfSetMessage::getMgmtAddrTlv() const
    {
        return (m_mgmtAddrTlv);
    }

    void  LldpGlobalScalarsConfSetMessage::setPortDescrTlv(const bool &portDescrTlv)
    {
        m_portDescrTlv  =  portDescrTlv;
    }

    bool  LldpGlobalScalarsConfSetMessage::getPortDescrTlv() const
    {
        return (m_portDescrTlv);
    }

    void  LldpGlobalScalarsConfSetMessage::setSysCapTlv(const bool &sysCapTlv)
    {
        m_sysCapTlv  =  sysCapTlv;
    }

    bool  LldpGlobalScalarsConfSetMessage::getSysCapTlv() const
    {
        return (m_sysCapTlv);
    }

    void  LldpGlobalScalarsConfSetMessage::setSysDescrTlv(const bool &sysDescrTlv)
    {
        m_sysDescrTlv  =  sysDescrTlv;
    }

    bool  LldpGlobalScalarsConfSetMessage::getSysDescrTlv() const
    {
        return (m_sysDescrTlv);
    }

    void  LldpGlobalScalarsConfSetMessage::setSysNameTlv(const bool &sysNameTlv)
    {
        m_sysNameTlv  =  sysNameTlv;
    }

    bool  LldpGlobalScalarsConfSetMessage::getSysNameTlv() const
    {
        return (m_sysNameTlv);
    }

    void  LldpGlobalScalarsConfSetMessage::setSysName(const string &sysName)
    {
        m_sysName  =  sysName;
    }

    string  LldpGlobalScalarsConfSetMessage::getSysName() const
    {
        return (m_sysName);
    }

    void  LldpGlobalScalarsConfSetMessage::setSysDescr(const string &sysDescr)
    {
        m_sysDescr  =  sysDescr;
    }

    string  LldpGlobalScalarsConfSetMessage::getSysDescr() const
    {
        return (m_sysDescr);
    }

    void  LldpGlobalScalarsConfSetMessage::setDisable(const bool &disable)
    {
        m_disable  =  disable;
    }

    bool  LldpGlobalScalarsConfSetMessage::getDisable() const
    {
        return (m_disable);
    }

}
