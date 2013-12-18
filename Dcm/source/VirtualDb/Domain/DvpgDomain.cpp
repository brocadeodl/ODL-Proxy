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
 * DvpgDomain.cpp
 *
 *  Created on: May 8, 2012
 *      Author: nfarhi
 */

#include "DvpgDomain.h"
#include "discover/vsphere/Debug.h"
#include "discover/events/vCenterEventsMessage.h"
using namespace std;
using namespace WaveNs;

namespace DcmNs {

    DvpgDomain::DvpgDomain() {
    }

    DvpgDomain::DvpgDomain(string id_val, string name_val, string vlan_val,
            string qos_val, string dvs_id_val, string dvs_nn_val,
            string dc_id_val, string nrpk_val,string dvpgtype_val) {
        id = id_val;
        name = name_val;
        vlan = vlan_val;
        qos = qos_val;
        dvs_id = dvs_id_val;
        dvs_nn = dvs_nn_val;
        dc_id = dc_id_val;
        nrpk = nrpk_val;
        dvpgtype = dvpgtype_val;
    }

    DvpgDomain::~DvpgDomain() {
    }

    string DvpgDomain::getId() {
        return id;
    }

    void DvpgDomain::setId(string id_val) {
        id = id_val;
    }

    string DvpgDomain::getName() {
        return name;
    }

    void DvpgDomain::setName(string name_val) {
        name = name_val;
    }

    string DvpgDomain::getVlan() {
        return vlan;
    }

    void DvpgDomain::setVlan(string vlan_val) {
        vlan = vlan_val;
    }

    string DvpgDomain::getQos() {
        return qos;
    }

    void DvpgDomain::setQos(string qos_val) {
        qos = qos_val;
    }

    string DvpgDomain::getPortProfile() {
        return port_prof;
    }

    void DvpgDomain::setPortProfile(string port_profile_val) {
        port_prof = port_profile_val;
    }

    string DvpgDomain::getDvsId() {
        return dvs_id;
    }

    void DvpgDomain::setDvsId(string dvs_id_val) {
        dvs_id = dvs_id_val;
    }

    string DvpgDomain::getDvsNN() {
        return dvs_nn;
    }

    void DvpgDomain::setDvsNN(string dvs_nn_val) {
        dvs_nn = dvs_nn_val;
    }

    string DvpgDomain::getDcId() {
        return dc_id;
    }

    void DvpgDomain::setDcId(string dc_id_val) {
        dc_id = dc_id_val;
    }


    string DvpgDomain::getNetworkResourcePoolKey() {
         return nrpk;;
    }

    void DvpgDomain::setNetworkResourcePoolKey(string nrpk_val) {
        nrpk = nrpk_val;
    }

    int DvpgDomain::getStatus() {
        return status;
    }

    void DvpgDomain::setStatus(int status_val) {
        status = status_val;
    }

    void DvpgDomain::isConfigUpdate(Domain* d, bool& added, bool& deleted, bool& updated) {
        if (this->getName() != d->getName()) { //Domain is renamed
            // delete old Domain and create new Domain
            deleted = true; added = true;
        }
        if (this->getVlan() != d->getVlan() || this->getQos() != d->getQos()) { //vlan or qos is updated
            updated = true;
        }
    }

    string DvpgDomain::getPgNN() {
        return getName();
    }

    void DvpgDomain::printDomain() {
        VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s:%s:%s:%s:%s:%s", id.c_str() , name.c_str() , vlan.c_str() , qos.c_str() , port_prof.c_str() , dc_id.c_str() );
    }


    void DvpgDomain::setupAttributesForSerialization ()
    {
        Domain::setupAttributesForSerialization ();

        addSerializableAttribute (new AttributeString       (&id,       "id"));
        addSerializableAttribute (new AttributeString       (&name,     "name"));
        addSerializableAttribute (new AttributeString       (&vlan,     "vlan"));
        addSerializableAttribute (new AttributeString       (&qos,      "qos"));
        addSerializableAttribute (new AttributeString       (&port_prof,"port_prof"));
        addSerializableAttribute (new AttributeString       (&dvs_id,   "dvs_id"));
        addSerializableAttribute (new AttributeString       (&dvs_nn,   "dvs_nn"));
        addSerializableAttribute (new AttributeString       (&nrpk,     "nrpk"));
        addSerializableAttribute (new AttributeString       (&dvpgtype, "dvpgtype"));
    }
    string DvpgDomain::getDVPGType() {
        return dvpgtype;
    }

    void DvpgDomain::setDVPGType(string dvpgtype_val) {
        dvpgtype = dvpgtype_val;
    }

    void DvpgDomain::convertToEventMessage(EventMessage** msg) {
        *msg = new DVPGEventMessage();
        DVPGEventMessage* m = static_cast<DVPGEventMessage*>(*msg);
        m->m_domain = *this;
    }

} /* namespace DcmNs */
