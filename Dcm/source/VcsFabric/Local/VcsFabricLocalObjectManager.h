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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vipool Prajapati                                             *
 ***************************************************************************/

#ifndef VCSFABRICLOCALOBJECTMANAGER_H
#define VCSFABRICLOCALOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveLocalObjectManager.h"
#include "VcsFabric/Local/VcsNodeLocalManagedObject.h"
#include "VcsFabric/Local/VcsNodeVcsIdMessages.h"
#include "VcsFabric/Local/VcsNodeRBridgeIdMessages.h"
#include "VcsFabric/Local/VcsNodeDomainIdMessages.h"
#include "VcsFabric/Local/VcsNodeCopyDefaultStartMessages.h"
#include "VcsFabric/Local/VcsNodeConfigDistributeMessage.h"
#include "VcsFabric/Local/VcsNodeMappedIdMessages.h"
#include "VcsFabric/Local/VcsNodeSwitchMacMessages.h"
#include "VcsFabric/Local/VcsNodeFcfMacMessages.h"
#include "VcsFabric/Local/VcsNodeVcsModeMessages.h"
#include "VcsFabric/Local/VcsNodeLocalMessagingContext.h"
#include "VcsFabric/Local/VcsEnqSwitchMessage.h"
#include "VcsFabric/Local/VcsClusterMergeMessage.h"
#include "VcsFabric/Local/VcsNodeLocalTypes.h"
#include "VcsFabric/Local/VcsNodeLocalLinecardTypes.h"
#include "VcsFabric/Local/VcsNodeGenerateSCNMessage.h"
#include "VcsFabric/Local/VcsNodePrincipalMessages.h"
#include "VcsFabric/Local/VcsNodeFabosDomainEvents.h"
#include "VcsFabric/Local/VcsNodeFabosPortEvents.h"
#include "VcsFabric/Local/VcsNodeFabosSlotEvents.h"
#include "VcsFabric/Local/VcsNodeFabosSystemEvents.h"
#include "VcsFabric/Local/VcsNodeIsReadyMessages.h"
#include "VcsFabric/Local/VcsNodeIsDcmdReadyForHasmLaunchMessages.h"
#include "VcsFabric/Local/VcsNodeIsHasmLaunchedMessages.h"
#include "VcsFabric/Local/VcsNodePostBootMessages.h"
#include "VcsFabric/Local/VcsNodeWWNMessages.h"
#include "VcsFabric/Local/VcsNodeIsNodeInClusterMessage.h"
#include "VcsFabric/Local/VcsNodeSetClusterConfigurationMessage.h"
#include "VcsFabric/Local/VcsNodeIsLocalNodeInClusterMessage.h"
#include "VcsFabric/Local/VcsNodeIpMessages.h"
#include "VcsFabric/Local/VcsNodeSegmentSwitchMessage.h"
#include "VcsFabric/Local/VcsIsIpReachableFromNodeMessage.h"
#include "VcsFabric/Local/VcsNodeGenerateSlotScnMessage.h"
#include "VcsFabric/Local/VcsNodeSetModeChangeParametersMessage.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForExternalStateSynchronization.h"
#include "VcsFabric/Local/VcsNodeSlotMessages.h"
#include "Framework/Boot/BootTypes.h"
#include "Framework/Boot/BootCompleteForThisLocationEvent.h"
#include "Framework/Boot/HaBootCompleteForThisLocationEvent.h"
#include "Framework/Core/WaveNodeLocalNodeDeletedEvent.h"
#include "Framework/Core/WaveNodeDeleteClusterEvent.h"
#include "Framework/Core/WaveNewNodesAddedEvent.h"
#include "Framework/Core/ClusterPhase3StartEvent.h"
#include "Framework/Core/ClusterPhase3CompleteEvent.h"
#include "Framework/Core/FrameworkObjectManagerTypes.h"
#include "Framework/Core/FrameworkObjectManagerMessages.h"
#include "Framework/Core/VcsClusterConfiguration.h"
#include "Framework/Postboot/PrismPostPersistentBootMessages.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Framework/Core/PostBootStartedEvent.h"
#include "Framework/Core/PostBootCompletedEvent.h"
#include "Framework/Core/ConfigReplayStartedEvent.h"
#include "Framework/Core/ConfigReplayCompletedEvent.h"
#include "Framework/Core/LastConfigReplayCompletedEvent.h"
#include "Framework/Core/PrimaryChangedEvent.h"
#include "VcsFabric/Local/VcsNodeGetConfigurationNumberMessage.h"
#include "VcsFabric/Local/VcsNodeGetConfigurationTimeMessage.h"
#include "VcsFabric/Local/VcsFabricLocalMessages.h"
#include "VcsFabric/Local/VcsFabricLocalManagementInterfaceMessages.h"
#include "VcsFabric/Global/VcsClusterManagedObject.h"
#include "VcsFabric/Global/VcsFabMessagingContext.h"
//#include "FabricService/Local/FabricServiceDisableIslsToNeighborMessage.h"
#include "VcsFabric/Local/VcsNodeIsFabricDistributionEnabledMessage.h"
#include "VcsFabric/Local/VcsNodeSwitchInfoMessages.h"
#include "VcsFabric/Local/VcsNodeTestClientForDualConnectMessage.h"
#include "VcsFabric/Local/VcsNodeHaMessages.h"
#include "VcsFabric/Local/VcsNodeCliAllowedFromThisLocationMessage.h"
#include "VcsFabric/Local/VcsNodeIsCliAllowedFromThisLocationMessage.h"
#include "VcsFabric/Local/VcsRemoveStandbyCfgFileMessage.h"
#include <map>
#include "VcsFabric/Local/VcsNodeGvlanMessages.h"

using namespace WaveNs;
using namespace std;

namespace DcmNs
{

class VcsNodePrincipalSelectionWorker;
class VcsNodeFCPrincipalTrackingWorker;
class VcsNodeSlotManagementWorker;
class VcsCopyToStartupWorker;
class VcsNodeConfigSnapshotWorker;
class HaCacheReloadEvent;
class VcsFabricDebugClientMessage;
class VcsFabricCreateMessage;
class VcsNodeInterDcmCcmConfigDistributeMessage;
class VcsFabricSetTtyNameMessage; 
class VcsNodeConfigDistributeContext;
class VcsNodeClusterInfoWorker;

class VcsFabricLocalObjectManager : public WaveLocalObjectManager
{
    private :
                                    VcsFabricLocalObjectManager                         ();
        PrismMessage               *createMessageInstance                               (const UI32 &operationCode);
        virtual WaveManagedObject  *createManagedObjectInstance                         (const string &managedClassName);
        void                        install                                             (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
        void                        hainstall                                           (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
        void                        boot                                                (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
        void                        initialize                                          (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
        void                        listenForEvents                                     (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);

        void                        shutdown                                            (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases);
        virtual void                disable                                             (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases);
        virtual void                zeroize                                             (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases);
        void                        setThisVcsNodeObjectId                              (const ObjectId &thisVcsNodeObjectId);
        void                        setThisVcsNodeSerialNumber                          (const string &thisVcsNodeSerialNumber);
        string                      getThisVcsNodeSerialNumber                          ();
        void                        setThisVcsNodeSwitchMac                             (const MacAddress &thisVcsNodeSwitchMac);
        MacAddress                  getThisVcsNodeSwitchMac                             ();
        void                        setThisVcsNodeSwitchWWN                             (const WorldWideName &thisVcsNodeSwitchWWN);
        WorldWideName               getThisVcsNodeSwitchWWN                             ();
        static void                 parseVcsConfFile                                    (bool &vcsMode, UI32 &vcsId);
        static void                 setThisVcsNodeClusterType                           (const bool &vcsMode, const bool &clusterMode);
        static int                  getThisVcsNodeSWBDModel                             ();
        void                        triggerPostBoot                                     (UI32 eventId, UI32 parameter, ResourceId recoveryType);
        void                        setThisVcsNodeSwitchFwVersion                       (const string &fwVersion);
        string                      getThisVcsNodeSwitchFwVersion                       ();
        ResourceId                  getLocalNodeSwitchname                              (string &switchname);
        void                        setThisVcsNodeBootReason                            (const WaveBootReason &thisVcsNodeBootReason);
        WaveBootReason              getThisVcsNodeBootReason                            ();
        void                        setThisVcsNodeSwmodelName                           (const string &thisVcsNodeSwmodelName);
        string                      getThisVcsNodeSwmodelName                           ();

        // message handlers
        void                        getVcsIdMessageHandler                              (VcsNodeGetVcsIdMessage *pVcsNodeGetVcsIdMessage);
        ResourceId                  getVcsIdStep                                        (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext);

        void                        setVcsIdMessageHandler                              (VcsNodeSetVcsIdMessage *pVcsNodeSetVcsIdMessage);
        ResourceId                  setVcsIdStep                                        (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext);
        
        void                        setRBridgeIdMessageHandler                          (VcsNodeSetRBridgeIdMessage *pVcsNodeSetRBridgeIdMessage);
        ResourceId                  setRBridgeIdStep                                    (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext);
        ResourceId                  setRBridgeIdBackendStep                             (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext);
        ResourceId                  setRBridgeIdBackend                                 (const UI32 & rbridgeId);

        void                        getRBridgeIdMessageHandler                          (VcsNodeGetRBridgeIdMessage *pVcsNodeGetRBridgeIdMessage);
        ResourceId                  getRBridgeIdStep                                    (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext);

        void                        setDomainIdMessageHandler                           (VcsNodeSetDomainIdMessage *pVcsNodeSetDomainIdMessage);
        ResourceId                  setDomainIdStep                                     (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext);

        void                        vcsFabricModeChangeParametersMessageHandler         (VcsNodeSetModeChangeParametersMessage *pVcsNodeSetModeChangeParametersMessage);
        void                        copyDefaultStartTriggerMessageHandler               (VcsNodeCopyDefaultStartTriggerMessage *pVcsNodeCopyDefaultStartTriggerMessage);

        ResourceId                  getFilteredConfigData                               (ResourceId configFilterContextType, string& filteredData);
        ResourceId                  saveFilteredConfigData                              (ResourceId configFilterContextType, const string& contextFilteredConfigData);
        bool                        determineIsDefaultConfigDetected                    ();

        void                        configDistributeMessageHandler                      (VcsNodeConfigDistributeMessage *pVcsNodeConfigDistributeMessage);
        ResourceId                  retrieveGlobalConfigAndSendReplyStep                (VcsNodeConfigDistributeContext *pVcsNodeConfigDistributeContext);
        ResourceId                  setupForGlobalOnlyConfigStep                        (VcsNodeConfigDistributeContext *pVcsNodeConfigDistributeContext);
        ResourceId                  validateGlobalOnlyConfigStep                        (VcsNodeConfigDistributeContext *pVcsNodeConfigDistributeContext);
        ResourceId                  setupForLocalOnlyConfigStep                         (VcsNodeConfigDistributeContext *pVcsNodeConfigDistributeContext);
        ResourceId                  retrieveContextFilteredConfigStep                   (VcsNodeConfigDistributeContext *pVcsNodeConfigDistributeContext);
        ResourceId                  saveContextFilteredConfigToFileStep                 (VcsNodeConfigDistributeContext *pVcsNodeConfigDistributeContext);

        void                        vcsNodeInterDcmCcmConfigDistributeMessageHandler    (VcsNodeInterDcmCcmConfigDistributeMessage *pVcsNodeInterDcmCcmConfigDistributeMessage);
        ResourceId                  doInterDcmCcmConfigDistributeStep                   (VcsNodeConfigDistributeContext *pVcsNodeConfigDistributeContext);
        ResourceId                  replyInterDcmCcmConfigDistributeStep                (VcsNodeConfigDistributeContext *pVcsNodeConfigDistributeContext);
        ResourceId                  prepareForDefaultConfigStep                         (VcsNodeConfigDistributeContext *pVcsNodeConfigDistributeContext);
        ResourceId                  replyPrimaryConfigDistributeStep                    (VcsNodeConfigDistributeContext *pVcsNodeConfigDistributeContext);
        ResourceId                  sendUnconfigureClusterSecondaryStep                 (VcsNodeConfigDistributeContext *pVcsNodeConfigDistributeContext);
        ResourceId                  setSwitchDeletePrimaryAfterControlledFailover       (VcsNodeConfigDistributeContext *pVcsNodeConfigDistributeContext);

        void                        getIsPrincipalMessageHandler                        (VcsNodeGetIsPrincipalMessage *pVcsNodeGetIsPrincipalMessage);
        ResourceId                  getIsPrincipalStep                                  (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext);

        void                        setIsPrincipalMessageHandler                        (VcsNodeSetIsPrincipalMessage *pVcsNodeSetIsPrincipalMessage);
        ResourceId                  setIsPrincipalStep                                  (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext);

        void                        setFcfMacMessageHandler                             (VcsNodeSetFcfMacMessage *pVcsNodeSetVcsIdMessage);
        ResourceId                  setFcfMacStep                                       (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext);

        void                        getSwitchMacMessageHandler                          (VcsNodeGetSwitchMacMessage *pVcsNodeGetSwitchMacMessage);
        ResourceId                  getSwitchMacStep                                    (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext);

        void                        getLocalMappedIdMessageHandler                      (VcsNodeGetLocalMappedIdMessage *pVcsNodeGetLocalMappedIdMessage);
        ResourceId                  getMappedIdStep                                     (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext);
        ResourceId                  getLocalMappedIdStep                                (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext);

        void                        getVcsModeMessageHandler                            (VcsNodeGetVcsModeMessage *pVcsNodeGetVcsIdMessage);
        ResourceId                  getVcsModeStep                                      (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext);

        void                        setVcsModeMessageHandler                            (VcsNodeSetVcsModeMessage *pVcsNodeSetVcsModeMessage);

        void                        getIsReadyMessageHandler                            (VcsNodeGetIsReadyMessage *pVcsNodeGetIsReadyMessage);
        ResourceId                  getIsReadyStep                                      (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext);

        void                        setIsReadyMessageHandler                            (VcsNodeSetIsReadyMessage *pVcsNodeSetIsReadyMessage);
        ResourceId                  setIsReadyStep                                      (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext);

        void                        getSwitchWWNMessageHandler                          (VcsNodeGetSwitchWWNMessage *pVcsNodeGetSwitchWWNMessage);
        ResourceId                  getSwitchWWNStep                                    (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext);

        void                        setSwitchWWNMessageHandler                          (VcsNodeSetSwitchWWNMessage *pVcsNodeSetSwitchWWNMessage);
        ResourceId                  setSwitchWWNStep                                    (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext);
        ResourceId                  switchStatusValidationStep                          (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext);
        ResourceId                  setErrorStringForIncorrectRbridgeIdStep             (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext);

        void                        getIsDcmdReadyForHasmLaunchMessageHandler           (VcsNodeGetIsDcmdReadyForHasmLaunchMessage *pVcsNodeGetIsDcmdReadyForHasmLaunchMessage); 
        ResourceId                  getIsDcmdReadyForHasmLaunchStep                     (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext);
        
        void                        setIsHasmLaunchedMessageHandler                     (VcsNodeSetIsHasmLaunchedMessage *pVcsNodeSetIsrHasmLaunchedMessage); 
        ResourceId                  setIsHasmLaunchedStep                               (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext);

        void                        vcsIsNodeInClusterMessageHandler                    (VcsNodeIsNodeInClusterMessage  *pVcsNodeIsNodeInClusterMessage);
        static void                 vcsIsLocalNodeInClusterStaticMessageHandler         (VcsNodeIsLocalNodeInClusterMessage  *pVcsNodeIsLocalNodeInClusterMessage);
        void                        vcsIsLocalNodeInClusterMessageHandler               (VcsNodeIsLocalNodeInClusterMessage  *pVcsNodeIsLocalNodeInClusterMessage);
        void                        vcsIsFabricDistributionEnabledMessageHandler        (VcsNodeIsFabricDistributionEnabledMessage *pVcsNodeIsFabricDistributionEnabledMessage);

        void                        getClusterTypeMessageHandler                        (VcsNodeGetClusterTypeMessage *pVcsNodeGetClusterTypeMessage);
        static void                 getClusterTypeStaticMessageHandler                  (VcsNodeGetClusterTypeMessage *pVcsNodeGetClusterTypeMessage);

        void                        vcsModeEnableMessageHandler                         (VcsNodeSetVcsModeMessage *pVcsNodeSetVcsModeMessage);
        void                        vcsModeDisableMessageHandler                        (VcsNodeSetVcsModeMessage *pVcsNodeSetVcsModeMessage);
        void                        vcsNodeEnableValidationHandler                      (VcsNodeEnableValidationMessage *pVcsNodeEnableValidationMessage);
        void                        vcsClusterMergeMessageHandler                       (VcsClusterMergeMessage * pVcsClusterMergeMessage);
        void                        vcsFabricSetClusterConfigurationMessageHandler      (VcsNodeSetClusterConfigurationMessage *pVcsNodeSetClusterConfigurationMessage);
        void                        vcsRemoveCfgFileOnStandbyMessageHandler             (VcsRemoveStandbyCfgFileMessage *pVcsRemoveStandbyCfgFileMessage);
        void                        syncConfigToKernelOnStandbyMessageHandler           (VcsNodeHaSyncConfigToKernelMessaage *pVcsNodeHaSyncConfigToKernelMessaage);
        static void                 vcsRemoveCfgFileOnStandbyMessageStaticHandler       (VcsRemoveStandbyCfgFileMessage *pVcsRemoveStandbyCfgFileMessage);

        ResourceId                  haSyncUpdateAtLocalNodeStep                         (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
        ResourceId                  haSyncUpdateAtLocalNode                             ();

        ResourceId                  validateSetClusterConfigurationStep                 (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext);
        ResourceId                  savePreviousConfigurationStep                       (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext);
        ResourceId                  changeClusterTypeAtLocalNodeStep                    (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext);
        ResourceId                  prepareForSetClusterConfigurationStep               (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext);
        ResourceId                  vcsFabricSetClusterConfigurationFailedStep          (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext);
        ResourceId                  vcsFabricSetClusterConfigurationCommitTransactionStep(VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext);
        ResourceId                  prepareForSetVcsModeStep                            (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext);
        ResourceId                  vcsEnableValidationStep                             (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext);
        ResourceId                  vcsDisableValidationStep                            (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext);
        ResourceId                  copyDefaultToStartInVcsModeDisableStep              (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext);
        ResourceId                  copyDefaultToStartInVcsModeEnableStep               (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext);
        ResourceId                  copyDefaultToStartInModeTransitionStep              (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
        ResourceId                  copyDefaultToStartInModeChange                      (const bool & vcsMode, const bool & reverseVcsModeOnFailure, string & retMessage);
        ResourceId                  copyDefaultToStartStep                              (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext);
        //ResourceId                  copyDefaultToStart                                (const bool & vcsMode);

        ResourceId                  removeCcmdCfgOnStandbyStep                          (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
        void                        sendHaSyncUpdateMessageToCcmd                       (const UI32 &context);
        ResourceId                  sendCcmdSyncUpdateMessage                           (const UI32 &context);    

        ResourceId                  rebootLocalNodeStep                                 (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
        ResourceId                  rebootLocalNode                                     (const bool &isStandbyRebootOnly);
        ResourceId                  setupConfigAndRebootSecondariesStep                 (VcsNodeConfigDistributeContext *pVcsNodeConfigDistributeContext);
        ResourceId                  setupConfigAndRebootPrimaryStep                     (VcsNodeConfigDistributeContext *pVcsNodeConfigDistributeContext);
        ResourceId                  vcsDisableStartTransactionStep                      (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext);
        ResourceId                  vcsDisableCommitTransactionStep                     (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext);
        ResourceId                  prepareForVcsModeDisableStep                        (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext);
        ResourceId                  unconfigurePrimaryStep                              (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext);
        ResourceId                  unconfigureSecondaryStep                            (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext);
        ResourceId                  disableVcsModeStep                                  (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext);
        ResourceId                  setConfFilesStep                                    (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext);
        ResourceId                  vcsModeEnableRunScript                              (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext);
        ResourceId                  runVcsModeEnableScript                              ();

        ResourceId                  setVcsConfModeStep                                  (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext);
        ResourceId                  setRbridgeIdModeStep                                (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext);
        ResourceId                  setClusterConfigModeStep                            (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext);
        ResourceId                  setStartupConfigModeStep                            (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext);
        ResourceId                  deleteConfigFilesStep                               (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext);

        ResourceId                  vcsModeDisableRunScript                             (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext);
        ResourceId                  runVcsModeDisableScript                             ();
        ResourceId                  setVcsModeStep                                      (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext);
        ResourceId                  setVcsConfStep                                      (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext);
        ResourceId                  reverseVcsMode                                      (const bool &vcsMode);
        ResourceId                  sendRemoveSwitchMessage                             (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext);
    
        ResourceId                  setSwitchDeleteAfterFailover                        (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext);
        ResourceId                  copyDefaultToStartAtLocalNode                       (bool vcs_mode);
        ResourceId                  setVcsIdVcsConfStep                                 (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext);
        ResourceId                  setVcsIdVcsConfModeChangeStep                       (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext);
        ResourceId                  setVcsConfFile                                      (bool vcs_mode, int vcs_id);
        ResourceId                  disconnectPrincipalFromAllLocationStep              (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext);

        void                        enqSwitchMessageHandler                             (VcsEnqSwitchMessage *pEnqSwitchMessage);
        ResourceId                  changeClusterTypeConfigDistributeStep               (VcsNodeConfigDistributeContext *pVcsNodeConfigDistributeContext);
        ResourceId                  setStartupConfig                                    (bool preserveConfig); 
        ResourceId                  copyRunningToStart                                  (); 
        ResourceId                  validateClusterOperationInProgress                  (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext);
        ResourceId                  getSwitchParamsStep                                 (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext);
		ResourceId 					validateSwitchReplacementCaseStep                   (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext);
		void 						prismLinearSequencerFailedStepForEnqSwitchMessage (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);

        void                        generateSCNMessageHandler                           (VcsNodeGenerateSCNMessage *pVcsNodeGenerateSCNMessage);
        void                        generateSlotScnMessageHandler                       (VcsNodeGenerateSlotScnMessage *pVcsNodeGenerateSlotScnMessage);
        ResourceId                  notifyStartSyncDumpHandler                          (const string& peerHaIpAddress, const SI32 peerHaIpPort, const UI32 eventId);
        void                        syncDumpStartEventHandler                           (const VcsNodeFabosSyncStartEvent *&pVcsNodeFabosSyncStartEvent);
        void                        bootCompleteEventHandler                            (const BootCompleteForThisLocationEvent *&pBootCompleteForThisLocationEvent);
        void                        habootCompleteEventHandler                          (const HaBootCompleteForThisLocationEvent *&pHaBootCompleteForThisLocationEvent);

        void                        domainValidEventHandler                             (const VcsNodeFabosDomainValidEvent *&pEvent);
        void                        domainCapabilitiesAvailableEventHandler             (const VcsNodeFabosDomainCapabilitiesAvailableEvent *&pEvent);
        void                        domainInvalidEventHandler                           (const VcsNodeFabosDomainInvalidEvent *&pEvent);
        void                        domainReachableEventHandler                         (const VcsNodeFabosDomainReachableEvent *&pEvent);
        void                        domainUnreachableEventHandler                       (const VcsNodeFabosDomainUnreachableEvent *&pEvent);

        void                        switchOfflineEventHandler                           (const VcsNodeFabosSwitchOfflineEvent *&pEvent);
        void                        zoneChangedEventHandler                             (const VcsNodeFabosZoneChangedEvent *&pEvent);
        void                        zoneAbortedEventHandler                             (const VcsNodeFabosZoneAbortedEvent *&pEvent);
        void                        switchPortDisableEventHandler                       (const VcsNodeFabosSwitchPortDisableEvent *&pEvent);
        void                        nodeDeletedEventHandler                             (const WaveNodeLocalNodeDeletedEvent* & pEvent);
        void                        clusterDeleteEventHandler                           (const WaveNodeDeleteClusterEvent* &pEvent);
        void                        nodeAddedEventHandler                               (const WaveNewNodesAddedEvent* &pEvent);
        void                        phase3StartEventHandler                             (const ClusterPhase3StartEvent* &pEvent);
        void                        phase3CompleteEventHandler                          (const ClusterPhase3CompleteEvent* &pEvent);
        void                        triggerPostBootMessageHandler                       (VcsNodeTriggerPostBootMessage *pVcsNodeTriggerPostBootMessage);
        void                        serviceReadyEventHandler                            (const VcsNodeFabosServiceReadyEvent *&pEvent);
        void                        systemReadyEventHandler                             (const VcsNodeFabosSystemReadyEvent *&pEvent);
        void                        linecardReadyEventHandler                           (const VcsNodeFabosLinecardReadyEvent *&pEvent);
        void                        NsmChassisReadyEventHandler                         (const NsmChassisReadyEvent *&pEvent);
        void                        NsmServiceReadyEventHandler                         (const NsmServiceReadyEvent *&pEvent);
        void                        NsmLinecardReadyEventHandler                        (const NsmLinecardReadyEvent *&pEvent);
        void                        NsmLinecardOnlineEventHandler                       (const NsmLinecardOnlineEvent *&pEvent);
        void                        handleServiceReadyEvent                             (const ResourceId &recoveryType);
        void                        handleSystemReadyEvent                              (const ResourceId &recoveryType);
        void                        handleLinecardReadyEvent                            (const UI32 &slotId, const ResourceId &recoveryType);
        ResourceId                  slotRemove                                          (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext);
        void                        postbootStartedEventHandler                         (const PostBootStartedEvent* &pEvent);
        void                        postbootCompletedEventHandler                       (const PostBootCompletedEvent* &pEvent);
        void                        configReplayStartedEventHandler                     (const ConfigReplayStartedEvent* &pEvent);
        void                        configReplayCompletedEventHandler                   (const ConfigReplayCompletedEvent* &pEvent);
        void                        lastConfigReplayCompletedEventHandler               (const LastConfigReplayCompletedEvent* &pEvent);
        void                        switchReadyToAcceptCommandsEventHandler             (const VcsNodeFabosSwitchReadyToAcceptCommandsEvent *&pEvent);
        void                        switchActivationCompleteEventHandler                (const VcsNodeFabosSwitchActivationCompleteEvent *&pEvent);
        void                        HaCacheReloadEventHandler                           (const HaCacheReloadEvent *&pEvent);
        void                        getSwitchWWNFromRBridgeIdMessageHandler             (VcsNodeGetSwitchWWNFromRBridgeIdMessage *pVcsNodeGetSwitchWWNFromRBridgeIdMessage);
        ResourceId                  getSwitchWWNFromRBridgeIdStep                       (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext);
        void                        getRBridgeIdFromSwitchWWNMessageHandler             (VcsNodeGetRBridgeIdFromSwitchWWNMessage *pVcsNodeGetRBridgeIdFromSwitchWWNMessage);
        ResourceId                  getRBridgeIdFromSwitchWWNStep                       (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext);
        void                        getSwitchIPFromRBridgeIdMessageHandler              (VcsNodeGetSwitchIPFromRBridgeIdMessage *pVcsNodeGetSwitchIPFromRBridgeIdMessage);
        ResourceId                  getSwitchIPFromRBridgeIdStep                        (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext);

        void                        getConfigurationNumberMessageHandler                (VcsNodeGetConfigurationNumberMessage *pVcsNodeGetConfigurationNumberMessage);
        static void                 getConfigurationNumberStaticMessageHandler          (VcsNodeGetConfigurationNumberMessage *pVcsNodeGetConfigurationNumberMessage);
        void                        setNewShowVcsFormatMessageHandler                   (VcsNodeSetNewShowVcsFormatMessage *pVcsNodeSetNewShowVcsFormatMessage);
        ResourceId                  setNewShowVcsFormat                                 (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext);           
        void                        getConfigurationTimeMessageHandler                  (VcsNodeGetConfigurationTimeMessage *pVcsNodeGetConfigurationTimeMessage); 
        void                        vcsSegmentSwitchMessageHandler                      (VcsNodeSegmentSwitchMessage *pVcsNodeSegmentSwitchMessage);
        void                        sendSegmentSwitchMessageToFabric                    (VcsFabMessagingContext *pVcsFabMessagingContext);

        void                        getSwitchIdentifierInfoMessageHandler               (VcsFabricGetSwitchIdentifierInfoMessage *pVcsFabricGetSwitchIdentifierInfoMessage);
        void                        vcsIsIpReachableFromNodeMessageHandler              (VcsIsIpReachableFromNodeMessage *pVcsIsIpReachableFromNodeMessage);

        void                        getClusterRbridgeIdFromWWNHandler                   (VcsGetClusterRbridgeIdFromWWNMessage *pVcsGetClusterRbridgeIdFromWWNMessage);
        static void                 getClusterRbridgeIdFromWWNStaticHandler             (VcsGetClusterRbridgeIdFromWWNMessage *pVcsGetClusterRbridgeIdFromWWNMessage);
        void                        getClusterRbridgeIdFromWWN                          (VcsGetClusterRbridgeIdFromWWNMessage *pVcsGetClusterRbridgeIdFromWWNMessage);
        static void                 cliAllowedFromThisLocationServiceIndependentStaticMessageHandler (VcsNodeCliAllowedFromThisLocationMessage *pVcsNodeCliAllowedFromThisLocationMessage);
        void                        cliAllowedFromThisLocationMessageHandler            (VcsNodeCliAllowedFromThisLocationMessage *pVcsNodeCliAllowedFromThisLocationMessage);
        void                        updateSwitchInfoMessageHandler                      (VcsNodeUpdateSwitchInfoMessage *pVcsNodeUpdateSwitchInfoMessage);
        void                        testClientForDualConnectMessageHandler              (VcsNodeTestClientForDualConnectMessage *pVcsNodeTestClientForDualConnectMessage);

        void                        getCurrentRbridgeIdsAndLocationIdsMessageHandler    (VcsFabricGetCurrentRbridgeIdsAndLocationIdsMessage *pVcsFabricGetCurrentRbridgeIdsAndLocationIdsMessage);
        void                        vcsSetWyserSessionRateLimitingDelayMessageHandler          (VcsSetWyserEaRateLimitingDelayMessage *pVcsSetWyserEaRateLimitingDelayMessage);
        void                        vcsGetWyserEaRateLimitingDelayMessageHandler          (VcsGetWyserEaRateLimitingDelayMessage *pVcsGetWyserEaRateLimitingDelayMessage);
        static  void                vcsSetWyserEaConfigFilterStaticMessageHandler         (VcsSetWyserEaConfigFilterServiceIndependentMessage *pVcsSetWyserEaConfigFilterServiceIndependentMessage);
        void                        vcsSetWyserEaConfigFilterMessageHandler               (VcsSetWyserEaConfigFilterServiceIndependentMessage *pVcsSetWyserEaConfigFilterServiceIndependentMessage);
        static  void                vcsRemoveWyserEaConfigFilterStaticMessageHandler      (VcsRemoveWyserEaConfigFilterServiceIndependentMessage *pVcsRemoveWyserEaConfigFilterServiceIndependentMessage);
        void                        vcsRemoveWyserEaConfigFilterMessageHandler            (VcsRemoveWyserEaConfigFilterServiceIndependentMessage *pVcsRemoveWyserEaConfigFilterServiceIndependentMessage);
        void                        vcsFabricSetTtyNameMessageHandler                   (VcsFabricSetTtyNameMessage *pVcsFabricSetTtyNameMessage);

        void                        vcsNodeGetPrincipalPriorityHandler                  (VcsNodeGetPrincipalPriorityMessage *pVcsNodeGetPrincipalPriorityMessage);
        void                        vcsNodePrincipalTriggerFailoverHandler              (VcsNodePrincipalTriggerFailover *pVcsNodePrincipalTriggerFailover);
        void                        vcsNodeSetPrincipalPriorityHandler                  (VcsNodeSetPrincipalPriorityMessage *pVcsNodeSetPrincipalPriorityMessage);
        static void                 isCliAllowedFromThisLocationStaticMessageHandler    (VcsNodeIsCliAllowedFromThisLocationMessage *pVcsNodeIsCliAllowedFromThisLocationMessage);
        void                        isCliAllowedFromThisLocationMessageHandler          (VcsNodeIsCliAllowedFromThisLocationMessage *pVcsNodeIsCliAllowedFromThisLocationMessage);

        // query functions
        ResourceId                  queryLocalManagedObjectFromMappedId                 (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext);
        ResourceId                  queryLocalManagedObjectFromDomainId                 (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext);
        ResourceId                  queryThisLocalManagedObject                         (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext);
        ResourceId                  queryLocalManagedObjectFromWWN                      (VcsNodeLocalMessagingContext *pVcsNodeLocalMessagingContext);

        // timer function
        void                        establishPrincipalTimerWithAsyncEnq                 (TimerHandle timerHandle, void * pContext);
        //Private methods related to setting the fields in VcsNodeLocalManagedObject of this node
        void                        setThisVcsNodeLocalManagedObject                    (VcsNodeLocalManagedObject*   pVcsNodeLocalManaged);
        void                        setThisVcsNodeLocalManagedObjectVcsMode             (UI32 vcsMode);
        void                        setThisVcsNodeLocalManagedObjectSwitchDeleteAfterFailover (bool switchDeleteAfterFailoverFlag);
        void                        setThisVcsNodeLocalManagedObjectVcsId               (UI32 vcsId);
        void                        setThisVcsNodeLocalManagedObjectStatus              (ResourceId genericStatus, ResourceId specificStatus);
        void                        setThisVcsNodeLocalManagedObjectDomainId            (UI32 domainId);
        void                        setThisVcsNodeLocalManagedObjectLocationId          (LocationId locationId);
        void                        setThisVcsNodeLocalManagedObjectSerialNumber        (const string & serialNumber);
        void                        setThisVcsNodeLocalManagedObjectSwitchMac           (MacAddress switchMac);
        void                        setThisVcsNodeLocalManagedObjectfcfMac              (MacAddress  fcfMac); 
        void                        setThisVcsNodeLocalManagedObjectInVcs               (bool inVcs);
        void                        setThisVcsNodeLocalManagedObjectIsPrincipal         (bool isPrincipal);
        void                        setThisVcsNodeLocalManagedObjectPublicIpAddress     (const vector<string> & ipAddress);
        void                        setThisVcsNodeLocalManagedObjectGenericStatus       (ResourceId genericStatus);
        void                        setThisVcsNodeLocalManagedObjectSpecificStatus      (ResourceId genericStatus);
        void                        setThisVcsNodeLocalManagedObjectSwitchPort          (SI32 port);
        void                        setThisVcsNodeLocalManagedObjectSwitchWWN           (WorldWideName  switchWWN); 
        void                        setThisVcsNodeLocalManagedObjectSWBDModel           (int SWBDModel);
        void                        setThisVcsNodeLocalManagedObjectSwitchFwVersion     (const string &fwVersion);
        void                        setThisVcsNodeLocalManagedObjectSwitchname          (const string &switchname);
        void                        setThisVcsNodeLocalManagedObjectSwmodelName         (const string &swmodelName);
        //Methods related to getting the fields in VcsNodeLocalManagedObject of this node
        UI32                        getThisVcsNodeLocalManagedObjectVcsId               () const;
        ResourceId                  getThisVcsNodeLocalManagedObjectGenericStatus       () const;
        ResourceId                  getThisVcsNodeLocalManagedObjectSpecificStatus      () const;
        UI32                        getThisVcsNodeLocalManagedObjectMappedId            () const;
        UI32                        getThisVcsNodeLocalManagedObjectDomainId            () const;
        LocationId                  getThisVcsNodeLocalManagedObjectLocationId          () const;
        ObjectId                    getThisVcsNodeLocalManagedObjectObjectId            () const;
        string                      getThisVcsNodeLocalManagedObjectSerialNumber        () const;
        MacAddress                  getThisVcsNodeLocalManagedObjectSwitchMac           () const;
        MacAddress                  getThisVcsNodeLocalManagedObjectfcfMac              () const; 
        bool                        getThisVcsNodeLocalManagedObjectInVcs               () const;
        bool                        getThisVcsNodeLocalManagedObjectWillingMode         () const;
        bool                        getThisVcsNodeLocalManagedObjectIsPrincipal         () const;
        WorldWideName               getThisVcsNodeLocalManagedObjectSwitchWWN           () const; 
        vector<string>              getThisVcsNodeLocalManagedObjectPublicIpAddress     () const;
        SI32                        getThisVcsNodeLocalManagedObjectSwitchPort          () const;
        SI32                        getThisVcsNodeLocalManagedObjectSWBDModel           () const;
        string                      getThisVcsNodeLocalManagedObjectSwitchFwVersion     () const;
        string                      getThisVcsNodeLocalManagedObjectSwitchname          () const;
        string                      getThisVcsNodeLocalManagedObjectSwmodelName         () const;
        void                        updateThisVcsNodeLocalManagedObject                 ();
        void                        createThisVcsNodeLocalManagedObject                 ();
        VcsNodeLocalManagedObject  *getThisVcsNodeLocalManagedObject                    () const;
        VcsClusterManagedObject    *getVcsClusterManagedObject                          ();
        ResourceId                  prismLinearSequencerSucceededStepForNoVcsEnable     (PrismSynchronousLinearSequencerContext *pPrismLinearSequencerContext);
        void                        prismLinearSequencerFailedStepForNoVcsEnable        (PrismSynchronousLinearSequencerContext *pPrismLinearSequencerContext);
        void                        resetThisNodeStateForClusterMerge                   (PrismLinearSequencerContext *pPrismLinearSequencerContext);
        void                        sendClusterMergeMessageToFramework                  (PrismLinearSequencerContext *pPrismLinearSequencerContext);
        void                        sendClusterMergeMessageToFrameworkCallback          (FrameworkStatus frameworkStatus, PrismMessage *pPrismMessage, void *pContext);
        //Private method related to controlling SCN handling
        bool                        isNodeReadyForSCNHandling                           () const; 
        bool                        isNodeReadyForSlotSCNHandling                       () const; 
        bool                        isOperationAllowedBeforeEnabling                    (const UI32 &operationCode);
        void                        externalStateSynchronization                        (WaveAsynchronousContextForExternalStateSynchronization *pWaveAsynchronousContextForExternalStateSynchronization);
        void                        removeUserCreatedFiles                              (const char* flashPath);
        void                        getFcfMacFromFcoeBackendAndNotifyCcmd               ();       
        void                        getFcfMacFromFcoeBackendAndUpdateMO                 ();       
        void                        setFcfMacAddress                                    ();
        static MacAddress           getFcfMacAddress                                    ();
        void                        setThisVcsNodeLocalManagedObjectPrincipalPriority   (UI32 principalPriority);
        UI32                        getThisVcsNodeLocalManagedObjectPrincipalPriority   () const;
        void                        setFcfMacAddressHandler                             (VcsNodeSetFcfMacMessageServiceIndependent *pVcsNodeSetFcfMacMessage);
        static void                 setFcfMacAddressStaticHandler                       (VcsNodeSetFcfMacMessageServiceIndependent *pVcsNodeSetFcfMacMessage);
        void                        prepareDomainListAfterHaRecoveryMessageHandler      (VcsNodePrepareDomainListAfterHaRecoveryMessage *pVcsNodePrepareDomainListAfterHaRecoveryMessage);
        bool                        triggerPrincipalSwitchoverBasedOnPrincipalPriority  ();
        UI32                        setThisNodePrincipalPriorityInFabric                ();
        //Following method gives the principal priority of the given locationId. If the locationId is 0 it give the lowest principal priority value.
        ResourceId                  getPrincipalPriority                                (UI32 locationId, UI32 &principalPriority);

        void                        vcsIsGvlanCapableMessageHandler                     (VcsNodeIsGvlanCapableMessage *pVcsNodeIsGvlanCapableMessage);
        static void                 vcsIsGvlanCapableStaticMessageHandler               (VcsNodeIsGvlanCapableMessage *pVcsNodeIsGvlanCapableMessage);
        void                        vcsIsGvlanEnabledMessageHandler                     (VcsNodeIsGvlanEnabledMessage *pVcsNodeIsGvlanMessage);
        static void                 vcsIsGvlanEnabledStaticMessageHandler               (VcsNodeIsGvlanEnabledMessage *pVcsNodeIsGvlanMessage);
    protected :
    public :
        virtual                    ~VcsFabricLocalObjectManager                         ();

        static string               getClassName                                        ();
        static VcsFabricLocalObjectManager   *getInstance                               ();
        static PrismServiceId       getPrismServiceId                                   ();
        static void                 debugSetDomainId                                    (UI32 argc, vector<string> argv);
        static void                 debugGenerateSCN                                    (UI32 argc, vector<string> argv);
        static ResourceId           addDomainsDebugMessageHandler                       (UI32 argc, vector<string> argv);
        static ResourceId           removeDomainsDebugMessageHandler                    (UI32 argc, vector<string> argv);
        PrismEvent                 *createEventInstance                                 (const UI32 &eventOperationCode);
        static void                 debugGenerateSlotScn                                (UI32 argc, vector<string> argv);
        static void                 debugTriggerPostBootMessage                         (UI32 argc, vector<string> argv);
        static void                 debugTriggerHaMessage                               (UI32 argc, vector<string> argv);
        static bool                 getIsWyserEaPhase2Complete                            ();
        static void                 setIsWyserEaPhase2Complete                            (bool isWyserEaPhase2Complete);

        // External API
        ObjectId                    getThisVcsNodeObjectId                              ();
        static  bool                getIsInstantiated                                   ();
        
        ObjectId                    getVcsNodeObjectIdFromMappedId                      (const UI32 &mapped_id);
        static ResourceId           getThisVcsNodeClusterType                           ();
        static void                 setThisVcsNodeSWBDModel                             (const int &swbdModel);

        // update
        UI32                        updateLocalManagedObject                            (string chassisString, MacAddress &switchMac, int domainId, WorldWideName &switchWWN, int SWBDModel, const string &fwVersion, const string &swmodelName);
        WaveMessageStatus           synchronousMessageSend                              (ManagementInterfaceMessage *message);
        VcsNodeLocalManagedObject*  getLocalManagedObjectFromMappedId                   (const UI32 mappedId);
        VcsNodeLocalManagedObject*  getLocalManagedObjectFromLocationId                 (const LocationId locationId);
        VcsNodeLocalManagedObject*  getLocalManagedObjectFromWWN                        (const WorldWideName &wwn);

        bool                        getNoVcsEnableInProgressFlag                        ();
        void                        getPtrToPrincipalTrackingWorker                     ();
        static bool                 parseClusterConfFile                                ();
        static ResourceId           setClusterConfFile                                  (bool clusterMode);
        static bool                 getNewShowVcs                                       ();    
        
        // Segment node request contexts.
        void                        addToSegmentMessagingContext                        (VcsFabMessagingContext *pVcsFabMessagingContext);
        vector<VcsFabMessagingContext *> getSegmentMessagingContext                     () const;
        void                        clearSegmentMessagingContext                        ();
        static void                 registerClusterEnabledCheckFunctionWithPersistence  ();
        static void                 registerAddRASLogFunctionWithPersistence            ();
        static void                 registerVCSEnableCheckFunctionWithWave              ();
        static void                 registerLinecardReadySlotIdBitMapFunction           ();
        static void                 addRASLogForFramework                               (ResourceId status);
               bool                 getThisVcsNodeLocalManagedObjectVcsMode             () ;
        static bool                 isVcsEnabled                                        ();
        static void                 getLinecardReadySlotIdBitMap                        (BitMap &linecardReadySlotIdBitMap, UI32 &maxLinecards);
        static UI32                 getVcsID                                            ();
        static int                  getRASLogIdFromFrameworkResourceId                  (ResourceId resourceIdFromFramework);
        static ResourceId           removeCcmdCfg                                       ();
        static ResourceId           removeDcmdCfg                                       ();
        
        // These two functions will bypass framework HA sync worker, that may cause some version compatibility issue. Please don't use them right now.
        static ResourceId           removeCcmdCfgOnStandby                              ();
        static ResourceId           removeDcmdCfgOnStandby                              ();

        static void                 getLocalMappedIdServiceIndependentStaticMessageHandler  (VcsNodeGetLocalMappedIdMessage *pVcsNodeGetLocalMappedIdMessage);
        void                        prismPostPersistenceBootMessageCallback             (FrameworkStatus frameworkStatus, PrismPostPersistenceBootMessage *pPrismPostPersistenceBootMessage, void *pContext);
        void                        notifyStartHaSyncMessageCallback                    (FrameworkStatus frameworkStatus, FrameworkObjectManagerStartHaSyncMessage *pFrameworkObjectManagerStartHaSyncMessage, void *pContext);
        void                        notifyStartCcmdHaSyncMessageCallback                (FrameworkStatus frameworkStatus, FrameworkObjectManagerStartCcmdHaSyncMessage *pFrameworkObjectManagerStartCcmdHaSyncMessage, void *pContext);
        static string               getFwVersion                                        ();
        void                        sendHaSyncUpdateMessage                             ();
        static ResourceId           setVcsClusterConfigHandler                          (VcsClusterConfiguration *pDcmVcsClusterConfigData, const UI32 &context);
        static void                 getVcsClusterConfigHandler                          (VcsClusterConfiguration *pDcmVcsClusterConfigData, const UI32 &context);

        void                        haSyncCollectValidationData                         (WaveObjectManagerHaSyncCollectValidationDataMessage *pMessage, PrismAsynchronousContext *pPrismAsynchronousContext);
        void                        haboot                                              (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
        void                        vcsFabricDebugClientMessageHandler                  (VcsFabricDebugClientMessage *pVcsFabricDebugClientMessage);
        static void                 setVcsFabricLocalObjectManagerState                 (const ResourceId &state);
        static ResourceId           getVcsFabricLocalObjectManagerState                 ();                      
        static void                 setCachedVcsClusterGuid                             (const UI64 &vcsClusterGuid);
        static ResourceId           getCachedVcsClusterGuid                             (UI64 &vcsClusterGuid);
        void                        resetDomainBitmapMessageHandler                     (VcsNodeResetDomainBitMapMessage *pVcsNodeResetDomainBitMapMessage);
        void                        enableSlotScnHandlingMessageHandler                 (VcsNodeEnableSlotScnHandlingMessage *pVcsNodeEnableSlotScnHandlingMessage);
        void                        dbInconsistencyCheck                                (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
        static bool                 isClusterFormationInProgress                        ();
		bool 						isLinecardPostBootComplete 			                ();
        static void                 addToWWNRbridgeIdMapCache                           (const WorldWideName &wwn, const UI32 rbridgeId);
        static void                 deleteFromWWNRbridgeIdMapCache                      (const WorldWideName &wwn);
        static void                 setWWNRbridgeIdMapCache                             (const map <string, UI32> &wwnRbridgeIdMap);
        static void                 getWWNRbridgeIdMapCache                             (map <string, UI32> &wwnRbridgeIdMap);
        static void                 updateWWNRbridgeIdMapCache                          ();
        static void                 clearWWNRbridgeIdMapCache                           ();
        static void                 getWWNRbridgeIdMapFromDB                            (map <string, UI32> &wwnRbridgeIdMap);

    // Now the data members

    private:
        ObjectId                                m_thisVcsNodeObjectId;
        string                                  m_thisVcsNodeSerialNumber;
        MacAddress                              m_thisVcsNodeSwitchMac;
        static MacAddress                       m_thisVcsNodeFcfMac;
        static PrismMutex                       m_thisVcsNodeFcfMacMutex;
        WorldWideName                           m_thisVcsNodeSwitchWWN;
        PrismMutex                              m_thisVcsNodeObjectIdMutex;
        static bool                             m_isInstantiated;
        PrismMutex                              m_thisVcsNodeFwVersionMutex;
        string                                  m_thisVcsNodeFwVersion;
        WaveBootReason                          m_thisVcsNodeBootReason;
        static bool                             m_newShowVcs;
        vector<VcsFabMessagingContext *>        m_segmentMessagingContext;
        bool                                    m_noVcsEnableInProgressFlag;
        bool                                    m_noVcsEnablePrincipalFlag;
        bool                                    m_isReady;
        bool                                    m_noVcsEnablePrincipalControlFailoverFlag;
        static PrismMutex                       m_thisVcsNodeLocalManagedObjectMutex;
        ResourceId                              m_systemRecoveryType;
        bool                                    m_serviceReadyEventReceived;
        bool                                    m_systemReadyEventReceived;
        bool                                    m_linecardReadyEventReceived;
        bool                                    m_systemReadyPostbootProcessingComplete;
        BitMap                                  m_linecardReadySlotIdBitMap;
        PrismMutex                              m_linecardReadySlotIdBitMapMutex;
        BitMap                                  m_linecardReadyPostBootCompleteBitMap;
        ResourceId                              m_linecardRecoveryType[MAX_LINECARD_NUM];
        static bool                             m_isWyserEaPhase2Complete;
        static PrismMutex                       m_isWyserEaPhase2CompleteMutex;
        static PrismMutex                       m_ClusterConfFileMutex;
        bool                                    m_isConfigReplayCompleted;
        bool                                    m_isHaSyncInProgress;

        VcsNodeLocalManagedObject              *m_pVcsNodeLocalManagedObject;
        VcsSlotLocalManagedObject              *m_pVcsSlotLocalManagedObject;

        VcsNodePrincipalSelectionWorker        *m_VcsPrincipalSelectionWorker;
        VcsNodeFCPrincipalTrackingWorker       *m_fcPrincipalTrackingWorker;
        VcsNodeSlotManagementWorker            *m_vcsNodeSlotManagementWorker;
        VcsCopyToStartupWorker                 *m_pVcsCopyToStartupWorker;

        VcsNodeConfigSnapshotWorker            *m_pVcsNodeConfigSnapshotWorker; 
        VcsNodeClusterInfoWorker               *m_pVcsNodeClusterInfoWorker;

        PrismMutex                              m_thisVcsNodeSwmodelNameMutex;
        string                                  m_thisVcsNodeSwmodelName;

        static ResourceId                       m_vcsFabricLocalObjectManagerState;
        static PrismMutex                       m_vcsFabricLocalObjectManagerStateMutex;

        static UI64                             m_vcsClusterGuid;
        static PrismMutex                       m_vcsClusterGuidMutex;
        bool                                    m_isNodeReadyForSlotScn;
        static map<string, UI32>                m_WWNRbridgeIdMapCache;
        static PrismMutex                       m_WWNRbridgeIdMapCacheMutex;
    protected:
    public :

        friend class VcsNodePrincipalSelectionWorker;
        friend class VcsNodeFCPrincipalTrackingWorker;
        friend class VcsFabObjectManager;
        friend class VcsNodeFabosInterfaceObjectManager;
        friend class VcsNodeSlotManagementWorker;
        friend class VcsFabAddRejoinNodeWorker;
        friend class VcsFabRemoveSegmentNodeWorker;
        friend class VirtualIpGlobalWorker;
        friend class VcsCopyToStartupWorker;
        friend class VcsNodeConfigSnapshotWorker;
        friend class VcsFabConfigSnapshotWorker;
        friend class VcsNodeClusterInfoWorker;
};

inline string VcsFabricLocalObjectManager::getClassName ()
{
    return ("VCS Fabric Local Node Base Services");
}

}

#endif // VCSFABRICLOCALOBJECTMANAGER_H
