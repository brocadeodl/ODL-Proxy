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
 *   Author : skbalasu.                                                     *
 **************************************************************************/

#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/NsmUpdatePoPrimaryLinkMessage.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"
#include "ClientInterface/Nsm/nsmdIpIntf.h"

namespace DcmNs
{

    NsmUpdatePoPrimaryLinkMessage::NsmUpdatePoPrimaryLinkMessage ()
        : ManagementInterfaceMessage (NsmLocalObjectManager::getClassName (),NSMUPDATEPOPRIMARYLINK)
    {
		m_cmdCode = 0;
		m_flag = true ;
		m_isAllowedVlanAll = false;
		m_mode = 0;
		m_mtu = NSM_MTU_DEF_VAL;
		m_nativeVlanId = 1;
		m_poIfIndex = 0;
		m_poMinLinks = 1;
		m_poAllowedSpeed = (LineSpeed)2;
		m_poMode = 0;
		m_poPartnerKey = 0;
		m_poPartnerSystemPriority = 0;
		m_poPrimaryLinkDomainId = 0;
		m_poType = 0;
		m_replyCode = 0;
		m_swMode = (swModeEnum)SW_MODE_ACCESS;
		m_swModeFlag = false;
		
    }
   NsmUpdatePoPrimaryLinkMessage::NsmUpdatePoPrimaryLinkMessage (const string &poId,const string &poName,const UI32 &poIfIndex,const UI32 &mtu,const string &desc,const bool &flag,const bool &swModeFlag,const UI32 &swMode,const bool &isAllowedVlanAll,const UI32 &poMode,const UI32 &poType,const UI32 &poMinLinks, const LineSpeed &poAllowedSpeed,const UI32 &poLoadBalanceMode, const UI32 &poPrimaryLinkDomainId, const UI32 &poPartnerSystemPriority, const UI32 &poPartnerKey, const string &poPartnerSystem,const string &poLacpSystemId, const UI32 &cmdCode)
        : ManagementInterfaceMessage (NsmLocalObjectManager::getClassName (), NSMUPDATEPOPRIMARYLINK),
        m_poId    (poId),
        m_poName    (poName),
        m_poIfIndex    (poIfIndex),
        m_mtu   (mtu),
        m_desc    (desc),
        m_flag    (flag),
        m_swModeFlag (swModeFlag),
        m_swMode    (swMode),
        m_isAllowedVlanAll    (isAllowedVlanAll),
        m_poMode    (poMode),
        m_poType    (poType),
        m_poMinLinks    (poMinLinks),
		m_poAllowedSpeed   (poAllowedSpeed),
        m_poLoadBalanceMode    (poLoadBalanceMode),
        m_poPrimaryLinkDomainId    (poPrimaryLinkDomainId),
        m_poPartnerSystemPriority    (poPartnerSystemPriority),
        m_poPartnerKey    (poPartnerKey),
        m_poPartnerSystem    (poPartnerSystem),
        m_poLacpSystemId    (poLacpSystemId),
        m_cmdCode    (cmdCode)
    {
		m_mode = 0;
		m_nativeVlanId = 1;
		m_replyCode = 0;
    }

    NsmUpdatePoPrimaryLinkMessage::~NsmUpdatePoPrimaryLinkMessage ()
    {
    }

    void  NsmUpdatePoPrimaryLinkMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeUI32(&m_replyCode,"replyCode"));
        addSerializableAttribute (new AttributeString(&m_desc,"desc"));
        addSerializableAttribute (new AttributeBool(&m_flag,"flag"));
        addSerializableAttribute (new AttributeBool(&m_isAllowedVlanAll,"isAllowedVlanAll"));
        addSerializableAttribute (new AttributeUI32(&m_mode,"mode"));
        addSerializableAttribute (new AttributeString(&m_poId,"poId"));
        addSerializableAttribute (new AttributeUI32(&m_poIfIndex,"poIfIndex"));
        addSerializableAttribute (new AttributeUI32(&m_mtu,"mtu"));
        addSerializableAttribute (new AttributeUI32(&m_poMode,"poMode"));
        addSerializableAttribute (new AttributeString(&m_poName,"poName"));
        addSerializableAttribute (new AttributeUI32(&m_poType,"poType"));
        addSerializableAttribute (new AttributeUI32(&m_poMinLinks,"poMinLinks"));
		addSerializableAttribute (new AttributeEnum((UI32*)(&m_poAllowedSpeed),"poAllowedSpeed"));
        addSerializableAttribute (new AttributeUI32(&m_poLoadBalanceMode,"poLoadBalanceMode"));
        addSerializableAttribute (new AttributeUI32(&m_poPrimaryLinkDomainId,"poPrimaryLinkDomainId"));
        addSerializableAttribute (new AttributeUI32(&m_poPartnerSystemPriority,"poPartnerSystemPriority"));
        addSerializableAttribute (new AttributeUI32(&m_poPartnerKey,"poPartnerKey"));
        addSerializableAttribute (new AttributeString(&m_poPartnerSystem,"poPartnerSystem"));
        addSerializableAttribute (new AttributeString(&m_poLacpSystemId,"poLacpSystemId"));
        addSerializableAttribute (new AttributeEnum((UI32*)(&m_swMode),"swMode"));
        addSerializableAttribute (new AttributeBool(&m_swModeFlag,"swModeFlag"));
        addSerializableAttribute (new AttributeUI32(&m_nativeVlanId,"nativeVlanId"));
    }

    void  NsmUpdatePoPrimaryLinkMessage::setPoId(const string &poId)
    {
        m_poId  =  poId;
    }

    string  NsmUpdatePoPrimaryLinkMessage::getPoId() const
    {
        return (m_poId);
    }

    void  NsmUpdatePoPrimaryLinkMessage::setPoName(const string &poName)
    {
        m_poName  =  poName;
    }

    string  NsmUpdatePoPrimaryLinkMessage::getPoName() const
    {
        return (m_poName);
    }

    void  NsmUpdatePoPrimaryLinkMessage::setPoIfIndex(const UI32 &poIfIndex)
    {
        m_poIfIndex  =  poIfIndex;
    }

    UI32  NsmUpdatePoPrimaryLinkMessage::getPoIfIndex() const
    {
        return (m_poIfIndex);
    }

    void  NsmUpdatePoPrimaryLinkMessage::setDesc(const string &desc)
    {
        m_desc  =  desc;
    }

    string  NsmUpdatePoPrimaryLinkMessage::getDesc() const
    {
        return (m_desc);
    }

    void  NsmUpdatePoPrimaryLinkMessage::setFlag(const bool &flag)
    {
        m_flag  =  flag;
    }

    bool  NsmUpdatePoPrimaryLinkMessage::getFlag() const
    {
        return (m_flag);
    }

    void  NsmUpdatePoPrimaryLinkMessage::setSwMode(const UI32 &swMode)
    {
        m_swMode  =  swMode;
    }

    UI32  NsmUpdatePoPrimaryLinkMessage::getSwMode() const
    {
        return (m_swMode);
    }

    void  NsmUpdatePoPrimaryLinkMessage::setMode(const UI32 &mode)
    {
        m_mode  =  mode;
    }

    UI32  NsmUpdatePoPrimaryLinkMessage::getMode() const
    {
        return (m_mode);
    }

    void  NsmUpdatePoPrimaryLinkMessage::setIsAllowedVlanAll(const bool &isAllowedVlanAll)
    {
        m_isAllowedVlanAll  =  isAllowedVlanAll;
    }

    bool  NsmUpdatePoPrimaryLinkMessage::getIsAllowedVlanAll() const
    {
        return (m_isAllowedVlanAll);
    }

    void  NsmUpdatePoPrimaryLinkMessage::setPoMode(const UI32 &poMode)
    {
        m_poMode  =  poMode;
    }

    UI32  NsmUpdatePoPrimaryLinkMessage::getPoMode() const
    {
        return (m_poMode);
    }

    void  NsmUpdatePoPrimaryLinkMessage::setPoType(const UI32 &poType)
    {
        m_poType  =  poType;
    }

    UI32  NsmUpdatePoPrimaryLinkMessage::getPoType() const
    {
        return (m_poType);
    }

    void  NsmUpdatePoPrimaryLinkMessage::setPoMinLinks(const UI32 &poMinLinks)
    {
        m_poMinLinks  =  poMinLinks;
    }

    UI32  NsmUpdatePoPrimaryLinkMessage::getPoMinLinks() const
    {
        return (m_poMinLinks);
    }

	void  NsmUpdatePoPrimaryLinkMessage::setPoAllowedSpeed(const LineSpeed &poAllowedSpeed)
	{
		m_poAllowedSpeed  =  poAllowedSpeed;
	}

	LineSpeed  NsmUpdatePoPrimaryLinkMessage::getPoAllowedSpeed() const
	{
		return (m_poAllowedSpeed);
	}

	void  NsmUpdatePoPrimaryLinkMessage::setPoLoadBalanceMode(const UI32 &poLoadBalanceMode)
    {
        m_poLoadBalanceMode  =  poLoadBalanceMode;
    }

    UI32  NsmUpdatePoPrimaryLinkMessage::getPoLoadBalanceMode() const
    {
        return (m_poLoadBalanceMode);
    }

    void  NsmUpdatePoPrimaryLinkMessage::setPoPrimaryLinkDomainId(const UI32 &poPrimaryLinkDomainId)
    {
        m_poPrimaryLinkDomainId  =  poPrimaryLinkDomainId;
    }

    UI32  NsmUpdatePoPrimaryLinkMessage::getPoPrimaryLinkDomainId() const
    {
        return (m_poPrimaryLinkDomainId);
    }

    void  NsmUpdatePoPrimaryLinkMessage::setPoPartnerSystemPriority(const UI32 &poPartnerSystemPriority)
    {
        m_poPartnerSystemPriority  =  poPartnerSystemPriority;
    }

    UI32  NsmUpdatePoPrimaryLinkMessage::getPoPartnerSystemPriority() const
    {
        return (m_poPartnerSystemPriority);
    }

    void  NsmUpdatePoPrimaryLinkMessage::setPoPartnerKey(const UI32 &poPartnerKey)
    {
        m_poPartnerKey  =  poPartnerKey;
    }

    UI32  NsmUpdatePoPrimaryLinkMessage::getPoPartnerKey() const
    {
        return (m_poPartnerKey);
    }

    void  NsmUpdatePoPrimaryLinkMessage::setPoPartnerSystem(const string &poPartnerSystem)
    {
        m_poPartnerSystem  =  poPartnerSystem;
    }

    string  NsmUpdatePoPrimaryLinkMessage::getPoPartnerSystem() const
    {
        return (m_poPartnerSystem);
    }

    void  NsmUpdatePoPrimaryLinkMessage::setPoLacpSystemId(const string &poLacpSystemId)
    {
        m_poLacpSystemId  =  poLacpSystemId;
    }

    string  NsmUpdatePoPrimaryLinkMessage::getPoLacpSystemId() const
    {
        return (m_poLacpSystemId);
    }

    void  NsmUpdatePoPrimaryLinkMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  NsmUpdatePoPrimaryLinkMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  NsmUpdatePoPrimaryLinkMessage::setSwModeFlag(const bool &swModeFlag)
    {
        m_swModeFlag  =  swModeFlag;
    }

    bool  NsmUpdatePoPrimaryLinkMessage::getSwModeFlag() const
    {
        return (m_swModeFlag);
    }

    void  NsmUpdatePoPrimaryLinkMessage::setMtu(const UI32 &mtu)
    {
        m_mtu  =  mtu;
    }

    UI32  NsmUpdatePoPrimaryLinkMessage::getMtu() const
    {
        return (m_mtu);
    }

    void  NsmUpdatePoPrimaryLinkMessage::setNativeVlanId(const UI32 &nativeVlanId)
    {
        m_nativeVlanId =  nativeVlanId;
    }

    UI32 NsmUpdatePoPrimaryLinkMessage::getNativeVlanId() const
    {
        return (m_nativeVlanId);
    }
    
    void  NsmUpdatePoPrimaryLinkMessage::setReplyCode(const UI32 &replyCode)
    {
        m_replyCode  =  replyCode;
    }

    UI32  NsmUpdatePoPrimaryLinkMessage::getReplyCode() const
    {
        return (m_replyCode);
    }
}
