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

#include "PgDomain.h"
#include "discover/vsphere/Debug.h"

using namespace std;

namespace DcmNs {

    PgDomain::PgDomain() {
    }

    PgDomain::PgDomain (string id_val, string name_val, string vlan_val, string dc_id_val) {
        id = id_val;
        name = name_val;
        vlan = vlan_val;
        dc_id = dc_id_val;
    }

    PgDomain::~PgDomain() {
    }
    
    string PgDomain::getId() {
        return id;
    }

    void PgDomain::setId(string id_val) {
        id = id_val;
    }

    string PgDomain::getName() {
        return name;
    }

    void PgDomain::setName(string name_val) {
        name = name_val;
    }

    string PgDomain::getVlan() {
        return vlan;
    }

    void PgDomain::setVlan(string vlan_val) {
        vlan = vlan_val;
    }

    string PgDomain::getPortProfile() {
        return port_prof;
    }

    void PgDomain::setPortProfile(string port_profile_val) {
        port_prof = port_profile_val;
    }

    string PgDomain::getDcId() {
        return dc_id;
    }

    void PgDomain::setDcId(string dc_id_val) {
        dc_id = dc_id_val;
    }

    int PgDomain::getStatus() {
        return status;
    }

    void PgDomain::setStatus(int status_val) {
        status = status_val;
    }


    void PgDomain::isConfigUpdate(Domain* d, bool& added, bool& deleted, bool& updated) {
        if (this->getName() != d->getName()) { //Domain is renamed
            // delete old Domain and create new Domain
            deleted = true; added = true;
        }
        if (this->getVlan() != d->getVlan()) { //vlan is updated
            updated = true;
        }
    }

    string PgDomain::getPgNN() {
        return getName();
    }

    void PgDomain::printDomain() {
        VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s:%s:%s:%s:%s", id.c_str() , name.c_str() , vlan.c_str() ,  port_prof.c_str() , dc_id.c_str() );
    }

} /* namespace DcmNs */
