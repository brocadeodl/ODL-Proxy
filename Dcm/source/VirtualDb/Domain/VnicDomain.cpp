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

#include "VnicDomain.h"
#include "discover/events/vCenterEventsMessage.h"
using namespace std;

namespace DcmNs {

    VnicDomain::VnicDomain() {
    }

    VnicDomain::VnicDomain(string name_val, string mac_val, string ip_val,
            string dvpg_id_val, string dvpg_nn_val, string vm_id_val,
            string dvs_val, string dc_id_val) {
        name = name_val;
        mac = mac_val;
        ip = ip_val;
        dvpg_id = dvpg_id_val;
        dvpg_nn = dvpg_nn_val;
        vm_id = vm_id_val;
        dvs = dvs_val;
        dc_id = dc_id_val;
    }

    VnicDomain::~VnicDomain() {
    }

    string VnicDomain::getName() {
        return name;
    }

    void VnicDomain::setName(string name_val) {
        name = name_val;
    }

    string VnicDomain::getMac() {
        return mac;
    }

    void VnicDomain::setMac(string mac_val) {
        mac = mac_val;
    }

    string VnicDomain::getIp() {
        return ip;
    }

    void VnicDomain::setIp(string ip_val) {
        ip = ip_val;
    }

    string VnicDomain::getDvpgId() {
        return dvpg_id;
    }

    void VnicDomain::setDvpgId(string dvpg_id_val) {
        dvpg_id = dvpg_id_val;
    }

    string VnicDomain::getDvpgNN() {
        return dvpg_nn;
    }

    void VnicDomain::setDvpgNN(string dvpg_nn_val) {
        dvpg_nn = dvpg_nn_val;
    }

    string VnicDomain::getPgNN() {
        return getDvpgNN();
    }

    void VnicDomain::setPgNN(string dvpg_nn_val) {
        setDvpgNN(dvpg_nn_val);
    }


    string VnicDomain::getPortProfile() {
        return port_prof;
    }

    void VnicDomain::setPortProfile(string port_profile_val) {
        port_prof = port_profile_val;
    }

    string VnicDomain::getVmId() {
        return vm_id;
    }

    void VnicDomain::setVmId(string vm_id_val) {
        vm_id = vm_id_val;
    }

    string VnicDomain::getDvs() {
        return dvs;
    }

    void VnicDomain::setDvs(string dvs_val) {
        dvs = dvs_val;
    }

    int VnicDomain::getStatus() {
        return status;
    }

    void VnicDomain::setStatus(int status_val) {
        status = status_val;
    }
    string VnicDomain::getDcId() {
        return dc_id;
    }

    void VnicDomain::setDcId(string dc_id_val) {
        dc_id = dc_id_val;
    }

    void VnicDomain::isConfigUpdate(Domain* d, bool& added, bool& deleted, bool& updated) {
        VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s","isConfigUpdate");    
        VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s","this ---");    
    	printDomain();
        VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s","...comparing....");    
//    	d->printDomain();
        VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s mac %s", this->getMac().c_str(), d->getMac().c_str());    
        VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s dvpg %s", this->getDvpgId().c_str(), d->getDvpgId().c_str());    

        if (this->getMac().compare(d->getMac()) == 0) {
        	// comparing same mac
	        if (this->getDvpgNN().compare(d->getDvpgNN()) == 0) {   	
				VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s","same VNIC ---");	            
	        }else {
	            deleted = true; added = true;
				VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s","diff VNIC ---");	            	        
	        }
        }
    	
    }

    void VnicDomain::printDomain() {
        VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s %s %s %s %s\n",name.c_str(),mac.c_str(),dvpg_id.c_str(),dvpg_nn.c_str(),dc_id.c_str());    
    }

    void VnicDomain::setupAttributesForSerialization ()
    {
        Domain::setupAttributesForSerialization ();

        addSerializableAttribute (new AttributeString       (&name,     "name"));
        addSerializableAttribute (new AttributeString       (&mac,      "mac"));
        addSerializableAttribute (new AttributeString       (&ip,       "ip"));
        addSerializableAttribute (new AttributeString       (&dvpg_id,  "dvpg_id"));
        addSerializableAttribute (new AttributeString       (&dvpg_nn,  "dvpg_nn"));
        addSerializableAttribute (new AttributeString       (&port_prof,"port_prof"));
        addSerializableAttribute (new AttributeString       (&vm_id,    "vm_id"));
        addSerializableAttribute (new AttributeString       (&dvs,      "dvs"));
        addSerializableAttribute (new AttributeString       (&dc_id,    "dc_id"));

    }


    void VnicDomain::convertToEventMessage(EventMessage** msg) {
        VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s ", "");
        *msg = new VNICEventMessage();
        VNICEventMessage* m = static_cast<VNICEventMessage*>(*msg);
        m->m_domain = *this;
    }


} /* namespace DcmNs */
