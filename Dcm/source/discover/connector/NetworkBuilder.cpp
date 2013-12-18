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

#include "NetworkBuilder.h"

namespace DcmNs {

    NetworkBuilder::NetworkBuilder(ServiceInstance *_serviceInstance) :
            AssetBuilder(_serviceInstance, "Network") {
        properties.push_back("name");


    }

    NetworkBuilder::~NetworkBuilder() {

    }

    void NetworkBuilder::extractData(vector<ObjectContent*> * objectContentList,
            size_t i, Asset * _network) {
        Network *network = (Network *) _network;
        ObjectContent *objContent = objectContentList->at(i);
        network->id = objContent->obj->getVal();
        network->type = objContent->obj->type;

        vector<DynamicProperty*> *propSet = objContent->propset;
        for (size_t j = 0; j < propSet->size(); j++) {
            if (propSet->at(j)->objType.compare("xsd:string") == 0) {
                string *str = (string*) (propSet->at(j)->obj);
                network->name = str->c_str();

            }

        }

    }

    int NetworkBuilder::getAsset(string morVal, Asset **asset) {
        return fetchFromVCenter<Network>(morVal, asset);
    }

    int NetworkBuilder::getAsset(vector<Asset*> & assetVector) {
        return fetchFromVCenter<Network>(assetVector);
    }

} /* namespace DcmNs */
