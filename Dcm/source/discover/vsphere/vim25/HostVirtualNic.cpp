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
 * HostVirtualNic.cpp
 *
 *  Created on: Mar 25, 2012
 *      Author: rmadapur
 */

#include "HostVirtualNic.h"

namespace DcmNs {

    HostVirtualNic::HostVirtualNic() {

    }

    HostVirtualNic::~HostVirtualNic() {

    }
    void HostVirtualNic::deSerialize(xmlNodePtr node) {
        xmlNodePtr childNode = node->children;
        while (childNode != NULL) {
            XMLUtil::extractString(childNode, "device", this->device);
            XMLUtil::extractString(childNode, "key", this->key);
            XMLUtil::extractString(childNode, "portgroup", this->portgroup);
            XMLUtil::extractString(childNode, "port", this->port);
            WSObject::extractObject(childNode, "spec", &this->spec);

            childNode = childNode->next;
        }
    }

} /* namespace DcmNs */
