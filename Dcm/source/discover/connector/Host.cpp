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
 * Host.cpp
 *
 *  Created on: Apr 23, 2012
 *      Author: rmadapur
 */

#include "Host.h"
#include "Network.h"
#include "VirtualDb/Domain/PgDomain.h"
#include "VirtualDb/Domain/VsDomain.h"
#include "VirtualDb/Domain/SpecialNicDomain.h"
#include "VirtualDb/Domain/SpecialPgDomain.h"
#include "VirtualDb/Domain/VmnicDomain.h"
#include "VirtualDb/Domain/VnicDomain.h"
#include "VirtualDb/Domain/PgHostVsDomain.h"
#include "VirtualDb/Domain/HostDomain.h"
#include "discover/vsphere/Debug.h"

namespace DcmNs {

    void VMNics::printAsset() {
        VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s:%s:%s:%s:%s", key.c_str() , device.c_str() , mac.c_str() , dvs_nn.c_str() , dvs.c_str() );
    }

    void VSwitch::printAsset() {
        VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s:%s", key.c_str() , name.c_str() );

        VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s", "pnicList:") ;
        vector<string>::iterator it = pnicList.begin();
        while (it != pnicList.end()) {
            VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s", (*it).c_str());   
            it++; 
        }
    }

    void VirtualMachine::printAsset() {
        VIC_PLUG_LOG(VIC_TRACE_DEBUG,"id %s", id.c_str());
    }

    void PortGroup::printAsset() {
        VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s:%s:%s:%s:%s", id.c_str() , name.c_str() , vlan.c_str() , vs_nn.c_str() , vs_id.c_str() );
    }

    void VNics::printAsset() {
        VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s:%s:%s:%s:%s:%s:%s", key.c_str() , name.c_str() , mac.c_str() , dvpg_nn.c_str() , dvpg_id.c_str(), ipaddress.c_str(), vm_id.c_str() );
    }

    void Host::printAsset() {
        VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s:%s:%s", id.c_str() , hostName.c_str() , domainName.c_str());

        vector<Asset*> temp;
        VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s", "vmnicList: " );
        temp = Asset::convertToVectorAsset<VMNics*>(vmnicsList);
        Asset::printAssetVector(temp);
        temp.clear();
        temp = Asset::convertToVectorAsset<VNics*>(vnicsList);
        Asset::printAssetVector(temp);
        temp.clear();
        temp = Asset::convertToVectorAsset<VSwitch*>(vswitchList);
        Asset::printAssetVector(temp);
        temp.clear();
        temp = Asset::convertToVectorAsset<PortGroup*>(portGroupList);
        Asset::printAssetVector(temp);
        temp.clear();
        temp = Asset::convertToVectorAsset<VirtualMachine*>(vmList);
        Asset::printAssetVector(temp);
        temp.clear();
        temp = Asset::convertToVectorAsset<Network*>(nwList);
        Asset::printAssetVector(temp);

    }

    Domain* PortGroup::convertToDomain(string host_mor, string host_name, string dc_mor) {
        if (type_internal == PORT_GROUP) {
            Domain* phv = new PgHostVsDomain();
            phv->setPgId(id);
            phv->setHostId(host_mor);
            phv->setHostNN(host_name);
            phv->setVlan(vlan);
            phv->setVs(vs_id);
            phv->setPgNN(name);
            phv->setDcId(dc_mor);
            return phv;
        }
        else {

            Domain* phv = new SpecialPgDomain();
            phv->setPgId("");
            phv->setHostId(host_mor);
            phv->setHostNN(host_name);
            phv->setVlan(vlan);
            phv->setVs(vs_nn);
            phv->setPgNN(name);
            phv->setDcId(dc_mor);
            return phv;

        }

    }




    Domain* VNics::convertToDomain(string host_mor, string host_name, string dc_mor) {

        Domain* nic = NULL;
        if (vm_id.compare("VMKernel") == 0) {
            //SpecialPgDomain specialPg(vnics->dvpg_nn, dc_id);
            nic = new SpecialNicDomain(key,
                    mac, ipaddress,
                    dvpg_nn,
                    host_mor, "", vm_id, dc_mor, dvpg_id);
        }else {
            nic = new VnicDomain();
            //vnicdomain
            nic->setName(name);
            nic->setMac(mac);
            //            nic->setIp(ipaddress);
            nic->setDvpgId(dvpg_id);
            nic->setDvpgNN(dvpg_nn);
            nic->setVmId(vm_id);
            nic->setDvs(vs);
            nic->setDcId(dc_mor);
        }

        return nic;

    }

    Domain* VSwitch::convertToDomain(string host_mor, string host_name, string dc_mor) {
        Domain* vs = new VsDomain();
        vs->setHostId(host_mor);
        vs->setName(name);
        vs->setDcId(dc_mor);
        vs->setHost(host_name);
        return vs;
    }

    Domain* VMNics::convertToDomain(string host_mor, string host_name, string dc_mor) {
        Domain* vmnic = new VmnicDomain();
        vmnic->setHostId(host_mor);
        vmnic->setName(device);
        vmnic->setMac(mac);
        vmnic->setDvs(dvs);
        vmnic->setDvsNN(dvs_nn);
        return vmnic;
    }

    Domain* Host::convertToDomain(string host_mor, string host_name, string dc_mor) {
        Domain* dom  = new HostDomain();
        //string hostName;
        //string domainName;

        dom->setId(id);
        dom->setName(hostName);
        dom->setDcId(dc_mor);
        return dom;
    }

    Host::Host() {
        // TODO Auto-generated constructor stub

    }

    Host::~Host() {
        ListUtil<VMNics>::clearPointersFromList(vmnicsList);
        ListUtil<VNics>::clearPointersFromList(vnicsList);
        ListUtil<VSwitch>::clearPointersFromList(vswitchList);
        ListUtil<PortGroup>::clearPointersFromList(portGroupList);
        ListUtil<VirtualMachine>::clearPointersFromList(vmList);
        ListUtil<Network>::clearPointersFromList(nwList);
    }

} /* namespace DcmNs */
