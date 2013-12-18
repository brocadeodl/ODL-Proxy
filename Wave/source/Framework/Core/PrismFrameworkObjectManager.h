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
 *   Copyright (C) 2005-2009 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef PRISMFRAMEWORKOBJECTMANAGER_H
#define PRISMFRAMEWORKOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveLocalObjectManager.h"
#include "Framework/Core/FrameworkSequenceGeneratorFactory.h"
#include "Framework/Utils/ServerStreamingSocket.h"
#include "Framework/ObjectRelationalMapping/OrmRepository.h"
#include "Framework/Database/DatabaseStandaloneTransaction.h"

namespace WaveNs
{

class PrismFrameworkObjectManagerInitializeWorker;
class PrismFrameworkConfigurationWorker;
class PrismLinearSequencerContext;
class PrismInitializeObjectManagerMessage;
class LocationBase;
class PrismCreateClusterWithNodesMessage;
class PrismConfigureClusterSecondaryMessage;
class PrismConfigureClusterSecondaryHaPeerMessage;
class PrismConfigureClusterSecondaryPhase1Message;
class PrismConfigureClusterSecondaryHaPeerPhase1Message;
class PrismConfigureClusterSecondaryPhase2Message;
class PrismConfigureClusterSecondaryPhase3Message;
class PrismConfigureClusterSecondaryHaPeerPhase3Message;
class CreateClusterWithNodesContext;
class FrameworkObjectManagerAddNodesToClusterMessage;
class FrameworkObjectManagerDeleteNodesFromClusterMessage;
class FrameworkObjectManagerDestroyClusterMessage;
class AddNodesToClusterContext;
class DeleteNodesFromClusterContext;
class DeleteNodeOnSecondaryContext;
class FrameworkObjectManagerUnconfigureClusterSecondaryMessage;
class PrismConfigureClusterSecondaryMessage;
class FrameworkObjectManagerRejoinNodesToClusterMessage;
class FrameworkObjectManagerRejoinClusterSecondaryMessage;
class FrameworkObjectManagerRejoinClusterSecondaryHaPeerMessage;
class RejoinNodesToClusterContext;
class DestroyClusterContext;
class FrameworkObjectManagerDetachFromClusterMessage;
class FrameworkObjectManagerLostHeartBeatMessage;
class PrismFinalizeWorker;
class HeartBeatLostContext;
class DestroyClusterAsynchronousContext;
class DetachFromClusterAsynchronousContext;
class PrismFrameworkFailoverWorker;
class PrismFrameworkFailoverWorkerContext;
class DetachFromClusterContext;
class DatabaseObjectManagerBackupMessage;
class DatabaseObjectManagerCleanPreparedTransactionsMessage;
class DatabaseObjectManagerEmptyMessage;
class FrameworkObjectManagerUpdateListOfSecondariesMessage;
class UpdateListOfSecondariesContext;
class FrameworkObjectManagerPrimaryChangedMessage;
class FrameworkObjectManagerPrimaryChangedHaPeerMessage;
class FrameworkObjectManagerPrimaryChangedPhase1Message;
class FrameworkObjectManagerPrimaryChangedHaPeerPhase1Message;
class FrameworkObjectManagerPrimaryChangedPhase2Message;
class FrameworkObjectManagerPrimaryChangedPhase3Message;
class FrameworkObjectManagerPrimaryChangedHaPeerPhase3Message;
class PrismFrameworkObjectManagerServiceControlWorker;
class WaveAsynchronousContextForBootPhases;
class PrismFrameworkObjectManagerPostbootWorker;
class PrismFrameworkObjectManagerPostPersistentBootWorker;
class PrismFrameworkObjectManagerHaSyncWorker;
class PrismRejoinClusterSecondaryPhase1Message;
class PrismRejoinClusterSecondaryHaPeerPhase1Message;
class PrismRejoinClusterSecondaryPhase2Message;
class PrismRejoinClusterSecondaryPhase3Message;
class PrismRejoinClusterSecondaryHaPeerPhase3Message;
class FrameworkObjectManagerStartExternalStateSynchronizationMessage;
class FrameworkObjectManagerStartSlotFailoverMessage;
class FrameworkObjectManagerNewPrincipalEstablishedMessage;
class FrameworkObjectManagerSecondaryNodeFailureNotificationMessage;
class SecondaryNodeClusterContext;
class SecondaryNodeClusterPhase2Context;
class SecondaryNodeClusterPhase3Context;
class FrameworkObjectManagerDisconnectFromAllNodesMessage;
class FrameworkObjectManagerPrepareNodeForHAMessage;
class FrameworkObjectManagerDisconnectFromAllInstanceClientsMessage;
class FrameworkObjectManagerEstablishPrincipalAfterClusterRebootMessage;
class RejoinNodesToClusterContext;
class FrameworkObjectManagerResetNodeToUnconfirmRole;
class FrameworkObjectManagerRemoveKnownLocationsMessage;
class ZeroizeForFIPSMessage;
class ZeroizeForFIPSLinearSequencerContext;
class VcsClusterConfiguration;
class FrameworkObjectManagerPrepareForWarmRecoveryWithDefaultConfiguration;
class FrameworkObjectManagerStoreConfigurationIntentMessage;
class FrameworkObjectManagerRemoveConfigurationIntentMessage;
class FrameworkObjectManagerPrepareForHaFailoverWithPersistentConfiguration;
class FrameworkObjectManagerRollbackStandbyOnActiveRollbackMessage;
class PrismFrameworkReplayConfigMessage;

typedef SI32        (*GetInstancesFunction)                     (vector<SI32> &connectedInstancesVector);
typedef SI32        (*GetInstancesAndSubInstancesFunction)      (vector<SI32> &connectedInstancesVector, vector<SI32> &connectedSubInstancesVector);
typedef SI32        (*GetThisSlotInstanceFunction)              ();
typedef void        (*CreateDummySlotFunction)                  (const UI32 slotNumber);
typedef void        (*RaslogAssertFunction)                     (const string &file, const string &function, const UI32 line, const string &assertString);
typedef string      (*GetFirmwareVersionFunction)               ();
typedef int         (*HandleDcmFssConfigUpdateFunction)         (void *dcmConfigData);
typedef ResourceId  (*SetVcsClusterConfigHandlerFunction)       (VcsClusterConfiguration *pDcmVcsClusterConfigData, const UI32 &context);
typedef void        (*GetVcsClusterConfigHandlerFunction)       (VcsClusterConfiguration *pDcmVcsClusterConfigData, const UI32 &context);
typedef bool        (*NotifySyncUpdateFailureFunction)          (const ResourceId &reason);
typedef void        (*NotifyClusterReadyStateFunction)          (bool &readyState);

class PrismFrameworkObjectManager : WaveLocalObjectManager
{
    private :
                              PrismFrameworkObjectManager                                      ();
                             ~PrismFrameworkObjectManager                                      ();
                void          boot                                                             (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
                LocationBase *getThisLocation                                                  () const;
        const   UI32          getThisLocationId                                                () const;
        static  void          setIsFrameworkReadyToBoot                                        (bool isFrameworkReadyToBoot);
        static  bool          getIsFrameworkReadyToBoot                                        ();
                void          getInstances                                                     (vector<SI32> &connectedInstanceVector);
                void          getInstancesAndSubInstances                                      (vector<SI32> &connectedInstanceVector, vector<SI32> &connectedSubInstanceVector);
                SI32          getThisSlotInstance                                              ();
                void          createDummySlot                                                  (const UI32 slotNumber);
                bool          isServiceToBeExcludedInClusterCommunications                     (const PrismServiceId &prismServiceId);

                void          createClusterWithNodesMessageHandler                             (PrismCreateClusterWithNodesMessage *pPrismCreateClusterWithNodesMessage);
                void          createClusterWithNodesValidateStep                               (CreateClusterWithNodesContext *pCreateClusterWithNodesContext);
                void          createClusterWithNodesCollectValidationDataStep                  (CreateClusterWithNodesContext *pCreateClusterWithNodesContext);
                void          createClusterWithNodesValidateLocationRoleStep                   (CreateClusterWithNodesContext *pCreateClusterWithNodesContext);
                void          createClusterWithNodesAddKnownLocationsStep                      (CreateClusterWithNodesContext *pCreateClusterWithNodesContext);
                void          createClusterWithNodesConnectToNewKnownLocationsStep             (CreateClusterWithNodesContext *pCreateClusterWithNodesContext);
                void          createClusterWithNodesReplayGlobalOnlyConfigIfRequiredStep       (CreateClusterWithNodesContext *pCreateClusterWithNodesContext);
                void          createClusterWithNodesPausePersistenceStep                       (CreateClusterWithNodesContext *pCreateClusterWithNodesContext);
                void          backUpDatabaseAfterSendingClusterPhase0Message                   ();
                void          createClusterWithNodesConfigureNewKnownLocationsStep             (CreateClusterWithNodesContext *pCreateClusterWithNodesContext);
                void          createClusterWithNodesConfigureNewKnownLocationsCallback         (FrameworkStatus frameworkStatus, PrismConfigureClusterSecondaryMessage *pPrismConfigureClusterSecondaryMessage, void *pContext);
                void          resumeDatabase                                                   ();
                void          createClusterWithNodesConfigureNewKnownLocationsPhase1Step       (CreateClusterWithNodesContext *pCreateClusterWithNodesContext);
                void          createClusterWithNodesConfigureNewKnownLocationsPhase1Callback   (FrameworkStatus frameworkStatus, PrismConfigureClusterSecondaryPhase1Message *pPrismConfigureClusterSecondaryPhase1Message, void *pContext);
                void          createClusterWithNodesConfigureNewKnownLocationsPhase2Step       (CreateClusterWithNodesContext *pCreateClusterWithNodesContext);
                void          createClusterWithNodesConfigureNewKnownLocationsPhase2Callback   (FrameworkStatus frameworkStatus, PrismConfigureClusterSecondaryPhase2Message *pPrismConfigureClusterSecondaryPhase2Message, void *pContext);
                void          createClusterWithNodesConfigureNewKnownLocationsPhase3Step       (CreateClusterWithNodesContext *pCreateClusterWithNodesContext);
                void          createClusterWithNodesConfigureNewKnownLocationsPhase3Callback   (FrameworkStatus frameworkStatus, PrismConfigureClusterSecondaryPhase3Message *pPrismConfigureClusterSecondaryPhase3Message, void *pContext);
                void          failoverforNodesFailedInPhase2                                   (CreateClusterWithNodesContext *pCreateClusterWithNodesContext);
                void          failoverforNodesFailedInPhase2Callback                           (PrismFrameworkFailoverWorkerContext *pPrismFrameworkFailoverWorkerContext);
                void          failoverforNodesFailedInPhase3Callback                           (PrismFrameworkFailoverWorkerContext *pPrismFrameworkFailoverWorkerContext);
                void          createClusterWithNodesResumeDatabaseStep                         (CreateClusterWithNodesContext *pCreateClusterWithNodesContext);
                void          createClusterWithNodesSendValidationResultsStep                  (CreateClusterWithNodesContext *pCreateClusterWithNodesContext);
                void          createClusterWithNodesSendListOfSecondariesToAllNodesStep        (CreateClusterWithNodesContext *pCreateClusterWithNodesContext);
                void          createClusterWithNodesSendListOfSecondariesToAllNodesCallback    (FrameworkStatus frameworkStatus, FrameworkObjectManagerUpdateListOfSecondariesMessage *pFrameworkObjectManagerUpdateListOfSecondariesMessage, void *pContext);
                void          sendReplyBackToClusterGlobalService                              (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void          startHeartBeatToSecondaryNodes                                   (CreateClusterWithNodesContext *pCreateClusterWithNodesContext);
                void          broadcastNewNodesAddedEventOnPrimaryStep                         (CreateClusterWithNodesContext *pCreateClusterWithNodesContext);
                void          configureSecondaryNodeMessageHandler                             (PrismConfigureClusterSecondaryMessage *pPrismConfigureClusterSecondaryMessage);
                void          configureSecondaryNodeHaPeerMessageHandler                       (PrismConfigureClusterSecondaryHaPeerMessage *pPrismConfigureClusterSecondaryHaPeerMessage);
                void          rollbackNodeIfRequiredStep                                       (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void          prepareNodeForAddNodeIfRequired                                  (SecondaryNodeClusterContext *pSecondaryNodeClusterContext);
                void          prepareSecondaryNodeForAddNodeIfRequiredCallback                 (PrismAsynchronousContext *pPrismAsynchronousContext);
                void          preparePrimaryNodeForAddNodeIfRequiredCallBack                   (DestroyClusterAsynchronousContext *pDestroyClusterAsynchronousContext);
                void          configureSecondaryNodeValidateDefaultConfigurationStep           (SecondaryNodeClusterContext *pSecondaryNodeClusterContext);
                void          configureSecondaryNodeValidateStep                               (SecondaryNodeClusterContext *pSecondaryNodeClusterContext);
		        //ResourceId    updateTableIdsInOrmWithSchemaInfoReceivedFromPrimary             (const vector<string> & managedObjectNamesForSchemaChange,const vector<UI32> & classIds,const vector<UI32>  & parentTableIds);
                bool          isSchemaReceivedFromPrimaryCompatible                            (const vector<string> & managedObjectNamesForSchemaChange,const vector<string> & fieldNamesStringsForSchemaChange,const vector<string> & fieldNamesTypesForSchemaChange,const vector<UI32> & classIds,const vector<UI32>  & parentTableIds);
                //string        generateSqlToAlterAuxilliaryTableNames                           (vector<string>& oldAuxilliaryTableNames,vector<string>& newAuxilliaryTableNames, vector<string>&parentTableNames, vector<string>& relatedToTableNames);
                //string        handleConstraintsForAuxilliaryTable                              (string WaveSchema, string &oldAuxilliaryTableName, string &newAuxilliaryTableName, string& parentTableName, string &relatedToTableName);
                void          configureSecondaryNodeValidateVersionStep                        (SecondaryNodeClusterContext *pSecondaryNodeClusterContext);
                void          configureSecondaryNodeValidateServicesStep                       (SecondaryNodeClusterContext *pSecondaryNodeClusterContext);
                void          configureSecondaryNodeAddNewKnownLocationsStep                   (SecondaryNodeClusterContext *pSecondaryNodeClusterContext);
                void          configureSecondaryNodeConnectToNewKnownLocationsStep             (SecondaryNodeClusterContext *pSecondaryNodeClusterContext);
                void          configureSecondaryNodeBackupCurrentDatabaseStep                  (SecondaryNodeClusterContext *pSecondaryNodeClusterContext);
                void          configureSecondaryNodeCleanPreparedTransactionsStep              (SecondaryNodeClusterContext *pSecondaryNodeClusterContext);
                void          configureSecondaryNodeEmptyDatabaseStep                          (SecondaryNodeClusterContext *pSecondaryNodeClusterContext);
                void          configureSecondaryNodeLoadDatabaseFromPrimaryDatabaseStep        (SecondaryNodeClusterContext *pSecondaryNodeClusterContext);
                void          configureSecondaryNodeUpdateInstanceIdsStep                      (SecondaryNodeClusterContext *pSecondaryNodeClusterContext);
                void          configureSecondaryNodeShutdownServicesStep                       (SecondaryNodeClusterContext *pSecondaryNodeClusterContext);
                void          configureSecondaryNodeNotifyHaPeerStep                           (SecondaryNodeClusterContext *pSecondaryNodeClusterContext);
                void          configureSecondaryNodeConfigureMyLocationIdStep                  (SecondaryNodeClusterContext *pSecondaryNodeClusterContext);
                void          configureSecondaryNodeBootServicesPrePhaseStep                   (SecondaryNodeClusterContext *pSecondaryNodeClusterContext);
                void          configureSecondaryNodeBootServicesPrePhaseForStandbyStep         (SecondaryNodeClusterContext *pSecondaryNodeClusterContext);
                void          configureSecondaryNodeSetLocationRoleOnStandbyStep               (SecondaryNodeClusterContext *pSecondaryNodeClusterContext);

                void          checkHaPeerSyncStatusStep                                        (SecondaryNodeClusterContext *pSecondaryNodeClusterContext);
                void          checkHaPeerSyncPostPhaseStep                                     (SecondaryNodeClusterPhase3Context *pSecondaryNodeClusterPhase3Context);
                void          configureClusterSecondaryHaPeerMessageCallback                   (FrameworkStatus frameworkStatus, PrismConfigureClusterSecondaryHaPeerMessage *pPrismConfigureClusterSecondaryHaPeerMessage, void *pContext);
                void          configureSecondaryHaPeerPrePhaseStep                             (SecondaryNodeClusterContext *pSecondaryNodeClusterContext);
                void          configureSecondaryHaPeerPrePhaseCallback                         (FrameworkStatus frameworkStatus, PrismConfigureClusterSecondaryHaPeerPhase1Message *pPrismConfigureClusterSecondaryHaPeerPhase1Message, void *pContext);
                void          configureSecondaryHaPeerPostPhaseStep                            (SecondaryNodeClusterPhase3Context *pSecondaryNodeClusterPhase3Context);
                void          configureSecondaryHaPeerPostPhaseCallback                        (FrameworkStatus frameworkStatus, PrismConfigureClusterSecondaryHaPeerPhase3Message *pPrismConfigureClusterSecondaryHaPeerPhase3Message, void *pContext);
                void          configureSecondaryNodePhase1MessageHandler                       (PrismConfigureClusterSecondaryPhase1Message *pPrismConfigureClusterSecondaryPhase1Message);
                void          configureSecondaryNodeHaPeerPhase1MessageHandler                 (PrismConfigureClusterSecondaryHaPeerPhase1Message *pPrismConfigureClusterSecondaryHaPeerPhase1Message);
                void          updateLocationRoleStepInPhase1                                   (SecondaryNodeClusterContext *pSecondaryNodeClusterContext);

                void          configureSecondaryNodePhase2MessageHandler                       (PrismConfigureClusterSecondaryPhase2Message *pPrismConfigureClusterSecondaryPhase2Message);
                void          updateLocationRoleStepInPhase2                                   (SecondaryNodeClusterPhase2Context *pSecondaryNodeClusterPhase2Context);
                void          configureSecondaryNodePhase2BootServicesPostPhaseStep            (SecondaryNodeClusterPhase2Context *pSecondaryNodeClusterPhase2Context);
                void          configureSecondaryNodePhase2ReplaceNodeIfNeededStep              (SecondaryNodeClusterPhase2Context *pSecondaryNodeClusterPhase2Context);
                void          configureSecondaryNodePhase2ConfigureThisLocationStep            (SecondaryNodeClusterPhase2Context *pSecondaryNodeClusterPhase2Context);
                void          configureSecondaryNodePhase3SendStartHearBeat                    (SecondaryNodeClusterPhase3Context *pSecondaryNodeClusterPhase3Context);
                void          configureSecondaryNodePhase3ExecutePostBootStep                  (SecondaryNodeClusterPhase3Context *pSecondaryNodeClusterPhase3Context);
                void          configureSecondaryNodePhase3MessageHandler                       (PrismConfigureClusterSecondaryPhase3Message *pPrismConfigureClusterSecondaryPhase3Message);
                void          configureSecondaryNodeHaPeerPhase3MessageHandler                 (PrismConfigureClusterSecondaryHaPeerPhase3Message *pPrismConfigureClusterSecondaryHaPeerPhase3Message);
                void          updateLocationRoleStepInPhase3                                   (SecondaryNodeClusterPhase3Context *pSecondaryNodeClusterPhase3Context);
                void          updateLocationRoleStepPhase3Complete                             (SecondaryNodeClusterPhase3Context *pSecondaryNodeClusterPhase3Context);
                void          configureSecondaryNodePhase3BootServicesAfterPostPhaseStep       (SecondaryNodeClusterPhase3Context *pSecondaryNodeClusterPhase3Context);
                void          configureSecondaryNodePhase3ReplayLocalOnlyConfigIfRequiredStep  (SecondaryNodeClusterPhase3Context *pSecondaryNodeClusterPhase3Context);
                void          updateListOfSecondariesMessageHandler                            (FrameworkObjectManagerUpdateListOfSecondariesMessage *pFrameworkObjectManagerUpdateListOfSecondariesMessage);
                void          updateListOfSecondariesSetupContextStep                          (UpdateListOfSecondariesContext *pUpdateListOfSecondariesContext);
                void          updateListOfSecondariesAddKnownLocationsStep                     (UpdateListOfSecondariesContext *pUpdateListOfSecondariesContext);
                void          updateListOfSecondariesConnectToNewKnownLocationsStep            (UpdateListOfSecondariesContext *pUpdateListOfSecondariesContext);
                void          broadcastListOfNewlyAddedNodesStep                               (UpdateListOfSecondariesContext *pUpdateListOfSecondariesContext);
                void          broadcastClusterPhase3StartEvent                                 (SecondaryNodeClusterPhase3Context *pSecondaryNodeClusterPhase3Context);
                void          broadcastClusterPhase3CompleteEvent                              (SecondaryNodeClusterPhase3Context *pSecondaryNodeClusterPhase3Context);
                void          broadcastNodesAdditionToClusterCompletedEvent                    (CreateClusterWithNodesContext *pCreateClusterWithNodesContext);

                void          broadcastPrimaryChangedEventForPlugins                           (SecondaryNodeClusterPhase3Context *pSecondaryNodeClusterPhase3Context);
                void          addNodesToClusterMessageHandler                                  (FrameworkObjectManagerAddNodesToClusterMessage *pFrameworkObjectManagerAddNodesToClusterMessage);
                void          addNodesToClusterValidateStep                                    (AddNodesToClusterContext *pAddNodesToClusterContext);
                void          addNodesToClusterAddKnownLocationsStep                           (AddNodesToClusterContext *pAddNodesToClusterContext);
                void          addNodesToClusterConnectToNewKnownLocationsStep                  (AddNodesToClusterContext *pAddNodesToClusterContext);
                void          addNodesToClusterConfigureNewKnownLocationsStep                  (AddNodesToClusterContext *pAddNodesToClusterContext);
                void          addNodesToClusterConfigureNewKnownLocationsCallback              (FrameworkStatus frameworkStatus, PrismConfigureClusterSecondaryMessage *pPrismConfigureClusterSecondaryMessage, void *pContext);

                void          deleteNodesFromClusterMessageHandler                             (FrameworkObjectManagerDeleteNodesFromClusterMessage *pFrameworkObjectManagerDeleteNodesFromClusterMessage);
                void          deleteNodesFromClusterValidateStep                               (DeleteNodesFromClusterContext *pDeleteNodesFromClusterContext);
                void          deleteNodesFromClusterUnconfigureKnownLocationsStep              (DeleteNodesFromClusterContext *pDeleteNodesFromClusterContext);
                void          deleteNodesFromClusterUnconfigureKnownLocationsCallback          (FrameworkStatus frameworkStatus, FrameworkObjectManagerUnconfigureClusterSecondaryMessage *pFrameworkObjectManagerUnconfigureClusterSecondaryMessage, void *pContext);
                void          deleteNodesFromClusterRemoveKnownLocationsStep                   (DeleteNodesFromClusterContext *pDeleteNodesFromClusterContext);
                void          deleteNodesFromClusterRunFailoverStep                            (DeleteNodesFromClusterContext *pDeleteNodesFromClusterContext);
                void          deleteNodesFromClusterRunFailoverCallback                        (PrismFrameworkFailoverWorkerContext *pPrismFrameworkFailoverWorkerContext);

                void          unconfigureClusterSecondaryMessageHandler                        (FrameworkObjectManagerUnconfigureClusterSecondaryMessage *pFrameworkObjectManagerUnconfigureClusterSecondaryMessage);
                void          unconfigureClusterSecondaryValidateStep                          (DeleteNodeOnSecondaryContext *pDeleteNodeOnSecondaryContext);
                void          unconfigureClusterSecondarySendStopHeartBeat                     (DeleteNodeOnSecondaryContext *pDeleteNodeOnSecondaryContext);
                void          unconfigureClusterSecondaryDisconnectFromKnownLocationStep       (DeleteNodeOnSecondaryContext *pDeleteNodeOnSecondaryContext);
                void          unconfigureClusterSecondaryDisconnectFromConnectedLocationStep   (DeleteNodeOnSecondaryContext *pDeleteNodeOnSecondaryContext);
                void          unconfigureClusterSecondaryUpdateThisLocationStep                (DeleteNodeOnSecondaryContext *pDeleteNodeOnSecondaryContext);
                void          unconfigureClusterSecondaryNodeBootServicesStep                  (DeleteNodeOnSecondaryContext *pDeleteNodeOnSecondaryContext);
                void          unconfigureClusterSecondaryUnconfigureThisLocationStep           (DeleteNodeOnSecondaryContext *pDeleteNodeOnSecondaryContext);
                void          unconfigureClusterSecondaryRunFailoverStep                       (DeleteNodeOnSecondaryContext *pDeleteNodeOnSecondaryContext);
                void          unconfigureClusterBroadcastLocalNodeDeletedEventStep             (DeleteNodeOnSecondaryContext *pDeleteNodeOnSecondaryContext);
                void          unconfigureClusterSecondaryRunFailoverCallback                   (PrismFrameworkFailoverWorkerContext *pPrismFrameworkFailoverWorkerContext);

                void          destroyClusterMessageHandler                                     (FrameworkObjectManagerDestroyClusterMessage *pFrameworkObjectManagerDestroyClusterMessage);
                void          destroyClusterMessageHandlerCallback                             (DestroyClusterAsynchronousContext *pDestroyClusterAsynchronousContext);
                void          destroyClusterAsynchronousHandler                                (DestroyClusterAsynchronousContext *pDestroyClusterAsynchronousContext);
                void          destroyClusterValidateStep                                       (DestroyClusterContext *pDestroyClusterContext);
                void          destroyClusterGetKnownLocationsStep                              (DestroyClusterContext *pDestroyClusterContext);
                void          destroyClusterUnconfigureKnownLocationsStep                      (DestroyClusterContext *pDestroyClusterContext);
                void          destroyClusterUnconfigureKnownLocationsCallback                  (FrameworkStatus frameworkStatus, FrameworkObjectManagerUnconfigureClusterSecondaryMessage *pFrameworkObjectManagerUnconfigureClusterSecondaryMessage, void *pContext);
                void          destroyClusterDeleteHeartBeatToKnownLocationsStep                (DestroyClusterContext *pDestroyClusterContext);
                void          destroyClusterTerminateClientConnectionsForKnownLocationsStep    (DestroyClusterContext *pDestroyClusterContext);
                void          destroyClusterRemoveKnownLocationsStep                           (DestroyClusterContext *pDestroyClusterContext);
                void          destroyClusterUnconfigureSelfStep                                (DestroyClusterContext *pDestroyClusterContext);
                void          destroyClusterRunFailoverStep                                    (DestroyClusterContext *pDestroyClusterContext);
                void          destroyClusterRunFailoverCallback                                (PrismFrameworkFailoverWorkerContext *pPrismFrameworkFailoverWorkerContext);
                void          destroyClusterBroadcastDeleteClusterEventStep                    (DestroyClusterContext *pDestroyClusterContext);

//Methods on the Primary to process Rejoin
                void          rejoinNodesToClusterMessageHandler                               (FrameworkObjectManagerRejoinNodesToClusterMessage *pFrameworkObjectManagerRejoinNodesToClusterMessage);
                void          rejoinNodesToClusterValidateStep                                 (RejoinNodesToClusterContext *pRejoinNodesToClusterContext);
                void          rejoinNodesToClusterConnectToNodesStep                           (RejoinNodesToClusterContext *pRejoinNodesToClusterContext);
                void          rejoinNodesToClusterRejoinClusterSecondariesStep                 (RejoinNodesToClusterContext *pRejoinNodesToClusterContext);
                void          rejoinNodesToClusterRejoinClusterSecondaryCallback               (FrameworkStatus frameworkStatus, FrameworkObjectManagerRejoinClusterSecondaryMessage *pFrameworkObjectManagerRejoinClusterSecondaryMessage, void *pContext);
                void          rejoinNodesToClusterRejoinClusterSecondariesPhase1Step           (RejoinNodesToClusterContext *pRejoinNodesToClusterContext);
                void          rejoinNodesToClusterRejoinClusterSecondariesPhase1StepCallback   (FrameworkStatus frameworkStatus, PrismRejoinClusterSecondaryPhase1Message *pPrismRejoinClusterSecondaryPhase1Message, void *pContext);
                void          rejoinNodesToClusterRejoinClusterSecondariesPhase2Step           (RejoinNodesToClusterContext *pRejoinNodesToClusterContext);
                void          rejoinNodesToClusterRejoinClusterSecondariesPhase2StepCallback   (FrameworkStatus frameworkStatus, PrismRejoinClusterSecondaryPhase2Message *pPrismRejoinClusterSecondaryPhase2Message, void *pContext);
                void          failoverforNodesFailedInRejoinPhase2                             (RejoinNodesToClusterContext *pRejoinNodesToClusterContext);
                void          failoverforNodesFailedInRejoinPhase2Callback                     (PrismFrameworkFailoverWorkerContext *pPrismFrameworkFailoverWorkerContext);
                void          rejoinNodesToClusterRejoinClusterSecondariesPhase3Step           (RejoinNodesToClusterContext *pRejoinNodesToClusterContext);
                void          rejoinNodesToClusterRejoinClusterSecondariesPhase3StepCallback   (FrameworkStatus frameworkStatus, PrismRejoinClusterSecondaryPhase3Message *pPrismRejoinClusterSecondaryPhase3Message, void *pContext);
                void          failoverforNodesFailedInRejoinPhase3Callback                     (PrismFrameworkFailoverWorkerContext *pPrismFrameworkFailoverWorkerContext);
//Methods on the Secondary for handling Rejoin
                void          rejoinClusterSecondaryMessageHandler                             (FrameworkObjectManagerRejoinClusterSecondaryMessage *pFrameworkObjectManagerRejoinClusterSecondaryMessage);
                void          rejoinClusterSecondaryHaPeerMessageHandler                       (FrameworkObjectManagerRejoinClusterSecondaryHaPeerMessage *pFrameworkObjectManagerRejoinClusterSecondaryHaPeerMessage);
                void          rejoinClusterSecondaryHaPeerMessageCallback                      (FrameworkStatus frameworkStatus, FrameworkObjectManagerRejoinClusterSecondaryHaPeerMessage *pFrameworkObjectManagerRejoinClusterSecondaryHaPeerMessage, void *pContext);
                void          rejoinSecondaryHaPeerPrePhaseStep                                (SecondaryNodeClusterContext *pSecondaryNodeClusterContext);
                void          rejoinSecondaryHaPeerPrePhaseCallback                            (FrameworkStatus frameworkStatus, PrismRejoinClusterSecondaryHaPeerPhase1Message *pPrismRejoinClusterSecondaryHaPeerPhase1Message, void *pContext);
                void          rejoinSecondaryHaPeerPostPhaseStep                               (SecondaryNodeClusterPhase3Context *pSecondaryNodeClusterPhase3Context);
                void          rejoinSecondaryHaPeerPostPhaseCallback                           (FrameworkStatus frameworkStatus, PrismRejoinClusterSecondaryHaPeerPhase3Message *pPrismRejoinClusterSecondaryHaPeerPhase3Message, void *pContext);
                void          rejoinSecondaryNodeValidateStep                                  (SecondaryNodeClusterContext *pSecondaryNodeClusterContext);
                void          rejoinSecondaryNodeValidateVersionStep                           (SecondaryNodeClusterContext *pSecondaryNodeClusterContext);
                void          rejoinSecondaryNodeValidateServicesStep                          (SecondaryNodeClusterContext *pSecondaryNodeClusterContext);
                void          rejoinSecondaryNodeConnectToPrimaryStep                          (SecondaryNodeClusterContext *pSecondaryNodeClusterContext);
                void          rejoinSecondaryNodeShutdownServicesStep                          (SecondaryNodeClusterContext *pSecondaryNodeClusterContext);
                void          rejoinSecondaryNodeNotifyHaPeerStep                              (SecondaryNodeClusterContext *pSecondaryNodeClusterContext);
                void          rejoinSecondaryNodeConfigureThisLocationStep                     (SecondaryNodeClusterContext *pSecondaryNodeClusterContext);
                void          rejoinSecondaryNodeBootServicesPrePhaseStep                      (SecondaryNodeClusterContext *pSecondaryNodeClusterContext);
                void          rejoinSecondaryNodeSetLocationRoleOnStandbyStep                  (SecondaryNodeClusterContext *pSecondaryNodeClusterContext);
                void          rejoinClusterSecondaryPhase1MessageHandler                       (PrismRejoinClusterSecondaryPhase1Message *pPrismRejoinClusterSecondaryPhase1Message);
                void          rejoinClusterSecondaryHaPeerPhase1MessageHandler                 (PrismRejoinClusterSecondaryHaPeerPhase1Message *pPrismRejoinClusterSecondaryHaPeerPhase1Message);
                void          rejoinClusterSecondaryPhase2MessageHandler                       (PrismRejoinClusterSecondaryPhase2Message *pPrismRejoinClusterSecondaryPhase2Message);
                void          rejoinClusterSecondaryPhase3MessageHandler                       (PrismRejoinClusterSecondaryPhase3Message *pPrismRejoinClusterSecondaryPhase3Message);
                void          rejoinClusterSecondaryHaPeerPhase3MessageHandler                 (PrismRejoinClusterSecondaryHaPeerPhase3Message *pPrismRejoinClusterSecondaryHaPeerPhase3Message);
                void          updateLocationRoleStepInRejoinPhase1                             (SecondaryNodeClusterContext *pSecondaryNodeClusterContext);
                void          updateLocationRoleStepInRejoinPhase2                             (SecondaryNodeClusterPhase2Context *pSecondaryNodeClusterPhase2Context);
                void          updateLocationRoleStepInRejoinPhase3                             (SecondaryNodeClusterPhase3Context *pSecondaryNodeClusterPhase3Context);
                void          updateLocationRoleStepInRejoinPhase3Complete                     (SecondaryNodeClusterPhase3Context *pSecondaryNodeClusterPhase3Context);
                void          rejoinSecondaryNodePhase2BootServicesPostPhaseStep               (SecondaryNodeClusterPhase2Context *pSecondaryNodeClusterPhase2Context);
                void          rejoinSecondaryNodePhase3BootServicesAfterPostPhaseStep          (SecondaryNodeClusterPhase3Context *pSecondaryNodeClusterPhase3Context);
                void          detachFromClusterMessageHandler                                  (FrameworkObjectManagerDetachFromClusterMessage *pFrameworkObjectManagerDetachFromClusterMessage);
                void          detachFromClusterMessageHandlerCallback                          (DetachFromClusterAsynchronousContext *pDetachFromClusterAsynchronousContext);
                void          detachFromClusterAsynchronousHandler                             (DetachFromClusterAsynchronousContext *pDetachFromClusterAsynchronousContext);
                void          detachFromClusterValidateStep                                    (DetachFromClusterContext *pDetachFromClusterContext);
                void          detachFromClusterTerminateClientConnectionsForKnownLocationsStep (DetachFromClusterContext *pDetachFromClusterContext);
                void          detachFromClusterDisconnectFromKnownLocationsStep                (DetachFromClusterContext *pDetachFromClusterContext);
                void          detachFromClusterUnconfigureThisLocationStep                     (DetachFromClusterContext *pDetachFromClusterContext);
                void          detachFromClusterRunFailoverStep                                 (DetachFromClusterContext *pDetachFromClusterContesxt);
                void          detachFromClusterRunFailoverCallback                             (PrismFrameworkFailoverWorkerContext *pPrismFrameworkFailoverWorkerContext);

                void          lostHeartBeatMessageHandler                                      (FrameworkObjectManagerLostHeartBeatMessage *pFrameworkObjectManagerLostHeartBeatMessage);
                void          lostHeartBeatStep                                                (HeartBeatLostContext *pHeartBeatLostContext);
                void          lostHeartBeatPrimaryUncontrolledFailoverCallback                 (PrismFrameworkFailoverWorkerContext *pPrismFrameworkFailoverWorkerContext);
                void          lostHeartBeatSecondaryUncontrolledFailoverCallback               (PrismFrameworkFailoverWorkerContext *pPrismFrameworkFailoverWorkerContext);
                void          lostHeartBeatSecondaryControlledFailoverCallback                 (PrismFrameworkFailoverWorkerContext *pPrismFrameworkFailoverWorkerContext);
                void          lostHeartBeatCleanupClusterDestroyClusterCallback                (DestroyClusterAsynchronousContext *pDestroyClusterAsynchronousContext);
                void          lostHeartBeatCleanupClusterDetachFromClusterCallback             (DetachFromClusterAsynchronousContext *pDetachFromClusterAsynchronousContext);

                void          primaryChangedMessageHandler                                     (FrameworkObjectManagerPrimaryChangedMessage *pFrameworkObjectManagerPrimaryChangedMessage);
                void          primaryChangedHaPeerMessageHandler                               (FrameworkObjectManagerPrimaryChangedHaPeerMessage *pFrameworkObjectManagerPrimaryChangedHaPeerMessage);
                void          primaryChangedValidateStep                                       (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void          primaryChangedNotifyHaPeerStep                                   (SecondaryNodeClusterContext *pSecondaryNodeClusterContext);
                void          primaryChangedNotifyHaPeerMessageCallback                        (FrameworkStatus frameworkStatus, FrameworkObjectManagerPrimaryChangedHaPeerMessage *pFrameworkObjectManagerPrimaryChangedHaPeerMessage, void *pContext);
                void          primaryChangedHaPeerPrePhaseStep                                 (SecondaryNodeClusterContext *pSecondaryNodeClusterContext);
                void          primaryChangedHaPeerPrePhaseCallback                             (FrameworkStatus frameworkStatus, FrameworkObjectManagerPrimaryChangedHaPeerPhase1Message *pFrameworkObjectManagerPrimaryChangedHaPeerPhase1Message, void *pContext);
                void          primaryChangedHaPeerPostPhaseStep                                (SecondaryNodeClusterPhase3Context *pSecondaryNodeClusterPhase3Context);
                void          primaryChangedHaPeerPostPhaseCallback                            (FrameworkStatus frameworkStatus, FrameworkObjectManagerPrimaryChangedHaPeerPhase3Message *pFrameworkObjectManagerPrimaryChangedHaPeerPhase3Message, void *pContext);
                void          primaryChangedStopHeartBeatToOldPrimayStep                       (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void          primaryChangedRemoveOldPrimaryLocationStep                       (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void          primaryChangedUpdatePrimaryDetailsStep                           (SecondaryNodeClusterContext *pSecondaryNodeClusterContext);
                void          primaryChangedEmptyDatabaseStep                                  (SecondaryNodeClusterContext *pSecondaryNodeClusterContext);
                void          primaryChangedLoadDatabaseFromPrimaryDatabaseStep                (SecondaryNodeClusterContext *pSecondaryNodeClusterContext);
                void          primaryChangedPhase1MessageHandler                               (FrameworkObjectManagerPrimaryChangedPhase1Message *pFrameworkObjectManagerPrimaryChangedPhase1Message);
                void          primaryChangedHaPeerPhase1MessageHandler                         (FrameworkObjectManagerPrimaryChangedHaPeerPhase1Message *pFrameworkObjectManagerPrimaryChangedHaPeerPhase1Message);
                void          primaryChangedPhase2MessageHandler                               (FrameworkObjectManagerPrimaryChangedPhase2Message *pFrameworkObjectManagerPrimaryChangedPhase2Message);
                void          primaryChangedPhase3MessageHandler                               (FrameworkObjectManagerPrimaryChangedPhase3Message *pFrameworkObjectManagerPrimaryChangedPhase3Message);
                void          primaryChangedHaPeerPhase3MessageHandler                         (FrameworkObjectManagerPrimaryChangedHaPeerPhase3Message *pFrameworkObjectManagerPrimaryChangedHaPeerPhase3Message);
                void          prepareStandbyAfterClusterPhaseSync                              (SecondaryNodeClusterPhase3Context *pSecondaryNodeClusterPhase3Context);
                void          savePrismConfigurationStep                                       (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void          resetNodeForClusterMergeHandler                                  (FrameworkObjectManagerResetNodeToUnconfirmRole *pFrameworkObjectManagerResetNodeToUnconfirmRole);
                void          stopHeartBeatToNode                                              (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void          disconnectFromAllNodes                                           (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void          disconnectFromAllConnectedNodes                                  ();
                void          resetNodeShutdownServicesStep                                    (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void          configureNodeForResetAndStartServices                            (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void          triggerUncontrolledFailoverForRemainingNodes                     (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void          triggerUncontrolledFailoverForRemainingNodesCallback             (PrismFrameworkFailoverWorkerContext *pPrismFrameworkFailoverWorkerContext);
                void          removePreviousDatabaseBackupFile                                 (PrismLinearSequencerContext *pPrismLinearSequencerContext);

                void          startExternalStateSynchronizationHandler                         (FrameworkObjectManagerStartExternalStateSynchronizationMessage *pFrameworkObjectManagerStartExternalStateSynchronizationMessage);
                void          startSlotFailoverHandler                                         (FrameworkObjectManagerStartSlotFailoverMessage *pFrameworkObjectManagerStartSlotFailoverMessage);

                void          getKnownRemoteLocations                                          (vector<LocationId> &remoteLocaionsVector);
                void          getKnownLocations                                                (vector<LocationId> &knownLocaionsVector);
                void          getConnectedLocations                                            (vector<LocationId> &connectedLocaionsVector);
                void          getFullyConnectedLocations                                       (vector<LocationId> &connectedLocaionsVector);
                void          getKnownSubLocations                                             (vector<LocationId> &knownSubLocaionsVector);
                bool          isAKnownLocation                                                 (LocationId locationId);

                void          initializeLastUsedLocationId                                     ();
                LocationId    getNextLocationId                                                ();
        static  void          setIsInstantiated                                                (const bool &isInstantiated);

                LocationId    getLastUsedLocationIdInPrismFrameworkObjectManager               () const;
                void          setLastUsedLocationIdInPrismFrameworkObjectManager               (const LocationId &locationId);

        virtual PrismMessage *createMessageInstance                                            (const UI32 &operationCode);
                void          secondaryNodeClusterFailureStep                                  (SecondaryNodeClusterContext *pSecondaryNodeClusterContext);
                void          primaryNodeClusterSuccessStep                                    (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void          primaryNodeClusterFailureStep                                    (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void          cleanupManageObject                                              (LocationId locationId);
                void          deleteObjects                                                    (vector<WaveManagedObject *> *pResults);

                void          setActivationCompleted                                           (bool isActivationCompleted);
                bool          getActivationCompleted                                           ();
                string        getActivationCompletedTimeStamp                                  ();

                void          setWarmHaRecoveryPreparationInProgress                           (const bool &isWarmRecovery);
                bool          isWarmHaRecoveryPreparationInProgress                            ();

                void          incrementLineCardPostBootCount                                   ();
                UI32          getNumberOfLineCardPostBootCurrentlyRunning                      ();
                void          decrementLineCardPostBootCount                                   ();

                void          setPostBootStarted                                               (bool isPostBootStarted);
                bool          getPostBootStarted                                               ();
                string        getPostBootStartedTimeStamp                                      ();

                void          setPostBootCompleted                                             (bool isPostBootCompleted); 
                bool          getPostBootCompleted                                             ();
                string        getPostBootCompletedTimeStamp                                    ();

                void          setConfigReplayStarted                                           (bool isConfigreplayStarted);
                bool          getConfigReplayStarted                                           ();
                string        getConfigReplayStartedTimeStamp                                  ();

                void          setConfigReplayCompleted                                         (bool isConfigReplayCompleted);
                bool          getConfigReplayCompleted                                         ();
                string        getConfigReplayCompletedTimeStamp                                ();

                bool          getConfigReplayInProgress                                        ();

                void          setFileReplayStarted                                             (bool isFileReplayStarted);
                bool          getFileReplayStarted                                             ();
                string        getFileReplayStartedTimeStamp                                    ();

                void          setFileReplayCompleted                                           (bool isFileReplayCompleted);
                bool          getFileReplayCompleted                                           ();
                string        getFileReplayCompletedTimeStamp                                  ();

                bool          getFileReplayInProgress                                          ();

                void          fileReplayCompletedProcessing                                    (bool fileReplayCompleted);
                void          startFileReplayEndAgent                                          ();
                
                void          setNodeReadyForAllCommands                                       (bool isNodeReadyForAllCommands);
                bool          getNodeReadyForAllCommands                                       ();                
                string        getNodeReadyForAllCommandsTimeStamp                              ();

                void          generateTimeStamp                                                (string & timeString);
                void          startClusterPhaseTimer                                           (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void          stopClusterPhaseTimer                                            (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void          clusterPhaseTimerCallback                                        (TimerHandle timerHandle, void * pContext);

                void          createDatabaseBackupBuffer                                       (char * &pDatabaseBackupBuffer, SI32 &sizeOfBackupFile);
                ResourceId    createBufferForFileToSync                                        (const string &filenameToSync, char* &pfileBuffer, UI32 &sizeOfFileBuffer );  
                ResourceId    createFileForSyncBuffer                                          (const string &filenameToSync, char* &pfileBuffer, UI32 &sizeOfFileBuffer ); 

                bool          getIsPostBootNeededDuringRejoin                                  ();
                void          setIsPostBootNeededDuringRejoin                                  (const bool isPostBootNeededDuringRejoin);
                /** FIPSZeroize
                *  setter for isNodeZeroized flag. This flag set only when FIPSZeroize message is receeived.
                */
                void          setNodeZeroized                                                  (bool nodeZeroized);
                void          removeFailedLocationsFromKnownLocationsHandler                   (FrameworkObjectManagerRemoveKnownLocationsMessage *pFrameworkObjectManagerRemoveKnownLocationsMessage);
                void          raslogAssert                                                     (const string &file, const string &function, const UI32 line, const string &assertString);
                void          prepareNodeForHASupportHandler                                   ();
                void          prepareFrameworkForHaRecoveryMessageHandler                      (FrameworkObjectManagerPrepareForHaFailoverWithPersistentConfiguration *pFrameworkObjectManagerPrepareForHaFailoverWithPersistentConfiguration);
                void          pausePersistence                                                 ();
                void          resumePersistence                                                ();
                void          resumeDatabaseOperation                                          ();

                // For Cluster support for DB Upgrade
                //void          setTableIdsChangedForSchemaCompatibilityFlag                      (bool tableIdsChanged);
                //bool          getTableIdsChangedForSchemaCompatibilityFlag                      ();

                void          setAuxilliaryTableDetailsForSchemaConversion                      (vector<string> oldAuxilliaryTables, vector<string> newAuxilliaryTables, vector<string> parentTables, vector<string> relatedToTables);
                void          getAuxilliaryTableDetailsForSchemaConversion                      (vector<string>& oldAuxilliaryTables, vector<string>& newAuxilliaryTables, vector<string>& parentTables, vector<string>& relatedToTables);
                void          revertTableIdsIfRequired                                          ();

                void          storeConfigurationIntentMessageHandler                            (FrameworkObjectManagerStoreConfigurationIntentMessage *pFrameworkObjectManagerStoreConfigurationIntentMessage);
                void          removeConfigurationIntentMessageHandler                           (FrameworkObjectManagerRemoveConfigurationIntentMessage *pFrameworkObjectManagerRemoveConfigurationIntentMessage);

        static  void          storeConfigurationIntentStaticMessageHandler                      (FrameworkObjectManagerStoreConfigurationIntentMessage *pFrameworkObjectManagerStoreConfigurationIntentMessage);
        static  void          removeConfigurationIntentStaticMessageHandler                     (FrameworkObjectManagerRemoveConfigurationIntentMessage *pFrameworkObjectManagerRemoveConfigurationIntentMessage);
                
                void          rollbackStandbyOnActiveRollbackHandler                            (FrameworkObjectManagerRollbackStandbyOnActiveRollbackMessage *pFrameworkObjectManagerRollbackStandbyOnActiveRollbackMessage);
                void          replayConfigFileMessageHandler                                    (PrismFrameworkReplayConfigMessage *pPrismFrameworkReplayConfigMessage);

    protected :
        PrismFrameworkConfigurationWorker   *getPConfigurationWorker                               ();
        PrismFinalizeWorker                 *getPFinalizeWorker                                    ();
        PrismFrameworkObjectManagerInitializeWorker *getPInitializeWorker                          ();
    public :
        static  PrismFrameworkObjectManager *getInstance                                           ();
        static  string                       getServiceName                                        ();
        static  PrismServiceId               getPrismServiceId                                     ();
        static  void                         bootPrism                                             ();
                void                         configureThisLocationAsLocation                       (SI32 port);
                bool                         addSubLocation                                        (LocationId locationId, string &ipAddress, SI32 port);
                void                         configureThisLocationAsSubLocation                    (LocationId parentLocationId, string &parentIpAddress, SI32 parentPort, LocationId locationId, string &ipAddress, SI32 port);
                void                         initializeServerCommunications                        ();
                void                         initializeClientCommunications                        ();
                bool                         acceptNewConnection                                   (ServerStreamingSocket &newSocket);
                void                         disconnectFromLocation                                (LocationId locationId, const bool &closePeerServerSocket = true);
                void                         disconnectFromLocation                                (const string &ipAddress, const SI32 &port, const bool &closePeerServerSocket = true);
                void                         removeKnownLocation                                   (const LocationId &locationId);
                ResourceId                   connectToLocation                                     (const string &ipAddress, const SI32 &port);
                ResourceId                   upgradeDatabase                                       ();
                void                         startThePrismServices                                 ();
        static  void                         waitForPrismServicesToFinish                          ();
        static  void                         setFrameworkSequenceType                              (FrameworkSequenceType frameworkSequenceType);
        static  bool                         getIsInstantiated                                     ();
        static  FrameworkSequenceGenerator  &getCurrentFrameworkSequenceGenerator                  ();
        static  void                         setConfigurationFile                                  (const string &configurationFile);
        static  void                         setGlobalConfigurationFile                            (const string &globalConfigurationFile);
        static  void                         setLockFileForConfigurationFile                       (const string &lockFile);
        static  string                       getConfigurationFileName                              ();
        static  string                       getConfigurationBackupFileName                        ();
        static  string                       getGlobalConfigurationFileName                        ();
        static  string                       getGlobalConfigurationTempFileName                    ();
        static  string                       getLockFileForConfigurationFile                       ();
                ResourceId                   saveConfiguration                                     (const bool &syncToStandby);
                ResourceId                   changePrismConfigurationValidity                      (const bool &validity ); 
        static  void                         configureAsLocation                                   (SI32 port);
        static  void                         configureAsSubLocation                                (LocationId parentLocationId, string &parentIpAddress, SI32 parentPort, LocationId locationId, string &ipAddress, SI32 port);
                void                         resetLocationToPrimary                                ();

        static  void                         acquireBootSynchronizationMutex                       ();
        static  void                         releaseBootSynchronizationMutex                       ();

                void                         dynamicallyExcludeServiceFromClusterCommunications    (const PrismServiceId &prismServiceId);
                bool                         isServiceDynamicallyExcludedFromClusterCommunications (const PrismServiceId &prismServiceId);

        static  void                         excludeServiceForClusterValidationPhase               (const PrismServiceId &prismServiceId);

        static  void                         setIpAddressForThisLocation                           (const string &ipAddressForThisLocation);
        static  string                       getIpAddressForThisLocation                           ();

        static  void                         updateIpAddressForThisLocation                        (const string &ipAddressForThisLocation);

        static  void                         setEthernetInterfaceForThisLocation                   (const string &ethernetInterfaceForThisLocation);
        static  string                       getEthernetInterfaceForThisLocation                   ();

                void                         setIsStartupValid                                     (const bool &isStartupValid);
                bool                         getIsStartupValid                                     ();
                void                         setStartupFileName                                    (const string &startupFileName);
                string                       getStartupFileName                                    () const;
                void                         setStartupFileType                                    (const ResourceId &startupFileType);
                ResourceId                   getStartupFileType                                    () const;

                void                         newPrincipalSelectedAfterFaioverMessageHandler        (FrameworkObjectManagerNewPrincipalEstablishedMessage *pFrameworkObjectManagerNewPrincipalEstablishedMessage);
                void                         validateNewPrincipalSelection                         (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                         processNewPrincipalEstablishedMessage                 (PrismLinearSequencerContext *pPrismLinearSequencerContext);

                void                         secondaryNodeFailureNotificationMessageHandler        (FrameworkObjectManagerSecondaryNodeFailureNotificationMessage *pFrameworkObjectManagerSecondaryNodeFailureNotificationMessage);
                void                         processSecondeyNodeFailureMessage                     (PrismLinearSequencerContext *pPrismLinearSequencerContext);


                bool                         getSecondaryNodeClusterCreationFlag                   ();
                void                         setSecondaryNodeClusterCreationFlag                   (bool secondaryNodeClusterCreationFlag);

                bool                         getPrimaryNodeClusterOperationFlag                    ();
                void                         setPrimaryNodeClusterOperationFlag                    (bool primaryNodeClusterOperationFlag);

                void                         resumePostponedMessages                               ();

                void                         disconnectFromAllNodesHandler                         (FrameworkObjectManagerDisconnectFromAllNodesMessage *pFrameworkObjectManagerDisconnectFromAllNodesMessage);
                void                         disconnectAllKnownLocationStep                        (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                         establishPrincipalAfterClusterRebootHandler           (FrameworkObjectManagerEstablishPrincipalAfterClusterRebootMessage* pFrameworkObjectManagerEstablishPrincipalAfterClusterRebootMessage);
                void                         establishPrincipalAfterClusterRebootStep              (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                 
                void                         invalidateClientStreamingSocketForRemoteLocation      (LocationId locationId);
        static  void                         disconnectFromAllKnownLocations                       (); 
                void                         disconnectFromAllInstanceClientsHandler 			   (FrameworkObjectManagerDisconnectFromAllInstanceClientsMessage *pFrameworkObjectManagerDisconnectFromAllInstanceClientsMessage);
                void                         disconnectAllInstanceClientsStep                      (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                         prepareNodeForWarmRecoveryWithDefaultConfiguration    (FrameworkObjectManagerPrepareForWarmRecoveryWithDefaultConfiguration *pFrameworkObjectManagerPrepareForWarmRecoveryWithDefaultConfiguration);
        static  void                         setGetInstancesFunction                               (GetInstancesFunction getInstancesFunction);
        static  void                         setGetThisSlotInstanceFunction                        (GetThisSlotInstanceFunction getThisSlotInstanceFunction);
        static  void                         setGetInstancesAndSubInstancesFunction                (GetInstancesAndSubInstancesFunction getInstancesAndSubInstancesFunction);
        static  void                         setCreateDummySlotFunction                            (CreateDummySlotFunction createDummySlotFunction);
        static  void                         setRaslogAssertFunction                               (RaslogAssertFunction raslogAssertFunction);

        static  void                         setGetFirmwareVersionFunction                         (GetFirmwareVersionFunction getFirmwareVersionFunction);
        static  void                         setHandleDcmFssConfigUpdateFunction                   (HandleDcmFssConfigUpdateFunction handleDcmFssConfigUpdateFunction);
        static  void                         setSetVcsClusterConfigHandlerFunction                 (SetVcsClusterConfigHandlerFunction setVcsClusterConfigHandlerFunction);
        static  void                         setGetVcsClusterConfigHandlerFunction                 (GetVcsClusterConfigHandlerFunction getVcsClusterConfigHandlerFunction);
        static  void                         setNotifySyncUpdateFailureFunction                    (NotifySyncUpdateFailureFunction notifySyncUpdateFailureFunction);
                bool                         notifySyncUpdateFailure                               (const ResourceId &reason);
        static  void                         setNotifyClusterReadyStateFunction                    (NotifyClusterReadyStateFunction notifyClusterReadyStateFunction);
                void                         notifyClusterReadyState                               (bool &readyState);
                bool                         getNeedNotifyClusterReadyState                        ();
                void                         setNeedNotifyClusterReadyState                        (bool needNotifyClusterReadyState);

                /** 
                * returns if FIPSZeroize flag value. This flag is set when FIPSZeroize message is received. 
                */
                bool                         isNodeZeroized                                        ();
                
                /** FIPSZeroize
                *  Handler for FIPSZeroize
                */
                void                         zeroizeForFIPSMessageHandler                          (ZeroizeForFIPSMessage*);
                void                         validateStandaloneStep                                (ZeroizeForFIPSLinearSequencerContext *pContext);
                void                         notifyAllClientSessionsStep                           (ZeroizeForFIPSLinearSequencerContext *pContext);
                void                         triggerFIPSZeroizeStep                                (ZeroizeForFIPSLinearSequencerContext *pContext);
                void                         zeroizeSuccessStep                                    (ZeroizeForFIPSLinearSequencerContext *pZeroizeForFIPSLinearSequencerContext);

         static string                       getFIPSZeroizeFile                                    ();
                void                         validateAndZeroizeAtBoot                              ();

                void                         setIsDBRestoreIncomplete                              (bool dbRestoreIncomplete);
                bool                         getIsDBRestoreIncomplete                              ();
                
                void                         setDbConversionStatus                                 (ResourceId dbConversionStatus);
                ResourceId                   getDbConversionStatus                                 () const;

                void                         rollbackCfgFile                                       ();
                string                       getFirmwareVersion                                    ();
                int                          handleDcmFssConfigUpdate                              (void *dcmConfigData);
                ResourceId                   setVcsClusterConfigData                               (VcsClusterConfiguration *pDcmVcsClusterConfigData, const UI32 &context);
                void                         getVcsClusterConfigData                               (VcsClusterConfiguration *pDcmVcsClusterConfigData, const UI32 &context);
                bool                         getIsSyncDumpCompleted                                ();
		static	bool						 isExternalStateSynchronizationRequired (PrismServiceId prismServiceId);
		static	void						 addToExternalStateSynchronizationRequiredList (PrismServiceId prismServiceId);
                UI32                         getSyncState                                          ();
                void                         setSyncState                                          (const UI32& syncState);
                UI32                         getStandbySyncState                                   ();
        static void                          addPrismThreadId                                      (const PrismThreadId &prismThreadId);
        static void                          removePrismThreadId                                   (const PrismThreadId &prismThreadId);
        static void                          getAllPrismThreads                                    (map<PrismThreadId, PrismThreadId> &prismThreadsMap);
        static void                          deleteAllPrismThreads                                 ();
                bool                         getPrismConfigurationValidity                         ();
               ClientStreamingSocket*        getClientStreamingSocketForRemoteLocation             (LocationId locationId);
        static  SI32                         getLocationPort                                       ();
                void                         resetFrameworkConfigurationToDefault                  ();
    // Now the data members

    private :
               PrismFrameworkObjectManagerInitializeWorker     *m_pInitializeWorker;
               PrismFinalizeWorker                             *m_pPrismFinalizeWorker;
               PrismFrameworkConfigurationWorker               *m_pConfigurationWorker;
               PrismFrameworkFailoverWorker                    *m_pPrismFrameworkFailoverWorker;
               PrismFrameworkObjectManagerServiceControlWorker *m_pPrismFrameworkObjectManagerServiceControlWorker;
               PrismFrameworkObjectManagerPostbootWorker       *m_pPrismFrameworkObjectManagerPostbootWorker;
               PrismFrameworkObjectManagerPostPersistentBootWorker *m_pPrismFrameworkObjectManagerPostPersistentBootWorker;
               PrismFrameworkObjectManagerHaSyncWorker         *m_pPrismFrameworkObjectManagerHaSyncWorker;
               LocationBase                                    *m_pThisLocation;
               LocationId                                       m_lastUsedLocationId;

               map<PrismServiceId, PrismServiceId>              m_servicesToBeExcludedForClusterCommunications;
               PrismMutex                                       m_servicesToBeExcludedForClusterCommunicationsLock;

		static vector<PrismServiceId>                           m_externalStateSynchronizationRequiredList;
		static PrismMutex										m_externalStateSynchronizationRequiredListLock;

        static string                                           m_ipAddressForThisLocation;
        static PrismMutex                                       m_ipAddressForThisLocationMutex;

        static string                                           m_ethernetInterfaceForThisLocation;

               bool                                             m_isStartupValid;
               string                                           m_startupFileName;
               ResourceId                                       m_startupFileType;
               bool                                             m_secondaryNodeClusterCreationFlag;
               PrismMutex                                       m_secondaryNodeClusterCreationFlagMutex;

               bool                                             m_primaryNodeClusterOperationFlag;
               PrismMutex                                       m_primaryNodeClusterOperationFlagMutex;

               bool                                             m_activationCompleted;
               string                                           m_activationCompletedTimeStamp;
               PrismMutex                                       m_activationCompletedMutex;

               //Status related Port boot progress
               bool                                             m_postBootStarted;
               string                                           m_postBootStartedTimeStamp;
               bool                                             m_postBootCompleted;
               string                                           m_postBootCompletedTimeStamp;
               PrismMutex                                       m_postBootProgressMutex;
               UI32                                             m_numberOfLineCardPostBootInProgress;
               PrismMutex                                       m_lineCardPostBootMutex;

               bool                                             m_configReplayStarted;
               string                                           m_configReplayStartedTimeStamp;
               bool                                             m_configReplayCompleted;
               string                                           m_configReplayCompletedTimeStamp;
               PrismMutex                                       m_configReplayInProgressMutex;

               bool                                             m_fileReplayStarted;
               string                                           m_fileReplayStartedTimeStamp;
               bool                                             m_fileReplayCompleted;
               string                                           m_fileReplayCompletedTimeStamp;
               PrismMutex                                       m_fileReplayInProgressMutex;

               bool                                             m_nodeReadyForAllCommands;
               string                                           m_nodeReadyForAllCommandsTimeStamp;
               PrismMutex                                       m_nodeReadyForAllCommandsMutex;
               TimerHandle                                      m_clusterPhaseTimerHandler;

               bool                                             m_isPostBootNeededDuringRejoin;
               bool                                             m_needNotifyClusterReadyState;

               PrismMutex                                       m_nodeZeroizedMutex;
               /** FIPSZeroize
               *  FIPSZeroize Flag
               */
               bool                                             m_nodeZeroized;
               bool                                             m_isDBRestoreIncomplete;
               ResourceId                                       m_dbConversionStatus;
               ResourceId                                       m_tableIdsChangedForSchemaCompatibility;

               vector<string>                                   m_oldAuxilliaryTableNamesBeforeConversion;
               vector<string>                                   m_newAuxilliaryTableNamesAfterConversion;
               vector<string>                                   m_parentTableNamesForAuxilliaryTables;
               vector<string>                                   m_relatedToTableNamesForAuxilliaryTables;

               bool                                             m_isWarmHaRecoveryPreparationInProgress;
               PrismMutex                                       m_warmHaRecoveryPreparationInProgressMutex;


        static GetInstancesFunction                             m_getInstancesFunction;
        static GetInstancesAndSubInstancesFunction              m_getInstancesAndSubInstancesFunction;
        static GetThisSlotInstanceFunction                      m_getThisSlotInstanceFunction;
        static CreateDummySlotFunction                          m_createDummySlotFunction;
        static RaslogAssertFunction                             m_raslogAssertFunction;
        static GetFirmwareVersionFunction                       m_getFirmwareVersionFunction;
        static HandleDcmFssConfigUpdateFunction                 m_handleDcmFssConfigUpdateFunction;
        static SetVcsClusterConfigHandlerFunction               m_setVcsClusterConfigHandlerFunction;
        static GetVcsClusterConfigHandlerFunction               m_getVcsClusterConfigHandlerFunction;
        static NotifySyncUpdateFailureFunction                  m_notifySyncUpdateFailureFunction;
        static NotifyClusterReadyStateFunction                  m_notifyClusterReadyStateFunction;
    protected :
    public :

    friend class PrismFrameworkThread;
    friend class WaveObjectManager;
    friend class FrameworkToolKit;
    friend class PrismFrameworkObjectManagerInitializeWorker;
    friend class PersistentPrismBootAgent;
    friend class HaStandbyPrismBootAgent;
    friend class PersistentWithDefaultPrismBootAgent;
    friend class PersistentWithDefaultForHABootAgent;
    friend class PrismFailoverAgent;
    friend class PrismPostbootAgent;
    friend class RecoverPrismBootAgent;
    friend class PrismFrameworkObjectManagerPostPersistentBootWorker;
    friend class PrismFrameworkObjectManagerHaSyncWorker;
    friend class QueryUtils;
    friend class PrismFrameworkConfigurationWorker;

    friend void                          trace              (TraceLevel traceLevel, const string &stringToTrace);
    friend void                          tracePrintf        (TraceLevel traceLevel, const bool &addNewLine, const bool &suppressPrefix, const char * const pFormat, ...);
    friend void                          tracePrintf        (TraceLevel traceLevel, const char * const pFormat, ...);

    friend void                          prismAssert        (bool isAssertRequired, const char *pFileName, UI32 lineNumber);
};

}

#endif //PRISMFRAMEWORKOBJECTMANAGER_H
