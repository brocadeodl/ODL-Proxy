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
 * DataCenterBuilder.h
 *
 *  Created on: Apr 22, 2012
 *      Author: rmadapur
 */

#ifndef DATACENTERBUILDER_H_
#define DATACENTERBUILDER_H_
#include "discover/connector/AssetBuilder.h"
#include "discover/connector/DataCenter.h"
namespace DcmNs {

    class DataCenterBuilder :public AssetBuilder{
        public:
            DataCenterBuilder(ServiceInstance *_serviceInstance);
            virtual ~DataCenterBuilder();
            int getAsset(vector<Asset *> &assetVector);
            int getAsset(string morVal, Asset **asset);

            int getDataCenterHostMap(vector<Asset *>&dataCenterVector,map<string,string> &hostDataCenterMap);
            int getDataCenterVMMap(vector<Asset *>&dataCenterVector,map<string,string> &vmDataCenterMap);
            int getDataCenterNetworkMap(vector<Asset *>&dataCenterVector,map<string,string> &networkDataCenterMap);
            int getDataCenterDVPGMap(vector<Asset *>&dataCenterVector,map<string,string> &dvpgDataCenterMap);
            int getDataCenterDVSMap(vector<Asset *>&dataCenterVector,map<string,string> &dvsDataCenterMap);



        protected:
            void extractData(vector<ObjectContent*> * objectContentList,
                                size_t i, Asset * vm);
            int getDataCenterChildren(vector<Asset *>&dataCenterVector, string childType,
                                        map<string, string> &vmDataCenterMap);
            int fetchFromDC(ServiceInstance *si,
                                                               string obj_type,ManagedObjectReference dc_mor, vector<string> &children);
        private:

            int fetchMors(ServiceInstance *serviceInstance,
                            vector<string> prop, ManagedObjectReference mor,
                            vector<ManagedObjectReference > *morList);


    };

} /* namespace DcmNs */
#endif /* DATACENTERBUILDER_H_ */
