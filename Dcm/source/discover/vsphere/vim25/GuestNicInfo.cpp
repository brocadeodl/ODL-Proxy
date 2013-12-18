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
 * GuestNicInfo.cpp
 *
 *  Created on: Jan 17, 2013
 *      Author: rmadapur
 */

#include "GuestNicInfo.h"
#include "discover/vsphere/vim25/ListUtil.h"

namespace DcmNs {
    GuestNicInfo::~GuestNicInfo() {

    }

    void GuestNicInfo::deSerialize(xmlNodePtr node) {
        xmlNodePtr childNode = node->children;
        while (childNode != NULL) {
            XMLUtil::extractString(childNode, "macAddress", this->macAddress);
            WSObject::extractObject(childNode, "ipConfig", &this->ipConfig);

            childNode = childNode->next;
        }
    }

    void NetIpConfigInfoIpAddress::deSerialize(xmlNodePtr node) {
        xmlNodePtr childNode = node->children;
        while (childNode != NULL) {
            XMLUtil::extractString(childNode, "ipAddress", this->ipAddress);

            childNode = childNode->next;
        }
    }
    NetIpConfigInfo::NetIpConfigInfo() {

        ipAddress = new vector<NetIpConfigInfoIpAddress *>();

    }

    NetIpConfigInfo::~NetIpConfigInfo() {
        ListUtil<NetIpConfigInfoIpAddress>::clearListofPointers(
                this->ipAddress);
    }

    void NetIpConfigInfo::deSerialize(xmlNodePtr node) {
        xmlNodePtr childNode = node->children;
        while (childNode != NULL) {
            ListUtil<NetIpConfigInfoIpAddress>::extractObjectIntoList(childNode,
                    "ipAddress", this->ipAddress);

            childNode = childNode->next;
        }
    }

} /* namespace DcmNs */
