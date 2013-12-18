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
 * AssetBuilder.cpp
 *
 *  Created on: Apr 22, 2012
 *      Author: rmadapur
 */

#include "AssetBuilder.h"

namespace DcmNs {

    AssetBuilder::AssetBuilder(ServiceInstance *_serviceInstance,string _morType):morType(_morType),serviceInstance(_serviceInstance) {
        // TODO Auto-generated constructor stub

    }

    AssetBuilder::~AssetBuilder() {
        // TODO Auto-generated destructor stub
    }

     void AssetBuilder::extractData(vector<ObjectContent*> * objectContentList,
                       size_t i, Asset * vm){

    }


    int AssetBuilder:: createObjectSpec(ServiceInstance *si, string obj_type, vector<string> &props, vector<PropertyFilterSpec> &specSet) {
        vector<string> type;

        type.push_back(obj_type);

        //put the checks
        ViewManager *viewManager = NULL;
        si->getViewManager(&viewManager);
        auto_ptr<ViewManager> pautoViewManager(viewManager);

        ServiceContent *serviceContent = NULL;
        si->getServiceContent(&serviceContent);

        ManagedObjectReference *containerView = NULL;
        int code = viewManager->createContainerView(&serviceContent->rootFolder, &type, true, &containerView);
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

        for (uint i = 0; i < props.size(); i++) {
            propertyspec.pathSet.push_back(props.at(i));
        }

        PropertyFilterSpec propertyFilterSpec;
        propertyFilterSpec.propSet.push_back(propertyspec);
        propertyFilterSpec.objectSet.push_back(objspec);
        //vector<PropertyFilterSpec> specSet;
        specSet.push_back(propertyFilterSpec);

        //return specSet;
        return VIC_WSCLIENT_SUCCESS;
    }
    int AssetBuilder::getAsset(vector<Asset *> &assetVector){
        return VIC_WSCLIENT_SUCCESS;
    }

    int AssetBuilder::getAsset(string morVal, Asset **asset){
        return VIC_WSCLIENT_SUCCESS;
    }

} /* namespace DcmNs */
