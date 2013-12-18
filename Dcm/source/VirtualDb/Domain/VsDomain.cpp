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
 * VsDomain.cpp
 *
 *  Created on: May 8, 2012
 *      Author: nfarhi
 */

#include "VsDomain.h"
#include "discover/connector/Host.h"
#include "discover/events/vCenterEventsMessage.h"

using namespace std;

namespace DcmNs {

    VsDomain::VsDomain() {

    }

    VsDomain::VsDomain(string name_val, string host_id_val, string host_val,string dc_id_val) {
        name = name_val;
        host_id = host_id_val;
        host = host_val;
        dc_id = dc_id_val;
    }

    VsDomain::~VsDomain() {
    }

    string VsDomain::getName() {
        return name;
    }

    void VsDomain::setName(string name_val) {
        name = name_val;
    }

    string VsDomain::getHostId() {
        return host_id;
    }

    void VsDomain::setHostId(string host_id_val) {
        host_id = host_id_val;
    }

    string VsDomain::getHost() {
        return host;
    }

    void VsDomain::setHost(string host_val) {
        host = host_val;
    }
    string VsDomain::getDcId() {
        return dc_id;
    }

    void VsDomain::setDcId(string dc_id_val) {
        dc_id = dc_id_val;
    }

    void VsDomain::printDomain() {
        VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s:%s:%s:%s", name.c_str() , host_id.c_str() , host.c_str() , dc_id.c_str() );
    }

    void VsDomain::setupAttributesForSerialization ()
    {
        Domain::setupAttributesForSerialization ();

        addSerializableAttribute (new AttributeString       (&name,     "name"));
        addSerializableAttribute (new AttributeString       (&host_id,  "host_id"));
        addSerializableAttribute (new AttributeString       (&host,     "host"));
        addSerializableAttribute (new AttributeString       (&dc_id,    "dc_id"));
    }

    void VsDomain::convertToEventMessage(EventMessage** msg) {
        *msg = new VswitchEventMessage();
        VswitchEventMessage* m = static_cast<VswitchEventMessage*>(*msg);
        m->m_domain = *this;
    }

    void VsDomain::findAssetDependentUpdates(Host* host_, vector<Domain*>& vmnics) {
        vector<VMNics*>::iterator it = (host_->vmnicsList).begin();
        while (it != (host_->vmnicsList).end()) {
            (*it)->printAsset();
            if (name == (*it)->dvs_nn) {
                vmnics.push_back((*it)->convertToDomain(host_->id));
            }
            it++;
        }
    }



} /* namespace DcmNs */
