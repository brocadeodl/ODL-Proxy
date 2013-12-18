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
 * VmnicDomain.cpp
 *
 *  Created on: May 8, 2012
 *      Author: nfarhi
 */

#include "VmnicDomain.h"
#include "discover/events/vCenterEventsMessage.h"

using namespace std;

namespace DcmNs {

    VmnicDomain::VmnicDomain() {
    }

    VmnicDomain::VmnicDomain(string name_val, string mac_val,
            string host_id_val, string dvs_val, string dvs_nn_val,
            string dc_id_val) {
        name = name_val;
        mac = mac_val;
        host_id = host_id_val;
        dvs = dvs_val;
        dvs_nn = dvs_nn_val;
        dc_id = dc_id_val;
    }

    VmnicDomain::~VmnicDomain() {
    }

    string VmnicDomain::getName() {
        return name;
    }

    void VmnicDomain::setName(string name_val) {
        name = name_val;
    }

    string VmnicDomain::getMac() {
        return mac;
    }

    void VmnicDomain::setMac(string mac_val) {
        mac = mac_val;
    }

    string VmnicDomain::getHostId() {
        return host_id;
    }

    void VmnicDomain::setHostId(string host_id_val) {
        host_id = host_id_val;
    }

    string VmnicDomain::getIface() {
        return iface;
    }

    void VmnicDomain::setIface(string iface_val) {
        iface = iface_val;
    }

    string VmnicDomain::getDvs() {
        return dvs;
    }

    void VmnicDomain::setDvs(string dvs_val) {
        dvs = dvs_val;
    }

    string VmnicDomain::getDvsNN() {
        return dvs_nn;
    }

    void VmnicDomain::setDvsNN(string dvs_nn_val) {
        dvs_nn = dvs_nn_val;
    }
    string VmnicDomain::getDcId() {
        return dc_id;
    }

    void VmnicDomain::setDcId(string dc_id_val) {
        dc_id = dc_id_val;
    }

    void VmnicDomain::setupAttributesForSerialization ()
    {
        Domain::setupAttributesForSerialization ();

        addSerializableAttribute (new AttributeString       (&name,     "name"));
        addSerializableAttribute (new AttributeString       (&mac,      "mac"));
        addSerializableAttribute (new AttributeString       (&host_id,  "host_id"));
        addSerializableAttribute (new AttributeString       (&iface,    "iface"));
        addSerializableAttribute (new AttributeString       (&dvs,      "dvs"));
        addSerializableAttribute (new AttributeString       (&dvs_nn,   "dvs_nn"));
        addSerializableAttribute (new AttributeString       (&dc_id,    "dc_id"));
    }
    void VmnicDomain::printDomain() {
        VIC_PLUG_LOG(VIC_TRACE_DEBUG,"printDomain %s %s %s %s %s\n",name.c_str(),mac.c_str(),
        		host_id.c_str(),iface.c_str(),dc_id.c_str());
    }

    void VmnicDomain::convertToEventMessage(EventMessage** msg) {
        *msg = new VmnicEventMessage();
        VmnicEventMessage* m = static_cast<VmnicEventMessage*>(*msg);
        m->m_domain = *this;
    }    


} /* namespace DcmNs */
