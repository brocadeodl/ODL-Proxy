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

 /**************************************************************************
 *   Copyright (C) 2005-2009 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Jitendra Singh		                                       *
 ***************************************************************************/

#include "VcsFabric/Local/VcsEnqSwitchMessage.h"
#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"
#include "VcsFabric/Local/VcsNodeLocalTypes.h"
#include "Framework/Attributes/AttributeSerializableObject.cpp"

namespace DcmNs
{

VcsEnqSwitchMessage::VcsEnqSwitchMessage ()
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_ENQ_SWITCH)
{
}

VcsEnqSwitchMessage::VcsEnqSwitchMessage (const string &className, const UI32 &messageOpcode)
    : ManagementInterfaceMessage (className, messageOpcode)
{
}

VcsEnqSwitchMessage::~VcsEnqSwitchMessage ()
{
}

void VcsEnqSwitchMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

 	addSerializableAttribute (new AttributeSerializableObject<VcsFabricLocalSwitchInfoObject> (&m_vcsFabricLocalSwitchInfoObject,       "vcsFabricLocalSwitchInfoObject"));
 	addSerializableAttribute (new AttributeUI32                                               (&m_domainId,                             "domainId"));
 	//addSerializableAttribute (new AttributeSerializableObject<NodeAutoUpgradeConfiguration> (&m_NodeAutoUpgradeConfiguration, "NodeAutoUpgradeConfiguration"));
 	addSerializableAttribute (new AttributeUI32                                               (&m_autoupgradeStatus,                    "autoupgradeStatus"));
}

void VcsEnqSwitchMessage::setRemoteVcsSwitchInfoObject (const VcsFabricLocalSwitchInfoObject &vcsFabricLocalSwitchInfoObject)
{
    m_vcsFabricLocalSwitchInfoObject = vcsFabricLocalSwitchInfoObject;
}

VcsFabricLocalSwitchInfoObject VcsEnqSwitchMessage::getRemoteVcsSwitchInfoObject () const
{
    return (m_vcsFabricLocalSwitchInfoObject);
}

UI32 VcsEnqSwitchMessage::getRemoteNodeDomainId () const
{
    return (m_domainId);
}

void VcsEnqSwitchMessage::setRemoteNodeDomainId (const UI32 &domainId)
{
    m_domainId = domainId;
}

/*void VcsEnqSwitchMessage::setNodeAutoUpgradeConfiguration(const NodeAutoUpgradeConfiguration &UpgradeConfigData)
{
	
	
    m_NodeAutoUpgradeConfiguration = UpgradeConfigData;
}

NodeAutoUpgradeConfiguration VcsEnqSwitchMessage::getNodeAutoUpgradeConfiguration() const
{
    return (m_NodeAutoUpgradeConfiguration);
}*/

UI32 VcsEnqSwitchMessage::getAutoUpgradeStatus() const
{
    return (m_autoupgradeStatus);
}

void VcsEnqSwitchMessage::setAutoUpgradeStatus(const UI32 &autoupgradeStatus)
{
    m_autoupgradeStatus = autoupgradeStatus;
}

}
