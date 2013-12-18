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
 * HostVirtualNic.h
 *
 *  Created on: Mar 25, 2012
 *      Author: rmadapur
 */

#ifndef HOSTVIRTUALNIC_H_
#define HOSTVIRTUALNIC_H_
#include <string>
#include "discover/vsphere/vim25/WSObject.h"
#include "discover/vsphere/vim25/HostVirtualNicSpec.h"

using namespace std;
namespace DcmNs {

    class HostVirtualNic: public WSObject {
        public:
            string device;
            string key;
            string portgroup;
            HostVirtualNicSpec spec;
            string port;
            HostVirtualNic();
            virtual ~HostVirtualNic();
            void deSerialize(xmlNodePtr dyPropNode);
    };

} /* namespace DcmNs */
#endif /* HOSTVIRTUALNIC_H_ */
