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
 * SpecialPgDomain.cpp
 *
 *  Created on: Sep 1, 2012
 *      Author: nfarhi
 */

#include "SpecialPgDomain.h"
#include "discover/events/vCenterEventsMessage.h"

using namespace std;

namespace DcmNs {

    SpecialPgDomain::SpecialPgDomain() {
    }

    SpecialPgDomain::SpecialPgDomain (string pg_nn_val, string dc_id_val,string host_id_val,
            string host_nn_val, string vs_val, string vlan_val) {
        pg_nn = pg_nn_val;
        dc_id = dc_id_val;
        host_id = host_id_val;
        vlan = vlan_val;
        host_nn = host_nn_val;
        vs = vs_val;
    }
    void SpecialPgDomain::isConfigUpdate(Domain* d, bool& added, bool& deleted, bool& updated) {
            if ((this->getVlan() != d->getVlan()) ||
                (this->getVs() != d->getVs())) { //vlan is updated
                updated = true;
            }
    }
    bool SpecialPgDomain::fncompid(Domain* rhs) {

        int ret = (this->getDcId()).compare(rhs->getDcId());
        if (ret < 0)
            return true;

        if (ret == 0) {
            ret = this->getHostId().compare((rhs->getHostId()));
            if (ret < 0)
                return true;
        }

        if (ret == 0) {
            ret = (this->getPgNN()).compare(rhs->getPgNN());
            if (ret < 0)
                return true;
        }
        return false;
    }

    SpecialPgDomain::~SpecialPgDomain() {
    }

    string SpecialPgDomain::getPgNN() {
        return pg_nn;
    }

    void SpecialPgDomain::setPgNN(string pg_nn_val) {
        pg_nn = pg_nn_val;
    }

    string SpecialPgDomain::getPortProfile() {
        return port_prof;
    }

    void SpecialPgDomain::setPortProfile(string port_profile_val) {
        port_prof = port_profile_val;
    }

    int SpecialPgDomain::getStatus() {
        return status;
    }

    void SpecialPgDomain::setStatus(int status_val) {
        status = status_val;
    }
    string SpecialPgDomain::getDcId() {
        return dc_id;
    }

    void SpecialPgDomain::setDcId(string dc_id_val) {
        dc_id = dc_id_val;
    }
    string SpecialPgDomain::getHostNN() {
        return host_nn;
    }

    void SpecialPgDomain::setHostNN(string host_nn_val) {
        host_nn = host_nn_val;
    }

    string SpecialPgDomain::getVlan() {
        return vlan;
    }

    void SpecialPgDomain::setVlan(string vlan_val) {
        vlan = vlan_val;
    }

    string SpecialPgDomain::getVs() {
        return vs;
    }

    void SpecialPgDomain::setVs(string vs_val) {
        vs = vs_val;
    }

    string SpecialPgDomain::getHostId() {
        return host_id;
    }

    void SpecialPgDomain::setHostId(string host_id_val) {
        host_id = host_id_val;
    }

    void SpecialPgDomain::printDomain() {
        VIC_PLUG_LOG(VIC_TRACE_DEBUG," %p, %s:%s:%s %s %s %s", this,pg_nn.c_str() , port_prof.c_str() , dc_id.c_str(),host_id.c_str(),host_nn.c_str(),vs.c_str() );
    }
    void SpecialPgDomain::convertToEventMessage(EventMessage** msg) {
            *msg = new SPGEventMessage();
            SPGEventMessage* m = static_cast<SPGEventMessage*>(*msg);
            m->m_domain = *this;
        }

    void SpecialPgDomain::setupAttributesForSerialization ()
    {
        Domain::setupAttributesForSerialization ();

        addSerializableAttribute (new AttributeString       (&pg_nn,    "pg_nn"));
        addSerializableAttribute (new AttributeString       (&port_prof,"port_prof"));
        addSerializableAttribute (new AttributeSI32         (&status,   "status"));
        addSerializableAttribute (new AttributeString       (&dc_id,    "dc_id"));
        addSerializableAttribute (new AttributeString       (&host_id,  "host_id"));
        addSerializableAttribute (new AttributeString       (&host_nn,  "host_nn"));
        addSerializableAttribute (new AttributeString       (&vs,       "vs"));
        addSerializableAttribute (new AttributeString       (&vlan,     "vlan"));
    }


} /* namespace DcmNs */
