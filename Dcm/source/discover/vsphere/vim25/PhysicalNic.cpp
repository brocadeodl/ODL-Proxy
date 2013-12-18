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
 * PhysicalNic.cpp
 *
 *  Created on: Apr 1, 2012
 *      Author: rmadapur
 */

#include "PhysicalNic.h"

namespace DcmNs {

    PhysicalNic::PhysicalNic() {

    }

    PhysicalNic::~PhysicalNic() {

    }
    void PhysicalNic::deSerialize(xmlNodePtr node) {
        xmlNodePtr childNode = node->children;
        while (childNode != NULL) {
            XMLUtil::extractString(childNode, "device", this->device);
            XMLUtil::extractString(childNode, "key", this->key);
            XMLUtil::extractString(childNode, "mac", this->mac);

            childNode = childNode->next;
        }
    }

} /* namespace DcmNs */
