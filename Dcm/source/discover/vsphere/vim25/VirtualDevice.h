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
 * VirtualDevice.h
 *
 *  Created on: Apr 4, 2012
 *      Author: rmadapur
 */

#ifndef VIRTUALDEVICE_H_
#define VIRTUALDEVICE_H_
#include "discover/vsphere/vim25/WSObject.h"
#include <string>
#include "discover/vsphere/vim25/VirtualEthernetCardNetworkBackingInfo.h"
using namespace std;
namespace DcmNs {

    class Description: public WSObject {
        public:
            string label;
            string summary;
            Description();
            virtual ~Description();
            void deSerialize(xmlNodePtr node);
    };
    class VirtualDevice: public WSObject {
        public:
            string key;
            string backingType;
            string macAddress;
            VirtualDeviceBackingInfo *backing;
            Description deviceInfo;
            VirtualDevice();
            virtual ~VirtualDevice();
            void deSerialize(xmlNodePtr node);
    };

} /* namespace DcmNs */
#endif /* VIRTUALDEVICE_H_ */
