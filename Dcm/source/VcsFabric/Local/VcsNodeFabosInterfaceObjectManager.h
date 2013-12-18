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

 /**************************************************************************
 *   Copyright (C) 2010-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vipool Prajapati                                             *
 ***************************************************************************/

#ifndef VCSNODEFABOSINTERFACEOBJECTMANAGER_H
#define VCSNODEFABOSINTERFACEOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveLocalObjectManagerForUserSpecificTasks.h"
#include "utils/mqueue.h"
#include "sysmod/syscomm.h"
#include "VcsFabric/Local/VcsNodeLocalTypes.h"
#include "VcsFabric/Local/VcsNodeLocalLinecardTypes.h"
#include "Framework/Types/MacAddress.h"
#include "Framework/Boot/BootCompleteForThisLocationEvent.h"
#include "VcsFabric/Local/VcsNodeFabosScnQueue.h"
#include "VcsFabric/Local/VcsNodeSlotMessagingContext.h"
#include <deque>
#include "sys/fabos/fabsys.h"
extern "C"
{
#include "sys/fss/fss.h"
#include "isc/isc.h"
#include "ipc/ipc.h"
}

using namespace WaveNs;

namespace DcmNs
{

class VcsNodeLocalManagedObject;
class VcsNodeFabosScnQueueObject;
class VcsNodeFabosSlotScnQueueObject;
class EarlyConfigurationReplayWorker;

typedef struct {
    bool        m_vcsMode;
    UI32        m_vcsId;
    UI32        m_rbridgeId;
    UI32        m_clusterType;
    UI32        m_startupFileType;
    char        m_startupFileName[256];
    SI32        m_clusterConf;
} dcmConfigFssSyncData_t;

class VcsNodeFabosInterfaceObjectManager : public WaveLocalObjectManagerForUserSpecificTasks
{
    private :
                                            VcsNodeFabosInterfaceObjectManager ();

                void                        install                             (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
                void                        initialize                          (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
                void                        boot                                (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
                PrismEvent                  *createEventInstance                (const UI32 &eventOperationCode);

                VcsNodeLocalManagedObject*  getLocalManagedObject               ();
                void                        handleZoneChanged                   ();
                void                        handleZoneAborted                   ();
                void                        handleSwitchBootupStateUpdate       (UI32 state);
        virtual void                        listenForEvents                     (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
                void                        bootCompleteForThisLocationEventHandler (const BootCompleteForThisLocationEvent *&pBootCompleteForThisLocationEvent);
                UI32                        queueThisScn                        (UI32 eventType,UI32 domainId, UI32 fcPrincipalDomainId);
                ResourceId                  popPScnQueueObject                  (UI32 eventOid);
                void                        pushPScnQueueObject                 (VcsNodeFabosScnQueueObject * pScnQueueObject);
                VcsNodeFabosScnQueueObject* getFrontScnQueueObject              ();
                            
                UI32                        scnQueueSize                        ();
                void                        initializeSCNQueue                  ();
                UI32                        slotScnQueueSize                    ();
                ResourceId                  popPSlotScnQueueObject              (UI32 eventOid);
                void                        pushPSlotScnQueueObject             (VcsNodeFabosSlotScnQueueObject* pSlotScnQueueObject);
                VcsNodeFabosSlotScnQueueObject* getFrontSlotScnQueueObject      ();

                UI32                        queueThisSlotScn (UI32 slotNumber, UI32 objectType, UI32 hwType, UI32 hwId, UI32 slotState, UI32 reason, UI32 version);

        static  bool                        getIsConfigInitDone                 ();
        static  int                         dcmFSSSendDumpNull                  (void *context, int vers, int tid);
        static  int                         dcmFSSRecvDumpNull                  (void *context, int vers, int sgno);
        static  int                         dcmFSSSendDumpConfig                (void *context, int vers, int tid);
        static  int                         dcmFSSSendConfigUpdate              (int fssh, int vers, int tid);
        static  int                         dcmFSSRecvConfigUpdate              (void *context, fssd_udb_t *updt);
                int                         dcmFssHandleConfigUpdate            (dcmConfigFssSyncData_t *dcmConfigData);
                void                        dcmFssNotifyStartSyncDump           ();
                int                         dcmInitHaIpAddress                  (string &localIp, SI32 &localPort);
                int                         terminateDcmHaSync                  ();
                void                        disconnectFromHaPeer                ();
        static  void                        dcmNotifyHasmConfigReplayDone       ();
        static  void                        dcmNotifyHasmSlotPostbootDone       (UI32 &slotId);
        static  ResourceId                  waitForWyserEaStartPhasesCompletion   ();
                int                         dcmNotifyPostBootEvents             (UI32 &eventType, UI32 &slotId, ResourceId &recoveryType);
                UI64                        getBreakoutConfig (VcsNodeSlotMessagingContext *pContext);


    protected :
    public :
        virtual                             ~VcsNodeFabosInterfaceObjectManager ();
                void                         setChassisString                   (string chassisString);

        static  VcsNodeFabosInterfaceObjectManager*    getInstance              ();
        static  PrismServiceId               getPrismServiceId                  ();
        static  string                       getClassName                       ();

        static  int                          vcsNodeAspInit                     ();
        static  int                          vcsNodeAspExit                     ();
        static  void                         vcsNodeServe                       (msg_t *msg);
        static  int                          dcmCtor                            (said_t *aid, mq_t mq, smi_cfg_t *config);
        static  int                          dcmDtor                            (said_t *aid);
        static  int                          dcmCtorChassis                     (mq_t mq);
        static  int                          dcmCtorFcsw                        (mq_t mq);
        static  int                          dcmRecoverChassis                  (void *context, int stage);
        static  int                          dcmRecoverEthsw                    (void *context, int stage);
        static  int                          dcmRecoverFcsw                     (void *context, int stage);
        static  int                          dcmNotify                          (said_t *aid, smi_evt_t *evt);

                void                         handleDomainMessage                (VcsNodeDomainEvent event, UI32 domainId, UI32 principalDomainId);
                void                         handlePortMessage                  (VcsNodeDomainEvent event, int portNumber, int reasonCode);
        static  ResourceId                   fssStageDebugMessageHandler        (UI32 argc, vector<string> argv);
        static  void                         setChassisStringWrapper            (string chassisString);
        static  void                         setSwmodelNameWrapper              (const string &swmodelName);
        static  void                         setIsDcmdReadyForHasmLaunch        (bool isDcmdReadyForHasmLaunch);
        static  bool                         getIsDcmdReadyForHasmLaunch        ();
        static  void                         setIsHasmLaunched                  (bool isHasmLaunched);
        static  bool                         getIsHasmLaunched                  ();
        static  bool                         getIsSyncEnabled                   ();
        static  void                         setIsSyncEnabled                   (bool syncEnabled);
        static  void                         dcmHASyncConfigUpdate              ();
        static  void                         dcmHaSyncDumpEnd                   (SI32 status);
        static  bool                         dcmHaSyncUpdateFailure             (const ResourceId &reason);
        static  void                         dcmNotifyHasmClusterReadyState      (bool &readyState);
        static  void                         handleSlotEvent                    (UI32 &slotNumber, UI32 &objectType, UI32 &hwType, UI32 &hwId, UI32 &slotState, UI32 &reason, UI32 &version);
        void                                 handleSlotOnlineEvent              (UI32 &slotNumber, UI32 &objectType, UI32 &hwType, UI32 &hwId, UI32 &slotState, UI32 &reason, UI32 &version);
        void                                 handleSlotOfflineEvent             (UI32 &slotNumber, UI32 &objectType, UI32 &hwType, UI32 &hwId, UI32 &slotState, UI32 &reason, UI32 &version);
        int                                  getDomainId                        (); 
        void                                 setDomainId                        (int domainId);
        static  int                          getActivePartitionIdBySlot         (const UI32 &slotId);
        static  void                         setActivePartitionIdBySlot         (const UI32 &slotId, const int &partitionId);
        static  int                          dcmIscReceive                      (void *ctxt, isc_msg_t *msg, int reqh);
        static  int                          dcmIpcCallback                     (IPC_ADDR *addr, MSG_TYPE type, char *buf, size_t size, void *stuff);
        void                                 handleSlotTypeMessage              (int reqh, int type, UI32 slotNumber, UI32 hwType);
        void                                 getSlotObjects                     (VcsNodeSlotMessagingContext *pVcsNodeSlotMessagingContext);
        void                                 getSlotType                        (VcsNodeSlotMessagingContext *pVcsNodeSlotMessagingContext);
        void                                 replySlotQueryIscMessage            (VcsNodeSlotMessagingContext *pVcsNodeSlotMessagingContext);
        static  int                          handleDcmFssConfigUpdate           (void *dcmConfigData);
        static  void *                       getDcmConfigData                   (UI32 & dcmConfigDataSize);
        static  bool                         getIsActiveNode                    ();

        string                              getSwmodelName                      () const;
        void                                setSwmodelName                      (const string &swmodelName);
        bool                                isScnEventInQueue                   (UI32 eventId);
        bool                                isSlotEventInQueue                  (UI32 eventId);
        string                              getIpAddressBySlotParitionId        (const UI32 &slotId, const int &partitionId);
        bool                                isWarmRecoveryInvolvedInActivation  ();
        void                                setWarmRecoveryInvolvedInActivation (const bool &warmRecovery);

        static  bool                        getIssuInProgress                   (void);
        static  void                        setIssuInProgress                   (const bool &flag);

    // Now the Data Members

    private :
    static mq_t                         m_VcsMsgQ;
    static sysModHandle_t*              m_sysmodh;
    swbd_model                          m_swModel;
    MacAddress                          m_switchMac;
    WorldWideName                       m_switchWWN;
    string                              m_chassisString;
    std::deque<VcsNodeFabosScnQueueObject *> m_scnQueue;
    PrismMutex                          m_scnQueueMutex;
    
           int                          m_domainId;
     PrismMutex                         m_domainIdMutex;
    static bool                         m_isDcmdReadyForHasmLaunch;
    static bool                         m_isHasmLaunched;
    static bool                         m_isConfigInitDone;
    string                              m_fwVersion;
    static int                          m_fssHandles[3];
    static int                          m_iscHandle;
    static bool                         m_syncEnabled;
    static PrismMutex                   m_syncEnabledMutex;
    static bool                         m_syncUpdateOn;
    static UI32                         m_linecardState[MAX_LINECARD_NUM];
    static int                          m_linecardActivePartitionId[MAX_LINECARD_NUM];
    PrismMutex                          m_slotScnQueueMutex;
    static bool                         m_isActiveNode;
    std::deque<VcsNodeFabosSlotScnQueueObject *> m_slotScnQueue;
    static EarlyConfigurationReplayWorker     *m_earlyConfigurationReplayWorker;
           string                                   m_swmodelName;
    static PrismMutex                   m_linecardActivePartitionIdMutex;
           bool                         m_isWarmRecoveryInvolvedInActivation;
    static bool                         m_issuInProgress;
    static PrismMutex                   m_issuInProgressMutex;

    protected :
    public :

    friend class VcsFabricLocalObjectManager;
    friend class VcsNodePrincipalSelectionWorker;
    friend class VcsNodeFCPrincipalTrackingWorker;
    friend class VcsNodeSlotManagementWorker;
    friend class EarlyConfigurationReplayWorker;
};

}

#endif // VCSNODEFABOSINTERFACEOBJECTMANAGER_H
