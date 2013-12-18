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

#include "Lldp/Local/LldpLocalObjectManager.h"
#include "Lldp/Local/LldpProfileConfSetMessage.h"
#include "Lldp/Local/LldpTypes.h"

namespace DcmNs
{

    LldpProfileConfSetMessage::LldpProfileConfSetMessage ()
        : PrismMessage (LldpLocalObjectManager::getPrismServiceId (),LLDPPROFILECONFSET)
    {
    }

    LldpProfileConfSetMessage::LldpProfileConfSetMessage (const UI32 &mgmtFlags,const UI32 &cmdcode,const UI32 &msgType,const string &ProfileName,const string &description,const UI32 &hello,const UI8 &mode,const UI32 &multiplier,const bool &dcbxFcoeAppTlv,const bool &dcbxFcoeLogicalLinkTlv,const bool &dcbxIscsiAppTlv,const bool &dcbxTlv,const bool &dot1xTlv,const bool &dot3xTlv,const bool &mgmtAddrTlv,const bool &portDescrTlv,const bool &sysCapTlv,const bool &sysDescrTlv,const bool &sysNameTlv)
        : PrismMessage (LldpLocalObjectManager::getPrismServiceId (),LLDPPROFILECONFSET),
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

    LldpProfileConfSetMessage::~LldpProfileConfSetMessage ()
    {
    }

    void  LldpProfileConfSetMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
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

    void  LldpProfileConfSetMessage::setMgmtFlags(const UI32 &mgmtFlags)
    {
        m_mgmtFlags  =  mgmtFlags;
    }

    UI32  LldpProfileConfSetMessage::getMgmtFlags() const
    {
        return (m_mgmtFlags);
    }

    void  LldpProfileConfSetMessage::setCmdcode(const UI32 &cmdcode)
    {
        m_cmdcode  =  cmdcode;
    }

    UI32  LldpProfileConfSetMessage::getCmdcode() const
    {
        return (m_cmdcode);
    }

    void  LldpProfileConfSetMessage::setMsgType(const UI32 &msgType)
    {
        m_msgType  =  msgType;
    }

    UI32  LldpProfileConfSetMessage::getMsgType() const
    {
        return (m_msgType);
    }

    void  LldpProfileConfSetMessage::setProfileName(const string &ProfileName)
    {
        m_ProfileName  =  ProfileName;
    }

    string  LldpProfileConfSetMessage::getProfileName() const
    {
        return (m_ProfileName);
    }

    void  LldpProfileConfSetMessage::setDescription(const string &description)
    {
        m_description  =  description;
    }

    string  LldpProfileConfSetMessage::getDescription() const
    {
        return (m_description);
    }

    void  LldpProfileConfSetMessage::setHello(const UI32 &hello)
    {
        m_hello  =  hello;
    }

    UI32  LldpProfileConfSetMessage::getHello() const
    {
        return (m_hello);
    }

    void  LldpProfileConfSetMessage::setMode(const UI32 &mode)
    {
        m_mode  =  mode;
    }

    UI32  LldpProfileConfSetMessage::getMode() const
    {
        return (m_mode);
    }

    void  LldpProfileConfSetMessage::setMultiplier(const UI32 &multiplier)
    {
        m_multiplier  =  multiplier;
    }

    UI32  LldpProfileConfSetMessage::getMultiplier() const
    {
        return (m_multiplier);
    }

    void  LldpProfileConfSetMessage::setDcbxFcoeAppTlv(const bool &dcbxFcoeAppTlv)
    {
        m_dcbxFcoeAppTlv  =  dcbxFcoeAppTlv;
    }

    bool  LldpProfileConfSetMessage::getDcbxFcoeAppTlv() const
    {
        return (m_dcbxFcoeAppTlv);
    }

    void  LldpProfileConfSetMessage::setDcbxFcoeLogicalLinkTlv(const bool &dcbxFcoeLogicalLinkTlv)
    {
        m_dcbxFcoeLogicalLinkTlv  =  dcbxFcoeLogicalLinkTlv;
    }

    bool  LldpProfileConfSetMessage::getDcbxFcoeLogicalLinkTlv() const
    {
        return (m_dcbxFcoeLogicalLinkTlv);
    }

	void  LldpProfileConfSetMessage::setDcbxIscsiAppTlv(const bool &dcbxIscsiAppTlv)
	{
		m_dcbxIscsiAppTlv  =  dcbxIscsiAppTlv;
	}

	bool  LldpProfileConfSetMessage::getDcbxIscsiAppTlv() const
	{
		return (m_dcbxIscsiAppTlv);
	}

    void  LldpProfileConfSetMessage::setDcbxTlv(const bool &dcbxTlv)
    {
        m_dcbxTlv  =  dcbxTlv;
    }

    bool  LldpProfileConfSetMessage::getDcbxTlv() const
    {
        return (m_dcbxTlv);
    }

    void  LldpProfileConfSetMessage::setDot1xTlv(const bool &dot1xTlv)
    {
        m_dot1xTlv  =  dot1xTlv;
    }

    bool  LldpProfileConfSetMessage::getDot1xTlv() const
    {
        return (m_dot1xTlv);
    }

    void  LldpProfileConfSetMessage::setDot3xTlv(const bool &dot3xTlv)
    {
        m_dot3xTlv  =  dot3xTlv;
    }

    bool  LldpProfileConfSetMessage::getDot3xTlv() const
    {
        return (m_dot3xTlv);
    }

    void  LldpProfileConfSetMessage::setMgmtAddrTlv(const bool &mgmtAddrTlv)
    {
        m_mgmtAddrTlv  =  mgmtAddrTlv;
    }

    bool  LldpProfileConfSetMessage::getMgmtAddrTlv() const
    {
        return (m_mgmtAddrTlv);
    }

    void  LldpProfileConfSetMessage::setPortDescrTlv(const bool &portDescrTlv)
    {
        m_portDescrTlv  =  portDescrTlv;
    }

    bool  LldpProfileConfSetMessage::getPortDescrTlv() const
    {
        return (m_portDescrTlv);
    }

    void  LldpProfileConfSetMessage::setSysCapTlv(const bool &sysCapTlv)
    {
        m_sysCapTlv  =  sysCapTlv;
    }

    bool  LldpProfileConfSetMessage::getSysCapTlv() const
    {
        return (m_sysCapTlv);
    }

    void  LldpProfileConfSetMessage::setSysDescrTlv(const bool &sysDescrTlv)
    {
        m_sysDescrTlv  =  sysDescrTlv;
    }

    bool  LldpProfileConfSetMessage::getSysDescrTlv() const
    {
        return (m_sysDescrTlv);
    }

    void  LldpProfileConfSetMessage::setSysNameTlv(const bool &sysNameTlv)
    {
        m_sysNameTlv  =  sysNameTlv;
    }

    bool  LldpProfileConfSetMessage::getSysNameTlv() const
    {
        return (m_sysNameTlv);
    }

}
