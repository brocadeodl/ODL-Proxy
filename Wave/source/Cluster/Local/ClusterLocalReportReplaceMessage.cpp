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
 *   Author : Vipool Prajapati                                             *
 ***************************************************************************/
#include "Framework/Messaging/Local/PrismMessage.h"
#include "Cluster/Local/ClusterLocalReportReplaceMessage.h"
#include "Cluster/Local/ClusterLocalTypes.h"
#include "Cluster/Local/ClusterLocalObjectManager.h"

namespace WaveNs
{
/// Name
/// ClusterLocalReportReplaceMessage
/// Description
/// Just Calls the base class constructor
/// Constructor
/// Input
/// None
/// Output
/// None
/// Return
/// None
ClusterLocalReportReplaceMessage::ClusterLocalReportReplaceMessage ()
    : PrismMessage (ClusterLocalObjectManager::getPrismServiceId (), CLUSTER_LOCAL_REPORT_REPLACE),
      m_thisNodeIpAddress(""),
      m_thisNodePort(0),
      m_thisNodeLocationId(0),
      m_replacedNodeLocationId(0)
{


}

/// Name
/// ClusterLocalReportReplaceMessage
/// Description
/// Non-Default Constructor
/// Input
/// None
/// Output
/// None
/// Return
/// None
ClusterLocalReportReplaceMessage::ClusterLocalReportReplaceMessage (const string & thisNodeIpAddress,
                                                                                          const UI32   thisNodePort,
                                                                                          const LocationId   thisNodeLocationId,
                                                                                          const LocationId   replacedNodeLocationId)
    : PrismMessage (ClusterLocalObjectManager::getPrismServiceId (), CLUSTER_LOCAL_REPORT_REPLACE),
      m_thisNodeIpAddress(thisNodeIpAddress),
      m_thisNodePort(thisNodePort),
      m_thisNodeLocationId(thisNodeLocationId),
      m_replacedNodeLocationId(replacedNodeLocationId)

{
}

/// Name
/// ClusterLocalReportReplaceMessage
/// Description
/// Destructor
/// Input
/// None
/// Output
/// None
/// Return
/// None
ClusterLocalReportReplaceMessage::~ClusterLocalReportReplaceMessage ()
{
}


//Accessors and Mutators for fields related to LocalNode Info as well as primary node info



void ClusterLocalReportReplaceMessage::setThisNodeIpAddress(const string &thisNodeIpAddress)
{
    m_thisNodeIpAddress = thisNodeIpAddress;

}

void ClusterLocalReportReplaceMessage::setThisNodePort(const UI32 thisNodePort)
{
    m_thisNodePort = thisNodePort;
}

void ClusterLocalReportReplaceMessage::setThisNodeLocationId(const UI32 thisLocationId)
{
    m_thisNodeLocationId = thisLocationId;
}

void ClusterLocalReportReplaceMessage::setReplacedNodeLocationId(const UI32 replacedLocationId)
{
    m_replacedNodeLocationId = replacedLocationId;
}

void ClusterLocalReportReplaceMessage::setupAttributesForSerialization ()
{
     PrismMessage::setupAttributesForSerialization ();
      addSerializableAttribute (new AttributeString     (&m_thisNodeIpAddress,      "thisNodeIpAddress"));
      addSerializableAttribute (new AttributeUI32       (&m_thisNodePort,           "thisNodePort"));
      addSerializableAttribute (new AttributeLocationId (&m_thisNodeLocationId,     "thisNodeLocationId"));
      addSerializableAttribute (new AttributeLocationId (&m_replacedNodeLocationId, "replacedNodeLocationId"));
}

string const & ClusterLocalReportReplaceMessage::getThisNodeIpAddress() const
{
    return m_thisNodeIpAddress;
}


UI32 ClusterLocalReportReplaceMessage::getThisNodePort() const
{
    return m_thisNodePort;
}


LocationId ClusterLocalReportReplaceMessage::getThisNodeLocationId() const
{
    return m_thisNodeLocationId;
}

LocationId ClusterLocalReportReplaceMessage::getReplacedNodeLocationId() const
{
    return m_replacedNodeLocationId;
}

}
