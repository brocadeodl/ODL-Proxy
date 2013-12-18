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

#ifndef PRISMWORKER_H
#define PRISMWORKER_H

#include "Framework/ObjectModel/PrismElement.h"
#include "Framework/Utils/PrismAsynchronousContext.h"

namespace WaveNs
{

class WaveObjectManager;
class WaveManagedObjectQueryContext;
class WaveManagedObject;
class WaveAsynchronousContextForBootPhases;
class WaveAsynchronousContextForPostbootPhase;
class WaveAsynchronousContextForShutDownPhases;
class WaveAsynchronousContextForExternalStateSynchronization;
class WaveAsynchronousContextForConfigReplayEnd;
class WaveAsynchronousContextForFileReplayEnd;
class WaveAsynchronousContextForSlotFailover;
class WaveAsynchronousContextForMultiPartitionCleanup;
class WaveAsynchronousContextForUpgradePhase;
class FailoverAsynchronousContext;
class WaveAsynchronousContextForDebugInformation;
class WaveObjectManagerStatisticsTracker;
class WaveManagedObjectSynchronousQueryContextForDeletion;
class WaveManagedObjectSynchronousQueryContextForUpdate;
class WaveSendMulticastContext;
class WaveObjectManagerCommitTransactionContext;
class CliBlockContext;
class WaveBrokerPublishMessage;


class WaveWorker : public PrismElement
{
    private :
        virtual void initialize                         (WaveAsynchronousContextForBootPhases                   *pWaveAsynchronousContextForBootPhases);
        virtual void listenForEvents                    (WaveAsynchronousContextForBootPhases                   *pWaveAsynchronousContextForBootPhases);
        virtual void install                            (WaveAsynchronousContextForBootPhases                   *pWaveAsynchronousContextForBootPhases);
        virtual void enable                             (WaveAsynchronousContextForBootPhases                   *pWaveAsynchronousContextForBootPhases);
        virtual void upgrade                            (WaveAsynchronousContextForUpgradePhase                 *pWaveAsynchronousContextForUpgradePhase);
        virtual void boot                               (WaveAsynchronousContextForBootPhases                   *pWaveAsynchronousContextForBootPhases);
        virtual void hainstall                          (WaveAsynchronousContextForBootPhases                   *pWaveAsynchronousContextForBootPhases);
        virtual void haboot                             (WaveAsynchronousContextForBootPhases                   *pWaveAsynchronousContextForBootPhases);
        virtual void postboot                           (WaveAsynchronousContextForPostbootPhase                *pWaveAsynchronousContextForPostbootPhase);
        virtual void heartBeatFailure                   (PrismAsynchronousContext                               *pPrismAsynchronousContext);
        virtual void externalStateSynchronization       (WaveAsynchronousContextForExternalStateSynchronization *pWaveAsynchronousContextForExternalStateSynchronization);
        virtual void configReplayEnd                    (WaveAsynchronousContextForConfigReplayEnd              *pWaveAsynchronousContextForConfigReplayEnd);
        virtual void fileReplayEnd                      (WaveAsynchronousContextForFileReplayEnd                *pWaveAsynchronousContextForFileReplayEnd);
        virtual void slotFailover                       (WaveAsynchronousContextForSlotFailover                 *pWaveAsynchronousContextForSlotFailover);
        virtual void multiPartitionCleanup              (WaveAsynchronousContextForMultiPartitionCleanup        *pWaveAsynchronousContextForMultiPartitionCleanup);
        virtual void multiPartitionPartialCleanup       (WaveAsynchronousContextForMultiPartitionCleanup        *pWaveAsynchronousContextForMultiPartitionCleanup);
        virtual void shutdown                           (WaveAsynchronousContextForShutDownPhases               *pWaveAsynchronousContextForShutDownPhases);
        virtual void uninstall                          (WaveAsynchronousContextForShutDownPhases               *pWaveAsynchronousContextForShutDownPhases);
        virtual void disable                            (WaveAsynchronousContextForShutDownPhases               *pWaveAsynchronousContextForShutDownPhases);
        virtual void uninitialize                       (WaveAsynchronousContextForShutDownPhases               *pWaveAsynchronousContextForShutDownPhases);
        virtual void destruct                           (WaveAsynchronousContextForShutDownPhases               *pWaveAsynchronousContextForShutDownPhases);
        virtual void backendSyncUp                      (PrismAsynchronousContext                               *pPrismAsynchronousContext);

        virtual void failover                           (FailoverAsynchronousContext                            *pFailoverAsynchronousContext);

        virtual void getDebugInformation                (WaveAsynchronousContextForDebugInformation             *pWaveAsynchronousContextForDebugInformation);
        virtual void resetDebugInformation              (WaveAsynchronousContextForDebugInformation             *pWaveAsynchronousContextForDebugInformation);

        virtual void updateTimeConsumedInThisThread     (const UI32 &operationCode, const UI32 &sequencerStepIndex, const UI32 &numberOfSeconds, const SI32 &numberOfNanoSeconds);
        virtual void updateRealTimeConsumedInThisThread (const UI32 &operationCode, const UI32 &sequencerStepIndex, const UI32 &numberOfSeconds, const SI32 &numberOfNanoSeconds);

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
        virtual WaveMessageStatus             reply                                     (const PrismEvent *&pPrismEvent);
        virtual WaveMessageStatus             broadcast                                 (PrismEvent *pPrismEvent);
        virtual void                          trace                                     (TraceLevel traceLevel, const string &stringToTrace);
        virtual void                          tracePrintf                               (TraceLevel traceLevel, const bool &addNewLine, const bool &suppressPrefix, const char * const pFormat, ...);
        virtual void                          tracePrintf                               (TraceLevel traceLevel, const char * const pFormat, ...);
        virtual void                          prismAssert                               (bool isAssertNotRequired, const char *pFileName, UI32 lineNumber);
                void                          addOperationMap                           (UI32 operationCode, PrismMessageHandler pPrismMessageHandler, PrismElement *pPrismElement = NULL);
                void                          removeOperationMap                        (const UI32 &operationCode);
                void                          addServiceIndependentOperationMap         (UI32 operationCode, WaveServiceIndependentMessageHandler pWaveServiceIndependentMessageHandler);
                void                          addServiceIndependentOperationMap         (UI32 operationCode, ManagementInterfaceServiceIndependentMessageHandler pManagementInterfaceServiceIndependentMessageHandler);
                void                          removeServiceIndependentOperationMap      (UI32 operationCode);
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

                void                          addManagedClass                           (const string &managedClassName);

        virtual void                          startTransaction                          ();
        virtual bool                          isTransactionInProgress                   ();
        virtual ResourceId                    commitTransaction                         (WaveObjectManagerCommitTransactionContext *pWaveObjectManagerCommitTransactionContext = NULL);
        virtual void                          rollbackTransaction                       ();
        virtual void                          addWaveConfigEntry                        (Attribute *attribute);
        virtual void                          updateWaveConfigEntry                     (Attribute *attribute);
        virtual void                          getWaveConfigEntry                        (string &configName, Attribute *attribute, bool &configFound);
        virtual void                          updateWaveManagedObject                   (WaveManagedObject *pWaveManagedObject);
        virtual void                          updateMultipleWaveManagedObjects          (WaveManagedObjectSynchronousQueryContextForUpdate *pWaveManagedObjectSynchronousQueryContextForUpdate);
        virtual void                          deleteWaveManagedObject                   (const ObjectId &objectId);
        virtual void                          deleteWaveManagedObject                   (WaveManagedObjectSynchronousQueryContextForDeletion *pWaveManagedObjectSynchronousQueryContextForDeletion);
        virtual void                          deleteMultipleWaveManagedObjects          (WaveManagedObjectSynchronousQueryContextForDeletion *pWaveManagedObjectSynchronousQueryContextForDeletion);
        virtual void                          deleteManagedObjectCompositions           (WaveManagedObjectSynchronousQueryContextForDeletion *pWaveManagedObjectSynchronousQueryContextForDeletion, const set<string> relationsSet = set<string>());
        virtual void                          deleteManagedObjectCompositions           (const ObjectId &objectId, const set<string> relationsSet = set<string>());
	virtual void                          deletePartialManagedObjectCompositions    (const string &parentClassName, const string &compositionName, WaveManagedObjectSynchronousQueryContextForDeletion *pQueryContextForDeletionOnChildMO);
        virtual bool                          isManagedClassSupported                   (const string &managedClass);

        virtual void                          addRelationship                           (const string &parentClassName, const string &childClassName, const string &relationshipName, const ObjectId &parentObjectId, const ObjectId &childObjectId);
        virtual void                          deleteRelationship                        (const string &parentClassName, const string &childClassName, const string &relationshipName, const ObjectId &parentObjectId, const ObjectId &childObjectId);
        virtual void                          addToComposition                          (const string &parentClassName, const string &childCalssName, const string &compositionName, const ObjectId &parentObjectId, const ObjectId &childObjectId);
        virtual void                          deleteFromComposition                     (const string &parentClassName, const string &childCalssName, const string &compositionName, const ObjectId &parentObjectId, const ObjectId &childObjectId);
        virtual void                          addToAssociation                          (const string &parentClassName, const string &childCalssName, const string &associationName, const ObjectId &parentObjectId, const ObjectId &childObjectId);
        virtual void                          deleteFromAssociation                     (const string &parentClassName, const string &childCalssName, const string &associationName, const ObjectId &parentObjectId, const ObjectId &childObjectId);

        virtual void                          query                                     (WaveManagedObjectQueryContext *pWaveManagedObjectQueryContext);
        virtual vector<WaveManagedObject *> *querySynchronouslyLocalManagedObjectsForLocationId (const LocationId &locationId, const string &className);
        virtual vector<WaveManagedObject *> *querySynchronouslyLocalManagedObjectsForLocationId (const LocationId &locationId, const string &className, const vector<string> &selectFields);
        virtual vector<WaveManagedObject *> *querySynchronouslyLocalManagedObjectsForLocationId (const LocationId &locationId, WaveManagedObjectSynchronousQueryContext *pWaveManagedObjectSynchronousQueryContext);
        virtual vector<WaveManagedObject *> *querySynchronously                         (WaveManagedObjectSynchronousQueryContext *pWaveManagedObjectSynchronousQueryContext);
        virtual WaveManagedObject           *createInMemoryManagedObject                (const  string &className);
        virtual ResourceId                   querySynchronouslyForCount                 (WaveManagedObjectSynchronousQueryContext *pWaveManagedObjectSynchronousQueryContext, UI32 &count);
        virtual ResourceId                   querySynchronouslyForCount                 (const string &managedClassName, UI32 &count, const string &schema = OrmRepository::getWaveCurrentSchema ());
        virtual ResourceId                   querySynchronouslyForCount                 (const string &managedClassName, const string &fieldName, const string &range, UI32 &count, const string &schema = OrmRepository::getWaveCurrentSchema ());
        virtual vector<WaveManagedObject *> *querySynchronously                         (const string &managedClassName, const string &schema = OrmRepository::getWaveCurrentSchema ());
        virtual vector<WaveManagedObject *> *querySynchronously                         (const string &managedClassName, vector<ObjectId> &objectIds, const string &schema = OrmRepository::getWaveCurrentSchema ());
        virtual vector<WaveManagedObject *> *querySynchronouslyByName                   (const string &managedClassName, const string &managedObjectName, const string &schema = OrmRepository::getWaveCurrentSchema ());
        virtual WaveManagedObject           *queryManagedObject                         (const ObjectId &managedObjectId, const string &schema = OrmRepository::getWaveCurrentSchema ());
        virtual vector<WaveManagedObject *> *queryManagedObjectAssociatedWithSlot       (const string &managedClassName, UI32 slotNumber, LocationId locationId = 0, const string &schema = OrmRepository::getWaveCurrentSchema ());

        virtual TraceClientId                  getTraceClientId                         ();

        virtual void                           addEventType                             (const UI32 &eventOperationCode);

                void                           setCpuAffinity                           (const vector<UI32> &cpuAffinityVector);
                PrismServiceId                 getServiceId                             ();

        virtual void                           listenForEvent                           (PrismServiceId prismServiceId, UI32 sourceOperationCode, PrismEventHandler pPrismEventHandler, PrismElement *pPrismElement = NULL, const LocationId &sourceLocationId = 0);
        virtual ResourceId                     sendSynchronouslyToWaveClient            (const string &waveClientName, ManagementInterfaceMessage *pManagementInterfaceMessage, const SI32 &Instance = 0);
        virtual WaveMessageStatus              sendToWaveServer                         (const UI32 &waveServerId, ManagementInterfaceMessage *pManagementInterfaceMessage, PrismMessageResponseHandler messageCallback, PrismElement *pPrismMessageSender, void *pInputContext, UI32 timeOutInMilliSeconds);
        virtual ResourceId                     sendToWaveClient                         (const string &waveClientName, ManagementInterfaceMessage *pManagementInterfaceMessage, PrismMessageResponseHandler pPrismMessageCallback, void *pPrismMessageContext = NULL, UI32 timeOutInMilliSeconds = 0, const SI32 &Instance = 0);
        virtual void                          sendToWaveClients                         (WaveSendToClientsContext *pWaveSendToClientsContext);
        virtual ResourceId                    sendOneWayToAllWaveClients                (ManagementInterfaceMessage *pManagementInterfaceMessage);
        virtual WaveManagedObject             *createManagedObjectInstance              (const string &managedClassName);
                void                           addManagedClass                          (const string &managedClassName, PrismElement *pOwnerForInstantiation = NULL);
        virtual PrismMessage                  *createMessageInstance                    (const UI32 &operationCode);

        virtual void                           printfToWaveClientSession                (const WaveClientSessionContext &waveClientSessionContext, const char * const pFormat, ...);
        virtual void                           printfToAllWaveClientSessions            (const WaveClientSessionContext &waveClientSessionContext, const char * const pFormat, ...);
        virtual ResourceId                     addLog                                   (ResourceId logType, ResourceId logDescriptionType, const vector<Attribute *> &logDescriptionArguments);
                void                           registerLock                             (const string &serviceString);
        virtual bool                           isBeingSurrogated                        ();

        virtual ResourceId                     updateHardwareSynchronizationState       (ResourceId hardwareSynchronizationState, const vector<LocationId> &locationIds);
        virtual ResourceId                     updateHardwareSynchronizationState       (ResourceId hardwareSynchronizationState, LocationId locationId = 0);

        virtual void                           postponeMessageHandling                  (PrismMessage *pPrismMessage);
        virtual void                           resumeAllPostponedMessages               ();

        /**
         * This API has few limitations.
         * 1. The MO/class 'className' should not be part of any c++ inheritance hierarchy.
         * 2. The table/MO should have a relation pointing to it or the MO should have 1-m relation point from it.
         *    This means the MO should have its own DerivationsInstances table.
         * So, Use deleteMultipleWaveManagedObjects API to avoid these limitations.
         */
        virtual void                           deleteAllManagedObjectInstances          (const string &className);
        virtual void                           dbBasicSanityCheck                       (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
        virtual void                           dbInconsistencyCheck                     (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);

        virtual void						   zeroize (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases);
                WaveObjectManagerStatisticsTracker  *getPWaveObjectManagerStatisticsTracker () const;

        virtual ResourceId                     blockCli                                  (const CliBlockContext &cliBlockContext, const bool &clusterWide);
        virtual ResourceId                     unblockCli                                (const CliBlockContext &cliBlockContext, const bool &clusterWide);

        virtual WaveMessageBrokerStatus        connectToMessageBroker                    (const string &brokerName, const string &brokerIpAddress, const SI32 &brokerPort);
        virtual WaveMessageBrokerStatus        subscribeToMessageBroker                  (const string &brokerName, const vector<string> &topicNames, const vector<WaveBrokerPublishMessageHandler> publishMessageHandlers, PrismElement *pSubscriber = NULL);
        virtual WaveMessageBrokerStatus        subscribeToMessageBroker                  (const string &brokerName, const vector<string> &topicNames, WaveBrokerPublishMessageHandler publishMessageHandler, PrismElement *pSubscriber = NULL);
        virtual WaveMessageBrokerStatus        subscribeToMessageBroker                  (const string &brokerName, const string &topicName, WaveBrokerPublishMessageHandler publishMessageHandler, PrismElement *pSubscriber = NULL);
        virtual WaveMessageBrokerStatus        unsubscribeToMessageBroker                (const string &brokerName, const vector<string> &topicNames, PrismElement *pSubscriber = NULL);
        virtual WaveMessageBrokerStatus        unsubscribeToMessageBroker                (const string &brokerName, const string &topicName, PrismElement *pSubscriber = NULL);
        virtual WaveMessageBrokerStatus        publishToMessageBroker                    (const string &brokerName, WaveBrokerPublishMessage *pWaveBrokerPublishMessage, PrismElement *pSubscriber = NULL);

                void                           deliverWaveBrokerPublishedEvent           (const string &brokerName, const string &topicName, WaveBrokerPublishMessage *pWaveBrokerPublishMessage);

    public :
                 WaveWorker (WaveObjectManager *pWaveObjectManager);
                 WaveWorker (WaveObjectManager *pWaveObjectManager, const bool &linkWorkerToParentObjectManager);

        virtual ~WaveWorker ();


    // Now the data members

    private :
		bool m_linkWorkerToParentObjectManager;

    protected :
    public :

    friend class WaveObjectManager;
    friend class PrismPostbootWorker;
    friend class PrismShutdownWorker;
    friend class WaveDebugInformationWorker;
};

}

#endif //PRISMWORKER_H
