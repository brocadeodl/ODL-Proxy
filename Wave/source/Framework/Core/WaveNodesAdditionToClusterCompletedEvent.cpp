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
 *   Copyright (C) 2010 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Sudhakar Jha                                              *
 ***************************************************************************/


#include "Framework/Core/WaveNodesAdditionToClusterCompletedEvent.h"
#include "Framework/Core/FrameworkObjectManagerTypes.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"

namespace WaveNs
{

WaveNodesAdditionToClusterCompletedEvent::WaveNodesAdditionToClusterCompletedEvent ()
    : PrismEvent (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_NODES_ADDITION_TO_CLUSTER_COMPLETED_EVENT )
{
}

WaveNodesAdditionToClusterCompletedEvent::~WaveNodesAdditionToClusterCompletedEvent ()
{
}

void WaveNodesAdditionToClusterCompletedEvent::setupAttributesForSerialization ()
{
    PrismEvent::setupAttributesForSerialization ();
}

void WaveNodesAdditionToClusterCompletedEvent::addNewNodeDetails (const LocationId &locationId, const string &ipAddress, const UI32 &port)
{
  m_locationIdsForNewNodes.push_back(locationId);
  m_IpAddressesForNewNodes.push_back(ipAddress);
  m_portsForNewNodes.push_back(port);
}

UI32 WaveNodesAdditionToClusterCompletedEvent::getNumberOfNewLocations () const
{
  return m_locationIdsForNewNodes.size();
    
}

LocationId  WaveNodesAdditionToClusterCompletedEvent::getLocationIdAtIndex (const UI32 &index) const
{
  return m_locationIdsForNewNodes[index];
}

string WaveNodesAdditionToClusterCompletedEvent::getIpAddressAtIndex  (const UI32 &index) const
{
    return m_IpAddressesForNewNodes[index];
}

UI32 WaveNodesAdditionToClusterCompletedEvent::getPortAtIndex  (const UI32 &index) const
{
     return m_portsForNewNodes[index];
}
}

