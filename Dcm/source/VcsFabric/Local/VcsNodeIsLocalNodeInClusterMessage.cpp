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
 *   Author : Himanshu Varshney                                            *
 ***************************************************************************/

#include "VcsFabric/Local/VcsNodeIsLocalNodeInClusterMessage.h"
#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"
#include "VcsFabric/Local/VcsNodeLocalTypes.h"

namespace DcmNs
{

VcsNodeIsLocalNodeInClusterMessage::VcsNodeIsLocalNodeInClusterMessage ()
    : ManagementInterfaceServiceIndependentMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_IS_LOCAL_NODE_IN_CLUSTER)
{
    m_isNodeInCluster = false;
}

VcsNodeIsLocalNodeInClusterMessage::~VcsNodeIsLocalNodeInClusterMessage ()
{
}

void VcsNodeIsLocalNodeInClusterMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceServiceIndependentMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeBool (&m_isNodeInCluster, "isNodeInCluster"));
}

bool VcsNodeIsLocalNodeInClusterMessage::getIsLocalNodeInCluster () const
{
    return (m_isNodeInCluster);
}

void VcsNodeIsLocalNodeInClusterMessage::setIsLocalNodeInCluster (const bool &isNodeInCluster)
{
    m_isNodeInCluster = isNodeInCluster;
}

}
