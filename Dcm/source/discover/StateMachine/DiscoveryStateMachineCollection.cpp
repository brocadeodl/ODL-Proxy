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
 * DiscoveryStateMachineCollection.cpp
 *
 *  Created on: May 25, 2012
 *      Author: rmadapur
 */

#include "discover/StateMachine/DiscoveryStateMachineCollection.h"

#include "discover/vsphere/Debug.h"
#include <algorithm>


namespace DcmNs {

    DiscoveryStateMachineCollection::DiscoveryStateMachineCollection() {


    }

    DiscoveryStateMachineCollection::~DiscoveryStateMachineCollection() {

    }

    vector<DiscoveryStateMachine *> DiscoveryStateMachineCollection:: getStateMachines(){
        vector<DiscoveryStateMachine *> vectorResult;
        map<string,DiscoveryStateMachine *>::iterator it;
        for ( it=vcenterStateMachineMap.begin() ; it != vcenterStateMachineMap.end(); it++ )
            vectorResult.push_back((*it).second);


        return vectorResult;
    }
    DiscoveryStateMachine * DiscoveryStateMachineCollection::findStateMachine(
                string _vcenterName) {
            VIC_PLUG_LOG(VIC_TRACE_DEBUG,"findStateMachine vCenter %s",_vcenterName.c_str());
        map<string, DiscoveryStateMachine *>::iterator it = vcenterStateMachineMap.find(
                      _vcenterName);
              if (it != vcenterStateMachineMap.end()) {
                  return (*it).second;
              }
              else {
                  return NULL;
              }

    }

    DiscoveryStateMachine * DiscoveryStateMachineCollection::createStateMachine(
            StateActivateMessage *msg) {
        string _vcenterName = msg->vcenter;
        map<string, DiscoveryStateMachine *>::iterator it = vcenterStateMachineMap.find(
                _vcenterName);
        if (it != vcenterStateMachineMap.end()) {
            return (*it).second;
        }
        else {
            VIC_PLUG_LOG(VIC_TRACE_INFO,"Creating New vCenter %s",_vcenterName.c_str());

            vCenterState_t *state = new vCenterState_t();
            state->vcenterName = _vcenterName;
            state->discState = DISC_IDLE;
            vcenterStatesMap.insert(pair<string, vCenterState_t *>(_vcenterName,state));
            DiscoveryStateMachine *sm = new DiscoveryStateMachine(state);
            sm->vCenterdata = msg;
            vcenterStateMachineMap.insert(pair<string, DiscoveryStateMachine *>(_vcenterName,sm));
            return sm;
        }
    }
    void   DiscoveryStateMachineCollection::deleteStateMachine(
                string _vcenterName) {
            map<string, DiscoveryStateMachine *>::iterator it = vcenterStateMachineMap.find(
                    _vcenterName);
            if (it != vcenterStateMachineMap.end()) {
                VIC_PLUG_LOG(VIC_TRACE_INFO,"Deleting  vCenter %s",_vcenterName.c_str());
                delete ((*it).second);
                vcenterStateMachineMap.erase(it);

            }

        }

} /* namespace DcmNs */
