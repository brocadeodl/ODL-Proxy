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

#include "Lldp/Global/LldpGlobalProfileConfSetMessage.h"
#include "Lldp/Global/LldpGlobalObjectManager.h"
#include "Framework/Utils/TraceUtils.h"
#include "Lldp/Global/LldpGlobalTypes.h"
#include "Utils/DcmMemUtil.h"
#include "Framework/Attributes/AttributeUI32UC.h"
#include "Framework/Attributes/AttributeEnumUC.h"
#include "Framework/Attributes/AttributeBoolUC.h"
#include "Framework/Attributes/AttributeStringUC.h"

namespace DcmNs
{

    LldpGlobalProfileConfSetMessage::LldpGlobalProfileConfSetMessage ()
        : ManagementInterfaceMessage (LldpGlobalObjectManager::getClassName (),LLDPGLOBALPROFILECONFSET)
    {
		trace (TRACE_LEVEL_INFO,"ldpGlobalProfileConfSetMessage::Constructor"
						                  "Entering..");
		DcmMemUtil::memStatUpdate (DCM_MEM_TYPE_LLDP_GLOBALPROFILECONFSETMESSAGE,sizeof(LldpGlobalProfileConfSetMessage),DCMMEMSTATINCREMENT);
    }

    LldpGlobalProfileConfSetMessage::LldpGlobalProfileConfSetMessage (const UI32 &mgmtFlags,const UI32 &cmdcode,const UI32 &msgType,const string &profileName,const StringUC &description,const UI32UC &hello,const EnumUC &mode,const UI32UC &multiplier,const BoolUC &dcbxFcoeAppTlv,const BoolUC &dcbxFcoeLogicalLinkTlv,const BoolUC &dcbxIscsiAppTlv,const BoolUC &dcbxTlv,const BoolUC &dot1xTlv,const BoolUC &dot3xTlv,const BoolUC &mgmtAddrTlv,const BoolUC &portDescrTlv,const BoolUC &sysCapTlv,const BoolUC &sysDescrTlv,const BoolUC &sysNameTlv)
        : ManagementInterfaceMessage (LldpGlobalObjectManager::getClassName (),LLDPGLOBALPROFILECONFSET),
        m_mgmtFlags    (mgmtFlags),
        m_cmdcode    (cmdcode),
        m_msgType    (msgType),
        m_profileName    (profileName),
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
		DcmMemUtil::memStatUpdate (DCM_MEM_TYPE_LLDP_GLOBALPROFILECONFSETMESSAGE,sizeof(LldpGlobalProfileConfSetMessage),DCMMEMSTATINCREMENT);
    	trace (TRACE_LEVEL_INFO,"ldpGlobalProfileConfSetMessage::Constructor:"                                                                                                                    "Entering.."); 
	}

    LldpGlobalProfileConfSetMessage::~LldpGlobalProfileConfSetMessage ()
    {
		DcmMemUtil::memStatUpdate (DCM_MEM_TYPE_LLDP_GLOBALPROFILECONFSETMESSAGE,sizeof(LldpGlobalProfileConfSetMessage),DCMMEMSTATDECREMENT);
		trace (TRACE_LEVEL_INFO,"ldpGlobalProfileConfSetMessage::Destructor:"                                                                                                                    "Leaving.."); 
    }

    void  LldpGlobalProfileConfSetMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_mgmtFlags,"mgmtFlags"));
        addSerializableAttribute (new AttributeUI32(&m_cmdcode,"cmdcode"));
        addSerializableAttribute (new AttributeUI32(&m_msgType,"msgType"));
        addSerializableAttribute (new AttributeString(&m_profileName,"profileName"));
        addSerializableAttribute (new AttributeStringUC(&m_description,false,"description"));
        addSerializableAttribute (new AttributeUI32UC(&m_hello,false,"hello"));
        addSerializableAttribute (new AttributeEnumUC(&m_mode,false,"mode"));
        addSerializableAttribute (new AttributeUI32UC(&m_multiplier,false,"multiplier"));
        addSerializableAttribute (new AttributeBoolUC(&m_dcbxFcoeAppTlv,false,"dcbxFcoeAppTlv"));
        addSerializableAttribute (new AttributeBoolUC(&m_dcbxFcoeLogicalLinkTlv,false,"dcbxFcoeLogicalLinkTlv"));
		addSerializableAttribute (new AttributeBoolUC(&m_dcbxIscsiAppTlv,false,"dcbxIscsiAppTlv"));
        addSerializableAttribute (new AttributeBoolUC(&m_dcbxTlv,false,"dcbxTlv"));
        addSerializableAttribute (new AttributeBoolUC(&m_dot1xTlv,false,"dot1xTlv"));
        addSerializableAttribute (new AttributeBoolUC(&m_dot3xTlv,false,"dot3xTlv"));
        addSerializableAttribute (new AttributeBoolUC(&m_mgmtAddrTlv,false,"mgmtAddrTlv"));
        addSerializableAttribute (new AttributeBoolUC(&m_portDescrTlv,false,"portDescrTlv"));
        addSerializableAttribute (new AttributeBoolUC(&m_sysCapTlv,false,"sysCapTlv"));
        addSerializableAttribute (new AttributeBoolUC(&m_sysDescrTlv,false,"sysDescrTlv"));
        addSerializableAttribute (new AttributeBoolUC(&m_sysNameTlv,false,"sysNameTlv"));
    }

    void  LldpGlobalProfileConfSetMessage::setMgmtFlags(const UI32 &mgmtFlags)
    {
        m_mgmtFlags  =  mgmtFlags;
    }

    UI32  LldpGlobalProfileConfSetMessage::getMgmtFlags() const
    {
        return (m_mgmtFlags);
    }

    void  LldpGlobalProfileConfSetMessage::setCmdcode(const UI32 &cmdcode)
    {
        m_cmdcode  =  cmdcode;
    }

    UI32  LldpGlobalProfileConfSetMessage::getCmdcode() const
    {
        return (m_cmdcode);
    }

    void  LldpGlobalProfileConfSetMessage::setMsgType(const UI32 &msgType)
    {
        m_msgType  =  msgType;
    }

    UI32  LldpGlobalProfileConfSetMessage::getMsgType() const
    {
        return (m_msgType);
    }

    void  LldpGlobalProfileConfSetMessage::setProfileName(const string &profileName)
    {
        m_profileName  =  profileName;
    }

    string  LldpGlobalProfileConfSetMessage::getProfileName() const
    {
        return (m_profileName);
    }

    void  LldpGlobalProfileConfSetMessage::setDescription(const StringUC &description)
    {
        m_description  =  description;
    }

    StringUC  LldpGlobalProfileConfSetMessage::getDescription() const
    {
        return (m_description);
    }

    void  LldpGlobalProfileConfSetMessage::setHello(const UI32UC &hello)
    {
        m_hello  =  hello;
    }

    UI32UC  LldpGlobalProfileConfSetMessage::getHello() const
    {
        return (m_hello);
    }

    void  LldpGlobalProfileConfSetMessage::setMode(const UI32UC &mode)
    {
        m_mode  =  mode;
    }

    UI32UC  LldpGlobalProfileConfSetMessage::getMode() const
    {
        return (m_mode);
    }

    void  LldpGlobalProfileConfSetMessage::setMultiplier(const UI32UC &multiplier)
    {
        m_multiplier  =  multiplier;
    }

    UI32UC  LldpGlobalProfileConfSetMessage::getMultiplier() const
    {
        return (m_multiplier);
    }

    void  LldpGlobalProfileConfSetMessage::setDcbxFcoeAppTlv(const BoolUC &dcbxFcoeAppTlv)
    {
        m_dcbxFcoeAppTlv  =  dcbxFcoeAppTlv;
    }

    BoolUC  LldpGlobalProfileConfSetMessage::getDcbxFcoeAppTlv() const
    {
        return (m_dcbxFcoeAppTlv);
    }

    void  LldpGlobalProfileConfSetMessage::setDcbxFcoeLogicalLinkTlv(const BoolUC &dcbxFcoeLogicalLinkTlv)
    {
        m_dcbxFcoeLogicalLinkTlv  =  dcbxFcoeLogicalLinkTlv;
    }

    BoolUC  LldpGlobalProfileConfSetMessage::getDcbxFcoeLogicalLinkTlv() const
    {
        return (m_dcbxFcoeLogicalLinkTlv);
    }

	void  LldpGlobalProfileConfSetMessage::setDcbxIscsiAppTlv(const BoolUC &dcbxIscsiAppTlv)
	{
		m_dcbxIscsiAppTlv  =  dcbxIscsiAppTlv;
	}

	BoolUC  LldpGlobalProfileConfSetMessage::getDcbxIscsiAppTlv() const
	{
		return (m_dcbxIscsiAppTlv);
	}

    void  LldpGlobalProfileConfSetMessage::setDcbxTlv(const BoolUC &dcbxTlv)
    {
        m_dcbxTlv  =  dcbxTlv;
    }

    BoolUC  LldpGlobalProfileConfSetMessage::getDcbxTlv() const
    {
        return (m_dcbxTlv);
    }

    void  LldpGlobalProfileConfSetMessage::setDot1xTlv(const BoolUC &dot1xTlv)
    {
        m_dot1xTlv  =  dot1xTlv;
    }

    BoolUC  LldpGlobalProfileConfSetMessage::getDot1xTlv() const
    {
        return (m_dot1xTlv);
    }

    void  LldpGlobalProfileConfSetMessage::setDot3xTlv(const BoolUC &dot3xTlv)
    {
        m_dot3xTlv  =  dot3xTlv;
    }

    BoolUC  LldpGlobalProfileConfSetMessage::getDot3xTlv() const
    {
        return (m_dot3xTlv);
    }

    void  LldpGlobalProfileConfSetMessage::setMgmtAddrTlv(const BoolUC &mgmtAddrTlv)
    {
        m_mgmtAddrTlv  =  mgmtAddrTlv;
    }

    BoolUC  LldpGlobalProfileConfSetMessage::getMgmtAddrTlv() const
    {
        return (m_mgmtAddrTlv);
    }

    void  LldpGlobalProfileConfSetMessage::setPortDescrTlv(const BoolUC &portDescrTlv)
    {
        m_portDescrTlv  =  portDescrTlv;
    }

    BoolUC  LldpGlobalProfileConfSetMessage::getPortDescrTlv() const
    {
        return (m_portDescrTlv);
    }

    void  LldpGlobalProfileConfSetMessage::setSysCapTlv(const BoolUC &sysCapTlv)
    {
        m_sysCapTlv  =  sysCapTlv;
    }

    BoolUC  LldpGlobalProfileConfSetMessage::getSysCapTlv() const
    {
        return (m_sysCapTlv);
    }

    void  LldpGlobalProfileConfSetMessage::setSysDescrTlv(const BoolUC &sysDescrTlv)
    {
        m_sysDescrTlv  =  sysDescrTlv;
    }

    BoolUC  LldpGlobalProfileConfSetMessage::getSysDescrTlv() const
    {
        return (m_sysDescrTlv);
    }

    void  LldpGlobalProfileConfSetMessage::setSysNameTlv(const BoolUC &sysNameTlv)
    {
        m_sysNameTlv  =  sysNameTlv;
    }

    BoolUC  LldpGlobalProfileConfSetMessage::getSysNameTlv() const
    {
        return (m_sysNameTlv);
    }
}
