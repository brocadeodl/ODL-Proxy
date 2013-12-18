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
 * VMBuilder.cpp
 *
 *  Created on: Apr 24, 2012
 *      Author: rmadapur
 */

#include "VMBuilder.h"

namespace DcmNs {

    VMBuilder::VMBuilder(ServiceInstance *_serviceInstance) :
            AssetBuilder(_serviceInstance, "VirtualMachine") {

        properties.push_back("config.name");
        properties.push_back("config.template");
        properties.push_back("config.hardware.device");
        properties.push_back("runtime.powerState");
        properties.push_back("runtime.host");
        properties.push_back("guest.net");

    }

    VMBuilder::~VMBuilder() {

    }

    void VMBuilder::extractData(vector<ObjectContent*> * objectContentList,
            size_t i, Asset * _vm) {
        VM *vm = (VM *) _vm;
        ObjectContent *objContent = objectContentList->at(i);
        vm->id = objContent->obj->getVal().c_str();
        vector<DynamicProperty*> *propSet = objContent->propset;
        for (size_t j = 0; j < propSet->size(); j++) {
        VIC_PLUG_LOG(VIC_TRACE_DEBUG,
                "type = \n%s\n", propSet->at(j)->objType.c_str());
            if (propSet->at(j)->objType.compare("xsd:string") == 0) {
                string *value = (string*) (propSet->at(j)->obj);
                string name = propSet->at(j)->name;
                if(name.compare("config.name")==0){
                    vm->name = value->c_str();
                }
            }
            else if (propSet->at(j)->objType.compare("HostSystem") == 0) {
                ManagedObjectReference *mor =
                        (ManagedObjectReference*) (propSet->at(j)->obj);
VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s, %s.","Host System ",mor->val.c_str());

                vm->host_id = mor->val;
            }
            else if (propSet->at(j)->objType.compare("xsd:boolean") == 0) {
                string *str = (string*) (propSet->at(j)->obj);
                vm->isTemplate = str->compare("true") == 0;
            }
            else if (propSet->at(j)->objType.compare("ArrayOfGuestNicInfo")
                    == 0) {
                vector<GuestNicInfo*> *_objList =
                        (vector<GuestNicInfo*>*) (propSet->at(j)->obj);
                for (size_t s = 0; s < _objList->size(); s++) {
                    GuestNicInfo *_obj = _objList->at(s);

                    string ip = "";
                    vector<NetIpConfigInfoIpAddress *> *ipaddress =
                            _obj->ipConfig.ipAddress;
                    for (size_t p = 0; p < ipaddress->size(); p++) {
                        NetIpConfigInfoIpAddress *ipaddressObj = ipaddress->at(
                                p);

                        if (ip.compare("") == 0) {
                            ip = ipaddressObj->ipAddress;
                        }
                        else {
                            ip = ip + "," + ipaddressObj->ipAddress;
                        }

                    }
                    vm->ipMacMap[_obj->macAddress] = ip;

                }
            }
            else if (propSet->at(j)->objType.compare("ArrayOfVirtualDevice")
                    == 0) {
                vector<VirtualDevice*> *_objList =
                        (vector<VirtualDevice*>*) (propSet->at(j)->obj);
                for (size_t s = 0; s < _objList->size(); s++) {
                    VirtualDevice *_obj = _objList->at(s);
                    if (_obj->backingType.compare(
                            "VirtualEthernetCardNetworkBackingInfo") == 0) {
                        VNics *vnic = new VNics();
                        vnic->key = _obj->key;
                        vnic->vm_id = vm->id;
                        vnic->mac = _obj->macAddress;
                        vnic->name = _obj->deviceInfo.label;
                        vnic->dvpg_nn = _obj->deviceInfo.summary;
                        vnic->isdistributed = false;
                        VirtualEthernetCardNetworkBackingInfo *backing =
                                (VirtualEthernetCardNetworkBackingInfo*) (_obj->backing);

                        vnic->dvpg_id = backing->network.val;
                        vm->vnicsList.push_back(vnic);
                    }
                    else if (_obj->backingType.compare(
                            "VirtualEthernetCardDistributedVirtualPortBackingInfo")
                            == 0) {
                        VNics *vnic = new VNics();
                        vnic->key = _obj->key;
                        vnic->vm_id = vm->id;
                        vnic->mac = _obj->macAddress;
                        vnic->name = _obj->deviceInfo.label;
                        vnic->isdistributed = true;
                        VirtualEthernetCardDistributedVirtualPortBackingInfo *backing =
                                (VirtualEthernetCardDistributedVirtualPortBackingInfo*) (_obj->backing);
                        /* printf("Virtual Device  portKey %s\n",
                         backing->port.portKey.c_str());
                         printf("Virtual Device portgroupKey %s\n",
                         backing->port.portgroupKey.c_str());*/
                        vnic->dvpg_id = backing->port.portgroupKey;
                        //TODO Populate dvpg_nn from DVPG
                        vm->vnicsList.push_back(vnic);
                    }

                }

            }

        }

    }

    int VMBuilder::getAsset(string morVal, Asset **asset) {
        return fetchFromVCenter<VM>(morVal, asset);
    }

    int VMBuilder::getAsset(vector<Asset*> & assetVector) {
        return fetchFromVCenter<VM>(assetVector);
    }

    int VMBuilder::getAsset(vector<string> morValList, vector<Asset*> & assetVector) {
        return fetchFromVCenter<VM>(morValList, assetVector);
    }


} /* namespace DcmNs */
