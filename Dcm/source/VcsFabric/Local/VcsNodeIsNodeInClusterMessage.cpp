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
 *   Author : Pritee Ghosh                                                 *
 ***************************************************************************/

#include "VcsFabric/Local/VcsNodeIsNodeInClusterMessage.h"
#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"
#include "VcsFabric/Local/VcsNodeLocalTypes.h"

namespace DcmNs
{

VcsNodeIsNodeInClusterMessage::VcsNodeIsNodeInClusterMessage ()
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_IS_NODE_IN_CLUSTER)
{
    m_isNodeInCluster = false;
    m_rbridgeId = 0;
}

VcsNodeIsNodeInClusterMessage::VcsNodeIsNodeInClusterMessage (const UI32 rbridgeId)
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_IS_NODE_IN_CLUSTER),
      m_rbridgeId                    (rbridgeId),
      m_isNodeInCluster              (0)
{
}

VcsNodeIsNodeInClusterMessage::~VcsNodeIsNodeInClusterMessage ()
{
}

void VcsNodeIsNodeInClusterMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeUI32 (&m_rbridgeId,         "rbridgeId"));
     addSerializableAttribute (new AttributeBool (&m_isNodeInCluster,   "isNodeInCluster"));
}

UI32 VcsNodeIsNodeInClusterMessage::getRBridgeId () const
{
    return (m_rbridgeId);
}

void VcsNodeIsNodeInClusterMessage::setRBridgeId (UI32 rbridgeid) 
{
    m_rbridgeId = rbridgeid;
}

bool VcsNodeIsNodeInClusterMessage::getIsNodeInCluster () const
{
    return (m_isNodeInCluster);
}

void VcsNodeIsNodeInClusterMessage::setIsNodeInCluster (bool isNodeInCluster)
{
    m_isNodeInCluster = isNodeInCluster;
}
}
