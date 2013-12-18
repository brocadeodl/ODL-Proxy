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
 * VMBuilder.h
 *
 *  Created on: Apr 24, 2012
 *      Author: rmadapur
 */

#ifndef VMBUILDER_H_
#define VMBUILDER_H_
#include "discover/connector/AssetBuilder.h"
#include "discover/connector/VM.h"
namespace DcmNs {

    class VMBuilder: public AssetBuilder {
        public:

            VMBuilder(ServiceInstance *_serviceInstance);
            virtual ~VMBuilder();
            int getAsset(vector<Asset *> &assetVector);
            int getAsset(string morVal, Asset **asset);
            int getAsset(vector<string> morValList, vector<Asset*> &assetVector);

        protected:
            void extractData(vector<ObjectContent*> * objectContentList,
                    size_t i, Asset * vm);
    };

} /* namespace DcmNs */
#endif /* VMBUILDER_H_ */
