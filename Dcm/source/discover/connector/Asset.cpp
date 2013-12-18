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
 * Asset.cpp
 *
 *  Created on: Apr 22, 2012
 *      Author: rmadapur
 */

#include "Asset.h"

using namespace std;

namespace DcmNs {

    Asset::Asset() {
        // TODO Auto-generated constructor stub

    }

    Asset::~Asset() {
        // TODO Auto-generated destructor stub
    }

    Domain* Asset::convertToDomain(string host_mor, string host_name, string dc_mor) {
        return NULL;
    }
    void Asset::printAsset() {}

    void Asset::printAssetVector(vector<Asset*> a) {
        vector<Asset*>::iterator it = a.begin();
        while(it != a.end()) {
            (*it)->printAsset();
            it++;
        }
    }

} /* namespace DCM */
