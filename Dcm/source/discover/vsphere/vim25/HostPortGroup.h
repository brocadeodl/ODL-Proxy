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
 * HostPortGroup.h
 *
 *  Created on: Apr 1, 2012
 *      Author: rmadapur
 */

#ifndef HOSTPORTGROUP_H_
#define HOSTPORTGROUP_H_
#include "discover/vsphere/vim25/WSObject.h"
#include <string>
using namespace std;
namespace DcmNs {




    class HostPortGroupSpec: public WSObject {
        public:

            string name;
            string vlanId;
            string vswitchName;
            HostPortGroupSpec();
            virtual ~HostPortGroupSpec();
            void deSerialize(xmlNodePtr node);
    };
    class HostPortGroup: public WSObject {
           public:
               string key;
               string vswitch;
               HostPortGroupSpec spec;
               HostPortGroup();
               virtual ~HostPortGroup();
               void deSerialize(xmlNodePtr node);
       };

} /* namespace DcmNs */
#endif /* HOSTPORTGROUP_H_ */
