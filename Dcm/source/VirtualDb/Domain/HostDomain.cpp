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

#include "HostDomain.h"
#include "discover/events/vCenterEventsMessage.h"

using namespace std;

namespace DcmNs {

    HostDomain::HostDomain() {

    }

    HostDomain::HostDomain(string id_val, string name_val, string dc_id_val) {
        id = id_val;
        name = name_val;
        dc_id = dc_id_val;
    }


    HostDomain::~HostDomain() {
    }

    string HostDomain::getId() {
        return id;
    }

    void HostDomain::setId(string id_val) {
        id = id_val;
    }

    string HostDomain::getName() {
        return name;
    }

    void HostDomain::setName(string name_val) {
        name = name_val;
    }

    string HostDomain::getDcId() {
        return dc_id;
    }

    void HostDomain::setDcId(string dc_id_val) {
        dc_id = dc_id_val;
    }

    void HostDomain::setupAttributesForSerialization ()
    {
    	Domain::setupAttributesForSerialization ();

        addSerializableAttribute (new AttributeString       (&id,   "id"));
        addSerializableAttribute (new AttributeString       (&name, "name"));
        addSerializableAttribute (new AttributeString       (&dc_id,"dc_id"));
    }
    void HostDomain::printDomain() {
        VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s %s %s ",id.c_str(),name.c_str(),dc_id.c_str());
    }

    void HostDomain::convertToEventMessage(EventMessage** msg) {
        *msg = new HostEventMessage();
        HostEventMessage* m = static_cast<HostEventMessage*>(*msg);
        m->m_domain = *this;
    }

} /* namespace DcmNs */
