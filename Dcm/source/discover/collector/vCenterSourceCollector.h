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
 * vCenterSourceCollector.h
 *
 *  Created on: Apr 24, 2012
 *      Author: rmadapur
 */

#ifndef VCENTERSOURCECOLLECTOR_H_
#define VCENTERSOURCECOLLECTOR_H_
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
#include  "discover/connector/NetworkBuilder.h"
#include  "discover/connector/DVPGBuilder.h"
#include  "discover/connector/VMBuilder.h"
#include "discover/collector/SourceCollector.h"
#include "discover/Framework/discoverMessages.h"
#include "discover/User/DiscoverMultiWorker.h"


#include <memory>
#include <map>
#include <string>
#include "VirtualDb/DAO/DAO.h"
#include "VirtualDb/Domain/SpecialNicDomain.h"

using namespace std;
namespace DcmNs {




    class vCenterSourceCollector: public SourceCollector {

        public:
            string vcenter;
            string url;
            string user;
            string password;
            UI32 ignoreValue;
            bool always;
            string sm_uuid;

            DISCOVERY_TYPE discType;
            DISCOVERY_STATE nextSuccesState;
            map<string,string> vmDataCenterMap;
            map<string,string> hostDataCenterMap;
            map<string,string> networkDataCenterMap;
            map<string,string> dvsDataCenterMap;
            map<string,string> dvpgDataCenterMap;
            map<string,string> dvpgidTodvpgNameMap;
            map<string,string> dvsidTodvsNameMap;
            map<string, string> nrpToQosMap;
            // special nics with no DVPG name.
            vector <SpecialNicDomain> spnicVector;
            //map<string, Network *> networkNameNetworkIdMap;

            vCenterSourceCollector(string vcenter,string url,string user,string password,UI32 ignoreValue, bool always,string sm_uuid, DISCOVERY_TYPE discType,DISCOVERY_STATE nextSuccesState);
            void run();
            virtual ~vCenterSourceCollector();

        private:
            int retrieveHost(ServiceInstance *serviceInstance);
            void checkAndPrintError(int vcRes, Fault *fault);
            int intitializeEnvironMent();
            int retrieveDataCenter(ServiceInstance *serviceInstance);
            int retrieveDVS(ServiceInstance *serviceInstance);
            int retrieveDVPGS(ServiceInstance *serviceInstance);
            int retrieveVMS(ServiceInstance *serviceInstance);
            int populateDVPGwithDVSName();
            int populateSPNICwithDVPGName();
            int fetchDVPGForBasicDsicovery();
            bool ignoreDeleteAllResponse(string vcenterName);
            bool isDeleteAllResponse(string vcenter, daoType_t type);
            vector<Domain*> getSource(daoType_t type, std::string db_file, std::string mor = "");
            void releaseSource(vector<Domain*> v);
            virtual void cleanup();
            void updatevCenterExitStatus(string vcenter, int phase, int code);

    };

} /* namespace DcmNs */
#endif /* VCENTERSOURCECOLLECTOR_H_ */
