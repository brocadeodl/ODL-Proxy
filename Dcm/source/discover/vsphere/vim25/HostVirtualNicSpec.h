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
 * HostVirtualNicSpec.h
 *
 *  Created on: Mar 26, 2012
 *      Author: rmadapur
 */

#ifndef HOSTVIRTUALNICSPEC_H_
#define HOSTVIRTUALNICSPEC_H_
#include <string>
#include "discover/vsphere/vim25/HostIpConfig.h"
#include "discover/vsphere/vim25/WSObject.h"
#include "discover/vsphere/vim25/DistributedVirtualSwitchPortConnection.h"
namespace DcmNs {

    class HostVirtualNicSpec: public WSObject {
        public:
            HostIpConfig ip;
            string mac;
            DistributedVirtualSwitchPortConnection distributedVirtualPort;
            string portgroup;
            string mtu;
            bool tsoEnabled;
            HostVirtualNicSpec();
            virtual ~HostVirtualNicSpec();
            void deSerialize(xmlNodePtr node);
    };

} /* namespace DcmNs */
#endif /* HOSTVIRTUALNICSPEC_H_ */
