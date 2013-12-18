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
 * VMwareDVSPortSetting.cpp
 *
 *  Created on: Apr 3, 2012
 *      Author: rmadapur
 */

#include "VMwareDVSPortSetting.h"

namespace DcmNs {

    VMwareDVSPortSetting::VMwareDVSPortSetting():vlan(NULL) {
        // TODO Auto-generated constructor stub

    }

    VMwareDVSPortSetting::~VMwareDVSPortSetting() {
        // TODO Auto-generated destructor stub
        if (this->vlan != NULL) {
            delete vlan;
        }
    }
    void VMwareDVSPortSetting::deSerialize(xmlNodePtr node) {
        xmlNodePtr childNode = node->children;
        while (childNode != NULL) {
            if (xmlStrEqual(childNode->name, BAD_CAST "vlan")) {
                xmlAttrPtr attribute = childNode->properties;
                XMLUtil::extractAttributeString(attribute, "type", type);

                if (type.compare("VmwareDistributedVirtualSwitchTrunkVlanSpec")
                        == 0) {
                    this->vlan =
                            new VmwareDistributedVirtualSwitchTrunkVlanSpec();
                    WSObject::extractObject(childNode, "vlan", this->vlan);
                }
                else if (type.compare(
                        "VmwareDistributedVirtualSwitchVlanIdSpec") == 0) {
                    this->vlan = new VmwareDistributedVirtualSwitchVlanIdSpec();
                    WSObject::extractObject(childNode, "vlan", this->vlan);
                }
            }


            WSObject::extractObject(childNode, "networkResourcePoolKey", &this->networkResourcePoolKey);

            childNode = childNode->next;
        }
    }

    VmwareDistributedVirtualSwitchTrunkVlanSpec::VmwareDistributedVirtualSwitchTrunkVlanSpec() {
        this->vlanId = new vector<NumericRange *>();

    }

    VmwareDistributedVirtualSwitchTrunkVlanSpec::~VmwareDistributedVirtualSwitchTrunkVlanSpec() {
        ListUtil<NumericRange>::clearListofPointers(this->vlanId);
    }
    void VmwareDistributedVirtualSwitchTrunkVlanSpec::deSerialize(
            xmlNodePtr node) {
        xmlNodePtr childNode = node->children;
        while (childNode != NULL) {
            ListUtil<NumericRange>::extractObjectIntoList(childNode, "vlanId",
                    this->vlanId);

            childNode = childNode->next;
        }
    }

    VmwareDistributedVirtualSwitchVlanIdSpec::VmwareDistributedVirtualSwitchVlanIdSpec() {

    }

    VmwareDistributedVirtualSwitchVlanIdSpec::~VmwareDistributedVirtualSwitchVlanIdSpec() {

    }
    void VmwareDistributedVirtualSwitchVlanIdSpec::deSerialize(
            xmlNodePtr node) {
        xmlNodePtr childNode = node->children;
        while (childNode != NULL) {

            XMLUtil::extractString(childNode, "vlanId", this->vlanId);
            childNode = childNode->next;
        }
    }

    VmwareDistributedVirtualSwitchVlanSpec::VmwareDistributedVirtualSwitchVlanSpec() {

    }

    VmwareDistributedVirtualSwitchVlanSpec::~VmwareDistributedVirtualSwitchVlanSpec() {

    }
    void VmwareDistributedVirtualSwitchVlanSpec::deSerialize(xmlNodePtr node) {

    }
    NumericRange::NumericRange() {
        // TODO Auto-generated constructor stub

    }

    NumericRange::~NumericRange() {
        // TODO Auto-generated destructor stub
    }
    void NumericRange::deSerialize(xmlNodePtr node) {
        xmlNodePtr childNode = node->children;
        while (childNode != NULL) {
            XMLUtil::extractString(childNode, "start", this->start);
            XMLUtil::extractString(childNode, "end", this->end);

            childNode = childNode->next;
        }
    }

    StringPolicy::StringPolicy() {

    }

    StringPolicy::~StringPolicy() {

    }
    void StringPolicy::deSerialize(
            xmlNodePtr node) {
        xmlNodePtr childNode = node->children;
        while (childNode != NULL) {

            XMLUtil::extractString(childNode, "value", this->value);
            XMLUtil::extractBoolean(childNode, "inherited", this->inherited);
            childNode = childNode->next;
        }
    }

} /* namespace DcmNs */
