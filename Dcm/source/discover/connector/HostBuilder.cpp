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
 * HostBuilder.cpp
 *
 *  Created on: Apr 23, 2012
 *      Author: rmadapur
 */

#include "HostBuilder.h"

namespace DcmNs {

    HostBuilder::HostBuilder(ServiceInstance *_serviceInstance) :
            AssetBuilder(_serviceInstance, "HostSystem") {
        properties.push_back("name");
        properties.push_back("config.network.dnsConfig.hostName");
        properties.push_back("config.network.dnsConfig.domainName");
        properties.push_back("config.network.pnic");
        properties.push_back("config.network.vswitch");
        properties.push_back("config.network.portgroup");
        properties.push_back("config.network.proxySwitch");
        properties.push_back("config.network.vnic");
        properties.push_back("config.network.consoleVnic");
        properties.push_back("vm");
        properties.push_back("summary.config.product.version");
        properties.push_back("network");
    }

    HostBuilder::HostBuilder(ServiceInstance *_serviceInstance, int level) :
            AssetBuilder(_serviceInstance, "HostSystem") {
		if (level == 1) {
        	properties.push_back("name");
        	properties.push_back("config.network.dnsConfig.hostName");
        	properties.push_back("config.network.dnsConfig.domainName");
		}
    }

    HostBuilder::~HostBuilder() {
        // TODO Auto-generated destructor stub
    }
    void HostBuilder::extractData(vector<ObjectContent*> * objectContentList,
            size_t i, Asset * _host) {
        Host *host = (Host *) _host;

        ObjectContent *objContent = objectContentList->at(i);
        host->id = objContent->obj->getVal().c_str();

        vector<DynamicProperty *> *propSet = objContent->propset;
        for (size_t j = 0; j < propSet->size(); j++) {
            if (propSet->at(j)->objType.compare("xsd:string") == 0) {
                string *str = (string *) propSet->at(j)->obj;
                if (propSet->at(j)->name.compare(
                        "config.network.dnsConfig.hostName") == 0)
                    host->hostName = str->c_str();
                else if (propSet->at(j)->name.compare(
                        "config.network.dnsConfig.domainName") == 0)
                    host->domainName = str->c_str();
            }
            else if (propSet->at(j)->objType.compare("ArrayOfPhysicalNic")
                    == 0) {
                vector<PhysicalNic*> *phyNicList =
                        (vector<PhysicalNic*> *) propSet->at(j)->obj;
                for (size_t s = 0; s < phyNicList->size(); s++) {

                    //Create Vmnic Object
                    VMNics *vmnics = new VMNics();
                    PhysicalNic *phyNic = phyNicList->at(s);
                    vmnics->key = phyNic->key;
                    vmnics->device = phyNic->device;
                    vmnics->mac = phyNic->mac;
                    host->vmnicsList.push_back(vmnics);
                }
            }
            else if (propSet->at(j)->objType.compare("ArrayOfHostVirtualSwitch")
                    == 0) {
                vector<HostVirtualSwitch*> *list =
                        (vector<HostVirtualSwitch*> *) propSet->at(j)->obj;
                for (size_t s = 0; s < list->size(); s++) {
                    HostVirtualSwitch *_obj = list->at(s);

                    VSwitch * vswitch = new VSwitch();
                    vswitch->name = _obj->name;
                    vswitch->key = _obj->key;

                    host->vswitchList.push_back(vswitch);

                    /* vector<string*> *portgroupList = _obj->portgroup;
                     for (size_t k = 0; k < portgroupList->size(); k++) {
                     printf("HostProxySwitch portgroup %s\n",
                     portgroupList->at(k)->c_str());
                     }*/
                    vector<string*> *pnicList = _obj->pnic;
                    for (size_t k = 0; k < pnicList->size(); k++) {
                        vswitch->pnicList.push_back(*pnicList->at(k));
                    }

                }
            }
            else if (propSet->at(j)->objType.compare("ArrayOfHostPortGroup")
                    == 0) {
                //TODO: this is not the right place to get network info
                //for host pgs get it from host.network (the ones with mor like network-)
                vector<HostPortGroup*> *morlist =
                        (vector<HostPortGroup*> *) propSet->at(j)->obj;
                for (size_t s = 0; s < morlist->size(); s++) {
                    HostPortGroup *hportGr = morlist->at(s);
                    PortGroup *portGroup = new PortGroup();
                    portGroup->id = hportGr->key;
                    portGroup->name = hportGr->spec.name;
                    portGroup->vlan = hportGr->spec.vlanId;
                    portGroup->vs_nn = hportGr->spec.vswitchName;
                    portGroup->vs_id = hportGr->vswitch;
                    host->portGroupList.push_back(portGroup);
                }
            }
            else if (propSet->at(j)->objType.compare(
                    "ArrayOfManagedObjectReference") == 0) {
                vector<ManagedObjectReference*> *morlist = (vector<
                        ManagedObjectReference*> *) propSet->at(j)->obj;
                for (size_t s = 0; s < morlist->size(); s++) {
                    ManagedObjectReference *mor = morlist->at(s);
                    if (mor->type.compare("VirtualMachine") == 0) {
                        VirtualMachine *vm = new VirtualMachine();
                        vm->id = mor->val;
                        host->vmList.push_back(vm);
                    }
                    if(mor->type.compare("Network") == 0) {
                        Network *nw = new Network();
                        nw->id = mor->val;
                        host->nwList.push_back(nw);
                    }

                }
            }
            else if (propSet->at(j)->objType.compare("ArrayOfHostVirtualNic")
                    == 0) {
                // these are the vmkernelnics 
                vector<HostVirtualNic*> *hostVirtualNicList = (vector<
                        HostVirtualNic*> *) propSet->at(j)->obj;
                for (size_t s = 0; s < hostVirtualNicList->size(); s++) {
                    HostVirtualNic *hvnic = hostVirtualNicList->at(s);
                    VNics *vnic = new VNics();

                    vnic->key = hvnic->device;
                    vnic->mac = hvnic->spec.mac;
                    vnic->ipaddress = hvnic->spec.ip.ipAddress;
                    vnic->dvpg_nn = hvnic->portgroup;
                    vnic->dvpg_id = hvnic->spec.distributedVirtualPort.portgroupKey;

                    vnic->vm_id = "VMKernel";

                    host->vnicsList.push_back(vnic);
                }
            }
            //TODO: another case for consoleVnics

        }

    }
    int HostBuilder::getAsset(string morVal, Asset **asset) {
            return fetchFromVCenter<Host>(morVal, asset);
        }
    int HostBuilder::getAsset(vector<Asset *> &assetVector) {
        return fetchFromVCenter<Host>(assetVector);

    }

} /* namespace DcmNs */
