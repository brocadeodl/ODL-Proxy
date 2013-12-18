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

#include "Nsm/Global/NsmGlobalUpdatePoIntfMessage.h"
#include "Nsm/Global/PoIntfManagedObject.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/NsmGlobalTypes.h"
#include "ClientInterface/Nsm/nsmdIpIntf.h"

namespace DcmNs
{

    NsmGlobalUpdatePoIntfMessage::NsmGlobalUpdatePoIntfMessage ()
        : ManagementInterfaceMessage (NsmGlobalObjectManager::getClassName (),NSMGLOBALUPDATEPOINTF)
    {
		m_cmdCode = 0;
		m_flag = false;
        m_isAllowedVlanAll = true;
        m_mtu = NSM_MTU_DEF_VAL;
        m_nativeVlanId = 1;
        m_poIfIndex = 0;
		m_poLacpSystemIdDomainId = 0;
        m_poMinLinks = 1;
		m_poAllowedSpeed = (LineSpeed)2;
        m_poMode = 0;
        m_poPartnerSystemPriority = 0;
        m_poPartnerKey = 0;
        m_poPrimaryLinkDomainId = 0;
        m_poType = 0;
        m_replyCode = 0;
        m_swModeFlag = false;
        m_ignore_split = true;
        m_swMode = SW_MODE_UNKNOWN;
        m_isPortProfiled = false;
        m_portProfileName = "";
        m_portProfileDomainName = "";
		m_eld_prio_val = 128;
		m_vlanid.clear();
		m_vlanidList.clear();	
		m_vlanIdRange = UI32Range("");
		m_raGuard = false;
		m_pVlanNativeVlanId = 0;
		m_pVlanIsAllowedVlanAll = false;
		m_pVlanHostPrimVlanId = 0;
		m_pVlanHostSecVlanId = 0;
    }
    NsmGlobalUpdatePoIntfMessage::NsmGlobalUpdatePoIntfMessage (const string &poId,const string &poName,const UI32 &poIfIndex,const string &desc,const bool &flag,const bool &swModeFlag,const swModeEnum &swMode,const bool &isAllowedVlanAll,const UI32 &poMode,const UI32 &poType,const UI32 &poMinLinks,const LineSpeed &poAllowedSpeed, const UI32 &poLoadBalanceMode, const UI32 &cmdCode, const bool &isPortProfiled,const UI32 &pVlanNativeVlanId, const bool &pVlanIsAllowedVlanAll)
        : ManagementInterfaceMessage (NsmGlobalObjectManager::getClassName (),NSMGLOBALUPDATEPOINTF),
        m_poId    (poId),
        m_poName    (poName),
        m_poIfIndex    (poIfIndex),
        m_desc    (desc),
        m_flag    (flag),
        m_swModeFlag    (swModeFlag),
        m_swMode    (swMode),
        m_isAllowedVlanAll    (isAllowedVlanAll),
        m_poMode    (poMode),
        m_poType    (poType),
        m_poMinLinks    (poMinLinks),
		m_poAllowedSpeed    (poAllowedSpeed),
        m_poLoadBalanceMode    (poLoadBalanceMode),
        m_cmdCode    (cmdCode),
		m_pVlanNativeVlanId		(pVlanNativeVlanId),
		m_pVlanIsAllowedVlanAll  (pVlanIsAllowedVlanAll),
		m_isPortProfiled    (isPortProfiled)
    {
		m_mtu = NSM_MTU_DEF_VAL;
		m_nativeVlanId = 1;
		m_poLacpSystemIdDomainId = 0;
		m_poPartnerKey = 0;
		m_poPartnerSystemPriority = 0;
		m_poPrimaryLinkDomainId = 0;
		m_replyCode = 0;
        m_ignore_split = true;
		m_raGuard = false;
        m_portProfileName = "";
        m_portProfileDomainName = "";
    }

    NsmGlobalUpdatePoIntfMessage::~NsmGlobalUpdatePoIntfMessage ()
    {
    }

    void  NsmGlobalUpdatePoIntfMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeUI32(&m_replyCode,"replyCode"));
        addSerializableAttribute (new AttributeString(&m_desc,"desc"));
        addSerializableAttribute (new AttributeBool(&m_flag,"flag"));
        addSerializableAttribute (new AttributeBool(&m_isAllowedVlanAll,"isAllowedVlanAll"));
        addSerializableAttribute (new AttributeString(&m_poId,"poId"));
        addSerializableAttribute (new AttributeUI32(&m_poIfIndex,"poIfIndex"));
        addSerializableAttribute (new AttributeUI32(&m_mtu,"mtu"));
        addSerializableAttribute (new AttributeUI32(&m_poMode,"poMode"));
        addSerializableAttribute (new AttributeString(&m_poName,"poName"));
        addSerializableAttribute (new AttributeUI32(&m_poType,"poType"));
        addSerializableAttribute (new AttributeUI32(&m_poMinLinks,"poMinLinks"));
        addSerializableAttribute (new AttributeEnum((UI32 *)(&m_poAllowedSpeed),"poAllowedSpeed"));
        addSerializableAttribute (new AttributeUI32(&m_poLoadBalanceMode,"poLoadBalanceMode"));
        addSerializableAttribute (new AttributeUI32(&m_poPrimaryLinkDomainId,"poPrimaryLinkDomainId"));
        addSerializableAttribute (new AttributeUI32(&m_poPartnerSystemPriority,"poPartnerSystemPriority"));
        addSerializableAttribute (new AttributeUI32(&m_poPartnerKey,"poPartnerKey"));
        addSerializableAttribute (new AttributeString(&m_poPartnerSystem,"poPartnerSystem"));
        addSerializableAttribute (new AttributeString(&m_poLacpSystemId,"poLacpSystemId"));
        addSerializableAttribute (new AttributeUI32(&m_poLacpSystemIdDomainId,"poLacpSystemIdDomainId"));
        addSerializableAttribute (new AttributeEnum((UI32*)(&m_swMode),"swMode"));
        addSerializableAttribute (new AttributeBool(&m_swModeFlag,"swModeFlag"));
        addSerializableAttribute (new AttributeUI32Range(&m_vlanRange,"vlanRange"));
        addSerializableAttribute (new AttributeUI32(&m_pVlanNativeVlanId,"pVlanNativeVlan"));
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
        addSerializableAttribute (new AttributeBool(&m_ignore_split, "ignore_split"));
        addSerializableAttribute (new AttributeUI32(&m_eld_prio_val,"eldprio"));
        addSerializableAttribute (new AttributeObjectIdVector(&m_vlanid,"vlanid"));
        addSerializableAttribute (new AttributeUI32Vector(&m_vlanidList,"vlanidList"));
        addSerializableAttribute (new AttributeUI32Range(&m_vlanIdRange,"vlanIdRange"));
        addSerializableAttribute (new AttributeBool(&m_raGuard, "raGuard"));
    }

    /* ELD */
    void  NsmGlobalUpdatePoIntfMessage::setEldPrioVal(const UI32 &eldPrio)
    {
        m_eld_prio_val = eldPrio;
    }

    UI32  NsmGlobalUpdatePoIntfMessage::getEldPrioVal() const
    {
        return (m_eld_prio_val);
    }

    void  NsmGlobalUpdatePoIntfMessage::setVlanid(const vector<ObjectId> &vlanid)
    {
        m_vlanid  =  vlanid;
    }

    vector<ObjectId>  NsmGlobalUpdatePoIntfMessage::getVlanid() const
    {
        return (m_vlanid);
    }
    void  NsmGlobalUpdatePoIntfMessage::setVlanIdRange(const UI32Range &vlanRange)
    {
        m_vlanIdRange  =  vlanRange;
    }

    UI32Range NsmGlobalUpdatePoIntfMessage::getVlanIdRange() const
    {
        return (m_vlanIdRange);
    }

    void  NsmGlobalUpdatePoIntfMessage::setVlanidList(const vector<UI32> &vlanidList)
    {
        m_vlanidList  =  vlanidList;
    }

    vector<UI32>  NsmGlobalUpdatePoIntfMessage::getVlanidList() const
    {
        return (m_vlanidList);
    }
    /* ELD */
    void  NsmGlobalUpdatePoIntfMessage::setPoId(const string &poId)
    {
        m_poId  =  poId;
    }

    string  NsmGlobalUpdatePoIntfMessage::getPoId() const
    {
        return (m_poId);
    }

    void  NsmGlobalUpdatePoIntfMessage::setPoName(const string &poName)
    {
        m_poName  =  poName;
    }

    string  NsmGlobalUpdatePoIntfMessage::getPoName() const
    {
        return (m_poName);
    }

    void  NsmGlobalUpdatePoIntfMessage::setPoIfIndex(const UI32 &poIfIndex)
    {
        m_poIfIndex  =  poIfIndex;
    }

    UI32  NsmGlobalUpdatePoIntfMessage::getPoIfIndex() const
    {
        return (m_poIfIndex);
    }

    void  NsmGlobalUpdatePoIntfMessage::setDesc(const string &desc)
    {
        m_desc  =  desc;
    }

    string  NsmGlobalUpdatePoIntfMessage::getDesc() const
    {
        return (m_desc);
    }

    void  NsmGlobalUpdatePoIntfMessage::setFlag(const bool &flag)
    {
        m_flag  =  flag;
    }

    bool  NsmGlobalUpdatePoIntfMessage::getFlag() const
    {
        return (m_flag);
    }

    void  NsmGlobalUpdatePoIntfMessage::setIgnoreSplit(const bool &flag)
    {
        m_ignore_split  =  flag;
    }

    bool  NsmGlobalUpdatePoIntfMessage::getIgnoreSplit() const
    {
        return (m_ignore_split);
    }

    void  NsmGlobalUpdatePoIntfMessage::setSwModeFlag(const bool &swModeFlag)
    {
        m_swModeFlag  =  swModeFlag;
    }

    bool  NsmGlobalUpdatePoIntfMessage::getSwModeFlag() const
    {
        return (m_swModeFlag);
    }

    void  NsmGlobalUpdatePoIntfMessage::setSwMode(const swModeEnum &swMode)
    {
        m_swMode  =  swMode;
    }

    swModeEnum  NsmGlobalUpdatePoIntfMessage::getSwMode() const
    {
        return (m_swMode);
    }

    void  NsmGlobalUpdatePoIntfMessage::setIsAllowedVlanAll(const bool &isAllowedVlanAll)
    {
        m_isAllowedVlanAll  =  isAllowedVlanAll;
    }

    bool  NsmGlobalUpdatePoIntfMessage::getIsAllowedVlanAll() const
    {
        return (m_isAllowedVlanAll);
    }

    void  NsmGlobalUpdatePoIntfMessage::setPoMode(const UI32 &poMode)
    {
        m_poMode  =  poMode;
    }

    UI32  NsmGlobalUpdatePoIntfMessage::getPoMode() const
    {
        return (m_poMode);
    }

    void  NsmGlobalUpdatePoIntfMessage::setPoType(const UI32 &poType)
    {
        m_poType  =  poType;
    }

    UI32  NsmGlobalUpdatePoIntfMessage::getPoType() const
    {
        return (m_poType);
    }

    void  NsmGlobalUpdatePoIntfMessage::setPoMinLinks(const UI32 &poMinLinks)
    {
        m_poMinLinks  =  poMinLinks;
    }

    UI32  NsmGlobalUpdatePoIntfMessage::getPoMinLinks() const
    {
        return (m_poMinLinks);
    }

	void  NsmGlobalUpdatePoIntfMessage::setPoAllowedSpeed(const LineSpeed &poAllowedSpeed)
	{
		m_poAllowedSpeed  =  poAllowedSpeed;
	}

	LineSpeed  NsmGlobalUpdatePoIntfMessage::getPoAllowedSpeed() const
	{
		return (m_poAllowedSpeed);
	}

    void  NsmGlobalUpdatePoIntfMessage::setPoLoadBalanceMode(const UI32 &poLoadBalanceMode)
    {
        m_poLoadBalanceMode =  poLoadBalanceMode;
    }

    UI32  NsmGlobalUpdatePoIntfMessage::getPoLoadBalanceMode() const
    {
        return (m_poLoadBalanceMode);
    }


    void  NsmGlobalUpdatePoIntfMessage::setPoPrimaryLinkDomainId(const UI32 &poPrimaryLinkDomainId)
    {
        m_poPrimaryLinkDomainId  =  poPrimaryLinkDomainId;
    }

    UI32  NsmGlobalUpdatePoIntfMessage::getPoPrimaryLinkDomainId() const
    {
        return (m_poPrimaryLinkDomainId);
    }

    void  NsmGlobalUpdatePoIntfMessage::setPoPartnerSystemPriority(const UI32 &poPartnerSystemPriority)
    {
        m_poPartnerSystemPriority  =  poPartnerSystemPriority;
    }

    UI32  NsmGlobalUpdatePoIntfMessage::getPoPartnerSystemPriority() const
    {
        return (m_poPartnerSystemPriority);
    }

    void  NsmGlobalUpdatePoIntfMessage::setPoPartnerKey(const UI32 &poPartnerKey)
    {
        m_poPartnerKey  =  poPartnerKey;
    }

    UI32  NsmGlobalUpdatePoIntfMessage::getPoPartnerKey() const
    {
        return (m_poPartnerKey);
    }

    void  NsmGlobalUpdatePoIntfMessage::setPoPartnerSystem(const string &poPartnerSystem)
    {
        m_poPartnerSystem  =  poPartnerSystem;
    }

    string  NsmGlobalUpdatePoIntfMessage::getPoPartnerSystem() const
    {
        return (m_poPartnerSystem);
    }

    void  NsmGlobalUpdatePoIntfMessage::setPoLacpSystemId(const string &poLacpSystemId)
    {
        m_poLacpSystemId  =  poLacpSystemId;
    }

    string  NsmGlobalUpdatePoIntfMessage::getPoLacpSystemId() const
    {
        return (m_poLacpSystemId);
    }

    void  NsmGlobalUpdatePoIntfMessage::setPoLacpSystemIdDomainId(const UI32 &poLacpSystemIdDomainId)
    {
        m_poLacpSystemIdDomainId  =  poLacpSystemIdDomainId;
    }

    UI32  NsmGlobalUpdatePoIntfMessage::getPoLacpSystemIdDomainId() const
    {
        return (m_poLacpSystemIdDomainId);
    }

    void  NsmGlobalUpdatePoIntfMessage::setReplyCode(const UI32 &replyCode)
    {
        m_replyCode  =  replyCode;
    }

    UI32  NsmGlobalUpdatePoIntfMessage::getReplyCode() const
    {
        return (m_replyCode);
    }

    void  NsmGlobalUpdatePoIntfMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  NsmGlobalUpdatePoIntfMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }
	
	void NsmGlobalUpdatePoIntfMessage::setPvlanNativeVlanId(const UI32 &pVlanNativeVlanId)
	{
		m_pVlanNativeVlanId = pVlanNativeVlanId;
	}
	
	UI32 NsmGlobalUpdatePoIntfMessage::getPvlanNativeVlanId() const
	{
		return (m_pVlanNativeVlanId);
	}
	
	void  NsmGlobalUpdatePoIntfMessage::setPvlanIsAllowedVlanAll(const bool &pVlanIsAllowedVlanAll)
    {
        m_pVlanIsAllowedVlanAll = pVlanIsAllowedVlanAll;
    }

    bool  NsmGlobalUpdatePoIntfMessage::getPvlanIsAllowedVlanAll() const
    {
        return (m_pVlanIsAllowedVlanAll);
    }
	
	void  NsmGlobalUpdatePoIntfMessage::setPvlanRange (const UI32Range &pVlanRange)
    {
        m_pVlanRange = pVlanRange;
    }

    UI32Range NsmGlobalUpdatePoIntfMessage::getPvlanRange() const
    {
        return (m_pVlanRange);
    }

	void  NsmGlobalUpdatePoIntfMessage::setPvlanHostPrimVlanId(const UI32 &pVlanHostPrimVlanId)
    {
        m_pVlanHostPrimVlanId = pVlanHostPrimVlanId;
    }

   	UI32 NsmGlobalUpdatePoIntfMessage::getPvlanHostPrimVlanId() const
    {
        return (m_pVlanHostPrimVlanId);
    }

	void  NsmGlobalUpdatePoIntfMessage::setPvlanHostSecVlanId(const UI32 &pVlanHostSecVlanId)
    {
		m_pVlanHostSecVlanId = pVlanHostSecVlanId;
    }

   	UI32 NsmGlobalUpdatePoIntfMessage::getPvlanHostSecVlanId() const
    {
        return (m_pVlanHostSecVlanId);
    }
    void  NsmGlobalUpdatePoIntfMessage::setMtu(const UI32 &mtu)
    {
        m_mtu  =  mtu;
    }

    UI32  NsmGlobalUpdatePoIntfMessage::getMtu() const
    {
        return (m_mtu);
    }

    void  NsmGlobalUpdatePoIntfMessage::setNativeVlanId(const UI32 &nativeVlanId)
    {
        m_nativeVlanId =  nativeVlanId;
    }

    UI32 NsmGlobalUpdatePoIntfMessage::getNativeVlanId() const
    {
        return (m_nativeVlanId);
    }
    
	void  NsmGlobalUpdatePoIntfMessage::setNativevlanDisable(const bool 
		&nativevlanDisable)
    {
        m_nativevlanDisable  =  nativevlanDisable;
    }

    bool  NsmGlobalUpdatePoIntfMessage::getNativevlanDisable() const
    {
        return (m_nativevlanDisable);
    }
    
    void  NsmGlobalUpdatePoIntfMessage::setVlanRange(const UI32Range &vlanRange)
    {
        m_vlanRange  =  vlanRange;
    }

    UI32Range NsmGlobalUpdatePoIntfMessage::getVlanRange() const
    {
        return (m_vlanRange);
    }

    PoIntfManagedObject *NsmGlobalUpdatePoIntfMessage::getPoRef()
    {
        return m_refPoIntfManagedObject;
    }

    void NsmGlobalUpdatePoIntfMessage::setPoRef(PoIntfManagedObject *refPoIntfManagedObject)
    {
        m_refPoIntfManagedObject = refPoIntfManagedObject;
    }

    void NsmGlobalUpdatePoIntfMessage::setIsPortProfiled(const bool &isPortProfiled)
    {
        m_isPortProfiled = isPortProfiled;
    }

    bool NsmGlobalUpdatePoIntfMessage::getIsPortProfiled() const
    {
        return m_isPortProfiled;
    }

    void NsmGlobalUpdatePoIntfMessage::setPortProfileName(const string &portProfileName) {
        m_portProfileName = portProfileName;
    }

    string NsmGlobalUpdatePoIntfMessage::getPortProfileName() const {
        return m_portProfileName;
    }
    void NsmGlobalUpdatePoIntfMessage::setPortProfileDomainName(const string &portProfileDomainName) {
        m_portProfileDomainName = portProfileDomainName;
    }

    string NsmGlobalUpdatePoIntfMessage::getPortProfileDomainName() const {
        return m_portProfileDomainName;
    }

    void  NsmGlobalUpdatePoIntfMessage::setRaGuard(const bool &raGuard)
    {
        m_raGuard = raGuard;
    }

    bool  NsmGlobalUpdatePoIntfMessage::getRaGuard() const
    {
        return (m_raGuard);
    }
}
