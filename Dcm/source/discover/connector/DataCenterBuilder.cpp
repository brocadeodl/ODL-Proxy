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
 * DataCenterBuilder.cpp
 *
 *  Created on: Apr 22, 2012
 *      Author: rmadapur
 */

#include "DataCenterBuilder.h"

namespace DcmNs {

    DataCenterBuilder::DataCenterBuilder(ServiceInstance *_serviceInstance) :
            AssetBuilder(_serviceInstance, "Datacenter") {

        properties.push_back("name");

    }

    DataCenterBuilder::~DataCenterBuilder() {

    }

    void DataCenterBuilder::extractData(
            vector<ObjectContent*> * objectContentList, size_t i, Asset * obj) {
        DataCenter *dataCenter = (DataCenter *) obj;
        ObjectContent *objContent = objectContentList->at(i);
        dataCenter->id = objContent->obj->getVal().c_str();

        vector<DynamicProperty *> *propSet = objContent->propset;
        for (size_t j = 0; j < propSet->size(); j++) {

            if (propSet->at(j)->objType.compare("xsd:string") == 0) {
                string *str = (string *) propSet->at(j)->obj;
                dataCenter->name = str->c_str();
            }

        }
    }
    int DataCenterBuilder::getAsset(vector<Asset *> &assetVector) {
        return fetchFromVCenter<DataCenter>(assetVector);

    }
    int DataCenterBuilder::getAsset(string morVal, Asset **asset) {
        return fetchFromVCenter<DataCenter>(morVal, asset);
    }



    int DataCenterBuilder::fetchMors(ServiceInstance *si, vector<string> prop,
            ManagedObjectReference mor,
            vector<ManagedObjectReference> *morList) {
        //Get Property Collector
        vector<ObjectContent *> *objectContentList;
        PropertyCollector *propCollector = NULL;
        int code = si->getPropertyCollector(&propCollector);
        if (code != VIC_WSCLIENT_SUCCESS) {
            VIC_PLUG_LOG(VIC_TRACE_INFO,"%s",
                    "Unable to get Property Collector");
            return code;
        }
        auto_ptr<PropertyCollector> pautoPropertyCollector(propCollector);
        propCollector->retrieveProperties(&mor, prop, &objectContentList);

        for (size_t i = 0; i < objectContentList->size(); i++) {

            ObjectContent *objContent = objectContentList->at(i);

            vector<DynamicProperty *> *propSet = objContent->propset;
            for (size_t j = 0; j < propSet->size(); j++) {

                if (propSet->at(j)->objType.compare("Folder") == 0) {
                    ManagedObjectReference *foldermor =
                            (ManagedObjectReference *) propSet->at(j)->obj;

                    morList->push_back(*foldermor);

                }
                else if (propSet->at(j)->objType.compare(
                        "ArrayOfManagedObjectReference") == 0) {
                    vector<ManagedObjectReference*> *morlist = (vector<
                            ManagedObjectReference*> *) propSet->at(j)->obj;
                    for (size_t s = 0; s < morlist->size(); s++) {
                        ManagedObjectReference *objmor = morlist->at(s);
                        morList->push_back(*objmor);

                    }
                }

            }

        }
        //Delete the Pointer Vectors implicitly
        ListUtil<ObjectContent>::clearListofPointers(objectContentList);
        return VIC_WSCLIENT_SUCCESS;

    }


    int DataCenterBuilder::getDataCenterHostMap(
            vector<Asset *>&dataCenterVector,
            map<string, string> &hostDataCenterMap) {
        return getDataCenterChildren(dataCenterVector, "HostSystem",
                hostDataCenterMap);
    }

    int DataCenterBuilder::getDataCenterVMMap(vector<Asset *>&dataCenterVector,
            map<string, string> &vmDataCenterMap) {
        return getDataCenterChildren(dataCenterVector, "VirtualMachine",
                vmDataCenterMap);
    }

    int DataCenterBuilder::getDataCenterNetworkMap(
            vector<Asset *>&dataCenterVector,
            map<string, string> &networkDataCenterMap) {
        return getDataCenterChildren(dataCenterVector, "Network",
                networkDataCenterMap);

    }
    int DataCenterBuilder::getDataCenterDVPGMap(
            vector<Asset *>&dataCenterVector,
            map<string, string> &dvpgDataCenterMap) {
        return getDataCenterChildren(dataCenterVector,
                "DistributedVirtualPortgroup", dvpgDataCenterMap);

    }
    int DataCenterBuilder::getDataCenterDVSMap(vector<Asset *>&dataCenterVector,
            map<string, string> &dvsDataCenterMap) {
        return getDataCenterChildren(dataCenterVector,
                "VmwareDistributedVirtualSwitch", dvsDataCenterMap);

    }
    int DataCenterBuilder::fetchFromDC(ServiceInstance *si, string obj_type,
            ManagedObjectReference dc_mor, vector<string> &children) {

        vector<string> type;

        type.push_back(obj_type);

        //put the checks
        ViewManager *viewManager = NULL;
        si->getViewManager(&viewManager);
        auto_ptr<ViewManager> pautoViewManager(viewManager);

        ServiceContent *serviceContent = NULL;
        si->getServiceContent(&serviceContent);

        ManagedObjectReference *containerView = NULL;
        int code = viewManager->createContainerView(&dc_mor, &type, true, &containerView);
        if (code != VIC_WSCLIENT_SUCCESS) {
            VIC_PLUG_LOG(VIC_TRACE_INFO,"%s","Unable to create ContainerView");
            return code;
        }

        auto_ptr<ManagedObjectReference> pautoContainerView(containerView);
        ObjectSpec objspec;
        objspec.obj = *containerView;
        objspec.skip = true;
        TraversalSpec traversalspec;
        traversalspec.name = "traverseEntities";
        traversalspec.path = "view";
        traversalspec.skip = false;

        traversalspec.type = "ContainerView";
        objspec.traversalSet.push_back(traversalspec);
        PropertySpec propertyspec;
        propertyspec.type = obj_type;

        PropertyFilterSpec propertyFilterSpec;
        propertyFilterSpec.propSet.push_back(propertyspec);
        propertyFilterSpec.objectSet.push_back(objspec);
        vector<PropertyFilterSpec> specSet;
        specSet.push_back(propertyFilterSpec);

        //Get Property Collector
        PropertyCollector *propCollector = NULL;
        code = serviceInstance->getPropertyCollector(&propCollector);
        if (code != VIC_WSCLIENT_SUCCESS) {

            VIC_PLUG_LOG(VIC_TRACE_INFO,"%s",
                    "Unable to get Property Collector");
            return code;
        }
        auto_ptr<PropertyCollector> pautoPropertyCollector(propCollector);
        //Retrieve Properties
        vector<ObjectContent *> *objectContentList = NULL;
        code = propCollector->retrieveProperties(&specSet, &objectContentList);
        if (code != VIC_WSCLIENT_SUCCESS) {
            VIC_PLUG_LOG(VIC_TRACE_INFO,
                    "Unable to Fetch Details for %s", morType.c_str());
            return code;
        }
        for (size_t i = 0; i < objectContentList->size(); i++) {

            ObjectContent *objContent = objectContentList->at(i);
            string val = objContent->obj->val;
            children.push_back(val);

        }
        //Delete the Pointer Vectors implicitly
        ListUtil<ObjectContent>::clearListofPointers(objectContentList);
        return VIC_WSCLIENT_SUCCESS;
    }
    int DataCenterBuilder::getDataCenterChildren(
            vector<Asset *>&dataCenterVector, string childType,
            map<string, string> &childDataCenterMap) {

        for (size_t i = 0; i < dataCenterVector.size(); i++) {
            DataCenter *dataCenter = (DataCenter *) dataCenterVector.at(i);

            ManagedObjectReference data_mor;
            data_mor.type = "Datacenter";
            data_mor.val = dataCenter->id;
            vector<string> children;

            int code = fetchFromDC(serviceInstance, childType, data_mor,
                    children);
            if (code != VIC_WSCLIENT_SUCCESS) {
                VIC_PLUG_LOG(VIC_TRACE_INFO,
                        "Unable to get %s from DataCenter", childType.c_str());
                return code;
            }

            for (size_t j = 0; j < children.size(); j++) {
                string child = children.at(j);
                childDataCenterMap[child] = dataCenter->id;
            }

        }

        return VIC_WSCLIENT_SUCCESS;
    }

} /* namespace DcmNs */
