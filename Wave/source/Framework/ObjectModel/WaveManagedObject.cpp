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

#include "Framework/ObjectModel/WaveManagedObject.h"
#include "Framework/ObjectModel/WaveLocalManagedObjectBase.h"
#include "Framework/MultiThreading/PrismThread.h"
#include "Framework/Database/DatabaseObjectManager.h"
#include "Framework/ObjectModel/WaveManagedObjectLoadOperationalDataContext.h"
#include "Framework/ObjectModel/LoadOperationalDataSynchronousContext.h"
#include "Framework/Core/PrismFrameworkMessages.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/ObjectTracker/ObjectTracker.h"
#include "Framework/Attributes/AttributeResourceId.h"
#include "Framework/ObjectRelationalMapping/OrmRepository.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContextForDeletion.h"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.h"
#include "Framework/ObjectModel/WaveManagedObjectUpdateContext.h"
#include "Framework/ObjectModel/WaveManagedObjectCreateContext.h"
#include "Framework/ObjectModel/WaveManagedObjectDeleteContext.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/Database/DatabaseObjectManagerExecuteQueryMessage.h"
#include "Framework/ObjectModel/WaveSendMulticastContext.h"
#include "Modeling/YANG/ObjectModel/YangDisplayConfigurationContext.h"
#include "Framework/ObjectModel/WaveObjectManagerCommitTransactionContext.h"
#include "Framework/CliBlockManagement/CliBlockMessage.h"

#include "libpq-fe.h"

#include <map>
#include <stdarg.h>
#include <Framework/Attributes/AttributeDateTime.h>

using namespace std;

namespace WaveNs
{

WaveManagedObject::WaveManagedObject (WaveObjectManager *pWaveObjectManager)
    : PrismElement                     (pWaveObjectManager),
      PrismPersistableObject           (WaveManagedObject::getClassName (), PrismPersistableObject::getClassName ()),
      m_pCurrentOwnerWaveObjectManager (pWaveObjectManager),
      m_isPartitionNameSetByUser(false)
{
    ObjectId thisObjectId = getObjectId ();

    m_name = thisObjectId.toString ();

    // FIXME: sagar : May be we need to initialize the generic and specific status here.
    m_genericStatus  = 0;
    m_specificStatus = 0;

    // Request Object Manager to track this object if necessary.
    // Do not request for tracking if the Wave Managed Object is being instantiated because of query

    if ((PrismThread::getSelf ()) != (PrismThread::getPrismThreadForServiceId (DatabaseObjectManagerExecuteQueryMessage::getDatabaseObjectManagerServiceId ()))->getId ())
    {
        pWaveObjectManager->trackObjectCreatedDuringCurrentTransaction (this);

        ObjectTracker::addToObjectTracker (this);
    }

    m_lastModifiedTimeStamp = m_createdTimeStamp;
}

WaveManagedObject::~WaveManagedObject ()
{
    // Request Object Manager to track this object if necessary.
    // Do not request for tracking if the Wave Managed Object is being instantiated because of query

    if ((PrismThread::getSelf ()) != (PrismThread::getPrismThreadForServiceId (DatabaseObjectManagerExecuteQueryMessage::getDatabaseObjectManagerServiceId ()))->getId ())
    {
        m_pCurrentOwnerWaveObjectManager->trackObjectDeletedDuringCurrentTransaction (this);
    }

    ObjectTracker::deleteFromObjectTracker (this);
}

string WaveManagedObject::getClassName ()
{
    return ("WaveManagedObject");
}

WaveMessageStatus WaveManagedObject::send  (PrismMessage *pPrismMessage, PrismMessageResponseHandler pPrismMessageCallback, void *pPrismMessageContext, UI32 timeOutInMilliSeconds, LocationId locationId, PrismElement *pPrismMessageSender)
{
    return (m_pCurrentOwnerWaveObjectManager->send (pPrismMessage, pPrismMessageCallback, pPrismMessageContext, timeOutInMilliSeconds, locationId, pPrismMessageSender != NULL ? pPrismMessageSender : this));
}

WaveMessageStatus WaveManagedObject::sendOneWay (PrismMessage *pPrismMessage, const LocationId &locationId)
{
    return (m_pCurrentOwnerWaveObjectManager->sendOneWay (pPrismMessage, locationId));
}

WaveMessageStatus WaveManagedObject::sendOneWayToFront (PrismMessage *pPrismMessage, const LocationId &locationId)
{
    return (m_pCurrentOwnerWaveObjectManager->sendOneWayToFront (pPrismMessage, locationId));
}

WaveMessageStatus WaveManagedObject::sendSynchronously (PrismMessage *pPrismMessage, const LocationId &locationId)
{
    return (m_pCurrentOwnerWaveObjectManager->sendSynchronously (pPrismMessage, locationId));
}

void WaveManagedObject::sendToWaveCluster (WaveSendToClusterContext *pWaveSendToClusterContext)
{
    m_pCurrentOwnerWaveObjectManager->sendToWaveCluster (pWaveSendToClusterContext);
}

void WaveManagedObject::sendOneWayToWaveCluster (WaveSendToClusterContext *pWaveSendToClusterContext)
{
    m_pCurrentOwnerWaveObjectManager->sendOneWayToWaveCluster (pWaveSendToClusterContext);
}

bool WaveManagedObject::isBeingSurrogated ()
{
    return (m_pCurrentOwnerWaveObjectManager->isBeingSurrogated ());
}

WaveMessageStatus WaveManagedObject::recall (PrismMessage *pPrismMessage)
{
    return (m_pCurrentOwnerWaveObjectManager->recall (pPrismMessage));
}

WaveMessageStatus WaveManagedObject::reply (PrismMessage *pPrismMessage)
{
    return (m_pCurrentOwnerWaveObjectManager->reply (pPrismMessage));
}

WaveMessageStatus WaveManagedObject::broadcast (PrismEvent *pPrismEvent)
{
    return (m_pCurrentOwnerWaveObjectManager->broadcast (pPrismEvent));
}

void WaveManagedObject::trace (TraceLevel traceLevel, const string &stringToTrace)
{
    m_pCurrentOwnerWaveObjectManager->trace (traceLevel, stringToTrace);
}

void WaveManagedObject::tracePrintf (TraceLevel traceLevel, const bool &addNewLine, const bool &suppressPrefix, const char * const pFormat, ...)
{
    va_list variableArguments;

    va_start (variableArguments, pFormat);

    m_pCurrentOwnerWaveObjectManager->tracePrintf (traceLevel, addNewLine, suppressPrefix, pFormat, variableArguments);

    va_end (variableArguments);
}

void WaveManagedObject::tracePrintf (TraceLevel traceLevel, const char * const pFormat, ...)
{
    va_list variableArguments;

    va_start (variableArguments, pFormat);

    m_pCurrentOwnerWaveObjectManager->tracePrintf (traceLevel, pFormat, variableArguments);

    va_end (variableArguments);
}

void WaveManagedObject::prismAssert (bool isAssertNotRequired, const char *pFileName, UI32 lineNumber)
{
    m_pCurrentOwnerWaveObjectManager->prismAssert (isAssertNotRequired, pFileName, lineNumber);
}

void WaveManagedObject::addOperationMap (UI32 operationCode, PrismMessageHandler pPrismMessageHandler, PrismElement *pPrismElement)
{
    if (NULL == pPrismElement)
    {
        pPrismElement = this;
    }

    m_pCurrentOwnerWaveObjectManager->addOperationMap (operationCode, pPrismMessageHandler, pPrismElement);
}

ResourceId WaveManagedObject::startTimer (TimerHandle &timerHandle, timeval &startInterval, timeval &periodicInterval, PrismTimerExpirationHandler pPrismTimerExpirationCallback, void *pPrismTimerExpirationContext, PrismElement *pPrismTimerSender)
{
    return (m_pCurrentOwnerWaveObjectManager->startTimer (timerHandle, startInterval, periodicInterval, pPrismTimerExpirationCallback, pPrismTimerExpirationContext, pPrismTimerSender != NULL ? pPrismTimerSender : this));
}

ResourceId WaveManagedObject::startTimer (TimerHandle &timerHandle, UI32 timeInMilliSeconds, PrismTimerExpirationHandler pPrismTimerExpirationCallback, void *pPrismTimerExpirationContext, PrismElement *pPrismTimerSender)
{
    return (m_pCurrentOwnerWaveObjectManager->startTimer (timerHandle, timeInMilliSeconds, pPrismTimerExpirationCallback, pPrismTimerExpirationContext, pPrismTimerSender != NULL ? pPrismTimerSender : this));
}

ResourceId WaveManagedObject::deleteTimer (TimerHandle timerHandle)
{
    return (m_pCurrentOwnerWaveObjectManager->deleteTimer (timerHandle));
}

void WaveManagedObject::holdMessages ()
{
    m_pCurrentOwnerWaveObjectManager->holdMessages ();
}

void WaveManagedObject::holdHighPriorityMessages ()
{
    m_pCurrentOwnerWaveObjectManager->holdHighPriorityMessages ();
}

void WaveManagedObject::holdEvents ()
{
    m_pCurrentOwnerWaveObjectManager->holdEvents ();
}

void WaveManagedObject::holdAll ()
{
    m_pCurrentOwnerWaveObjectManager->holdAll ();
}

void WaveManagedObject::unholdMessages ()
{
    m_pCurrentOwnerWaveObjectManager->unholdMessages ();
}

void WaveManagedObject::unholdHighPriorityMessages ()
{
    m_pCurrentOwnerWaveObjectManager->unholdHighPriorityMessages ();
}

void WaveManagedObject::unholdEvents ()
{
    m_pCurrentOwnerWaveObjectManager->unholdEvents ();
}

void WaveManagedObject::unholdAll ()
{
    m_pCurrentOwnerWaveObjectManager->unholdAll ();
}

UI32 WaveManagedObject::getNumberOfPendingTimerExpirationMessages ()
{
    return (m_pCurrentOwnerWaveObjectManager->getNumberOfPendingTimerExpirationMessages ());
}

UI32 WaveManagedObject::getNumberOfPendingNormalMessages ()
{
    return (m_pCurrentOwnerWaveObjectManager->getNumberOfPendingNormalMessages ());
}

UI32 WaveManagedObject::getNumberOfPendingHighPriorityMessages ()
{
    return (m_pCurrentOwnerWaveObjectManager->getNumberOfPendingHighPriorityMessages ());
}

string WaveManagedObject::getName () const
{
    return (m_name);
}

void WaveManagedObject::setName (const string &name)
{
    m_name = name;
}

ResourceId WaveManagedObject::getGenericStatus () const
{
    return (m_genericStatus);
}

void WaveManagedObject::setGenericStatus (ResourceId genericStatus)
{
    m_genericStatus = genericStatus;
}

ResourceId WaveManagedObject::getSpecificStatus () const
{
    return (m_specificStatus);
}

void WaveManagedObject::setSpecificStatus (ResourceId specificStatus)
{
    m_specificStatus = specificStatus;
}

void WaveManagedObject::setupAttributesForPersistence ()
{
    PrismPersistableObject::setupAttributesForPersistence ();

    addPersistableAttribute (new AttributeString     (&m_name,                           "name"));
    addPersistableAttribute (new AttributeResourceId (&m_genericStatus,                  "genericStatus"));
    addPersistableAttribute (new AttributeResourceId (&m_specificStatus,                 "specificStatus"));
    addPersistableAttribute (new AttributeObjectId   (&m_ownerManagedObjectId,           "ownerManagedObjectId"));
    addPersistableAttribute (new AttributeObjectId   (&m_ownerPartitionManagedObjectId,  "ownerPartitionManagedObjectId"));
    addPersistableAttribute (new AttributeObjectId   (&m_ownerUserManagedObjectId,       "ownerUserManagedObjectId"));
    addPersistableAttribute (new AttributeDateTime   (&m_createdTimeStamp,               "createdTimeStamp"));
    addPersistableAttribute (new AttributeDateTime   (&m_lastModifiedTimeStamp,          "lastModifiedTimeStamp"));
}

void WaveManagedObject::setupAttributesForCreate ()
{
    PrismPersistableObject::setupAttributesForCreate ();

    addPersistableAttributeForCreate (new AttributeString     (&m_name,                           "name"));
    addPersistableAttributeForCreate (new AttributeResourceId (&m_genericStatus,                  "genericStatus"));
    addPersistableAttributeForCreate (new AttributeResourceId (&m_specificStatus,                 "specificStatus"));
    addPersistableAttributeForCreate (new AttributeObjectId   (&m_ownerManagedObjectId,           "ownerManagedObjectId"));
    addPersistableAttributeForCreate (new AttributeObjectId   (&m_ownerPartitionManagedObjectId,  "ownerPartitionManagedObjectId"));
    addPersistableAttributeForCreate (new AttributeObjectId   (&m_ownerUserManagedObjectId,       "ownerUserManagedObjectId"));
    addPersistableAttributeForCreate (new AttributeDateTime   (&m_createdTimeStamp,               "createdTimeStamp"));
    addPersistableAttributeForCreate (new AttributeDateTime   (&m_lastModifiedTimeStamp,          "lastModifiedTimeStamp"));
}

void WaveManagedObject::startTransaction ()
{
    m_pCurrentOwnerWaveObjectManager->startTransaction ();
}

bool WaveManagedObject::isTransactionInProgress ()
{
    return (m_pCurrentOwnerWaveObjectManager->isTransactionInProgress ());
}

ResourceId WaveManagedObject::commitTransaction (WaveObjectManagerCommitTransactionContext *pWaveObjectManagerCommitTransactionContext)
{
    return (m_pCurrentOwnerWaveObjectManager->commitTransaction (pWaveObjectManagerCommitTransactionContext));
}

void WaveManagedObject::rollbackTransaction ()
{
    m_pCurrentOwnerWaveObjectManager->rollbackTransaction ();
}

void WaveManagedObject::query (WaveManagedObjectQueryContext *pWaveManagedObjectQueryContext)
{
    m_pCurrentOwnerWaveObjectManager->query (pWaveManagedObjectQueryContext);
}

vector<WaveManagedObject *> *WaveManagedObject::querySynchronouslyLocalManagedObjectsForLocationId (const LocationId &locationId, const string &className)
{
    return (m_pCurrentOwnerWaveObjectManager->querySynchronouslyLocalManagedObjectsForLocationId (locationId, className));
}

vector<WaveManagedObject *> *WaveManagedObject::querySynchronouslyLocalManagedObjectsForLocationId (const LocationId &locationId, const string &className, const vector<string> &selectFields)
{
    return (m_pCurrentOwnerWaveObjectManager->querySynchronouslyLocalManagedObjectsForLocationId (locationId, className, selectFields));
}

vector<WaveManagedObject *> *WaveManagedObject::querySynchronouslyLocalManagedObjectsForLocationId (const LocationId &locationId, WaveManagedObjectSynchronousQueryContext *pWaveManagedObjectSynchronousQueryContext)
{
    return (m_pCurrentOwnerWaveObjectManager->querySynchronouslyLocalManagedObjectsForLocationId (locationId, pWaveManagedObjectSynchronousQueryContext));
}

vector<WaveManagedObject *> *WaveManagedObject::querySynchronously (WaveManagedObjectSynchronousQueryContext *pWaveManagedObjectSynchronousQueryContext)
{
    return (m_pCurrentOwnerWaveObjectManager->querySynchronously (pWaveManagedObjectSynchronousQueryContext));
}

ResourceId WaveManagedObject::querySynchronouslyForCount (WaveManagedObjectSynchronousQueryContext *pWaveManagedObjectSynchronousQueryContext, UI32 &count)
{
    return (m_pCurrentOwnerWaveObjectManager->querySynchronouslyForCount (pWaveManagedObjectSynchronousQueryContext, count));
}

ResourceId WaveManagedObject::querySynchronouslyForCount (const string &managedClassName, UI32 &count, const string &schema)
{
    return (m_pCurrentOwnerWaveObjectManager->querySynchronouslyForCount (managedClassName, count, schema));
}

ResourceId WaveManagedObject::querySynchronouslyForCount (const string &managedClassName, const string &fieldName, const string &range, UI32 &count, const string &schema)
{
    return (m_pCurrentOwnerWaveObjectManager->querySynchronouslyForCount (managedClassName, fieldName, range, count, schema));
}

vector<WaveManagedObject *> *WaveManagedObject::querySynchronously (const string &managedClassName, const string &schema)
{
    return (m_pCurrentOwnerWaveObjectManager->querySynchronously (managedClassName, schema));
}

vector<WaveManagedObject *> *WaveManagedObject::querySynchronously (const string &managedClassName, vector<ObjectId> &objectIds, const string &schema)
{
    return (m_pCurrentOwnerWaveObjectManager->querySynchronously (managedClassName, objectIds, schema));
}

vector<WaveManagedObject *> *WaveManagedObject::querySynchronouslyByName (const string &managedClassName, const string &managedObjectName, const string &schema)
{
    return (m_pCurrentOwnerWaveObjectManager->querySynchronouslyByName (managedClassName, managedObjectName, schema));
}

void WaveManagedObject::updateWaveManagedObject (WaveManagedObject *pWaveManagedObject)
{
    m_pCurrentOwnerWaveObjectManager->updateWaveManagedObject (pWaveManagedObject);
}

void WaveManagedObject::deleteWaveManagedObject (const ObjectId &objectId)
{
    m_pCurrentOwnerWaveObjectManager->deleteWaveManagedObject (objectId);
}

void WaveManagedObject::deleteWaveManagedObject (WaveManagedObjectSynchronousQueryContextForDeletion *pWaveManagedObjectSynchronousQueryContextForDeletion)
{
    m_pCurrentOwnerWaveObjectManager->deleteWaveManagedObject (pWaveManagedObjectSynchronousQueryContextForDeletion);
}

void WaveManagedObject::updateMultipleWaveManagedObjects (WaveManagedObjectSynchronousQueryContextForUpdate *pWaveManagedObjectSynchronousQueryContextForUpdate)
{
    m_pCurrentOwnerWaveObjectManager->updateMultipleWaveManagedObjects (pWaveManagedObjectSynchronousQueryContextForUpdate);
}

void WaveManagedObject::deleteMultipleWaveManagedObjects (WaveManagedObjectSynchronousQueryContextForDeletion *pWaveManagedObjectSynchronousQueryContextForDeletion)
{
    m_pCurrentOwnerWaveObjectManager->deleteMultipleWaveManagedObjects (pWaveManagedObjectSynchronousQueryContextForDeletion);
}

void WaveManagedObject::deleteManagedObjectCompositions (WaveManagedObjectSynchronousQueryContextForDeletion *pWaveManagedObjectSynchronousQueryContextForDeletion, const set<string> relationsSet)
{
    m_pCurrentOwnerWaveObjectManager->deleteManagedObjectCompositions (pWaveManagedObjectSynchronousQueryContextForDeletion, relationsSet);
}

void WaveManagedObject::deleteManagedObjectCompositions (const ObjectId &objectId, const set<string> relationsSet)
{
    m_pCurrentOwnerWaveObjectManager->deleteManagedObjectCompositions (objectId, relationsSet);
}

bool WaveManagedObject::isManagedClassSupported (const string &managedClass)
{
    return (m_pCurrentOwnerWaveObjectManager->isManagedClassSupported (managedClass));
}

void WaveManagedObject::addRelationship (const string &parentClassName, const string &childClassName, const string &relationshipName, const ObjectId &parentObjectId, const ObjectId &childObjectId)
{
    m_pCurrentOwnerWaveObjectManager->addRelationship (parentClassName, childClassName, relationshipName, parentObjectId, childObjectId);
}

void WaveManagedObject::deleteRelationship (const string &parentClassName, const string &childClassName, const string &relationshipName, const ObjectId &parentObjectId, const ObjectId &childObjectId)
{
    m_pCurrentOwnerWaveObjectManager->deleteRelationship (parentClassName, childClassName, relationshipName, parentObjectId, childObjectId);
}

void WaveManagedObject::addToComposition (const string &parentClassName, const string &childCalssName, const string &compositionName, const ObjectId &parentObjectId, const ObjectId &childObjectId)
{
    m_pCurrentOwnerWaveObjectManager->addToComposition (parentClassName, childCalssName, compositionName, parentObjectId, childObjectId);
}

void WaveManagedObject::deleteFromComposition (const string &parentClassName, const string &childCalssName, const string &compositionName, const ObjectId &parentObjectId, const ObjectId &childObjectId)
{
    m_pCurrentOwnerWaveObjectManager->deleteFromComposition (parentClassName, childCalssName, compositionName, parentObjectId, childObjectId);
}

void WaveManagedObject::addToAssociation (const string &parentClassName, const string &childCalssName, const string &associationName, const ObjectId &parentObjectId, const ObjectId &childObjectId)
{
    m_pCurrentOwnerWaveObjectManager->addToAssociation (parentClassName, childCalssName, associationName, parentObjectId, childObjectId);
}

void WaveManagedObject::deleteFromAssociation (const string &parentClassName, const string &childCalssName, const string &associationName, const ObjectId &parentObjectId, const ObjectId &childObjectId)
{
    m_pCurrentOwnerWaveObjectManager->deleteFromAssociation (parentClassName, childCalssName, associationName, parentObjectId, childObjectId);
}

WaveManagedObject *WaveManagedObject::queryManagedObject (const ObjectId &managedObjectId, const string &schema)
{
    return (m_pCurrentOwnerWaveObjectManager->queryManagedObject (managedObjectId, schema));
}

vector<WaveManagedObject *> *WaveManagedObject::queryManagedObjectAssociatedWithSlot (const string &managedClassName, UI32 slotNumber, LocationId locationId, const string &schema)
{
    return (m_pCurrentOwnerWaveObjectManager->queryManagedObjectAssociatedWithSlot (managedClassName, slotNumber, locationId, schema));
}

TraceClientId WaveManagedObject::getTraceClientId ()
{
    return (m_pCurrentOwnerWaveObjectManager->getTraceClientId ());
}

void WaveManagedObject::addEventType (const UI32 &eventOperationCode)
{
    m_pCurrentOwnerWaveObjectManager->addEventType (eventOperationCode);
}

void WaveManagedObject::listenForEvent (PrismServiceId prismServiceCode, UI32 sourceOperationCode, PrismEventHandler pPrismEventHandler, PrismElement *pPrismElement, const LocationId &sourceLocationId)
{
    m_pCurrentOwnerWaveObjectManager->listenForEvent (prismServiceCode, sourceOperationCode, pPrismEventHandler, pPrismElement != NULL ? pPrismElement : this, sourceLocationId);
}

ResourceId WaveManagedObject::sendSynchronouslyToWaveClient (const string &waveClientName, ManagementInterfaceMessage *pManagementInterfaceMessage, const SI32 &Instance)
{
    return (m_pCurrentOwnerWaveObjectManager->sendSynchronouslyToWaveClient (waveClientName, pManagementInterfaceMessage, Instance));
}

WaveMessageStatus WaveManagedObject::sendToWaveServer (const UI32 &waveServerId, ManagementInterfaceMessage *pManagementInterfaceMessage, PrismMessageResponseHandler messageCallback, PrismElement *pPrismMessageSender, void *pInputContext, UI32 timeOutInMilliSeconds)
{
    return (m_pCurrentOwnerWaveObjectManager->sendToWaveServer (waveServerId, pManagementInterfaceMessage, messageCallback, pPrismMessageSender, pInputContext, timeOutInMilliSeconds));
}

ResourceId WaveManagedObject::sendToWaveClient (const string &waveClientName, ManagementInterfaceMessage *pManagementInterfaceMessage, PrismMessageResponseHandler pPrismMessageCallback, void *pPrismMessageContext, UI32 timeOutInMilliSeconds, const SI32 &Instance)
{
    return (m_pCurrentOwnerWaveObjectManager->sendToWaveClient (waveClientName, pManagementInterfaceMessage, pPrismMessageCallback, pPrismMessageContext, timeOutInMilliSeconds, Instance));
}

void WaveManagedObject::sendToWaveClients (WaveSendToClientsContext *pWaveSendToClientsContext)
{
    m_pCurrentOwnerWaveObjectManager->sendToWaveClients (pWaveSendToClientsContext);
}

ResourceId WaveManagedObject::sendOneWayToAllWaveClients(ManagementInterfaceMessage *pManagementInterfaceMessage)
{
    return (m_pCurrentOwnerWaveObjectManager->sendOneWayToAllWaveClients(pManagementInterfaceMessage));
}

WaveManagedObject *WaveManagedObject::createManagedObjectInstance (const string &managedClassName)
{
    trace (TRACE_LEVEL_FATAL, "WaveManagedObject::createManagedObjectInstance : NOT SUPPORTED on ManagedObjects and their derivations.");
    prismAssert (false, __FILE__, __LINE__);
    return (NULL);
}

PrismMessage *WaveManagedObject::createMessageInstance (const UI32 &operationCode)
{
    trace (TRACE_LEVEL_FATAL, "WaveManagedObject::createMessageInstance : NOT SUPPORTED on ManagedObjects and their derivations.");
    prismAssert (false, __FILE__, __LINE__);
    return (NULL);
}

void WaveManagedObject::loadOperationalData (WaveManagedObjectLoadOperationalDataContext *pWaveManagedObjectLoadOperationalDataContext)
{
    trace (TRACE_LEVEL_FATAL, "WaveManagedObject::loadOperationalData : The Derived Managed Objects must overwrite this implementation.");

    prismAssert (false, __FILE__, __LINE__);

    pWaveManagedObjectLoadOperationalDataContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveManagedObjectLoadOperationalDataContext->callback ();
}

ResourceId WaveManagedObject::loadOperationalDataSynchronousWrapper (const vector<string> &operationalDataFields, WaveManagedObject ** const pWaveManagedObject)
{
    WaveNs::PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&WaveManagedObject::loadOperationalDataSynchronouslyLoadStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&WaveManagedObject::loadOperationalDataSynchronouslyRequeryStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&WaveManagedObject::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&WaveManagedObject::prismSynchronousLinearSequencerFailedStep)
    };

    LoadOperationalDataSynchronousContext *pLoadOperationalDataSynchronousContext = new LoadOperationalDataSynchronousContext (reinterpret_cast<PrismMessage *> (NULL), dynamic_cast<PrismElement *> (this), sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pLoadOperationalDataSynchronousContext->setWaveManagedObjectId   (getObjectId ());
    pLoadOperationalDataSynchronousContext->setOperationalDataFields (operationalDataFields);
    pLoadOperationalDataSynchronousContext->setPWaveManagedObject    (NULL);

    ResourceId status = WAVE_MESSAGE_SUCCESS; // pLoadOperationalDataSynchronousContext->execute ();

    status = loadOperationalDataSynchronouslyLoadStep (pLoadOperationalDataSynchronousContext);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        status = loadOperationalDataSynchronouslyRequeryStep (pLoadOperationalDataSynchronousContext);
    }

    *pWaveManagedObject = pLoadOperationalDataSynchronousContext->getPWaveManagedObject ();

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        prismAssert (NULL == (*pWaveManagedObject), __FILE__, __LINE__);
    }
    else
    {
        prismAssert (NULL != (*pWaveManagedObject), __FILE__, __LINE__);
    }

    delete pLoadOperationalDataSynchronousContext;

    return (status);
}

ResourceId WaveManagedObject::loadOperationalDataSynchronouslyLoadStep (LoadOperationalDataSynchronousContext *pLoadOperationalDataSynchronousContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveManagedObject::loadOperationalDataSynchronouslyLoadStep : Entering ...");

    ObjectId                                                      waveManagedObjectId         = pLoadOperationalDataSynchronousContext->getWaveManagedObjectId ();
    vector<string>                                                operationalDataFields       = pLoadOperationalDataSynchronousContext->getOperationalDataFields ();
    WaveObjectManager                                            *pWaveObjectManager          = getPWaveObjectManager ();
    PrismServiceId                                                ownerServiceId              = pWaveObjectManager->getServiceId ();
    PrismLoadOperationalDataForManagedObjectObjectManagerMessage  message                       (ownerServiceId, waveManagedObjectId, operationalDataFields);
    ResourceId                                                    status                      = WAVE_MESSAGE_SUCCESS;
    WaveLocalManagedObjectBase                                   *pWaveLocalManagedObjectBase = NULL;

    // Loading operational data should not be considered a configuration change.

    message.setIsConfigurationChange (false);
    message.setIsConfigurationTimeChange (false);

    pWaveLocalManagedObjectBase = dynamic_cast<WaveLocalManagedObjectBase *> (this);

    if (NULL != pWaveLocalManagedObjectBase)
    {
        status = sendSynchronously (&message, pWaveLocalManagedObjectBase->getOwnerLocationId ());
    }
    else
    {
        status = sendSynchronously (&message);
    }

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "WaveManagedObject::loadOperationalDataSynchronouslyLoadStep : Could not send message to load operational data : Status : " + FrameworkToolKit::localize (status));
    }
    else
    {
        status = message.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, "WaveManagedObject::loadOperationalDataSynchronouslyLoadStep : Could not successfully load operational data : Status : " + FrameworkToolKit::localize (status));
        }
    }

    return (status);
}

ResourceId WaveManagedObject::loadOperationalDataSynchronouslyRequeryStep (LoadOperationalDataSynchronousContext *pLoadOperationalDataSynchronousContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveManagedObject::loadOperationalDataSynchronouslyRequeryStep : Entering ...");

    ObjectId           waveManagedObjectId = pLoadOperationalDataSynchronousContext->getWaveManagedObjectId ();
    WaveManagedObject *pWaveManagedObject  = NULL;
    ResourceId         status              = WAVE_MESSAGE_SUCCESS;

    pWaveManagedObject = queryManagedObject (waveManagedObjectId);

    if (NULL == pWaveManagedObject)
    {
        status = WAVE_MANAGED_OBJECT_LOAD_OPERATIONAL_DATA_OBJECT_NOT_FOUND;
    }
    else
    {
        pLoadOperationalDataSynchronousContext->setPWaveManagedObject (pWaveManagedObject);
    }

    return (status);
}

ObjectId WaveManagedObject::getOwnerManagedObjectId () const
{
    return (m_ownerManagedObjectId);
}

void WaveManagedObject::setOwnerManagedObjectId (const ObjectId &ownerManagedObjectId)
{
    m_ownerManagedObjectId = ownerManagedObjectId;
}

ObjectId WaveManagedObject::getOwnerPartitionManagedObjectId () const
{
    return (m_ownerPartitionManagedObjectId);
}

ObjectId WaveManagedObject::getOwnerUserManagedObjectId () const
{
    return (m_ownerUserManagedObjectId);
}

void WaveManagedObject::setOwnerUserManagedObjectId (const ObjectId &ownerUserManagedObjectId)
{
    m_ownerUserManagedObjectId = ownerUserManagedObjectId;
}

void WaveManagedObject::setOwnerPartitionManagedObjectId (const ObjectId &ownerPartitionManagedObjectId)
{
    trace(TRACE_LEVEL_DEVEL, string("For classname [") + getObjectClassName () + string("], Setting Partition ManagedObjectId to [") + ownerPartitionManagedObjectId.toString() + string("]"));
    m_ownerPartitionManagedObjectId = ownerPartitionManagedObjectId;
}

void WaveManagedObject::loadFromPostgresQueryResult (PGresult *pResult, const UI32 &row, const string &schema, const vector<string> &selectFields, const bool loadOneToManyRelationships, const bool loadCompositions)
 {
    PrismPersistableObject::loadFromPostgresQueryResult (pResult, row, schema, selectFields, m_pCurrentOwnerWaveObjectManager, loadOneToManyRelationships, loadCompositions);
 }

void WaveManagedObject::loadFromPostgresAuxilliaryQueryResult (map<string, PGresult *> &auxilliaryResultsMap, const string &schema, const vector<string> &selectFields, const bool loadCompositions)
{
    PrismPersistableObject::loadFromPostgresAuxilliaryQueryResult (auxilliaryResultsMap, schema, selectFields, m_pCurrentOwnerWaveObjectManager, loadCompositions);
}

void WaveManagedObject::setPCurrentOwnerWaveObjectManager (WaveObjectManager *pCurrentOwnerWaveObjectManager)
{
    m_pCurrentOwnerWaveObjectManager = pCurrentOwnerWaveObjectManager;

    ObjectTracker::addToObjectTracker (this, true, m_pCurrentOwnerWaveObjectManager);
}

void WaveManagedObject::printfToWaveClientSession (const WaveClientSessionContext &waveClientSessionContext, const char * const pFormat, ...)
{
    va_list variableArguments;

    va_start (variableArguments, pFormat);

    m_pCurrentOwnerWaveObjectManager->printfToWaveClientSession (waveClientSessionContext, pFormat, variableArguments);

    va_end (variableArguments);
}

void WaveManagedObject::printfToAllWaveClientSessions (const WaveClientSessionContext &waveClientSessionContext, const char * const pFormat, ...)
{
    va_list variableArguments;

    va_start (variableArguments, pFormat);

    m_pCurrentOwnerWaveObjectManager->printfToAllWaveClientSessions (waveClientSessionContext, pFormat, variableArguments);

    va_end (variableArguments);
}

ResourceId WaveManagedObject::addLog (ResourceId logType, ResourceId logDescriptionType, const vector<Attribute *> &logDescriptionArguments)
{
    return (m_pCurrentOwnerWaveObjectManager->addLog (logType, logDescriptionType, logDescriptionArguments));
}

void WaveManagedObject::registerLock (const string &serviceString)
{
    m_pCurrentOwnerWaveObjectManager->registerLock (serviceString);
}

ResourceId WaveManagedObject::updateHardwareSynchronizationState (ResourceId hardwareSynchronizationState, const vector<LocationId> &locationIds)
{
    return (m_pCurrentOwnerWaveObjectManager->updateHardwareSynchronizationState (hardwareSynchronizationState, locationIds));
}

ResourceId WaveManagedObject::updateHardwareSynchronizationState (ResourceId hardwareSynchronizationState, LocationId locationId)
{
    return (m_pCurrentOwnerWaveObjectManager->updateHardwareSynchronizationState (hardwareSynchronizationState, locationId));
}

void WaveManagedObject::deleteAllManagedObjectInstances (const string &className)
{
    m_pCurrentOwnerWaveObjectManager->deleteAllManagedObjectInstances (className);
}

bool WaveManagedObject::canBeDeletedForOperation (const WaveManagedObjectOperation &operation)
{
    return (true);
}

bool WaveManagedObject::isHierarchyDeletableForOperation (const WaveManagedObjectOperation &operation)
{
    /* Check if this managed object can be deleted */
    if (true == canBeDeletedForOperation (operation))
    {
        /* Check if all the related objects are deletable */
        return (PrismPersistableObject::isHierarchyDeletableForOperation (operation));
    }

    return (false);
}

void WaveManagedObject::createPostUpdateForOperateOnWaveManagedObject (PrismAsynchronousContext *pPrismAsynchronousContext)
{
    pPrismAsynchronousContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pPrismAsynchronousContext->callback ();
}

void WaveManagedObject::createPostUpdateForInputWaveManagedObject (PrismAsynchronousContext *pPrismAsynchronousContext)
{
    pPrismAsynchronousContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pPrismAsynchronousContext->callback ();
}

void WaveManagedObject::preCreateHardwareStepForOperateOnWaveManagedObject (PrismAsynchronousContext *pPrismAsynchronousContext)
{
    WaveManagedObjectCreateContext *pWaveManagedObjectCreateContext =  reinterpret_cast<WaveManagedObjectCreateContext *> (pPrismAsynchronousContext->getPCallerContext ());
    ObjectId                        operateOnWaveManagedObjectId    = pWaveManagedObjectCreateContext->getOperateOnWaveManagedObjectId ();
    ObjectId                        newOperateOnWaveManagedObjectId;
    vector<LocationId>              locationIds;
    PrismServiceId                  prismServiceId;
    bool                            isNeedSurrogateSupportFlag = false;
    bool                            isPartialSuccessFlag = false;

    if (true == getPluginDetailsForDistribution (newOperateOnWaveManagedObjectId, prismServiceId, locationIds, isNeedSurrogateSupportFlag, isPartialSuccessFlag ))
    {
        trace(TRACE_LEVEL_DEVEL, "WaveManagedObject::preCreateHardwareStepForOperateOnWaveManagedObject entering .. ");

        WaveObjectManagerCreateWaveManagedObjectMessage *pMessage           = new WaveObjectManagerCreateWaveManagedObjectMessage (prismServiceId);
        vector<string>                                   attributeNames     = pWaveManagedObjectCreateContext->getAttributeNames    ();
        vector<string>                                   attributeValues    = pWaveManagedObjectCreateContext->getAttributeValues   ();

        if (ObjectId::NullObjectId == newOperateOnWaveManagedObjectId)
        {
            newOperateOnWaveManagedObjectId = operateOnWaveManagedObjectId;
        }

        pMessage->setAttributeNames                             (attributeNames);
        pMessage->setAttributeValues                            (attributeValues);
        pMessage->setOperateOnWaveManagedObjectId               (newOperateOnWaveManagedObjectId);
        pMessage->setNeedSurrogateSupportFlag                   (isNeedSurrogateSupportFlag);
        pMessage->setParentObjectId                             (pWaveManagedObjectCreateContext->getParentObjectId());
        //Set the ManagedClass name in the Message, so as to create a InMemory Mo in the Lccal Service.
        pMessage->setManagedObjectClassNameNeedToBeCreated      (pWaveManagedObjectCreateContext->getManagedObjectClassNameNeedToBeCreated ());
        pMessage->setConfigReplayInProgressFlag                 (pWaveManagedObjectCreateContext->getConfigReplayInProgressFlag ());

        sendToClusterLocation(pPrismAsynchronousContext, pMessage, isPartialSuccessFlag, locationIds);

    }
    else
    {
        pPrismAsynchronousContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        pPrismAsynchronousContext->callback ();
    }

}

void WaveManagedObject::preDeleteHardwareStepForOperateOnWaveManagedObject (PrismAsynchronousContext *pPrismAsynchronousContext)
{
    WaveManagedObjectDeleteContext *pWaveManagedObjectDeleteContext = reinterpret_cast<WaveManagedObjectDeleteContext *> (pPrismAsynchronousContext->getPCallerContext ());
    ObjectId                        operateOnWaveManagedObjectId    = pWaveManagedObjectDeleteContext->getOperateOnWaveManagedObjectId ();
    ObjectId                        newOperateOnWaveManagedObjectId;
    vector<LocationId>              locationIds;
    PrismServiceId                  prismServiceId;
    bool                            isNeedSurrogateSupportFlag = false;
    bool                            isPartialSuccessFlag = false;

    if (true == getPluginDetailsForDistribution (newOperateOnWaveManagedObjectId, prismServiceId, locationIds, isNeedSurrogateSupportFlag, isPartialSuccessFlag ))
    {
        trace(TRACE_LEVEL_DEVEL, "WaveManagedObject::preDeleteHardwareStepForOperateOnWaveManagedObject entering .. ");

        WaveObjectManagerDeleteWaveManagedObjectMessage *pMessage           = new WaveObjectManagerDeleteWaveManagedObjectMessage (prismServiceId);
        vector<string>                                   attributeNames     = pWaveManagedObjectDeleteContext->getAttributeNames    ();
        vector<string>                                   attributeValues    = pWaveManagedObjectDeleteContext->getAttributeValues   ();

        if (ObjectId::NullObjectId == newOperateOnWaveManagedObjectId)
        {
            newOperateOnWaveManagedObjectId = operateOnWaveManagedObjectId;
        }

        pMessage->setAttributeNames                 (attributeNames);
        pMessage->setAttributeValues                (attributeValues);
        pMessage->setOperateOnWaveManagedObjectId   (newOperateOnWaveManagedObjectId);
        pMessage->setNeedSurrogateSupportFlag       (isNeedSurrogateSupportFlag);
        pMessage->setIsMO                           (pWaveManagedObjectDeleteContext->isNeedToDeleteManagedObject());
        pMessage->setConfigReplayInProgressFlag     (pWaveManagedObjectDeleteContext->getConfigReplayInProgressFlag ());

        sendToClusterLocation(pPrismAsynchronousContext, pMessage, isPartialSuccessFlag, locationIds);
    }
    else
    {
        pPrismAsynchronousContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        pPrismAsynchronousContext->callback ();
    }
}
/* Use this function for Delete and Create. It is OK that it uses an UPdate message */
void WaveManagedObject::preUpdateHardwareStepForOperateOnWaveManagedObject (PrismAsynchronousContext *pPrismAsynchronousContext)
{
    WaveManagedObjectUpdateContext *pWaveManagedObjectUpdateContext = reinterpret_cast<WaveManagedObjectUpdateContext *> (pPrismAsynchronousContext->getPCallerContext ());
    ObjectId                        operateOnWaveManagedObjectId    = pWaveManagedObjectUpdateContext->getOperateOnWaveManagedObjectId ();
    ObjectId                        newOperateOnWaveManagedObjectId;
    vector<LocationId>              locationIds;
    PrismServiceId                  prismServiceId;
    bool                            isNeedSurrogateSupportFlag = false;
    bool                            isPartialSuccessFlag = false;

    /*
       Plugin developer overrides the getPluginDetailsForDistribution function.
       newOperateOnWaveManagedObjectId - set the objectId of the managed object, which the plugin developer wants to send to the local services.
       prismServiceId                  - set the serviceId of the service to which the message is need to send.
       locationIds                     - set locationId of the cluster nodes to which message is need to send, if not set, message will be send to all cluster nodes.
    */
    if (true == getPluginDetailsForDistribution (newOperateOnWaveManagedObjectId, prismServiceId, locationIds, isNeedSurrogateSupportFlag, isPartialSuccessFlag ))
    {
        trace(TRACE_LEVEL_DEVEL, "WaveManagedObject::preUpdateHardwareStepForOperateOnWaveManagedObject entering .. ");

        WaveObjectManagerUpdateWaveManagedObjectMessage *pMessage           = new WaveObjectManagerUpdateWaveManagedObjectMessage (prismServiceId);
        vector<string>                                   attributeNames     = pWaveManagedObjectUpdateContext->getAttributeNames    ();
        vector<string>                                   attributeValues    = pWaveManagedObjectUpdateContext->getAttributeValues   ();


        if (ObjectId::NullObjectId == newOperateOnWaveManagedObjectId)
        {
           /* if newOperateOnWaveManagedObjectId is not set by plugin developer, send the Global MO only. */
           newOperateOnWaveManagedObjectId = operateOnWaveManagedObjectId;
        }

        pMessage->setAttributeNames                 (attributeNames);
        pMessage->setAttributeValues                (attributeValues);
        pMessage->setOperateOnWaveManagedObjectId   (newOperateOnWaveManagedObjectId);
        pMessage->setNeedSurrogateSupportFlag       (isNeedSurrogateSupportFlag);
        pMessage->setConfigReplayInProgressFlag     (pWaveManagedObjectUpdateContext->getConfigReplayInProgressFlag ());

        sendToClusterLocation(pPrismAsynchronousContext, pMessage, isPartialSuccessFlag, locationIds);
    }
    else
    {
        pPrismAsynchronousContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        pPrismAsynchronousContext->callback ();
    }
}

void WaveManagedObject::sendToClusterLocation(PrismAsynchronousContext *pPrismAsynchronousContext, PrismMessage *pMessage, bool isPartialSuccessFlag, vector<LocationId> locationIds)
{
    WaveSendToClusterContext  *pWaveSendToClusterContext = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback>(&WaveManagedObject::getPluginDetailsForDistributionCallback), pPrismAsynchronousContext);

    pWaveSendToClusterContext->setPPrismMessageForPhase1 (pMessage);

    pWaveSendToClusterContext->setPartialSuccessFlag (isPartialSuccessFlag);

    if (0 == locationIds.size ())
    {
        if (true == (OrmRepository::isALocalManagedObject (getObjectClassName ())))
        {
            locationIds.push_back (FrameworkToolKit::getThisLocationId ());
            pWaveSendToClusterContext->setLocationsToSendToForPhase1 (locationIds);
        }
    }
    else
    {
        pWaveSendToClusterContext->setLocationsToSendToForPhase1 (locationIds);
    }

    sendToWaveCluster (pWaveSendToClusterContext);
}

void WaveManagedObject::preUpdateHardwareStepForInputWaveManagedObject (PrismAsynchronousContext *pPrismAsynchronousContext)
{
    pPrismAsynchronousContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pPrismAsynchronousContext->callback ();
}

void WaveManagedObject::getPluginDetailsForDistributionCallback (WaveSendToClusterContext *pWaveSendToClusterContext)
{
    ResourceId status = pWaveSendToClusterContext->getCompletionStatus ();

    PrismAsynchronousContext *pPrismAsynchronousContext = reinterpret_cast<PrismAsynchronousContext *>(pWaveSendToClusterContext->getPCallerContext());

    WaveManagedObjectUpdateContext *pWaveManagedObjectUpdateContext = reinterpret_cast<WaveManagedObjectUpdateContext *>(pPrismAsynchronousContext->getPCallerContext());

    WaveObjectManagerUpdateWaveManagedObjectMessage *pWaveObjectManagerUpdateWaveManagedObjectMessage = reinterpret_cast<WaveObjectManagerUpdateWaveManagedObjectMessage *>(pWaveManagedObjectUpdateContext->getPPrismMessage());

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace(TRACE_LEVEL_DEVEL,"WaveManagedObject::getPluginDetailsForDistributionCallback : Send To Cluster Success");

        WaveObjectManagerUpdateWaveManagedObjectMessage *pMessage = reinterpret_cast<WaveObjectManagerUpdateWaveManagedObjectMessage *> (pWaveSendToClusterContext->getResultingMessageForPhase1(FrameworkToolKit::getThisLocationId ()));

        pWaveObjectManagerUpdateWaveManagedObjectMessage->setWarningResourceId (pMessage->getWarningResourceId());
    }
    else
    {
        trace(TRACE_LEVEL_ERROR,"WaveManagedObject::getPluginDetailsForDistributionCallback : Send To Cluster failed");

        pWaveObjectManagerUpdateWaveManagedObjectMessage->setIsPropagateErrorCodeNeeded (pWaveSendToClusterContext->getPartialSuccessFlag());
    }

    delete (pWaveSendToClusterContext->getPPrismMessageForPhase1 ());
    delete (pWaveSendToClusterContext);

    pPrismAsynchronousContext->setCompletionStatus (status);
    pPrismAsynchronousContext->callback ();
}

bool WaveManagedObject::getPluginDetailsForDistribution (ObjectId &newOperateOnWaveManagedObjectId, PrismServiceId &prismServiceId, vector<LocationId> &locationIds, bool &isNeedSurrogateSupportFlag, bool &isPartialSuccessFlag)
{
    return (false);
}

void WaveManagedObject::getHardwareConfigurationDetailsForCreate (GetHardwareConfigurationDetailsForCreateContext *pContext)
{
    pContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pContext->callback ();
}

void WaveManagedObject::getHardwareConfigurationDetailsForUpdate (GetHardwareConfigurationDetailsForUpdateContext *pContext)
{
    pContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pContext->callback ();
}

void WaveManagedObject::getHardwareConfigurationDetailsForDelete (GetHardwareConfigurationDetailsForDeleteContext *pContext)
{
    pContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pContext->callback ();
}

void WaveManagedObject::getHardwareConfigurationDetailsForPostboot (GetHardwareConfigurationDetailsForPostbootContext *pContext)
{
    pContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pContext->callback ();
}

DateTime WaveManagedObject::getCreatedTimeStamp () const
{
    return (m_createdTimeStamp);
}

void WaveManagedObject::setCreatedTimeStamp (const DateTime &createdTimeStamp)
{
    m_createdTimeStamp = createdTimeStamp;
}

DateTime WaveManagedObject::getLastModifiedTimeStamp () const
{
    return (m_lastModifiedTimeStamp);
}

void WaveManagedObject::setLastModifiedTimeStamp (const DateTime &lastModifiedTimeStamp)
{
    m_lastModifiedTimeStamp = lastModifiedTimeStamp;
}

void WaveManagedObject::setAttributeDefaultValue (string AttributeName)
{
   return;
}

void WaveManagedObject::setAttributeDefaultValue (UI32 AttributeUserTag)
{
   return;
}

void WaveManagedObject::updateTimeConsumedInThisThread (const UI32 &operationCode, const UI32 &sequencerStepIndex, const UI32 &numberOfSeconds, const SI32 &numberOfNanoSeconds)
{
    m_pCurrentOwnerWaveObjectManager->updateTimeConsumedInThisThread (operationCode, sequencerStepIndex, numberOfSeconds, numberOfNanoSeconds);
}

void WaveManagedObject::updateRealTimeConsumedInThisThread (const UI32 &operationCode, const UI32 &sequencerStepIndex, const UI32 &numberOfSeconds, const SI32 &numberOfNanoSeconds)
{
    m_pCurrentOwnerWaveObjectManager->updateRealTimeConsumedInThisThread (operationCode, sequencerStepIndex, numberOfSeconds, numberOfNanoSeconds);
}


void WaveManagedObject::validateForUpdateAtThisNode (ValidateForUpdateAtThisNodeContext *pContext)
{
    pContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pContext->callback ();
}

void WaveManagedObject::validateGlobalForUpdateAtThisNode (ValidateForUpdateAtThisNodeContext *pContext)
{
    pContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pContext->callback ();
}

void WaveManagedObject::validateForCreateAtThisNode (ValidateForCreateAtThisNodeContext *pContext)
{
    pContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pContext->callback ();
}

void WaveManagedObject::validateGlobalForCreateAtThisNode (ValidateForCreateAtThisNodeContext *pContext)
{
    pContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pContext->callback ();
}

void WaveManagedObject::validateForDeleteAtThisNode (ValidateForDeleteAtThisNodeContext *pContext)
{
    pContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pContext->callback ();
}

void WaveManagedObject::validateGlobalForDeleteAtThisNode (ValidateForDeleteAtThisNodeContext *pContext)
{
    pContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pContext->callback ();
}

void WaveManagedObject::isAssociatedManagedObjectToBeCreated (IsAssociatedManagedObjectToBeCreatedContext *pContext)
{
    pContext->setMessage(NULL);
    pContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pContext->setIsAssociatedMOToBeCreated (false);
    pContext->callback ();
}

void WaveManagedObject::isManagedObjectsAssociatedToCurrentMONeedsToBeDeleted(WaveManagedObjectDeleteContext *pContext)
{
    pContext->setMessage(NULL);
}

void WaveManagedObject::createWaveManagedObject ()
{
    if ((PrismThread::getSelf ()) != (PrismThread::getPrismThreadForServiceId (DatabaseObjectManagerExecuteQueryMessage::getDatabaseObjectManagerServiceId ()))->getId ())
    {
        m_pCurrentOwnerWaveObjectManager->trackObjectCreatedDuringCurrentTransaction (this);
    }
}

bool WaveManagedObject::updateHardwareBeforeWaveManagedObject (const vector<Attribute *> &attributeVector)
{
    return (true);
}

void WaveManagedObject::addNewAttributeToBeUpdatedForCreate (UpdateNewAttributeInManagedObjectContext *pContext)
{
    pContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pContext->callback ();
}

void WaveManagedObject::addNewAttributeToBeUpdatedForUpdate (UpdateNewAttributeInManagedObjectContext *pContext)
{
    pContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pContext->callback ();
}

void WaveManagedObject::addNewAttributeToBeUpdatedForDelete (UpdateNewAttributeInManagedObjectContext *pContext)
{
    pContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pContext->callback ();
}

UI32 WaveManagedObject::getCase (const UI32 &attributeUserTag)
{
    AttributeUI32 *pAttribute = dynamic_cast<AttributeUI32 *> (getAttributeByUserTag (attributeUserTag));

    prismAssert (NULL != pAttribute, __FILE__, __LINE__);

    UI32 replyUserTag         = pAttribute->getValue ();

    return (replyUserTag);

}

UI32 WaveManagedObject::getManagedObjectUserTag () const
{
    return (0);
}

bool WaveManagedObject::getIsPartitionNameSetByUser (void)
{
    return m_isPartitionNameSetByUser;
}

void WaveManagedObject::setUserSpecifiedPartitionName (const string &partitionName)
{
  m_isPartitionNameSetByUser = true;
  m_userSpecifiedPartitionName = partitionName;
  // add this partition name in the set of partitionnames currently referenced in the associated OM.
  m_pCurrentOwnerWaveObjectManager->addPartitionNameToSetOfPartitionNamesReferencedInCurrentTransaction (partitionName);
}

string WaveManagedObject::getUserSpecifiedPartitionName (void)
{
    return m_userSpecifiedPartitionName;
}

void WaveManagedObject::sendMulticast (WaveSendMulticastContext *pWaveSendMulticastContext)
{
    m_pCurrentOwnerWaveObjectManager->sendMulticast (pWaveSendMulticastContext);
}

void WaveManagedObject::loadFromPostgresQueryResult2 (PGresult *pResult, const UI32 &row, const string &schema, const vector<string> &selectFields)
{
    PrismPersistableObject::loadFromPostgresQueryResult2 (pResult, row, schema, selectFields, m_pCurrentOwnerWaveObjectManager);
}

void WaveManagedObject::getOidsOfOneToOneCompositions (vector<ObjectId> &vectorOfCompositionOids)
{
    PrismPersistableObject::getOidsOfOneToOneCompositions (vectorOfCompositionOids);
}

void WaveManagedObject::popOneToOneCompositionsFromResults (map<ObjectId, WaveManagedObject*> &oidTopManagedObjectMap, const vector<string> &selectFieldsInManagedObject)
{
    PrismPersistableObject::popOneToOneCompositionsFromResults (oidTopManagedObjectMap, selectFieldsInManagedObject);
}

void WaveManagedObject::storeRelatedObjectIdVectorForAOneToNAssociation (const string &relationName, const ObjectId &parentObjectId, const vector<ObjectId> &vectorOfRelatedObjectIds)
{
    PrismPersistableObject::storeRelatedObjectIdVectorForAOneToNAssociation (relationName, parentObjectId, vectorOfRelatedObjectIds);
}

void WaveManagedObject::storeRelatedObjectVectorForAOneToNComposition (const string &relationName, const ObjectId &parentObjectId, const vector<WaveManagedObject *> &vectorOfRelatedObjects)
{
    PrismPersistableObject::storeRelatedObjectVectorForAOneToNComposition (relationName, parentObjectId, vectorOfRelatedObjects);
}

string WaveManagedObject::customConfigurationDisplay (WaveCustomCliDisplayConfigurationContext *waveCustomCliDisplayConfigurationContext)
{
    // the call should not come here
    trace (TRACE_LEVEL_FATAL, "WaveManagedObject::customConfigurationDisplay : The Derived Managed Objects must overwrite this implementation to support runtemplate.");

    prismAssert (false, __FILE__, __LINE__);

    return string("");
}

void WaveManagedObject::postponeMessageHandling (PrismMessage *pPrismMessage)
{
    m_pCurrentOwnerWaveObjectManager->postponeMessageHandling (pPrismMessage);
}

void WaveManagedObject::resumeAllPostponedMessages ()
{
    m_pCurrentOwnerWaveObjectManager->resumeAllPostponedMessages ();
}

ResourceId WaveManagedObject::blockCli (const CliBlockContext &cliBlockContext, const bool &clusterWide)
{
    ResourceId status = m_pCurrentOwnerWaveObjectManager->blockCli (cliBlockContext, clusterWide);
    return status;
}

ResourceId WaveManagedObject::unblockCli (const CliBlockContext &cliBlockContext, const bool &clusterWide)
{
    ResourceId status = m_pCurrentOwnerWaveObjectManager->unblockCli (cliBlockContext, clusterWide);
    return status;
}

WaveMessageBrokerStatus WaveManagedObject::connectToMessageBroker (const string &brokerName, const string &brokerIpAddress, const SI32 &brokerPort)
{
    return (m_pCurrentOwnerWaveObjectManager->connectToMessageBroker (brokerName, brokerIpAddress, brokerPort));
}

WaveMessageBrokerStatus WaveManagedObject::subscribeToMessageBroker (const string &brokerName, const vector<string> &topicNames, const vector<WaveBrokerPublishMessageHandler> publishMessageHandlers, PrismElement *pSubscriber)
{
    return (m_pCurrentOwnerWaveObjectManager->subscribeToMessageBroker (brokerName, topicNames, publishMessageHandlers, pSubscriber != NULL ? pSubscriber : this));
}

WaveMessageBrokerStatus WaveManagedObject::subscribeToMessageBroker (const string &brokerName, const vector<string> &topicNames, WaveBrokerPublishMessageHandler publishMessageHandler, PrismElement *pSubscriber)
{
    return (m_pCurrentOwnerWaveObjectManager->subscribeToMessageBroker (brokerName, topicNames, publishMessageHandler, pSubscriber != NULL ? pSubscriber : this));
}

WaveMessageBrokerStatus WaveManagedObject::subscribeToMessageBroker (const string &brokerName, const string &topicName, WaveBrokerPublishMessageHandler publishMessageHandler, PrismElement *pSubscriber)
{
    return (m_pCurrentOwnerWaveObjectManager->subscribeToMessageBroker (brokerName, brokerName, publishMessageHandler, pSubscriber != NULL ? pSubscriber : this));
}

WaveMessageBrokerStatus WaveManagedObject::unsubscribeToMessageBroker (const string &brokerName, const vector<string> &topicNames, PrismElement *pSubscriber)
{
    return (m_pCurrentOwnerWaveObjectManager->unsubscribeToMessageBroker (brokerName, topicNames, pSubscriber != NULL ? pSubscriber : this));
}

WaveMessageBrokerStatus WaveManagedObject::unsubscribeToMessageBroker (const string &brokerName, const string &topicName, PrismElement *pSubscriber)
{
    return (m_pCurrentOwnerWaveObjectManager->unsubscribeToMessageBroker (brokerName, topicName, pSubscriber != NULL ? pSubscriber : this));
}

WaveMessageBrokerStatus WaveManagedObject::publishToMessageBroker (const string &brokerName, WaveBrokerPublishMessage *pWaveBrokerPublishMessage, PrismElement *pSubscriber)
{
    return (m_pCurrentOwnerWaveObjectManager->publishToMessageBroker (brokerName, pWaveBrokerPublishMessage));
}

}
