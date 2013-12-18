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
 * VirtualEthernetCardNetworkBackingInfo.h
 *
 *  Created on: Apr 5, 2012
 *      Author: rmadapur
 */

#ifndef VIRTUALETHERNETCARDNETWORKBACKINGINFO_H_
#define VIRTUALETHERNETCARDNETWORKBACKINGINFO_H_
#include "discover/vsphere/vim25/WSObject.h"
#include "discover/vsphere/vim25/DistributedVirtualSwitchPortConnection.h"
#include "discover/vsphere/vim25/ManagedObjectReference.h"
#include <string>
using namespace std;
namespace DcmNs {

    class VirtualDeviceBackingInfo: public WSObject {
        public:

            VirtualDeviceBackingInfo();
            virtual ~VirtualDeviceBackingInfo();

    };
    class VirtualDeviceDeviceBackingInfo: public VirtualDeviceBackingInfo {
        public:
            string deviceName;
            VirtualDeviceDeviceBackingInfo();
            virtual ~VirtualDeviceDeviceBackingInfo();
            virtual void deSerialize(xmlNodePtr node);
    };
    class VirtualEthernetCardNetworkBackingInfo: public VirtualDeviceDeviceBackingInfo {
        public:
            ManagedObjectReference network;
            VirtualEthernetCardNetworkBackingInfo();
            virtual ~VirtualEthernetCardNetworkBackingInfo();
            virtual void deSerialize(xmlNodePtr node);

    };



    class VirtualEthernetCardDistributedVirtualPortBackingInfo: public VirtualDeviceBackingInfo {
        public:
            DistributedVirtualSwitchPortConnection port;
            VirtualEthernetCardDistributedVirtualPortBackingInfo();
            virtual ~VirtualEthernetCardDistributedVirtualPortBackingInfo();
            virtual void deSerialize(xmlNodePtr node);

    };

} /* namespace DcmNs */
#endif /* VIRTUALETHERNETCARDNETWORKBACKINGINFO_H_ */
