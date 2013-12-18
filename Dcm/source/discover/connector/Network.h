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
 * NETWORK.h
 *
 *  Created on: Sep 13, 2012
 *      Author: rmadapur
 */

#ifndef NETWORK_H_
#define NETWORK_H_
#include "discover/connector/Asset.h"
namespace DcmNs {

    class Network :public Asset{
           public:

           string id;
           string vlan;
           string name;
           string type;
           string vs;
           Network();
               virtual ~Network();
           Domain* convertToDomain(std::string host_mor = "", std::string host_name = "", std::string dc_mor = "");
           void printAsset(); 
       };

} /* namespace DcmNs */
#endif /* NETWORK_H_ */
