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
 * DistributedVirtualSwitchHostMember.h
 *
 *  Created on: Apr 2, 2012
 *      Author: rmadapur
 */

#ifndef DISTRIBUTEDVIRTUALSWITCHHOSTMEMBER_H_
#define DISTRIBUTEDVIRTUALSWITCHHOSTMEMBER_H_
#include "discover/vsphere/vim25/WSObject.h"
#include "discover/vsphere/vim25/ManagedObjectReference.h"
#include "discover/vsphere/vim25/HostProxySwitch.h"
#include "discover/vsphere/vim25/ListUtil.h"
#include <string>
using namespace std;
namespace DcmNs {

    class DistributedVirtualSwitchHostMemberConfigInfo: public WSObject {
        public:
            ManagedObjectReference host;
            DistributedVirtualSwitchHostMemberPnicBacking backing;

            DistributedVirtualSwitchHostMemberConfigInfo();
            virtual ~DistributedVirtualSwitchHostMemberConfigInfo();
            void deSerialize(xmlNodePtr node);
    };

    class DistributedVirtualSwitchHostMember: public WSObject {
        public:
            DistributedVirtualSwitchHostMemberConfigInfo config;
            DistributedVirtualSwitchHostMember();
            virtual ~DistributedVirtualSwitchHostMember();
            void deSerialize(xmlNodePtr node);
    };

} /* namespace DcmNs */
#endif /* DISTRIBUTEDVIRTUALSWITCHHOSTMEMBER_H_ */
