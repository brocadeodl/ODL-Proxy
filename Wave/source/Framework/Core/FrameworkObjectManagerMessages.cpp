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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/Core/FrameworkObjectManagerMessages.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Attributes/AttributeResourceId.h"
#include "Framework/Attributes/AttributeSerializableObject.cpp"

namespace WaveNs
{

PrismCreateClusterWithNodesMessage::PrismCreateClusterWithNodesMessage (PrismServiceId serviceCode, UI32 operationCode)
    : PrismMessage (serviceCode, operationCode),
      m_currentValidtionResultsBufferTag (0),
      m_isAddNodeForSpecialCase (false)
{
}

PrismCreateClusterWithNodesMessage::PrismCreateClusterWithNodesMessage ()
    : PrismMessage (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_CREATE_CLUSTER_WITH_NODES),
      m_currentValidtionResultsBufferTag (0),
      m_isAddNodeForSpecialCase (false)
{
}

PrismCreateClusterWithNodesMessage::~PrismCreateClusterWithNodesMessage ()
{
}

bool PrismCreateClusterWithNodesMessage::isNewNodeIpAddressExists (const string &newNodeIpAddress, const SI32 &newNodePort)
{
    bool exists              = false;
    UI32 numberOfIpAddresses = m_newNodesIpAddresses.size ();
    UI32 i                   = 0;

    for (i = 0; i < numberOfIpAddresses; i++)
    {
        if ((newNodeIpAddress == m_newNodesIpAddresses[i]) && (newNodePort == m_newNodesPorts[i]))
        {
            exists = true;
            break;
        }
    }

    return (exists);
}

void PrismCreateClusterWithNodesMessage::addNewNodeIpAddressAndPort (const string &newNodeIpAddress, const SI32 &newNodePort)
{
    if (false == (isNewNodeIpAddressExists (newNodeIpAddress, newNodePort)))
    {
        m_newNodesIpAddresses.push_back (newNodeIpAddress);
        m_newNodesPorts.push_back (newNodePort);
    }
    else
    {
        // Adding duplicate ip addresses.

        trace (TRACE_LEVEL_FATAL, "PrismCreateClusterWithNodesMessage::addNewNodeIpAddressAndPort : Adding duplicate IP Address and Port.");
        prismAssert (false, __FILE__, __LINE__);
    }
}

UI32 PrismCreateClusterWithNodesMessage::getNumberOfNewNodes ()
{
    return (m_newNodesIpAddresses.size ());
}

string PrismCreateClusterWithNodesMessage::getNodeAt (UI32 i)
{
    if (i < (m_newNodesIpAddresses.size ()))
    {
        return (m_newNodesIpAddresses[i]);
    }
    else
    {
        // index out of range.

        trace (TRACE_LEVEL_FATAL, string ("PrismCreateClusterWithNodesMessage::getNodeAt : Index out of range, i = ") + i + string (" , Max Allowed = ") + m_newNodesIpAddresses.size ());
        prismAssert (false, __FILE__, __LINE__);

        return (string (""));
    }
}

SI32 PrismCreateClusterWithNodesMessage::getNodePortAt (UI32 i)
{
    if (i < (m_newNodesPorts.size ()))
    {
        return (m_newNodesPorts[i]);
    }
    else
    {
        // index out of range.

        trace (TRACE_LEVEL_FATAL, string ("PrismCreateClusterWithNodesMessage::getNodePortAt : Index out of range, i = ") + i + string (" , Max Allowed = ") + m_newNodesPorts.size ());
        prismAssert (false, __FILE__, __LINE__);

        return (0);
    }
}

void PrismCreateClusterWithNodesMessage::setNewNodeStatus (const string &newNodeIpAddress, const SI32 &newNodePort, ResourceId status)
{
    string                            uniqueTag = newNodeIpAddress + "#" + newNodePort;
    map<string, ResourceId>::iterator element   = m_newNodesStatus.find (uniqueTag);
    map<string, ResourceId>::iterator end       = m_newNodesStatus.end ();

    if (end == element)
    {
        m_newNodesStatus[uniqueTag] = status;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("PrismCreateClusterWithNodesMessage::setNewNodeStatus : This node (") + newNodeIpAddress + ":" + newNodePort + ") status is already set.");
        prismAssert (false, __FILE__, __LINE__);
    }
}

void PrismCreateClusterWithNodesMessage::updateNewNodeStatus (const string &newNodeIpAddress, const SI32 &newNodePort, ResourceId status)
{
    string                            uniqueTag = newNodeIpAddress + "#" + newNodePort;
    map<string, ResourceId>::iterator element   = m_newNodesStatus.find (uniqueTag);
    map<string, ResourceId>::iterator end       = m_newNodesStatus.end ();

    if (end != element)
    {
        m_newNodesStatus[uniqueTag] = status;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("PrismCreateClusterWithNodesMessage::updateNewNodeStatus : This node (") + newNodeIpAddress + ":" + newNodePort + ") status is not previously set.");
        prismAssert (false, __FILE__, __LINE__);
    }
}

ResourceId PrismCreateClusterWithNodesMessage::getNewNodeStatus (const string &newNodeIpAddress, const SI32 &newNodePort)
{
    string                            uniqueTag = newNodeIpAddress + "#" + newNodePort;
    map<string, ResourceId>::iterator element   = m_newNodesStatus.find (uniqueTag);
    map<string, ResourceId>::iterator end       = m_newNodesStatus.end ();

    if (end != element)
    {
        return (m_newNodesStatus[uniqueTag]);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("PrismCreateClusterWithNodesMessage::getNewNodeStatus : This node (") + newNodeIpAddress + ":" + newNodePort + ") status is not set.");
        prismAssert (false, __FILE__, __LINE__);
        return (WAVE_MESSAGE_ERROR);
    }
}

bool PrismCreateClusterWithNodesMessage::isNewNodeStatusSet (const string &newNodeIpAddress, const SI32 &newNodePort)
{
    string                            uniqueTag = newNodeIpAddress + "#" + newNodePort;
    map<string, ResourceId>::iterator element   = m_newNodesStatus.find (uniqueTag);
    map<string, ResourceId>::iterator end       = m_newNodesStatus.end ();

    if (end != element)
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

void PrismCreateClusterWithNodesMessage::setValidationDetails (const UI32 &bufferSize, void *&pBuffer, bool transferBufferOwnershipToMessage)
{
    addBuffer (VALIDATION_DETAILS_BUFFER_TAG, bufferSize, pBuffer, transferBufferOwnershipToMessage);
}

void PrismCreateClusterWithNodesMessage::getValidationDetails (UI32 &bufferSize, void *&pBuffer, bool transferBufferOwnershipToUser)
{
    if (true == transferBufferOwnershipToUser)
    {
        pBuffer = transferBufferToUser (VALIDATION_DETAILS_BUFFER_TAG, bufferSize);
    }
    else
    {
        pBuffer = findBuffer (VALIDATION_DETAILS_BUFFER_TAG, bufferSize);
    }
}

void PrismCreateClusterWithNodesMessage::setValidationResults (const string &newNodeIpAddress, const SI32 &newNodePort, const UI32 &bufferSize, void *&pBuffer, bool transferBufferOwnershipToMessage)
{
    UI32   bufferTag = VALIDATION_RESULTS_BUFFER_TAG + m_currentValidtionResultsBufferTag;
    string uniqueTag = newNodeIpAddress + "#" + newNodePort;

    addBuffer (bufferTag, bufferSize, pBuffer, transferBufferOwnershipToMessage);
    m_newNodeValidationResults[uniqueTag] = bufferTag;

    // Increment it so that the next time we add it will be at the next tag.

    m_currentValidtionResultsBufferTag++;
}

void PrismCreateClusterWithNodesMessage::getValidationResults (const string &newNodeIpAddress, const SI32 &newNodePort, UI32 &bufferSize, void *&pBuffer, bool transferBufferOwnershipToUser)
{
    string                      uniqueTag = newNodeIpAddress + "#" + newNodePort;
    map<string, UI32>::iterator element   = m_newNodeValidationResults.find (uniqueTag);
    map<string, UI32>::iterator end       = m_newNodeValidationResults.end ();

    UI32 bufferTag;

    if (element != end)
    {
        bufferTag = element->second;
    }
    else
    {
        bufferSize = 0;
        pBuffer    = NULL;

        return;
    }

    if (true == transferBufferOwnershipToUser)
    {
        pBuffer = transferBufferToUser (bufferTag, bufferSize);
    }
    else
    {
        pBuffer = findBuffer (bufferTag, bufferSize);
    }
}

void PrismCreateClusterWithNodesMessage::setIsAddNodeForSpecialCaseFlag (const bool &isAddNodeForSpecialCase)
{
    m_isAddNodeForSpecialCase = isAddNodeForSpecialCase;
}

bool PrismCreateClusterWithNodesMessage::getIsAddNodeForSpecialCaseFlag () const
{
    return (m_isAddNodeForSpecialCase);
}

void PrismCreateClusterWithNodesMessage::setFilenamesToSync ( const vector<string> &filenamesToSync )
{   
    m_filenamesToSync = filenamesToSync;
}

void PrismCreateClusterWithNodesMessage::getFilenamesToSync ( vector<string> &filenamesToSync )
{   
    filenamesToSync = m_filenamesToSync;
}

PrismConfigureClusterSecondaryMessage::PrismConfigureClusterSecondaryMessage ()
    : PrismMessage (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_CONFIGURE_CLUSTER_SECONDARY),
      m_clusterPrimaryPort (0),
      m_clusterPrimaryLocationId (0),
      m_port (0),
      m_locationId (0),
      m_isAddNodeForSpecialCase (false)
{
}

PrismConfigureClusterSecondaryMessage::PrismConfigureClusterSecondaryMessage (PrismServiceId serviceCode, UI32 operationCode)
    : PrismMessage (serviceCode, operationCode),
      m_clusterPrimaryPort (0),
      m_clusterPrimaryLocationId (0),
      m_port (0),
      m_locationId (0),
      m_isAddNodeForSpecialCase (false)
{
}

PrismConfigureClusterSecondaryMessage::PrismConfigureClusterSecondaryMessage (PrismServiceId serviceCode, UI32 operationCode, const string &clusterPrimaryPrismVersion, const string &clusterPrimaryIpAddress, const SI32 &clusterPrimaryPort, const LocationId &clusterPrimaryLocationId, const string &ipAddress, const SI32 &port, const LocationId &locationId)
    : PrismMessage (serviceCode, operationCode),
      m_clusterPrimaryPrismVersion (clusterPrimaryPrismVersion),
      m_clusterPrimaryIpAddress (clusterPrimaryIpAddress),
      m_clusterPrimaryPort (clusterPrimaryPort),
      m_clusterPrimaryLocationId (clusterPrimaryLocationId),
      m_ipAddress (ipAddress),
      m_port (port),
      m_locationId (locationId),
      m_isAddNodeForSpecialCase (false)
{
}

PrismConfigureClusterSecondaryMessage::PrismConfigureClusterSecondaryMessage (const string &clusterPrimaryPrismVersion, const string &clusterPrimaryIpAddress, const SI32 &clusterPrimaryPort, const LocationId &clusterPrimaryLocationId, const string &ipAddress, const SI32 &port, const LocationId &locationId)
    : PrismMessage (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_CONFIGURE_CLUSTER_SECONDARY),
      m_clusterPrimaryPrismVersion (clusterPrimaryPrismVersion),
      m_clusterPrimaryIpAddress (clusterPrimaryIpAddress),
      m_clusterPrimaryPort (clusterPrimaryPort),
      m_clusterPrimaryLocationId (clusterPrimaryLocationId),
      m_ipAddress (ipAddress),
      m_port (port),
      m_locationId (locationId),
      m_isAddNodeForSpecialCase (false)
{
}

PrismConfigureClusterSecondaryMessage::~PrismConfigureClusterSecondaryMessage ()
{
}

string PrismConfigureClusterSecondaryMessage::getClusterPrimaryPrismVersion ()
{
    return (m_clusterPrimaryPrismVersion);
}

string PrismConfigureClusterSecondaryMessage::getClusterPrimaryIpAddress ()
{
    return (m_clusterPrimaryIpAddress);
}

SI32 PrismConfigureClusterSecondaryMessage::getClusterPrimaryPort ()
{
    return (m_clusterPrimaryPort);
}

LocationId PrismConfigureClusterSecondaryMessage::getClusterPrimaryLocationId ()
{
    return (m_clusterPrimaryLocationId);
}

string PrismConfigureClusterSecondaryMessage::getIpAddress ()
{
    return (m_ipAddress);
}

SI32 PrismConfigureClusterSecondaryMessage::getPort ()
{
    return (m_port);
}

LocationId PrismConfigureClusterSecondaryMessage::getLocationId ()
{
    return (m_locationId);
}

void PrismConfigureClusterSecondaryMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeString     (&m_clusterPrimaryPrismVersion,          "clusterPrimaryPrismVersion"));
     addSerializableAttribute (new AttributeString     (&m_clusterPrimaryIpAddress,             "clusterPrimaryIpAddress"));
     addSerializableAttribute (new AttributeSI32       (&m_clusterPrimaryPort,                  "clusterPrimaryPort"));
     addSerializableAttribute (new AttributeLocationId (&m_clusterPrimaryLocationId,            "clusterPrimaryLocationId"));
     addSerializableAttribute (new AttributeString     (&m_ipAddress,                           "ipAddress"));
     addSerializableAttribute (new AttributeSI32       (&m_port,                                "port"));
     addSerializableAttribute (new AttributeLocationId (&m_locationId,                          "locationId"));
     addSerializableAttribute (new AttributeBool       (&m_isAddNodeForSpecialCase,             "isAddNodeForSpecialCase"));

     addSerializableAttribute (new AttributeStringVector (&m_managedObjectNamesForSchemaChange, "managedObjectNamesForSchemaChange"));
     addSerializableAttribute (new AttributeStringVector (&m_fieldNamesStringsForSchemaChange,  "fieldNamesStringsForSchemaChange"));
     addSerializableAttribute (new AttributeStringVector (&m_fieldNamesTypesForSchemaChange,    "fieldNamesTypesForSchemaChange"));
     addSerializableAttribute (new AttributeUI32Vector   (&m_classIds,                          "classIds"));
     addSerializableAttribute (new AttributeUI32Vector   (&m_parentTableIds,                    "parentTableIds"));

}

void  PrismConfigureClusterSecondaryMessage::getSchemaChangeVectors (vector<string>   & managedObjectNamesForSchemaChange,vector<string>   & fieldNamesStringsForSchemaChange,vector<string> & fieldNamesTypesForSchemaChange,vector<UI32>   & classIds,vector<UI32> & parentTableIds)
{
     managedObjectNamesForSchemaChange = m_managedObjectNamesForSchemaChange;
     fieldNamesStringsForSchemaChange = m_fieldNamesStringsForSchemaChange;
     fieldNamesTypesForSchemaChange = m_fieldNamesTypesForSchemaChange;
     classIds = m_classIds;
     parentTableIds = m_parentTableIds;
}


void  PrismConfigureClusterSecondaryMessage::setSchemaChangeVectors ()
{
     vector<string>  managedObjectNamesForSchemaChange;
     vector<string>  fieldNamesStringsForSchemaChange;
     vector<string>  fieldNamesTypesForSchemaChange;
     vector<UI32>    classIds;
     vector<UI32>    parentTableIds;

     OrmRepository  *pOrmRepository = OrmRepository::getInstance();
     pOrmRepository->getSchemaChangeVectors (managedObjectNamesForSchemaChange,fieldNamesStringsForSchemaChange,fieldNamesTypesForSchemaChange, classIds, parentTableIds);

     m_managedObjectNamesForSchemaChange = managedObjectNamesForSchemaChange;
     m_fieldNamesStringsForSchemaChange = fieldNamesStringsForSchemaChange;
     m_fieldNamesTypesForSchemaChange = fieldNamesTypesForSchemaChange;
     m_classIds = classIds;
     m_parentTableIds = parentTableIds;
}

void PrismConfigureClusterSecondaryMessage::setValidationDetails (const UI32 &bufferSize, void *&pBuffer, bool transferBufferOwnershipToMessage)
{
    addBuffer (VALIDATION_DETAILS_BUFFER_TAG, bufferSize, pBuffer, transferBufferOwnershipToMessage);
}

void PrismConfigureClusterSecondaryMessage::getValidationDetails (UI32 &bufferSize, void *&pBuffer, bool transferBufferOwnershipToUser)
{
    if (true == transferBufferOwnershipToUser)
    {
        pBuffer = transferBufferToUser (VALIDATION_DETAILS_BUFFER_TAG, bufferSize);
    }
    else
    {
        pBuffer = findBuffer (VALIDATION_DETAILS_BUFFER_TAG, bufferSize);
    }
}

void PrismConfigureClusterSecondaryMessage::setValidationResults (const UI32 &bufferSize, void *&pBuffer, bool transferBufferOwnershipToMessage)
{
    addBuffer (VALIDATION_RESULTS_BUFFER_TAG, bufferSize, pBuffer, transferBufferOwnershipToMessage);
}

void PrismConfigureClusterSecondaryMessage::getValidationResults (UI32 &bufferSize, void *&pBuffer, bool transferBufferOwnershipToUser)
{
    if (true == transferBufferOwnershipToUser)
    {
        pBuffer = transferBufferToUser (VALIDATION_RESULTS_BUFFER_TAG, bufferSize);
    }
    else
    {
        pBuffer = findBuffer (VALIDATION_RESULTS_BUFFER_TAG, bufferSize);
    }
}

void PrismConfigureClusterSecondaryMessage::setIsAddNodeForSpecialCaseFlag (const bool &isAddNodeForSpecialCase)
{
    m_isAddNodeForSpecialCase = isAddNodeForSpecialCase;
}

bool PrismConfigureClusterSecondaryMessage::getIsAddNodeForSpecialCaseFlag () const
{
    return (m_isAddNodeForSpecialCase);
}

PrismConfigureClusterSecondaryHaPeerMessage::PrismConfigureClusterSecondaryHaPeerMessage ()
    : PrismConfigureClusterSecondaryMessage (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_CONFIGURE_CLUSTER_SECONDARY_HA_PEER)
{
}

PrismConfigureClusterSecondaryHaPeerMessage::~PrismConfigureClusterSecondaryHaPeerMessage ()
{
}

void PrismConfigureClusterSecondaryHaPeerMessage::setupAttributesForSerialization ()
{
    PrismConfigureClusterSecondaryMessage::setupAttributesForSerialization ();
}

PrismConfigureClusterSecondaryPhase1Message::PrismConfigureClusterSecondaryPhase1Message ()
    : PrismMessage (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_CONFIGURE_CLUSTER_SECONDARY_PHASE_1)
{
}

PrismConfigureClusterSecondaryPhase1Message::PrismConfigureClusterSecondaryPhase1Message (PrismServiceId serviceCode, UI32 operationCode)
    : PrismMessage (serviceCode, operationCode)
{
}

PrismConfigureClusterSecondaryPhase1Message::~PrismConfigureClusterSecondaryPhase1Message ()
{
}

void PrismConfigureClusterSecondaryPhase1Message::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();
     addSerializableAttribute (new AttributeString (&m_filenameToSync, "filenameToSync"));
}

UI32 PrismConfigureClusterSecondaryPhase1Message::getDatabaseBackupBufferTag ()
{
    return (UINT_MAX);
}

void PrismConfigureClusterSecondaryPhase1Message::setFilenameToSync ( const string &filenameToSync )
{   
    m_filenameToSync = filenameToSync;
}

void PrismConfigureClusterSecondaryPhase1Message::getFilenameToSync ( string &filenameToSync )
{   
    filenameToSync = m_filenameToSync;
}

PrismConfigureClusterSecondaryHaPeerPhase1Message::PrismConfigureClusterSecondaryHaPeerPhase1Message ()
    : PrismConfigureClusterSecondaryPhase1Message (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_CONFIGURE_CLUSTER_SECONDARY_HA_PEER_PHASE_1)
{
}

PrismConfigureClusterSecondaryHaPeerPhase1Message::~PrismConfigureClusterSecondaryHaPeerPhase1Message ()
{
}

void PrismConfigureClusterSecondaryHaPeerPhase1Message::setupAttributesForSerialization ()
{
    PrismConfigureClusterSecondaryPhase1Message::setupAttributesForSerialization ();
}

PrismConfigureClusterSecondaryPhase2Message::PrismConfigureClusterSecondaryPhase2Message ()
    : PrismMessage (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_CONFIGURE_CLUSTER_SECONDARY_PHASE_2)
{
}

PrismConfigureClusterSecondaryPhase2Message::~PrismConfigureClusterSecondaryPhase2Message ()
{
}

void PrismConfigureClusterSecondaryPhase2Message::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();
}

PrismConfigureClusterSecondaryPhase3Message::PrismConfigureClusterSecondaryPhase3Message ()
    : PrismMessage (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_CONFIGURE_CLUSTER_SECONDARY_PHASE_3)
{
}

PrismConfigureClusterSecondaryPhase3Message::PrismConfigureClusterSecondaryPhase3Message (PrismServiceId serviceCode, UI32 operationCode)
    : PrismMessage (serviceCode, operationCode)
{
}

PrismConfigureClusterSecondaryPhase3Message::~PrismConfigureClusterSecondaryPhase3Message ()
{
}

void PrismConfigureClusterSecondaryPhase3Message::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();
}

PrismConfigureClusterSecondaryHaPeerPhase3Message::PrismConfigureClusterSecondaryHaPeerPhase3Message ()
    : PrismConfigureClusterSecondaryPhase3Message (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_CONFIGURE_CLUSTER_SECONDARY_HA_PEER_PHASE_3)
{
}

PrismConfigureClusterSecondaryHaPeerPhase3Message::~PrismConfigureClusterSecondaryHaPeerPhase3Message ()
{
}

void PrismConfigureClusterSecondaryHaPeerPhase3Message::setupAttributesForSerialization ()
{
    PrismConfigureClusterSecondaryPhase3Message::setupAttributesForSerialization ();
}

PrismRejoinClusterSecondaryPhase1Message::PrismRejoinClusterSecondaryPhase1Message ()
    : PrismConfigureClusterSecondaryPhase1Message (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_REJOIN_CLUSTER_SECONDARY_PHASE_1)
{
}

PrismRejoinClusterSecondaryPhase1Message::PrismRejoinClusterSecondaryPhase1Message (PrismServiceId serviceCode, UI32 operationCode)
    : PrismConfigureClusterSecondaryPhase1Message (serviceCode, operationCode)
{
}

PrismRejoinClusterSecondaryPhase1Message::~PrismRejoinClusterSecondaryPhase1Message ()
{
}

void PrismRejoinClusterSecondaryPhase1Message::setupAttributesForSerialization ()
{
    PrismConfigureClusterSecondaryPhase1Message::setupAttributesForSerialization ();
}

PrismRejoinClusterSecondaryHaPeerPhase1Message::PrismRejoinClusterSecondaryHaPeerPhase1Message ()
    : PrismRejoinClusterSecondaryPhase1Message (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_REJOIN_CLUSTER_SECONDARY_HA_PEER_PHASE_1)
{
}

PrismRejoinClusterSecondaryHaPeerPhase1Message::~PrismRejoinClusterSecondaryHaPeerPhase1Message ()
{
}

void PrismRejoinClusterSecondaryHaPeerPhase1Message::setupAttributesForSerialization ()
{
    PrismRejoinClusterSecondaryPhase1Message::setupAttributesForSerialization ();
}

/// Name
/// PrismRejoinClusterSecondaryPhase2Message
/// Description
/// Constructor
/// Input
/// none
/// Output
/// none
/// Return
/// none
PrismRejoinClusterSecondaryPhase2Message::PrismRejoinClusterSecondaryPhase2Message ()
    : PrismMessage (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_REJOIN_CLUSTER_SECONDARY_PHASE_2),
      m_isReplaceRejoin (false),
      m_replacedNodeLocationId (0)
{
}

/// Name
/// ~PrismRejoinClusterSecondaryPhase2Message
/// Description
/// Destructor
/// Input
/// none
/// Output
/// none
/// Return
/// none
PrismRejoinClusterSecondaryPhase2Message::~PrismRejoinClusterSecondaryPhase2Message ()
{
}
/// Name
/// setupAttributesForSerialization
/// Description
/// Calls the base class function to
/// adds serializable attribute  to add
/// to the attributeMap in the PrismMessage
/// Input
/// none
/// Output
/// none
/// Return
/// none

void PrismRejoinClusterSecondaryPhase2Message::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();
     addSerializableAttribute (new AttributeBool (&m_isReplaceRejoin, "isReplaceRejoin"));
}

bool PrismRejoinClusterSecondaryPhase2Message::getIsReplaceRejoin ()
{
    return (m_isReplaceRejoin);
}

void PrismRejoinClusterSecondaryPhase2Message::setIsReplaceRejoin (const bool &isReplaceRejoin)
{
    m_isReplaceRejoin = isReplaceRejoin;
}

/// Name
/// PrismRejoinClusterSecondaryPhase3Message
/// Description
/// Constructor
/// Input
/// none
/// Output
/// none
/// Return
/// none
PrismRejoinClusterSecondaryPhase3Message::PrismRejoinClusterSecondaryPhase3Message ()
    : PrismMessage (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_REJOIN_CLUSTER_SECONDARY_PHASE_3)
{
}

PrismRejoinClusterSecondaryPhase3Message::PrismRejoinClusterSecondaryPhase3Message (PrismServiceId serviceCode, UI32 operationCode)
    : PrismMessage (serviceCode, operationCode)
{
}

/// Name
/// ~PrismRejoinClusterSecondaryPhase3Message
/// Description
/// Destructor
/// Input
/// none
/// Output
/// none
/// Return
/// none
PrismRejoinClusterSecondaryPhase3Message::~PrismRejoinClusterSecondaryPhase3Message ()
{
}
/// Name
/// setupAttributesForSerialization
/// Description
/// Calls the base class function to
/// adds serializable attribute  to add
/// to the attributeMap in the PrismMessage
/// Input
/// none
/// Output
/// none
/// Return
/// none

void PrismRejoinClusterSecondaryPhase3Message::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();
}

PrismRejoinClusterSecondaryHaPeerPhase3Message::PrismRejoinClusterSecondaryHaPeerPhase3Message ()
    : PrismRejoinClusterSecondaryPhase3Message (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_REJOIN_CLUSTER_SECONDARY_HA_PEER_PHASE_3)
{
}

PrismRejoinClusterSecondaryHaPeerPhase3Message::~PrismRejoinClusterSecondaryHaPeerPhase3Message ()
{
}

void PrismRejoinClusterSecondaryHaPeerPhase3Message::setupAttributesForSerialization ()
{
    PrismRejoinClusterSecondaryPhase3Message::setupAttributesForSerialization ();
}

FrameworkObjectManagerUpdateListOfSecondariesMessage::FrameworkObjectManagerUpdateListOfSecondariesMessage ()
    : PrismMessage (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_UPDATE_LIST_OF_CLUSTER_SECONDARIES),
      m_isDeletion(false),
      m_lastUsedLocationId (0)
{
}

FrameworkObjectManagerUpdateListOfSecondariesMessage::~FrameworkObjectManagerUpdateListOfSecondariesMessage ()
{
}

void FrameworkObjectManagerUpdateListOfSecondariesMessage::addSecondaryNodeDetails (const LocationId &locationId, const string &ipAddress, const UI32 &port, bool isNewNode)
{
    m_locationIdsForSecondaryNodes.push_back (locationId);
    m_IpAddressesForSecondaryNodes.push_back (ipAddress);
    m_portsForSecondaryNodes.push_back       (port);
    m_isNodeNewVector.push_back (isNewNode);
}

UI32 FrameworkObjectManagerUpdateListOfSecondariesMessage::getNumberOfSecondaryLocations () const
{
    UI32 numberOfSecondaryLocations1 = m_locationIdsForSecondaryNodes.size ();
    UI32 numberOfSecondaryLocations2 = m_IpAddressesForSecondaryNodes.size ();
    UI32 numberOfSecondaryLocations3 = m_portsForSecondaryNodes.size ();
    UI32 numberOfSecondaryLocations4 = m_isNodeNewVector.size();

    prismAssert (numberOfSecondaryLocations1 == numberOfSecondaryLocations2, __FILE__, __LINE__);
    prismAssert (numberOfSecondaryLocations2 == numberOfSecondaryLocations3, __FILE__, __LINE__);
    prismAssert (numberOfSecondaryLocations3 == numberOfSecondaryLocations4, __FILE__, __LINE__);
    return (numberOfSecondaryLocations1);
}

LocationId FrameworkObjectManagerUpdateListOfSecondariesMessage::getLocationIdAtIndex (const UI32 &index) const
{
    UI32 numberOfSecondaryLocations = getNumberOfSecondaryLocations ();

    prismAssert (index < numberOfSecondaryLocations, __FILE__, __LINE__);

    return (m_locationIdsForSecondaryNodes[index]);
}

string FrameworkObjectManagerUpdateListOfSecondariesMessage::getIpAddressAtIndex (const UI32 &index) const
{
    UI32 numberOfSecondaryLocations = getNumberOfSecondaryLocations ();

    prismAssert (index < numberOfSecondaryLocations, __FILE__, __LINE__);

    return (m_IpAddressesForSecondaryNodes[index]);
}

UI32 FrameworkObjectManagerUpdateListOfSecondariesMessage::getPortAtIndex (const UI32 &index) const
{
    UI32 numberOfSecondaryLocations = getNumberOfSecondaryLocations ();

    prismAssert (index < numberOfSecondaryLocations, __FILE__, __LINE__);

    return (m_portsForSecondaryNodes[index]);
}

bool FrameworkObjectManagerUpdateListOfSecondariesMessage::isNodeNewAtIndex(const UI32 &index) const
{
    UI32 numberOfSecondaryLocations = getNumberOfSecondaryLocations ();
    prismAssert(index < numberOfSecondaryLocations, __FILE__, __LINE__);
    return (m_isNodeNewVector[index]);
}

bool FrameworkObjectManagerUpdateListOfSecondariesMessage::isDeletion() const
{
    return m_isDeletion;
}

void  FrameworkObjectManagerUpdateListOfSecondariesMessage::setIsDeletion (bool deletion)
{
    m_isDeletion = deletion;
}

LocationId FrameworkObjectManagerUpdateListOfSecondariesMessage::getLastUsedLocationId () const
{
    return (m_lastUsedLocationId);
}

void FrameworkObjectManagerUpdateListOfSecondariesMessage::setLastUsedLocationId (const LocationId &locationId)
{
    m_lastUsedLocationId = locationId;
}

void FrameworkObjectManagerUpdateListOfSecondariesMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeLocationIdVector (&m_locationIdsForSecondaryNodes,  "locationIdsForSecondaryNodes"));
     addSerializableAttribute (new AttributeStringVector     (&m_IpAddressesForSecondaryNodes,  "IpAddressesForSecondaryNodes"));
     addSerializableAttribute (new AttributeUI32Vector       (&m_portsForSecondaryNodes,        "portsForSecondaryNodes"));
     addSerializableAttribute (new AttributeBoolVector       (&m_isNodeNewVector,               "isNodeNewVector"));
     addSerializableAttribute (new AttributeBool             (&m_isDeletion,                    "isDeletion"));
     addSerializableAttribute (new AttributeLocationId       (&m_lastUsedLocationId,            "lastUsedLocationId"));
     addSerializableAttribute (new AttributeLocationIdVector (&m_connectedLocationIds,          "connectedLocationIds"));
}

void FrameworkObjectManagerUpdateListOfSecondariesMessage::getConnectedLocationIds (vector<LocationId> &locationIds)
{
    locationIds = m_connectedLocationIds;
}

void FrameworkObjectManagerUpdateListOfSecondariesMessage::setConnectedLocationIds (vector<LocationId> &locationIds)
{
    m_connectedLocationIds = locationIds;
}

FrameworkObjectManagerAddNodesToClusterMessage::FrameworkObjectManagerAddNodesToClusterMessage ()
    : PrismCreateClusterWithNodesMessage (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_ADD_NODES_TO_CLUSTER)
{
}

FrameworkObjectManagerAddNodesToClusterMessage::~FrameworkObjectManagerAddNodesToClusterMessage ()
{
}

FrameworkObjectManagerDeleteNodesFromClusterMessage::FrameworkObjectManagerDeleteNodesFromClusterMessage (PrismServiceId serviceCode, UI32 operationCode)
    : PrismCreateClusterWithNodesMessage (serviceCode, operationCode),
      m_isDisconnected(false)
{
}

FrameworkObjectManagerDeleteNodesFromClusterMessage::FrameworkObjectManagerDeleteNodesFromClusterMessage ()
    : PrismCreateClusterWithNodesMessage (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_DELETE_NODES_FROM_CLUSTER),
      m_isDisconnected(false)
{
}

FrameworkObjectManagerDeleteNodesFromClusterMessage::~FrameworkObjectManagerDeleteNodesFromClusterMessage ()
{
}

void FrameworkObjectManagerDeleteNodesFromClusterMessage::addNodeIpAddressAndPort (const string &nodeIpAddress, const SI32 &nodePort)
{
    addNewNodeIpAddressAndPort (nodeIpAddress, nodePort);
}

UI32 FrameworkObjectManagerDeleteNodesFromClusterMessage::getNumberOfNodes ()
{
    return (getNumberOfNewNodes ());
}

string FrameworkObjectManagerDeleteNodesFromClusterMessage::getNodeAt (UI32 i)
{
    return (PrismCreateClusterWithNodesMessage::getNodeAt (i));
}

SI32 FrameworkObjectManagerDeleteNodesFromClusterMessage::getNodePortAt (UI32 i)
{
    return (PrismCreateClusterWithNodesMessage::getNodePortAt (i));
}

void FrameworkObjectManagerDeleteNodesFromClusterMessage::setNodeStatus (const string &nodeIpAddress, const SI32 &nodePort, ResourceId status)
{
    setNewNodeStatus (nodeIpAddress, nodePort, status);
}

ResourceId FrameworkObjectManagerDeleteNodesFromClusterMessage::getNodeStatus (const string &nodeIpAddress, const SI32 &nodePort)
{
    return (getNewNodeStatus (nodeIpAddress, nodePort));
}

bool FrameworkObjectManagerDeleteNodesFromClusterMessage::isNodeStatusSet (const string &nodeIpAddress, const SI32 &nodePort)
{
    return (isNewNodeStatusSet (nodeIpAddress, nodePort));
}

void FrameworkObjectManagerDeleteNodesFromClusterMessage::setIsDisconnected(bool isDisconnected)
{
    m_isDisconnected = isDisconnected;
}

bool FrameworkObjectManagerDeleteNodesFromClusterMessage::getIsDisconnected ()
{
    return m_isDisconnected;
}

FrameworkObjectManagerDestroyClusterMessage::FrameworkObjectManagerDestroyClusterMessage ()
    : PrismMessage (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_DESTORY_CLUSTER),
      m_reasonForDestroyingCluster (FRAMEWORK_OBJECT_MANAGER_FAILOVER_REASON_CONTROLLED),
      m_originalRequester(0),
      m_isRebootRequired (true)
{
}

FrameworkObjectManagerDestroyClusterMessage::~FrameworkObjectManagerDestroyClusterMessage ()
{
}

FrameworkObjectManagerFailoverReason FrameworkObjectManagerDestroyClusterMessage::getReasonForDestroyingCluster () const
{
    return (m_reasonForDestroyingCluster);
}

PrismServiceId FrameworkObjectManagerDestroyClusterMessage::getOriginalRequester () const
{
    return (m_originalRequester);
}

void FrameworkObjectManagerDestroyClusterMessage::setOriginalRequester (const PrismServiceId &originalRequester)
{
    m_originalRequester = originalRequester;
}

void FrameworkObjectManagerDestroyClusterMessage::setReasonForDestroyingCluster (const FrameworkObjectManagerFailoverReason &reasonForDestroyingCluster)
{
    m_reasonForDestroyingCluster = reasonForDestroyingCluster;
}

void FrameworkObjectManagerDestroyClusterMessage::setIsRebootRequired (const bool &isRebootRequired)
{
    m_isRebootRequired = isRebootRequired;
}

bool FrameworkObjectManagerDestroyClusterMessage::getIsRebootRequired () const
{
    return (m_isRebootRequired);
}

FrameworkObjectManagerUnconfigureClusterSecondaryMessage::FrameworkObjectManagerUnconfigureClusterSecondaryMessage (const string &clusterSecondaryIpAddress, const SI32 &clusterSecondaryPort)
    : PrismMessage                (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_UNCONFIGURE_CLUSTER_SECONDARY),
      m_clusterSecondaryIpAddress (clusterSecondaryIpAddress),
      m_clusterSecondaryPort      (clusterSecondaryPort),
      m_isPerformWarmRecovery     (false)
{
}

FrameworkObjectManagerUnconfigureClusterSecondaryMessage::FrameworkObjectManagerUnconfigureClusterSecondaryMessage ()
    : PrismMessage                  (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_UNCONFIGURE_CLUSTER_SECONDARY),
      m_clusterSecondaryIpAddress (""),
      m_clusterSecondaryPort (0),
      m_isPerformWarmRecovery (false)
{
}

FrameworkObjectManagerUnconfigureClusterSecondaryMessage::~FrameworkObjectManagerUnconfigureClusterSecondaryMessage ()
{
}

string FrameworkObjectManagerUnconfigureClusterSecondaryMessage::getClusterSecondaryIpAddress ()
{
    return (m_clusterSecondaryIpAddress);
}

SI32 FrameworkObjectManagerUnconfigureClusterSecondaryMessage::getClusterSecondaryPort ()
{
    return (m_clusterSecondaryPort);
}

void FrameworkObjectManagerUnconfigureClusterSecondaryMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeString (&m_clusterSecondaryIpAddress,   "clusterSecondaryIpAddress"));
     addSerializableAttribute (new AttributeSI32 (&m_clusterSecondaryPort,          "clusterSecondaryPort"));
     addSerializableAttribute (new AttributeBool (&m_isPerformWarmRecovery,         "isPerformWarmRecovery"));
}

bool FrameworkObjectManagerUnconfigureClusterSecondaryMessage::getIsPerformWarmRecovery ()
{
    return (m_isPerformWarmRecovery);
}

void FrameworkObjectManagerUnconfigureClusterSecondaryMessage::setIsPerformWarmRecovery (const bool &isPerformWarmRecovery)
{
    m_isPerformWarmRecovery = isPerformWarmRecovery;
}

FrameworkObjectManagerRejoinNodesToClusterMessage::FrameworkObjectManagerRejoinNodesToClusterMessage ()
    : FrameworkObjectManagerDeleteNodesFromClusterMessage (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_REJOIN_NODES_TO_CLUSTER),
    m_isReplaceRejoin (false)
{
}

FrameworkObjectManagerRejoinNodesToClusterMessage::~FrameworkObjectManagerRejoinNodesToClusterMessage ()
{
}

bool FrameworkObjectManagerRejoinNodesToClusterMessage::getIsReplaceRejoin ()
{
    return (m_isReplaceRejoin);
}

void FrameworkObjectManagerRejoinNodesToClusterMessage::setIsReplaceRejoin (const bool &isReplaceRejoin)
{
    m_isReplaceRejoin = isReplaceRejoin;
}

void FrameworkObjectManagerRejoinNodesToClusterMessage::setupAttributesForSerialization ()
{
    FrameworkObjectManagerDeleteNodesFromClusterMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeBool (&m_isReplaceRejoin,           "isReplaceRejoin"));
     addSerializableAttribute (new AttributeLocationIdVector (&m_locationIds,   "locationIds"));
}

void FrameworkObjectManagerRejoinNodesToClusterMessage::setNodeLocationId (const LocationId & locationId)
{
    m_locationIds.push_back(locationId);
    return;
}

LocationId FrameworkObjectManagerRejoinNodesToClusterMessage::getNodeLocationId (const UI32 index) const
{
    return (m_locationIds[index]);
}

FrameworkObjectManagerRejoinClusterSecondaryMessage::FrameworkObjectManagerRejoinClusterSecondaryMessage ()
    : PrismConfigureClusterSecondaryMessage (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_REJOIN_CLUSTER_SECONDARY),
      m_isReplaceRejoin (false)
{
}

FrameworkObjectManagerRejoinClusterSecondaryMessage::FrameworkObjectManagerRejoinClusterSecondaryMessage (PrismServiceId serviceCode, UI32 operationCode)
    : PrismConfigureClusterSecondaryMessage (serviceCode, operationCode),
      m_isReplaceRejoin (false)
{
}

FrameworkObjectManagerRejoinClusterSecondaryMessage::FrameworkObjectManagerRejoinClusterSecondaryMessage (const string &clusterPrimaryPrismVersion, const string &clusterPrimaryIpAddress, const SI32 &clusterPrimaryPort, const LocationId &clusterPrimaryLocationId, const string &ipAddress, const SI32 &port, const LocationId &locationId)
    : PrismConfigureClusterSecondaryMessage (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_REJOIN_CLUSTER_SECONDARY, clusterPrimaryPrismVersion, clusterPrimaryIpAddress, clusterPrimaryPort, clusterPrimaryLocationId, ipAddress, port, locationId),
    m_isReplaceRejoin (false)
{
}

FrameworkObjectManagerRejoinClusterSecondaryMessage::~FrameworkObjectManagerRejoinClusterSecondaryMessage ()
{
}

bool FrameworkObjectManagerRejoinClusterSecondaryMessage::getIsReplaceRejoin ()
{
    return (m_isReplaceRejoin);
}

void FrameworkObjectManagerRejoinClusterSecondaryMessage::setIsReplaceRejoin (const bool &isReplaceRejoin)
{
    m_isReplaceRejoin = isReplaceRejoin;
}

void FrameworkObjectManagerRejoinClusterSecondaryMessage::setupAttributesForSerialization ()
{
    PrismConfigureClusterSecondaryMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeBool         (&m_isReplaceRejoin,       "isReplaceRejoin"));
     addSerializableAttribute (new AttributeUI64Vector   (&m_configurationNumber,   "configurationNumber"));
}

void FrameworkObjectManagerRejoinClusterSecondaryMessage::setConfigurationNumber (const vector<UI64> configurationNumber)
{
    m_configurationNumber = configurationNumber;
}

vector<UI64> FrameworkObjectManagerRejoinClusterSecondaryMessage::getConfigurationNumber ()
{
    return (m_configurationNumber);
}

FrameworkObjectManagerRejoinClusterSecondaryHaPeerMessage::FrameworkObjectManagerRejoinClusterSecondaryHaPeerMessage ()
    : FrameworkObjectManagerRejoinClusterSecondaryMessage (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_REJOIN_CLUSTER_SECONDARY_HA_PEER)
{
}

FrameworkObjectManagerRejoinClusterSecondaryHaPeerMessage::~FrameworkObjectManagerRejoinClusterSecondaryHaPeerMessage ()
{
}

void FrameworkObjectManagerRejoinClusterSecondaryHaPeerMessage::setupAttributesForSerialization ()
{
    FrameworkObjectManagerRejoinClusterSecondaryMessage::setupAttributesForSerialization ();
}

FrameworkObjectManagerDetachFromClusterMessage::FrameworkObjectManagerDetachFromClusterMessage ()
    : PrismMessage (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_DETACH_FROM_CLUSTER),
      m_reasonForDetachingFromCluster (FRAMEWORK_OBJECT_MANAGER_FAILOVER_REASON_CONTROLLED)
{
}

FrameworkObjectManagerDetachFromClusterMessage::~FrameworkObjectManagerDetachFromClusterMessage ()
{
}

FrameworkObjectManagerFailoverReason FrameworkObjectManagerDetachFromClusterMessage::getReasonForDetachingFromCluster () const
{
    return (m_reasonForDetachingFromCluster);
}

void FrameworkObjectManagerDetachFromClusterMessage::setReasonForDetachingFromCluster (const FrameworkObjectManagerFailoverReason &reasonForDetachingFromCluster)
{
    m_reasonForDetachingFromCluster = reasonForDetachingFromCluster;
}

FrameworkObjectManagerLostHeartBeatMessage::FrameworkObjectManagerLostHeartBeatMessage ()
    : FrameworkObjectManagerDeleteNodesFromClusterMessage (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_LOST_HEART_BEAT_MESSAGE)
{
}

FrameworkObjectManagerLostHeartBeatMessage::~FrameworkObjectManagerLostHeartBeatMessage ()
{
}

FrameworkObjectManagerPrimaryChangedMessage::FrameworkObjectManagerPrimaryChangedMessage ()
    : PrismMessage           (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_PRIMARY_CHANGED_MESSAGE),
      m_newPrimaryLocationId (0),
      m_newPrimaryIpAddress  (""),
      m_newPrimaryPort       (0),
      m_isPrimaryChangeDueToControlledFailover(false),
      m_anyConfiguraitonChangeTrackingNumber (0)
{
}

FrameworkObjectManagerPrimaryChangedMessage::FrameworkObjectManagerPrimaryChangedMessage (PrismServiceId serviceCode, UI32 operationCode)
    : PrismMessage           (serviceCode, operationCode),
      m_newPrimaryLocationId (0),
      m_newPrimaryIpAddress  (""),
      m_newPrimaryPort       (0),
      m_isPrimaryChangeDueToControlledFailover(false),
      m_anyConfiguraitonChangeTrackingNumber (0)
{
}
    
FrameworkObjectManagerPrimaryChangedMessage::FrameworkObjectManagerPrimaryChangedMessage (const LocationId &newPrimaryLocationId, const string &newPrimaryIpAddress, const UI32 &newPrimaryPort)
    : PrismMessage                            (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_PRIMARY_CHANGED_MESSAGE),
      m_newPrimaryLocationId                  (newPrimaryLocationId),
      m_newPrimaryIpAddress                   (newPrimaryIpAddress),
      m_newPrimaryPort                        (newPrimaryPort),
      m_isPrimaryChangeDueToControlledFailover(false),
      m_anyConfiguraitonChangeTrackingNumber  (0)
{
}

FrameworkObjectManagerPrimaryChangedMessage::~FrameworkObjectManagerPrimaryChangedMessage ()
{
}

void FrameworkObjectManagerPrimaryChangedMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeLocationId (&m_newPrimaryLocationId,                    "newPrimaryLocationId"));
     addSerializableAttribute (new AttributeString     (&m_newPrimaryIpAddress,                     "newPrimaryIpAddress"));
     addSerializableAttribute (new AttributeUI32       (&m_newPrimaryPort,                          "newPrimaryPort"));
     addSerializableAttribute (new AttributeBool       (&m_isPrimaryChangeDueToControlledFailover,  "isPrimaryChangeDueToControlledFailover"));

     addSerializableAttribute (new AttributeStringVector (&m_managedObjectNamesForSchemaChange,     "managedObjectNamesForSchemaChange"));
     addSerializableAttribute (new AttributeStringVector (&m_fieldNamesStringsForSchemaChange,      "fieldNamesStringsForSchemaChange"));
     addSerializableAttribute (new AttributeStringVector (&m_fieldNamesTypesForSchemaChange,        "fieldNamesTypesForSchemaChange"));
     addSerializableAttribute (new AttributeUI32Vector   (&m_classIds,                              "classIds"));
     addSerializableAttribute (new AttributeUI32Vector   (&m_parentTableIds,                        "parentTableIds"));
    addSerializableAttribute (new AttributeUI64         (&m_anyConfiguraitonChangeTrackingNumber,   "anyConfiguraitonChangeTrackingNumber"));
}

LocationId FrameworkObjectManagerPrimaryChangedMessage::getNewPrimaryLocationId () const
{
    return (m_newPrimaryLocationId);
}

string FrameworkObjectManagerPrimaryChangedMessage::getNewPrimaryIpAddress () const
{
    return (m_newPrimaryIpAddress);
}

UI32 FrameworkObjectManagerPrimaryChangedMessage::getNewPrimaryPort () const
{
    return (m_newPrimaryPort);
}

void   FrameworkObjectManagerPrimaryChangedMessage::setPrimaryChangeDueToControlledFailover     ()
{
     m_isPrimaryChangeDueToControlledFailover = true;
}

bool FrameworkObjectManagerPrimaryChangedMessage::isPrimaryChangeDueToControlledFailover () const
{
    return m_isPrimaryChangeDueToControlledFailover;

}

void  FrameworkObjectManagerPrimaryChangedMessage::getSchemaChangeVectors (vector<string>   & managedObjectNamesForSchemaChange,vector<string>   & fieldNamesStringsForSchemaChange,vector<string> & fieldNamesTypesForSchemaChange,vector<UI32>   & classIds,vector<UI32> & parentTableIds)
{
     managedObjectNamesForSchemaChange = m_managedObjectNamesForSchemaChange;
     fieldNamesStringsForSchemaChange = m_fieldNamesStringsForSchemaChange;
     fieldNamesTypesForSchemaChange = m_fieldNamesTypesForSchemaChange;
     classIds = m_classIds;
     parentTableIds = m_parentTableIds;
}

void  FrameworkObjectManagerPrimaryChangedMessage::setSchemaChangeVectors ()
{
     vector<string>  managedObjectNamesForSchemaChange;
     vector<string>  fieldNamesStringsForSchemaChange;
     vector<string>  fieldNamesTypesForSchemaChange;
     vector<UI32>    classIds;
     vector<UI32>    parentTableIds;

     (OrmRepository::getInstance())->getSchemaChangeVectors (managedObjectNamesForSchemaChange,fieldNamesStringsForSchemaChange,fieldNamesTypesForSchemaChange, classIds, parentTableIds);

     m_managedObjectNamesForSchemaChange = managedObjectNamesForSchemaChange;
     m_fieldNamesStringsForSchemaChange = fieldNamesStringsForSchemaChange;
     m_fieldNamesTypesForSchemaChange = fieldNamesTypesForSchemaChange;
     m_classIds = classIds;
     m_parentTableIds = parentTableIds;
}

void  FrameworkObjectManagerPrimaryChangedMessage::setAnyConfigurationChangeTrackingNumber (const UI64 &trackingNumber)
{
    m_anyConfiguraitonChangeTrackingNumber = trackingNumber;
}

UI64  FrameworkObjectManagerPrimaryChangedMessage::getAnyConfigurationChangeTrackingNumber () const
{
    return (m_anyConfiguraitonChangeTrackingNumber);
}

FrameworkObjectManagerPrimaryChangedHaPeerMessage::FrameworkObjectManagerPrimaryChangedHaPeerMessage ()
    : FrameworkObjectManagerPrimaryChangedMessage (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_PRIMARY_CHANGED_MESSAGE_HA_PEER),
      m_isDBEmptyRequired (true)
{
}

FrameworkObjectManagerPrimaryChangedHaPeerMessage::~FrameworkObjectManagerPrimaryChangedHaPeerMessage ()
{
}

void FrameworkObjectManagerPrimaryChangedHaPeerMessage::setupAttributesForSerialization ()
{
    FrameworkObjectManagerPrimaryChangedMessage::setupAttributesForSerialization ();

    addSerializableAttribute (new AttributeBool (&m_isDBEmptyRequired,  "isDBEmptyRequired"));
}

void FrameworkObjectManagerPrimaryChangedHaPeerMessage::setDBEmptyRequired (const bool &isDBEmptyRequired)
{
    m_isDBEmptyRequired = isDBEmptyRequired;
}

bool FrameworkObjectManagerPrimaryChangedHaPeerMessage::getIsDBEmptyRequired () const
{
    return (m_isDBEmptyRequired);
}

FrameworkObjectManagerPrimaryChangedPhase1Message::FrameworkObjectManagerPrimaryChangedPhase1Message ()
    : PrismMessage (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_PRIMARY_CHANGED_PHASE_1_MESSAGE),
      m_anyConfiguraitonChangeTrackingNumber (0)
{
}

FrameworkObjectManagerPrimaryChangedPhase1Message::FrameworkObjectManagerPrimaryChangedPhase1Message (PrismServiceId serviceCode, UI32 operationCode)
    : PrismMessage (serviceCode, operationCode),
      m_anyConfiguraitonChangeTrackingNumber (0)
{
}

FrameworkObjectManagerPrimaryChangedPhase1Message::~FrameworkObjectManagerPrimaryChangedPhase1Message ()
{
}

UI32 FrameworkObjectManagerPrimaryChangedPhase1Message::getDatabaseBackupBufferTag ()
{
    return (0);
}

void FrameworkObjectManagerPrimaryChangedPhase1Message::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();

    addSerializableAttribute (new AttributeUI64         (&m_anyConfiguraitonChangeTrackingNumber,   "anyConfiguraitonChangeTrackingNumber"));
}

void FrameworkObjectManagerPrimaryChangedPhase1Message::setAnyConfigurationChangeTrackingNumber (const UI64 &trackingNumber)
{
    m_anyConfiguraitonChangeTrackingNumber = trackingNumber;
}

UI64 FrameworkObjectManagerPrimaryChangedPhase1Message::getAnyConfigurationChangeTrackingNumber () const
{
    return (m_anyConfiguraitonChangeTrackingNumber);
}

FrameworkObjectManagerPrimaryChangedHaPeerPhase1Message::FrameworkObjectManagerPrimaryChangedHaPeerPhase1Message ()
    : FrameworkObjectManagerPrimaryChangedPhase1Message (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_PRIMARY_CHANGED_PHASE_1_HA_PEER_MESSAGE),
      m_isDBRestoreRequired (true)
{
}

FrameworkObjectManagerPrimaryChangedHaPeerPhase1Message::~FrameworkObjectManagerPrimaryChangedHaPeerPhase1Message ()
{
}

void FrameworkObjectManagerPrimaryChangedHaPeerPhase1Message::setupAttributesForSerialization ()
{
    FrameworkObjectManagerPrimaryChangedPhase1Message::setupAttributesForSerialization ();

    addSerializableAttribute (new AttributeBool (&m_isDBRestoreRequired, "isDBRestoreRequired"));
}

void FrameworkObjectManagerPrimaryChangedHaPeerPhase1Message::setDBRestoreRequired (const bool &isDBRestoreRequired)
{
    m_isDBRestoreRequired = isDBRestoreRequired;
}

bool FrameworkObjectManagerPrimaryChangedHaPeerPhase1Message::getIsDBRestoreRequired () const
{
    return (m_isDBRestoreRequired);
}

FrameworkObjectManagerPrimaryChangedPhase2Message::FrameworkObjectManagerPrimaryChangedPhase2Message ()
    : PrismMessage (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_PRIMARY_CHANGED_PHASE_2_MESSAGE)
{
}

FrameworkObjectManagerPrimaryChangedPhase2Message::~FrameworkObjectManagerPrimaryChangedPhase2Message ()
{
}

FrameworkObjectManagerPrimaryChangedPhase3Message::FrameworkObjectManagerPrimaryChangedPhase3Message ()
    : PrismMessage (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_PRIMARY_CHANGED_PHASE_3_MESSAGE)
{
}

FrameworkObjectManagerPrimaryChangedPhase3Message::FrameworkObjectManagerPrimaryChangedPhase3Message (PrismServiceId serviceCode, UI32 operationCode)
    : PrismMessage (serviceCode, operationCode)
{
}

FrameworkObjectManagerPrimaryChangedPhase3Message::~FrameworkObjectManagerPrimaryChangedPhase3Message ()
{
}

void FrameworkObjectManagerPrimaryChangedPhase3Message::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();
}

FrameworkObjectManagerPrimaryChangedHaPeerPhase3Message::FrameworkObjectManagerPrimaryChangedHaPeerPhase3Message ()
    : FrameworkObjectManagerPrimaryChangedPhase3Message (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_PRIMARY_CHANGED_PHASE_3_HA_PEER_MESSAGE)
{
}

FrameworkObjectManagerPrimaryChangedHaPeerPhase3Message::~FrameworkObjectManagerPrimaryChangedHaPeerPhase3Message ()
{
}

void FrameworkObjectManagerPrimaryChangedHaPeerPhase3Message::setupAttributesForSerialization ()
{
    FrameworkObjectManagerPrimaryChangedPhase3Message::setupAttributesForSerialization ();
}

FrameworkObjectManagerRollbackStandbyOnActiveRollbackMessage::FrameworkObjectManagerRollbackStandbyOnActiveRollbackMessage ()
    : PrismMessage (PrismFrameworkObjectManager::getPrismServiceId(), FRAMEWORK_OBJECT_MANAGER_ROLLBACK_STANDBY_ON_ACTIVE_ROLLBACK)
{
}

FrameworkObjectManagerRollbackStandbyOnActiveRollbackMessage::~FrameworkObjectManagerRollbackStandbyOnActiveRollbackMessage ()
{
}

void FrameworkObjectManagerRollbackStandbyOnActiveRollbackMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();
}

FrameworkObjectManagerServiceControlListMessage::FrameworkObjectManagerServiceControlListMessage ()
    : ManagementInterfaceMessage (PrismFrameworkObjectManager::getServiceName (), FRAMEWORK_OBJECT_MANAGER_SERVICE_CONTROL_LIST_MESSAGE)
{
}

FrameworkObjectManagerServiceControlListMessage::~FrameworkObjectManagerServiceControlListMessage ()
{
}

void FrameworkObjectManagerServiceControlListMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributePrismServiceIdVector (&m_prismServiceIds,   "prismServiceIds"));
     addSerializableAttribute (new AttributeStringVector         (&m_serviceNames,      "serviceNames"));
     addSerializableAttribute (new AttributeBoolVector           (&m_enabled,           "enabled"));
     addSerializableAttribute (new AttributeBoolVector           (&m_local,             "local"));
     addSerializableAttribute (new AttributeStringVector         (&m_cpuAffinity,       "cpuAffinity"));
}

UI32 FrameworkObjectManagerServiceControlListMessage::getNumberOfServices () const
{
    return (m_prismServiceIds.size ());
}

void FrameworkObjectManagerServiceControlListMessage::addAService (const PrismServiceId &prismServiceId, const string &serviceName, const bool &isEnabled, const bool &isLocal, const string &cpuAffinity)
{
    m_prismServiceIds.push_back (prismServiceId);
    m_serviceNames.push_back    (serviceName);
    m_enabled.push_back         (isEnabled);
    m_local.push_back           (isLocal);
    m_cpuAffinity.push_back     (cpuAffinity);
}

void FrameworkObjectManagerServiceControlListMessage::getServiceAtIndex (const UI32 &index, PrismServiceId &prismServiceId, string &serviceName, bool &isEnabled, bool &isLocal, string &cpuAffinity)
{
    UI32 numberOfServices = getNumberOfServices ();

    prismAssert (index < numberOfServices, __FILE__, __LINE__);

    prismServiceId = m_prismServiceIds[index];
    serviceName    = m_serviceNames[index];
    isEnabled      = m_enabled[index];
    isLocal        = m_local[index];
    cpuAffinity    = m_cpuAffinity[index];
}

FrameworkObjectManagerServiceSetCpuAffinityMessage::FrameworkObjectManagerServiceSetCpuAffinityMessage ()
    : ManagementInterfaceMessage (PrismFrameworkObjectManager::getServiceName (), FRAMEWORK_OBJECT_MANAGER_SERVICE_SET_CPU_AFFINITY_MESSAGE),
      m_prismServiceId(0)
{
}

FrameworkObjectManagerServiceSetCpuAffinityMessage::FrameworkObjectManagerServiceSetCpuAffinityMessage (const PrismServiceId &prismServiceId, const vector<UI32> &cpuAffinityVector)
    : ManagementInterfaceMessage (PrismFrameworkObjectManager::getServiceName (), FRAMEWORK_OBJECT_MANAGER_SERVICE_SET_CPU_AFFINITY_MESSAGE),
      m_prismServiceId           (prismServiceId),
      m_cpuAffinityVector        (cpuAffinityVector)
{
}

FrameworkObjectManagerServiceSetCpuAffinityMessage::~FrameworkObjectManagerServiceSetCpuAffinityMessage ()
{
}

void FrameworkObjectManagerServiceSetCpuAffinityMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributePrismServiceId (&m_prismServiceId,      "prismServiceId"));
     addSerializableAttribute (new AttributeUI32Vector     (&m_cpuAffinityVector,   "cpuAffinityVector"));
}

PrismServiceId FrameworkObjectManagerServiceSetCpuAffinityMessage::getPrismServiceId ()
{
    return (m_prismServiceId);
}

vector<UI32> FrameworkObjectManagerServiceSetCpuAffinityMessage::getCpuAffinityVector () const
{
    return (m_cpuAffinityVector);
}

FrameworkObjectManagerStartExternalStateSynchronizationMessage::FrameworkObjectManagerStartExternalStateSynchronizationMessage (const UI32 fssStageNumber, const ResourceId serviceType)
    : ManagementInterfaceMessage (PrismFrameworkObjectManager::getServiceName (), FRAMEWORK_OBJECT_MANAGER_START_EXTERNAL_STATE_SYNCHRONIZATION),
    m_fssStageNumber    (fssStageNumber),
    m_serviceType       (serviceType)
{

}

void FrameworkObjectManagerStartExternalStateSynchronizationMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeUI32     (&m_fssStageNumber,    "fssStageNumber"));
     addSerializableAttribute (new AttributeResourceId (&m_serviceType,     "serviceType"));
}

UI32 FrameworkObjectManagerStartExternalStateSynchronizationMessage::getFssStageNumber () const
{
    return (m_fssStageNumber);
}

ResourceId FrameworkObjectManagerStartExternalStateSynchronizationMessage::getServiceType () const
{
    return (m_serviceType);
}



FrameworkObjectManagerStartSlotFailoverMessage::FrameworkObjectManagerStartSlotFailoverMessage (const UI32 slotNumber)
    : ManagementInterfaceMessage (PrismFrameworkObjectManager::getServiceName (), FRAMEWORK_OBJECT_MANAGER_START_SLOT_FAILOVER),
    m_slotNumber (slotNumber)
{

}

void FrameworkObjectManagerStartSlotFailoverMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeUI32     (&m_slotNumber, "slotNumber"));
}

UI32 FrameworkObjectManagerStartSlotFailoverMessage::getSlotNumber () const
{
    return (m_slotNumber);
}


FrameworkObjectManagerNewPrincipalEstablishedMessage::FrameworkObjectManagerNewPrincipalEstablishedMessage(const string & failedPrincipalIpAddress,UI32 failedPrincipalPort,LocationId failedPrincipalLocationId)
      :PrismMessage(PrismFrameworkObjectManager::getPrismServiceId(),FRAMEWORK_OBJECT_MANAGER_NEW_PRINCIPAL_ESTABLISHED_AFTER_FAILOVER),
       mFailedPrincipalIpAddress(failedPrincipalIpAddress),
       mFailedPrincipalPort(failedPrincipalPort),
       mFailedPrincipalLocationId(failedPrincipalLocationId),
       mTriggerControlledFailover (false)
{
}


FrameworkObjectManagerNewPrincipalEstablishedMessage::~FrameworkObjectManagerNewPrincipalEstablishedMessage()
{
}

void FrameworkObjectManagerNewPrincipalEstablishedMessage::setFailedPrincipalIpAddress(const string & failedPrincipalIpAddress)
{
    mFailedPrincipalIpAddress = failedPrincipalIpAddress;
}

void FrameworkObjectManagerNewPrincipalEstablishedMessage::setFailedPrinciapalPort(UI32 failedPrincipalPort)
{
    mFailedPrincipalPort = failedPrincipalPort;
}

void FrameworkObjectManagerNewPrincipalEstablishedMessage::setFailedPrincipalLocationId(LocationId locationId)
{
    mFailedPrincipalLocationId = locationId;
}

void FrameworkObjectManagerNewPrincipalEstablishedMessage::setTriggerControlledFaileover ()
{
    mTriggerControlledFailover = true;
}

string const & FrameworkObjectManagerNewPrincipalEstablishedMessage::getFailedPrincipalIpAddress() const
{
    return mFailedPrincipalIpAddress;
}
UI32  FrameworkObjectManagerNewPrincipalEstablishedMessage::getFailedPrincipalPort()  const
{
    return mFailedPrincipalPort;
}
LocationId     FrameworkObjectManagerNewPrincipalEstablishedMessage::getFailedPrincipalLocationId() const
{
    return mFailedPrincipalLocationId;
}

bool FrameworkObjectManagerNewPrincipalEstablishedMessage::getTriggerControlledFailover () const
{
    return mTriggerControlledFailover;
}

FrameworkObjectManagerSecondaryNodeFailureNotificationMessage::FrameworkObjectManagerSecondaryNodeFailureNotificationMessage()
      :PrismMessage(PrismFrameworkObjectManager::getPrismServiceId(),FRAMEWORK_OBJECT_MANAGER_SECONDARY_NODE_FAILURE_NOTIFICATION),
       m_isControlledFailoverInSpecialCase (false)
      
{
}

FrameworkObjectManagerSecondaryNodeFailureNotificationMessage::FrameworkObjectManagerSecondaryNodeFailureNotificationMessage(const string & failedIpAddress,UI32 failedPort,LocationId failedLocationId)
      :PrismMessage(PrismFrameworkObjectManager::getPrismServiceId(),FRAMEWORK_OBJECT_MANAGER_SECONDARY_NODE_FAILURE_NOTIFICATION),
       m_isControlledFailoverInSpecialCase (false)
      
{
    mFailedIpAddresses.push_back(failedIpAddress);
    mFailedPorts.push_back(failedPort);
    mFailedLocationIds.push_back(failedLocationId);
}


FrameworkObjectManagerSecondaryNodeFailureNotificationMessage::~FrameworkObjectManagerSecondaryNodeFailureNotificationMessage()
{
}

void FrameworkObjectManagerSecondaryNodeFailureNotificationMessage::addFailedIpAddress(const string & failedIpAddress)
{
    mFailedIpAddresses.push_back(failedIpAddress);
}

void FrameworkObjectManagerSecondaryNodeFailureNotificationMessage::addFailedPort(UI32 failedPort)
{
    mFailedPorts.push_back(failedPort);
}

void FrameworkObjectManagerSecondaryNodeFailureNotificationMessage::addFailedLocationId(LocationId locationId)
{
    mFailedLocationIds.push_back(locationId);
}

vector<string> const & FrameworkObjectManagerSecondaryNodeFailureNotificationMessage::getFailedIpAddresses() const
{
    return mFailedIpAddresses;
}
vector<UI32> const &  FrameworkObjectManagerSecondaryNodeFailureNotificationMessage::getFailedPorts()  const
{
    return mFailedPorts;
}
vector<LocationId> const &     FrameworkObjectManagerSecondaryNodeFailureNotificationMessage::getFailedLocationIds() const
{
    return mFailedLocationIds;
}

void FrameworkObjectManagerSecondaryNodeFailureNotificationMessage::setIsControlledFailoverInSpecialCase (bool const &isControlledFailoverInSpecialCase)
{
    m_isControlledFailoverInSpecialCase = isControlledFailoverInSpecialCase;
}

bool FrameworkObjectManagerSecondaryNodeFailureNotificationMessage::getIsControlledFailoverInSpecialCase () const
{
    return (m_isControlledFailoverInSpecialCase);
}

FrameworkObjectManagerDisconnectFromAllNodesMessage::FrameworkObjectManagerDisconnectFromAllNodesMessage()
      :PrismMessage(PrismFrameworkObjectManager::getPrismServiceId(),FRAMEWORK_OBJECT_MANAGER_DISCONNECT_FROM_ALL_NODES)
{
}


FrameworkObjectManagerDisconnectFromAllNodesMessage::~FrameworkObjectManagerDisconnectFromAllNodesMessage()
{
}

FrameworkObjectManagerEstablishPrincipalAfterClusterRebootMessage::FrameworkObjectManagerEstablishPrincipalAfterClusterRebootMessage ()
      :PrismMessage(PrismFrameworkObjectManager::getPrismServiceId(),FRAMEWORK_OBJECT_MANAGER_ESTABLISH_PRINCIPAL_AFTER_CLUSTER_REBOOT)

{

}

FrameworkObjectManagerEstablishPrincipalAfterClusterRebootMessage::~FrameworkObjectManagerEstablishPrincipalAfterClusterRebootMessage ()
{

}

FrameworkObjectManagerResetNodeToUnconfirmRole::FrameworkObjectManagerResetNodeToUnconfirmRole ()
    :PrismMessage(PrismFrameworkObjectManager::getPrismServiceId(), FRAMEWORK_OBJECT_MANAGER_RESET_NODE_FOR_CLUSTER_MERGE)
{

}

FrameworkObjectManagerResetNodeToUnconfirmRole::~FrameworkObjectManagerResetNodeToUnconfirmRole ()
{

}

FrameworkObjectManagerRemoveKnownLocationsMessage::FrameworkObjectManagerRemoveKnownLocationsMessage ()
      :PrismMessage(PrismFrameworkObjectManager::getPrismServiceId(), FRAMEWORK_OBJECT_MANAGER_REMOVE_KNOWN_LOCATIONS_MESSAGE)
{
}


FrameworkObjectManagerRemoveKnownLocationsMessage::~FrameworkObjectManagerRemoveKnownLocationsMessage ()
{
}

void FrameworkObjectManagerRemoveKnownLocationsMessage::setFailedLocations (const vector<LocationId> locations)
{
    m_failedLocationIds = locations;
}

vector<LocationId> FrameworkObjectManagerRemoveKnownLocationsMessage::getFailedLocationIds () const
{
    return (m_failedLocationIds);
}

void FrameworkObjectManagerRemoveKnownLocationsMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeUI32Vector (&m_failedLocationIds, "failedLocationIds"));
}

FrameworkObjectManagerStartHaSyncMessage::FrameworkObjectManagerStartHaSyncMessage (const string &peerHaIpAddress, const SI32 &peerHaIpPort)
      :PrismMessage (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_START_HA_SYNC_MESSAGE),
       m_peerHaIpAddress(peerHaIpAddress),
       m_peerHaIpPort(peerHaIpPort),
       m_firmwareVersion (""),
       m_haVersion (0),
       m_returnData ("")
{
}

FrameworkObjectManagerStartHaSyncMessage::~FrameworkObjectManagerStartHaSyncMessage()
{
}

void FrameworkObjectManagerStartHaSyncMessage::setPeerHaIpAddress(const string &peerHaIpAddress)
{
    m_peerHaIpAddress = peerHaIpAddress;
}

void FrameworkObjectManagerStartHaSyncMessage::setPeerHaIpPort(const SI32 &peerHaIpPort)
{
    m_peerHaIpPort = peerHaIpPort;
}

string FrameworkObjectManagerStartHaSyncMessage::getPeerHaIpAddress() const
{
    return m_peerHaIpAddress;
}

SI32 FrameworkObjectManagerStartHaSyncMessage::getPeerHaIpPort() const
{
    return m_peerHaIpPort;
}

void FrameworkObjectManagerStartHaSyncMessage::setFirmwareVersion(const string &firmwareVersion)
{
    m_firmwareVersion = firmwareVersion;
}

string FrameworkObjectManagerStartHaSyncMessage::getFirmwareVersion() const
{
    return m_firmwareVersion;
}

void FrameworkObjectManagerStartHaSyncMessage::setHAVersion(const UI32 &haVersion)
{
    m_haVersion = haVersion;
}

UI32 FrameworkObjectManagerStartHaSyncMessage::getHAVersion() const
{
    return m_haVersion;
}

void FrameworkObjectManagerStartHaSyncMessage::setReturnData (const string &returnData)
{
    m_returnData = returnData;
}

string FrameworkObjectManagerStartHaSyncMessage::getReturnData () const
{
    return m_returnData ;
}

void FrameworkObjectManagerStartHaSyncMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeString (&m_peerHaIpAddress, "peerHaIpAddress"));
     addSerializableAttribute (new AttributeSI32 (&m_peerHaIpPort,      "peerHaIpPort"));
     addSerializableAttribute (new AttributeString (&m_firmwareVersion, "firmwareVersion"));
     addSerializableAttribute (new AttributeUI32 (&m_haVersion,         "haVersion"));
     addSerializableAttribute (new AttributeString (&m_returnData,      "returnData"));
}

FrameworkObjectManagerEndHaSyncMessage::FrameworkObjectManagerEndHaSyncMessage ()
      :PrismMessage (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_END_HA_SYNC_MESSAGE)
{
}

FrameworkObjectManagerEndHaSyncMessage::~FrameworkObjectManagerEndHaSyncMessage()
{
}

FrameworkObjectManagerInitHaIpAddressMessage::FrameworkObjectManagerInitHaIpAddressMessage (const string &localHaIpAddress, const SI32 &localHaIpPort)
      :PrismMessage (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_INIT_HA_IPADDR_MESSAGE),
       m_localHaIpAddress(localHaIpAddress),
       m_localHaIpPort(localHaIpPort)
{
}

FrameworkObjectManagerInitHaIpAddressMessage::~FrameworkObjectManagerInitHaIpAddressMessage()
{
}

void FrameworkObjectManagerInitHaIpAddressMessage::setLocalHaIpAddress(const string &localHaIpAddress)
{
    m_localHaIpAddress = localHaIpAddress;
}

void FrameworkObjectManagerInitHaIpAddressMessage::setLocalHaIpPort(const SI32 &localHaIpPort)
{
    m_localHaIpPort = localHaIpPort;
}

string FrameworkObjectManagerInitHaIpAddressMessage::getLocalHaIpAddress() const
{
    return m_localHaIpAddress;
}

SI32 FrameworkObjectManagerInitHaIpAddressMessage::getLocalHaIpPort() const
{
    return m_localHaIpPort;
}

void FrameworkObjectManagerInitHaIpAddressMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeString (&m_localHaIpAddress,    "localHaIpAddress"));
     addSerializableAttribute (new AttributeSI32 (&m_localHaIpPort,         "localHaIpPort"));
}

PrismHaSyncConfigureStandbyMessage::PrismHaSyncConfigureStandbyMessage ()
    : PrismMessage (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_HA_SYNC_CONFIGURE_STANDBY),
    m_activePrismVersion (""),
    m_activeLocationRole (LOCATION_STAND_ALONE)
{
}

PrismHaSyncConfigureStandbyMessage::PrismHaSyncConfigureStandbyMessage (const string &activePrismVersion, const LocationRole &activeLocationRole)
    : PrismMessage (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_HA_SYNC_CONFIGURE_STANDBY),
      m_activePrismVersion (activePrismVersion),
      m_activeLocationRole (activeLocationRole)
{
}

PrismHaSyncConfigureStandbyMessage::~PrismHaSyncConfigureStandbyMessage ()
{
}

void PrismHaSyncConfigureStandbyMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();
    addSerializableAttribute (new AttributeString (&m_activePrismVersion,   "activePrismVersion"));
    addSerializableAttribute (new AttributeUI32   (&m_activeLocationRole,   "activeLocationRole"));
}

void PrismHaSyncConfigureStandbyMessage::setupAttributesForSerializationInAttributeOrderFormat ()
{
    PrismMessage::setupAttributesForSerializationInAttributeOrderFormat ();

    addAttributeNameForOrderToNameMapping ("activePrismVersion");
    addAttributeNameForOrderToNameMapping ("activeLocationRole");
}

string PrismHaSyncConfigureStandbyMessage::getActivePrismVersion ()
{
    return (m_activePrismVersion);
}

LocationRole PrismHaSyncConfigureStandbyMessage::getActiveLocationRole ()
{
    return ((LocationRole)(m_activeLocationRole));
}

UI32 PrismHaSyncConfigureStandbyMessage::getDatabaseBackupBufferTag ()
{
    return (UINT_MAX);
}

void PrismHaSyncConfigureStandbyMessage::setValidationDetails (const UI32 &bufferSize, void *&pBuffer, bool transferBufferOwnershipToMessage)
{
    addBuffer (VALIDATION_DETAILS_BUFFER_TAG, bufferSize, pBuffer, transferBufferOwnershipToMessage);
}

void PrismHaSyncConfigureStandbyMessage::getValidationDetails (UI32 &bufferSize, void *&pBuffer, bool transferBufferOwnershipToUser)
{
    if (true == transferBufferOwnershipToUser)
    {
        pBuffer = transferBufferToUser (VALIDATION_DETAILS_BUFFER_TAG, bufferSize);
    }
    else
    {
        pBuffer = findBuffer (VALIDATION_DETAILS_BUFFER_TAG, bufferSize);
    }
}

void PrismHaSyncConfigureStandbyMessage::setValidationResults (const UI32 &bufferSize, void *&pBuffer, bool transferBufferOwnershipToMessage)
{
    addBuffer (VALIDATION_RESULTS_BUFFER_TAG, bufferSize, pBuffer, transferBufferOwnershipToMessage);
}

void PrismHaSyncConfigureStandbyMessage::getValidationResults (UI32 &bufferSize, void *&pBuffer, bool transferBufferOwnershipToUser)
{
    if (true == transferBufferOwnershipToUser)
    {
        pBuffer = transferBufferToUser (VALIDATION_RESULTS_BUFFER_TAG, bufferSize);
    }
    else
    {
        pBuffer = findBuffer (VALIDATION_RESULTS_BUFFER_TAG, bufferSize);
    }
}

FrameworkObjectManagerDisconnectFromAllInstanceClientsMessage::FrameworkObjectManagerDisconnectFromAllInstanceClientsMessage()
      : ManagementInterfaceMessage (PrismFrameworkObjectManager::getServiceName(), FRAMEWORK_OBJECT_MANAGER_DISCONNECT_FROM_ALL_INSTANCE_CLIENTS)
{
}


FrameworkObjectManagerDisconnectFromAllInstanceClientsMessage::~FrameworkObjectManagerDisconnectFromAllInstanceClientsMessage()
{
}

void FrameworkObjectManagerDisconnectFromAllInstanceClientsMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

    addSerializableAttribute (new AttributeString (&m_clientIpAddress, "clientIpAddress1"));
}

void FrameworkObjectManagerDisconnectFromAllInstanceClientsMessage::setClientIpAddress(const string & clientIpAddress)
{
    m_clientIpAddress = clientIpAddress;
}

string const & FrameworkObjectManagerDisconnectFromAllInstanceClientsMessage::getClientIpAddress() const
{
    return m_clientIpAddress;
}

FrameworkObjectManagerPrepareNodeForHAMessage::FrameworkObjectManagerPrepareNodeForHAMessage ()
      : PrismMessage (PrismFrameworkObjectManager::getPrismServiceId(), FRAMEWORK_OBJECT_MANAGER_PREPARE_NODE_FOR_HA)
{

}

FrameworkObjectManagerPrepareNodeForHAMessage::~FrameworkObjectManagerPrepareNodeForHAMessage ()
{

}

FrameworkObjectManagerUpdateDatabaseSchema::FrameworkObjectManagerUpdateDatabaseSchema()
    : PrismMessage(PrismFrameworkObjectManager::getPrismServiceId(), FRAMEWORK_OBJECT_MANAGER_UPDATE_DATABASE_SCHEMA)
{
}

FrameworkObjectManagerUpdateDatabaseSchema::~FrameworkObjectManagerUpdateDatabaseSchema()
{
}

FrameworkObjectManagerListenForManagedObjectMessage::FrameworkObjectManagerListenForManagedObjectMessage (const string &managedObjectClassName, const WaveManagedObjectListenOperation &managedObjectOperation)
    : ManagementInterfaceMessage (PrismFrameworkObjectManager::getServiceName(), FRAMEWORK_OBJECT_MANAGER_LISTEN_FOR_MANAGED_OBJECT),
      m_managedObjectClassName (managedObjectClassName),
      m_managedObjectOperation (managedObjectOperation)
{
}

FrameworkObjectManagerListenForManagedObjectMessage::~FrameworkObjectManagerListenForManagedObjectMessage ()
{
}

void FrameworkObjectManagerListenForManagedObjectMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeString     (               &m_managedObjectClassName,  "managedObjectClassName"));
    addSerializableAttribute (new AttributeResourceId ((ResourceId *) &m_managedObjectOperation,   "managedObjectOperation"));
}

FrameworkObjectManagerHaSyncUpdateMessage::FrameworkObjectManagerHaSyncUpdateMessage()
    :PrismMessage(PrismFrameworkObjectManager::getPrismServiceId(), FRAMEWORK_OBJECT_MANAGER_HA_SYNC_UPDATE), 
     m_isDbSyncRequired (false)
{
}

FrameworkObjectManagerHaSyncUpdateMessage::~FrameworkObjectManagerHaSyncUpdateMessage()
{
}

void FrameworkObjectManagerHaSyncUpdateMessage::setIsDbSyncRequired (const bool &isDbSyncRequired)
{
    m_isDbSyncRequired = isDbSyncRequired;
}

bool FrameworkObjectManagerHaSyncUpdateMessage::getIsDbSyncRequired ()
{
    return (m_isDbSyncRequired);
}

FrameworkObjectManagerGetFirmwareVersionMessage::FrameworkObjectManagerGetFirmwareVersionMessage()
    :PrismMessage(PrismFrameworkObjectManager::getPrismServiceId(), FRAMEWORK_OBJECT_MANAGER_GET_FIRMWARE_VERSION),
     m_firmwareVersion ("")
{
}

FrameworkObjectManagerGetFirmwareVersionMessage::~FrameworkObjectManagerGetFirmwareVersionMessage()
{
}

void FrameworkObjectManagerGetFirmwareVersionMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();

    addSerializableAttribute (new AttributeString (&m_firmwareVersion, "firmwareVersion"));
}

void FrameworkObjectManagerGetFirmwareVersionMessage::setupAttributesForSerializationInAttributeOrderFormat ()
{
    PrismMessage::setupAttributesForSerializationInAttributeOrderFormat ();

    addAttributeNameForOrderToNameMapping ("firmwareVersion");
}

void FrameworkObjectManagerGetFirmwareVersionMessage::setFirmwareVersion (const string &firmwareVersion)
{
    m_firmwareVersion = firmwareVersion;
}

string FrameworkObjectManagerGetFirmwareVersionMessage::getFirmwareVersion () 
{
    return m_firmwareVersion;
}

FrameworkObjectManagerStartCcmdHaSyncMessage::FrameworkObjectManagerStartCcmdHaSyncMessage (const string &peerHaIpAddress, const SI32 &peerHaIpPort)
      :PrismMessage (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_START_CCMD_HA_SYNC_MESSAGE),
       m_peerHaIpAddress(peerHaIpAddress),
       m_peerHaIpPort(peerHaIpPort),
       m_firmwareVersion (""),
       m_haVersion (0),
       m_returnData ("")
{
}

FrameworkObjectManagerStartCcmdHaSyncMessage::~FrameworkObjectManagerStartCcmdHaSyncMessage()
{
}

void FrameworkObjectManagerStartCcmdHaSyncMessage::setPeerHaIpAddress(const string &peerHaIpAddress)
{
    m_peerHaIpAddress = peerHaIpAddress;
}

void FrameworkObjectManagerStartCcmdHaSyncMessage::setPeerHaIpPort(const SI32 &peerHaIpPort)
{
    m_peerHaIpPort = peerHaIpPort;
}

string FrameworkObjectManagerStartCcmdHaSyncMessage::getPeerHaIpAddress() const
{
    return m_peerHaIpAddress;
}

SI32 FrameworkObjectManagerStartCcmdHaSyncMessage::getPeerHaIpPort() const
{
    return m_peerHaIpPort;
}

void FrameworkObjectManagerStartCcmdHaSyncMessage::setFirmwareVersion(const string &firmwareVersion)
{
    m_firmwareVersion = firmwareVersion;
}

string FrameworkObjectManagerStartCcmdHaSyncMessage::getFirmwareVersion() const
{
    return m_firmwareVersion;
}

void FrameworkObjectManagerStartCcmdHaSyncMessage::setHAVersion(const UI32 &haVersion)
{
    m_haVersion = haVersion;
}

UI32 FrameworkObjectManagerStartCcmdHaSyncMessage::getHAVersion() const
{
    return m_haVersion;
}

void FrameworkObjectManagerStartCcmdHaSyncMessage::setReturnData (const string &returnData)
{
    m_returnData = returnData;
}

string FrameworkObjectManagerStartCcmdHaSyncMessage::getReturnData () const
{
    return m_returnData ;
}

void FrameworkObjectManagerStartCcmdHaSyncMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeString  (&m_peerHaIpAddress,    "peerHaIpAddress"));
     addSerializableAttribute (new AttributeSI32    (&m_peerHaIpPort,       "peerHaIpPort"));
     addSerializableAttribute (new AttributeString  (&m_firmwareVersion,    "firmwareVersion"));
     addSerializableAttribute (new AttributeUI32    (&m_haVersion,          "haVersion"));
     addSerializableAttribute (new AttributeString  (&m_returnData,         "returnData"));
}

FrameworkObjectManagerCcmdHaSyncUpdateMessage::FrameworkObjectManagerCcmdHaSyncUpdateMessage ()
    : ManagementInterfaceMessage (PrismFrameworkObjectManager::getServiceName (), FRAMEWORK_OBJECT_MANAGER_CCMD_HA_SYNC_UPDATE_MESSAGE)
{
}

FrameworkObjectManagerCcmdHaSyncUpdateMessage::~FrameworkObjectManagerCcmdHaSyncUpdateMessage ()
{
}

void FrameworkObjectManagerCcmdHaSyncUpdateMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();
}

FrameworkObjectManagerSetSyncStateMessage::FrameworkObjectManagerSetSyncStateMessage ()
    : PrismMessage (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_SET_SYNC_STATE_MESSAGE),
      m_syncState (UNKNOWN_SYNC_STATE)
{
}

FrameworkObjectManagerSetSyncStateMessage::FrameworkObjectManagerSetSyncStateMessage (const UI32 &syncState)
    : PrismMessage (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_SET_SYNC_STATE_MESSAGE),
      m_syncState (syncState)
{
}

FrameworkObjectManagerSetSyncStateMessage::~FrameworkObjectManagerSetSyncStateMessage ()
{
}

void FrameworkObjectManagerSetSyncStateMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeUI32 (&m_syncState, "syncState"));
}

void FrameworkObjectManagerSetSyncStateMessage::setSyncState (const UI32 &syncState)
{
    m_syncState = syncState;
}

UI32 FrameworkObjectManagerSetSyncStateMessage::getSyncState () const
{
    return m_syncState;
}

FrameworkObjectManagerGetSyncStateMessage::FrameworkObjectManagerGetSyncStateMessage ()
    : PrismMessage (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_GET_SYNC_STATE_MESSAGE),
      m_syncState (UNKNOWN_SYNC_STATE)
{
}

FrameworkObjectManagerGetSyncStateMessage::~FrameworkObjectManagerGetSyncStateMessage ()
{
}

void FrameworkObjectManagerGetSyncStateMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeUI32 (&m_syncState, "syncState"));
}

void FrameworkObjectManagerGetSyncStateMessage::setSyncState (const UI32 &syncState)
{
    m_syncState = syncState;
}

UI32 FrameworkObjectManagerGetSyncStateMessage::getSyncState () const
{
    return m_syncState;
}

FrameworkObjectManagerPrepareForWarmRecoveryWithDefaultConfiguration::FrameworkObjectManagerPrepareForWarmRecoveryWithDefaultConfiguration ()
    : PrismMessage (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_PREPARE_FOR_WARM_RECOV_WITH_DEFAULT_CONFIG_MESSAGE)
{
}

FrameworkObjectManagerPrepareForWarmRecoveryWithDefaultConfiguration::~FrameworkObjectManagerPrepareForWarmRecoveryWithDefaultConfiguration ()
{
}

void FrameworkObjectManagerPrepareForWarmRecoveryWithDefaultConfiguration::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();
}

FrameworkObjectManagerPrepareForHaFailoverWithPersistentConfiguration::FrameworkObjectManagerPrepareForHaFailoverWithPersistentConfiguration ()
    : PrismMessage (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_PREPARE_FOR_HA_RECOV_WITH_PERSISTENT_CONFIG_MESSAGE),
      m_isWarmRecoveryInvolved (false)
{
}

FrameworkObjectManagerPrepareForHaFailoverWithPersistentConfiguration::~FrameworkObjectManagerPrepareForHaFailoverWithPersistentConfiguration ()
{
}

void FrameworkObjectManagerPrepareForHaFailoverWithPersistentConfiguration::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeBool (&m_isWarmRecoveryInvolved, "isWarmRecoveryInvolved"));
}

bool FrameworkObjectManagerPrepareForHaFailoverWithPersistentConfiguration::isWarmHaRecoveryPreparationInProgress ()
{
    return (m_isWarmRecoveryInvolved);
}

void FrameworkObjectManagerPrepareForHaFailoverWithPersistentConfiguration::setWarmHaRecoveryPreparationInProgress (const bool &isWarmRecovery)
{
    m_isWarmRecoveryInvolved = isWarmRecovery;
}

FrameworkObjectManagerSyncConfigurationMessage::FrameworkObjectManagerSyncConfigurationMessage ()
     :PrismMessage (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_SYNC_CONFIGURATION_MESSAGE),
      m_prismFrameworkConfigurationFileName ("")
{
}

FrameworkObjectManagerSyncConfigurationMessage::FrameworkObjectManagerSyncConfigurationMessage (const string &prismFrameworkConfigurationFileName)
     :PrismMessage (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_SYNC_CONFIGURATION_MESSAGE),
      m_prismFrameworkConfigurationFileName (prismFrameworkConfigurationFileName)
{
}

FrameworkObjectManagerSyncConfigurationMessage::~FrameworkObjectManagerSyncConfigurationMessage ()
{
}

void FrameworkObjectManagerSyncConfigurationMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeString (&m_prismFrameworkConfigurationFileName, "prismFrameworkConfigurationFileName"));
 //    addSerializableAttribute (new AttributeSerializableObject<PrismFrameworkConfiguration> (&m_prismFrameworkConfigurationObject, "prismFrameworkConfigurationObject"));
}

string FrameworkObjectManagerSyncConfigurationMessage::getPrismFrameworkConfigurationFileName () const
{
    return (m_prismFrameworkConfigurationFileName);
}

void FrameworkObjectManagerSyncConfigurationMessage::setPrismFrameworkConfigurationFileName (const string &prismFrameworkConfigurationFileName)
{
    m_prismFrameworkConfigurationFileName = prismFrameworkConfigurationFileName;
}

PrismFrameworkConfiguration FrameworkObjectManagerSyncConfigurationMessage::getPrismFrameworkConfigurationObject () const
{
    return (m_prismFrameworkConfigurationObject);
}

void FrameworkObjectManagerSyncConfigurationMessage::setPrismFrameworkConfigurationObject (const PrismFrameworkConfiguration &prismFrameworkConfiguration)
{
    m_prismFrameworkConfigurationObject = prismFrameworkConfiguration;
}

}
