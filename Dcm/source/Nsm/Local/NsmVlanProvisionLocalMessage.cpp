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

/* **
 * NsmVlanProvisionLocalMessage.cpp
 *
 * Jun 27, 2013
 * Copyright (C) 2013 Brocade Communications Systems, Inc.
 * All rights reserved.
 */

#include "Nsm/Local/NsmVlanProvisionLocalMessage.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/NsmTypes.h"
#include "Framework/Attributes/AttributeSerializableObject.h"

namespace DcmNs {

NsmVlanProvisionLocalMessage::NsmVlanProvisionLocalMessage ()
    : ManagementInterfaceMessage (NsmLocalObjectManager::getClassName(), NSM_LOCAL_VLAN_PROVISION_MSG)
    , m_unprovisionVlans         ( )
    , m_provisionVlans           ( )
    , m_needsExplicitUnprovision ( false )
    , m_triggerVlanUnprovision ( false )
{
}

NsmVlanProvisionLocalMessage::~NsmVlanProvisionLocalMessage ()
{
}

void NsmVlanProvisionLocalMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization();
    addSerializableAttribute(new AttributeUI32Range(&m_unprovisionVlans, "unprovList"));
    addSerializableAttribute(new AttributeUI32Range(&m_provisionVlans, "provList"));
    addSerializableAttribute(new AttributeBool(&m_needsExplicitUnprovision, "needsUnprov"));
    addSerializableAttribute(new AttributeBool(&m_triggerVlanUnprovision, "triggerVlanUnprov"));
}

void NsmVlanProvisionLocalMessage::setUnprovisionVlans (const UI32Range &gvids) {
    m_unprovisionVlans = gvids;
}

UI32Range NsmVlanProvisionLocalMessage::getUnprovisionVlans () const {
    return m_unprovisionVlans;
}

bool NsmVlanProvisionLocalMessage::hasUnprovisionVlans () const {
    return !m_unprovisionVlans.isEmpty();
}

void NsmVlanProvisionLocalMessage::setProvisionVlans (const UI32Range &gvids) {
    m_provisionVlans = gvids;
}

UI32Range NsmVlanProvisionLocalMessage::getProvisionVlans () const {
    return m_provisionVlans;
}

bool NsmVlanProvisionLocalMessage::hasProvisionVlans () const {
    return !m_provisionVlans.isEmpty();
}

void NsmVlanProvisionLocalMessage::setNeedsExplicitUnprovision (bool flag) {
    m_needsExplicitUnprovision = flag;
}

bool NsmVlanProvisionLocalMessage::getNeedsExplicitUnprovision () const {
    return m_needsExplicitUnprovision;
}

bool NsmVlanProvisionLocalMessage::isTriggerVlanUnprovision() const {
    return m_triggerVlanUnprovision;
}

void NsmVlanProvisionLocalMessage::setTriggerVlanUnprovision(
        bool triggerVlanUnprovision) {
    m_triggerVlanUnprovision = triggerVlanUnprovision;
}

}
