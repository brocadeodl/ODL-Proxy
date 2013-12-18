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

#ifndef VCSNODEPRINCIPALSELECTIONWORKER_H
#define VCSNODEPRINCIPALSELECTIONWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "VcsFabric/Local/VcsNodeDomainEvents.h"
#include "VcsFabric/Local/VcsNodePSMessagingContext.h"
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

namespace DcmNs
{

class VcsFabricLocalSwitchInfoObject;

typedef enum 
{
    NO_SWITCHES_TO_ADD = 1,
    PRINCIPAL_AND_CLUSTER_ALREADY_EXIST,
    CREATE_CLUSTER_CASE,
    SWITCH_WILL_NOT_BE_PRINCIPAL,
    ANOTHER_SWITCH_IS_PRINCIPAL,
    CLUSTER_MERGE_CASE,
    PRINCIPAL_FAILOVER_CASE,
    ADD_SWITCH_CASE,
    DOMAINID_NOT_LOWEST,
    CLUSTER_FORMATION_IN_PROGRESS
} principalSelectionCases;

class VcsNodePrincipalSelectionWorker : public WaveWorker
{
    private :
        bool        isClusterEnabled                        ();
    protected :
    public :
                    VcsNodePrincipalSelectionWorker         (VcsFabricLocalObjectManager *pVcsFabricLocalObjectManager);
        virtual     ~VcsNodePrincipalSelectionWorker        ();
        static      VcsNodePrincipalSelectionWorker *getInstance ();

        string      getClassName                            ();

        ResourceId        DomainValidEventHandler                 (const VcsNodeDomainValidEvent *&pEvent);
        ResourceId        DomainInvalidEventHandler               (const VcsNodeDomainInvalidEvent *&pEvent);
        ResourceId        DomainReachableEventHandler             (const VcsNodeDomainReachableEvent *&pEvent);
        ResourceId        DomainUnReachableEventHandler           (const VcsNodeDomainUnreachableEvent *&pEvent);
        ResourceId        DomainCapabilitiesAvailableEventHandler (const VcsNodeDomainCapabilitiesAvailableEvent *&pEvent);
        void        addMessageCallback                      (FrameworkStatus frameworkStatus, PrismMessage *pPrismMessage, VcsNodePSMessagingContext *pVcsNodePSMessageSenderContext);
        void        rejoinMessageCallback                     (FrameworkStatus frameworkStatus, PrismMessage *pPrismMessage, VcsNodePSMessagingContext *pVcsNodePSMessageSenderContext);
        void        getRemoteSwitchInfoAsyncCallBack        (FrameworkStatus frameworkStatus, PrismMessage *pPrismMessage, VcsNodePSMessagingContext *pVcsNodePSMessagingContext);
//        void        disable                                 (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases);
//        void        shutdown                                (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases);
//        void        heartBeatFailure                        (PrismAsynchronousContext     *pPrismAsynchronousContext);

//        static  UI32        getMgmtPort                     (UI32 domainId);
//        static  string      getIpAddressFromDomainId        (UI32 domainId);
        static  UI32        getDomainIdFromIpAddressAndPort (string ipAddress, UI32 port);
        static  void        getx86ipAddressAndPort          (const UI32 &x86DomainId, string &x86ipAddress, UI32 &x86port);
//        static  bool        isEnvx86                        ();
        static vector<UI32>     getx86DomainList            ();
        static vector<string>   getx86ipAddresses           ();
        static vector<UI32>     getx86ports                 ();
        static void        getKnownDomainList               (scnDomainsStruct &knownDomainList);
        bool        getClusterFormationFlag                 ();
//        void        cancelPrincipalSelectionTimer           ();


    private :
        ResourceId   sendRejoinSwitchMessageStep                (VcsNodePSMessagingContext *pVcsNodePSMessagingContext);
        ResourceId   sendAddSwitchMessageStep               (VcsNodePSMessagingContext *pVcsNodePSMessagingContext);
        ResourceId   sendEnqMessageStep                     (VcsNodePSMessagingContext *pVcsNodePSMessagingContext);
        void         sendAddSwitchMessageSequencer          (VcsAddSwitchMessage *pAddSwitchMessage, UI32 establishPrincipalCase);
        void         sendRejoinSwitchMessageSequencer     (VcsRejoinSwitchMessage *pRejoinSwitchMessage);
        ResourceId   addSwitchWithSynchronousMessageStep    (VcsNodePSSynchronousMessagingContext *pVcsNodePSMessagingContext);
        void         sendSynchronousAddSwitchMessageSequencer (VcsAddSwitchMessage *pAddSwitchMessage, UI32 establishPrincipalCase);

        void         getSwitchInfoForDomainListAsyncMessageSequencer (u_short *fabDomainList);
        void         getSwitchInfoForDomainListAsyncMessageStep (VcsNodePSMessagingContext *pVcsNodePSMessagingContext);
        void         getRemoteSwitchInfoAsync               (VcsNodePSMessagingContext *pVcsNodePSMessagingContext);

        void         addSwitchesNotInCluster                ();
        void         createClusterAndAddSwitches            ();
        void         deleteObjects                          (vector<WaveManagedObject *> *pResults);
        void         disableTimer                           ();
        void         startPrincipalSelectionTimer           (UI32 timeoutValue);
        void         establishPrincipalTimerWithAsyncEnq    (TimerHandle timerHandle, void * pContext);
        void         becomePrincipal                        ();
          
        void         continueEstablishPrincipalAfterRemoteEnqComplete (VcsNodePSMessagingContext *pVcsNodePSMessagingContext);

        void         establishPrincipalTimer                ();
        void         clearReachableDomainBitForDomainId     (const u_short &unReachableDomain);
        void         clearRemoteSwitchInfoList              ();
        void         clearSwitchInfoRetrievedBitMap         ();
        void         doControlledPrincipalChangeOver        (UI32 fcPrincipalDomainId);
        UI32         establishVcsPrincipal                  (u_short *fabDomainList);

        bool         fabAndMyDomainListsMatch               (u_short *fabDomainList);
        vector<WaveManagedObject *>             *fabOMDoSynchronousQuery        (const string className);
        vector<VcsNodeLocalManagedObject *>     *getNodeLocalManagedObjects     ();

        bool         amIVcsPrincipal                        ();

        bool         isSwitchInfoRetrievedForDomainId       (const u_short &domainId);
        bool         isSwitchDomainIdInMyCluster            (const VcsFabricLocalSwitchInfoObject &switchInfo);
        bool         isSwitchDomainIdInMyCluster            (UI32 mappedId);
        bool         isPrincipalFailoverInProgress          ();
        static bool      isClusterFormationInProgress       ();
        bool         isThisAClusterMerge                    ();
        bool         isThereAnotherVcsPrincipalInCluster    ();

        bool         isMyDomainIdLowest                     (UI32 &minIndex);
        bool         isFabDomainValid                       ();
        bool         validMappedId                          (UI32 mappedId);

        void         performClusterMerge                    ();
        bool         principalFailOverInProgress            ();
        bool         vcsPrincipalExistsInDomainList         ();
        bool         clusterFormationInDomainList           ();

        bool         isNodeRollingBack                      ();

        UI32         wereAllSwitchesAdded                   (BitMap &switchesNotInCluster);

        UI32         getMyDomainId                          ();
        UI32         getMyMappedId                          ();
        VcsNodeLocalManagedObject * getNodeLocalManagedObjectForThisNode ();

        UI32         getLowestDomainId                      (UI32 &minIndex);
        UI32         retrieveRemoteSwitchInfo               (VcsFabricLocalSwitchInfoObject &remoteSwitchInfo);
        UI32         getSwitchInfoForDomainList             (u_short *domainIdList);

        VcsNodeLocalManagedObject * getSwitchObjectFromDomainId (UI32 domainId);
        UI32        getPortFromDomainId                         (UI32 domainId);
        VcsNodeLocalManagedObject * getSwitchObjectFromObjectId (const ObjectId &switchObjectId);
	//        UI32        getMappedIdForSwitchSerialNumber            (string switchSerialNumber);
        void        addRemoteSwitchInfo                         (const VcsFabricLocalSwitchInfoObject &remoteSwitchInfo);
        void        addInvalidParametersSwitchList              (const VcsFabricLocalSwitchInfoObject &invalidParmsSwitchInfo);
        void        addSwitchInfoNotRetrievedList               (const VcsFabricLocalSwitchInfoObject &infoNotRetrievedSwitchInfo);

        void        clearSwitchInfoLists                        ();
        static  void        getLocalSwitchInfo                  (VcsFabricLocalSwitchInfoObject &localSwitchInfo);
        UI32        getDomainsInCluster                         (BitMap &domainsIncluster);
        //UI32        getLocalSwitchMappedId                      ();
        ObjectId    getLocalSwitchObjectId                      ();
        UI32        getLocalNodeVcsId                           ();
        UI32        getVcsClusterId                             (bool &clusterExists);
        static vector<VcsFabricLocalSwitchInfoObject>   getRemoteSwitchInfoList          ();
        static vector<VcsFabricLocalSwitchInfoObject>   getInvalidParametersSwitchList   ();
        static vector<VcsFabricLocalSwitchInfoObject>   getSwitchInfoNotRetrievedList    ();

        bool        transactionMoved                                           ();
        vector<string> getClusterGUIDs                           ();
        vector<string> getVcsClusterGUIDsFromClusterMO           ();

        void         initializeDomainList                   ();
        void         initializeClusterGUIDs                 ();

        bool         myDomainListMatchesRemoteSwitches      ();
        void         relenquishPrincipal                    ();
        UI32         setPrincipalFlag                       (bool principalFlag);

        void         setReachableDomainBitForDomainId       (const u_short &reachableDomain);
        void         setDCADomainBitForDomainId             (const u_short &dcaDomain);
//        void         setDomainValid                         (const bool &domainValidOrInvalid);
        void         setSwitchInfoRetrievedBit              (const u_short &infoRetrievedDomain);
        ResourceId   setThisSwitchInfoInEnqMessage (VcsEnqSwitchMessage *pVcsEnqSwitchMessage);
        void         setBitMapSize                          (BitMap &bitMap);
        void         setSizeForDomainBitMaps                (scnDomainsStruct & domainBitMapStruct);
        static void  setClusterFormationInProgress          (const bool &clusterFormationInProgress);
        void         setVcsClusterGUIDs                     (const vector<string> &vcsClusterGUIDs);
        void         startEstablishPrincipalSteps           ();
        void         startPrincipalCleanupSteps         (PrismHeartbeatFailureObjectManagerMessage *pHeartbeatFailureMessage);

        bool         validDomainId                          (UI32 domainId);
        void         saveRemoteSwitchInfoFromEnqMessage     (VcsEnqSwitchMessage *pVcsEnqSwitchMessage);
        void         updateRemoteSwitchInfoList             (VcsFabricLocalSwitchInfoObject &switchInfo);
        bool         validateSwitchParameters               (VcsFabricLocalSwitchInfoObject &switchInfo);
        static bool  vcsModeEnabled                         ();

//        static void      removeFromx86DomainList            (UI32 &x86DomainId);
//        static void      addTox86DomainList                     (UI32 &x86DomainId);
//        static void      addTox86IpAddressList                  (string &x86IpAddress);
//        static void      addTox86PortList                       (UI32 &x86Port);
        static void         setx86DomainList                (vector<UI32> &x86DomainList);
        static void         setx86ipAddresses               (vector<string> &x86iPAddress);
        static void         setx86ports                     (vector<UI32> &x86ports);
        void                setClusterFormationFlag         (bool clusterFormationFlag);

        void        setFailedNodeIp(string ip);
        string      getFailedNodeIp();
        void        setFailedNodePort(UI32 port);
        UI32        getFailedNodePort();
#if 0
        SNDBObjectStates getSwitchSNDBStateAndMappedId (const string switchSerialNumber, UI32 &mappedId);
#endif
        void        cleanupTimerHandlerForDomainId(UI32 domainId, bool deleteTimer);
        void processCleanupTimerForDomainId (TimerHandle timerHandle, void * pContext);
		void replaySCNQueueEvents ();
		
//        void        setPrincipalFailureFlag (bool principalFailure);
//        bool        getPrincipalFailureFlag ();

    // Now the Data Members

        vector<VcsFabricLocalSwitchInfoObject>    m_remoteSwitchInfoList;
        vector<VcsFabricLocalSwitchInfoObject>    m_invalidParametersSwitchList;
        vector<VcsFabricLocalSwitchInfoObject>    m_switchInfoNotRetrievedList;

        scnDomainsStruct                m_knownDomainsList;
        UI32                            m_dcaDomainListSize;
        static TimerHandle              m_timerHandle;
        static bool                     m_timerRunningFlag;
        bool                            m_fabDomainValid;
        bool                            m_isNodeReadyForSCNHandling;
        bool                            m_willNotBePrincipalFlag;
        bool                            m_clusterFormationFlag;
        PrismMutex                      m_prismMutex;
        bool                            m_principalFailureFlag;
        PrismMutex                      m_principalFailureMutex;

        vector<VcsNodeTimerHandler*>    m_vcsNodeTimerHandler;

        string                          m_failedNodeIp;
        UI32                            m_failedNodePort;

        vector<UI32>                    m_x86domainList;
        vector<string>                  m_x86ipAddresses;
        vector<UI32>                    m_x86ports;
        vector<string>                  m_vcsClusterGUIDs;

        friend class VcsFabricLocalObjectManager;
        friend class VcsFabObjectManager;
        friend class VcsNodeRemoteMessagingObjectManager;

    protected :
    public :

};

}

#endif // VCSNODEPRINCIPALSELECTIONWORKER_H
