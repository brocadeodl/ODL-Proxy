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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Cluster/Local/ClusterLocalSetThisNodeIpAddressMessage.h"
#include "Cluster/Local/ClusterLocalTypes.h"
#include "Cluster/Local/ClusterLocalObjectManager.h"

namespace WaveNs
{

ClusterLocalSetThisNodeIpAddressMessage::ClusterLocalSetThisNodeIpAddressMessage ()
    : ManagementInterfaceMessage (ClusterLocalObjectManager::getPrismServiceName (), CLUSTER_LOCAL_SET_THIS_NODE_IP_ADDRESS),
      m_ipAddress                ("")
{
}

ClusterLocalSetThisNodeIpAddressMessage::ClusterLocalSetThisNodeIpAddressMessage (const string &ipAddress)
    : ManagementInterfaceMessage (ClusterLocalObjectManager::getPrismServiceName (), CLUSTER_LOCAL_SET_THIS_NODE_IP_ADDRESS),
      m_ipAddress                (ipAddress)
{
}

ClusterLocalSetThisNodeIpAddressMessage::~ClusterLocalSetThisNodeIpAddressMessage ()
{
}

string ClusterLocalSetThisNodeIpAddressMessage::getIpAddress () const
{
    return (m_ipAddress);
}

void ClusterLocalSetThisNodeIpAddressMessage::setIpAddress (const string &ipAddress)
{
    m_ipAddress = ipAddress;
}

}
