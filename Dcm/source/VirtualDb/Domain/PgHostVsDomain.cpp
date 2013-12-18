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
 * PgHostVsDomain.cpp
 *
 *  Created on: May 8, 2012
 *      Author: nfarhi
 */

#include "PgHostVsDomain.h"
#include "discover/events/vCenterEventsMessage.h"
#include "discover/vsphere/Debug.h"

using namespace std;

namespace DcmNs {

    PgHostVsDomain::PgHostVsDomain() {
    }

    PgHostVsDomain::PgHostVsDomain(string pg_id_val, string host_id_val,
            string host_nn_val, string vs_val, string vlan_val,
            string dc_id_val) {
        pg_id = pg_id_val;
        host_id = host_id_val;
        vlan = vlan_val;
        host_nn = host_nn_val;
        vs = vs_val;
        dc_id = dc_id_val;
    }

    PgHostVsDomain::~PgHostVsDomain() {
    }

    string PgHostVsDomain::getPgId() {
        return pg_id;
    }

    void PgHostVsDomain::setPgId(string pg_id_val) {
        pg_id = pg_id_val;
    }

    string PgHostVsDomain::getHostNN() {
        return host_nn;
    }

    void PgHostVsDomain::setHostNN(string host_nn_val) {
        host_nn = host_nn_val;
    }

    string PgHostVsDomain::getVlan() {
        return vlan;
    }

    void PgHostVsDomain::setVlan(string vlan_val) {
        vlan = vlan_val;
    }

    string PgHostVsDomain::getVs() {
        return vs;
    }

    void PgHostVsDomain::setVs(string vs_val) {
        vs = vs_val;
    }

    string PgHostVsDomain::getHostId() {
        return host_id;
    }

    void PgHostVsDomain::setHostId(string host_id_val) {
        host_id = host_id_val;
    }
    string PgHostVsDomain::getDcId() {
        return dc_id;
    }

    void PgHostVsDomain::setDcId(string dc_id_val) {
        dc_id = dc_id_val;
    }

    string PgHostVsDomain::getPgNN() {
        return pg_nn;
    }

    void PgHostVsDomain::setPgNN(string pg_nn_val) {
        pg_nn = pg_nn_val;
    }

    void PgHostVsDomain::isConfigUpdate(Domain* d, bool& added, bool& deleted, bool& updated) {
        if ((this->getVlan() != d->getVlan()) || 
            (this->getHostId() != d->getHostId())||
            (this->getVs() != d->getVs())) { //vlan is updated
                VIC_PLUG_LOG(VIC_TRACE_DEBUG,"vlan lhs %s rhs %s ",getVlan().c_str(),d->getVlan().c_str());
                VIC_PLUG_LOG(VIC_TRACE_DEBUG,"host_id lhs %s rhs %s ",getHostId().c_str(),d->getHostId().c_str());
                VIC_PLUG_LOG(VIC_TRACE_DEBUG,"vs lhs %s rhs %s ",getVs().c_str(),d->getVs().c_str());
                VIC_PLUG_LOG(VIC_TRACE_DEBUG,"pgid lhs %s rhs %s ",getPgId().c_str(),d->getPgId().c_str());
                VIC_PLUG_LOG(VIC_TRACE_DEBUG,"pgnn lhs %s rhs %s ",getPgNN().c_str(),d->getPgNN().c_str());

            updated = true;
        }
    }

    void PgHostVsDomain::setupAttributesForSerialization ()
    {
        Domain::setupAttributesForSerialization ();

        addSerializableAttribute (new AttributeString       (&pg_id,    "pg_id"));
        addSerializableAttribute (new AttributeString       (&host_id,  "host_id"));
        addSerializableAttribute (new AttributeString       (&host_nn,  "host_nn"));
        addSerializableAttribute (new AttributeString       (&vs,       "vs"));
        addSerializableAttribute (new AttributeString       (&vlan,     "vlan"));
        addSerializableAttribute (new AttributeString       (&dc_id,    "dc_id"));
        addSerializableAttribute (new AttributeString       (&pg_nn,    "pg_nn"));
    }

    void PgHostVsDomain::convertToEventMessage(EventMessage** msg) {
        *msg = new PgHostVsEventMessage();
        PgHostVsEventMessage* m = static_cast<PgHostVsEventMessage*>(*msg);
        m->m_domain = *this;
    }

    void PgHostVsDomain::printDomain() {
        VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s: %s : %s : %s : %s : %s : %s\n",
        		pg_nn.c_str(),
        		pg_id.c_str(),host_id.c_str(),
        		host_nn.c_str(),vs.c_str(),vlan.c_str(),dc_id.c_str());
    }



} /* namespace DcmNs */
