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

/****************************************************************************
 *   Copyright (C) 2013 Brocade Communications Systems, Inc.				*
 *   All rights reserved.													*
 *   Author : Shivanand														*
 ****************************************************************************/

#include "APPM/Global/APPMMacBasedVlanClassificationMO.h"
#include "APPM/Global/APPMGlobalObjectManager.h"
#include "Nsm/Local/NsmTypes.h"
#include "vcs.h"

namespace DcmNs {

    APPMMacAddressVlanClassificationMO::APPMMacAddressVlanClassificationMO (APPMGlobalObjectManager *pAPPMGlobalObjectManager)
        : PrismElement (pAPPMGlobalObjectManager),
        PrismPersistableObject (APPMMacAddressVlanClassificationMO::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pAPPMGlobalObjectManager),
        DcmManagedObject (pAPPMGlobalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pAPPMGlobalObjectManager) {
        }

    APPMMacAddressVlanClassificationMO::~APPMMacAddressVlanClassificationMO () {
    }

    string APPMMacAddressVlanClassificationMO::getClassName() {
        return ("APPMMacAddressVlanClassificationMO");
    }

    void APPMMacAddressVlanClassificationMO::setupAttributesForPersistence() {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeUI32(&m_accessVlanId, "vlanId", appm_access_vlan_id));
        addPersistableAttribute (new AttributeString(&m_mac, "mac", appm_access_mac_address));
    }

    void APPMMacAddressVlanClassificationMO::setupAttributesForCreate() {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate (new AttributeUI32(&m_accessVlanId, "vlanId", appm_access_vlan_id));
        addPersistableAttributeForCreate (new AttributeString(&m_mac, "mac", appm_access_mac_address));

        vector<string> keyName;
        keyName.push_back ("vlanId");
        keyName.push_back ("mac");
        setUserDefinedKeyCombination (keyName);
    }

    void APPMMacAddressVlanClassificationMO::setAccessVlanId(const UI32 &accessVlanId) {
        m_accessVlanId = accessVlanId;
    }

    UI32 APPMMacAddressVlanClassificationMO::getAccessVlanId() const {
        return m_accessVlanId;
    }

    void APPMMacAddressVlanClassificationMO::setMac(const string &mac) {
        m_mac = mac;
    }

    string APPMMacAddressVlanClassificationMO::getMac() const {
        return m_mac;
    }

    APPMMacGroupVlanClassificationMO::APPMMacGroupVlanClassificationMO (APPMGlobalObjectManager *pAPPMGlobalObjectManager)
        : PrismElement (pAPPMGlobalObjectManager),
        PrismPersistableObject (APPMMacGroupVlanClassificationMO::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pAPPMGlobalObjectManager),
        DcmManagedObject (pAPPMGlobalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pAPPMGlobalObjectManager) {
        }


    APPMMacGroupVlanClassificationMO::~APPMMacGroupVlanClassificationMO () {
    }

    string APPMMacGroupVlanClassificationMO::getClassName() {
        return ("APPMMacGroupVlanClassificationMO");
    }

    void APPMMacGroupVlanClassificationMO::setupAttributesForPersistence() {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeUI32(&m_vlanId, "vlanId", appm_access_vlan_id));
        addPersistableAttribute (new AttributeUI32(&m_macGroupId, "macGroupId", appm_access_vlan_mac_group));
    }

    void APPMMacGroupVlanClassificationMO::setupAttributesForCreate() {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate (new AttributeUI32(&m_vlanId, "vlanId", appm_access_vlan_id));
        addPersistableAttributeForCreate (new AttributeUI32(&m_macGroupId, "macGroupId", appm_access_vlan_mac_group));

        vector<string> keyName;
        keyName.push_back ("vlanId");
        keyName.push_back ("macGroupId");
        setUserDefinedKeyCombination (keyName);
    }

    void APPMMacGroupVlanClassificationMO::setVlanId(const UI32 &vlanId) {
        m_vlanId = vlanId;
    }

    UI32 APPMMacGroupVlanClassificationMO::getVlanId() const {
        return m_vlanId;
    }

    void APPMMacGroupVlanClassificationMO::setMacGroupId(const UI32 &macGroupId) {
        m_macGroupId = macGroupId;
    }

    UI32 APPMMacGroupVlanClassificationMO::getMacGroupId() const {
        return m_macGroupId;
    }
}

