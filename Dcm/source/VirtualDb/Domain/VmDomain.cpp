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
 * DataCenter.cpp
 *
 *  Created on: May 8, 2012
 *      Author: nfarhi
 */

#include "VmDomain.h"
#include "discover/events/vCenterEventsMessage.h"

using namespace std;

namespace DcmNs {

    VmDomain::VmDomain() {
    }

    VmDomain::VmDomain(string id_val, string name_val, string host_id_val, string host_nn_val, string dc_id_val) {
        id = id_val;
        name = name_val;
        host_id = host_id_val;
        host_nn = host_nn_val;
        dc_id = dc_id_val;
    }

    VmDomain::~VmDomain() {
    }

    string VmDomain::getId() {
        return id;
    }

    void VmDomain::setId(string id_val) {
        id = id_val;
    }

    string VmDomain::getName() {
        return name;
    }

    void VmDomain::setName(string name_val) {
        name = name_val;
    }

    string VmDomain::getHostId() {
        return host_id;
    }

    void VmDomain::setHostId(string host_id_val) {
        host_id = host_id_val;
    }

    string VmDomain::getHostNN() {
        return host_nn;
    }

    void VmDomain::setHostNN(string host_nn_val) {
        host_nn = host_nn_val;
    }

    string VmDomain::getDcId() {
        return dc_id;
    }

    void VmDomain::setDcId(string dc_id_val) {
        dc_id = dc_id_val;
    }

    void VmDomain::setupAttributesForSerialization ()
    {
    	Domain::setupAttributesForSerialization ();

        addSerializableAttribute (new AttributeString       (&id,       "id"));
        addSerializableAttribute (new AttributeString       (&name,     "name"));
        addSerializableAttribute (new AttributeString       (&host_id,  "host_id"));
        addSerializableAttribute (new AttributeString       (&host_nn,  "host_nn"));
        addSerializableAttribute (new AttributeString       (&dc_id,    "dc_id"));

    }
    void VmDomain::printDomain() {
        VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s %s %s %s %s\n",id.c_str(),name.c_str(),host_id.c_str(),host_nn.c_str(),dc_id.c_str());
    }

    void VmDomain::convertToEventMessage(EventMessage** msg) {
        *msg = new VMEventMessage();
        VMEventMessage* m = static_cast<VMEventMessage*>(*msg);
        m->m_domain = *this;
    }

    void VmDomain::isConfigUpdate(Domain* d, bool& added, bool& deleted, bool& updated) {
        if (this->getHostId() != d->getHostId() ||
        this->getName() != d->getName()) { //Host is moved vm is renamed
            updated = true;
        }
    }

} /* namespace DcmNs */
