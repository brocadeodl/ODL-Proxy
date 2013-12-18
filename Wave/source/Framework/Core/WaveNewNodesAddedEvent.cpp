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
 *   Author : Aashish Akhouri                                              *
 ***************************************************************************/


#include "Framework/Core/WaveNewNodesAddedEvent.h"
#include "Framework/Core/FrameworkObjectManagerTypes.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"

namespace WaveNs
{

WaveNewNodesAddedEvent::WaveNewNodesAddedEvent ()
    : PrismEvent (PrismFrameworkObjectManager::getPrismServiceId (),FRAMEWORK_OBJECT_MANAGER_BROADCAST_ADDITION_OF_NEW_NODES_EVENT )
{
}

WaveNewNodesAddedEvent::~WaveNewNodesAddedEvent ()
{
}

void WaveNewNodesAddedEvent::setupAttributesForSerialization ()
{
    PrismEvent::setupAttributesForSerialization ();
}

void WaveNewNodesAddedEvent::addNewNodeDetails (const LocationId &locationId, const string &ipAddress, const UI32 &port)
{
  m_locationIdsForNewNodes.push_back(locationId);
  m_IpAddressesForNewNodes.push_back(ipAddress);
  m_portsForNewNodes.push_back(port);
}

UI32 WaveNewNodesAddedEvent::getNumberOfNewLocations () const
{
  return m_locationIdsForNewNodes.size();
    
}

LocationId  WaveNewNodesAddedEvent::getLocationIdAtIndex        (const UI32 &index) const
{
  return m_locationIdsForNewNodes[index];
}

string WaveNewNodesAddedEvent::getIpAddressAtIndex         (const UI32 &index) const
{
    return m_IpAddressesForNewNodes[index];
}

UI32 WaveNewNodesAddedEvent::getPortAtIndex              (const UI32 &index) const
{
     return m_portsForNewNodes[index];
}
}
