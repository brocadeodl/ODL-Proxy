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
 * VirtualDevice.cpp
 *
 *  Created on: Apr 4, 2012
 *      Author: rmadapur
 */

#include "VirtualDevice.h"

namespace DcmNs {

    VirtualDevice::VirtualDevice() :
            backing(NULL) {

    }

    VirtualDevice::~VirtualDevice() {

        if (backing) {

            delete backing;

        }
    }

    void VirtualDevice::deSerialize(xmlNodePtr node) {
        xmlNodePtr childNode = node->children;
        while (childNode != NULL) {
            XMLUtil::extractString(childNode, "key", this->key);
            XMLUtil::extractString(childNode, "macAddress", this->macAddress);
            WSObject::extractObject(childNode, "deviceInfo", &this->deviceInfo);
            if (xmlStrEqual(childNode->name, BAD_CAST "backing")) {
                XMLUtil::extractAttributeString(childNode->properties, "type",
                        this->backingType);


                if (backingType.compare("VirtualEthernetCardNetworkBackingInfo")
                        == 0) {
                    VirtualEthernetCardNetworkBackingInfo * _obj =
                            new VirtualEthernetCardNetworkBackingInfo();
                    _obj->deSerialize(childNode);
                    backing = _obj;

                }
                else if (backingType.compare(
                        "VirtualEthernetCardDistributedVirtualPortBackingInfo")
                        == 0) {
                    VirtualEthernetCardDistributedVirtualPortBackingInfo * _obj =
                            new VirtualEthernetCardDistributedVirtualPortBackingInfo();
                    _obj->deSerialize(childNode);
                    backing = _obj;

                }
            }
            childNode = childNode->next;
        }
    }
    Description::Description() {
        // TODO Auto-generated constructor stub

    }

    Description::~Description() {
        // TODO Auto-generated destructor stub
    }

    void Description::deSerialize(xmlNodePtr node) {
        xmlNodePtr childNode = node->children;
        while (childNode != NULL) {
            XMLUtil::extractString(childNode, "label", this->label);
            XMLUtil::extractString(childNode, "summary", this->summary);
            childNode = childNode->next;
        }
    }
} /* namespace DcmNs */
