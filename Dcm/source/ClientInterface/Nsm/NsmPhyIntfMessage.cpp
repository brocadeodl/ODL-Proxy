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
 *   Copyright (C) 2005-2013 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : skbalasu                                                     *
 **************************************************************************/

#include "ClientInterface/Nsm/NsmPhyIntfMessage.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "ClientInterface/APPM/APPMMessageDef.h"

namespace DcmNs
{

    NsmPhyIntfMessage::NsmPhyIntfMessage ()
        : NsmVlanProvisionMessage (NSMPHYINTF)
    {
        m_bulkCmdCode = 0;
        m_ifType = IF_TYPE_INVALID;
		m_raGuard = false;
		m_lacpDefaultUp = LACP_DEFAULT_DEFAULT_UP;
    }
	 NsmPhyIntfMessage::NsmPhyIntfMessage (const string &ifName,const UI64 &swIfIndex,const UI32 &mtu,const bool &openflow,const string &desc,const bool &flag,const bool &swModeFlag,const UI32 &swMode,const bool &isAllowedVlanAll,const UI32 &cmdCode,const LacpTimeout &lacpTimeout, const UI32 &lacpPortPriority, const LineSpeed &lineSpeed, const UI32 &nativeVlanId,const string &poName, const bool &enable, const UI32 &pollingInterval, const UI32 &sampleRate, const bool &isPortProfiled, const UI32 &msgId, const UI32 &exception, const UI32 &pVlanNativeVlanId, const bool &pVlanIsAllowedVlanAll)
	 : NsmVlanProvisionMessage (NSMPHYINTF),
		 m_ifName    (ifName),
		 m_swIfIndex    (swIfIndex),
		 m_mtu    (mtu),
         m_openflow (openflow),
		 m_desc    (desc),
		 m_flag    (flag),
		 m_swModeFlag    (swModeFlag),
		 m_swMode    (swMode),
		 m_isAllowedVlanAll    (isAllowedVlanAll),
		 m_cmdCode    (cmdCode),
		 m_pVlanNativeVlanId		(pVlanNativeVlanId),
		 m_pVlanIsAllowedVlanAll	(pVlanIsAllowedVlanAll),
		 m_lacpTimeout    (lacpTimeout),
		 m_lacpPortPriority (lacpPortPriority),
		 m_lineSpeed    (lineSpeed),
		 m_nativeVlanId    (nativeVlanId),
		 m_poName (poName),
		 m_enable (enable),
		 m_pollingInterval (pollingInterval),
		 m_sampleRate (sampleRate),
		 m_isPortProfiled(isPortProfiled), 
		 m_msgId(msgId),
		 m_exception(exception)
    {
        m_bulkCmdCode = 0;
        m_ifType = IF_TYPE_INVALID;
		m_raGuard = false;
		m_lacpDefaultUp = LACP_DEFAULT_DEFAULT_UP;
    }

    NsmPhyIntfMessage::~NsmPhyIntfMessage ()
    {
    }

    void  NsmPhyIntfMessage::setupAttributesForSerialization()
    {
        NsmVlanProvisionMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeUI32(&m_bulkCmdCode,"bulkCmdCode"));
        addSerializableAttribute (new AttributeUI32(&m_ifType, "ifType"));
        addSerializableAttribute (new AttributeString(&m_desc,"desc"));
        addSerializableAttribute (new AttributeBool(&m_vepa,"vepa"));
        addSerializableAttribute (new AttributeBool(&m_flag,"flag"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeBool(&m_isAllowedVlanAll,"isAllowedVlanAll"));
        addSerializableAttribute (new AttributeUI32(&m_mtu,"mtu"));
        addSerializableAttribute (new AttributeBool(&m_openflow,"openflow_enable"));
        addSerializableAttribute (new AttributeUI64(&m_swIfIndex,"swIfIndex"));
        addSerializableAttribute (new AttributeUI32(&m_swMode,"swMode"));
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
        addSerializableAttribute (new AttributeString(&m_poName,"poName"));
        addSerializableAttribute (new AttributeBool(&m_enable,"enable"));
        addSerializableAttribute (new AttributeUI32(&m_pollingInterval,"pollingInterval"));
        addSerializableAttribute (new AttributeUI32(&m_sampleRate,"sampleRate"));
        addSerializableAttribute (new AttributeUI32Range(&m_vlanRange,"vlanRange"));
        addSerializableAttribute (new AttributeBool(&m_isPortProfiled,"isPortProfiled"));
        addSerializableAttribute (new AttributeUI32(&m_msgId,"msgId"));
        addSerializableAttribute (new AttributeUI32(&m_exception,"exception"));
        addSerializableAttribute (new AttributeUI32(&m_poMode,"mode"));
        addSerializableAttribute (new AttributeUI32(&m_poType,"type"));
        addSerializableAttribute (new AttributeString(&m_poLacpSystemId,"poLacpSystemId"));
        addSerializableAttribute (new AttributeUI32(&m_poLacpSystemIdDomainId,"poLacpSystemIdDomainId"));
 		addSerializableAttribute (new AttributeEnum((UI32 *)(&m_eld_prio_val),"eldprio"));
        addSerializableAttribute (new AttributeEnum((UI32 *)(&m_eld_vlan),"eldvlan"));
        addSerializableAttribute (new AttributeBool(&m_raGuard, "raGuard"));
        addSerializableAttribute (new AttributeString(&m_mac, "mac"));
        addSerializableAttribute (new AttributeUI32(&m_ctagId, "ctagId"));
        addSerializableAttribute (new AttributeUI32(&m_macGroupId, "macGroupId"));
    }

    void  NsmPhyIntfMessage::setPoLacpSystemId(const string &poLacpSystemId)
    {
        m_poLacpSystemId  =  poLacpSystemId;
    }

    string  NsmPhyIntfMessage::getPoLacpSystemId() const
    {
        return (m_poLacpSystemId);
    }

    void  NsmPhyIntfMessage::setPoLacpSystemIdDomainId(const UI32 &poLacpSystemIdDomainId)
    {
        m_poLacpSystemIdDomainId  =  poLacpSystemIdDomainId;
    }

    UI32  NsmPhyIntfMessage::getPoLacpSystemIdDomainId() const
    {
        return (m_poLacpSystemIdDomainId);
    }

    void  NsmPhyIntfMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  NsmPhyIntfMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  NsmPhyIntfMessage::setPortChannelName(const string &poName,const UI32 &mode, const UI32 &type)
    {
        m_poName  =  poName;
        m_poMode  =  mode;
        m_poType  =  type;
        m_bulkCmdCode |= NSMUPDATE_PHY_ADD_CHANNELGROUP_CMD;
    }
    
    void  NsmPhyIntfMessage::setPoName(const string &poName)
    {
        m_poName  =  poName;
    }

    string  NsmPhyIntfMessage::getPoName() const
    {
        return (m_poName);
    }

    void  NsmPhyIntfMessage::setSwIfIndex(const UI64 &swIfIndex)
    {
        m_swIfIndex  =  swIfIndex;
    }

    UI64  NsmPhyIntfMessage::getSwIfIndex() const
    {
        return (m_swIfIndex);
    }

    void  NsmPhyIntfMessage::setMtu(const UI32 &mtu)
    {
        m_mtu  =  mtu;
        m_bulkCmdCode |= NSMUPDATE_PHY_MTU_CMD;
    }

    UI32  NsmPhyIntfMessage::getMtu() const
    {
        return (m_mtu);
    }

    void  NsmPhyIntfMessage::setOpenflow(const bool &openflow)
    {
        m_openflow  =  openflow;
    }

    bool  NsmPhyIntfMessage::getOpenflow() const
    {
        return (m_openflow);
    }

    void  NsmPhyIntfMessage::setEld(const bool &eld)
    {
        m_eld = eld ;
    }

    bool NsmPhyIntfMessage::getEld() const
    {
        return (m_eld);
    }

    void  NsmPhyIntfMessage::setEldPrioVal(const UI32 &eldPrio)
    {
        m_eld_prio_val = eldPrio;
    }

    UI32  NsmPhyIntfMessage::getEldPrioVal() const
    {
        return (m_eld_prio_val);
    }

    void  NsmPhyIntfMessage::setEldVlan(const UI32 &eldVlan)
    {
        m_eld_vlan = eldVlan;
    }

    UI32  NsmPhyIntfMessage::getEldVlan() const
    {
        return (m_eld_vlan);
    }

    void  NsmPhyIntfMessage::setDesc(const string &desc)
    {
        m_desc  =  desc;
        m_bulkCmdCode |= NSMUPDATE_PHY_DESC_CMD;
    }

    string  NsmPhyIntfMessage::getDesc() const
    {
        return (m_desc);
    }

    void  NsmPhyIntfMessage::setFlag(const bool &flag)
    {
        m_flag  =  flag;
        if(flag)
            m_bulkCmdCode |= NSMUPDATE_PHY_STATUS_CMD;
        else
            m_bulkCmdCode |= NSMUPDATE_PHY_DELSTATUS_CMD;
            
    }

    bool  NsmPhyIntfMessage::getFlag() const
    {
        return (m_flag);
    }
    
    void  NsmPhyIntfMessage::setVepa(const bool &flag)
    {
        m_vepa  =  flag;
        m_bulkCmdCode |= NSMUPDATE_PHY_VEPA_CMD;
    }

    bool  NsmPhyIntfMessage::getVepa() const
    {
        return (m_vepa);
    }

    void  NsmPhyIntfMessage::setSwMode(const UI32 &swMode)
     {
         m_swMode  =  swMode;
         m_bulkCmdCode |= NSMUPDATE_PHY_MODE_CMD;
     }

     UI32  NsmPhyIntfMessage::getSwMode() const
     {
         return (m_swMode);
     }

    void  NsmPhyIntfMessage::setIsAllowedVlanAll(const bool &isAllowedVlanAll)
    {
        m_isAllowedVlanAll  =  isAllowedVlanAll;
         m_bulkCmdCode |= NSMUPDATE_PHY_SW_TRUNK_VLAN_ALL_CMD;
    }

    bool  NsmPhyIntfMessage::getIsAllowedVlanAll() const
    {
        return (m_isAllowedVlanAll);
    }
    
	void  NsmPhyIntfMessage::setLacpTimeout(const LacpTimeout &lacpTimeout)
    {
        m_lacpTimeout  =  lacpTimeout;
    }

    LacpTimeout  NsmPhyIntfMessage::getLacpTimeout() const
    {
        return (m_lacpTimeout);
    }
	
	void  NsmPhyIntfMessage::setLacpPortPriority(const UI32 &lacpPortPriority)
	{
		m_lacpPortPriority  =  lacpPortPriority;
	}

	UI32 NsmPhyIntfMessage::getLacpPortPriority() const
	{
		return (m_lacpPortPriority);
	}
	
    void  NsmPhyIntfMessage::setLacpDefaultUp(const bool &lacpDefaultUp)
    {
        m_lacpDefaultUp =  lacpDefaultUp;
    }

    bool NsmPhyIntfMessage::getLacpDefaultUp() const
    {
        return (m_lacpDefaultUp);
    }

	void  NsmPhyIntfMessage::setLineSpeed(const LineSpeed &lineSpeed)
    {
        m_lineSpeed  =  lineSpeed;
        m_bulkCmdCode |= NSMUPDATE_PHY_LINESPEED_CMD;
    }

    LineSpeed  NsmPhyIntfMessage::getLineSpeed() const
    {
        return (m_lineSpeed);
    }

    void  NsmPhyIntfMessage::setNativeVlanId(const UI32 &nativeVlanId)
    {
        m_nativeVlanId = nativeVlanId;
        m_bulkCmdCode |= NSMUPDATE_PHY_NATIVEVLANID_CMD;
    }

    UI32  NsmPhyIntfMessage::getNativeVlanId() const
    {
        return (m_nativeVlanId);
    }
	
	void  NsmPhyIntfMessage::setNativevlanDisable(const bool 
		&nativevlanDisable)
    {
        m_nativevlanDisable  =  nativevlanDisable;
        m_bulkCmdCode |= NSMUPDATE_PHY_NATIVEVLANDISABLE_CMD;
    }

    bool  NsmPhyIntfMessage::getNativevlanDisable() const
    {
        return (m_nativevlanDisable);
    }

    void  NsmPhyIntfMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  NsmPhyIntfMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  NsmPhyIntfMessage::setSwModeFlag(const bool &swModeFlag)
    {
        m_swModeFlag  =  swModeFlag;
        m_bulkCmdCode |= NSMUPDATE_PHY_SW_CMD;
    }

    bool  NsmPhyIntfMessage::getSwModeFlag() const
    {
        return (m_swModeFlag);
    }

	void NsmPhyIntfMessage::setPvlanNativeVlanId(const UI32 &pVlanNativeVlanId)
	{
		m_pVlanNativeVlanId = pVlanNativeVlanId;
	}

	UI32 NsmPhyIntfMessage::getPvlanNativeVlanId() const
	{
		return (m_pVlanNativeVlanId);
	}
	
	void NsmPhyIntfMessage::setPvlanIsAllowedVlanAll(const bool &pVlanIsAllowedVlanAll)
	{
		m_pVlanIsAllowedVlanAll = pVlanIsAllowedVlanAll;
	}

	bool NsmPhyIntfMessage::getPvlanIsAllowedVlanAll() const
	{
		return (m_pVlanIsAllowedVlanAll);
	}
	
	void  NsmPhyIntfMessage::setPvlanRange (const UI32Range &pVlanRange)
    {
        m_pVlanRange = pVlanRange;
    }

    UI32Range  NsmPhyIntfMessage::getPvlanRange () const
    {
        return (m_pVlanRange);
    }

	void  NsmPhyIntfMessage::setPvlanHostPrimVlanId(const UI32 &pVlanHostPrimVlanId)
    {
        m_pVlanHostPrimVlanId = pVlanHostPrimVlanId;
    }

   	UI32 NsmPhyIntfMessage::getPvlanHostPrimVlanId() const
    {
        return (m_pVlanHostPrimVlanId);
    }

	void  NsmPhyIntfMessage::setPvlanHostSecVlanId(const UI32 &pVlanHostSecVlanId)
    {
		m_pVlanHostSecVlanId = pVlanHostSecVlanId;
    }

   	UI32 NsmPhyIntfMessage::getPvlanHostSecVlanId() const
    {
        return (m_pVlanHostSecVlanId);
    }

    void  NsmPhyIntfMessage::setEnable(const bool &enable)
    {
        m_enable = enable;
    }

    bool NsmPhyIntfMessage::getEnable() const
    {
        return (m_enable);
    }
    
    void  NsmPhyIntfMessage::setPollingInterval(const UI32 &pollingInterval)
    {
        m_pollingInterval = pollingInterval;
    }

    UI32  NsmPhyIntfMessage::getPollingInterval() const
    {
        return (m_pollingInterval);
    }
    
    void  NsmPhyIntfMessage::setSampleRate(const UI32 &sampleRate)
    {
        m_sampleRate = sampleRate;
    }

    UI32  NsmPhyIntfMessage::getSampleRate() const
    {
        return (m_sampleRate);
    }

    void  NsmPhyIntfMessage::setVlanRange(const UI32Range &vlanRange)
    {
        m_vlanRange  =  vlanRange;
    }

    void  NsmPhyIntfMessage::setVlanRange(const UI32Range &vlanRange, const UI32 type )
    {
        m_vlanRange  =  vlanRange;
        m_bulkCmdCode |= type;
    }

    UI32Range  NsmPhyIntfMessage::getVlanRange() const
    {
        return (m_vlanRange);
    }

    void  NsmPhyIntfMessage::setIsPortProfiled(const bool &isPortProfiled)
    {
        m_isPortProfiled  =  isPortProfiled;
    }

	void  NsmPhyIntfMessage::setIsPortProfiled(const bool &isPortProfiled, const UI32 type)
      {
          m_isPortProfiled  =  isPortProfiled;
          m_bulkCmdCode |= type;
      }
    
    bool  NsmPhyIntfMessage::getIsPortProfiled() const
    {
        return (m_isPortProfiled);
    }
    void NsmPhyIntfMessage::setPortProfileName(const string &portProfileName) {
        m_portProfileName = portProfileName;
    }

    void NsmPhyIntfMessage::setPortProfileDomainName(const string &portProfileDomainName) {
        m_portProfileDomainName = portProfileDomainName;
    }

    string NsmPhyIntfMessage::getPortProfileName() const {
        return m_portProfileName;
    }

    string NsmPhyIntfMessage::getPortProfileDomainName() const {
        return m_portProfileDomainName;
    }

	void  NsmPhyIntfMessage::setMsgId(const UI32 &msgId)
    {
        m_msgId  =  msgId;
    }

    UI32  NsmPhyIntfMessage::getMsgId() const
    {
        return (m_msgId);
    }

    void  NsmPhyIntfMessage::setException(const UI32 &exception)
    {
        m_exception =  exception;
    }

    UI32  NsmPhyIntfMessage::getException() const
    {
        return (m_exception);
    }

    void  NsmPhyIntfMessage::setPoMode(const UI32 &mode)
    {
        m_poMode  =  mode;
    }

    UI32  NsmPhyIntfMessage::getPoMode() const
    {
        return (m_poMode);
    }

    void  NsmPhyIntfMessage::setPoType(const UI32 &type)
    {
        m_poType  =  type;
    }

    UI32  NsmPhyIntfMessage::getPoType() const
    {
        return (m_poType);
    }

    const void *NsmPhyIntfMessage::getCStructureForInputs ()
    {
		NsmPhyIntfMsg_t *pInput = new NsmPhyIntfMsg_t;
		
		strncpy (pInput->ifName, m_ifName.c_str (), 64);
		pInput->ifName[63] = '\0';
		strncpy (pInput->poName, m_poName.c_str (), 64);
		pInput->poName[63] = '\0';
        strncpy (pInput->swVlanName, (char*)(m_vlanRange.toString()).c_str(),64);		
		pInput->swVlanName[63] = '\0';
		pInput->swIfIndex = m_swIfIndex;
		strncpy (pInput->desc, m_desc.c_str (), 64);
		pInput->desc[63] = '\0';
		pInput->flag     = m_flag;
		pInput->swMode     = m_swMode;
		pInput->mtu     = m_mtu;
        pInput->openflow = m_openflow;
		pInput->vepa     = m_vepa;
		pInput->isAllowedVlanAll = m_isAllowedVlanAll;
		pInput->cmdCode     = m_cmdCode;
        pInput->bulkCmdCode     = m_bulkCmdCode;
		pInput->vlanList = NULL;

		if ( (pInput->cmdCode & NSMUPDATE_PHY_SW_ADD_TRUNK_VLAN) ||
		     (pInput->cmdCode & NSMUPDATE_PHY_SW_DELETE_TRUNK_VLAN) ||
		     (pInput->cmdCode & NSMUPDATE_PHY_SW_EXCEPT_TRUNK_VLAN) ||
		     (pInput->cmdCode == GVLAN_TRUNK_PORT_SET_CTAG) ||
		     (pInput->cmdCode == GVLAN_TRUNK_PORT_DEL_CTAG) ||
		     ( (pInput->cmdCode & NSMUPDATE_PHY_BULK_CONF) &&
		       ( (pInput->bulkCmdCode & NSMUPDATE_PHY_SW_ADD_TRUNK_VLAN_CMD) ||
		         (pInput->bulkCmdCode & NSMUPDATE_PHY_SW_DELETE_TRUNK_VLAN_CMD) ||
		         (pInput->bulkCmdCode & NSMUPDATE_PHY_SW_EXCEPT_TRUNK_VLAN_CMD) ) ) ) {
			char *vlanList = NULL;
			int vlSize = 0;

			vlanList = (char *)(m_vlanRange.toString()).c_str();
			vlSize = (m_vlanRange.toString()).size();

			if (vlanList) {
			    pInput->vlanList = (char *)malloc(vlSize + 1);
			    strncpy (pInput->vlanList, vlanList, vlSize);
			    pInput->vlanList[vlSize] = '\0';
			}
		}

		pInput->pVlanList = NULL;
		if ( (pInput->cmdCode & NSMUPDATE_PHY_SW_PVLAN_TRUNK_VLAN_ADD) ||
		     (pInput->cmdCode & NSMUPDATE_PHY_SW_PVLAN_TRUNK_VLAN_REMOVE) ||
		     (pInput->cmdCode & NSMUPDATE_PHY_SW_PVLAN_TRUNK_VLAN_EXCEPT)) { //PVLAN todo bulkCmdCode
			char *pvlanList = NULL;
			int pvlSize = 0;

			pvlanList = (char *)(m_pVlanRange.toString()).c_str();
			pvlSize = (m_pVlanRange.toString()).size();

			if (pvlanList) {
			    pInput->pVlanList = (char *)malloc(pvlSize + 1);
			    strncpy (pInput->pVlanList, pvlanList, pvlSize);
			    pInput->pVlanList[pvlSize] = '\0';
			}
		}

		pInput->swModeFlag     = m_swModeFlag;
		pInput->lacpTimeout     = m_lacpTimeout;
		pInput->lacpPortPriority = m_lacpPortPriority;
		pInput->lacpDefaultUp  = m_lacpDefaultUp;
		pInput->line_speed     = m_lineSpeed;
		pInput->native_vlan_disable     = m_nativevlanDisable;
		pInput->native_vlan_id     = m_nativeVlanId;
		pInput->enable     = m_enable;
		pInput->pollingInterval     = m_pollingInterval;
		pInput->sampleRate     = m_sampleRate;
		pInput->isPortProfiled     = m_isPortProfiled;

        strncpy(pInput->portProfileName, m_portProfileName.c_str(), MAX_WYSEREA_PROFILE_NAME_LEN);
        pInput->portProfileName[MAX_WYSEREA_PROFILE_NAME_LEN] = '\0';

        strncpy(pInput->portProfileDomainName, m_portProfileDomainName.c_str(), MAX_WYSEREA_PROFILE_NAME_LEN);
        pInput->portProfileDomainName[MAX_WYSEREA_PROFILE_NAME_LEN] = '\0';

		pInput->msgId     = m_msgId;
		pInput->exception     = m_exception;
        pInput->poMode  = m_poMode;
        pInput->poType = m_poType;
        pInput->ifType = m_ifType;

        pInput->poLacpSystemIdDomainId = m_poLacpSystemIdDomainId;
		strncpy (pInput->polacpSystemId, m_poLacpSystemId.c_str (), 64);
		pInput->raGuard = m_raGuard;

		pInput->pVlanHostPrimVlanId = m_pVlanHostPrimVlanId;
		pInput->pVlanHostSecVlanId = m_pVlanHostSecVlanId;
		pInput->pVlanNativeVlanId = m_pVlanNativeVlanId;

		if((m_cmdCode == GVLAN_ACCESS_PORT_SET_MAC_ADDRESS) || (m_cmdCode == GVLAN_ACCESS_PORT_DEL_MAC_ADDRESS))
		{
			unsigned char tempMac[MAC_ADDR_LENGTH_IN_BYTES] = {0};
			sscanf(m_mac.c_str(), "%4hx.%4hx.%4hx", (unsigned short *)&tempMac[0], (unsigned short *)&tempMac[2], (unsigned short *)&tempMac[4]);
			memcpy(pInput->mac, tempMac, MAC_ADDR_LENGTH_IN_BYTES);
		}

		pInput->ctagId = m_ctagId;
		pInput->macGroupId = m_macGroupId;

		return (pInput);
    }

    void  NsmPhyIntfMessage::setBulkCmdCode(const UI32 &cmdCode)
    {
        m_bulkCmdCode  =  cmdCode;
    }

    UI32  NsmPhyIntfMessage::getBulkCmdCode() const
    {
        return (m_bulkCmdCode);
    }

    void  NsmPhyIntfMessage::setIfType(const UI32 &ifType)
    {
        m_ifType =  ifType;
    }

    UI32  NsmPhyIntfMessage::getIfType() const
    {
        return (m_ifType);
    }
	
    void  NsmPhyIntfMessage::setRaGuard(const bool &raGuard)
    {
        m_raGuard = raGuard ;
        m_bulkCmdCode |= NSMUPDATE_PHY_RA_GUARD_CMD;
    }

    bool NsmPhyIntfMessage::getRaGuard() const
    {
        return (m_raGuard);
    }

    void  NsmPhyIntfMessage::setNbrDiscoveryDisable(const bool &discoveryFlag)
    {
        m_bulkCmdCode |= NSMUPDATE_PHY_NBR_DISCOVERY_DISABLE_CMD;
    }

    void  NsmPhyIntfMessage::setMac(const string &mac)
    {
        m_mac = mac;
    }

    string NsmPhyIntfMessage::getMac() const
    {
        return (m_mac);
    }

    void  NsmPhyIntfMessage::setCtagId(const UI32 &ctagId)
    {
        m_ctagId = ctagId;
    }

    UI32 NsmPhyIntfMessage::getCtagId() const
    {
        return (m_ctagId);
    }

    void NsmPhyIntfMessage::setMacGroupId(const UI32 &macGroupId)
    {
        m_macGroupId = macGroupId;
    }

    UI32 NsmPhyIntfMessage::getMacGroupId() const
    {
        return m_macGroupId;
    }
}
