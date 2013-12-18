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

#include "APPM/Global/APPMCtagBasedVlanClassificationMO.h"
#include "Nsm/Local/NsmTypes.h"
#include "vcs.h"

namespace DcmNs {

    APPMVlanClassificationMO::APPMVlanClassificationMO (WaveObjectManager *pWaveObjectManager)
        : PrismElement (pWaveObjectManager),
        PrismPersistableObject (APPMVlanClassificationMO::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pWaveObjectManager),
        DcmManagedObject (pWaveObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pWaveObjectManager) {
        }

    APPMVlanClassificationMO::~APPMVlanClassificationMO() {
    }

    string APPMVlanClassificationMO::getClassName() {
        return ("APPMVlanClassificationMO");
    }

    void APPMVlanClassificationMO::setupAttributesForPersistence() {
        DcmLocalManagedObject::setupAttributesForPersistence();
        addPersistableAttribute (new AttributeUI32(&m_vlanId, "vlanId", appm_trunk_vlan_id));
        addPersistableAttribute (new AttributeUI32(&m_ctagId, "ctagId", appm_trunk_ctag_id));
    }

    void APPMVlanClassificationMO::setupAttributesForCreate() {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate (new AttributeUI32(&m_vlanId, "vlanId", appm_trunk_vlan_id));
        addPersistableAttributeForCreate (new AttributeUI32(&m_ctagId, "ctagId", appm_trunk_ctag_id));
    }

    void APPMVlanClassificationMO::setupKeys () {
        vector<string> keyName;
        keyName.push_back ("vlanId");
        keyName.push_back ("ctagId");
        setUserDefinedKeyCombination (keyName);
    }

    void APPMVlanClassificationMO::setVlanId(const UI32 &vlanId) {
        m_vlanId  =  vlanId;
    }

    UI32 APPMVlanClassificationMO::getVlanId() const {
        return m_vlanId;
    }

    void APPMVlanClassificationMO::setCtagId(const UI32 &ctagId) {
        m_ctagId = ctagId;
    }

    UI32 APPMVlanClassificationMO::getCtagId() const {
        return m_ctagId;
    }

    APPMCtagVlanClassificationMO::APPMCtagVlanClassificationMO (WaveObjectManager *pWaveObjectManager)
        : PrismElement (pWaveObjectManager),
        PrismPersistableObject (APPMCtagVlanClassificationMO::getClassName (), APPMVlanClassificationMO::getClassName ()),
        WaveManagedObject (pWaveObjectManager),
        DcmManagedObject (pWaveObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pWaveObjectManager),
        APPMVlanClassificationMO (pWaveObjectManager) {
        }

    APPMCtagVlanClassificationMO::~APPMCtagVlanClassificationMO() {
    }

    string APPMCtagVlanClassificationMO::getClassName() {
        return ("APPMCtagVlanClassificationMO");
    }

    void APPMCtagVlanClassificationMO::setupAttributesForPersistence() {
        APPMVlanClassificationMO::setupAttributesForPersistence();
    }

    void  APPMCtagVlanClassificationMO::setupAttributesForCreate() {
    }

    APPMCtagVlanClassificationRemoveMO::APPMCtagVlanClassificationRemoveMO (WaveObjectManager *pWaveObjectManager)
        : PrismElement (pWaveObjectManager),
        PrismPersistableObject (APPMCtagVlanClassificationRemoveMO::getClassName (), APPMVlanClassificationMO::getClassName ()),
        WaveManagedObject (pWaveObjectManager),
        DcmManagedObject (pWaveObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pWaveObjectManager),
        APPMVlanClassificationMO (pWaveObjectManager) {
        }

    APPMCtagVlanClassificationRemoveMO::~APPMCtagVlanClassificationRemoveMO() {
    }

    string APPMCtagVlanClassificationRemoveMO::getClassName() {
        return ("APPMCtagVlanClassificationRemoveMO");
    }

    void APPMCtagVlanClassificationRemoveMO::setupAttributesForPersistence() {
        APPMVlanClassificationMO::setupAttributesForPersistence();
    }

    void  APPMCtagVlanClassificationRemoveMO::setupAttributesForCreate() {
    }

}
