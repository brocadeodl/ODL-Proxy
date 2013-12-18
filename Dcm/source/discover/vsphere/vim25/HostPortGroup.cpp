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
 * HostPortGroup.cpp
 *
 *  Created on: Apr 1, 2012
 *      Author: rmadapur
 */

#include "HostPortGroup.h"

namespace DcmNs {

    HostPortGroup::HostPortGroup() {

    }

    HostPortGroup::~HostPortGroup() {

    }
    void HostPortGroup::deSerialize(xmlNodePtr node) {
        xmlNodePtr childNode = node->children;
        while (childNode != NULL) {
            XMLUtil::extractString(childNode, "vswitch", this->vswitch);
            XMLUtil::extractString(childNode, "key", this->key);
            WSObject::extractObject(childNode, "spec", &this->spec);
            childNode = childNode->next;
        }
    }

    /********************* Definition of HostPortGroupSpec *****/

    HostPortGroupSpec::HostPortGroupSpec() {

    }

    HostPortGroupSpec::~HostPortGroupSpec() {

    }
    void HostPortGroupSpec::deSerialize(xmlNodePtr node) {
        xmlNodePtr childNode = node->children;
        while (childNode != NULL) {
            XMLUtil::extractString(childNode, "name", this->name);
            XMLUtil::extractString(childNode, "vlanId", this->vlanId);
            XMLUtil::extractString(childNode, "vswitchName", this->vswitchName);

            childNode = childNode->next;
        }
    }

} /* namespace DcmNs */
