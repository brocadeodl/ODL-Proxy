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
 * NETWORK.cpp
 *
 *  Created on: Sep 13, 2012
 *      Author: rmadapur
 */

#include "Network.h"
#include "VirtualDb/Domain/PgHostVsDomain.h"
#include "discover/vsphere/Debug.h"


namespace DcmNs {

    Network::Network() {
        // TODO Auto-generated constructor stub

    }

    Network::~Network() {
        // TODO Auto-generated destructor stub
    }

    Domain* Network::convertToDomain(string host_mor, string host_name, string dc_mor) {
        Domain* phv = new PgHostVsDomain();
        phv->setPgId(id);
        phv->setHostId(host_mor);
        phv->setHostNN(host_name);
        phv->setDcId(dc_mor);
        phv->setVs(vs);
        phv->setVlan(vlan);
        phv->setPgNN(name);
        return phv;
    }

    void Network::printAsset() {
        VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s:%s:%s:%s:%s", id.c_str() , name.c_str() , vlan.c_str() , type.c_str() , vs.c_str() );
    }
 


} /* namespace DcmNs */
