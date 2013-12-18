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
 * PropertySpec.h
 *
 *  Created on: Mar 23, 2012
 *      Author: rmadapur
 */

#ifndef PROPERTYSPEC_H_
#define PROPERTYSPEC_H_
#include "discover/vsphere/vim25/WSObject.h"
#include <vector>
#include <string>
using namespace std;
namespace DcmNs {

    class PropertySpec :public WSObject{
        public:
        string type;
        bool all;
        vector<string> pathSet;

            PropertySpec();

            virtual ~PropertySpec();
            string serialize(string tagName,string type);
    };

} /* namespace DcmNs */
#endif /* PROPERTYSPEC_H_ */
