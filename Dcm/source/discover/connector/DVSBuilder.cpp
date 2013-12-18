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
 * DVSBuilder.cpp
 *
 *  Created on: Apr 23, 2012
 *      Author: rmadapur
 */

#include "DVSBuilder.h"
#include "discover/vsphere/vim25/ServiceContent.h"

namespace DcmNs {

    DVSBuilder::DVSBuilder(ServiceInstance *_serviceInstance) : 
            AssetBuilder(_serviceInstance, "VmwareDistributedVirtualSwitch") {

        AboutInfo about;
        about = _serviceInstance->serviceContent->getAbout();

        if(about.getApiVersion() != "4.0" )
            properties.push_back("networkResourcePool");

        properties.push_back("config.name");
        properties.push_back("config.uplinkPortgroup");
        properties.push_back("config.host");

    }

    DVSBuilder::~DVSBuilder() {
        // TODO Auto-generated destructor stub
    }
    void DVSBuilder::extractData(vector<ObjectContent*> * objectContentList,
            size_t i, Asset * _dvs) {
        DVS *dvs = (DVS *) _dvs;

        ObjectContent *objContent = objectContentList->at(i);
        dvs->id = objContent->obj->getVal().c_str();

        vector<DynamicProperty *> *propSet = objContent->propset;
        for (size_t j = 0; j < propSet->size(); j++) {

            if (propSet->at(j)->objType.compare("xsd:string") == 0) {
                string *str = (string *) propSet->at(j)->obj;
                dvs->name = str->c_str();
            }
            else if (propSet->at(j)->objType.compare(
                    "ArrayOfManagedObjectReference") == 0) {
                vector<ManagedObjectReference*> *morlist = (vector<
                        ManagedObjectReference*> *) propSet->at(j)->obj;
                for (size_t s = 0; s < morlist->size(); s++) {
                    ManagedObjectReference *mor = morlist->at(s);
                    if (mor->type.compare("DistributedVirtualPortgroup") == 0) {
                        dvs->prgroup_nn = mor->val;
                        dvs->prgroup_id = mor->val;
                        break;
                    }

                }
            }
            else if (propSet->at(j)->objType.compare(
                    "ArrayOfDistributedVirtualSwitchHostMember") == 0) {
                vector<DistributedVirtualSwitchHostMember*> *list =
                        (vector<DistributedVirtualSwitchHostMember*> *) propSet->at(
                                j)->obj;
                for (size_t s = 0; s < list->size(); s++) {
                    DistributedVirtualSwitchHostMember *_obj = list->at(s);
                    VIC_PLUG_LOG(VIC_TRACE_DEBUG,"DVS id %s\n", dvs->id.c_str());
                    VIC_PLUG_LOG(VIC_TRACE_DEBUG," DistributedVirtualSwitchHostMember Name  %s %s\n ",
                            _obj->config.host.type.c_str(),
                            _obj->config.host.val.c_str());
                    string host_mor = _obj->config.host.val;
                    dvs->host_mors.push_back(host_mor);

                    vector<DistributedVirtualSwitchHostMemberPnicSpec *> *pnicSpecList =
                            _obj->config.backing.pnicSpec;
                    for (size_t l = 0; l < pnicSpecList->size(); l++) {
                        VIC_PLUG_LOG(VIC_TRACE_DEBUG,
                                "DistributedVirtualSwitchHostMember   PniSpec uplinkPortgroupKey %s\n",
                                pnicSpecList->at(l)->uplinkPortgroupKey.c_str());
                        VIC_PLUG_LOG(VIC_TRACE_DEBUG,
                                "DistributedVirtualSwitchHostMember   PniSpec pnicDevice %s\n",
                                pnicSpecList->at(l)->pnicDevice.c_str());

                        dvs->host_pnicList.insert(
                                pair<string, string>(host_mor,
                                        pnicSpecList->at(l)->pnicDevice));

                    }
                }
            }
            else if (propSet->at(j)->objType.compare(
                                "ArrayOfDVSNetworkResourcePool") == 0) {
                            vector<DVSNetworkResourcePool*> *list = (vector<
                                    DVSNetworkResourcePool*> *) propSet->at(j)->obj;
                            for (size_t s = 0; s < list->size(); s++) {
                                DVSNetworkResourcePool *_obj = list->at(s);
                                VIC_PLUG_LOG(VIC_TRACE_DEBUG,"DVS id %s\n", dvs->id.c_str());
                                VIC_PLUG_LOG(VIC_TRACE_DEBUG," DVSNetworkResourcePool Key  %s %s\n ",
                                        _obj->key.c_str(),
                                        _obj->allocationInfo.priorityTag.c_str());
                                string qos = _obj->allocationInfo.priorityTag.c_str();
                                if(qos.size()!=0){
                                    string key = _obj->key;
                                    dvs->nrp_qosList.insert(  pair<string, string>(key,
                                                    qos));
                                }

                            }
                        }

        }
    }
    int DVSBuilder::getAsset(string morVal, Asset **asset) {
        return fetchFromVCenter<DVS>(morVal, asset);
    }
    int DVSBuilder::getAsset(vector<Asset *> &assetVector) {
        return fetchFromVCenter<DVS>(assetVector);
    }

} /* namespace DcmNs */
