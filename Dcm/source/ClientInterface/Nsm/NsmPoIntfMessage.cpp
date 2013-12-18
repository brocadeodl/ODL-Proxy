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
 *   Author : skbalasu                                                     *
 **************************************************************************/

#include "ClientInterface/Nsm/NsmPoIntfMessage.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"
#include "ClientInterface/Eld/eld_dcm_def.h"
#include "Framework/Attributes/AttributeUI32Range.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "Framework/Utils/TraceUtils.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "Framework/Attributes/AttributeEnum.h"
namespace DcmNs
{

    NsmPoIntfMessage::NsmPoIntfMessage ()
        : NsmVlanProvisionMessage (NSMPOINTF)
    {
        m_bulkCmdCode = 0;
       /* ELD */
        m_vlanidList.clear();
    //  m_vlanIdRange = UI32Range("");
       /* ELD */
        m_raGuard = false;
    }

    NsmPoIntfMessage::NsmPoIntfMessage (const UI32 &bulkCmdCode)
        : NsmVlanProvisionMessage (NSMPOINTF)
    {
        m_bulkCmdCode = 0;
        m_bulkCmdCode |= bulkCmdCode;
        m_raGuard = false;
    }
    NsmPoIntfMessage::NsmPoIntfMessage(const string &poId, const string &poName,
        const UI32 &poIfIndex, const UI32 mtu, const string &desc,
        const bool &flag, const bool &swModeFlag, const UI32 &swMode,
        const bool &isAllowedVlanAll, const UI32 &poMode,
        const UI32 &poType, const UI32 &poMinLinks,
        const LineSpeed &poAllowedSpeed, const UI32 &poLoadBalanceMode,
        const UI32 &poPrimaryLinkDomainId,
        const UI32 &poPartnerSystemPriority, const UI32 &poPartnerKey,
        const string &poPartnerSystem, const string &poLacpSystemId,
        const UI32 &cmdCode, const bool &isPortProfiled,
        const UI32 &pVlanNativeVlanId, const bool &pVlanIsAllowedVlanAll)
        : NsmVlanProvisionMessage (NSMPOINTF),
        m_poId    (poId),
        m_poName    (poName),
        m_poIfIndex    (poIfIndex),
        m_mtu    (mtu),
        m_desc    (desc),
        m_flag    (flag),
        m_swModeFlag    (swModeFlag),
        m_swMode    (swMode),
        m_isAllowedVlanAll    (isAllowedVlanAll),
        m_poMode    (poMode),
        m_poType    (poType),
        m_poMinLinks    (poMinLinks),
		m_poAllowedSpeed   (poAllowedSpeed),
		m_poLoadBalanceMode (poLoadBalanceMode),
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
        m_bulkCmdCode = 0;
        m_ignore_split = false;
        m_raGuard = false;
        m_portProfileName = "";
        m_portProfileDomainName = "";
    }

    NsmPoIntfMessage::~NsmPoIntfMessage ()
    {
    }

    void  NsmPoIntfMessage::setupAttributesForSerialization()
    {
        NsmVlanProvisionMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeUI32(&m_bulkCmdCode,"bulkcmdCode"));
        addSerializableAttribute (new AttributeString(&m_desc,"desc"));
        addSerializableAttribute (new AttributeBool(&m_ignore_split,"ignore_split"));
        addSerializableAttribute (new AttributeBool(&m_flag,"flag"));
        addSerializableAttribute (new AttributeBool(&m_isAllowedVlanAll,"isAllowedVlanAll"));
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
        addSerializableAttribute (new AttributeUI32(&m_swMode,"swMode"));
        addSerializableAttribute (new AttributeBool(&m_swModeFlag,"swModeFlag"));
        addSerializableAttribute (new AttributeUI32(&m_pVlanNativeVlanId,"pVlanNativeVlanId"));
        addSerializableAttribute (new AttributeBool(&m_pVlanIsAllowedVlanAll,"pVlanIsAllowedVlanAll"));
        addSerializableAttribute (new AttributeUI32Range(&m_pVlanRange,"pVlanRange"));
        addSerializableAttribute (new AttributeUI32(&m_pVlanHostPrimVlanId,"pVlanHostPrimVlanId"));
        addSerializableAttribute (new AttributeUI32(&m_pVlanHostSecVlanId,"pVlanHostSecVlanId"));
        addSerializableAttribute (new AttributeUI32Range(&m_vlanRange,"vlanRange"));
        addSerializableAttribute (new AttributeBool(&m_nativevlanDisable,
														"nativevlanDisable"));
        addSerializableAttribute (new AttributeUI32(&m_nativeVlanId,"nativeVlanId"));
        addSerializableAttribute (new AttributeBool(&m_isPortProfiled,"isPortProfiled"));
        addSerializableAttribute (new AttributeString(&m_portProfileName, "portProfileName"));
        addSerializableAttribute (new AttributeString(&m_portProfileDomainName, "portProfileDomainName"));
       /* ELD */
        addSerializableAttribute (new AttributeUI32(&m_prio,"prio"));
        addSerializableAttribute (new AttributeUI32Vector(&m_vlanidList,"vlanidList"));
       /* ELD */
        addSerializableAttribute (new AttributeBool(&m_raGuard, "raGuard"));
        addSerializableAttribute (new AttributeUI32(&m_ctagId, "ctagId"));
        addSerializableAttribute (new AttributeString(&m_mac, "mac"));
        addSerializableAttribute (new AttributeUI32(&m_macGroupId, "macGroupId"));
    }

    void  NsmPoIntfMessage::setPrio(const UI32 &prio)
    {
        m_prio  =  prio;
    }

    UI32  NsmPoIntfMessage::getPrio() const
    {
        return (m_prio);
    }

    void  NsmPoIntfMessage::setVlanidList(const vector<UI32> &vlanidList)
    {
        unsigned int i;
        m_vlanidList  =  vlanidList;
        for (i = 0; i< m_vlanidList.size(); i++)
        {
             WaveNs::trace (TRACE_LEVEL_INFO, string ("\t TO CLIENT: setVlanidList(): i = ") + i + string("VLAN_ID = ") + m_vlanidList[i]);
            printf("setVlanidList():I = %d, VLAN_ID IS %d\n", i, m_vlanidList[i]);
        }
    }

    vector<UI32>  NsmPoIntfMessage::getVlanidList() const
    {
        return (m_vlanidList);
    }
       /* ELD */

    void  NsmPoIntfMessage::setPoId(const string &poId)
    {
        m_poId  =  poId;
        m_bulkCmdCode |= NSMUPDATE_PO_CREATE_CMD;
    }

    string  NsmPoIntfMessage::getPoId() const
    {
        return (m_poId);
    }

    void  NsmPoIntfMessage::setPoName(const string &poName)
    {
        m_poName  =  poName;
    }

    string  NsmPoIntfMessage::getPoName() const
    {
        return (m_poName);
    }

    void  NsmPoIntfMessage::setPoIfIndex(const UI32 &poIfIndex)
    {
        m_poIfIndex  =  poIfIndex;
    }

    UI32  NsmPoIntfMessage::getPoIfIndex() const
    {
        return (m_poIfIndex);
    }

    void  NsmPoIntfMessage::setDesc(const string &desc)
    {
        m_desc  =  desc;
        m_bulkCmdCode |= NSMUPDATE_PO_DESC_CMD;
    }

    string  NsmPoIntfMessage::getDesc() const
    {
        return (m_desc);
    }

    void  NsmPoIntfMessage::setFlag(const bool &flag)
    {
        m_flag  =  flag;
        if(flag)
            m_bulkCmdCode |= NSMUPDATE_PO_STATUS_CMD;
        else
            m_bulkCmdCode |= NSMUPDATE_PO_DELSTATUS_CMD;
            
    }

    bool  NsmPoIntfMessage::getFlag() const
    {
        return (m_flag);
    }
    
    void  NsmPoIntfMessage::setIgnoreSplit(const bool &flag)
    {
        m_ignore_split  =  flag;
        if(flag)
            m_bulkCmdCode |= NSMUPDATE_PO_IGNORE_SPLIT_CMD;
        else
            m_bulkCmdCode |= NSMUPDATE_PO_NO_IGNORE_SPLIT_CMD;
            
    }

    bool  NsmPoIntfMessage::getIgnoreSplit() const
    {
        return (m_ignore_split);
    }
    
    void  NsmPoIntfMessage::setSwMode(const UI32 &swMode)
    {
        m_swMode  =  swMode;
        m_bulkCmdCode |= NSMUPDATE_PO_MODE_CMD;
    }

    UI32  NsmPoIntfMessage::getSwMode() const
    {
        return (m_swMode);
    }

    void  NsmPoIntfMessage::setIsAllowedVlanAll(const bool &isAllowedVlanAll)
    {
        m_isAllowedVlanAll  =  isAllowedVlanAll;
        m_bulkCmdCode |= NSMUPDATE_PO_SW_TRUNK_VLAN_ALL_CMD;
    }

    bool  NsmPoIntfMessage::getIsAllowedVlanAll() const
    {
        return (m_isAllowedVlanAll);
    }

    void  NsmPoIntfMessage::setPoMode(const UI32 &poMode)
    {
        m_poMode  =  poMode;
    }

    UI32  NsmPoIntfMessage::getPoMode() const
    {
        return (m_poMode);
    }

    void  NsmPoIntfMessage::setPoTypeAndMode( const UI32 &poType,const UI32 &poMode)
    {
        m_poType = poType;
        m_poMode = poMode;
        m_bulkCmdCode |= NSMUPDATE_PO_MODETYPE_CMD;
    }
    void  NsmPoIntfMessage::setPoType(const UI32 &poType)
    {
        m_poType  =  poType;
    }

    UI32  NsmPoIntfMessage::getPoType() const
    {
        return (m_poType);
    }

    void  NsmPoIntfMessage::setPoMinLinks(const UI32 &poMinLinks)
    {
        m_poMinLinks  =  poMinLinks;
        m_bulkCmdCode |= NSMUPDATE_PO_MINLINKS_CMD;
    }

    UI32  NsmPoIntfMessage::getPoMinLinks() const
    {
        return (m_poMinLinks);
    }

	void  NsmPoIntfMessage::setPoAllowedSpeed(const LineSpeed &poAllowedSpeed)
	{
		m_poAllowedSpeed  =  poAllowedSpeed;
		m_bulkCmdCode |= NSMUPDATE_PO_ALLOWEDSPEED_CMD;
	}

	LineSpeed  NsmPoIntfMessage::getPoAllowedSpeed() const
	{
		return (m_poAllowedSpeed);
	}

	void  NsmPoIntfMessage::setPoLoadBalanceMode(const UI32 &poLoadBalanceMode)
    {
        m_poLoadBalanceMode  =  poLoadBalanceMode;
        m_bulkCmdCode |= NSMUPDATE_PO_LOADBALANCE_CMD;
    }

    UI32  NsmPoIntfMessage::getPoLoadBalanceMode() const
    {
        return (m_poLoadBalanceMode);
    }

    void  NsmPoIntfMessage::setPoPrimaryLinkDomainId(const UI32 &poPrimaryLinkDomainId)
    {
        m_poPrimaryLinkDomainId  =  poPrimaryLinkDomainId;
    }

    UI32  NsmPoIntfMessage::getPoPrimaryLinkDomainId() const
    {
        return (m_poPrimaryLinkDomainId);
    }

    void  NsmPoIntfMessage::setPoPartnerSystemPriority(const UI32 &poPartnerSystemPriority)
    {
        m_poPartnerSystemPriority  =  poPartnerSystemPriority;
    }

    UI32  NsmPoIntfMessage::getPoPartnerSystemPriority() const
    {
        return (m_poPartnerSystemPriority);
    }

    void  NsmPoIntfMessage::setPoPartnerKey(const UI32 &poPartnerKey)
    {
        m_poPartnerKey  =  poPartnerKey;
    }

    UI32  NsmPoIntfMessage::getPoPartnerKey() const
    {
        return (m_poPartnerKey);
    }

    void  NsmPoIntfMessage::setPoPartnerSystem(const string &poPartnerSystem)
    {
        m_poPartnerSystem  =  poPartnerSystem;
    }

    string  NsmPoIntfMessage::getPoPartnerSystem() const
    {
        return (m_poPartnerSystem);
    }

    void  NsmPoIntfMessage::setPoLacpSystemId(const string &poLacpSystemId)
    {
        m_poLacpSystemId  =  poLacpSystemId;
    }

    string  NsmPoIntfMessage::getPoLacpSystemId() const
    {
        return (m_poLacpSystemId);
    }

    void  NsmPoIntfMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  NsmPoIntfMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  NsmPoIntfMessage::setSwModeFlag(const bool &swModeFlag)
    {
        m_swModeFlag  =  swModeFlag;
        m_bulkCmdCode |= NSMUPDATE_PO_SW_CMD;
    }

    bool  NsmPoIntfMessage::getSwModeFlag() const
    {
        return (m_swModeFlag);
    }

	void  NsmPoIntfMessage::setPvlanNativeVlanId(const UI32 &pVlanNativeVlanId)
    {
        m_pVlanNativeVlanId = pVlanNativeVlanId;
    }
    UI32  NsmPoIntfMessage::getPvlanNativeVlanId() const
    {
        return (m_pVlanNativeVlanId);
	}
	
	void NsmPoIntfMessage::setPvlanIsAllowedVlanAll(const bool &pVlanIsAllowedVlanAll)
	{
		m_pVlanIsAllowedVlanAll = pVlanIsAllowedVlanAll;
	}

	bool NsmPoIntfMessage::getPvlanIsAllowedVlanAll() const
	{
		return (m_pVlanIsAllowedVlanAll);
	}
		
	void  NsmPoIntfMessage::setPvlanRange (const UI32Range &pVlanRange)
    {
        m_pVlanRange = pVlanRange;
    }

    UI32Range  NsmPoIntfMessage::getPvlanRange () const
    {
        return (m_pVlanRange);
    }

	void  NsmPoIntfMessage ::setPvlanHostPrimVlanId(const UI32 &pVlanHostPrimVlanId)
    {
        m_pVlanHostPrimVlanId = pVlanHostPrimVlanId;
    }

   	UI32 NsmPoIntfMessage ::getPvlanHostPrimVlanId() const
    {
        return (m_pVlanHostPrimVlanId);
    }

	void NsmPoIntfMessage ::setPvlanHostSecVlanId(const UI32 &pVlanHostSecVlanId)
    {
		m_pVlanHostSecVlanId = pVlanHostSecVlanId;
    }

   	UI32 NsmPoIntfMessage ::getPvlanHostSecVlanId() const
    {
        return (m_pVlanHostSecVlanId);
    }
    void  NsmPoIntfMessage::setMtu(const UI32 &mtu)
    {
        m_mtu  =  mtu;
        m_bulkCmdCode |= NSMUPDATE_PO_MTU_CMD;
    }

    UI32  NsmPoIntfMessage::getMtu() const
    {
        return (m_mtu);
    }

    void  NsmPoIntfMessage::setVlanRange(const UI32Range &vlanRange)
    {
        m_vlanRange  =  vlanRange;
    }

    void  NsmPoIntfMessage::setVlanRange(const UI32Range &vlanRange, const UI32 type )
    {
        m_vlanRange  =  vlanRange;
        m_bulkCmdCode |= type;
    }

    UI32Range  NsmPoIntfMessage::getVlanRange() const
    {
        return (m_vlanRange);
    }

    void  NsmPoIntfMessage::setNativeVlanId(const UI32 &nativeVlanId)
    {
        m_nativeVlanId =  nativeVlanId;
        m_bulkCmdCode |= NSMUPDATE_PO_NATIVEVLANID_CMD;
    }

    UI32 NsmPoIntfMessage::getNativeVlanId() const
    {
        return (m_nativeVlanId);
    }
	
	void  NsmPoIntfMessage::setNativevlanDisable(const bool 
		&nativevlanDisable)
    {
        m_nativevlanDisable  =  nativevlanDisable;
        m_bulkCmdCode |= NSMUPDATE_PO_NATIVEVLANDISABLE_CMD;
    }

    bool  NsmPoIntfMessage::getNativevlanDisable() const
    {
        return (m_nativevlanDisable);
    }

    void  NsmPoIntfMessage::setIsPortProfiled(const bool &isPortProfiled)
    {
        m_isPortProfiled  =  isPortProfiled;
        m_bulkCmdCode |= NSMUPDATE_PO_PORT_PROFILED_CMD;
    }

    bool  NsmPoIntfMessage::getIsPortProfiled() const
    {
        return (m_isPortProfiled);
    }

    void NsmPoIntfMessage::setPortProfileName(const string &portProfileName) {
        m_portProfileName = portProfileName;
    }

    string NsmPoIntfMessage::getPortProfileName() const {
        return m_portProfileName;
    }

    void NsmPoIntfMessage::setPortProfileDomainName(const string &portProfileDomainName) {
        m_portProfileDomainName = portProfileDomainName;
    }

    string NsmPoIntfMessage::getPortProfileDomainName() const {
        return m_portProfileDomainName;
    }

    void  NsmPoIntfMessage::setBulkCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  NsmPoIntfMessage::getBulkCmdCode() const
    {
        return (m_cmdCode);
    }

    void  NsmPoIntfMessage::setRaGuard(const bool &raGuard)
    {
        m_raGuard = raGuard;
        m_bulkCmdCode |= NSMUPDATE_PO_RA_GUARD_CMD;
    }

    bool  NsmPoIntfMessage::getRaGuard() const
    {
        return (m_raGuard);
    }

    const void *NsmPoIntfMessage::getCStructureForInputs ()
    {
		if (
				(!IS_CMDCODE_SET(m_cmdCode, ELD_INTF_PO_CONF_PRIORITY))
				&&
				(
				 (!IS_CMDCODE_SET(m_cmdCode, ELD_INTF_PO_CONF_ELD_ENABLE))
				 &&
				 (!IS_CMDCODE_SET(m_cmdCode, ELD_INTF_PO_CONF_ELD_DISABLE))
				)
		   )

		{
        NsmPoIntfMsg_t *pInput = new NsmPoIntfMsg_t;

        strncpy (pInput->poId, m_poId.c_str (), 64);
		pInput->poId[63] = '\0';
        strncpy (pInput->poName, m_poName.c_str (), 64);
		pInput->poName[63] = '\0';
		strncpy (pInput->swVlanName, (char*)(m_vlanRange.toString()).c_str(),64);
	pInput->swVlanName[63] = '\0';	
        pInput->poIfIndex = m_poIfIndex;
        strncpy (pInput->desc, m_desc.c_str (), 64);
		pInput->desc[63] = '\0';
        pInput->flag     = m_flag;
        pInput->ignore_split     = m_ignore_split;
        pInput->swMode     = m_swMode;
        pInput->mtu     = m_mtu;
        pInput->isAllowedVlanAll = m_isAllowedVlanAll;
        pInput->poMode     = m_poMode;
        pInput->poType     = m_poType;
        pInput->poMinLinks     = m_poMinLinks;
		pInput->poAllowedSpeed    = m_poAllowedSpeed;
        pInput->poLoadBalanceMode     = m_poLoadBalanceMode;
        pInput->poPrimaryLinkDomainId = m_poPrimaryLinkDomainId;
        pInput->poPartnerSystemPriority = m_poPartnerSystemPriority;
        pInput->poPartnerKey = m_poPartnerKey;
        memcpy(pInput->poPartnerSystem, m_poPartnerSystem.c_str(), VLAG_LACP_GRP_ADDR_LEN);
        memcpy(pInput->poLacpSystemId, m_poLacpSystemId.c_str(), VLAG_LACP_GRP_ADDR_LEN);
        pInput->cmdCode     = m_cmdCode;
        pInput->bulkCmdCode     = m_bulkCmdCode;
        pInput->vlanList = NULL;

		pInput->pVlanHostPrimVlanId = m_pVlanHostPrimVlanId;
		pInput->pVlanHostSecVlanId = m_pVlanHostSecVlanId;
		pInput->pVlanNativeVlanId = m_pVlanNativeVlanId;

        if((m_cmdCode == NSMUPDATE_PO_SET_MAC_ADDRESS) || (m_cmdCode == NSMUPDATE_PO_DEL_MAC_ADDRESS))
		{
			unsigned char tempMac[MAC_ADDR_LENGTH_IN_BYTES] = {0};
			sscanf(m_mac.c_str(), "%4hx.%4hx.%4hx", (unsigned short *)&tempMac[0], (unsigned short *)&tempMac[2], (unsigned short *)&tempMac[4]);
			memcpy(pInput->mac, tempMac, MAC_ADDR_LENGTH_IN_BYTES);
		}

	if ( (pInput->cmdCode & NSMUPDATE_PO_SW_ADD_TRUNK_VLAN) ||
	     (pInput->cmdCode & NSMUPDATE_PO_SW_DELETE_TRUNK_VLAN) ||
	     (pInput->cmdCode & NSMUPDATE_PO_SW_EXCEPT_TRUNK_VLAN) ||
	     (pInput->cmdCode == NSMUPDATE_PO_SET_MAC_ADDRESS) ||
	     (pInput->cmdCode == NSMUPDATE_PO_DEL_MAC_ADDRESS) ||
	     (pInput->cmdCode == NSMUPDATE_PO_SET_MAC_GROUP) ||
	     (pInput->cmdCode == NSMUPDATE_PO_DEL_MAC_GROUP) ||
	     (pInput->cmdCode == NSMUPDATE_PO_SET_CTAG) ||
	     (pInput->cmdCode == NSMUPDATE_PO_DEL_CTAG) ||
	     ( (pInput->cmdCode & NSMUPDATE_PO_BULK_CONF) &&
	       ( (pInput->bulkCmdCode & NSMUPDATE_PO_SW_ADD_TRUNK_VLAN_CMD) ||
	         (pInput->bulkCmdCode & NSMUPDATE_PO_SW_DELETE_TRUNK_VLAN_CMD) ||
	         (pInput->bulkCmdCode & NSMUPDATE_PO_SW_EXCEPT_TRUNK_VLAN_CMD) ) ) ) {
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
	if ( (pInput->cmdCode & NSMUPDATE_PO_PVLAN_TRUNK_VLAN_ADD) ||
	     (pInput->cmdCode & NSMUPDATE_PO_PVLAN_TRUNK_VLAN_REMOVE) ||
	     (pInput->cmdCode & NSMUPDATE_PO_PVLAN_TRUNK_VLAN_EXCEPT)) { //PVLAN todo bulkCmdCode
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
		pInput->native_vlan_disable  = m_nativevlanDisable;
		pInput->native_vlan_id  = m_nativeVlanId;

        pInput->isPortProfiled = m_isPortProfiled;

        strncpy(pInput->portProfileName, m_portProfileName.c_str(), MAX_WYSEREA_PROFILE_NAME_LEN);
        pInput->portProfileName[MAX_WYSEREA_PROFILE_NAME_LEN] = '\0';

        strncpy(pInput->portProfileDomainName, m_portProfileDomainName.c_str(), MAX_WYSEREA_PROFILE_NAME_LEN);
        pInput->portProfileDomainName[MAX_WYSEREA_PROFILE_NAME_LEN] = '\0';

        pInput->raGuard = m_raGuard;
        pInput->ctagId = m_ctagId;
        pInput->macGroupId = m_macGroupId;
        pInput->raGuard = m_raGuard;
        return (pInput);
    }

        if (
            (IS_CMDCODE_SET(m_cmdCode, ELD_INTF_PO_CONF_PRIORITY))
                                        ||
                (
                    (IS_CMDCODE_SET(m_cmdCode, ELD_INTF_PO_CONF_ELD_ENABLE))
                                        ||
                    (IS_CMDCODE_SET(m_cmdCode, ELD_INTF_PO_CONF_ELD_DISABLE))
                )
           )
        {
            unsigned int i, size = 0;
            unsigned char *ptr = NULL;
            eld_mgmt_conf_req_t *eld_intf_config = NULL;

            size = sizeof(eld_mgmt_conf_req_t) + (m_vlanidList.size() * sizeof(uint16_t));
            ptr = (unsigned char *)calloc(1, size);
            if (ptr == NULL) {
                WaveNs::trace (TRACE_LEVEL_INFO, string(" Calloc failed for  size :"));
                return (eld_intf_config);
            }
            eld_intf_config = (eld_mgmt_conf_req_t *)&ptr[0];

            eld_intf_config->entry.intf_conf.cmd_code = m_cmdCode;
            printf("OP_CODE IS %d\n", eld_intf_config->entry.intf_conf.cmd_code);
            eld_intf_config->entry.intf_conf.po_info.poIfIndex = m_poIfIndex;
            printf("IFINDEX IS Ox%x\n", eld_intf_config->entry.intf_conf.po_info.poIfIndex);

            strncpy (eld_intf_config->entry.intf_conf.po_info.poId, m_poId.c_str (), 64);
            printf("PO_ID IS %s\n", eld_intf_config->entry.intf_conf.po_info.poId);

            if (IS_CMDCODE_SET(m_cmdCode, ELD_INTF_PO_CONF_PRIORITY))
                eld_intf_config->entry.intf_conf.priority = m_prio;
            printf("PRIORITY IS %d\n", eld_intf_config->entry.intf_conf.priority);
            if ((IS_CMDCODE_SET(m_cmdCode, ELD_INTF_PO_CONF_ELD_ENABLE))
                                            ||
                (IS_CMDCODE_SET(m_cmdCode, ELD_INTF_PO_CONF_ELD_DISABLE)))
            {
                if (!m_vlanidList.empty()) {
#if 0
                    // vlan_id is used for ELD_DISABLE only, although it's populated for both 
                    // ELD_ENABLE and ELD_DISABLE cases
                    eld_intf_config->entry.intf_conf.vlan_id = m_vlanidList[0];
                    printf("VLAN-ID IS %d\n", eld_intf_config->entry.intf_conf.vlan_id);
#endif
                    // vid and vlan_num are used for ELD_ENABLE only
                    eld_intf_config->entry.intf_conf.vlan_num = m_vlanidList.size();
                    printf("VLAN_NUM IS %d\n", eld_intf_config->entry.intf_conf.vlan_num);

                    if (eld_intf_config->entry.intf_conf.vlan_num) {
                        for (i = 0; i < eld_intf_config->entry.intf_conf.vlan_num; i++)
                        {
                            eld_intf_config->entry.intf_conf.vid[i] = m_vlanidList[i];
                            printf("getCStructureForInputs(): I = %d, VLAN_ID IS %d\n", i, eld_intf_config->entry.intf_conf.vid[i]);
                        }
                    }
                }
            }
            return (eld_intf_config);
		}
        return (NULL);
    }

    void NsmPoIntfMessage::setCtagId(const UI32 &ctagId)
    {
        m_ctagId = ctagId;
    }

    UI32 NsmPoIntfMessage::getCtagId() const
    {
        return m_ctagId;
    }

    void NsmPoIntfMessage::setMac(const string &mac)
    {
        m_mac = mac;
    }

    string NsmPoIntfMessage::getMac() const
    {
        return m_mac;
    }

    void NsmPoIntfMessage::setMacGroupId(const UI32 &macGroupId)
    {
        m_macGroupId = macGroupId;
    }

    UI32 NsmPoIntfMessage::getMacGroupId() const
    {
        return m_macGroupId;
    }

}
