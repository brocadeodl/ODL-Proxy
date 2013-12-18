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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "VcsFabric/Global/VcsDeleteClusterMessage.h"
#include "VcsFabric/Global/VcsFabObjectManager.h"
#include "VcsFabric/Global/VcsFabObjectManagerTypes.h"

namespace DcmNs
{
VcsDeleteClusterMessage::VcsDeleteClusterMessage ()
: ManagementInterfaceMessage (VcsFabObjectManager::getClassName (), VCS_FABRIC_DELETE_CLUSTER),
    m_vcsClusterId           (0)
{
}

VcsDeleteClusterMessage::VcsDeleteClusterMessage (const UI32 clusterId)
: ManagementInterfaceMessage (VcsFabObjectManager::getClassName (), VCS_FABRIC_DELETE_CLUSTER),
	m_vcsClusterId                 (clusterId)
{
}

VcsDeleteClusterMessage::~VcsDeleteClusterMessage ()
{
}

/*
 addSerializableAttribute (new AttributeObjectId (&m_interfaceObjectId, "interfaceObjectId"));
 addSerializableAttribute (new AttributeString   (&m_ipAddress, "ipAddress"));
 addSerializableAttribute (new AttributeUI32     (&m_mtu, "mtu"));
*/

void VcsDeleteClusterMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeUI32 (&m_vcsClusterId, "vcsClusterId"));

}

UI32 VcsDeleteClusterMessage::getClusterId ()
{
	return m_vcsClusterId;
}
}

