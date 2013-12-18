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
 * Correlator.h
 *
 * Created on: Sep 1, 2012
 *      Author: nfarhi
 */

#ifndef CORRELATOR_H_
#define CORRELATOR_H_

#include <iostream>
#include <string>
#include <string.h>
#include "correlator/SwitchAdapter.h"
#include <vector> 
#include <algorithm> 
#include <iterator>
#include "DcmCore/DcmToolKit.h"
#include "discover/utils/URLEncode.h"
#include "discover/vsphere/Debug.h"
#include "correlator/VcenterUtils.h"
#include "VirtualDb/Domain/Domain.h"
#include "VirtualDb/DAO/DAO.h"
#include "discover/connector/Asset.h"


using namespace std;

namespace DcmNs {

    class Correlator {
        public:
            Correlator() {}
            virtual ~Correlator() {}
            void releaseSource(vector<Domain*> v);
        protected:
            
            vector<Domain*> getSource(daoType_t, std::string, std::string mor = "");
            vector<Domain*> getSource(vector<Asset*>, std::string host_mor = "", std::string host_name = "", std::string dc_mor = "");
            vector<Domain*> diff(vector<Domain*> a, vector<Domain*> b);
            vector<Domain*> intersect(vector<Domain*> a, vector<Domain*> b);
            void findUpdatedConfig(vector<Domain*> a, vector<Domain*> b, vector<Domain*>& added, vector<Domain*>& deleted, vector<Domain*>& updated);
            void printVectorOfDomain(vector<Domain*> a);
            void printDomainChanges(vector<Domain*> added, vector<Domain*> deleted, vector<Domain*> updated);
            
    };

} /* namespace DcmNs */

#endif /* CORRELATOR_H_ */
                

        
