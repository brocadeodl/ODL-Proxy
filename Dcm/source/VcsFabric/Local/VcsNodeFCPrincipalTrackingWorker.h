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
 *   Author : Jitendra Singh                                               *
 ***************************************************************************/

#ifndef VCSNODEFCPRINCIPALTRACKINGWORKER_H
#define VCSNODEFCPRINCIPALTRACKINGWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "VcsFabric/Local/VcsNodeDomainEvents.h"
#include "VcsFabric/Local/VcsNodeSystemEvents.h"
#include "VcsFabric/Global/VcsFabObjectManagerTypes.h"
#include "VcsFabric/Global/VcsFabObjectManager.h"
#include "VcsFabric/Global/VcsAddSwitchMessage.h"
#include "Framework/Boot/BootCompleteForThisLocationEvent.h"
#include "Framework/Boot/BootTypes.h"
#include "Framework/ObjectModel/WaveObjectManager.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForShutdownPhases.h"
#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "VcsFabric/Local/VcsNodeTimerHandler.h"
#include "VcsFabric/Global/VcsSegmentSwitchMessage.h"
#include "Framework/Core/ClusterFailoverCompleteEvent.h"
#include "Cluster/ClusterMessages.h"
#include "Framework/Core/FrameworkObjectManagerMessages.h"
#include "Framework/Utils/PrismAsynchronousContext.h"
#include "VirtualIp/Global/VirtualIpGlobalBindVipMessage.h"
#include "VirtualIp/Global/VirtualIpGlobalUnbindVipMessage.h"

namespace DcmNs
{

typedef enum 
{
    NO_SWITCHES_TO_ADD_FC = 1,
    PRINCIPAL_AND_CLUSTER_ALREADY_EXIST_FC,
    CREATE_CLUSTER_CASE_FC,
    SWITCH_WILL_NOT_BE_PRINCIPAL_FC,
    ANOTHER_SWITCH_IS_PRINCIPAL_FC,
    CLUSTER_MERGE_CASE_FC,
    PRINCIPAL_FAILOVER_CASE_FC,
    ADD_SWITCH_CASE_FC,
    DOMAINID_NOT_LOWEST_FC,
    CLUSTER_FORMATION_IN_PROGRESS_FC,
    UNCONFIGURE_SECONDARY_CREATE_CLUSTER_CASE_FC
} principalTrackingCases;

class VcsNodeHaObjectManager;
class VcsNodeResetDomainBitMapAfterHAFailover;
class VcsNodePrepareDomainListAfterWarmRecoveryMessage;

class VcsNodeFCPrincipalTrackingWorker : public WaveWorker
{
    public :
                                                        VcsNodeFCPrincipalTrackingWorker                                    (VcsFabricLocalObjectManager *pVcsFabricLocalObjectManager);
        virtual                                        ~VcsNodeFCPrincipalTrackingWorker                                    ();
        static                                          VcsNodeFCPrincipalTrackingWorker *getInstance                       ();
                void                                    boot                                                                (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
    
                string                                  getClassName                                                        ();


                ResourceId                              DomainValidEventHandler                                             (const VcsNodeDomainValidEvent *&pEvent);
                ResourceId                              DomainInvalidEventHandler                                           (const VcsNodeDomainInvalidEvent *&pEvent);
                ResourceId                              DomainReachableEventHandler                                         (const VcsNodeDomainReachableEvent *&pEvent);
                ResourceId                              DomainUnReachableEventHandler                                       (const VcsNodeDomainUnreachableEvent *&pEvent);
                ResourceId                              DomainCapabilitiesAvailableEventHandler                             (const VcsNodeDomainCapabilitiesAvailableEvent *&pEvent);
                ResourceId                              SwitchOfflineEventHandler                                           (const VcsNodeSwitchOfflineEvent *&pEvent);
                ResourceId                              triggerSwitchOffline                                                ();
                void                                    ResetNodeToUnconfirmRoleCallBack                                    (FrameworkStatus frameworkStatus, FrameworkObjectManagerResetNodeToUnconfirmRole *pMessage, void *pContext);      
                ResourceId                              switchReadyToAcceptCommandsEventHandler                             (const VcsNodeSwitchReadyToAcceptCommandsEvent *pSwitchReadyEvent);
                ResourceId                              switchActivationCompleteEventHandler                                (const VcsNodeSwitchActivationCompleteEvent *pSwitchActivationCompleteEvent);

                void                                    sendSwitchOfflineUnconfigureMessageCallBack                         (FrameworkStatus frameworkStatus, FrameworkObjectManagerUnconfigureClusterSecondaryMessage *pMessage, void *pContext);
                void                                    sendSwitchOfflineDeleteClusterMessageCallBack                       (FrameworkStatus frameworkStatus, FrameworkObjectManagerDestroyClusterMessage *pClusterObjectManagerDeleteClusterMessage, void *pContext);

                void                                    addMessageCallback                                                  (FrameworkStatus frameworkStatus, PrismMessage *pPrismMessage, void *pContext);
                void                                    handleFCPrincipalChangeCallback                                     (FrameworkStatus frameworkStatus, FrameworkObjectManagerSecondaryNodeFailureNotificationMessage *pMessage, void *pContext);
                void                                    listenForEvents                                                     (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
                void                                    disable                                                             (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases);
                void                                    shutdown                                                            (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases);
                void                                    resetDomainBitmap                                                   ();
                void                                    prepareDomainListAfterHaRecovery                                    (const bool &isWarmRecovery); 
                void                                    heartBeatFailure                                                    (PrismAsynchronousContext *pPrismAsynchronousContext);
                void                                    heartbeatFailureDeleteNodeCallback                                  (FrameworkStatus frameworkStatus, FrameworkObjectManagerDeleteNodesFromClusterMessage *pMessage, void *pContext);
                void                                    heartbeatFailureUnControlledFailoverCallback                        (FrameworkStatus frameworkStatus, FrameworkObjectManagerSecondaryNodeFailureNotificationMessage*pMessage, void *pContext);
                
        static  UI32                                    getMgmtPort                                                         (UI32 domainId);
        static  string                                  getIpAddressFromDomainId                                            (UI32 domainId);
        static  UI32                                    getx86DomainIdFromIpAddressAndMgmtPort                              (string ipAddress, UI32 mgmtPort);
        static  void                                    getx86ipAddressAndPort                                              (const UI32 &x86DomainId, string &x86ipAddress, UI32 &x86port);
        static  bool                                    isEnvx86                                                            ();
        static  vector<UI32>                            getx86DomainList                                                    ();
        static  vector<string>                          getx86ipAddresses                                                   ();
        static  vector<UI32>                            getx86ports                                                         ();
                
        static  void                                    addTox86DomainList                                                  (UI32 &x86DomainId);
        static  void                                    addTox86IpAddressList                                               (string &x86IpAddress);
        static  void                                    addTox86PortList                                                    (UI32 &x86Port);
        static  void                                    setx86DomainList                                                    (vector<UI32> &x86DomainList);
        static  void                                    setx86ipAddresses                                                   (vector<string> &x86iPAddress);
        static  void                                    setx86ports                                                         (vector<UI32> &x86ports);

        static  void                                    getKnownDomainList                                                  (scnDomainsStruct &knownDomainList);

        static  int                                     fabGetCosDomainList                                                 (u_short *domainList, size_t recv_size);
        
                void                                    replayConfigFileCallback                                            (FrameworkStatus frameworkStatus, PrismMessage *pPrismMessage, void *pContext);

    protected :
    private :
                bool                                    isClusterEnabled                                                    ();
                void                                    sendClusterMergeMessagesToRemoteLocationsStep                       (VcsNodeRemoteMessagingContext *pVcsNodeRemoteMessagingContext);
                void                                    clusterMergeMessageAsyncCallBack                                    (FrameworkStatus frameworkStatus, PrismMessage *pPrismMessage, void *pContext);
                void                                    handleClusterMergeRejoinAndAddNode                                  (vector<UI32> &domainForTrivialMerge);
                void                                    handleClusterMergeRejoinAndAddNodeCallback                          (PrismAsynchronousContext *pPrismAsynchronousContext);
                void                                    becomePrincipalAfterRebootSequencer                                 ();
        
                void                                    getRemoteSwitchInfoForDomainList                                    (u_short *fabDomainList);
                void                                    getRemoteSwitchInfoForDomainListCallback                            (PrismAsynchronousContext *pPrismAsynchronousContext);
                void                                    getSwitchInfoForDomainListAsyncMessageStep                          (VcsNodeRemoteMessagingContext *pVcsNodeRemoteMessagingContext);
                void                                    getRemoteSwitchInfoAsyncCallBack                                    (FrameworkStatus frameworkStatus, PrismMessage *pPrismMessage, void *pContext);

                void                                    addSwitchesNotInCluster                                             ();
                void                                    createClusterAndAddSwitches                                         ();
                void                                    deleteObjects                                                       (vector<WaveManagedObject *> *pResults);
                bool                                    disableTimer                                                        ();
                bool                                    startPrincipalTrackingTimer                                         (UI32 timeoutValue);
                void                                    trackPrincipalTimerWithAsyncEnq                                     (TimerHandle timerHandle, void * pContext);
                void                                    becomePrincipal                                                     ();
                void                                    becomePrincipalCallBack                                             (FrameworkStatus frameworkStatus, PrismMessage *pPrismMessage, void *pContext);  
                void                                    performFailoverForFailedLocation                                    (LocationId &locationId, PrismAsynchronousContext *pPrismAsynchronousContext = NULL);
                void                                    continueTrackPrincipalAfterRemoteEnqComplete                        (VcsNodeRemoteMessagingContext *pVcsNodeRemoteMessagingContext);

                void                                    clearReachableDomainBitForDomainId                                  (const u_short &unReachableDomain);
                void                                    clearRemoteSwitchInfoList                                           ();
                void                                    clearSwitchInfoRetrievedBitMap                                      ();
                void                                    doControlledPrincipalChangeOver                                     (UI32 fcPrincipalDomainId);
                void                                    trackFCPrincipal                                                    ();

                bool                                    fabAndMyDomainListsMatch                                            (u_short *fabDomainList);
                bool                                    doesFabricDomainAndConnectedDomainListMatch                         (u_short *fabDomainList);
                vector<WaveManagedObject *>            *fabOMDoSynchronousQuery                                             (const string className);
                vector<VcsNodeLocalManagedObject *>    *getNodeLocalManagedObjects                                          ();

                bool                                    amIVcsPrincipal                                                     ();

                bool                                    isSwitchInfoRetrievedForDomainId                                    (const u_short &domainId);
                bool                                    isSwitchDomainIdInMyCluster                                         (const VcsFabricLocalSwitchInfoObject &switchInfo);
                bool                                    isSwitchDomainIdInMyCluster                                         (UI32 mappedId);
        static  bool                                    isClusterFormationInProgress                                        ();
                bool                                    isThereAnotherVcsPrincipalInThisCluster                             (vector<UI32> &domainIds);
                bool                                    isNodeInTrivialMergeState                                           (vector<WaveManagedObject *> *pResults, UI32 domainId);
        
                bool                                    isThereANodeForTrivialMerge                                         (vector<UI32> &domainForTrivialMerge);
                bool                                    isSwitchConnectedToThisCluster                                      (UI32 domainId);

                bool                                    isMyDomainIdLowest                                                  (UI32 &minIndex);
                bool                                    validMappedId                                                       (UI32 mappedId);

                UI32                                    wereAllSwitchesAdded                                                (BitMap &switchesNotInCluster);

                UI32                                    getMyDomainId                                                       ();
                UI32                                    getMyMappedId                                                       ();
                VcsNodeLocalManagedObject              *getNodeLocalManagedObjectForThisNode                                ();

                UI32                                    getLowestDomainId                                                   (UI32 &minIndex);

                VcsNodeLocalManagedObject              *getSwitchObjectFromDomainId                                         (UI32 domainId);
                UI32                                    getPortFromDomainId                                                 (UI32 domainId);
                VcsNodeLocalManagedObject              *getSwitchObjectFromObjectId                                         (const ObjectId &switchObjectId);
//                UI32                                    getMappedIdForSwitchSerialNumber                                    (string switchSerialNumber);
                void                                    addRemoteSwitchInfo                                                 (const VcsFabricLocalSwitchInfoObject &remoteSwitchInfo);
                void                                    removeSwitchInfo                                                    (const UI32 &domainId);

                void                                    clearSwitchInfoLists                                                ();
                void                                    getLocalSwitchInfo                                                  (VcsFabricLocalSwitchInfoObject &localSwitchInfo);
                UI32                                    getDomainsInCluster                                                 (BitMap &domainsIncluster);
                UI32                                    getConnectedDomainsInCluster                                        (BitMap &domainsIncluster);
//                UI32                                    getLocalSwitchMappedId                                              ();
                ObjectId                                getLocalSwitchObjectId                                              ();
                UI32                                    getLocalNodeVcsId                                                   ();
                UI32                                    getVcsClusterId                                                     (bool &clusterExists);
                vector<string>                          getVcsClusterGUIDsFromClusterMO                                     ();

                vector<string>                          getClusterGUIDs                                                     ();

                void                                    initializeDomainList                                                ();
                void                                    initializeClusterGUIDs                                              ();
                void                                    setVcsClusterGUIDs                                                  (const vector<string> &vcsClusterGUIDs);

                bool                                    myDomainListMatchesRemoteSwitches                                   ();
                UI32                                    segmentMismatchedNodes                                              (u_short *fabDomainList);
                void                                    relenquishPrincipal                                                 ();
                UI32                                    setPrincipalFlag                                                    (bool principalFlag);

                void                                    setReachableDomainBitForDomainId                                    (const u_short &reachableDomain);
                void                                    setDCADomainBitForDomainId                                          (const u_short &dcaDomain);
                void                                    setDomainValid                                                      (const bool &domainValidOrInvalid);
                bool                                    getDomainValid                                                      ();
                void                                    setSwitchInfoRetrievedBit                                           (const u_short &infoRetrievedDomain);
                ResourceId                              setThisSwitchInfoInEnqMessage                                       (VcsEnqSwitchMessage *pVcsEnqSwitchMessage);
                void                                    setBitMapSize                                                       (BitMap &bitMap);
                void                                    setSizeForDomainBitMaps                                             (scnDomainsStruct & domainBitMapStruct);
        static  void                                    setClusterFormationInProgress                                       (const bool &clusterFormationInProgress);
                void                                    trackFCPrincipalSteps                                               ();

                bool                                    validDomainId                                                       (UI32 domainId);
                void                                    saveRemoteSwitchInfoFromEnqMessage                                  (VcsEnqSwitchMessage *pVcsEnqSwitchMessage);
                void                                    updateRemoteSwitchInfoList                                          (VcsFabricLocalSwitchInfoObject &switchInfo);
        static  bool                                    vcsModeEnabled                                                      ();
        static  void                                    removeFromx86DomainList                                             (UI32 &x86DomainId);

                UI32                                    getFCPrincipalDomainId                                              ();
                void                                    setFCPrincipalDomainId                                              (const UI32 fcPrincipalDomainId);
                void                                    cleanupTimerHandlerForDomainId                                      (UI32 domainId, bool deleteTimer);
                void                                    processCleanupTimerForDomainId                                      (TimerHandle timerHandle, void * pContext);
                UI32                                    replaySCNQueueEvents                                                ();
                void                                    handleFCPrincipalChange                                             ();
                void                                    startTimerIfFabDomainListIncludesNodesNotInVcsClusterOrDisconnected (const UI32 &timerTimeout = 0);
        
                void                                    segmentSwitchFromCluster                                            (const vector<UI32> &domainIdsToSegment, const vector<UI32> &reasonsToSegment);                        
                void                                    segmentSwitchFromClusterCallBack                                    (FrameworkStatus frameworkStatus, VcsSegmentSwitchMessage *pVcsSegmentSwitchMessage, void *pContext);

                void                                    prepareNodeForClusterCreation                                       ();
                void                                    prepareNodeForClusterCreationCallBack                               (FrameworkStatus frameworkStatus, FrameworkObjectManagerUnconfigureClusterSecondaryMessage *pMessage, void *pContext);

                ResourceId                              bindVirtualIp                                                       ();
                ResourceId                              unbindVirtualIp                                                     ();
                void                                    bindVirtualIpCallback                                               (FrameworkStatus frameworkStatus, VirtualIpGlobalBindVipMessage *pVirtualIpGlobalBindVipMessage, void *pContext);
                void                                    unbindVirtualIpCallback                                             (FrameworkStatus frameworkStatus, VirtualIpGlobalUnbindVipMessage *pVirtualIpGlobalUnbindVipMessage, void *pContext);
				void 									getWWNAndSwbdForReplacementNodes (VcsFabricLocalSwitchInfoObject & switchInfoObject);

    // Now the Data Members

    private :
        vector<VcsFabricLocalSwitchInfoObject>          m_remoteSwitchInfoList;

        scnDomainsStruct                                m_knownDomainsList;
        UI32                                            m_dcaDomainListSize;
        static TimerHandle                              m_timerHandle;
        static bool                                     m_timerRunningFlag;
        bool                                            m_fabDomainValid;
        bool                                            m_isNodeReadyForSCNHandling;
        PrismMutex                                      m_prismMutex;
        bool                                            m_clusterMergeFlag;
        UI32                                            m_fcPrincipalDomainId;

        vector<VcsNodeTimerHandler*>                    m_vcsNodeTimerHandler;

        vector<UI32>                                    m_x86domainList;
        vector<string>                                  m_x86ipAddresses;
        vector<UI32>                                    m_x86ports;

        vector<string>                                  m_vcsClusterGUIDs;
        bool                                            m_principalTrackingTimerRunInUnconfirmedState;
        bool                                            m_isNeedToStartTimerForWarmRecovery;
        static PrismMutex                               m_fabGetCosDomainListMutex;

        static const UI32                               PRINCIPAL_TRACKING_TIMEOUT_IN_MILLISECONDS_IN_UNCONFIRMED_STATE;
        set<LocationId>                                 m_failedNodesLocationList;

        friend class VcsFabricLocalObjectManager;
        friend class VcsFabObjectManager;
//        friend class VcsNodeRemoteMessagingObjectManager;
        friend class VcsNodeHaObjectManager;

    protected :
    public :

};

}

#endif // VCSNODEFCPRINCIPALTRACKINGWORKER_H
