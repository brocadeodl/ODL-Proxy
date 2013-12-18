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
 * VM.cpp
 *
 *  Created on: Apr 24, 2012
 *      Author: rmadapur
 */

#include "VM.h"
#include "VirtualDb/Domain/VmDomain.h"
#include "discover/vsphere/Debug.h"

namespace DcmNs {

    VM::VM() {
        // TODO Auto-generated constructor stub
    }

    VM::~VM() {
        ListUtil<VNics>::clearPointersFromList(vnicsList);
    }


    void VM::printAsset() {
        VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s:%s", id.c_str() , name.c_str(),host_id.c_str()  );
    }

    Domain* VM::convertToDomain(string host_mor, string host_name, string dc_mor) {
        VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s", dc_mor.c_str()  );
        VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s", host_mor.c_str()  );
        VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s", host_name.c_str()  );
        Domain* dom = new VmDomain();
        dom->setId(id);
        dom->setName(name);
        dom->setHostId(host_id);
        dom->setHostNN(host_name);
        dom->setDcId(dc_mor);
        return dom;
    }


} /* namespace DcmNs */
