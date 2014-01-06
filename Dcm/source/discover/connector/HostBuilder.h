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
 * HostBuilder.h
 *
 *  Created on: Apr 23, 2012
 *      Author: rmadapur
 */

#ifndef HOSTBUILDER_H_
#define HOSTBUILDER_H_
#include "discover/connector/AssetBuilder.h"
#include "discover/connector/Host.h"
namespace DcmNs {

    class HostBuilder: public AssetBuilder {
        public:
            HostBuilder(ServiceInstance *_serviceInstance);
            HostBuilder(ServiceInstance *_serviceInstance, int level);
            virtual ~HostBuilder();
            int getAsset(vector<Asset *> &assetVector);
            int getAsset(string morVal, Asset **asset);
        protected:
            void extractData(vector<ObjectContent*> * objectContentList,
                                     size_t i, Asset * _host);
    };

} /* namespace DcmNs */
#endif /* HOSTBUILDER_H_ */