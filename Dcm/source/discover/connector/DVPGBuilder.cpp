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
 * DVPGBuilder.cpp
 *
 *  Created on: Apr 23, 2012
 *      Author: rmadapur
 */

#include "DVPGBuilder.h"

namespace DcmNs {

    DVPGBuilder::DVPGBuilder(ServiceInstance *_serviceInstance) :
            AssetBuilder(_serviceInstance, "DistributedVirtualPortgroup") {
        //Properties

        properties.push_back("config.name");
        properties.push_back("config.distributedVirtualSwitch");
        properties.push_back("config.defaultPortConfig");
        properties.push_back("tag");

    }

    DVPGBuilder::~DVPGBuilder() {
        // TODO Auto-generated destructor stub
    }

    void DVPGBuilder::extractData(vector<ObjectContent*> * objectContentList,
            size_t i, Asset * _dvpg) {
        DVPG *dvpg = (DVPG *) _dvpg;
        ObjectContent *objContent = objectContentList->at(i);
        dvpg->id = objContent->obj->getVal();
        dvpg->uplinktype = "regular";
        VIC_PLUG_LOG(VIC_TRACE_DEBUG,
                "dvpg_id %s\n", dvpg->id.c_str());

        vector<DynamicProperty*> *propSet = objContent->propset;
        for (size_t j = 0; j < propSet->size(); j++) {
            VIC_PLUG_LOG(VIC_TRACE_DEBUG,"Object type %s\n", propSet->at(j)->objType.c_str());
            if (propSet->at(j)->objType.compare("xsd:string") == 0) {
                string *str = (string*) (propSet->at(j)->obj);
                dvpg->name = str->c_str();
            }
            else if (propSet->at(j)->objType.compare("VMwareDVSPortSetting")
                    == 0) {
                VMwareDVSPortSetting *_obj =
                        (VMwareDVSPortSetting*) (propSet->at(j)->obj);
                if (_obj->type.compare(
                        "VmwareDistributedVirtualSwitchTrunkVlanSpec") == 0) {
                    VmwareDistributedVirtualSwitchTrunkVlanSpec *spec =
                            (VmwareDistributedVirtualSwitchTrunkVlanSpec*) (_obj->vlan);
                    vector<NumericRange*> *rangeList = spec->vlanId;
                    for (uint s = 0; s < rangeList->size(); s++) {
                        if( rangeList->at(s)->start.compare(rangeList->at(s)->end)==0)
                        {
                            dvpg->vlan = dvpg->vlan + rangeList->at(s)->start +",";
                        }else{
                            dvpg->vlan = dvpg->vlan + rangeList->at(s)->start + "-"
                                + rangeList->at(s)->end + ",";
                        }
                    }
                }
                else if (_obj->type.compare(
                        "VmwareDistributedVirtualSwitchVlanIdSpec") == 0) {
                    VmwareDistributedVirtualSwitchVlanIdSpec *spec =
                            (VmwareDistributedVirtualSwitchVlanIdSpec*) (_obj->vlan);
                    VIC_PLUG_LOG(VIC_TRACE_DEBUG,"VMwareDVSPortSetting  vlan  %s\n",
                            spec->vlanId.c_str());
                    dvpg->vlan = dvpg->vlan + spec->vlanId + ",";
                }
                VIC_PLUG_LOG(VIC_TRACE_DEBUG,"VMwareDVSPortSetting  networkResourcePoolKey  %s\n",
                        _obj->networkResourcePoolKey.value.c_str());
                dvpg->networkResourcePoolKey = _obj->networkResourcePoolKey.value;

            }
            else if (propSet->at(j)->objType.compare(
                    "ArrayOfManagedObjectReference") == 0) {
               /* vector<ManagedObjectReference*> *morlist = (vector<
                        ManagedObjectReference*>*) (propSet->at(j)->obj);
                for (size_t s = 0; s < morlist->size(); s++) {
                    ManagedObjectReference *mor = morlist->at(s);
                }*/
            }
            else if (propSet->at(j)->objType.compare(
                    "VmwareDistributedVirtualSwitch") == 0) {
                ManagedObjectReference *mor =
                        (ManagedObjectReference*) (propSet->at(j)->obj);
                dvpg->dvs_id = mor->val;
            }
            else if (propSet->at(j)->objType.compare("ArrayOfTag") == 0) {
                vector<Tag*> *tagList = (vector<Tag*>*) (propSet->at(j)->obj);
                if (tagList->size() != 0) {
                    dvpg->uplinktype = "uplink";
                }
                for (size_t s = 0; s < tagList->size(); s++) {
                  //  Tag *tag = tagList->at(s);
                }
            }

        }
        if(dvpg->vlan.compare("")==0)
		    dvpg->vlan="0";

    }

    int DVPGBuilder::getAsset(vector<Asset*> & assetVector) {
        return fetchFromVCenter<DVPG>(assetVector);
    }
    int DVPGBuilder::getAsset(string morVal, Asset **asset) {
        return fetchFromVCenter<DVPG>(morVal, asset);
    }

} /* namespace DcmNs */
