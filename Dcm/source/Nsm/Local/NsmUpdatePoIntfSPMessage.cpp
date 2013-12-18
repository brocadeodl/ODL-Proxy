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
#include "Nsm/Local/NsmUpdatePoIntfSPMessage.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "ClientInterface/Nsm/nsmdIpIntf.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"

namespace DcmNs
{

    NsmUpdatePoIntfSPMessage::NsmUpdatePoIntfSPMessage ()
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId (),NSMUPDATEPOINTFSP)
    {
		m_cmdCode = 0;
		m_flag = false;
		m_isAllowedVlanAll = true;
		m_mode = 0;
		m_mtu = NSM_MTU_DEF_VAL;
		m_nativeVlanId = 1;
		m_poIfIndex = 0;
		m_poMinLinks = 1;
		m_poAllowedSpeed = (LineSpeed)2;
		m_poMode = 0;
		m_poPartnerSystemPriority = 0;
		m_poPartnerKey = 0;
		m_poPrimaryLinkDomainId = 0;
		m_poType = 0;
		m_replyCode = 0;
		m_swModeFlag = false;
		m_swMode = 0;
		m_isPortProfiled = false;
        m_portProfileName = "";
        m_portProfileDomainName = "";
        m_ignore_split = true;
		m_vlanidList.clear();
		m_eldallowedvlanall = 0;
		m_pVlanNativeVlanId = 0;
		m_pVlanIsAllowedVlanAll = false;
		m_pVlanHostPrimVlanId = 0;
		m_pVlanHostSecVlanId = 0;
		m_raGuard = false;
    }
   NsmUpdatePoIntfSPMessage::NsmUpdatePoIntfSPMessage (const string &poId,const string &poName,const UI32 &poIfIndex,const UI32 &mtu,const string &desc,const bool &flag,const bool &swModeFlag,const UI32 &swMode,const bool &isAllowedVlanAll,const UI32 &poMode,const UI32 &poType,const UI32 &poMinLinks,const LineSpeed &poAllowedSpeed,const UI32 &poLoadBalanceMode, const UI32 &poPrimaryLinkDomainId, const UI32 &poPartnerSystemPriority, const UI32 &poPartnerKey, const string &poPartnerSystem,const string &poLacpSystemId, const UI32 &cmdCode, const bool &isPortProfiled, const UI32 &pVlanNativeVlanId, const bool &pVlanIsAllowedVlanAll)
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId (),NSMUPDATEPOINTFSP),
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
        m_poMinLinks   (poMinLinks),
		m_poAllowedSpeed  (poAllowedSpeed),
        m_poLoadBalanceMode   (poLoadBalanceMode),
        m_poPrimaryLinkDomainId    (poPrimaryLinkDomainId),
        m_poPartnerSystemPriority    (poPartnerSystemPriority),
        m_poPartnerKey    (poPartnerKey),
        m_poPartnerSystem    (poPartnerSystem),
        m_poLacpSystemId    (poLacpSystemId),
        m_cmdCode    (cmdCode),
		m_pVlanNativeVlanId	(pVlanNativeVlanId),
		m_pVlanIsAllowedVlanAll (pVlanIsAllowedVlanAll),
		m_isPortProfiled    (isPortProfiled)
    {
		m_mode = 0 ;
		m_nativeVlanId = 1;
		m_replyCode = 0;
        m_ignore_split = true;
		m_eldallowedvlanall = 0;
		m_raGuard = false;
        m_portProfileName = "";
        m_portProfileDomainName = "";
    }

    NsmUpdatePoIntfSPMessage::~NsmUpdatePoIntfSPMessage ()
    {
    }

    void  NsmUpdatePoIntfSPMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeUI32(&m_replyCode,"replyCode"));
        addSerializableAttribute (new AttributeString(&m_desc,"desc"));
        addSerializableAttribute (new AttributeBool(&m_flag,"flag"));
        addSerializableAttribute (new AttributeBool(&m_isAllowedVlanAll,"isAllowedVlanAll"));
        addSerializableAttribute (new AttributeUI32Range(&m_vlanRange,"vlanRange"));
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
        addSerializableAttribute (new AttributeUI32(&m_pVlanNativeVlanId,"pVlanNativeVlanId"));
        addSerializableAttribute (new AttributeBool(&m_pVlanIsAllowedVlanAll,"pVlanIsAllowedVlanAll"));
        addSerializableAttribute (new AttributeUI32Range(&m_pVlanRange,"pVlanRange"));
        addSerializableAttribute (new AttributeUI32(&m_pVlanHostPrimVlanId,"pVlanHostPrimVlanId"));
        addSerializableAttribute (new AttributeUI32(&m_pVlanHostSecVlanId,"pVlanHostSecVlanId"));
        addSerializableAttribute (new AttributeBool(&m_nativevlanDisable,
														"nativevlanDisable"));
        addSerializableAttribute (new AttributeUI32(&m_nativeVlanId,"nativeVlanId"));
		addSerializableAttribute (new AttributeBool(&m_isPortProfiled,"isPortProfiled"));
        addSerializableAttribute (new AttributeString(&m_portProfileName, "portProfileName"));
        addSerializableAttribute (new AttributeString(&m_portProfileDomainName, "portProfileDomainName"));
        addSerializableAttribute (new AttributeUI32(&m_prio,"prio"));
		addSerializableAttribute (new AttributeUI32(&m_eldallowedvlanall,"eldallowedvlanall"));
        addSerializableAttribute (new AttributeUI32Vector(&m_vlanidList,"vlanidList"));
        addSerializableAttribute (new AttributeBool(&m_ignore_split,"ignore_split"));
        addSerializableAttribute (new AttributeBool(&m_raGuard, "raGuard"));
        addSerializableAttribute (new AttributeUI32(&m_ctagId, "ctagId"));
        addSerializableAttribute (new AttributeString(&m_mac, "mac"));
        addSerializableAttribute (new AttributeUI32(&m_macGroupId, "macGroupId"));
    }

	void  NsmUpdatePoIntfSPMessage::setEldAllowedVlanAll(const UI32 &eldallowedvlanall)
	{
		m_eldallowedvlanall  =  eldallowedvlanall;
	}

	UI32  NsmUpdatePoIntfSPMessage::getEldAllowedVlanAll() const
	{
		return (m_eldallowedvlanall);
	}

    void  NsmUpdatePoIntfSPMessage::setPrio(const UI32 &prio)
    {
        m_prio  =  prio;
    }

    UI32  NsmUpdatePoIntfSPMessage::getPrio() const
    {
        return (m_prio);
	}

	void  NsmUpdatePoIntfSPMessage::setVlanidList(const vector<UI32> &vlanidList)
	{
		m_vlanidList  =  vlanidList;
	} 
	
	vector<UI32>  NsmUpdatePoIntfSPMessage::getVlanidList() const
	{
		return (m_vlanidList);
    }

    void  NsmUpdatePoIntfSPMessage::setPoId(const string &poId)
    {
        m_poId  =  poId;
    }

    string  NsmUpdatePoIntfSPMessage::getPoId() const
    {
        return (m_poId);
    }

    void  NsmUpdatePoIntfSPMessage::setPoName(const string &poName)
    {
        m_poName  =  poName;
    }

    string  NsmUpdatePoIntfSPMessage::getPoName() const
    {
        return (m_poName);
    }

    void  NsmUpdatePoIntfSPMessage::setPoIfIndex(const UI32 &poIfIndex)
    {
        m_poIfIndex  =  poIfIndex;
    }

    UI32  NsmUpdatePoIntfSPMessage::getPoIfIndex() const
    {
        return (m_poIfIndex);
    }

    void  NsmUpdatePoIntfSPMessage::setDesc(const string &desc)
    {
        m_desc  =  desc;
    }

    string  NsmUpdatePoIntfSPMessage::getDesc() const
    {
        return (m_desc);
    }

    void  NsmUpdatePoIntfSPMessage::setFlag(const bool &flag)
    {
        m_flag  =  flag;
    }

    bool  NsmUpdatePoIntfSPMessage::getFlag() const
    {
        return (m_flag);
    }

    void  NsmUpdatePoIntfSPMessage::setIgnoreSplit(const bool &flag)
    {
        m_ignore_split  =  flag;
    }

    bool  NsmUpdatePoIntfSPMessage::getIgnoreSplit() const
    {
        return (m_ignore_split);
    }

    void  NsmUpdatePoIntfSPMessage::setSwMode(const UI32 &swMode)
    {
        m_swMode  =  swMode;
    }

    UI32  NsmUpdatePoIntfSPMessage::getSwMode() const
    {
        return (m_swMode);
    }

    void  NsmUpdatePoIntfSPMessage::setMode(const UI32 &mode)
    {
        m_mode  =  mode;
    }

    UI32  NsmUpdatePoIntfSPMessage::getMode() const
    {
        return (m_mode);
    }

    void  NsmUpdatePoIntfSPMessage::setIsAllowedVlanAll(const bool &isAllowedVlanAll)
    {
        m_isAllowedVlanAll  =  isAllowedVlanAll;
    }

    bool  NsmUpdatePoIntfSPMessage::getIsAllowedVlanAll() const
    {
        return (m_isAllowedVlanAll);
    }

    void  NsmUpdatePoIntfSPMessage::setPoMode(const UI32 &poMode)
    {
        m_poMode  =  poMode;
    }

    UI32  NsmUpdatePoIntfSPMessage::getPoMode() const
    {
        return (m_poMode);
    }

    void  NsmUpdatePoIntfSPMessage::setPoType(const UI32 &poType)
    {
        m_poType  =  poType;
    }

    UI32  NsmUpdatePoIntfSPMessage::getPoType() const
    {
        return (m_poType);
    }

    void  NsmUpdatePoIntfSPMessage::setPoMinLinks(const UI32 &poMinLinks)
    {
        m_poMinLinks  =  poMinLinks;
    }

    UI32  NsmUpdatePoIntfSPMessage::getPoMinLinks() const
    {
        return (m_poMinLinks);
    }

	void  NsmUpdatePoIntfSPMessage::setPoAllowedSpeed(const LineSpeed &poAllowedSpeed)
	{
		m_poAllowedSpeed  =  poAllowedSpeed;
	}

	LineSpeed  NsmUpdatePoIntfSPMessage::getPoAllowedSpeed() const
	{
		return (m_poAllowedSpeed);
	}

	void  NsmUpdatePoIntfSPMessage::setPoLoadBalanceMode(const UI32 &poLoadBalanceMode)
    {
        m_poLoadBalanceMode  =  poLoadBalanceMode;
    }

    UI32  NsmUpdatePoIntfSPMessage::getPoLoadBalanceMode() const
    {
        return (m_poLoadBalanceMode);
    }

    void  NsmUpdatePoIntfSPMessage::setPoPrimaryLinkDomainId(const UI32 &poPrimaryLinkDomainId)
    {
        m_poPrimaryLinkDomainId  =  poPrimaryLinkDomainId;
    }

    UI32  NsmUpdatePoIntfSPMessage::getPoPrimaryLinkDomainId() const
    {
        return (m_poPrimaryLinkDomainId);
    }

    void  NsmUpdatePoIntfSPMessage::setPoPartnerSystemPriority(const UI32 &poPartnerSystemPriority)
    {
        m_poPartnerSystemPriority  =  poPartnerSystemPriority;
    }

    UI32  NsmUpdatePoIntfSPMessage::getPoPartnerSystemPriority() const
    {
        return (m_poPartnerSystemPriority);
    }

    void  NsmUpdatePoIntfSPMessage::setPoPartnerKey(const UI32 &poPartnerKey)
    {
        m_poPartnerKey  =  poPartnerKey;
    }

    UI32  NsmUpdatePoIntfSPMessage::getPoPartnerKey() const
    {
        return (m_poPartnerKey);
    }

    void  NsmUpdatePoIntfSPMessage::setPoPartnerSystem(const string &poPartnerSystem)
    {
        m_poPartnerSystem  =  poPartnerSystem;
    }

    string  NsmUpdatePoIntfSPMessage::getPoPartnerSystem() const
    {
        return (m_poPartnerSystem);
    }

    void  NsmUpdatePoIntfSPMessage::setPoLacpSystemId(const string &poLacpSystemId)
    {
        m_poLacpSystemId  =  poLacpSystemId;
    }

    string  NsmUpdatePoIntfSPMessage::getPoLacpSystemId() const
    {
        return (m_poLacpSystemId);
    }

    void  NsmUpdatePoIntfSPMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  NsmUpdatePoIntfSPMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  NsmUpdatePoIntfSPMessage::setSwModeFlag(const bool &swModeFlag)
    {
        m_swModeFlag  =  swModeFlag;
    }

    bool  NsmUpdatePoIntfSPMessage::getSwModeFlag() const
    {
        return (m_swModeFlag);
    }

	void  NsmUpdatePoIntfSPMessage::setPvlanNativeVlanId(const UI32 &pVlanNativeVlanId)
    {
        m_pVlanNativeVlanId = pVlanNativeVlanId;
    }
    UI32  NsmUpdatePoIntfSPMessage::getPvlanNativeVlanId() const
    {
        return (m_pVlanNativeVlanId);
    }
	
	void NsmUpdatePoIntfSPMessage::setPvlanIsAllowedVlanAll(const bool &pVlanIsAllowedVlanAll)
	{
		m_pVlanIsAllowedVlanAll = pVlanIsAllowedVlanAll;
	}

	bool NsmUpdatePoIntfSPMessage::getPvlanIsAllowedVlanAll() const
	{
		return (m_pVlanIsAllowedVlanAll);
	}
	
	void  NsmUpdatePoIntfSPMessage::setPvlanRange (const UI32Range &pVlanRange)
    {
        m_pVlanRange = pVlanRange;
    }

    UI32Range NsmUpdatePoIntfSPMessage::getPvlanRange () const
    {
        return (m_pVlanRange);
    }

	void  NsmUpdatePoIntfSPMessage::setPvlanHostPrimVlanId(const UI32 &pVlanHostPrimVlanId)
    {
        m_pVlanHostPrimVlanId = pVlanHostPrimVlanId;
    }

   	UI32 NsmUpdatePoIntfSPMessage::getPvlanHostPrimVlanId() const
    {
        return (m_pVlanHostPrimVlanId);
    }

	void  NsmUpdatePoIntfSPMessage::setPvlanHostSecVlanId(const UI32 &pVlanHostSecVlanId)
    {
		m_pVlanHostSecVlanId = pVlanHostSecVlanId;
    }

   	UI32 NsmUpdatePoIntfSPMessage::getPvlanHostSecVlanId() const
    {
        return (m_pVlanHostSecVlanId);
    }
    void  NsmUpdatePoIntfSPMessage::setMtu(const UI32 &mtu)
    {
        m_mtu  =  mtu;
    }

    UI32  NsmUpdatePoIntfSPMessage::getMtu() const
    {
        return (m_mtu);
    }

    void  NsmUpdatePoIntfSPMessage::setNativeVlanId(const UI32 &nativeVlanId)
    {
        m_nativeVlanId =  nativeVlanId;
    }

    UI32 NsmUpdatePoIntfSPMessage::getNativeVlanId() const
    {
        return (m_nativeVlanId);
    }
    
	void  NsmUpdatePoIntfSPMessage::setNativevlanDisable(const bool 
		&nativevlanDisable)
    {
        m_nativevlanDisable  =  nativevlanDisable;
    }

    bool  NsmUpdatePoIntfSPMessage::getNativevlanDisable() const
    {
        return (m_nativevlanDisable);
    }
    
    void  NsmUpdatePoIntfSPMessage::setVlanRange(const UI32Range &vlanRange)
    {
        m_vlanRange  =  vlanRange;
    }

    UI32Range NsmUpdatePoIntfSPMessage::getVlanRange() const
    {
        return (m_vlanRange);
    }

    void  NsmUpdatePoIntfSPMessage::setReplyCode(const UI32 &replyCode)
    {
        m_replyCode  =  replyCode;
    }

    UI32  NsmUpdatePoIntfSPMessage::getReplyCode() const
    {
        return (m_replyCode);
    }

	void NsmUpdatePoIntfSPMessage::setIsPortProfiled(const bool &isPortProfiled)
	{
		m_isPortProfiled = isPortProfiled;
	}

	bool NsmUpdatePoIntfSPMessage::getIsPortProfiled() const
	{
		return m_isPortProfiled;
	}

    void NsmUpdatePoIntfSPMessage::setPortProfileName(const string &portProfileName) {
        m_portProfileName = portProfileName;
    }

    string NsmUpdatePoIntfSPMessage::getPortProfileName() const {
        return m_portProfileName;
    }
    void NsmUpdatePoIntfSPMessage::setPortProfileDomainName(const string &portProfileDomainName) {
        m_portProfileDomainName = portProfileDomainName;
    }

    string NsmUpdatePoIntfSPMessage::getPortProfileDomainName() const {
        return m_portProfileDomainName;
    }
            
    void NsmUpdatePoIntfSPMessage::setRaGuard(const bool &raGuard)
    {
        m_raGuard = raGuard;
    }

    bool NsmUpdatePoIntfSPMessage::getRaGuard() const
    {
        return m_raGuard;
    }

    void NsmUpdatePoIntfSPMessage::setCtagId(const UI32 &ctagId)
    {
        m_ctagId = ctagId;
    }

    UI32 NsmUpdatePoIntfSPMessage::getCtagId() const
    {
        return m_ctagId;
    }

    void NsmUpdatePoIntfSPMessage::setMac(const string &mac)
    {
        m_mac = mac;
    }

    string NsmUpdatePoIntfSPMessage::getMac() const
    {
        return m_mac;
    }

    void NsmUpdatePoIntfSPMessage::setMacGroupId(const UI32 &macGroupId)
    {
        m_macGroupId = macGroupId;
    }

    UI32 NsmUpdatePoIntfSPMessage::getMacGroupId() const
    {
        return m_macGroupId;
    }

}
