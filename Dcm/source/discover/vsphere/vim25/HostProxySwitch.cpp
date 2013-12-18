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
 * HostProxySwitch.cpp
 *
 *  Created on: Apr 2, 2012
 *      Author: rmadapur
 */

#include "HostProxySwitch.h"

namespace DcmNs {

    HostProxySwitch::HostProxySwitch() {

    }

    HostProxySwitch::~HostProxySwitch() {

    }

    void HostProxySwitch::deSerialize(xmlNodePtr node) {
        xmlNodePtr childNode = node->children;
        while (childNode != NULL) {
            XMLUtil::extractString(childNode, "dvsUuid", this->dvsUuid);
            XMLUtil::extractString(childNode, "key", this->key);
            XMLUtil::extractString(childNode, "dvsName", this->dvsName);
            XMLUtil::extractString(childNode, "name", this->name);
            WSObject::extractObject(childNode, "spec", &this->spec);
            childNode = childNode->next;
        }
    }

    HostProxySwitchSpec::HostProxySwitchSpec() {

    }

    HostProxySwitchSpec::~HostProxySwitchSpec() {

    }
    void HostProxySwitchSpec::deSerialize(xmlNodePtr node) {
        xmlNodePtr childNode = node->children;
        while (childNode != NULL) {
            WSObject::extractObject(childNode, "backing", &this->backing);
            childNode = childNode->next;
        }
    }

    DistributedVirtualSwitchHostMemberPnicBacking::DistributedVirtualSwitchHostMemberPnicBacking() {

        pnicSpec = new vector<DistributedVirtualSwitchHostMemberPnicSpec *>();

    }

    DistributedVirtualSwitchHostMemberPnicBacking::~DistributedVirtualSwitchHostMemberPnicBacking() {
        ListUtil<DistributedVirtualSwitchHostMemberPnicSpec>::clearListofPointers(
                this->pnicSpec);
    }
    void DistributedVirtualSwitchHostMemberPnicBacking::deSerialize(
            xmlNodePtr node) {
        xmlNodePtr childNode = node->children;
        while (childNode != NULL) {

            ListUtil<DistributedVirtualSwitchHostMemberPnicSpec>::extractObjectIntoList(
                    childNode, "pnicSpec", this->pnicSpec);
            childNode = childNode->next;
        }
    }

    DistributedVirtualSwitchHostMemberPnicSpec::DistributedVirtualSwitchHostMemberPnicSpec() {

    }

    DistributedVirtualSwitchHostMemberPnicSpec::~DistributedVirtualSwitchHostMemberPnicSpec() {

    }
    void DistributedVirtualSwitchHostMemberPnicSpec::deSerialize(
            xmlNodePtr node) {
        xmlNodePtr childNode = node->children;
        while (childNode != NULL) {

            XMLUtil::extractString(childNode, "pnicDevice", this->pnicDevice);
            XMLUtil::extractString(childNode, "uplinkPortgroupKey",
                    this->uplinkPortgroupKey);

            childNode = childNode->next;
        }
    }

} /* namespace DcmNs */
