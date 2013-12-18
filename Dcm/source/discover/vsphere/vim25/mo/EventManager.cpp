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
 * EventManager.cpp
 *
 *  Created on: Apr 13, 2012
 *      Author: rmadapur
 */

#include "EventManager.h"

namespace DcmNs {

    EventManager::EventManager(vimStub *_stub, ManagedObjectReference *_mor) :
            ManagedObject(_stub, _mor) {
        // TODO Auto-generated constructor stub

    }

    EventManager::~EventManager() {
        // TODO Auto-generated destructor stub
    }
    int EventManager::createCollectorForEvents(EventFilterSpec *filter,
            ManagedObjectReference **r_mor) {
        return stub->createCollectorForEvents(this->mor, filter, r_mor);

    }

    int EventManager::createCollectorForEvents(EventFilterSpec *filter,
            EventHistoryCollector **r_mor) {
        ManagedObjectReference *temp_mor;

        int code = stub->createCollectorForEvents(this->mor, filter, &temp_mor);
        if (code == VIC_WSCLIENT_SUCCESS) {
            *r_mor = new EventHistoryCollector(stub, temp_mor);
        }
        return code;
    }

} /* namespace DcmNs */
