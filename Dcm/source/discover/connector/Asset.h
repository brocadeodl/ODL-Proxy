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
 * Asset.h
 *
 *  Created on: Apr 22, 2012
 *      Author: rmadapur
 */

#ifndef ASSET_H_
#define ASSET_H_
#include <string>
#include <vector>
#include <map>
#include "discover/vsphere/vim25/ListUtil.h"
#include "VirtualDb/Domain/Domain.h"

using namespace std;
namespace DcmNs {

    class Asset {
        public:
            Asset();
            virtual ~Asset();
            virtual Domain* convertToDomain(std::string host_mor = "", std::string host_name = "", std::string dc_mor = "");
            virtual void printAsset();

            template<class T>
            static vector<Asset*> convertToVectorAsset(vector<T> v) {
                vector<Asset*> result;
                typename std::vector<T>::iterator it = v.begin();
                while(it != v.end()) {
                    result.push_back(*it);
                    it++;
                }
                return result;
            }

            static void printAssetVector(vector<Asset*> a);
    };

} /* namespace DCM */
#endif /* ASSET_H_ */
