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
 * HostVirtualNicSpec.cpp
 *
 *  Created on: Mar 26, 2012
 *      Author: rmadapur
 */

#include "HostVirtualNicSpec.h"

namespace DcmNs {

    HostVirtualNicSpec::HostVirtualNicSpec():tsoEnabled(false) {


    }

    HostVirtualNicSpec::~HostVirtualNicSpec() {

    }
    void HostVirtualNicSpec::deSerialize(xmlNodePtr node){
        xmlNodePtr childNode =  node->children;
        while(childNode!=NULL){
            WSObject::extractObject(childNode, "ip", &this->ip);
            XMLUtil::extractString(childNode, "mac", this->mac);
            WSObject::extractObject(childNode, "distributedVirtualPort", &this->distributedVirtualPort);
            XMLUtil::extractString(childNode, "portgroup", this->portgroup);
            XMLUtil::extractString(childNode, "mtu", this->mtu);
            XMLUtil::extractBoolean(childNode, "tsoEnabled", this->tsoEnabled);
            childNode = childNode->next;
        }
    }

} /* namespace DcmNs */
