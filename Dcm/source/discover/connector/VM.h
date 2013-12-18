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
 * VM.h
 *
 *  Created on: Apr 24, 2012
 *      Author: rmadapur
 */

#ifndef VM_H_
#define VM_H_
#include "discover/connector/Asset.h"
#include "discover/connector/Host.h"
namespace DcmNs {



    class VM :public Asset{
        public:
        string id;
        string name;
        string host_id;
        bool isTemplate;
        vector<VNics *> vnicsList;
        map<string,string> ipMacMap;
        VM();
        virtual ~VM();
        void printAsset();
        Domain* convertToDomain(std::string host_mor = "", std::string host_name = "", std::string dc_mor = "");

    };

} /* namespace DcmNs */
#endif /* VM_H_ */
