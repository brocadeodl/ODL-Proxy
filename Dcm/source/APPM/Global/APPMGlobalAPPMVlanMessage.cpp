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
 *   Author : kverma                                                     *
 **************************************************************************/

#include "APPM/Global/APPMGlobalAPPMVlanMessage.h"
#include "APPM/Global/APPMGlobalObjectManager.h"
#include "APPM/Global/APPMGlobalTypes.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"
#include "ClientInterface/APPM/APPMMessageDef.h"
namespace DcmNs
{

    APPMGlobalAPPMVlanMessage::APPMGlobalAPPMVlanMessage ()
        : ManagementInterfaceMessage (APPMGlobalObjectManager::getClassName (),APPMGLOBALAPPMVLAN)
    {
		m_nativeVlan = APPM_NSM_NATIVE_VLAN_DEFAULT;
    }

    APPMGlobalAPPMVlanMessage::APPMGlobalAPPMVlanMessage (const UI8 &opCode,const UI8 &cmdCode,const string &profileName,const UI32 &profileId,const bool &swModeFlag,const UI8 &swMode,const bool &isChangeVlanList,const bool &isAllowedVlanAll,const bool &isAllowedVlanNone,const UI32 &accessVlan,const UI32Range &vlanRange,const UI32 &vlanClassifierGroupId,const UI32 &vlanClassifierVlan)
        : ManagementInterfaceMessage (APPMGlobalObjectManager::getClassName (),APPMGLOBALAPPMVLAN),
        m_opCode    (opCode),
        m_cmdCode    (cmdCode),
        m_profileName    (profileName),
        m_profileId    (profileId),
        m_swModeFlag    (swModeFlag),
        m_swMode    (swMode),
        m_isChangeVlanList    (isChangeVlanList),
        m_isAllowedVlanAll    (isAllowedVlanAll),
        m_isAllowedVlanNone    (isAllowedVlanNone),
		m_accessVlan   (accessVlan),
        m_vlanRange    (vlanRange),
        m_vlanClassifierGroupId    (vlanClassifierGroupId),
        m_vlanClassifierVlan    (vlanClassifierVlan),
		m_nativeVlan	(APPM_NSM_NATIVE_VLAN_DEFAULT)
    {
    }

    APPMGlobalAPPMVlanMessage::~APPMGlobalAPPMVlanMessage ()
    {
    }

    void  APPMGlobalAPPMVlanMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI8(&m_opCode,"opCode"));
        addSerializableAttribute (new AttributeUI8(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeString(&m_profileName,"profileName"));
        addSerializableAttribute (new AttributeUI32(&m_profileId,"profileId"));
        addSerializableAttribute (new AttributeBool(&m_swModeFlag,"swModeFlag"));
        addSerializableAttribute (new AttributeUI8(&m_swMode,"swMode"));
        addSerializableAttribute (new AttributeBool(&m_isChangeVlanList,"isChangeVlanList"));
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

    void  APPMGlobalAPPMVlanMessage::setOpCode(const UI8 &opCode)
    {
        m_opCode  =  opCode;
    }

    UI8  APPMGlobalAPPMVlanMessage::getOpCode() const
    {
        return (m_opCode);
    }

    void  APPMGlobalAPPMVlanMessage::setCmdCode(const UI8 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI8  APPMGlobalAPPMVlanMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  APPMGlobalAPPMVlanMessage::setProfileName(const string &profileName)
    {
        m_profileName  =  profileName;
    }

    string  APPMGlobalAPPMVlanMessage::getProfileName() const
    {
        return (m_profileName);
    }

    void  APPMGlobalAPPMVlanMessage::setProfileId(const UI32 &profileId)
    {
        m_profileId  =  profileId;
    }

    UI32  APPMGlobalAPPMVlanMessage::getProfileId() const
    {
        return (m_profileId);
    }

    void  APPMGlobalAPPMVlanMessage::setSwModeFlag(const bool &swModeFlag)
    {
        m_swModeFlag  =  swModeFlag;
    }

    bool  APPMGlobalAPPMVlanMessage::getSwModeFlag() const
    {
        return (m_swModeFlag);
    }

    void  APPMGlobalAPPMVlanMessage::setSwMode(const UI8 &swMode)
    {
        m_swMode  =  swMode;
    }

    UI8  APPMGlobalAPPMVlanMessage::getSwMode() const
    {
        return (m_swMode);
    }

    void  APPMGlobalAPPMVlanMessage::setIsChangeVlanList(const bool &isChangeVlanList)
    {
        m_isChangeVlanList  =  isChangeVlanList;
    }

    bool  APPMGlobalAPPMVlanMessage::getIsChangeVlanList() const
    {
        return (m_isChangeVlanList);
    }

    void  APPMGlobalAPPMVlanMessage::setIsAllowedVlanAll(const bool &isAllowedVlanAll)
    {
        m_isAllowedVlanAll  =  isAllowedVlanAll;
    }

    bool  APPMGlobalAPPMVlanMessage::getIsAllowedVlanAll() const
    {
        return (m_isAllowedVlanAll);
    }

    void  APPMGlobalAPPMVlanMessage::setIsAllowedVlanNone(const bool &isAllowedVlanNone)
    {
        m_isAllowedVlanNone  =  isAllowedVlanNone;
    }

    bool  APPMGlobalAPPMVlanMessage::getIsAllowedVlanNone() const
    {
        return (m_isAllowedVlanNone);
    }
	
    void  APPMGlobalAPPMVlanMessage::setAccessVlan(const UI32 &accessVlan)
    {
        m_accessVlan  =  accessVlan;
    }

    UI32  APPMGlobalAPPMVlanMessage::getAccessVlan() const
    {
        return (m_accessVlan);
    }

    void  APPMGlobalAPPMVlanMessage::setVlanRange(const UI32Range &vlanRange)
    {
        m_vlanRange  =  vlanRange;
    }

    UI32Range  APPMGlobalAPPMVlanMessage::getVlanRange() const
    {
        return (m_vlanRange);
    }

    void  APPMGlobalAPPMVlanMessage::setVlanClassifierGroupId(const UI32 &vlanClassifierGroupId)
    {
        m_vlanClassifierGroupId  =  vlanClassifierGroupId;
    }

    UI32  APPMGlobalAPPMVlanMessage::getVlanClassifierGroupId() const
    {
        return (m_vlanClassifierGroupId);
    }

    void  APPMGlobalAPPMVlanMessage::setVlanClassifierVlan(const UI32 &vlanClassifierVlan)
    {
        m_vlanClassifierVlan  =  vlanClassifierVlan;
    }

    UI32  APPMGlobalAPPMVlanMessage::getVlanClassifierVlan() const
    {
        return (m_vlanClassifierVlan);
    }

    void  APPMGlobalAPPMVlanMessage::setNativeVlanId(const UI32 &nativeVlan)
    {
        m_nativeVlan = nativeVlan;
    }

    UI32 APPMGlobalAPPMVlanMessage::getNativeVlanId() const
    {
        return (m_nativeVlan);
    }

    void APPMGlobalAPPMVlanMessage::setTrunkVlanId(const UI32 &trunkVlanId) {
        m_trunkVlanId = trunkVlanId;
    }
    UI32 APPMGlobalAPPMVlanMessage::getTrunkVlanId() const {
        return m_trunkVlanId;
    }
    void APPMGlobalAPPMVlanMessage::setVlanClassificationType(const UI32 &vlanClassificationType) {
        m_vlanClassificationType = vlanClassificationType;
    }
    UI32 APPMGlobalAPPMVlanMessage::getVlanClassificationType() const {
        return m_vlanClassificationType;
    }
    void APPMGlobalAPPMVlanMessage::setCtag(const UI32 &ctag) {
        m_ctag = ctag;
    }
    UI32 APPMGlobalAPPMVlanMessage::getCtag() const{
        return m_ctag;
    }
    void APPMGlobalAPPMVlanMessage::setMac(const string &mac){
        m_mac = mac;
    }
    string APPMGlobalAPPMVlanMessage::getMac() const {
        return m_mac;
    }
    void APPMGlobalAPPMVlanMessage::setMacGroup(const UI32 &macGroup) {
        m_macGroup = macGroup;
    }
    UI32 APPMGlobalAPPMVlanMessage::getMacGroup() const {
        return m_macGroup;
    }
}
