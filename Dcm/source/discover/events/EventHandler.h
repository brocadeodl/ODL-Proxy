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


#ifndef EVENTHANDLER_H_
#define EVENTHANDLER_H_
#include "discover/vsphere/vim25/Event.h"
#include "discover/vsphere/vim25/mo/ServiceInstance.h"
#include "discover/Framework/discoverTypes.h"
#include "discover/StateMachine/DistributionSender.h"
#include "vCenterEventsMessage.h"
#include "discover/connector/Host.h"
#include "discover/connector/DVPG.h"
#include "discover/connector/DVS.h"
#include "EventDefines.h"
#include "discover/utils/StopWatch.h"
#include "discover/utils/FileUtils.h"
#include "discover/connector/Asset.h"
#include "discover/connector/AssetBuilder.h"
#include "discover/connector/DVSBuilder.h"
#include "discover/connector/HostBuilder.h"
#include "discover/connector/VMBuilder.h"
#include "discover/connector/DVPGBuilder.h"
#include "discover/utils/URLEncode.h"
#include "correlator/EventCorrelator.h"
#include "discover/connector/VM.h"

using namespace std;

namespace DcmNs {


    class EventHandler {

        public:
            EventHandler(string vcenter, Event* event);
            virtual ~EventHandler();
            virtual void handle();
            virtual void handle(ServiceInstance *serviceInstance);
        protected:
            string  m_vcenter;
            Event*  m_event;
            class VM* retrieveOneVM(ServiceInstance *, string );
            vector<Asset *> retrieveVMs(ServiceInstance *);
            vector<Asset *> retrieveHosts(ServiceInstance *);
            void vmCreate(ServiceInstance *, string vm_id, string host_id, string host_name, string vc_id, string dc_id);
            void vmDelete();
            void dvpgCreate(ServiceInstance *serviceInstance);
            void dvpgReconf(ServiceInstance *serviceInstance);
            void dvpgDelete(string);

            Host*   retrieveAHost(ServiceInstance *, string);
            Host*   retrieveOneHost(ServiceInstance *, string);
            DVPG*   retrieveOneDVPG(ServiceInstance *, string );
            DVS*    retrieveOneDVS(ServiceInstance *, string );
            void parseOneHost(ServiceInstance *, Host *, string);
            void* printHost(Host* h);
            void printEvent();
            void waitForUpdates(int t);

            void hostCreate(ServiceInstance *, string host_id, string vc_id, string dc_id);
            void hostRemove(string host_id, string vc_id, string dc_id);
            void dvsRemove();
            void dvsRename();
            void updateVmnicsOnHostRemove(string host_id, string vc_id, string dc_id);
            void updateVnicsOnHostRemove(string host_id, string vc_id, string dc_id);
            void updateVmsOnHostRemove(string host_id, string vc_id, string dc_id);
            void updateSnicsOnHostRemove(string host_id, string vc_id, string dc_id);
            void updateVssOnHostRemove(string host_id, string vc_id, string dc_id);
            void updatePgsOnHostRemove(string host_id, string vc_id, string dc_id);
            void vmRename(ServiceInstance *serviceInstance, string host_id, string vm_id, string vm_name, string dc_id);
            void vmReconf(ServiceInstance *serviceInstance, string host_id, string vm_id, string vm_name, string dc_id);

            void sendDbConfigUpdate(vector<Domain*> , UI32 );
            string getTaskStatus(ServiceInstance *serviceInstance,ManagedObjectReference mor);
            int retrieveQosMap(ServiceInstance *serviceInstance, map<string, string> &nrpToQosMap);
            void publishVNICEvent(vector<VNics *> vnicsList);
            bool isHostPresent(string host_id, string vc_id, string dc_id);
    };

} /* namespace DcmNs */
#endif /* EVENTHANDLER_H_ */
