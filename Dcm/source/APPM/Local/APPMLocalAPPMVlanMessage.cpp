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

#include "APPM/Local/APPMLocalObjectManager.h"
#include "APPM/Local/APPMLocalAPPMVlanMessage.h"
#include "APPM/Local/APPMLocalTypes.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"

namespace DcmNs
{

    APPMLocalAPPMVlanMessage::APPMLocalAPPMVlanMessage ()
        : PrismMessage (APPMLocalObjectManager::getPrismServiceId (),APPMLOCALAPPMVLAN)
    {
    }

    APPMLocalAPPMVlanMessage::APPMLocalAPPMVlanMessage (const UI8 &opCode,const UI8 &cmdCode,const string &profileName,const UI32 &profileId,const bool &swModeFlag,const UI8 &swMode,const bool &isAllowedVlanAll,const bool &isAllowedVlanNone,const UI32 &accessVlan,const UI32Range &vlanRange,const UI32 &vlanClassifierGroupId,const UI32 &vlanClassifierVlan)
        : PrismMessage (APPMLocalObjectManager::getPrismServiceId (),APPMLOCALAPPMVLAN),
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
		m_nativeVlan(NSM_NATIVE_VLAN_DEFAULT)
    {
    }

    APPMLocalAPPMVlanMessage::~APPMLocalAPPMVlanMessage ()
    {
    }

    void  APPMLocalAPPMVlanMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
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

    void  APPMLocalAPPMVlanMessage::setOpCode(const UI8 &opCode)
    {
        m_opCode  =  opCode;
    }

    UI8  APPMLocalAPPMVlanMessage::getOpCode() const
    {
        return (m_opCode);
    }

    void  APPMLocalAPPMVlanMessage::setCmdCode(const UI8 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI8  APPMLocalAPPMVlanMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  APPMLocalAPPMVlanMessage::setProfileName(const string &profileName)
    {
        m_profileName  =  profileName;
    }

    string  APPMLocalAPPMVlanMessage::getProfileName() const
    {
        return (m_profileName);
    }

    void  APPMLocalAPPMVlanMessage::setProfileId(const UI32 &profileId)
    {
        m_profileId  =  profileId;
    }

    UI32  APPMLocalAPPMVlanMessage::getProfileId() const
    {
        return (m_profileId);
    }

    void  APPMLocalAPPMVlanMessage::setSwModeFlag(const bool &swModeFlag)
    {
        m_swModeFlag  =  swModeFlag;
    }

    bool  APPMLocalAPPMVlanMessage::getSwModeFlag() const
    {
        return (m_swModeFlag);
    }

    void  APPMLocalAPPMVlanMessage::setSwMode(const UI8 &swMode)
    {
        m_swMode  =  swMode;
    }

    UI8  APPMLocalAPPMVlanMessage::getSwMode() const
    {
        return (m_swMode);
    }

    void  APPMLocalAPPMVlanMessage::setIsAllowedVlanAll(const bool &isAllowedVlanAll)
    {
        m_isAllowedVlanAll  =  isAllowedVlanAll;
    }

    bool  APPMLocalAPPMVlanMessage::getIsAllowedVlanAll() const
    {
        return (m_isAllowedVlanAll);
    }

    void  APPMLocalAPPMVlanMessage::setIsAllowedVlanNone(const bool &isAllowedVlanNone)
    {
        m_isAllowedVlanNone  =  isAllowedVlanNone;
    }

    bool  APPMLocalAPPMVlanMessage::getIsAllowedVlanNone() const
    {
        return (m_isAllowedVlanNone);
    }

    void  APPMLocalAPPMVlanMessage::setAccessVlan(const UI32 &accessVlan)
    {
        m_accessVlan  =  accessVlan;
    }

    UI32  APPMLocalAPPMVlanMessage::getAccessVlan() const
    {
        return (m_accessVlan);
    }

    void  APPMLocalAPPMVlanMessage::setVlanRange(const UI32Range &vlanRange)
    {
        m_vlanRange  =  vlanRange;
    }

    UI32Range  APPMLocalAPPMVlanMessage::getVlanRange() const
    {
        return (m_vlanRange);
    }

    void  APPMLocalAPPMVlanMessage::setVlanClassifierGroupId(const UI32 &vlanClassifierGroupId)
    {
        m_vlanClassifierGroupId  =  vlanClassifierGroupId;
    }

    UI32  APPMLocalAPPMVlanMessage::getVlanClassifierGroupId() const
    {
        return (m_vlanClassifierGroupId);
    }

    void  APPMLocalAPPMVlanMessage::setVlanClassifierVlan(const UI32 &vlanClassifierVlan)
    {
        m_vlanClassifierVlan  =  vlanClassifierVlan;
    }

    UI32  APPMLocalAPPMVlanMessage::getVlanClassifierVlan() const
    {
        return (m_vlanClassifierVlan);
    }

	void  APPMLocalAPPMVlanMessage::setNativeVlanId(const UI32 &nativeVlan)
	{
		m_nativeVlan = nativeVlan;
	}

	UI32 APPMLocalAPPMVlanMessage::getNativeVlanId() const
	{
		return (m_nativeVlan);
    }
    void APPMLocalAPPMVlanMessage::setTrunkVlanId(const UI32 &trunkVlanId) {
        m_trunkVlanId = trunkVlanId;
    }
    UI32 APPMLocalAPPMVlanMessage::getTrunkVlanId() const {
        return m_trunkVlanId;
    }
    void APPMLocalAPPMVlanMessage::setVlanClassificationType(const UI32 &vlanClassificationType) {
        m_vlanClassificationType = vlanClassificationType;
    }
    UI32 APPMLocalAPPMVlanMessage::getVlanClassificationType() const {
        return m_vlanClassificationType;
    }
    void APPMLocalAPPMVlanMessage::setCtag(const UI32 &ctag) {
        m_ctag = ctag;
    }
    UI32 APPMLocalAPPMVlanMessage::getCtag() const{
        return m_ctag;
    }
    void APPMLocalAPPMVlanMessage::setMac(const string &mac){
        m_mac = mac;
    }
    string APPMLocalAPPMVlanMessage::getMac() const {
        return m_mac;
    }
    void APPMLocalAPPMVlanMessage::setMacGroup(const UI32 &macGroup) {
        m_macGroup = macGroup;
    }
    UI32 APPMLocalAPPMVlanMessage::getMacGroup() const {
        return m_macGroup;
    }
}
