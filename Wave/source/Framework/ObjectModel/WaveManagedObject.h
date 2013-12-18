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

#ifndef WAVEMANAGEDOBJECT_H
#define WAVEMANAGEDOBJECT_H

#include "Framework/ObjectModel/PrismElement.h"
#include "Framework/ObjectModel/WaveObjectManager.h"
#include "Framework/ObjectModel/PrismPersistableObject.h"
#include "Framework/Types/DateTime.h"
#include "Framework/ObjectModel/WaveManagedObjectAsynchronousPluginContext.h"
#include "Framework/ObjectModel/WaveManagedObjectDeleteContext.h"

#include <string>

namespace WaveNs
{

class WaveManagedObjectQueryContext;
class WaveManagedObjectSynchronousQueryContextForDeletion;
class WaveManagedObjectSynchronousQueryContextForUpdate;
class WaveManagedObjectLoadOperationalDataContext;
class LoadOperationalDataSynchronousContext;
class WaveManagedObjectOperation;
class WaveSendToClusterContext;
class WaveManagedObjectDeleteContext;
class WaveSendMulticastContext;
class WaveCustomCliDisplayConfigurationContext;
class WaveObjectManagerCommitTransactionContext;
class CliBlockContext;

class WaveManagedObject : virtual public PrismElement, virtual public PrismPersistableObject
{
    private :
                void       setPCurrentOwnerWaveObjectManager           (WaveObjectManager *pCurrentOwnerWaveObjectManager);

                ResourceId loadOperationalDataSynchronouslyLoadStep    (LoadOperationalDataSynchronousContext *pLoadOperationalDataSynchronousContext);
                ResourceId loadOperationalDataSynchronouslyRequeryStep (LoadOperationalDataSynchronousContext *pLoadOperationalDataSynchronousContext);

        virtual void       updateTimeConsumedInThisThread              (const UI32 &operationCode, const UI32 &sequencerStepIndex, const UI32 &numberOfSeconds, const SI32 &numberOfNanoSeconds);
        virtual void       updateRealTimeConsumedInThisThread          (const UI32 &operationCode, const UI32 &sequencerStepIndex, const UI32 &numberOfSeconds, const SI32 &numberOfNanoSeconds);

    protected :
        virtual WaveMessageStatus             send                                      (PrismMessage *pPrismMessage, PrismMessageResponseHandler pPrismMessageCallback, void *pPrismMessageContext, UI32 timeOutInMilliSeconds = 0, LocationId locationId = 0, PrismElement *pPrismMessageSender = NULL);
        virtual WaveMessageStatus             sendOneWay                                (PrismMessage *pPrismMessage, const LocationId &locationId = 0);
        virtual WaveMessageStatus             sendOneWayToFront                         (PrismMessage *pPrismMessage, const LocationId &locationId = 0);
        virtual WaveMessageStatus             sendSynchronously                         (PrismMessage *pPrismMessage, const LocationId &locationId = 0);
        virtual void                          sendToWaveCluster                         (WaveSendToClusterContext *pWaveSendToClusterContext);
        virtual void                          sendMulticast                             (WaveSendMulticastContext *pWaveSendMulticastContext);
        virtual void                          sendOneWayToWaveCluster                   (WaveSendToClusterContext *pWaveSendToClusterContext);
        virtual WaveMessageStatus             recall                                    (PrismMessage *pPrismMessage);
        virtual WaveMessageStatus             reply                                     (PrismMessage *pPrismMessage);
        virtual WaveMessageStatus             broadcast                                 (PrismEvent *pPrismEvent);
        virtual void                          trace                                     (TraceLevel traceLevel, const string &stringToTrace);
        virtual void                          tracePrintf                               (TraceLevel traceLevel, const bool &addNewLine, const bool &suppressPrefix, const char * const pFormat, ...);
        virtual void                          tracePrintf                               (TraceLevel traceLevel, const char * const pFormat, ...);
        virtual void                          prismAssert                               (bool isAssertNotRequired, const char *pFileName, UI32 lineNumber);
                void                          addOperationMap                           (UI32 operationCode, PrismMessageHandler pPrismMessageHandler, PrismElement *pPrismElement = NULL);
        virtual ResourceId                    startTimer                                (TimerHandle &timerHandle, timeval &startInterval, timeval &periodicInterval, PrismTimerExpirationHandler pPrismTimerExpirationCallback, void *pPrismTimerExpirationContext = NULL, PrismElement *pPrismTimerSender = NULL);
        virtual ResourceId                    startTimer                                (TimerHandle &timerHandle, UI32 timeInMilliSeconds, PrismTimerExpirationHandler pPrismTimerExpirationCallback, void *pPrismTimerExpirationContext = NULL, PrismElement *pPrismTimerSender = NULL);
        virtual ResourceId                    deleteTimer                               (TimerHandle timerHandle);

        virtual void                          holdMessages                              ();
        virtual void                          holdHighPriorityMessages                  ();
        virtual void                          holdEvents                                ();
        virtual void                          holdAll                                   ();
        virtual void                          unholdMessages                            ();
        virtual void                          unholdHighPriorityMessages                ();
        virtual void                          unholdEvents                              ();
        virtual void                          unholdAll                                 ();

        virtual UI32                          getNumberOfPendingTimerExpirationMessages ();
        virtual UI32                          getNumberOfPendingNormalMessages          ();
        virtual UI32                          getNumberOfPendingHighPriorityMessages    ();

        virtual void                          setupAttributesForPersistence             ();
        virtual void                          setupAttributesForCreate                  ();

        virtual void                          startTransaction                          ();
        virtual bool                          isTransactionInProgress                   ();
        virtual ResourceId                    commitTransaction                         (WaveObjectManagerCommitTransactionContext *pWaveObjectManagerCommitTransactionContext = NULL);
        virtual void                          rollbackTransaction                       ();
        virtual void                          updateWaveManagedObject                   (WaveManagedObject *pWaveManagedObject);
        virtual void                          deleteWaveManagedObject                   (const ObjectId &objectId);
        virtual void                          deleteWaveManagedObject                   (WaveManagedObjectSynchronousQueryContextForDeletion *pWaveManagedObjectSynchronousQueryContextForDeletion);
        virtual void                          updateMultipleWaveManagedObjects          (WaveManagedObjectSynchronousQueryContextForUpdate *pWaveManagedObjectSynchronousQueryContextForUpdate);
        virtual void                          deleteMultipleWaveManagedObjects          (WaveManagedObjectSynchronousQueryContextForDeletion *pWaveManagedObjectSynchronousQueryContextForDeletion);
        virtual void                          deleteManagedObjectCompositions           (WaveManagedObjectSynchronousQueryContextForDeletion *pWaveManagedObjectSynchronousQueryContextForDeletion, const set<string> relationsSet = set<string>());
        virtual void                          deleteManagedObjectCompositions           (const ObjectId &objectId, const set<string> relationsSet = set<string>());
        virtual bool                          isManagedClassSupported                   (const string &managedClass);
        virtual void                          addRelationship                           (const string &parentClassName, const string &childClassName, const string &relationshipName, const ObjectId &parentObjectId, const ObjectId &childObjectId);
        virtual void                          deleteRelationship                        (const string &parentClassName, const string &childClassName, const string &relationshipName, const ObjectId &parentObjectId, const ObjectId &childObjectId);
        virtual void                          addToComposition                          (const string &parentClassName, const string &childCalssName, const string &compositionName, const ObjectId &parentObjectId, const ObjectId &childObjectId);
        virtual void                          deleteFromComposition                     (const string &parentClassName, const string &childCalssName, const string &compositionName, const ObjectId &parentObjectId, const ObjectId &childObjectId);
        virtual void                          addToAssociation                          (const string &parentClassName, const string &childCalssName, const string &associationName, const ObjectId &parentObjectId, const ObjectId &childObjectId);
        virtual void                          deleteFromAssociation                     (const string &parentClassName, const string &childCalssName, const string &associationName, const ObjectId &parentObjectId, const ObjectId &childObjectId);

        virtual vector<WaveManagedObject *>  *querySynchronouslyLocalManagedObjectsForLocationId (const LocationId &locationId, const string &className);
        virtual vector<WaveManagedObject *>  *querySynchronouslyLocalManagedObjectsForLocationId (const LocationId &locationId, const string &className, const vector<string> &selectFields);
        virtual vector<WaveManagedObject *>  *querySynchronouslyLocalManagedObjectsForLocationId (const LocationId &locationId, WaveManagedObjectSynchronousQueryContext *pWaveManagedObjectSynchronousQueryContext);
        virtual void                          query                                     (WaveManagedObjectQueryContext *pWaveManagedObjectQueryContext);
        virtual vector<WaveManagedObject *>  *querySynchronously                        (WaveManagedObjectSynchronousQueryContext *pWaveManagedObjectSynchronousQueryContext);
        virtual ResourceId                    querySynchronouslyForCount                (WaveManagedObjectSynchronousQueryContext *pWaveManagedObjectSynchronousQueryContext, UI32 &count);
        virtual ResourceId                    querySynchronouslyForCount                (const string &managedClassName, UI32 &count, const string &schema = OrmRepository::getWaveCurrentSchema ());
        virtual ResourceId                    querySynchronouslyForCount                (const string &managedClassName, const string &fieldName, const string &range, UI32 &count, const string &schema = OrmRepository::getWaveCurrentSchema ());
        virtual vector<WaveManagedObject *>  *querySynchronously                        (const string &managedClassName, const string &schema = OrmRepository::getWaveCurrentSchema ());
        virtual vector<WaveManagedObject *>  *querySynchronously                        (const string &managedClassName, vector<ObjectId> &objectIds, const string &schema = OrmRepository::getWaveCurrentSchema ());
        virtual vector<WaveManagedObject *>  *querySynchronouslyByName                  (const string &managedClassName, const string &managedObjectName, const string &schema = OrmRepository::getWaveCurrentSchema ());
        virtual WaveManagedObject            *queryManagedObject                        (const ObjectId &managedObjectId, const string &schema = OrmRepository::getWaveCurrentSchema ());
        virtual vector<WaveManagedObject *> *queryManagedObjectAssociatedWithSlot       (const string &managedClassName, UI32 slotNumber, LocationId locationId = 0, const string &schema = OrmRepository::getWaveCurrentSchema ());

        virtual TraceClientId                 getTraceClientId                          ();

        virtual void                          addEventType                              (const UI32 &eventOperationCode);

        virtual void                          listenForEvent                            (PrismServiceId prismServiceId, UI32 sourceOperationCode, PrismEventHandler pPrismEventHandler, PrismElement *pPrismElement = NULL, const LocationId &sourceLocationId = 0);

        virtual ResourceId                    sendSynchronouslyToWaveClient             (const string &waveClientName, ManagementInterfaceMessage *pManagementInterfaceMessage, const SI32 &Instnace = 0);
        virtual WaveMessageStatus             sendToWaveServer                          (const UI32 &waveServerId, ManagementInterfaceMessage *pManagementInterfaceMessage, PrismMessageResponseHandler messageCallback, PrismElement *pPrismMessageSender, void *pInputContext, UI32 timeOutInMilliSeconds);
        virtual ResourceId                    sendToWaveClient                          (const string &waveClientName, ManagementInterfaceMessage *pManagementInterfaceMessage, PrismMessageResponseHandler pPrismMessageCallback, void *pPrismMessageContext = NULL, UI32 timeOutInMilliSeconds = 0, const SI32 &Instnace = 0);
        virtual void                          sendToWaveClients                         (WaveSendToClientsContext *pWaveSendToClientsContext);
        virtual ResourceId                    sendOneWayToAllWaveClients                (ManagementInterfaceMessage *pManagementInterfaceMessage);
        virtual WaveManagedObject            *createManagedObjectInstance               (const string &managedClassName);
        virtual PrismMessage                 *createMessageInstance                     (const UI32 &operationCode);

        virtual void                          loadOperationalData                       (WaveManagedObjectLoadOperationalDataContext *pWaveManagedObjectLoadOperationalDataContext);

        virtual void                          loadFromPostgresQueryResult               (PGresult *pResult, const UI32 &row, const string &schema, const vector<string> &selectFields, const bool loadOneToManyRelationships = true, const bool loadCompositions = true);
        virtual void                          loadFromPostgresAuxilliaryQueryResult     (map<string, PGresult *> &auxilliaryResultsMap, const string &schema, const vector<string> &selectFields, const bool loadCompositions = true);
        virtual void                          loadFromPostgresQueryResult2              (PGresult *pResult, const UI32 &row, const string &schema, const vector<string> &selectFields);

        virtual void                          printfToWaveClientSession                 (const WaveClientSessionContext &waveClientSessionContext, const char * const pFormat, ...);
        virtual void                          printfToAllWaveClientSessions             (const WaveClientSessionContext &waveClientSessionContext, const char * const pFormat, ...);
        virtual ResourceId                    addLog                                    (ResourceId logType, ResourceId logDescriptionType, const vector<Attribute *> &logDescriptionArguments);
        virtual bool                          isBeingSurrogated                         ();

        virtual ResourceId                    updateHardwareSynchronizationState        (ResourceId hardwareSynchronizationState, const vector<LocationId> &locationIds);
        virtual ResourceId                    updateHardwareSynchronizationState        (ResourceId hardwareSynchronizationState, LocationId locationId = 0);

        virtual void                          postponeMessageHandling                   (PrismMessage *pPrismMessage);
        virtual void                          resumeAllPostponedMessages                ();

        virtual ResourceId                    blockCli                                  (const CliBlockContext &cliBlockContext, const bool &clusterWide);
        virtual ResourceId                    unblockCli                                (const CliBlockContext &cliBlockContext, const bool &clusterWide);


        /**
         * This API has few limitations.
         * 1. The MO/class 'className' should not be part of any c++ inheritance hierarchy.
         * 2. The table/MO should have a relation pointing to it or the MO should have 1-m relation point from it.
         *    This means the MO should have its own DerivationsInstances table.
         * So, Use deleteMultipleWaveManagedObjects API to avoid these limitations.
         */
        virtual void                          deleteAllManagedObjectInstances           (const string &className);

        virtual bool                          canBeDeletedForOperation                  (const WaveManagedObjectOperation &operation);

        virtual WaveMessageBrokerStatus       connectToMessageBroker                    (const string &brokerName, const string &brokerIpAddress, const SI32 &brokerPort);
        virtual WaveMessageBrokerStatus       subscribeToMessageBroker                  (const string &brokerName, const vector<string> &topicNames, const vector<WaveBrokerPublishMessageHandler> publishMessageHandlers, PrismElement *pSubscriber = NULL);
        virtual WaveMessageBrokerStatus       subscribeToMessageBroker                  (const string &brokerName, const vector<string> &topicNames, WaveBrokerPublishMessageHandler publishMessageHandler, PrismElement *pSubscriber = NULL);
        virtual WaveMessageBrokerStatus       subscribeToMessageBroker                  (const string &brokerName, const string &topicName, WaveBrokerPublishMessageHandler publishMessageHandler, PrismElement *pSubscriber = NULL);
        virtual WaveMessageBrokerStatus       unsubscribeToMessageBroker                (const string &brokerName, const vector<string> &topicNames, PrismElement *pSubscriber = NULL);
        virtual WaveMessageBrokerStatus       unsubscribeToMessageBroker                (const string &brokerName, const string &topicName, PrismElement *pSubscriber = NULL);
        virtual WaveMessageBrokerStatus       publishToMessageBroker                    (const string &brokerName, WaveBrokerPublishMessage *pWaveBrokerPublishMessage, PrismElement *pSubscriber = NULL);

    public :
                                              WaveManagedObject                         (WaveObjectManager *pWaveObjectManager);
        virtual                              ~WaveManagedObject                         ();
                string                        getName                                   () const;
                void                          setName                                   (const string &name);
                ResourceId                    getGenericStatus                          () const;
                void                          setGenericStatus                          (ResourceId genericStatus);
                ResourceId                    getSpecificStatus                         () const;
                void                          setSpecificStatus                         (ResourceId specificStatus);

        static  string                        getClassName                              ();

                ResourceId                    loadOperationalDataSynchronousWrapper     (const vector<string> &operationalDataFields, WaveManagedObject ** const pWaveManagedObject);

                ObjectId                      getOwnerManagedObjectId                   () const;
                void                          setOwnerManagedObjectId                       (const ObjectId &ownerManagedObjectId);

                ObjectId                      getOwnerPartitionManagedObjectId                 () const;
                void                          setOwnerPartitionManagedObjectId                 (const ObjectId &ownerManagedObjectId);

                ObjectId                      getOwnerUserManagedObjectId                           () const;
                void                          setOwnerUserManagedObjectId                           (const ObjectId &ownerUserManagedObjectId);

                void                          registerLock                                  (const string &serviceString);

                bool                          isHierarchyDeletableForOperation              (const WaveManagedObjectOperation &operation);

        virtual void                          createPostUpdateForOperateOnWaveManagedObject (PrismAsynchronousContext *pPrismAsynchronousContext);
        virtual void                          createPostUpdateForInputWaveManagedObject     (PrismAsynchronousContext *pPrismAsynchronousContext);
        virtual void                          preUpdateHardwareStepForInputWaveManagedObject    (PrismAsynchronousContext *pPrismAsynchronousContext);
        virtual void                          preUpdateHardwareStepForOperateOnWaveManagedObject (PrismAsynchronousContext *pPrismAsynchronousContext);
        virtual void                          preCreateHardwareStepForOperateOnWaveManagedObject (PrismAsynchronousContext *pPrismAsynchronousContext);
        virtual void                          preDeleteHardwareStepForOperateOnWaveManagedObject (PrismAsynchronousContext *pPrismAsynchronousContext);
                void                          sendToClusterLocation                              (PrismAsynchronousContext *pPrismAsynchronousContext, PrismMessage *pMessage, bool isPartialSuccessFlag, vector<LocationId> locationIds);
        virtual bool                          getPluginDetailsForDistribution                    (ObjectId &newOperateOnWaveManagedObjectId, PrismServiceId &prismServiceId, vector<LocationId> &locationIds, bool &isNeedSurrogateSupportFlag, bool &isPartialSuccessFlag);
                void                          getPluginDetailsForDistributionCallback            (WaveSendToClusterContext *pWaveSendToClusterContext);

        virtual void                          getHardwareConfigurationDetailsForUpdate      (GetHardwareConfigurationDetailsForUpdateContext *pContext);
        virtual void                          getHardwareConfigurationDetailsForCreate      (GetHardwareConfigurationDetailsForCreateContext *pContext);
        virtual void                          getHardwareConfigurationDetailsForDelete      (GetHardwareConfigurationDetailsForDeleteContext *pContext);
        virtual void                          getHardwareConfigurationDetailsForPostboot    (GetHardwareConfigurationDetailsForPostbootContext *pContext);

                DateTime                      getCreatedTimeStamp                           () const;
                void                          setCreatedTimeStamp                           (const DateTime &createdTimeStamp);
                DateTime                      getLastModifiedTimeStamp                      () const;
                void                          setLastModifiedTimeStamp                      (const DateTime &lastModifiedTimeStamp);

        virtual void                          setAttributeDefaultValue                      (const string AttributeName);
        virtual void                          setAttributeDefaultValue                      (const UI32 AttributeUserTag);
        virtual void                          validateForUpdateAtThisNode                   (ValidateForUpdateAtThisNodeContext *pContext);
        virtual void                          validateGlobalForUpdateAtThisNode             (ValidateForUpdateAtThisNodeContext *pContext);
        virtual void                          validateForDeleteAtThisNode                   (ValidateForCreateAtThisNodeContext *pContext);
        virtual void                          validateGlobalForDeleteAtThisNode             (ValidateForCreateAtThisNodeContext *pContext);
        virtual void                          validateForCreateAtThisNode                   (ValidateForDeleteAtThisNodeContext *pContext);
        virtual void                          validateGlobalForCreateAtThisNode             (ValidateForCreateAtThisNodeContext *pContext);
        virtual void                          isAssociatedManagedObjectToBeCreated          (IsAssociatedManagedObjectToBeCreatedContext *pContext);
        virtual void                          isManagedObjectsAssociatedToCurrentMONeedsToBeDeleted      (WaveManagedObjectDeleteContext *pContext);
                void                          createWaveManagedObject                       ();
        virtual bool                          updateHardwareBeforeWaveManagedObject         (const vector<Attribute *> &attributeVector);
        virtual void                          addNewAttributeToBeUpdatedForCreate           (UpdateNewAttributeInManagedObjectContext *pContext);
        virtual void                          addNewAttributeToBeUpdatedForUpdate           (UpdateNewAttributeInManagedObjectContext *pContext);
        virtual void                          addNewAttributeToBeUpdatedForDelete           (UpdateNewAttributeInManagedObjectContext *pContext);
        virtual UI32                          getCase                                       (const UI32 &attributeUserTag);
        virtual UI32                          getManagedObjectUserTag                       () const;
        virtual string                        customConfigurationDisplay                    (WaveCustomCliDisplayConfigurationContext *waveCustomCliDisplayConfigurationContext);


                bool                          getIsPartitionNameSetByUser                       (void);
                void                          setUserSpecifiedPartitionName                     (const string &partitionName);
                string                        getUserSpecifiedPartitionName                     (void);

                void                          getOidsOfOneToOneCompositions                     (vector<ObjectId> &vectorOfCompositionOids);
                void                          popOneToOneCompositionsFromResults                (map<ObjectId, WaveManagedObject*> &oidTopManagedObjectMap, const vector<string> &selectFieldsInManagedObject);
                void                          storeRelatedObjectIdVectorForAOneToNAssociation   (const string &relationName, const ObjectId &parentObjectId, const vector<ObjectId> &vectorOfRelatedObjectIds);
                void                          storeRelatedObjectVectorForAOneToNComposition     (const string &relationName, const ObjectId &parentObjectId, const vector<WaveManagedObject *> &vectorOfRelatedObjects);

    // Now the data members

    private :
                string                        m_name;
                ResourceId                    m_genericStatus;
                ResourceId                    m_specificStatus;
                ObjectId                      m_ownerManagedObjectId;
                DateTime                      m_createdTimeStamp;
                DateTime                      m_lastModifiedTimeStamp;

                WaveObjectManager            *m_pCurrentOwnerWaveObjectManager;

                // Multi Partition
                // Persisted
                ObjectId                      m_ownerPartitionManagedObjectId;
                // In-memory only.
                bool                          m_isPartitionNameSetByUser;
                string                        m_userSpecifiedPartitionName;
                bool                          m_isManagedView;

                ObjectId                      m_ownerUserManagedObjectId;

    protected :
    public :

    friend class WaveLocalManagedObjectBase;
    friend class WaveManagedObjectLoadOperationalDataWorker;
    friend class DatabaseObjectManagerExecuteQueryWorker;
    friend class DatabaseObjectManager;
    friend class WaveSlotLocalManagedObjectBase;
};

}

#endif // WAVEMANAGEDOBJECT_H
