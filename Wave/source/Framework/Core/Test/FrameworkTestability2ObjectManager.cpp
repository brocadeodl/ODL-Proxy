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

#include "Framework/Core/Test/FrameworkTestability2ObjectManager.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Messaging/Local/Test/FrameworkTestabilityEvents.h"
#include "Framework/Messaging/Local/Test/FrameworkLocalMessagingTestTypes.h"
#include "Framework/Messaging/Local/Test/FrameworkLocalMessagingTestObjectManager.h"
#include "Framework/Core/WaveNewNodesAddedEvent.h"
#include "Framework/Core/WaveNodesAdditionToClusterCompletedEvent.h"
#include "Framework/Core/WaveNodeLocalNodeDeletedEvent.h"
#include "Framework/Core/PrimaryChangedEvent.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/Core/FrameworkObjectManagerTypes.h"
#include "Framework/Persistence/StartupSchemaChangeEvent.h"
#include "Framework/Persistence/PersistenceLocalObjectManager.h"
#include "Framework/Persistence/PersistenceObjectManagerTypes.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"

namespace WaveNs
{

FrameworkTestability2ObjectManager::FrameworkTestability2ObjectManager ()
    : WaveLocalObjectManager ("Framework Testability2")
{
}

FrameworkTestability2ObjectManager *FrameworkTestability2ObjectManager::getInstance ()
{
    static FrameworkTestability2ObjectManager *pFrameworkTestability2ObjectManager = NULL;

    if (NULL == pFrameworkTestability2ObjectManager)
    {
        pFrameworkTestability2ObjectManager = new FrameworkTestability2ObjectManager ();
        WaveNs::prismAssert (NULL != pFrameworkTestability2ObjectManager, __FILE__, __LINE__);
    }

    return (pFrameworkTestability2ObjectManager);
}

FrameworkTestability2ObjectManager::~FrameworkTestability2ObjectManager ()
{
}

PrismServiceId FrameworkTestability2ObjectManager::getPrismServiceId ()
{
    return ((getInstance ())->getServiceId ());
}

void FrameworkTestability2ObjectManager::listenForEvents (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    listenForEvent (FrameworkLocalMessagingTestObjectManager::getPrismServiceId (), FRAMEWORK_MESSAGING_LOCAL_TEST_EVENT1, reinterpret_cast<PrismEventHandler> (&FrameworkTestability2ObjectManager::frameworkTestabilityEvent1EventHandler));
    listenForEvent (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_BROADCAST_ADDITION_OF_NEW_NODES_EVENT, reinterpret_cast<PrismEventHandler> (&FrameworkTestability2ObjectManager::WaveNewNodesAddedEventHandler));
    listenForEvent (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_NODES_ADDITION_TO_CLUSTER_COMPLETED_EVENT, reinterpret_cast<PrismEventHandler> (&FrameworkTestability2ObjectManager::WaveNodesAdditionToClusterCompletedEventHandler));
    listenForEvent (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_LOCAL_NODE_DELETED_EVENT, reinterpret_cast<PrismEventHandler> (&FrameworkTestability2ObjectManager::WaveNodeLocalNodeDeletedEventHandler));
    listenForEvent (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_PRIMARY_CHANGED_EVENT, reinterpret_cast<PrismEventHandler> (&FrameworkTestability2ObjectManager::PrimaryChangedEventHandler));
    listenForEvent (PersistenceLocalObjectManager::getPrismServiceId (), STARTUP_SCHEMA_CHANGE_EVENT, reinterpret_cast<PrismEventHandler> (&FrameworkTestability2ObjectManager::StartupSchemaChangeEventHandler));

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForBootPhases->callback ();
}

void FrameworkTestability2ObjectManager::frameworkTestabilityEvent1EventHandler (const FrameworkTestabilityEvent1 *&pEvent)
{
    static UI32 numberOfEventsRceivedSoFar = 0;

    numberOfEventsRceivedSoFar++;

    if (0 == (numberOfEventsRceivedSoFar % 10000))
    {
        trace (TRACE_LEVEL_INFO, string ("FrameworkTestability2ObjectManager::frameworkTestabilityEvent1EventHandler : Received Events ... ") + numberOfEventsRceivedSoFar);
        trace (TRACE_LEVEL_DEBUG, string ("m_eventUI32   = ") + pEvent->getEventUI32 ());
        trace (TRACE_LEVEL_DEBUG, string ("m_eventString = ") + pEvent->getEventString ());
    }

     reply (reinterpret_cast<const PrismEvent *&> (pEvent));
}

/// Name
/// WaveNewNodesAddedEventHandler
/// Description
/// Test Handler for handling broadcast event thrown during add node 
/// or cluster creation
/// Input
/// const WaveNewNodesAddedEvent* & : Actual event object
/// Output
/// none
/// Return
/// none
void FrameworkTestability2ObjectManager::WaveNewNodesAddedEventHandler(const WaveNewNodesAddedEvent* & pEvent)
{
  trace(TRACE_LEVEL_INFO, "FrameworkTestability2ObjectManager::WaveNewNodesAddedEventHandler..Entering");
  UI32 number_of_nodes = pEvent->getNumberOfNewLocations();

  for(UI32 i=0; i<number_of_nodes; ++i) 
  {
      tracePrintf(TRACE_LEVEL_INFO,"IpAddress: %s: Port: %d: LocationId: %d",pEvent->getIpAddressAtIndex(i).c_str(), pEvent->getPortAtIndex(i), pEvent->getLocationIdAtIndex(i));
  }

   reply (reinterpret_cast<const PrismEvent *&> (pEvent));

   trace(TRACE_LEVEL_INFO, "FrameworkTestability2ObjectManager::WaveNewNodesAddedEventHandler..Exiting");
  return;

}

/// Name
/// WaveNodesAdditionToClusterCompletedEventHandler
/// Description
/// Test Handler for handling broadcast event thrown after the completion of cluster formation
/// Input
/// const WaveNodesAdditionToClusterCompletedEvent* & : Actual event object
/// Output
/// none
/// Return
/// none
void FrameworkTestability2ObjectManager::WaveNodesAdditionToClusterCompletedEventHandler(const WaveNodesAdditionToClusterCompletedEvent* & pEvent)
{
  trace(TRACE_LEVEL_INFO, "FrameworkTestability2ObjectManager::WaveNodesAdditionToClusterCompletedEventHandler..Entering");
  UI32 number_of_nodes = pEvent->getNumberOfNewLocations();

  for(UI32 i=0; i<number_of_nodes; ++i) 
  {
      tracePrintf(TRACE_LEVEL_INFO,"IpAddress: %s: Port: %d: LocationId: %d",pEvent->getIpAddressAtIndex(i).c_str(), pEvent->getPortAtIndex(i), pEvent->getLocationIdAtIndex(i));
  }

   reply (reinterpret_cast<const PrismEvent *&> (pEvent));

   trace(TRACE_LEVEL_INFO, "FrameworkTestability2ObjectManager::WaveNodesAdditionToClusterCompletedEventHandler..Exiting");
  return;

}


/// Name
/// WaveNodeLocalNodeDeletedEventHandler
/// Description
/// Test Handler for handling broadcast event thrown during delete
/// node on the secondary
/// Input
/// const WaveNewNodesAddedEvent* & : Actual event object
/// Output
/// none
/// Return
/// none
void FrameworkTestability2ObjectManager::WaveNodeLocalNodeDeletedEventHandler(const WaveNodeLocalNodeDeletedEvent* & pEvent)
{
  trace(TRACE_LEVEL_INFO, "FrameworkTestability2ObjectManager::WaveNodeLocalNodeDeletedEventHandler..Entering");

  reply (reinterpret_cast<const PrismEvent *&> (pEvent));

  return;
}

// Name : PrimaryChangedEventHandler
// Description : The handler for an event PrimaryChangedEvent which is broadcasted by New Primary and all secondaries in a
//               cluster on Old Primary failover. 
// Input       : const PrimaryChangedEvent* & : actual event object
// Output      : None.

void FrameworkTestability2ObjectManager::PrimaryChangedEventHandler (const PrimaryChangedEvent* &pEvent)
{
    trace (TRACE_LEVEL_INFO, "FrameworkTestability2ObjectManager::PrimaryChangedEventHandler : Entering...");
    const LocationId newPrimaryLocation = pEvent->getNewPrimaryLocationId ();

    trace (TRACE_LEVEL_INFO, string("New Primary locationId : ") + newPrimaryLocation);
    
    reply (reinterpret_cast<const PrismEvent *&> (pEvent));

    trace (TRACE_LEVEL_INFO, "FrameworkTestability2ObjectManager::PrimaryChangedEventHandler : Exiting...");
    return;
}

void FrameworkTestability2ObjectManager::StartupSchemaChangeEventHandler (const StartupSchemaChangeEvent* & pEvent)
{
    trace (TRACE_LEVEL_DEBUG, "FrameworkTestability2ObjectManager::StartupSchemaChangeEventHandler : Entering...");

    const bool          operationStatus = pEvent->getOperationStatus ();
    const SchemaType    fromSchema      = pEvent->getFromSchema ();
    const string        schemaFile      = pEvent->getFromFileSchemaPath ();   
    const SchemaType    toSchema        = pEvent->getToSchema ();
    const string        toSchemaFile    = pEvent->getToFileSchemaPath ();   
    
    trace (TRACE_LEVEL_DEBUG, string ("Operation status : ") + operationStatus);
    
    string originSchema = pEvent->getSchemaName (fromSchema);
    string targetSchema = pEvent->getSchemaName (toSchema);

    trace (TRACE_LEVEL_DEBUG, string ("operation is : COPY ") + originSchema + string (" ") + targetSchema); 

    trace (TRACE_LEVEL_DEBUG, string ("fromFilePath : ") + schemaFile);
    trace (TRACE_LEVEL_DEBUG, string ("toFilePath : ") + toSchemaFile);

    reply (reinterpret_cast<const PrismEvent *&> (pEvent));
}

}
