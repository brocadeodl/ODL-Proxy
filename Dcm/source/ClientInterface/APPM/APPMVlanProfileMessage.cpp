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
 *   Author : Kunjan Kumar Verma                                           *
 **************************************************************************/

#include "ClientInterface/APPM/APPMVlanProfileMessage.h"
#include "ClientInterface/APPM/APPMMessageDef.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"
#include "ClientInterface/Nsm/NsmUtils.h"

namespace DcmNs
{

    APPMVlanProfileMessage::APPMVlanProfileMessage ()
	    : DcmManagementInterfaceMessage (APPMVLANPROFILE)
    {
    }

    APPMVlanProfileMessage::APPMVlanProfileMessage (const UI8 &opCode,const UI8 &cmdCode,const string &profileName,const UI32 &profileId,const bool &swModeFlag,const UI8 &swMode,const bool &isAllowedVlanAll,const bool &isAllowedVlanNone,const UI32 &accessVlan,const UI32Range &vlanRange,const UI32 &vlanClassifierGroupId,const UI32 &vlanClassifierVlan)
	    : DcmManagementInterfaceMessage (APPMVLANPROFILE),
        m_opCode    (opCode),
        m_cmdCode    (cmdCode),
        m_profileName    (profileName),
        m_profileId    (profileId),
        m_swModeFlag    (swModeFlag),
        m_swMode    (swMode),
        m_isAllowedVlanAll    (isAllowedVlanAll),
        m_isAllowedVlanNone    (isAllowedVlanNone),
		m_accessVlan   (accessVlan),
        m_vlanRange    (vlanRange),
        m_vlanClassifierGroupId    (vlanClassifierGroupId),
        m_vlanClassifierVlan    (vlanClassifierVlan),
		m_nativeVlan	(NSM_NATIVE_VLAN_DEFAULT)
    {
    }

    APPMVlanProfileMessage::~APPMVlanProfileMessage ()
    {
    }

    void  APPMVlanProfileMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI8(&m_opCode,"opCode"));
        addSerializableAttribute (new AttributeUI8(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeString(&m_profileName,"profileName"));
        addSerializableAttribute (new AttributeUI32(&m_profileId,"profileId"));
        addSerializableAttribute (new AttributeBool(&m_swModeFlag,"swModeFlag"));
        addSerializableAttribute (new AttributeUI8(&m_swMode,"swMode"));
        addSerializableAttribute (new AttributeBool(&m_isAllowedVlanAll,"isAllowedVlanAll"));
        addSerializableAttribute (new AttributeBool(&m_isAllowedVlanNone,"isAllowedVlanNone"));
        addSerializableAttribute (new AttributeUI32(&m_accessVlan,"accessVlan"));
        addSerializableAttribute (new AttributeUI32Range(&m_vlanRange,"vlanRange"));
        addSerializableAttribute (new AttributeUI32(&m_vlanClassifierGroupId,"vlanClassifierGroupId"));
        addSerializableAttribute (new AttributeUI32(&m_vlanClassifierVlan,"vlanClassifierVlan"));
		addSerializableAttribute (new AttributeUI32(&m_nativeVlan,"nativeVlan"));
        addSerializableAttribute (new AttributeUI32(&m_trunkVlanId, "trunkVlanId"));
        addSerializableAttribute (new AttributeUI32(&m_vlanClassificationType, "vlanClassificationType"));
        addSerializableAttribute (new AttributeUI32(&m_ctag, "ctag"));
        addSerializableAttribute (new AttributeString(&m_mac, "mac"));
        addSerializableAttribute (new AttributeUI32(&m_macGroup, "macGroup"));
    }

    void  APPMVlanProfileMessage::setOpCode(const UI8 &opCode)
    {
        m_opCode  =  opCode;
    }

    UI8  APPMVlanProfileMessage::getOpCode() const
    {
        return (m_opCode);
    }

    void  APPMVlanProfileMessage::setCmdCode(const UI8 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI8  APPMVlanProfileMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  APPMVlanProfileMessage::setProfileName(const string &profileName)
    {
        m_profileName  =  profileName;
    }

    string  APPMVlanProfileMessage::getProfileName() const
    {
        return (m_profileName);
    }

    void  APPMVlanProfileMessage::setProfileId(const UI32 &profileId)
    {
        m_profileId  =  profileId;
    }

    UI32  APPMVlanProfileMessage::getProfileId() const
    {
        return (m_profileId);
    }

    void  APPMVlanProfileMessage::setSwModeFlag(const bool &swModeFlag)
    {
        m_swModeFlag  =  swModeFlag;
    }

    bool  APPMVlanProfileMessage::getSwModeFlag() const
    {
        return (m_swModeFlag);
    }

    void  APPMVlanProfileMessage::setSwMode(const UI8 &swMode)
    {
        m_swMode  =  swMode;
    }

    UI8  APPMVlanProfileMessage::getSwMode() const
    {
        return (m_swMode);
    }

    void  APPMVlanProfileMessage::setIsAllowedVlanAll(const bool &isAllowedVlanAll)
    {
        m_isAllowedVlanAll  =  isAllowedVlanAll;
    }

    bool  APPMVlanProfileMessage::getIsAllowedVlanAll() const
    {
        return (m_isAllowedVlanAll);
    }

    void  APPMVlanProfileMessage::setIsAllowedVlanNone(const bool &isAllowedVlanNone)
    {
        m_isAllowedVlanNone  =  isAllowedVlanNone;
    }

    bool  APPMVlanProfileMessage::getIsAllowedVlanNone() const
    {
        return (m_isAllowedVlanNone);
    }

    void  APPMVlanProfileMessage::setAccessVlan(const UI32 &accessVlan)
    {
        m_accessVlan  =  accessVlan;
    }

    UI32  APPMVlanProfileMessage::getAccessVlan() const
    {
        return (m_accessVlan);
    }

    void  APPMVlanProfileMessage::setVlanRange(const UI32Range &vlanRange)
    {
        m_vlanRange  =  vlanRange;
    }

    UI32Range  APPMVlanProfileMessage::getVlanRange() const
    {
        return (m_vlanRange);
    }

    void  APPMVlanProfileMessage::setVlanClassifierGroupId(const UI32 &vlanClassifierGroupId)
    {
        m_vlanClassifierGroupId  =  vlanClassifierGroupId;
    }

    UI32  APPMVlanProfileMessage::getVlanClassifierGroupId() const
    {
        return (m_vlanClassifierGroupId);
    }

    void  APPMVlanProfileMessage::setVlanClassifierVlan(const UI32 &vlanClassifierVlan)
    {
        m_vlanClassifierVlan  =  vlanClassifierVlan;
    }

    UI32  APPMVlanProfileMessage::getVlanClassifierVlan() const
    {
        return (m_vlanClassifierVlan);
    }

	void  APPMVlanProfileMessage::setNativeVlanId(const UI32 &nativeVlan)
	{
		m_nativeVlan = nativeVlan;
	}

	UI32 APPMVlanProfileMessage::getNativeVlanId() const
	{
		return (m_nativeVlan);
	}
    void APPMVlanProfileMessage::setTrunkVlanId(const UI32 &trunkVlanId) {
        m_trunkVlanId = trunkVlanId;
    }
    UI32 APPMVlanProfileMessage::getTrunkVlanId() const {
        return m_trunkVlanId;
    }
    void APPMVlanProfileMessage::setVlanClassificationType(const UI32 &vlanClassificationType) {
        m_vlanClassificationType = vlanClassificationType;
    }
    UI32 APPMVlanProfileMessage::getVlanClassificationType() const {
        return m_vlanClassificationType;
    }   
    void APPMVlanProfileMessage::setCtag(const UI32 &ctag) {
        m_ctag = ctag;
    }
    UI32 APPMVlanProfileMessage::getCtag() const{
        return m_ctag;
    }
    void APPMVlanProfileMessage::setMac(const string &mac){
        m_mac = mac;
    }
    string APPMVlanProfileMessage::getMac() const {
        return m_mac;
    }
    void APPMVlanProfileMessage::setMacGroup(const UI32 &macGroup) {
        m_macGroup = macGroup;
    }
    UI32 APPMVlanProfileMessage::getMacGroup() const {
        return m_macGroup;
    }

    const void *APPMVlanProfileMessage::getCStructureForInputs ()
    {
        vlanProfileMsg_t *pInput = (vlanProfileMsg_t *) malloc(sizeof(vlanProfileMsg_t)/* + 
            ((m_numberOfVlanClassifications-1)*sizeof(APPMVlanClassificationValue_t))*/);
        unsigned int profileNameLen = strlen(m_profileName.c_str ());
	    pInput->opCode = m_opCode; 
	    pInput->cmdCode = m_cmdCode; 
        strncpy (pInput->profileName, m_profileName.c_str (), profileNameLen);
		pInput->profileName[profileNameLen] = '\0';
	    pInput->profileId = m_profileId; 
	    pInput->m_swModeFlag = (unsigned char)m_swModeFlag; 
	    pInput->m_swMode = m_swMode;
		pInput->m_isAllowedVlanAll = (unsigned char)m_isAllowedVlanAll;
		pInput->m_isAllowedVlanNone = (unsigned char)m_isAllowedVlanNone;
		pInput->m_accessVlan = m_accessVlan;
        pInput->m_vlanRange = (char*)(m_vlanRange.toString()).c_str();
		pInput->m_vlanClassifierGroupId = m_vlanClassifierGroupId;
		pInput->m_vlanClassifierVlan = m_vlanClassifierVlan;
		pInput->m_nativeVlan = m_nativeVlan;
        pInput->trunkVlanId = m_trunkVlanId;
        pInput->vlanClassificationType = (APPMVlanClassificationType_t)m_vlanClassificationType;
        pInput->numberOfVlanClassifications = 1;
            //Need to handle all classifications per vlan in postboot.
            //In config path numberOfVlanClassifications is 1.

        switch (m_vlanClassificationType) 
        {
            case APPM_VLAN_NOCLASSIFICATION : 
                {
                    break;
                }
            case APPM_VLAN_CTAGCLASSIFICATION :
                {
                    pInput->vlanClassificationRecords[0].ctag.ctag = m_ctag;
                    break;
                }
            case APPM_VLAN_MACCLASSIFICATION :
                {
                    snprintf(pInput->vlanClassificationRecords[0].mac.macAddress,
                        MAC_ADDR_STR_LEN,m_mac.c_str());
                    break;
                }
            case APPM_VLAN_MACGROUPCLASSIFICATION :
                {
                    pInput->vlanClassificationRecords[0].macGroup.macGroupId = m_macGroup;
                    break;
                }
            case APPM_VLAN_VNICLASSIFICATION :
                {
                    //pInput->vlanClassificationRecords[0].vni.vni = m_vni;
                    break;
                }
            default : 
                {
                    break;
                }
        }


        //PRINT
        /*trace (TRACE_LEVEL_INFO, string("APPMVlanProfileMessage::getCStructureForInputs: OpCode<") +  pInput->opCode + ">\n");
        trace (TRACE_LEVEL_INFO, string("APPMVlanProfileMessage::getCStructureForInputs: cmdCode<") + pInput->cmdCode + ">\n");
        trace (TRACE_LEVEL_INFO, string("APPMVlanProfileMessage::getCStructureForInputs: PPName<") +  pInput->profileName + ">\n");
        trace (TRACE_LEVEL_INFO, string("APPMVlanProfileMessage::getCStructureForInputs: PPID<") +  pInput->profileId + ">\n");
        trace (TRACE_LEVEL_INFO, string("APPMVlanProfileMessage::getCStructureForInputs: SwModeFlag<") +  pInput->m_swModeFlag + ">\n");
        trace (TRACE_LEVEL_INFO, string("APPMVlanProfileMessage::getCStructureForInputs: SwMode<") +  pInput->m_swMode + ">\n");
        trace (TRACE_LEVEL_INFO, string("APPMVlanProfileMessage::getCStructureForInputs: IsTrunkAll<") +  pInput->m_isAllowedVlanAll + ">\n");
        trace (TRACE_LEVEL_INFO, string("APPMVlanProfileMessage::getCStructureForInputs: IsAllowedTNone<") +  pInput->m_isAllowedVlanNone + ">\n");
        trace (TRACE_LEVEL_INFO, string("APPMVlanProfileMessage::getCStructureForInputs: AccesVlanId<") +  pInput->m_accessVlan + ">\n");
        trace (TRACE_LEVEL_INFO, string("APPMVlanProfileMessage::getCStructureForInputs: TVlanRange<") +  pInput->m_vlanRange + ">\n");
        trace (TRACE_LEVEL_INFO, string("APPMVlanProfileMessage::getCStructureForInputs: MacGroupId<") +  pInput->m_vlanClassifierGroupId + ">\n");
        trace (TRACE_LEVEL_INFO, string("APPMVlanProfileMessage::getCStructureForInputs: VlanClassId<") +  pInput->m_vlanClassifierVlan + ">\n");
        trace (TRACE_LEVEL_INFO, string("APPMVlanProfileMessage::getCStructureForInputs: NativeVlanId<") +  pInput->m_nativeVlan + ">\n");
        trace (TRACE_LEVEL_INFO, string("APPMVlanProfileMessage::getCStructureForInputs: TrunkVlanId<") +  pInput->trunkVlanId + ">\n");
        trace (TRACE_LEVEL_INFO, string("APPMVlanProfileMessage::getCStructureForInputs: VlanClassificationType<") +  pInput->vlanClassificationType + ">\n");
        trace (TRACE_LEVEL_INFO, string("APPMVlanProfileMessage::getCStructureForInputs: NumberOfVlanClassifications<") +  pInput->numberOfVlanClassifications + ">\n");

        switch(pInput->vlanClassificationType) {
            case APPM_VLAN_NOCLASSIFICATION :
                {
                    printf("NO VLAN CLASSIFICATION.");
                    break;
                }
            case APPM_VLAN_CTAGCLASSIFICATION :
                {
                    trace (TRACE_LEVEL_INFO, string("APPMVlanProfileMessage::getCStructureForInputs: CTAG <") 
                            + pInput->vlanClassificationRecords[0].ctag.ctag + ">\n");
                    break;
                }
            case APPM_VLAN_MACCLASSIFICATION :
                {
                    trace (TRACE_LEVEL_INFO, string("APPMVlanProfileMessage::getCStructureForInputs: MAC <")
                            + pInput->vlanClassificationRecords[0].mac.macAddress + ">\n");
                    break;
                }
            case APPM_VLAN_MACGROUPCLASSIFICATION :
                {
                    trace (TRACE_LEVEL_INFO, string("APPMVlanProfileMessage::getCStructureForInputs: MACGROUP <")
                            +  pInput->vlanClassificationRecords[0].macGroup.macGroupId + ">\n");
                    break;
                }
            case APPM_VLAN_VNICLASSIFICATION :
                {
                    printf("VNI");
                    break;
                }
        }*/









        return (pInput);
    }

	void APPMVlanProfileMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
	}
}
