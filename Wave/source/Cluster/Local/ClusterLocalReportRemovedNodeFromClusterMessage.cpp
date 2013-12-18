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
#include "Cluster/Local/ClusterLocalReportRemovedNodeFromClusterMessage.h"
#include "Cluster/Local/ClusterLocalTypes.h"
#include "Cluster/Local/ClusterLocalObjectManager.h"

namespace WaveNs
{

/// Name
/// ClusterLocalReportRemovedNodeFromClusterMessage
/// Description
/// Just Calls the base class constructor
/// Constructor
/// Input
/// None
/// Output
/// None
/// Return
/// None

ClusterLocalReportRemovedNodeFromClusterMessage::ClusterLocalReportRemovedNodeFromClusterMessage ()
    : PrismMessage (ClusterLocalObjectManager::getPrismServiceId (), CLUSTER_LOCAL_REPORT_NODE_REMOVED_FROM_CLUSTER),
      m_thisNodePort  (0),
      m_thisNodeLocationId(0)
{


}
/// Name
/// ClusterLocalReportRemovedNodeFromClusterMessage
/// Description
/// Non-Default Constructor
/// Input
/// None
/// Output
/// None
/// Return
/// None


ClusterLocalReportRemovedNodeFromClusterMessage::ClusterLocalReportRemovedNodeFromClusterMessage (const string & thisNodeIpAddress,UI32 thisNodePort, LocationId thisNodeLocationId)
    : PrismMessage (ClusterLocalObjectManager::getPrismServiceId (), CLUSTER_LOCAL_REPORT_NODE_REMOVED_FROM_CLUSTER),m_thisNodeIpAddress(thisNodeIpAddress), m_thisNodePort(thisNodePort), m_thisNodeLocationId(thisNodeLocationId)

{
}
/// Name
/// ClusterLocalReportRemovedNodeFromClusterMessage
/// Description
/// Destructor
/// Input
/// None
/// Output
/// None
/// Return
/// None

ClusterLocalReportRemovedNodeFromClusterMessage::~ClusterLocalReportRemovedNodeFromClusterMessage ()
{
}

//Mutators and Accessors
void ClusterLocalReportRemovedNodeFromClusterMessage::setThisNodeIpAddress(const string &thisNodeIpAddress)
{
    m_thisNodeIpAddress = thisNodeIpAddress;

}

void ClusterLocalReportRemovedNodeFromClusterMessage::setThisNodePort(const UI32 thisNodePort)
{
    m_thisNodePort = thisNodePort;
}


void ClusterLocalReportRemovedNodeFromClusterMessage::setThisNodeLocationId(const UI32 thisLocationId)
{
    m_thisNodeLocationId = thisLocationId;
}

void ClusterLocalReportRemovedNodeFromClusterMessage::setupAttributesForSerialization()
{
    PrismMessage::setupAttributesForSerialization ();
     addSerializableAttribute (new AttributeString      (&m_thisNodeIpAddress,  "thisNodeIpAddress"));
     addSerializableAttribute (new AttributeUI32        (&m_thisNodePort,       "thisNodePort"));
     addSerializableAttribute (new AttributeLocationId  (&m_thisNodeLocationId, "thisNodeLocationId"));
}
string const & ClusterLocalReportRemovedNodeFromClusterMessage::getThisNodeIpAddress() const
{
    return m_thisNodeIpAddress;
}


UI32 ClusterLocalReportRemovedNodeFromClusterMessage::getThisNodePort() const
{
    return m_thisNodePort;
}



LocationId ClusterLocalReportRemovedNodeFromClusterMessage::getThisNodeLocationId() const
{
    return m_thisNodeLocationId;
}

}
