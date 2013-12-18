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
 * DiscoveryStateMachineCollection.h
 *
 *  Created on: May 25, 2012
 *      Author: rmadapur
 */

#ifndef DISCOVERYSTATEMACHINECOLLECTION_H_
#define DISCOVERYSTATEMACHINECOLLECTION_H_
#include <string>
#include "discover/StateMachine/DiscoveryStateMachine.h"
#include <discover/StateMachine/StateEnum.h>

#include <map>
#include <vector>
using namespace std;
namespace DcmNs {


   class DiscoveryStateMachine;

   class DiscoveryStateMachineCollection {
        map <string,vCenterState_t *> vcenterStatesMap;
        map <string,DiscoveryStateMachine *> vcenterStateMachineMap;
        public:
            DiscoveryStateMachineCollection();
            virtual ~DiscoveryStateMachineCollection();


            DiscoveryStateMachine * createStateMachine(StateActivateMessage * msg);

            DiscoveryStateMachine * findStateMachine(string _vcenterName);

            vector<DiscoveryStateMachine *>  getStateMachines();
            void deleteStateMachine(string _vcenterName);

    };

} /* namespace DcmNs */
#endif /* DISCOVERYSTATEMACHINECOLLECTION_H_ */
