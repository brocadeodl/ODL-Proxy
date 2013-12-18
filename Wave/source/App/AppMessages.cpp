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

namespace WaveNs
{
//
//
//
AppInterfaceObjectManagerRemoteCommandsMessage::AppInterfaceObjectManagerRemoteCommandsMessage ()
    : PrismMessage (0, APP_REMOTE_COMMANDS)
{
    m_nResults  = 0;
    m_nCommands = 0;
    m_nodePort  = 0;

}

AppInterfaceObjectManagerRemoteCommandsMessage::AppInterfaceObjectManagerRemoteCommandsMessage (PrismServiceId serviceId)
    : PrismMessage (serviceId, APP_REMOTE_COMMANDS)
{
    m_nResults  = 0;
    m_nCommands = 0;
    m_nodePort  = 0;
}

AppInterfaceObjectManagerRemoteCommandsMessage::~AppInterfaceObjectManagerRemoteCommandsMessage ()
{
}

void AppInterfaceObjectManagerRemoteCommandsMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeUI32 (&m_nResults,  "nResults"));
     addSerializableAttribute (new AttributeUI32 (&m_nCommands, "nCommands"));
     addSerializableAttribute (new AttributeString (&m_nodeName,"nodeName"));
     addSerializableAttribute (new AttributeUI32 (&m_nodePort,  "nodePort"));
}

void AppInterfaceObjectManagerRemoteCommandsMessage::setNodeName (string nodeName)
{
    m_nodeName = nodeName;
}

void AppInterfaceObjectManagerRemoteCommandsMessage::setNodePort (UI32 nodePort)
{
    m_nodePort = nodePort;
}

void AppInterfaceObjectManagerRemoteCommandsMessage::setNCommands (UI32 nCommands)
{
    m_nCommands = nCommands;
}

UI32 AppInterfaceObjectManagerRemoteCommandsMessage::getNCommands ()
{
    return (m_nCommands);
}

void AppInterfaceObjectManagerRemoteCommandsMessage::setNResults (UI32 nResults)
{
    m_nResults = nResults;
}

UI32 AppInterfaceObjectManagerRemoteCommandsMessage::getNResults ()
{
    return (m_nResults);
}

string AppInterfaceObjectManagerRemoteCommandsMessage::getNodeName ()
{
    return (m_nodeName);
}


UI32 AppInterfaceObjectManagerRemoteCommandsMessage::getNodePort ()
{
    return (m_nodePort);
}

void AppInterfaceObjectManagerRemoteCommandsMessage::setDuplicateCmdInputBuffer (UI32 bufferNum, UI32 size, void *pBuffer)
{
    addBuffer (DUPLICATE_CMD_INPUT_BUFFER + bufferNum, size, pBuffer, false);

}

void AppInterfaceObjectManagerRemoteCommandsMessage::getDuplicateCmdInputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer)
{
    pBuffer   =   findBuffer(DUPLICATE_CMD_INPUT_BUFFER + bufferNum, size);
}

void AppInterfaceObjectManagerRemoteCommandsMessage::transferCmdInputBufferToUser (UI32 bufferNum, UI32 &size, void *&pBuffer)
{
    pBuffer = transferBufferToUser (DUPLICATE_CMD_INPUT_BUFFER + bufferNum, size);
}

void AppInterfaceObjectManagerRemoteCommandsMessage::setDuplicateCmdOuputBuffer (UI32 bufferNum, UI32 size, void *pBuffer)
{
    addBuffer (DUPLICATE_CMD_OUTPUT_BUFFER + bufferNum, size, pBuffer, false);
}

void AppInterfaceObjectManagerRemoteCommandsMessage::getDuplicateCmdOuputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer)
{
    pBuffer   =   findBuffer(DUPLICATE_CMD_OUTPUT_BUFFER + bufferNum, size);
}

void AppInterfaceObjectManagerRemoteCommandsMessage::transferOutputBufferToUser (UI32 bufferNum, UI32 &size, void *&pBuffer)
{
    pBuffer = transferBufferToUser (DUPLICATE_CMD_OUTPUT_BUFFER + bufferNum, size);
}

void *AppInterfaceObjectManagerRemoteCommandsMessage::transferOutputBufferToUser (UI32 &size)
{
    return transferBufferToUser (DUPLICATE_CMD_OUTPUT_BUFFER, size);
}

void AppInterfaceObjectManagerRemoteCommandsMessage::setDuplicateCmdOuputBuffer (UI32 size, void *pBuffer)
{
    addBuffer (DUPLICATE_CMD_OUTPUT_BUFFER, size, pBuffer, false);
}

void AppInterfaceObjectManagerRemoteCommandsMessage::getDuplicateCmdOuputBuffer (UI32 &size, void *&pBuffer)
{
    pBuffer   =   findBuffer(DUPLICATE_CMD_OUTPUT_BUFFER, size);
}

void AppInterfaceObjectManagerRemoteCommandsMessage::transferOutputBufferToUser (UI32 &size, void *&pBuffer)
{
    pBuffer = transferBufferToUser (DUPLICATE_CMD_OUTPUT_BUFFER, size);
}



ClusterInterfaceObjectManagerCreateClusterDoneMessage::ClusterInterfaceObjectManagerCreateClusterDoneMessage ()
    : PrismMessage (0, APP_INTERFACE_CREATE_CLUSTER_DONE)
{
    m_nSecondaryNodes = 0;
    m_operationStatus = 0;
}

ClusterInterfaceObjectManagerCreateClusterDoneMessage::ClusterInterfaceObjectManagerCreateClusterDoneMessage (PrismServiceId serviceId)
    : PrismMessage (serviceId, APP_INTERFACE_CREATE_CLUSTER_DONE)
{
    m_nSecondaryNodes = 0;
    m_operationStatus = 0;
    
}

ClusterInterfaceObjectManagerCreateClusterDoneMessage::~ClusterInterfaceObjectManagerCreateClusterDoneMessage ()
{
}

void ClusterInterfaceObjectManagerCreateClusterDoneMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeString             (&m_primaryNodeName,     "primaryNodeName"));
     addSerializableAttribute (new AttributeUI32               (&m_nSecondaryNodes,     "nSecondaryNodes"));
     addSerializableAttribute (new AttributeStringVector       (&m_secondaryNodeNames,  "secondaryNodeNames"));
     addSerializableAttribute (new AttributeSI32Vector         (&m_secondaryNodePorts,  "secondaryNodePorts"));
     addSerializableAttribute (new AttributeStringVector       (&m_statusNodeNames,     "statusNodeNames"));
     addSerializableAttribute (new AttributeSI32Vector         (&m_statusNodePorts,     "statusNodePorts"));
     addSerializableAttribute (new AttributeUI32Vector         (&m_statusNodeStatus,    "statusNodeStatus"));
}

void ClusterInterfaceObjectManagerCreateClusterDoneMessage::setPrimaryNodeName (const string &primaryNodeName)
{
    m_primaryNodeName = primaryNodeName;
}

string ClusterInterfaceObjectManagerCreateClusterDoneMessage::getPrimaryNodeName ()
{
    return (m_primaryNodeName);
}

void ClusterInterfaceObjectManagerCreateClusterDoneMessage::setNSecondaryNodes (const UI32 nSecondaryNodes)
{
    m_nSecondaryNodes = nSecondaryNodes;
}

UI32 ClusterInterfaceObjectManagerCreateClusterDoneMessage::getNSecondaryNodes ()
{
    return (m_nSecondaryNodes);
}

void ClusterInterfaceObjectManagerCreateClusterDoneMessage::setSecondaryNodeNameAndPort (const string &nodeName, const SI32 &nodePort)
{
    m_secondaryNodeNames.push_back (nodeName);
    m_secondaryNodePorts.push_back (nodePort);
}

string ClusterInterfaceObjectManagerCreateClusterDoneMessage::getSecondaryNodeName (UI32 index)
{
    if (index < m_secondaryNodeNames.size ())
    {
        return ( m_secondaryNodeNames [index]);
    }
    else
    {
        WaveNs::trace (TRACE_LEVEL_ERROR, "ClusterInterfaceObjectManagerCreateClusterDoneMessage::getSecondaryNodeName: Invalid Index.");
        return 0;
    }
}

SI32 ClusterInterfaceObjectManagerCreateClusterDoneMessage::getSecondaryNodePort (UI32 index)
{
    if (index < m_secondaryNodePorts.size ())
    {
        return ( m_secondaryNodePorts [index]);
    }
    else
    {
        WaveNs::trace (TRACE_LEVEL_ERROR, "ClusterInterfaceObjectManagerCreateClusterDoneMessage::getSecondaryNodePort: Invalid Index.");
        return 0;
    }
}
/*
Node ClusterInterfaceObjectManagerCreateClusterDoneMessage::getSecondaryNode (UI32 index)
{
    return ( m_secondaryNode [index]);
}
*/


string ClusterInterfaceObjectManagerCreateClusterDoneMessage::getStatusNodeName (UI32 index)
{
    if (index < m_statusNodeNames.size ())
    {
        return (m_statusNodeNames [index]);
    }
    else
    {
        WaveNs::trace (TRACE_LEVEL_ERROR, string ("ClusterInterfaceObjectManagerCreateClusterDoneMessage::getStatusNodeName: Invalid Index.") + index);
        return 0;
    }
}

SI32 ClusterInterfaceObjectManagerCreateClusterDoneMessage::getStatusNodePort (UI32 index)
{
    if (index < m_statusNodePorts.size ())
    {
        return (m_statusNodePorts [index]);
    }
    else
    {
        WaveNs::trace (TRACE_LEVEL_ERROR, string ("ClusterInterfaceObjectManagerCreateClusterDoneMessage::getStatusNodePort: Invalid Index:") + index);
        return 0;
    }
}


UI32 ClusterInterfaceObjectManagerCreateClusterDoneMessage::getStatusNodeStatus (UI32 index)
{
    if (index < m_statusNodeStatus.size ())
    {
        return (m_statusNodeStatus [index]);
    }
    else
    {
        WaveNs::trace (TRACE_LEVEL_ERROR, string ("ClusterInterfaceObjectManagerCreateClusterDoneMessage::getNodeStatus: Invalid Index.") + index);
        return 0;
    }
}


void ClusterInterfaceObjectManagerCreateClusterDoneMessage::setNodeStaus (string nodeName, UI32 nodePort, UI32 status)
{

    m_statusNodeNames.push_back (nodeName);
    m_statusNodePorts.push_back (nodePort);
    m_statusNodeStatus.push_back (status);

}


void ClusterInterfaceObjectManagerCreateClusterDoneMessage::setCreateClusterResultCallBack (createClusterCallBack_t createClusterResultCb)
{
    m_createClusterResultCb = createClusterResultCb;
}

createClusterCallBack_t ClusterInterfaceObjectManagerCreateClusterDoneMessage::getCreateClusterResultCallBack ()
{
    return (m_createClusterResultCb);
}

void ClusterInterfaceObjectManagerCreateClusterDoneMessage::setContext (void *pContext)
{
    m_pContext = pContext;
}

void *ClusterInterfaceObjectManagerCreateClusterDoneMessage::getContext ()
{
    return (m_pContext);
}

void ClusterInterfaceObjectManagerCreateClusterDoneMessage::setOperationStatus (UI32 operationStatus)
{
    m_operationStatus = operationStatus;
}

UI32 ClusterInterfaceObjectManagerCreateClusterDoneMessage::getOperationStatus ()
{
    return (m_operationStatus);
}

ClusterInterfaceObjectManagerDeleteClusterDoneMessage::ClusterInterfaceObjectManagerDeleteClusterDoneMessage ()
    : PrismMessage (0, APP_INTERFACE_DELETE_CLUSTER_DONE),
      m_operationStatus(0)
{
}

ClusterInterfaceObjectManagerDeleteClusterDoneMessage::ClusterInterfaceObjectManagerDeleteClusterDoneMessage (PrismServiceId serviceId)
    : PrismMessage (serviceId, APP_INTERFACE_DELETE_CLUSTER_DONE),
      m_operationStatus(0)
{
}

ClusterInterfaceObjectManagerDeleteClusterDoneMessage::~ClusterInterfaceObjectManagerDeleteClusterDoneMessage ()
{
}

void ClusterInterfaceObjectManagerDeleteClusterDoneMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();
}

void ClusterInterfaceObjectManagerDeleteClusterDoneMessage::setRemoveClusterResultCallBack (removeClusterCallBack_t removeClusterResultCb)
{
    m_removeClusterResultCb = removeClusterResultCb;
}

removeClusterCallBack_t ClusterInterfaceObjectManagerDeleteClusterDoneMessage::getRemoveClusterResultCallBack ()
{
    return (m_removeClusterResultCb);
}

void ClusterInterfaceObjectManagerDeleteClusterDoneMessage::setContext (void *pContext)
{
    m_pContext = pContext;
}

void *ClusterInterfaceObjectManagerDeleteClusterDoneMessage::getContext ()
{
    return (m_pContext);
}


void ClusterInterfaceObjectManagerDeleteClusterDoneMessage::setOperationStatus (UI32 operationStatus)
{
    m_operationStatus = operationStatus;
}

UI32 ClusterInterfaceObjectManagerDeleteClusterDoneMessage::getOperationStatus ()
{
    return (m_operationStatus);
}

ClusterInterfaceObjectManagerAddNodeDoneMessage::ClusterInterfaceObjectManagerAddNodeDoneMessage ()
    : PrismMessage (0, APP_INTERFACE_ADD_NODE_DONE)
{
    m_nodePort          = 0;
    m_operationStatus   = 0;

}

ClusterInterfaceObjectManagerAddNodeDoneMessage::ClusterInterfaceObjectManagerAddNodeDoneMessage (PrismServiceId serviceId)
    : PrismMessage (serviceId, APP_INTERFACE_ADD_NODE_DONE)
{
    m_nodePort          = 0;
    m_operationStatus   = 0;
}

ClusterInterfaceObjectManagerAddNodeDoneMessage::~ClusterInterfaceObjectManagerAddNodeDoneMessage ()
{
}


void ClusterInterfaceObjectManagerAddNodeDoneMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeString (&m_nodeName, "nodeName"));
     addSerializableAttribute (new AttributeSI32   (&m_nodePort, "nodePort"));
}

void ClusterInterfaceObjectManagerAddNodeDoneMessage::setNodeName (const string &nodeName)
{
    m_nodeName  =   nodeName;
}

string ClusterInterfaceObjectManagerAddNodeDoneMessage::getNodeName ()
{
    return (m_nodeName);
}

void ClusterInterfaceObjectManagerAddNodeDoneMessage::setNodePort (const SI32 &nodePort)
{
    m_nodePort = nodePort;
}

SI32 ClusterInterfaceObjectManagerAddNodeDoneMessage::getNodePort ()
{
    return (m_nodePort);
}

 void ClusterInterfaceObjectManagerAddNodeDoneMessage::setAddNodeResultCallBack (addNodeCallBack_t addNodeResultCb)
{
    m_addNodeResultCb = addNodeResultCb;
}

addNodeCallBack_t ClusterInterfaceObjectManagerAddNodeDoneMessage::getAddNodeResultCallBack ()
{
    return (m_addNodeResultCb);
}

void ClusterInterfaceObjectManagerAddNodeDoneMessage::setContext (void *pContext)
{
    m_pContext = pContext;
}

void *ClusterInterfaceObjectManagerAddNodeDoneMessage::getContext ()
{
    return (m_pContext);
}


void ClusterInterfaceObjectManagerAddNodeDoneMessage::setOperationStatus (UI32 operationStatus)
{
    m_operationStatus = operationStatus;
}

UI32 ClusterInterfaceObjectManagerAddNodeDoneMessage::getOperationStatus ()
{
    return (m_operationStatus);
}


ClusterInterfaceObjectManagerJoinNodeDoneMessage::ClusterInterfaceObjectManagerJoinNodeDoneMessage ()
    : PrismMessage (0, APP_INTERFACE_JOIN_NODE_DONE)
{
    m_nodePort          = 0;
    m_operationStatus   = 0;

}

ClusterInterfaceObjectManagerJoinNodeDoneMessage::ClusterInterfaceObjectManagerJoinNodeDoneMessage (PrismServiceId serviceId)
    : PrismMessage (serviceId, APP_INTERFACE_JOIN_NODE_DONE)
{
    m_nodePort          = 0;
    m_operationStatus   = 0;
}

ClusterInterfaceObjectManagerJoinNodeDoneMessage::~ClusterInterfaceObjectManagerJoinNodeDoneMessage ()
{
}


void ClusterInterfaceObjectManagerJoinNodeDoneMessage::setNodeName (const string &nodeName)
{
    m_nodeName  =   nodeName;
}

string ClusterInterfaceObjectManagerJoinNodeDoneMessage::getNodeName ()
{
    return (m_nodeName);
}

void ClusterInterfaceObjectManagerJoinNodeDoneMessage::setNodePort (const SI32 &nodePort)
{
    m_nodePort = nodePort;
}

SI32 ClusterInterfaceObjectManagerJoinNodeDoneMessage::getNodePort ()
{
    return (m_nodePort);
}

void ClusterInterfaceObjectManagerJoinNodeDoneMessage::setJoinNodeResultCallBack (joinNodeCallBack_t joinNodeResultCb)
{
    m_joinNodeResultCb = joinNodeResultCb;
}

joinNodeCallBack_t ClusterInterfaceObjectManagerJoinNodeDoneMessage::getJoinNodeResultCallBack ()
{
    return (m_joinNodeResultCb);
}

void ClusterInterfaceObjectManagerJoinNodeDoneMessage::setContext (void *pContext)
{
    m_pContext = pContext;
}

void *ClusterInterfaceObjectManagerJoinNodeDoneMessage::getContext ()
{
    return (m_pContext);
}


void ClusterInterfaceObjectManagerJoinNodeDoneMessage::setOperationStatus (UI32 operationStatus)
{
    m_operationStatus = operationStatus;
}

UI32 ClusterInterfaceObjectManagerJoinNodeDoneMessage::getOperationStatus ()
{
    return (m_operationStatus);
}

ClusterInterfaceObjectManagerDeleteNodeDoneMessage::ClusterInterfaceObjectManagerDeleteNodeDoneMessage ()
    : PrismMessage (0, APP_INTERFACE_DELETE_NODE_DONE)
{
    m_nodePort          = 0;
    m_operationStatus   = 0;
}

ClusterInterfaceObjectManagerDeleteNodeDoneMessage::ClusterInterfaceObjectManagerDeleteNodeDoneMessage (PrismServiceId serviceId)
    : PrismMessage (serviceId, APP_INTERFACE_DELETE_NODE_DONE)
{
    m_nodePort          = 0;
    m_operationStatus   = 0;

}


ClusterInterfaceObjectManagerDeleteNodeDoneMessage::~ClusterInterfaceObjectManagerDeleteNodeDoneMessage ()
{
}

void ClusterInterfaceObjectManagerDeleteNodeDoneMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeString (&m_nodeName, "nodeName"));
     addSerializableAttribute (new AttributeSI32   (&m_nodePort, "nodePort"));
}

void ClusterInterfaceObjectManagerDeleteNodeDoneMessage::setNodeName (const string &nodeName)
{
    m_nodeName  =   nodeName;
}

string ClusterInterfaceObjectManagerDeleteNodeDoneMessage::getNodeName ()
{
    return (m_nodeName);
}

void ClusterInterfaceObjectManagerDeleteNodeDoneMessage::setNodePort (const SI32 &nodePort)
{
    m_nodePort = nodePort;
}

SI32 ClusterInterfaceObjectManagerDeleteNodeDoneMessage::getNodePort ()
{
    return (m_nodePort);
}

void ClusterInterfaceObjectManagerDeleteNodeDoneMessage::setRemoveNodeResultCallBack (removeNodeCallBack_t removeNodeResultCb)
{
    m_removeNodeResultCb = removeNodeResultCb;
}

removeNodeCallBack_t ClusterInterfaceObjectManagerDeleteNodeDoneMessage::getRemoveNodeResultCallBack ()
{
    return (m_removeNodeResultCb);
}

void ClusterInterfaceObjectManagerDeleteNodeDoneMessage::setContext (void *pContext)
{
    m_pContext = pContext;
}

void *ClusterInterfaceObjectManagerDeleteNodeDoneMessage::getContext ()
{
    return (m_pContext);
}


void ClusterInterfaceObjectManagerDeleteNodeDoneMessage::setOperationStatus (UI32 operationStatus)
{
    m_operationStatus = operationStatus;
}

UI32 ClusterInterfaceObjectManagerDeleteNodeDoneMessage::getOperationStatus ()
{
    return (m_operationStatus);
}

}
