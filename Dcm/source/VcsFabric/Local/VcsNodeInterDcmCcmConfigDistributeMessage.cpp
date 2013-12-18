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
 *   Copyright (C) 2005-2009 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vipool Prajapati                                         *
 ***************************************************************************/

#include "VcsFabric/Local/VcsNodeInterDcmCcmConfigDistributeMessage.h"
#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"
#include "VcsFabric/Local/VcsNodeLocalTypes.h"
#include "Framework/Attributes/AttributeResourceId.h"

namespace DcmNs
{

VcsNodeInterDcmCcmConfigDistributeMessage::VcsNodeInterDcmCcmConfigDistributeMessage ()
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_INTER_DCM_CCM_CONFIG_DISTRIBUTE),
    m_vcsClusterType    (VCS_FABRIC_LOCAL_CLUSTER_TYPE_FABRIC)
{
}

VcsNodeInterDcmCcmConfigDistributeMessage::~VcsNodeInterDcmCcmConfigDistributeMessage ()
{
}

void VcsNodeInterDcmCcmConfigDistributeMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeResourceId   (&m_vcsClusterType, "vcsClusterType"));
}

ResourceId VcsNodeInterDcmCcmConfigDistributeMessage::getVcsClusterType () const
{
    return (m_vcsClusterType);
}

void VcsNodeInterDcmCcmConfigDistributeMessage::setVcsClusterType (const ResourceId vcsClusterType)
{
    m_vcsClusterType = vcsClusterType;
}

}
