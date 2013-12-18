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
 * VMwareDVSPortSetting.h
 *
 *  Created on: Apr 3, 2012
 *      Author: rmadapur
 */

#ifndef VMWAREDVSPORTSETTING_H_
#define VMWAREDVSPORTSETTING_H_
#include "discover/vsphere/vim25/WSObject.h"
#include <string>
#include <vector>
#include "discover/vsphere/vim25/ListUtil.h"
using namespace std;

namespace DcmNs {
    class NumericRange: public WSObject {
        public:
            string start;
            string end;
            NumericRange();
            virtual ~NumericRange();
            void deSerialize(xmlNodePtr node);
    };
    class VmwareDistributedVirtualSwitchVlanSpec: public WSObject {
        public:
            VmwareDistributedVirtualSwitchVlanSpec();
            virtual ~VmwareDistributedVirtualSwitchVlanSpec();
            void deSerialize(xmlNodePtr node);
    };
    class VmwareDistributedVirtualSwitchTrunkVlanSpec: public VmwareDistributedVirtualSwitchVlanSpec {
        public:

            vector<NumericRange *> *vlanId;

            VmwareDistributedVirtualSwitchTrunkVlanSpec();
            virtual ~VmwareDistributedVirtualSwitchTrunkVlanSpec();
            void deSerialize(xmlNodePtr node);
    };
    class VmwareDistributedVirtualSwitchVlanIdSpec: public VmwareDistributedVirtualSwitchVlanSpec {
        public:

            string vlanId;

            VmwareDistributedVirtualSwitchVlanIdSpec();
            virtual ~VmwareDistributedVirtualSwitchVlanIdSpec();
            void deSerialize(xmlNodePtr node);
    };

    class StringPolicy: public WSObject {
        public:
            bool inherited;
            string value;
            StringPolicy();
            virtual ~StringPolicy();
            void deSerialize(xmlNodePtr node);
    };

    class VMwareDVSPortSetting: public WSObject {
        public:
            string type;
            VmwareDistributedVirtualSwitchVlanSpec *vlan;
            StringPolicy networkResourcePoolKey;
            VMwareDVSPortSetting();
            virtual ~VMwareDVSPortSetting();
            void deSerialize(xmlNodePtr node);
    };

} /* namespace DcmNs */
#endif /* VMWAREDVSPORTSETTING_H_ */
