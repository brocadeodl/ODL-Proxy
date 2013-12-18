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
 * Host.h
 *
 *  Created on: Apr 23, 2012
 *      Author: rmadapur
 */

#ifndef HOST_H_
#define HOST_H_
#include "discover/connector/Asset.h"
#include "discover/vsphere/vim25/ListUtil.h"
#include "VirtualDb/Domain/Domain.h"
#include "Network.h"
#define PORT_GROUP 0
#define SP_PORT_GROUP 1

namespace DcmNs {

    class VMNics: public Asset {
        public:
            string key;
            string device;
            string mac;
            string dvs_nn;
            string dvs; //vswitch id which has this physical nic
            void printAsset();
            Domain* convertToDomain(std::string host_mor = "", std::string host_name = "", std::string dc_mor = "");
    };

    class VSwitch :public Asset {
        public:
        string key;//config.network.vswitch.key
        string name;//config.network.vswitch.name
        vector<string> pnicList; //pnic Key (refers to jkey in VMNics )
        Domain* convertToDomain(std::string host_mor = "", std::string host_name = "", std::string dc_mor = "");
        void printAsset();
        void findAssetDependentUpdates(Host* host, vector<Asset*>& vmnics);
    };
    class VirtualMachine: public Asset{
        public:
        string id;
        void printAsset();
    };


    class PortGroup: public Asset{
        public:
        int type_internal;
        string id;
        string name;
        string vlan;
        string vs_nn;
        string vs_id;
        PortGroup():type_internal(PORT_GROUP){

        }
        Domain* convertToDomain(std::string host_mor = "", std::string host_name = "", std::string dc_mor = "");
        void printAsset();
    };

    class VNics :public Asset {
        public:
        // these are vmkernel nics. id is device 
        string key; //same as key
        string name;
        string mac;
        string dvpg_nn;
        string dvpg_id;
        string ipaddress;
        string vm_id;
        string vs;
        bool isdistributed;
        Domain* convertToDomain(std::string host_mor = "", std::string host_name = "", std::string dc_mor = "");
        void printAsset();

    };

    class Host: public Asset {
        public:
            string id;
            string hostName;
            string domainName;
            vector<VMNics *> vmnicsList;
            vector<VNics *> vnicsList;
            vector<VSwitch *> vswitchList;
            vector<PortGroup*> portGroupList;
            vector<VirtualMachine *> vmList;
            vector<Network*> nwList;
            Host();
            virtual ~Host();
            void printAsset();
        	Domain* convertToDomain(std::string host_mor = "", std::string host_name = "", std::string dc_mor = "");
    };

} /* namespace DcmNs */
#endif /* HOST_H_ */
