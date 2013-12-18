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
 * Fault.h
 *
 *  Created on: Apr 10, 2012
 *      Author: rmadapur
 */

#ifndef FAULT_H_
#define FAULT_H_
#include "discover/vsphere/vim25/WSObject.h"
#include <string>
#include <vector>
#include "discover/vsphere/vim25/ManagedObjectReference.h"
using namespace std;
namespace DcmNs {

    class Fault: public WSObject {
        public:
            string faultcode;
            string faultstring;
            Fault();
            virtual ~Fault();
            void deSerialize(xmlNodePtr node);
    };

} /* namespace DcmNs */
#endif /* FAULT_H_ */
