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
 * DynamicProperty.h
 *
 *  Created on: Mar 25, 2012
 *      Author: rmadapur
 */

#ifndef DYNAMICPROPERTY_H_
#define DYNAMICPROPERTY_H_
#include "discover/vsphere/vim25/WSObject.h"
#include "discover/vsphere/vim25/ListUtil.h"
#include "discover/vsphere/vim25/ManagedObjectReference.h"
#include <string>
#include <vector>
#include "discover/vsphere/vim25/HostVirtualNic.h"
#include "discover/vsphere/vim25/GuestNicInfo.h"
#include "discover/vsphere/vim25/PhysicalNic.h"
#include "discover/vsphere/vim25/HostPortGroup.h"
#include "discover/vsphere/vim25/HostProxySwitch.h"
#include "discover/vsphere/vim25/HostVirtualSwitch.h"
#include "discover/vsphere/vim25/DistributedVirtualSwitchHostMember.h"
#include "discover/vsphere/vim25/DVSNetworkResourcePool.h"
#include "discover/vsphere/vim25/VMwareDVSPortSetting.h"
#include "discover/vsphere/vim25/VirtualMachinePowerState.h"
#include "discover/vsphere/vim25/VirtualDevice.h"
#include "discover/vsphere/vim25/Event.h"
#include "discover/vsphere/vim25/Tag.h"
using namespace std;
namespace DcmNs {

    /**
     * DataObject that represents a Name-Value pair
     */
    class DynamicProperty: public WSObject {

        public:
            /**
             * Path to the property
             */
            string name;
            /**
             * An internal representation of the Object present in obj
             */
            string objType;
            /**
             * Value of the property
             */
            void * obj;
            DynamicProperty();
            DynamicProperty(const DynamicProperty &ref);
            void operator=(const DynamicProperty &ref);
            virtual ~DynamicProperty();
            void deSerialize(xmlNodePtr node);

            };

        } /* namespace DcmNs */
#endif /* DYNAMICPROPERTY_H_ */
