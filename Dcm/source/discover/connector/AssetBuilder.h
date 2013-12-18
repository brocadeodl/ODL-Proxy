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
 * AssetBuilder.h
 *
 *  Created on: Apr 22, 2012
 *      Author: rmadapur
 */

#ifndef ASSETBUILDER_H_
#define ASSETBUILDER_H_
#include "discover/connector/Asset.h"
#include "discover/vsphere/Debug.h"
#include "discover/vsphere/vim25/mo/ServiceInstance.h"
#include <vector>
#include <string>
#include <memory>
using namespace std;
namespace DcmNs {

    class AssetBuilder {

        public:
            vector<string> properties;
            string morType;
            AssetBuilder(ServiceInstance *_serviceInstance,string morType);
            ServiceInstance *serviceInstance;
            virtual ~AssetBuilder();

            virtual int getAsset(vector<Asset *> &assetVector);
            virtual int getAsset(string morVal, Asset **asset);

        protected:
            int createObjectSpec( ServiceInstance *serviceInstance, string obj_type, vector<string> &properties,vector<PropertyFilterSpec> &specSet);

            template<class T>
            int fetchFromVCenter(vector<Asset*> & assetVector) {
              vector<PropertyFilterSpec> specSet;
                int ret = createObjectSpec( serviceInstance, morType, properties, specSet);

                if (ret != VIC_WSCLIENT_SUCCESS) {
                    VIC_PLUG_LOG(VIC_TRACE_INFO,"%s","Unable to create ObjectSpec");
                    return ret;
                }
                //Get Property Collector
                PropertyCollector *propCollector = NULL;
                int code = serviceInstance->getPropertyCollector( &propCollector);
                if (code != VIC_WSCLIENT_SUCCESS) {
                    VIC_PLUG_LOG(VIC_TRACE_INFO,"%s","Unable to get Property Collector");
                    return code;
                }
                auto_ptr<PropertyCollector> pautoPropertyCollector(
                        propCollector);
                //Retrieve Properties
                vector<ObjectContent *> *objectContentList = NULL;
                code = propCollector->retrieveProperties(&specSet,
                        &objectContentList);
                if (code != VIC_WSCLIENT_SUCCESS) {
                    VIC_PLUG_LOG(VIC_TRACE_INFO,"%s","Unable to Fetch Details for %s", morType.c_str());
                    return code;
                }
                for (size_t i = 0; i < objectContentList->size(); i++) {
                    T *obj = new T();
                    extractData(objectContentList, i, obj);
                    assetVector.push_back(obj);

                }
                //Delete the Pointer Vectors implicitly
                ListUtil<ObjectContent>::clearListofPointers(objectContentList);
                return VIC_WSCLIENT_SUCCESS;
            }
            template<class T>
            int fetchFromVCenter( string morVal, Asset **asset) {
                ManagedObjectReference vmMor;
                vmMor.type = morType;
                vmMor.val = morVal;
                //Get Property Collector
                PropertyCollector *propCollector = NULL;
                int code = serviceInstance->getPropertyCollector(
                        &propCollector);
                if (code != VIC_WSCLIENT_SUCCESS) {
                    VIC_PLUG_LOG(VIC_TRACE_INFO,"%s","Unable to get Property Collector");
                    return code;
                }
                auto_ptr<PropertyCollector> pautoPropertyCollector(
                        propCollector);
                //Retrieve Properties
                vector<ObjectContent *> *objectContentList = NULL;
                code = propCollector->retrieveProperties(&vmMor, properties,
                        &objectContentList);
                if (code != VIC_WSCLIENT_SUCCESS) {
                    VIC_PLUG_LOG(VIC_TRACE_INFO,"%s","Unable to Fetch VirtualMachine Details\n");
                    return code;
                }

                if (objectContentList->size() == 1) {
                    T *vm = new T();
                    extractData(objectContentList, 0, vm);
                    *asset = vm;
                }
                //Delete the Pointer Vectors implicitly
                ListUtil<ObjectContent>::clearListofPointers(objectContentList);
                return VIC_WSCLIENT_SUCCESS;
            }

            template<class T>
            int fetchFromVCenter(vector<string> morList,
                    vector<Asset*> & assetVector) {

                vector<ManagedObjectReference> morObjList;
                for (size_t i = 0; i < morList.size(); i++) {

                    ManagedObjectReference vmMor;
                    vmMor.type = morType;
                    vmMor.val = morList.at(i);

                    morObjList.push_back(vmMor);
                }
                //Get Property Collector
                PropertyCollector *propCollector = NULL;
                int code = serviceInstance->getPropertyCollector(
                        &propCollector);
                if (code != VIC_WSCLIENT_SUCCESS) {
                    printf("Unable to get Property Collector");
                    return code;
                }
                auto_ptr<PropertyCollector> pautoPropertyCollector(
                        propCollector);
                //Retrieve Properties
                vector<ObjectContent *> *objectContentList = NULL;
                code = propCollector->retrieveProperties(morObjList, morType,
                        properties, &objectContentList);
                if (code != VIC_WSCLIENT_SUCCESS) {
                    printf("Unable to Fetch VirtualMachine Details\n");
                    return code;
                }

                for (size_t i = 0; i < objectContentList->size(); i++) {
                    T *obj = new T();
                    extractData(objectContentList, i, obj);
                    assetVector.push_back(obj);

                }
                //Delete the Pointer Vectors implicitly
                ListUtil<ObjectContent>::clearListofPointers(objectContentList);
                return VIC_WSCLIENT_SUCCESS;
            }

            virtual void extractData(vector<ObjectContent*> * objectContentList,
                    size_t i, Asset * vm);

    };

} /* namespace DcmNs */
#endif /* ASSETBUILDER_H_ */
