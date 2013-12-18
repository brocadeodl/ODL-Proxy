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
 * DVSNetworkResourcePool.cpp
 *
 *  Created on: Jul 10, 2012
 *      Author: rmadapur
 */

#include "DVSNetworkResourcePool.h"

namespace DcmNs {

    DVSNetworkResourcePool::DVSNetworkResourcePool() {
        // TODO Auto-generated constructor stub

    }

    DVSNetworkResourcePool::~DVSNetworkResourcePool() {
        // TODO Auto-generated destructor stub
    }

    void DVSNetworkResourcePool::deSerialize(xmlNodePtr node) {
        xmlNodePtr childNode = node->children;
        while (childNode != NULL) {
            WSObject::extractObject(childNode, "allocationInfo",
                    &this->allocationInfo);
            XMLUtil::extractString(childNode, "key", this->key);
            childNode = childNode->next;
        }
    }

    DVSNetworkResourcePoolAllocationInfo::DVSNetworkResourcePoolAllocationInfo() {
            // TODO Auto-generated constructor stub

        }

    DVSNetworkResourcePoolAllocationInfo::~DVSNetworkResourcePoolAllocationInfo() {
            // TODO Auto-generated destructor stub
        }

        void DVSNetworkResourcePoolAllocationInfo::deSerialize(xmlNodePtr node) {
            xmlNodePtr childNode = node->children;
            while (childNode != NULL) {

                XMLUtil::extractString(childNode, "priorityTag", this->priorityTag);
                childNode = childNode->next;
            }
        }

} /* namespace DcmNs */
