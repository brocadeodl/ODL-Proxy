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
 * HostProxySwitch.h
 *
 *  Created on: Apr 2, 2012
 *      Author: rmadapur
 */

#ifndef HOSTPROXYSWITCH_H_
#define HOSTPROXYSWITCH_H_
#include "discover/vsphere/vim25/WSObject.h"
#include "discover/vsphere/vim25/ListUtil.h"
#include <string>
using namespace std;
namespace DcmNs {

    class DistributedVirtualSwitchHostMemberPnicSpec: public WSObject {
        public:
            string pnicDevice;

            string uplinkPortgroupKey;

            DistributedVirtualSwitchHostMemberPnicSpec();
            virtual ~DistributedVirtualSwitchHostMemberPnicSpec();
            void deSerialize(xmlNodePtr node);
    };
    class DistributedVirtualSwitchHostMemberPnicBacking: public WSObject {
        public:
            vector<DistributedVirtualSwitchHostMemberPnicSpec *> *pnicSpec;

            DistributedVirtualSwitchHostMemberPnicBacking();
            virtual ~DistributedVirtualSwitchHostMemberPnicBacking();
            void deSerialize(xmlNodePtr node);
    };
    class HostProxySwitchSpec: public WSObject {
        public:
            DistributedVirtualSwitchHostMemberPnicBacking backing;
            HostProxySwitchSpec();
            virtual ~HostProxySwitchSpec();
            void deSerialize(xmlNodePtr node);
    };
    class HostProxySwitch: public WSObject {
        public:
            string dvsUuid;
            string dvsName;
            string key;
            string name;
            HostProxySwitchSpec spec;
            HostProxySwitch();
            virtual ~HostProxySwitch();
            void deSerialize(xmlNodePtr node);
    };

} /* namespace DcmNs */
#endif /* HOSTPROXYSWITCH_H_ */
