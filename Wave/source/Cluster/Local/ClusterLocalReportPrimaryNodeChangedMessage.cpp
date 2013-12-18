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
 *   Author : Aashish                                                      *
 ***************************************************************************/
#include "Framework/Messaging/Local/PrismMessage.h"
#include "Cluster/Local/ClusterLocalReportPrimaryNodeChangedMessage.h"
#include "Cluster/Local/ClusterLocalTypes.h"
#include "Cluster/Local/ClusterLocalObjectManager.h"

namespace WaveNs
{
/// Name
/// ClusterLocalReportPrimaryNodeChangedMessage
/// Description
/// Just Calls the base class constructor
/// Constructor
/// Input
/// None
/// Output
/// None
/// Return
/// None
ClusterLocalReportPrimaryNodeChangedMessage::ClusterLocalReportPrimaryNodeChangedMessage ()
    : PrismMessage (ClusterLocalObjectManager::getPrismServiceId (), CLUSTER_LOCAL_REPORT_PRIMARY_NODE_CHANGED),
      m_thisNodeIpAddress(""),
      m_thisNodePort(0),
      m_thisNodeLocationId(0)
{


}

/// Name
/// ClusterLocalReportPrimaryNodeChangedMessage
/// Description
/// Non-Default Constructor
/// Input
/// None
/// Output
/// None
/// Return
/// None
ClusterLocalReportPrimaryNodeChangedMessage::ClusterLocalReportPrimaryNodeChangedMessage (const string & thisNodeIpAddress,
                                                                                          const UI32   thisNodePort,
                                                                                          const LocationId   thisNodeLocationId)
    : PrismMessage (ClusterLocalObjectManager::getPrismServiceId (), CLUSTER_LOCAL_REPORT_PRIMARY_NODE_CHANGED),
      m_thisNodeIpAddress(thisNodeIpAddress),
      m_thisNodePort(thisNodePort),
      m_thisNodeLocationId(thisNodeLocationId)

{
}

/// Name
/// ClusterLocalReportPrimaryNodeChangedMessage
/// Description
/// Destructor
/// Input
/// None
/// Output
/// None
/// Return
/// None
ClusterLocalReportPrimaryNodeChangedMessage::~ClusterLocalReportPrimaryNodeChangedMessage ()
{
}


//Accessors and Mutators for fields related to LocalNode Info as well as primary node info



void ClusterLocalReportPrimaryNodeChangedMessage::setThisNodeIpAddress(const string &thisNodeIpAddress)
{
    m_thisNodeIpAddress = thisNodeIpAddress;

}

void ClusterLocalReportPrimaryNodeChangedMessage::setThisNodePort(const UI32 thisNodePort)
{
    m_thisNodePort = thisNodePort;
}

void ClusterLocalReportPrimaryNodeChangedMessage::setThisNodeLocationId(const UI32 thisLocationId)
{
    m_thisNodeLocationId = thisLocationId;
}

void ClusterLocalReportPrimaryNodeChangedMessage::setupAttributesForSerialization ()
{
     PrismMessage::setupAttributesForSerialization ();
      addSerializableAttribute (new AttributeString     (&m_thisNodeIpAddress,  "thisNodeIpAddress"));
      addSerializableAttribute (new AttributeUI32       (&m_thisNodePort,       "thisNodePort"));
      addSerializableAttribute (new AttributeLocationId (&m_thisNodeLocationId, "thisNodeLocationId"));
}

string const & ClusterLocalReportPrimaryNodeChangedMessage::getThisNodeIpAddress() const
{
    return m_thisNodeIpAddress;
}


UI32 ClusterLocalReportPrimaryNodeChangedMessage::getThisNodePort() const
{
    return m_thisNodePort;
}


LocationId ClusterLocalReportPrimaryNodeChangedMessage::getThisNodeLocationId() const
{
    return m_thisNodeLocationId;
}

}
