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
 * Domain.cpp
 *
 *  Created on: May 8, 2012
 *      Author: nfarhi
 */


#include <string>
#include <vector>
#include "VirtualDb/Domain/Domain.h"
#include "discover/events/vCenterEventsMessage.h"
#include "discover/connector/Host.h"
#include "discover/vsphere/Debug.h"

using namespace WaveNs;
using namespace std;

namespace DcmNs {
    void   Domain::setupAttributesForSerialization() {
        SerializableObject::setupAttributesForSerialization ();
    }
    Domain::Domain() : SerializableObject () {

    }

    Domain::~Domain() {} 
    string Domain::getId() {return "";} 
    void Domain::setId(string s) {}
    string Domain::getName() {return "";}
    void Domain::setName(string s) {}
    string Domain::getVlan() {return "0";}
    void Domain::setVlan(string s) {}
    string Domain::getQos() {return "";}
    void Domain::setQos(string s) {} 
    string Domain::getHostId() {return "";}
    void Domain::setHostId(string s) {}
    string Domain::getDevice() {return "";}
    void Domain::setDevice(string s) {}
    string Domain::getVmId() {return "";}
    void Domain::setVmId(string s) {}
    string Domain::getDcId() {return "";}
    void Domain::setDcId(string s) {}
    string Domain::getMac() {return "";}
    void Domain::setMac(string s) {}
    string Domain::getPgNN() {return "";}
    void Domain::setPgNN(string s) {}
    string Domain::getDvpgNN() {return "";}
    void Domain::setDvpgNN(string s) {}
    string Domain::getDvpgId() {return "";}
    void Domain::setDvpgId(string s) {}
    string Domain::getPgId() { return "";}
    void Domain::setPgId(string s) {}
    string Domain::getDvs() { return "";}
    void Domain::setDvs(string s) {}

    std::string Domain::getDVPGType(){return "";}
    std::string Domain::getNetworkResourcePoolKey(){return "";}
    std::string Domain::getDvsId(){return "";}

    void Domain::setDVPGType(std::string s){}

    std::string Domain::getIface() {return "";}
    void Domain::setIface(std::string){}

    bool Domain::compare(Domain* lhs, Domain* rhs) {
        return lhs->fncompid(rhs);
    }
    bool Domain::fncompid(Domain* rhs) {
        int ret = (this->getId()).compare(rhs->getId());
        if (ret < 0)
            return true;
        return false;
    }
    void Domain::isConfigUpdate(Domain* d, bool& added, bool& deleted, bool& updated) {added = false; deleted = false; updated = false;}
    
    void Domain::printDomain() {}

    void Domain::printDomainVector(vector<Domain*> a) {
        vector<Domain*>::iterator it = a.begin();
        while(it != a.end()) {
            (*it)->printDomain();
            it++;
        }
    }
    void Domain::convertToEventMessage(EventMessage** msg) {}
    void Domain::findAssetDependentUpdates(Host* h, vector<Domain*>& a) {}
    void Domain::setDvsNN(string s) {}
    string Domain::getVs() {return "";}
    void Domain::setVs(string s) {}
    void Domain::setHost(string s) {}
    void Domain::setHostNN(string s) {}
}

