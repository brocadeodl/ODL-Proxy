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

/*
 * DiscoverMultiWorker.h
 *
 *  Created on: May 28, 2012
 *      Author: rmadapur
 */

#ifndef DISCOVERMULTIWORKER_H_
#define DISCOVERMULTIWORKER_H_
#include "Framework/ObjectModel/WaveWorker.h"
#include "discover/User/discoverTimers.h"
#include "discover/User/discoverDcmDef.h"
#include "vCenter/Framework/vCenterManagedObjects.h"
#include "discover/Framework/discoverManagedObjects.h"
#include "discover/Framework/discoverManagementInterfaceMessages.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForShutdownPhases.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "VcsFabric/Local/VcsNodeDomainIdMessages.h"
#include "Framework/Core/FrameworkObjectManagerTypes.h"
#include "Framework/Core/NodeReadyForAllCommandsEvent.h"
#include "discover/WorkerPool/WorkerThreadPool.h"

#include "discover/StateMachine/MultiDiscoveryRoutine.h"
#include "discover/StateMachine/DistributionSender.h"
#include <string>
#include "discover/events/vCenterEventsMessage.h"

using namespace std;
using namespace WaveNs;
namespace DcmNs {

    class DiscoveryStateMachineCollection;

    class StateActivateMessage {

        public:
			//StateActivateMessage(string _vcenter,string _url,string _userName, string _password): vcenter(_vcenter),url(_url),userName(_userName),password(_password),activated(false)
			//{
			//}
            StateActivateMessage(string _vcenter,string _url,string _userName, string _password,UI32 _ignoreValue,bool _always):
                vcenter(_vcenter),url(_url),userName(_userName),password(_password),ignoreValue(_ignoreValue),always(_always),activated(false),runReconcile(false)
                {

                }
            string vcenter;
            string url;
            string userName;
            string password;
            UI32   ignoreValue;
            bool always;
            bool activated;
            bool runReconcile;
            ~StateActivateMessage(){
                VIC_PLUG_LOG(TRACE_LEVEL_INFO,"%s",

                                           "StateActivaateMessage destructor");
            }

    };

    class DiscoverMultiWorker: public WaveWorker {
        friend class MultiDiscoveryRoutine;
        friend class DistributionSender;
        private:
            PrismMutex *m_pSynchronizingMutex;
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(
                    const string &managedClassName);

            void DiscoverDiscoverMessageHandler(
                    DiscoverDiscoverMessage *pDiscoverDiscoverMessage);
            void DiscoverInternalMessageHandler(
                        DiscoverInternalMessage *pDiscoverDiscoverMessage);
            void DiscoverDistributeMessageHandler(
                    DiscoverDistributeMessage *pDiscoverDiscoverMessage);
            void DiscoverDistributeDBMessageHandler(
                       DistributeDBMessage *pDiscoverDistributeDBMessage);
			void DiscoverUpdateMessageHandler(DiscoverUpdateMessage *pDiscoverUpdateMessage);
			void DatacenterEventMessageHandler(
                    DatacenterEventMessage *pEventMessage);
            void VswitchEventMessageHandler(
                    VswitchEventMessage *pEventMessage);
            void VmnicEventMessageHandler(
                    VmnicEventMessage *pEventMessage);
            void PgHostVsEventMessageHandler(
                    PgHostVsEventMessage *pEventMessage);
		    void VMEventMessageHandler(
		    		VMEventMessage *pEventMessage);
		    void DVSEventMessageHandler(
		    		DVSEventMessage *pEventMessage);
		    void DVPGEventMessageHandler(
		    		DVPGEventMessage *pEventMessage);
		    void HostEventMessageHandler(
		    		HostEventMessage *pEventMessage);
		    void VnicEventMessageHandler(
		    		VNICEventMessage *pEventMessage);
		    void SpgEventMessageHandler(
		    		SPGEventMessage *pEventMessage);
		    void SnicEventMessageHandler(
		    		SNICEventMessage *pEventMessage);
		    void DPEventMessageHandler(
		    		VNMDPMessage *pEventMessage);


        public:
            DiscoverMultiWorker(DiscoverObjectManager *pDiscoverObjectManager);
            virtual ~DiscoverMultiWorker();

            static WorkerThreadPool * workerPool;
            static WorkerThreadPool* getWorkerPoolInstance();

            static DiscoveryStateMachineCollection *smColl;
            static DiscoveryStateMachineCollection* getStateMachineCollection();

            static DiscoverMultiWorker *instance;
            static DiscoverMultiWorker * getInstance();
            ResourceId startDiscoveryTimer (TimerHandle &timerHandle, UI32 timeInMilliSeconds, PrismTimerExpirationHandler pPrismTimerExpirationCallback, void *pPrismTimerExpirationContext, PrismElement *pPrismTimerSender);
            ResourceId stopDiscoveryTimer (TimerHandle &timerHandle);
            WaveMessageStatus           sendToLocalPlugins                         (PrismMessage *pPrismMessage, const LocationId &locationId =0);
            void           publishEventsToCluster                     (EventMessage *pPrismMessage);
    		void publishEventsToCluster ( LocalEventMessage *pEventLocalMessage, EventMessage      *pEventMessage);

    		DiscoverManagedObject*      getDiscoverManagedObject(string vcenterName);
            ResourceId deleteDiscoverManagedObject(string vcenterName);
            void       createDiscoverManagedObject(string vcenterName);
            void updateDiscoveryStatus(string vcenterName,
                       discoverStageMachineStates_t state ,discoverExitStatus_t exitStatus=DISC_EXIT_STATUS_SUCCESS);

            void DistributerSendToClusterCallback(
                            WaveSendToClusterContext *ctx_p);
            void DistributerSendDPToClusterCallback(
                            WaveSendToClusterContext *ctx_p);
            void DistributerEventSendToClusterCallback(
                            WaveSendToClusterContext *ctx_p);
            void disable (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhase);
            void       configCdpProtocol(bool enable);
            UI32 getVcCount();

            friend class MultiDiscoveryroutine;



    };

} /* namespace DcmNs */

#endif /* DISCOVERMULTIWORKER_H_ */
