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
 * SpecialNicDomain.cpp
 *
 *  Created on: May 8, 2012
 *      Author: nfarhi
 */

#include "SpecialNicDomain.h"
#include "discover/events/vCenterEventsMessage.h"

using namespace std;

namespace DcmNs {

    SpecialNicDomain::SpecialNicDomain() {
    }

    SpecialNicDomain::SpecialNicDomain (string device_val, string mac_val, string ip_val, string pg_nn_val, string host_id_val, string vs_val, string vnic_type_val,string dc_id_val, string dvpg_id_val) {
        device = device_val;
        mac = mac_val;
        ip = ip_val;
        pg_nn = pg_nn_val;
        host_id = host_id_val;
        vs = vs_val;
        vnic_type = vnic_type_val;
        dc_id = dc_id_val;
        dvpg_id = dvpg_id_val;
    }

    SpecialNicDomain::~SpecialNicDomain() {
    }

    string SpecialNicDomain::getDevice() {
        return device;
    }

    void SpecialNicDomain::setDevice(string device_val) {
        device = device_val;
    }

    string SpecialNicDomain::getDvpgId() {
        return dvpg_id;
    }

    void SpecialNicDomain::setDvpgId(string dvpg_id_val) {
        dvpg_id = dvpg_id_val;
    }

    string SpecialNicDomain::getMac() {
        return mac;
    }

    void SpecialNicDomain::setMac(string mac_val) {
        mac = mac_val;
    }

    string SpecialNicDomain::getIp() {
        return ip;
    }

    void SpecialNicDomain::setIp(string ip_val) {
        ip = ip_val;
    }

    string SpecialNicDomain::getPgNN() {
        return pg_nn;
    }

    void SpecialNicDomain::setPgNN(string pg_nn_val) {
        pg_nn = pg_nn_val;
    }

    string SpecialNicDomain::getPortProfile() {
        return port_prof;
    }

    void SpecialNicDomain::setPortProfile(string port_profile_val) {
        port_prof = port_profile_val;
    }

    string SpecialNicDomain::getHostId() {
        return host_id;
    }

    void SpecialNicDomain::setHostId(string host_id_val) {
        host_id = host_id_val;
    }

    string SpecialNicDomain::getVs() {
        return vs;
    }

    void SpecialNicDomain::setVs(string vs_val) {
        vs = vs_val;
    }

    string SpecialNicDomain::getVnicType() {
        return vnic_type;
    }

    void SpecialNicDomain::setVnicType(string vnic_type_val) {
        vnic_type = vnic_type_val;
    }

    int SpecialNicDomain::getStatus() {
        return status;
    }

    void SpecialNicDomain::setStatus(int status_val) {
        status = status_val;
    }
    string SpecialNicDomain::getDcId() {
        return dc_id;
    }

    void SpecialNicDomain::setDcId(string dc_id_val) {
        dc_id = dc_id_val;
    }

    void SpecialNicDomain::isConfigUpdate(Domain* d, bool& added, bool& deleted, bool& updated) {
        if (this->getMac() != d->getMac() ||
                this->getPgNN() != d->getPgNN()) {
            deleted = true; added = true;
        }
    }

    void SpecialNicDomain::setupAttributesForSerialization ()
    {
        Domain::setupAttributesForSerialization ();

        addSerializableAttribute (new AttributeString       (&device,   "device"));
        addSerializableAttribute (new AttributeString       (&mac,      "mac"));
        addSerializableAttribute (new AttributeString       (&ip,       "ip"));
        addSerializableAttribute (new AttributeString       (&pg_nn,    "pg_nn"));
        addSerializableAttribute (new AttributeString       (&port_prof,"port_prof"));
        addSerializableAttribute (new AttributeString       (&host_id,  "host_id"));
        addSerializableAttribute (new AttributeString       (&vs,       "vs"));
        addSerializableAttribute (new AttributeString       (&vnic_type,"vnic_type"));
        addSerializableAttribute (new AttributeSI32         (&status,   "status"));
        addSerializableAttribute (new AttributeString       (&dc_id,    "dc_id"));
        addSerializableAttribute (new AttributeString       (&dvpg_id,  "dvpg_id"));
    }

    void SpecialNicDomain::convertToEventMessage(EventMessage** msg) {
        VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s ", "Event Message");
        *msg = new SNICEventMessage();
        SNICEventMessage* m = static_cast<SNICEventMessage*>(*msg);
        m->m_domain = *this;
    }


    void SpecialNicDomain::printDomain() {
        VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s : %s : %s : %s : %s : %s\n",
        		device.c_str(),mac.c_str(),
        		pg_nn.c_str(),host_id.c_str(),dc_id.c_str(), dvpg_id.c_str());
    }

} /* namespace DcmNs */
