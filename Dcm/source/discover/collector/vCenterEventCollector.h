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


#ifndef VCENTEREVENTCOLLECTOR_H_
#define VCENTEREVENTCOLLECTOR_H_
#include "discover/vsphere/vim25/ServiceContent.h"
#include "discover/vsphere/vim25/ListUtil.h"
#include "discover/vsphere/vim25/ManagedObjectReference.h"
#include "discover/vsphere/vim25/ws/vimStub.h"

#include "discover/vsphere/vim25/ObjectSpec.h"
#include "discover/vsphere/vim25/TraversalSpec.h"
#include "discover/vsphere/vim25/PropertySpec.h"
#include "discover/vsphere/vim25/PropertyFilterSpec.h"
#include "discover/vsphere/vim25/HostVirtualNic.h"
#include "discover/vsphere/vim25/PhysicalNic.h"
#include "discover/vsphere/vim25/HostPortGroup.h"
#include "discover/vsphere/vim25/HostProxySwitch.h"
#include "discover/vsphere/vim25/HostVirtualSwitch.h"
#include "discover/vsphere/vim25/DistributedVirtualSwitchHostMember.h"
#include "discover/vsphere/vim25/VMwareDVSPortSetting.h"
#include "discover/vsphere/vim25/VirtualMachinePowerState.h"
#include "discover/vsphere/vim25/VirtualDevice.h"
#include "discover/vsphere/vim25/Tag.h"
#include "discover/vsphere/vim25/EventFilterSpec.h"
#include "discover/vsphere/vim25/mo/ServiceInstance.h"

#include "discover/vsphere/vim25/mo/ViewManager.h"
#include  "discover/connector/Asset.h"
#include  "discover/connector/AssetBuilder.h"
#include  "discover/connector/DataCenterBuilder.h"
#include  "discover/connector/DVSBuilder.h"
#include  "discover/connector/HostBuilder.h"
#include  "discover/connector/DVPGBuilder.h"
#include  "discover/connector/VMBuilder.h"
#include "discover/collector/SourceCollector.h"
#include "discover/Framework/discoverMessages.h"
#include "discover/User/DiscoverMultiWorker.h"
#include "discover/Framework/EventConcurrentQueue.h"


#include <memory>
#include <map>
#include <string>
#include <iostream>
#include <list>
#include <queue>
using namespace std;
namespace DcmNs {
#define MAX_EVENT_QUEUE_SIZE        5
#define EVENT_QUEUE_TIMEOUT       10000
    typedef enum
    {
        EVT_DISCOVER = 0,
        EVT_EVENT
    } DiscoverEvents;

    class vCenterEventCollector: public SourceCollector {

        public:
            string vcenter;
            string url;
            string user;
            string password;
            string sm_uuid;
            TimerHandle eventTimerHandle;
            EventConcurrentQueue eventsQueue;

            vCenterEventCollector(string vcenter,string url,string user,string
                password, string sm_uuid);
            void run();
            virtual ~vCenterEventCollector();
            virtual void cleanup();
            void stopEvents();


        private:
            //            vCenterEventConcurrentQueue eventsQueue;
            void handleEventAsynchronously(Event * event);
            void processEventTimerExpiry( TimerHandle timerHandle, void * pContext);
            string getVCPass(string _vcenter);
            void restartEventTimer(); 
            void stopTimer(); 


            void queryEvents(ServiceInstance *serviceInstance);
            void handleEvent(Event *event, ServiceInstance* serviceInstance);
            void handleUpdates(UpdateSet *updateSet, ServiceInstance* serviceInstance);
            EventFilterSpec getEventFilterSpec();
            PropertyFilterSpec createEventFilterSpec(ManagedObjectReference *eventHistoryCollector);
            void checkAndPrintError(int vcRes, Fault *fault);
            UI32 sendUpdateDiscoveryStatus(string vcenter, UI32 state, UI32 exitStatus);
            UI32 startDiscoveryAfterReconnect (string vcentername, string url, string user, string pass);
    };


} /* namespace DcmNs */
#endif /* VCENTEREVENTCOLLECTOR_H_ */
