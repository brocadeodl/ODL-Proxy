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
 *   Author : Aashish Akhouri                                              *
 ***************************************************************************/

#include "VcsFabric/Local/VcsNodeIsFabricDistributionEnabledMessage.h"
#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"
#include "VcsFabric/Local/VcsNodeLocalTypes.h"

namespace DcmNs
{

VcsNodeIsFabricDistributionEnabledMessage::VcsNodeIsFabricDistributionEnabledMessage ()
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_IS_FABRIC_DIST_SERVICE_ENABLED)
{
    m_isFabricDistributionEnabled = false;
}

VcsNodeIsFabricDistributionEnabledMessage::~VcsNodeIsFabricDistributionEnabledMessage ()
{
}

void VcsNodeIsFabricDistributionEnabledMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeBool (&m_isFabricDistributionEnabled, "isFabricDistributionEnabled"));
}

bool VcsNodeIsFabricDistributionEnabledMessage::isFabricDistributionEnabled () const
{
    return (m_isFabricDistributionEnabled);
}

void VcsNodeIsFabricDistributionEnabledMessage::setFabricDistributionEnabled (bool isDistributionEnabled)
{
    m_isFabricDistributionEnabled = isDistributionEnabled;
}

}
