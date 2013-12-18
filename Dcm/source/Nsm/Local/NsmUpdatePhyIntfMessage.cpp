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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : skbalasu                                                     *
 **************************************************************************/

#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/NsmUpdatePhyIntfMessage.h"
#include "Nsm/Local/NsmTypes.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"
#include "ClientInterface/Nsm/nsmdIpIntf.h"

namespace DcmNs
{

    NsmUpdatePhyIntfMessage::NsmUpdatePhyIntfMessage ()
        : ManagementInterfaceMessage (NsmLocalObjectManager::getClassName (),NSMUPDATEPHYINTF)
    {
		m_cmdCode = 0;
		m_enable = false;
		m_flag = false;
        m_isAllowedVlanAll = true;
        m_isPortProfiled = false;
        m_portProfileName = "";
        m_portProfileDomainName = "";
		m_lacpTimeout = LACP_TIMEOUT_DEFAULT;
		m_lacpPortPriority = DCM_LACP_DEFAULT_PORT_PRIORITY;
		m_lacpDefaultUp = LACP_DEFAULT_DEFAULT_UP;
		m_mode = SW_MODE_UNKNOWN;
        m_mtu = NSM_MTU_DEF_VAL;
        m_openflow = NSM_OPENFLOW_DEF_STATUS;
        m_nativeVlanId = 1;
		m_pollingInterval = 0;
		m_sampleRate = 0;
		m_swIfIndex = 0;
        m_swModeFlag = false;
        m_swMode = SW_MODE_UNKNOWN;
        m_ifType = IF_TYPE_INVALID;
        m_eld = false;
        m_eld_prio_val = 128;
        m_eld_vlan = 0;
        m_vlanid.clear();
        m_vlanidList.clear();
        m_vlanIdRange = UI32Range("");
		m_raGuard = false;
		m_pVlanNativeVlanId = 0;
		m_pVlanIsAllowedVlanAll = false;
		m_pVlanHostPrimVlanId = 0;
		m_pVlanHostSecVlanId = 0;
    }

    NsmUpdatePhyIntfMessage::NsmUpdatePhyIntfMessage (const string &ifName,const UI64 &swIfIndex,const UI32 &mtu,const bool &openflow,const string &desc,const bool &flag,const swModeEnum &swMode,const swModeEnum &mode,const bool &isAllowedVlanAll,const UI32 &cmdCode,const LacpTimeout &lacpTimeout, const UI32 &lacpPortPriority, const LineSpeed &lineSpeed, const UI32 &nativeVlanId,const bool &enable, const UI32 &pollingInterval, const UI32 &sampleRate, const bool &isPortProfiled, const UI32 &msgId, const UI32 &exception, const UI32 &pVlanNativeVlanId, const bool &pVlanIsAllowedVlanAll)
		: ManagementInterfaceMessage (NsmLocalObjectManager::getClassName (),NSMUPDATEPHYINTF),
		m_ifName    (ifName),
		m_swIfIndex    (swIfIndex),
		m_mtu    (mtu),
        m_openflow(openflow),
		m_desc    (desc),
		m_flag    (flag),
		m_swMode    (swMode),
		m_mode    (mode),
		m_isAllowedVlanAll    (isAllowedVlanAll),
		m_cmdCode    (cmdCode),
		m_pVlanNativeVlanId	(pVlanNativeVlanId),
		m_pVlanIsAllowedVlanAll (pVlanIsAllowedVlanAll),
		m_lacpTimeout    (lacpTimeout),
		m_lacpPortPriority  (lacpPortPriority),
		m_lineSpeed    (lineSpeed),
		m_nativeVlanId    (nativeVlanId),
		m_enable    (enable),
		m_pollingInterval    (pollingInterval),
		m_sampleRate    (sampleRate),
		m_isPortProfiled(isPortProfiled) ,
		m_msgId(msgId),
		m_exception(exception)

		{
			m_swModeFlag = false;
        	m_ifType = IF_TYPE_INVALID;
        	m_lacpDefaultUp = LACP_DEFAULT_DEFAULT_UP;
			m_raGuard = false;
            m_portProfileName = "";
            m_portProfileDomainName = "";
    }

    NsmUpdatePhyIntfMessage::~NsmUpdatePhyIntfMessage ()
    {
    }

    void  NsmUpdatePhyIntfMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeString(&m_desc,"desc"));
        addSerializableAttribute (new AttributeBool(&m_flag,"flag"));
        addSerializableAttribute (new AttributeBool(&m_active,"active"));
        addSerializableAttribute (new AttributeBool(&m_vepa,"vepa"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeBool(&m_isAllowedVlanAll,"isAllowedVlanAll"));
        addSerializableAttribute (new AttributeEnum((UI32*)(&m_mode),"mode"));
        addSerializableAttribute (new AttributeUI32(&m_mtu,"mtu"));
        addSerializableAttribute (new AttributeBool(&m_openflow,"openflow_enable"));
        addSerializableAttribute (new AttributeUI64(&m_swIfIndex,"swIfIndex"));
        addSerializableAttribute (new AttributeEnum((UI32*)(&m_swMode),"swMode"));
        addSerializableAttribute (new AttributeBool(&m_swModeFlag,"swModeFlag"));
        addSerializableAttribute (new AttributeUI32(&m_pVlanNativeVlanId,"pVlanNativeVlanId"));
        addSerializableAttribute (new AttributeBool(&m_pVlanIsAllowedVlanAll,"pVlanIsAllowedVlanAll"));
        addSerializableAttribute (new AttributeUI32Range(&m_pVlanRange,"pVlanRange"));
        addSerializableAttribute (new AttributeUI32(&m_pVlanHostPrimVlanId,"pVlanHostPrimVlanId"));
        addSerializableAttribute (new AttributeUI32(&m_pVlanHostSecVlanId,"pVlanHostSecVlanId"));
        addSerializableAttribute (new AttributeEnum((UI32 *)(&m_lacpTimeout),"lacpTimeout"));
	 	addSerializableAttribute (new AttributeEnum((UI32 *)(&m_lacpPortPriority),"lacpPortPriority"));
	 	addSerializableAttribute (new AttributeBool(&m_lacpDefaultUp, "lacpDefaultUp"));
        addSerializableAttribute (new AttributeEnum((UI32 *)(&m_lineSpeed),"lineSpeed"));
        addSerializableAttribute (new AttributeBool(&m_nativevlanDisable,
														"nativevlanDisable"));
        addSerializableAttribute (new AttributeUI32(&m_nativeVlanId,"nativeVlanId"));
        addSerializableAttribute (new AttributeBool(&m_enable,"enable"));
        addSerializableAttribute (new AttributeUI32(&m_pollingInterval,"pollingInterval"));
        addSerializableAttribute (new AttributeUI32(&m_sampleRate,"sampleRate"));
        addSerializableAttribute (new AttributeUI32Range(&m_vlanRange,"vlanRange"));
        addSerializableAttribute (new AttributeBool(&m_isPortProfiled,"isPortProfiled"));
        addSerializableAttribute (new AttributeString(&m_portProfileName, "portProfileName"));
        addSerializableAttribute (new AttributeString(&m_portProfileDomainName, "portProfileDomainName"));
        addSerializableAttribute (new AttributeUI32(&m_msgId,"msgId"));
        addSerializableAttribute (new AttributeUI32(&m_exception,"exception"));
        addSerializableAttribute (new AttributeBool(&m_eld,"eld"));
        addSerializableAttribute (new AttributeUI32(&m_eld_prio_val,"eldprio"));
        addSerializableAttribute (new AttributeUI32(&m_eld_vlan,"eldvlan"));
        addSerializableAttribute (new AttributeObjectIdVector(&m_vlanid,"vlanid"));
        addSerializableAttribute (new AttributeUI32Vector(&m_vlanidList,"vlanidList"));
        addSerializableAttribute (new AttributeUI32Range(&m_vlanIdRange,"vlanIdRange"));
        addSerializableAttribute (new AttributeUI32(&m_ifType, "ifType"));
        addSerializableAttribute (new AttributeBool(&m_raGuard, "raGuard"));
    }
     void  NsmUpdatePhyIntfMessage::setIfType(const UI32 &ifType)
    {
        m_ifType =  ifType;
    }

	UI32  NsmUpdatePhyIntfMessage::getIfType() const
	{
		return (m_ifType);
	}

	void  NsmUpdatePhyIntfMessage::setVlanid(const vector<ObjectId> &vlanid)
	{
		m_vlanid  =  vlanid;
	}

	vector<ObjectId>  NsmUpdatePhyIntfMessage::getVlanid() const
	{
		return (m_vlanid);
	}
	void  NsmUpdatePhyIntfMessage::setVlanIdRange(const UI32Range &vlanRange)
	{
		m_vlanIdRange  =  vlanRange;
	}

	UI32Range NsmUpdatePhyIntfMessage::getVlanIdRange() const
	{
		return (m_vlanIdRange);
	}

	void  NsmUpdatePhyIntfMessage::setVlanidList(const vector<UI32> &vlanidList)
	{
		m_vlanidList  =  vlanidList;
	}

	vector<UI32>  NsmUpdatePhyIntfMessage::getVlanidList() const
	{
		return (m_vlanidList);
    }

    void  NsmUpdatePhyIntfMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  NsmUpdatePhyIntfMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  NsmUpdatePhyIntfMessage::setSwIfIndex(const UI64 &swIfIndex)
    {
        m_swIfIndex  =  swIfIndex;
    }

    UI64  NsmUpdatePhyIntfMessage::getSwIfIndex() const
    {
        return (m_swIfIndex);
    }

    void  NsmUpdatePhyIntfMessage::setMtu(const UI32 &mtu)
    {
        m_mtu  =  mtu;
    }

    UI32  NsmUpdatePhyIntfMessage::getMtu() const
    {
        return (m_mtu);
    }

    void  NsmUpdatePhyIntfMessage::setOpenflow(const bool &openflow)
    {
        m_openflow  = openflow;

    }

    bool  NsmUpdatePhyIntfMessage::getOpenflow() const
    {
        return (m_openflow);
    }

    void  NsmUpdatePhyIntfMessage::setDesc(const string &desc)
    {
        m_desc  =  desc;
    }

    string  NsmUpdatePhyIntfMessage::getDesc() const
    {
        return (m_desc);
    }

    void  NsmUpdatePhyIntfMessage::setActive(const bool &active)
    {
        m_active  =  active;
    }

    bool  NsmUpdatePhyIntfMessage::getActive() const
    {
        return (m_active);
    }

    void  NsmUpdatePhyIntfMessage::setFlag(const bool &flag)
    {
        m_flag  =  flag;
    }

    bool  NsmUpdatePhyIntfMessage::getFlag() const
    {
        return (m_flag);
    }

    void  NsmUpdatePhyIntfMessage::setVepa(const bool &flag)
    {
        m_vepa  =  flag;
    }

    bool  NsmUpdatePhyIntfMessage::getVepa() const
    {
        return (m_vepa);
    }

    void  NsmUpdatePhyIntfMessage::setEld(const bool &eld)
    {
        m_eld = eld ;
    }

    bool NsmUpdatePhyIntfMessage::getEld() const
    {
        return (m_eld);
    } 

    void  NsmUpdatePhyIntfMessage::setEldPrioVal(const UI32 &eldPrio)
    {
        m_eld_prio_val = eldPrio;
    }

    UI32  NsmUpdatePhyIntfMessage::getEldPrioVal() const
    {
        return (m_eld_prio_val);
    }   

    void  NsmUpdatePhyIntfMessage::setEldVlan(const UI32 &eldVlan)
    {
        m_eld_vlan = eldVlan;
    }

    UI32  NsmUpdatePhyIntfMessage::getEldVlan() const
    {
        return (m_eld_vlan);
    }

    void  NsmUpdatePhyIntfMessage::setSwMode(const swModeEnum &swMode)
    {
        m_swMode  =  swMode;
    }

    swModeEnum  NsmUpdatePhyIntfMessage::getSwMode() const
    {
        return (m_swMode);
    }

    void  NsmUpdatePhyIntfMessage::setMode(const swModeEnum &mode)
    {
        m_mode  =  mode;
    }

    swModeEnum  NsmUpdatePhyIntfMessage::getMode() const
    {
        return (m_mode);
    }

    void  NsmUpdatePhyIntfMessage::setIsAllowedVlanAll(const bool &isAllowedVlanAll)
    {
        m_isAllowedVlanAll  =  isAllowedVlanAll;
    }

    bool  NsmUpdatePhyIntfMessage::getIsAllowedVlanAll() const
    {
        return (m_isAllowedVlanAll);
    }

    void  NsmUpdatePhyIntfMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  NsmUpdatePhyIntfMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  NsmUpdatePhyIntfMessage::setSwModeFlag(const bool &swModeFlag)
    {
        m_swModeFlag  =  swModeFlag;
    }

    bool  NsmUpdatePhyIntfMessage::getSwModeFlag() const
    {
        return (m_swModeFlag);
    }

	void NsmUpdatePhyIntfMessage::setPvlanNativeVlanId(const UI32 &pVlanNativeVlanId)
	{
		m_pVlanNativeVlanId = pVlanNativeVlanId;
	}

	UI32 NsmUpdatePhyIntfMessage::getPvlanNativeVlanId() const
	{
		return (m_pVlanNativeVlanId);
	}
	
	void NsmUpdatePhyIntfMessage::setPvlanIsAllowedVlanAll(const bool &pVlanIsAllowedVlanAll)
	{
		m_pVlanIsAllowedVlanAll = pVlanIsAllowedVlanAll;
	}

	bool NsmUpdatePhyIntfMessage::getPvlanIsAllowedVlanAll() const
	{
		return (m_pVlanIsAllowedVlanAll);
	}
		
	void  NsmUpdatePhyIntfMessage::setPvlanRange (const UI32Range &pVlanRange)
    {
        m_pVlanRange = pVlanRange;
    }

    UI32Range NsmUpdatePhyIntfMessage::getPvlanRange () const
    {
        return (m_pVlanRange);
    }

	void  NsmUpdatePhyIntfMessage::setPvlanHostPrimVlanId(const UI32 &pVlanHostPrimVlanId)
    {
        m_pVlanHostPrimVlanId = pVlanHostPrimVlanId;
    }

   	UI32 NsmUpdatePhyIntfMessage::getPvlanHostPrimVlanId() const
    {
        return (m_pVlanHostPrimVlanId);
    }

	void  NsmUpdatePhyIntfMessage::setPvlanHostSecVlanId(const UI32 &pVlanHostSecVlanId)
    {
		m_pVlanHostSecVlanId = pVlanHostSecVlanId;
    }

   	UI32 NsmUpdatePhyIntfMessage::getPvlanHostSecVlanId() const
    {
        return (m_pVlanHostSecVlanId);
    }

    void  NsmUpdatePhyIntfMessage::setLacpTimeout(const LacpTimeout &lacpTimeout)
    {
        m_lacpTimeout  =  lacpTimeout;
    }

    LacpTimeout  NsmUpdatePhyIntfMessage::getLacpTimeout() const
    {
        return (m_lacpTimeout);
    }

	void  NsmUpdatePhyIntfMessage::setLacpPortPriority(const UI32 &lacpPortPriority)
	{
		m_lacpPortPriority =  lacpPortPriority;
	}

	UI32 NsmUpdatePhyIntfMessage::getLacpPortPriority() const
	{
		return (m_lacpPortPriority);
	}

    void  NsmUpdatePhyIntfMessage::setLacpDefaultUp(const bool &lacpDefaultUp)
    {
        m_lacpDefaultUp =  lacpDefaultUp;
    }

    bool NsmUpdatePhyIntfMessage::getLacpDefaultUp() const
    {
        return (m_lacpDefaultUp);
    }
    
	void  NsmUpdatePhyIntfMessage::setLineSpeed(const LineSpeed &lineSpeed)
    {
        m_lineSpeed  =  lineSpeed;
    }

    LineSpeed  NsmUpdatePhyIntfMessage::getLineSpeed() const
    {
        return (m_lineSpeed);
    }
    
	void  NsmUpdatePhyIntfMessage::setNativeVlanId(const UI32 &nativeVlanId)
    {
        m_nativeVlanId =  nativeVlanId;
    }

    UI32 NsmUpdatePhyIntfMessage::getNativeVlanId() const
    {
        return (m_nativeVlanId);
    }
	
	void  NsmUpdatePhyIntfMessage::setNativevlanDisable(const bool 
		&nativevlanDisable)
    {
        m_nativevlanDisable  =  nativevlanDisable;
    }

    bool  NsmUpdatePhyIntfMessage::getNativevlanDisable() const
    {
        return (m_nativevlanDisable);
    }


    void  NsmUpdatePhyIntfMessage::setEnable(const bool &enable)
    {
        m_enable =  enable;
    }

    bool NsmUpdatePhyIntfMessage::getEnable() const
    {
        return (m_enable);
    }

    void  NsmUpdatePhyIntfMessage::setPollingInterval(const UI32 &pollingInterval)
    {
        m_pollingInterval =  pollingInterval;
    }

    UI32 NsmUpdatePhyIntfMessage::getPollingInterval() const
    {
        return (m_pollingInterval);
    }

    void  NsmUpdatePhyIntfMessage::setSampleRate(const UI32 &sampleRate)
    {
        m_sampleRate =  sampleRate;
    }

    UI32 NsmUpdatePhyIntfMessage::getSampleRate() const
    {
        return (m_sampleRate);
    }

    void  NsmUpdatePhyIntfMessage::setVlanRange(const UI32Range &vlanRange)
    {
        m_vlanRange  =  vlanRange;
    }

    UI32Range NsmUpdatePhyIntfMessage::getVlanRange() const
    {
        return (m_vlanRange);
    }

    void  NsmUpdatePhyIntfMessage::setIsPortProfiled(const bool &isPortProfiled)
    {
        m_isPortProfiled  =  isPortProfiled;
    }

    bool  NsmUpdatePhyIntfMessage::getIsPortProfiled() const
    {
        return (m_isPortProfiled);
    }

    void NsmUpdatePhyIntfMessage::setPortProfileName(const string &portProfileName) {
        m_portProfileName = portProfileName;
    }

    string NsmUpdatePhyIntfMessage::getPortProfileName() const {
        return m_portProfileName;
    }
    void NsmUpdatePhyIntfMessage::setPortProfileDomainName(const string &portProfileDomainName) {
        m_portProfileDomainName = portProfileDomainName;
    }

    string NsmUpdatePhyIntfMessage::getPortProfileDomainName() const {
        return m_portProfileDomainName;
    }

	void  NsmUpdatePhyIntfMessage::setMsgId(const UI32 &msgId)
    {
        m_msgId  =  msgId;
    }

   	UI32  NsmUpdatePhyIntfMessage::getMsgId() const
    {
        return (m_msgId);
    }

    void  NsmUpdatePhyIntfMessage::setException(const UI32 &exception)
    {
        m_exception  =  exception;
    }

    UI32  NsmUpdatePhyIntfMessage::getException() const
    {
        return (m_exception);
    }

    void  NsmUpdatePhyIntfMessage::setRaGuard(const bool &raGuard)
    {
        m_raGuard = raGuard;
    }

    bool  NsmUpdatePhyIntfMessage::getRaGuard() const
    {
        return (m_raGuard);
    }
    
}
