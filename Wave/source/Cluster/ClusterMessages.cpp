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
 *   Copyright (C) 2005 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Amr Sabaa                                                    *
 ***************************************************************************/

#include "Cluster/ClusterMessages.h"
#include "Cluster/CentralClusterConfigObjectManager.h"
#include "Cluster/LocalClusterConfigObjectManager.h"
#include "App/AppInterfaceObjectManager.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Attributes/AttributeResourceId.h"

namespace WaveNs
{

ClusterObjectManagerCreateClusterMessage::ClusterObjectManagerCreateClusterMessage ()
    : ManagementInterfaceMessage (CentralClusterConfigObjectManager::getServiceName (), CLUSTER_CREATE_CLUSTER),
      m_nSecondaryNodes(0),
      m_isAddNodeForSpecialCase (false)
{
}

ClusterObjectManagerCreateClusterMessage::ClusterObjectManagerCreateClusterMessage (const vector<string> &secondaryNodeNames, const vector<SI32> &secondaryNodePorts)
    : ManagementInterfaceMessage (CentralClusterConfigObjectManager::getServiceName (), CLUSTER_CREATE_CLUSTER),
      m_isAddNodeForSpecialCase (false)
{
    UI32 numberOfSecondaryNodeNames = secondaryNodeNames.size ();
    UI32 numberOfSecondaryNodePorts = secondaryNodePorts.size ();

    prismAssert (numberOfSecondaryNodeNames == numberOfSecondaryNodePorts, __FILE__, __LINE__);

    setNSecondaryNodes (numberOfSecondaryNodeNames);

    m_secondaryNodeNames = secondaryNodeNames;
    m_secondaryNodePorts = secondaryNodePorts;
}

ClusterObjectManagerCreateClusterMessage::ClusterObjectManagerCreateClusterMessage (const string &serviceName, const UI32 &operationId)
    : ManagementInterfaceMessage (serviceName, operationId),
      m_nSecondaryNodes          (0),
      m_isAddNodeForSpecialCase  (false)
{
}

ClusterObjectManagerCreateClusterMessage::ClusterObjectManagerCreateClusterMessage (const string &serviceName, const UI32 &operationId, const vector<string> &secondaryNodeNames, const vector<SI32> &secondaryNodePorts)
    : ManagementInterfaceMessage (serviceName, operationId),
      m_nSecondaryNodes          (0),
      m_isAddNodeForSpecialCase  (false)
{
    UI32 numberOfSecondaryNodeNames = secondaryNodeNames.size ();
    UI32 numberOfSecondaryNodePorts = secondaryNodePorts.size ();

    prismAssert (numberOfSecondaryNodeNames == numberOfSecondaryNodePorts, __FILE__, __LINE__);

    setNSecondaryNodes (numberOfSecondaryNodeNames);

    m_secondaryNodeNames = secondaryNodeNames;
    m_secondaryNodePorts = secondaryNodePorts;
}

ClusterObjectManagerCreateClusterMessage::~ClusterObjectManagerCreateClusterMessage ()
{
}

void ClusterObjectManagerCreateClusterMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeString             (&m_primaryNodeName,         "primaryNodeName"));
     addSerializableAttribute (new AttributeUI32               (&m_nSecondaryNodes,         "nSecondaryNodes"));
     addSerializableAttribute (new AttributeStringVector       (&m_secondaryNodeNames,      "secondaryNodeNames"));
     addSerializableAttribute (new AttributeSI32Vector         (&m_secondaryNodePorts,      "secondaryNodePorts"));
     addSerializableAttribute (new AttributeStringVector       (&m_statusNodeNames,         "statusNodeNames"));
     addSerializableAttribute (new AttributeSI32Vector         (&m_statusNodePorts,         "statusNodePorts"));
     addSerializableAttribute (new AttributeUI32Vector         (&m_statusNodeStatus,        "statusNodeStatus"));
     addSerializableAttribute (new AttributeBool               (&m_isAddNodeForSpecialCase, "isAddNodeForSpecialCase"));
     addSerializableAttribute (new AttributeStringVector       (&m_filenamesToSync,         "filenamesToSync"));
}

void ClusterObjectManagerCreateClusterMessage::setPrimaryNodeName (const string &primaryNodeName)
{
    m_primaryNodeName = primaryNodeName;
}

string ClusterObjectManagerCreateClusterMessage::getPrimaryNodeName ()
{
    return (m_primaryNodeName);
}

void ClusterObjectManagerCreateClusterMessage::setNSecondaryNodes (const UI32 nSecondaryNodes)
{
    m_nSecondaryNodes = nSecondaryNodes;
}

UI32 ClusterObjectManagerCreateClusterMessage::getNSecondaryNodes ()
{
    return (m_nSecondaryNodes);
}

void ClusterObjectManagerCreateClusterMessage::setSecondaryNodeNameAndPort (const string &nodeName, const SI32 &nodePort)
{
    m_secondaryNodeNames.push_back (nodeName);
    m_secondaryNodePorts.push_back (nodePort);
}

string ClusterObjectManagerCreateClusterMessage::getSecondaryNodeName (UI32 index)
{
    if (index < m_secondaryNodeNames.size ())
    {
        return ( m_secondaryNodeNames [index]);
    }
    else
    {
        WaveNs::trace (TRACE_LEVEL_ERROR, "ClusterObjectManagerCreateClusterMessage::getSecondaryNodeName: Invalid Index.");
        return 0;
    }
}

SI32 ClusterObjectManagerCreateClusterMessage::getSecondaryNodePort (UI32 index)
{
    if (index < m_secondaryNodePorts.size ())
    {
        return ( m_secondaryNodePorts [index]);
    }
    else
    {
        WaveNs::trace (TRACE_LEVEL_ERROR, "ClusterObjectManagerCreateClusterMessage::getSecondaryNodePort: Invalid Index.");
        return 0;
    }
}
/*
Node ClusterObjectManagerCreateClusterMessage::getSecondaryNode (UI32 index)
{
    return ( m_secondaryNode [index]);
}
*/


string ClusterObjectManagerCreateClusterMessage::getStatusNodeName (UI32 index)
{
    if (index < m_statusNodeNames.size ())
    {
        return (m_statusNodeNames [index]);
    }
    else
    {
        WaveNs::trace (TRACE_LEVEL_ERROR, "ClusterObjectManagerCreateClusterMessage::getStatusNodeName: Invalid Index.");
        return 0;
    }
}

SI32 ClusterObjectManagerCreateClusterMessage::getStatusNodePort (UI32 index)
{
    if (index < m_statusNodePorts.size ())
    {
        return (m_statusNodePorts [index]);
    }
    else
    {
        WaveNs::trace (TRACE_LEVEL_ERROR, "ClusterObjectManagerCreateClusterMessage::getStatusNodeAddr: Invalid Index.");
        return 0;
    }
}


UI32 ClusterObjectManagerCreateClusterMessage::getStatusNodeStatus (UI32 index)
{
    if (index < m_statusNodeStatus.size ())
    {
        return (m_statusNodeStatus [index]);
    }
    else
    {
        WaveNs::trace (TRACE_LEVEL_ERROR, "ClusterObjectManagerCreateClusterMessage::getNodeStatus: Invalid Index.");
        return 0;
    }
}


void ClusterObjectManagerCreateClusterMessage::setNodeStaus (const string &nodeName, UI32 nodePort, UI32 status)
{
    bool nodeAlreadyExists = false;
    UI32 numberOfNodes     = m_statusNodeNames.size ();
    UI32 i                 = 0;

    for (i = 0; i < numberOfNodes; i++)
    {
        if ((nodeName == m_statusNodeNames[i]) && (nodePort == ((UI32) m_statusNodePorts[i])))
        {
           nodeAlreadyExists = true;
           break;
        }
    }

    if (true == nodeAlreadyExists)
    {
        m_statusNodeStatus[i] = status;
    }
    else
    {
        m_statusNodeNames.push_back (nodeName);
        m_statusNodePorts.push_back (nodePort);
        m_statusNodeStatus.push_back (status);
    }
}

UI32 ClusterObjectManagerCreateClusterMessage::getNodeStatus (const string &nodeName, const UI32& nodePort)
{
    UI32 numberOfNodes     = m_statusNodeNames.size ();
    UI32 i                 = 0;

    for (i = 0; i < numberOfNodes; i++)
    {
        if ((nodeName == m_statusNodeNames[i]) && (nodePort == ((UI32) m_statusNodePorts[i])))
        {
           return (m_statusNodeStatus[i]);
        }
    }

    WaveNs::trace (TRACE_LEVEL_FATAL, string ("ClusterObjectManagerCreateClusterMessage::getNodeStatus : This node (") + nodeName + ":" + nodePort + ") status is not set.");
    prismAssert (false, __FILE__, __LINE__);
    return (WAVE_MESSAGE_ERROR);
}
void ClusterObjectManagerCreateClusterMessage::setIsAddNodeForSpecialCaseFlag (const bool &isAddNodeForSpecialCase)
{
    m_isAddNodeForSpecialCase = isAddNodeForSpecialCase;
}

bool ClusterObjectManagerCreateClusterMessage::getIsAddNodeForSpecialCaseFlag () const
{
    return (m_isAddNodeForSpecialCase);
}

void ClusterObjectManagerCreateClusterMessage::setFilenamesToSync ( const vector<string> &filenamesToSync )
{
    m_filenamesToSync = filenamesToSync;
}

void ClusterObjectManagerCreateClusterMessage::getFilenamesToSync ( vector<string> &filenamesToSync )
{
    filenamesToSync = m_filenamesToSync;
}

void ClusterObjectManagerCreateClusterMessage::addFilenameToSync  ( const string &filenameToSync )
{
    m_filenamesToSync.push_back ( filenameToSync );
}

ClusterObjectManagerDeleteClusterMessage::ClusterObjectManagerDeleteClusterMessage ()
    : PrismMessage (CentralClusterConfigObjectManager::getPrismServiceId (), CLUSTER_DELETE_CLUSTER),
      m_isRebootRequired (true)
{
}

ClusterObjectManagerDeleteClusterMessage::~ClusterObjectManagerDeleteClusterMessage ()
{
}

void ClusterObjectManagerDeleteClusterMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();
}

void ClusterObjectManagerDeleteClusterMessage::setIsRebootRequired (const bool &isRebootRequired)
{
    m_isRebootRequired = isRebootRequired;
}

bool ClusterObjectManagerDeleteClusterMessage::getIsRebootRequired () const
{
    return (m_isRebootRequired);
}

ClusterObjectManagerAddNodeMessage::ClusterObjectManagerAddNodeMessage ()
    : ClusterObjectManagerCreateClusterMessage (CentralClusterConfigObjectManager::getServiceName (), CLUSTER_ADD_NODE)
{
}

ClusterObjectManagerAddNodeMessage::ClusterObjectManagerAddNodeMessage  (const vector<string> &secondaryNodeNames, const vector<SI32> &secondaryNodePorts)
    : ClusterObjectManagerCreateClusterMessage (CentralClusterConfigObjectManager::getServiceName (), CLUSTER_ADD_NODE, secondaryNodeNames, secondaryNodePorts)
{
}

ClusterObjectManagerAddNodeMessage::~ClusterObjectManagerAddNodeMessage ()
{
}

void ClusterObjectManagerAddNodeMessage::setupAttributesForSerialization ()
{
    ClusterObjectManagerCreateClusterMessage::setupAttributesForSerialization ();
}


ClusterObjectManagerJoinNodeMessage::ClusterObjectManagerJoinNodeMessage ()
    : PrismMessage (CentralClusterConfigObjectManager::getPrismServiceId (), CLUSTER_JOIN_NODE),
      m_nodePort (0)
{
}

ClusterObjectManagerJoinNodeMessage::~ClusterObjectManagerJoinNodeMessage ()
{
}


void ClusterObjectManagerJoinNodeMessage::setNodeName (const string &nodeName)
{
    m_nodeName  =   nodeName;
}

string ClusterObjectManagerJoinNodeMessage::getNodeName ()
{
    return (m_nodeName);
}

void ClusterObjectManagerJoinNodeMessage::setNodePort (const SI32 &nodePort)
{
    m_nodePort = nodePort;
}

SI32 ClusterObjectManagerJoinNodeMessage::getNodePort ()
{
    return (m_nodePort);
}

//
//Name:
//ClusterObjectManagerRejoinNodeMessage
//
//Description:
//Constructor
//
//Input/s:
//None
//Output/s:
//None
//Returns:
//None
//
ClusterObjectManagerRejoinNodeMessage::ClusterObjectManagerRejoinNodeMessage ()
    : PrismMessage (CentralClusterConfigObjectManager::getPrismServiceId (), CLUSTER_JOIN_NODE)
{
    m_isReplaceRejoin = false;
}

//
//Name:
//ClusterObjectManagerRejoinNodeMessage
//
//Description:
//Desrtructor
//
//Input/s:
//None
//Output/s:
//None
//Returns:
//None
//
ClusterObjectManagerRejoinNodeMessage::~ClusterObjectManagerRejoinNodeMessage ()
{
}

/// Name:
/// setupAttributesForSerialization
/// Description:
/// This message is a prismMessage each prismMessage is a serilizableObject
/// that has a map to store the serializable attributes. Thi method is putting the
/// attributes into the map
/// Input
/// none
/// Output
/// none
/// Return
/// none


void ClusterObjectManagerRejoinNodeMessage::setupAttributesForSerialization ()
{

    PrismMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeStringVector (&m_nodeIpAddresses,   "nodeIpAddresses"));
     addSerializableAttribute (new AttributeUI32Vector  (&m_nodePorts,          "nodePorts"));
     addSerializableAttribute (new AttributeBool        (&m_isReplaceRejoin,    "isReplaceRejoin"));

    return;
}

////
// Get And Set Methods for IP and port
///
void ClusterObjectManagerRejoinNodeMessage::setNodeIpAddress (const string & nodeIpAddress)
{
    m_nodeIpAddresses.push_back(nodeIpAddress);
    return;
}

string const & ClusterObjectManagerRejoinNodeMessage::getNodeIpAddress (const UI32 index) const
{
    return (m_nodeIpAddresses[index]);
}

void ClusterObjectManagerRejoinNodeMessage::setNodePort (const UI32 & nodePort)
{
    m_nodePorts.push_back(nodePort);
    return;
}

UI32 ClusterObjectManagerRejoinNodeMessage::getNodePort (const UI32 index) const
{
    return (m_nodePorts[index]);
}

UI32 ClusterObjectManagerRejoinNodeMessage::getNNodes() const
{
    return m_nodeIpAddresses.size();
}

void ClusterObjectManagerRejoinNodeMessage::setIsReplaceRejoin (const bool &isReplaceRejoin)
{
    m_isReplaceRejoin = isReplaceRejoin;
}

bool ClusterObjectManagerRejoinNodeMessage::getIsReplaceRejoin () const
{
    return (m_isReplaceRejoin);
}

void ClusterObjectManagerRejoinNodeMessage::setNodeLocationId (const LocationId & locationId)
{
    m_locationIds.push_back(locationId);
    return;
}

LocationId ClusterObjectManagerRejoinNodeMessage::getNodeLocationId (const UI32 index) const
{
    return (m_locationIds[index]);
}

void ClusterObjectManagerRejoinNodeMessage::setNodeStatus (const string &nodeIP, SI32 nodePort, const UI32 status)
{
    string uniqueTag = nodeIP + "#" + nodePort;
    map<string, ResourceId>::iterator element = m_nodesStatus.find (uniqueTag);
    map<string, ResourceId>::iterator end  = m_nodesStatus.end ();

    if (end == element)
    {
        m_nodesStatus[uniqueTag] = status;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("ClusterObjectManagerRejoinNodeMessage::setNodeStatus : This node (") + nodeIP + ":" + nodePort + ") status is already set.");
        prismAssert (false, __FILE__, __LINE__);
    }
}

ResourceId ClusterObjectManagerRejoinNodeMessage::getNodeStatus (const string &nodeIP, SI32 nodePort)
{
    string uniqueTag = nodeIP + "#" + nodePort;
    map<string, ResourceId>::iterator element   = m_nodesStatus.find (uniqueTag);
    map<string, ResourceId>::iterator end       = m_nodesStatus.end ();

    if (end != element)
    {
        return (m_nodesStatus[uniqueTag]);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("ClusterObjectManagerRejoinNodeMessage::getNodeStatus : This node (") + nodeIP + ":" + nodePort + ") status is not set.");
        prismAssert (false, __FILE__, __LINE__);
        return (WAVE_MESSAGE_ERROR);
    }
}

ClusterObjectManagerDeleteNodeMessage::ClusterObjectManagerDeleteNodeMessage ()
    : PrismMessage (CentralClusterConfigObjectManager::getPrismServiceId (), CLUSTER_DELETE_NODE),
      m_isDisconnected (false)
{
}

ClusterObjectManagerDeleteNodeMessage::~ClusterObjectManagerDeleteNodeMessage ()
{
}

///
//Delete Node
///
void ClusterObjectManagerDeleteNodeMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeStringVector (&m_nodeName,          "nodeName"));
     addSerializableAttribute (new AttributeSI32Vector   (&m_nodePort,          "nodePort"));
     addSerializableAttribute (new AttributeUI32         (&m_nNodes,            "nNodes"));
     addSerializableAttribute (new AttributeUI32Vector   (&m_nodeStatus,        "nodeStatus"));
     addSerializableAttribute (new AttributeStringVector (&m_statusNodeNames,   "statusNodeNames"));
     addSerializableAttribute (new AttributeSI32Vector   (&m_statusNodePorts,   "statusNodePorts"));
     addSerializableAttribute (new AttributeBool         (&m_isDisconnected,    "isDisconnected"));
}

void ClusterObjectManagerDeleteNodeMessage::setSecondaryNodeNameAndPort (const string &nodeName, const SI32 &nodePort)
{
    m_nodeName.push_back (nodeName);
    m_nodePort.push_back (nodePort);
}

void ClusterObjectManagerDeleteNodeMessage::setNNodesToDelete (const UI32 nNodes)
{
    m_nNodes = nNodes;
}

string ClusterObjectManagerDeleteNodeMessage::getNodeName (UI32 index)
{
    if (index < m_nodeName.size ())
    {
        return (m_nodeName [index]);
    }
    else
    {
        WaveNs::trace (TRACE_LEVEL_ERROR, "ClusterObjectManagerDeleteNodeMessage::getSecondaryNodeName: Invalid Index.");
        return 0;
    }
}

SI32 ClusterObjectManagerDeleteNodeMessage::getNodePort (UI32 index)
{
    if (index < m_nodePort.size ())
    {
        return (m_nodePort [index]);
    }
    else
    {
        WaveNs::trace (TRACE_LEVEL_ERROR, "ClusterObjectManagerDeleteNodeMessage::getSecondaryNodePort: Invalid Index.");
        return 0;
    }
}

UI32 ClusterObjectManagerDeleteNodeMessage::getNNodesToDelete ()
{
    return (m_nNodes);
}

void ClusterObjectManagerDeleteNodeMessage::setNodeStatus (const string &nodeName, const SI32 nodePort, const UI32 Status)
{
    UI32 nNodes = m_statusNodeNames.size ();
    UI32 i;
    bool nodeAlreadyExists = false;

    for (i = 0; i < nNodes; i++)
    {
        if ((m_statusNodeNames[i] == nodeName) && m_statusNodePorts[i] == nodePort)
        {
            nodeAlreadyExists = true;
            break;
        }
    }

    if (true == nodeAlreadyExists)
    {
        m_nodeStatus[i] = Status;
    }
    else
    {
        m_statusNodeNames.push_back (nodeName);
        m_statusNodePorts.push_back (nodePort);
        m_nodeStatus.push_back (Status);
    }
}

UI32 ClusterObjectManagerDeleteNodeMessage::getNodeStatus (string &nodeName, SI32 &nodePort) const
{
    UI32 nNodes = m_statusNodeNames.size ();
    UI32 i;

    for (i = 0; i < nNodes; i++)
    {
        if ((m_statusNodeNames[i] == nodeName) && m_statusNodePorts[i] == nodePort)
        {
            return (m_nodeStatus[i]);
        }
    }

    WaveNs::trace (TRACE_LEVEL_ERROR, "ClusterObjectManagerDeleteNodeMessage::getNodeStatus: Invalid Node specified");

    return (0);
}

void ClusterObjectManagerDeleteNodeMessage::setIsDisconnected(bool isDisconnected)
{
    m_isDisconnected = isDisconnected;
}

bool ClusterObjectManagerDeleteNodeMessage::getIsDisconnected ()
{
    return m_isDisconnected;
}

LocalClusterConfigObjectManagerReportPrimaryNodeChangedMessage::LocalClusterConfigObjectManagerReportPrimaryNodeChangedMessage ()
    : PrismMessage (LocalClusterConfigObjectManager::getPrismServiceId (), CLUSTER_PRIMARY_NODE_CHANGED),
      m_thisNodePort    (0),
      m_nSecondaryNodes (0)
{
}

LocalClusterConfigObjectManagerReportPrimaryNodeChangedMessage::~LocalClusterConfigObjectManagerReportPrimaryNodeChangedMessage ()
{
}


void LocalClusterConfigObjectManagerReportPrimaryNodeChangedMessage::setPrimaryNodeName (const string &primaryNodeName)
{
    m_primaryNodeName = primaryNodeName;
}

void LocalClusterConfigObjectManagerReportPrimaryNodeChangedMessage::setPrimaryNodePort (const SI32 &primaryNodePort)
{
    m_primaryNodePort = primaryNodePort;
}

string LocalClusterConfigObjectManagerReportPrimaryNodeChangedMessage::getPrimaryNodeName ()
{
    return (m_primaryNodeName);
}

SI32 LocalClusterConfigObjectManagerReportPrimaryNodeChangedMessage::getPrimaryNodePort ()
{
    return (m_primaryNodePort);
}

void LocalClusterConfigObjectManagerReportPrimaryNodeChangedMessage::setNSecondaryNodes (const UI32 nSecondaryNodes)
{
    m_nSecondaryNodes = nSecondaryNodes;
}

UI32 LocalClusterConfigObjectManagerReportPrimaryNodeChangedMessage::getNSecondaryNodes ()
{
    return (m_nSecondaryNodes);
}

void LocalClusterConfigObjectManagerReportPrimaryNodeChangedMessage::setSecondaryNodeNameAndPort (const string &nodeName, const SI32 &nodePort)
{
    UI32        i;
    for ( i = 0; i < m_secondaryNodeNames.size (); i++)
    {
        if ( (m_secondaryNodeNames [i] == nodeName) && (m_secondaryNodePorts [i] == nodePort))
        {
            return;
        }
    }

    m_secondaryNodeNames.push_back (nodeName);
    m_secondaryNodePorts.push_back (nodePort);
    //m_secondaryNodeNames [index]    =   nodeName;
}

string LocalClusterConfigObjectManagerReportPrimaryNodeChangedMessage::getSecondaryNodeName (const UI32 &index)
{
    if (index < m_secondaryNodeNames.size ())
    {
        return (m_secondaryNodeNames[index]);
    }
    else
    {
        WaveNs::trace (TRACE_LEVEL_ERROR, "LocalClusterConfigObjectManagerReportPrimaryNodeChangedMessage::getSecondaryNode: Invalid Index.");
        return 0;
    }
}

SI32 LocalClusterConfigObjectManagerReportPrimaryNodeChangedMessage::getSecondaryNodePort (const UI32 &index)
{
    if (index < m_secondaryNodePorts.size ())
    {
        return (m_secondaryNodePorts[index]);
    }
    else
    {
        WaveNs::trace (TRACE_LEVEL_ERROR, "LocalClusterConfigObjectManagerReportPrimaryNodeChangedMessage::getSecondaryNodePort: Invalid Index.");
        return 0;
    }
}

void LocalClusterConfigObjectManagerReportPrimaryNodeChangedMessage::setThisNodeName (const string &thisNodeName)
{
    m_thisNodeName = thisNodeName;
}

string LocalClusterConfigObjectManagerReportPrimaryNodeChangedMessage::getThisNodeName () const
{
    return (m_thisNodeName);
}

void LocalClusterConfigObjectManagerReportPrimaryNodeChangedMessage::setThisNodePort (const SI32 &thisNodePort)
{
    m_thisNodePort = thisNodePort;
}

SI32 LocalClusterConfigObjectManagerReportPrimaryNodeChangedMessage::getThisNodePort () const
{
    return (m_thisNodePort);
}

LocalClusterConfigObjectManagerReportRemovedNodeFromClusterMessage::LocalClusterConfigObjectManagerReportRemovedNodeFromClusterMessage ()
    : PrismMessage (LocalClusterConfigObjectManager::getPrismServiceId (), CLUSTER_REPORT_REMOVED_NODE_FROM_CLUSTER)
{
}

LocalClusterConfigObjectManagerReportRemovedNodeFromClusterMessage::~LocalClusterConfigObjectManagerReportRemovedNodeFromClusterMessage ()
{
}

#if 0
LocalClusterConfigObjectManagerReportCenteralHeartBeatMessage::LocalClusterConfigObjectManagerReportCenteralHeartBeatMessage ()
    : PrismMessage (LocalClusterConfigObjectManager::getPrismServiceId (), CLUSTER_REPORT_CENTERAL_HEART_BEAT)
{
}

LocalClusterConfigObjectManagerReportCenteralHeartBeatMessage::~LocalClusterConfigObjectManagerReportCenteralHeartBeatMessage ()
{
}

void LocalClusterConfigObjectManagerReportCenteralHeartBeatMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeUI32    (&m_heartBeatNum,   "heartBeatNum"));
     addSerializableAttribute (new AttributeString  (&m_nodeName,       "nodeName"));
}

void LocalClusterConfigObjectManagerReportCenteralHeartBeatMessage::setHeartBeatNum (const UI32 heartBeatNum)
{
    m_heartBeatNum  =   heartBeatNum;
}

UI32 LocalClusterConfigObjectManagerReportCenteralHeartBeatMessage::getHeartBeatNum ()
{
    return (m_heartBeatNum);
}

void LocalClusterConfigObjectManagerReportCenteralHeartBeatMessage::setNodeName (const string nodeName)
{
    m_nodeName  =   nodeName;
}


string LocalClusterConfigObjectManagerReportCenteralHeartBeatMessage::getNodeName ()
{
    return (m_nodeName);
}


CenteralClusterConfigObjectManagerReportLocalHeartBeatMessage::CenteralClusterConfigObjectManagerReportLocalHeartBeatMessage ()
    : PrismMessage (CentralClusterConfigObjectManager::getPrismServiceId (), CLUSTER_REPORT_LOCAL_HEART_BEAT)
{
}

CenteralClusterConfigObjectManagerReportLocalHeartBeatMessage::~CenteralClusterConfigObjectManagerReportLocalHeartBeatMessage ()
{
}


void CenteralClusterConfigObjectManagerReportLocalHeartBeatMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeUI32 (&m_heartBeatNum, "heartBeatNum"));
     addSerializableAttribute (new AttributeString (&m_nodeName, "nodeName"));
}

void CenteralClusterConfigObjectManagerReportLocalHeartBeatMessage::setHeartBeatNum (const UI32 heartBeatNum)
{
    m_heartBeatNum  =   heartBeatNum;
}

UI32 CenteralClusterConfigObjectManagerReportLocalHeartBeatMessage::getHeartBeatNum ()
{
    return (m_heartBeatNum);
}

void CenteralClusterConfigObjectManagerReportLocalHeartBeatMessage::setNodeName (const string nodeName)
{
    m_nodeName  =   nodeName;
}


string CenteralClusterConfigObjectManagerReportLocalHeartBeatMessage::getNodeName ()
{
    return (m_nodeName);
}


LocalClusterConfigObjectManagerSetHeartBeatConfigMessage::LocalClusterConfigObjectManagerSetHeartBeatConfigMessage ()
    : PrismMessage (LocalClusterConfigObjectManager::getPrismServiceId (), CLUSTER_UPDATE_HEARTBEAT_CONFIG)
{
}

LocalClusterConfigObjectManagerSetHeartBeatConfigMessage::~LocalClusterConfigObjectManagerSetHeartBeatConfigMessage ()
{
}

void LocalClusterConfigObjectManagerSetHeartBeatConfigMessage::setHeartBeatInterval (const UI32 heartBeatInterval)
{
    m_heartBeatInterval =   heartBeatInterval;
}

UI32 LocalClusterConfigObjectManagerSetHeartBeatConfigMessage::getHeartBeatInterval ()
{
    return (m_heartBeatInterval);
}

void LocalClusterConfigObjectManagerSetHeartBeatConfigMessage::setNLostHeartBeatsToNotify (const UI32 nLostHeartBeatToNotify)
{
    m_nLostHeartBeatToNotify    =   nLostHeartBeatToNotify;
}

UI32 LocalClusterConfigObjectManagerSetHeartBeatConfigMessage::getNLostHeartBeatsToNotify ()
{
    return (m_nLostHeartBeatToNotify);
}
#endif

LocalClusterConfigObjectManagerGetNodeRoleMessage::LocalClusterConfigObjectManagerGetNodeRoleMessage ()
    : PrismMessage (LocalClusterConfigObjectManager::getPrismServiceId (), CLUSTER_GET_NODE_ROLE),
      m_nodeRole (0)
{
}

LocalClusterConfigObjectManagerGetNodeRoleMessage::~LocalClusterConfigObjectManagerGetNodeRoleMessage ()
{
}

void LocalClusterConfigObjectManagerGetNodeRoleMessage::setNodeRole (const UI32 nodeRole)
{
    m_nodeRole =   nodeRole;
}

UI32 LocalClusterConfigObjectManagerGetNodeRoleMessage::getNodeRole ()
{
    return (m_nodeRole);
}


LocalClusterConfigObjectManagerGetClusterConfigMessage::LocalClusterConfigObjectManagerGetClusterConfigMessage ()
    : PrismMessage (LocalClusterConfigObjectManager::getPrismServiceId (), CLUSTER_GET_CLUSTER_CONFIG),
      m_primaryNodeStatus (0),
      m_nSecondaryNodes(0)
{
}

LocalClusterConfigObjectManagerGetClusterConfigMessage::~LocalClusterConfigObjectManagerGetClusterConfigMessage ()
{
}

string LocalClusterConfigObjectManagerGetClusterConfigMessage::getPrimaryNodeName ()
{
    return (m_primaryNodeName);
}

UI32 LocalClusterConfigObjectManagerGetClusterConfigMessage::getPrimaryNodePort ()
{
    return (m_primaryNodePort);
}

UI32 LocalClusterConfigObjectManagerGetClusterConfigMessage::getPrimaryNodeStatus ()
{
    return (m_primaryNodeStatus);
}

UI32 LocalClusterConfigObjectManagerGetClusterConfigMessage::getNSecondaryNodes ()
{
    return (m_nSecondaryNodes);
}

string LocalClusterConfigObjectManagerGetClusterConfigMessage::getSecondaryNodeName (UI32 index)
{
    if (index < m_SecondaryNodeStatus.size ())
    {
        return (m_SecondaryNodeStatus [index].nodeName);
    }
    else
    {
        WaveNs::trace (TRACE_LEVEL_ERROR, "LocalClusterConfigObjectManagerGetClusterConfigMessage::getSecondaryNodeName: Invalid Index.");
        return 0;
    }
}

UI32 LocalClusterConfigObjectManagerGetClusterConfigMessage::getSecondaryNodePort (UI32 index)
{
    if (index < m_SecondaryNodeStatus.size ())
    {
        return (m_SecondaryNodeStatus [index].nodePort);
    }
    else
    {
        WaveNs::trace (TRACE_LEVEL_ERROR, "LocalClusterConfigObjectManagerGetClusterConfigMessage::getSecondaryNodePort: Invalid Index.");
        return 0;
    }
}

UI32 LocalClusterConfigObjectManagerGetClusterConfigMessage::getSecondaryNodeStatus (UI32 index)
{
    if (index < m_SecondaryNodeStatus.size ())
    {
        return (m_SecondaryNodeStatus [index].nodeStatus);
    }
    else
    {
        WaveNs::trace (TRACE_LEVEL_ERROR, "LocalClusterConfigObjectManagerGetClusterConfigMessage::getSecondaryNodeStatus: Invalid Index.");
        return 0;
    }
}

void LocalClusterConfigObjectManagerGetClusterConfigMessage::setPrimaryNodeName (string nodeName)
{
    m_primaryNodeName = nodeName;
}

void LocalClusterConfigObjectManagerGetClusterConfigMessage::setPrimaryNodePort (SI32 nodePort)
{
    m_primaryNodePort = nodePort;
}

void LocalClusterConfigObjectManagerGetClusterConfigMessage::setPrimaryNodeStatus (UI32 status)
{
    m_primaryNodeStatus = status;
}

void LocalClusterConfigObjectManagerGetClusterConfigMessage::setNSecondaryNodes (UI32 nSecondaryNodes)
{
    m_nSecondaryNodes = nSecondaryNodes;
}

void LocalClusterConfigObjectManagerGetClusterConfigMessage::setSecondaryNodeStatus (string nodeName, SI32 nodePort, SI32 status)
{
    nodeStatus_t    node;
    UI32            i;

    for ( i = 0; i < m_SecondaryNodeStatus.size (); i++)
    {
        if ( (m_SecondaryNodeStatus [i].nodeName == nodeName) && (m_SecondaryNodeStatus [i].nodePort == (UI32) nodePort))
        {
            m_SecondaryNodeStatus [i].nodeStatus = status;
            return;
        }
    }

    node.nodeName   = nodeName;
    node.nodePort   = nodePort;
    node.nodeStatus = status;
    m_SecondaryNodeStatus.push_back (node);
}



HeartBeatLostMessage::HeartBeatLostMessage (/* PrismServiceId id, */string dstIpAddress, UI32 dstPort /*, PrismElement *pStartHeartBeatSender */)
    : PrismMessage (LocalClusterConfigObjectManager::getPrismServiceId () /*id*/, CLUSTER_HEARTBEAT_LOST)
{
    m_dstIpAddress          =   dstIpAddress;
    m_dstPort               =   dstPort;
//    m_pStartHeartBeatSender =   pStartHeartBeatSender;

}

HeartBeatLostMessage::~HeartBeatLostMessage ()
{
}

string HeartBeatLostMessage::getDstIpAddress ()
{
    return (m_dstIpAddress);
}

void HeartBeatLostMessage::setDstIpAddress (string dstIpAddress)
{
    m_dstIpAddress   = dstIpAddress;
}


UI32 HeartBeatLostMessage::getDstPort()
{
    return (m_dstPort);
}

void HeartBeatLostMessage::setDstPort (UI32 dstPort)
{
    m_dstPort   = dstPort;
}

HeartBeatResumedMessage::HeartBeatResumedMessage (/*PrismServiceId id, */string dstIpAddress, UI32 dstPort /*, PrismElement *pStartHeartBeatSender */)
    : PrismMessage (LocalClusterConfigObjectManager::getPrismServiceId () /*id*/, CLUSTER_HEARTBEAT_RESUMED)
{
    m_dstIpAddress          =   dstIpAddress;
    m_dstPort               =   dstPort;
//    m_pStartHeartBeatSender =   pStartHeartBeatSender;

}

HeartBeatResumedMessage::~HeartBeatResumedMessage ()
{
}

string HeartBeatResumedMessage::getDstIpAddress ()
{
    return (m_dstIpAddress);
}

void HeartBeatResumedMessage::setDstIpAddress (string dstIpAddress)
{
    m_dstIpAddress   = dstIpAddress;
}

UI32 HeartBeatResumedMessage::getDstPort()
{
    return (m_dstPort);
}

void HeartBeatResumedMessage::setDstPort (UI32 dstPort)
{
    m_dstPort   = dstPort;
}


//
//Name:
//ClusterConfigObjectManagerGetClusterInfoMessage
//
//Description:
//Constructor
//
//Input/s:
//None
//Output/s:
//None
//Returns:
//None
//

ClusterConfigObjectManagerGetClusterInfoMessage::ClusterConfigObjectManagerGetClusterInfoMessage()
:PrismMessage(CentralClusterConfigObjectManager::getPrismServiceId (),CLUSTER_GET_DEBUGINFO),
m_ClusterCreated(false),
m_primaryNodePort(0),
m_primaryNodeLocationId(0),
m_primaryNodeGenericStatus(0),
m_primaryNodeSpecificStatus(0),
m_nSecondaryNodes(0)
{
}


//
//Name:
//~ClusterConfigObjectManagerGetClusterInfoMessage
//
//Description:
//Destructor
//
//Input/s:
//None
//Output/s:
//None
//Returns:
//None
//

ClusterConfigObjectManagerGetClusterInfoMessage::~ClusterConfigObjectManagerGetClusterInfoMessage()
{

}


///
/// Cluster Creation
///

bool ClusterConfigObjectManagerGetClusterInfoMessage::isClusterCreated() const
{
  return m_ClusterCreated;
}

void ClusterConfigObjectManagerGetClusterInfoMessage::setClusterCreated(const bool & clusterCreated)
{
    m_ClusterCreated  = clusterCreated;

}

///
//Get and Set Functions for Primary Node
///

string ClusterConfigObjectManagerGetClusterInfoMessage::getPrimaryNodeIpAddress() const
{
    return m_primaryNodeIpAddress;
}

void ClusterConfigObjectManagerGetClusterInfoMessage::setPrimaryNodeIpAddress(const string & nodeIpAddress)
{
    m_primaryNodeIpAddress = nodeIpAddress;
}

UI32 ClusterConfigObjectManagerGetClusterInfoMessage::getPrimaryNodePort() const
{
  return m_primaryNodePort;
}

void ClusterConfigObjectManagerGetClusterInfoMessage::setPrimaryNodePort(const UI32 & nodePort)
{
    m_primaryNodePort = nodePort;
}

LocationId ClusterConfigObjectManagerGetClusterInfoMessage::getPrimaryNodeLocationId() const
{
    return m_primaryNodeLocationId;
}
void ClusterConfigObjectManagerGetClusterInfoMessage::setPrimaryNodeLocationId(const LocationId & locationId)
{
    m_primaryNodeLocationId = locationId;
}



UI32 ClusterConfigObjectManagerGetClusterInfoMessage::getPrimaryNodeGenericStatus() const
{
    return  m_primaryNodeGenericStatus;
}

void ClusterConfigObjectManagerGetClusterInfoMessage::setPrimaryNodeGenericStatus(const ResourceId & status)
{
    m_primaryNodeGenericStatus = status;
}

ResourceId ClusterConfigObjectManagerGetClusterInfoMessage::getPrimaryNodeSpecificStatus() const
{
    return  m_primaryNodeSpecificStatus;
}

void ClusterConfigObjectManagerGetClusterInfoMessage::setPrimaryNodeSpecificStatus(const ResourceId & status)
{
    m_primaryNodeSpecificStatus = status;
}

///
//get and set methods for Secondary Nodes
///

UI32 ClusterConfigObjectManagerGetClusterInfoMessage::getNSecondaryNodes() const
{
    return m_nSecondaryNodes;
}

void ClusterConfigObjectManagerGetClusterInfoMessage::setNSecondaryNodes(const UI32 & nSecondaryNodes)
{
  m_nSecondaryNodes = nSecondaryNodes;
}

void ClusterConfigObjectManagerGetClusterInfoMessage::setSecondaryNodeDebugInfo(const string & secondaryNodeIpAddress,
										const UI32 &  secondaryNodePort,
										const LocationId & secondaryNodeLocationId,
										const ResourceId & secondaryNodeGenericStatus,
										const ResourceId & secondaryNodeSpecificStatus)
{
    UI32 numberOfElements = m_secondaryNodeIPAddresses.size();

    for(UI32 i=0;i<numberOfElements;++i)
    {
	if(((m_secondaryNodeIPAddresses[i]== secondaryNodeIpAddress ) && (m_secondaryNodePorts[i] == secondaryNodePort))){
	    tracePrintf(TRACE_LEVEL_FATAL,"Duplicate IP and Port Found: %s:%d",secondaryNodeIpAddress.c_str(),secondaryNodePort);
	}

    }


    m_secondaryNodeIPAddresses.push_back(secondaryNodeIpAddress);
    m_secondaryNodePorts.push_back(secondaryNodePort);
    m_secondaryNodeLocationIds.push_back(secondaryNodeLocationId);
    m_secondaryNodeGenericStatus.push_back(secondaryNodeGenericStatus);
    m_secondaryNodeSpecificStatus.push_back(secondaryNodeSpecificStatus);
}


/// Name
/// getSecondaryNodeDebugInfo
/// Description:
/// When passed an index to the vector,
/// returns the data from each of the vectors
/// containing info related to secondary nodes
/// Input
/// index of vector
/// Output
/// IP,Port,LocationId,Status
/// Return
/// void

void ClusterConfigObjectManagerGetClusterInfoMessage::getSecondaryNodeDebugInfo(const UI32 & index,
										string & secondaryNodeIpAddress,
										UI32 & secondaryNodePort,
										LocationId & secondaryNodeLocationId,
										ResourceId & secondaryNodeGenericStatus,
										ResourceId & secondaryNodeSpecificStatus) const
{
   prismAssert(index<m_secondaryNodeIPAddresses.size(),__FILE__,__LINE__);

   secondaryNodeIpAddress = m_secondaryNodeIPAddresses[index];
   secondaryNodePort = m_secondaryNodePorts[index];
   secondaryNodeLocationId = m_secondaryNodeLocationIds[index];
   secondaryNodeGenericStatus = m_secondaryNodeGenericStatus[index];
   secondaryNodeSpecificStatus = m_secondaryNodeSpecificStatus[index];
}

/// Name:
/// setupAttributesForSerialization
/// Description:
/// This message is a prismMessage each prismMessage is a serilizableObject
/// that has a map to store the serializable attributes. Thi method is putting the
/// attributes into the map
/// Input
/// none
/// Output
/// none
/// Return
/// none

void ClusterConfigObjectManagerGetClusterInfoMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeBool   (&m_ClusterCreated, "ClusterCreated"));
     addSerializableAttribute (new AttributeString (&m_primaryNodeIpAddress, "primaryNodeIpAddress"));
     addSerializableAttribute (new AttributeUI32   (&m_primaryNodePort, "primaryNodePort"));
     addSerializableAttribute (new AttributeLocationId   (&m_primaryNodeLocationId, "primaryNodeLocationId"));
     addSerializableAttribute (new AttributeResourceId   (&m_primaryNodeGenericStatus, "primaryNodeGenericStatus"));
     addSerializableAttribute (new AttributeResourceId   (&m_primaryNodeSpecificStatus, "primaryNodeSpecificStatus"));
     addSerializableAttribute (new AttributeUI32   (&m_nSecondaryNodes, "nSecondaryNodes"));
     addSerializableAttribute (new AttributeStringVector (&m_secondaryNodeIPAddresses, "secondaryNodeIPAddresses"));
     addSerializableAttribute (new AttributeUI32Vector (&m_secondaryNodePorts, "secondaryNodePorts"));
     addSerializableAttribute (new AttributeLocationIdVector (&m_secondaryNodeLocationIds, "secondaryNodeLocationIds"));
     addSerializableAttribute (new AttributeResourceIdVector (&m_secondaryNodeGenericStatus, "secondaryNodeGenericStatus"));
     addSerializableAttribute (new AttributeResourceIdVector (&m_secondaryNodeSpecificStatus, "secondaryNodeSpecificStatus"));
}


/**
 * Description: CentralClusterConfigUpdateHardwareSynchronizationStateMessage
 *              Definition.
 *
 * Author :     Brian Adaniya
 * Date :       05/05/2011
 */
CentralClusterConfigUpdateHardwareSynchronizationStateMessage::CentralClusterConfigUpdateHardwareSynchronizationStateMessage ()
    : PrismMessage (CentralClusterConfigObjectManager::getPrismServiceId (), CENTRAL_CLUSTER_CONFIG_UPDATE_HARDWARE_SYNCHRONIZATION_STATE),
    m_hardwareSynchronizationState  (WAVE_NODE_HARDWARE_SYNCHRONIZATION_STATE_UNINITIALIZED)
{
}

CentralClusterConfigUpdateHardwareSynchronizationStateMessage::CentralClusterConfigUpdateHardwareSynchronizationStateMessage (ResourceId hardwareSynchronizationState, const vector<LocationId> &locationIds)
    : PrismMessage (CentralClusterConfigObjectManager::getPrismServiceId (), CENTRAL_CLUSTER_CONFIG_UPDATE_HARDWARE_SYNCHRONIZATION_STATE),
    m_hardwareSynchronizationState  (hardwareSynchronizationState),
    m_locationIds                   (locationIds)
{
}

CentralClusterConfigUpdateHardwareSynchronizationStateMessage::~CentralClusterConfigUpdateHardwareSynchronizationStateMessage ()
{
}

void CentralClusterConfigUpdateHardwareSynchronizationStateMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeResourceId       (&m_hardwareSynchronizationState, "hardwareSynchronizationState"));
     addSerializableAttribute (new AttributeLocationIdVector (&m_locationIds, "locationIds"));
}

vector<LocationId> CentralClusterConfigUpdateHardwareSynchronizationStateMessage::getLocationIds () const
{
    return (m_locationIds);
}

void CentralClusterConfigUpdateHardwareSynchronizationStateMessage::setLocationIds (const vector<LocationId> &locationIds)
{
    m_locationIds = locationIds;
}

ResourceId CentralClusterConfigUpdateHardwareSynchronizationStateMessage::getHardwareSynchronizationState () const
{
    return (m_hardwareSynchronizationState);
}

void CentralClusterConfigUpdateHardwareSynchronizationStateMessage::setHardwareSynchronizationState (ResourceId hardwareSynchronizationState)
{
    m_hardwareSynchronizationState = hardwareSynchronizationState;
}


}


