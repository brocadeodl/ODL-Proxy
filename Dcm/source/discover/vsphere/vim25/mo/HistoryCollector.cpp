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
 * HistoryCollector.cpp
 *
 *  Created on: Jul 4, 2012
 *      Author: rmadapur
 */

#include "discover/vsphere/vim25/mo/HistoryCollector.h"

namespace DcmNs {

    HistoryCollector::HistoryCollector(vimStub *_stub,
            ManagedObjectReference *_mor) :
            ManagedObject(_stub, _mor) {


    }

    HistoryCollector::~HistoryCollector() {

    }

    int HistoryCollector::destroyCollector() {
       return  stub->destroyCollector(this->mor);
    }

    int HistoryCollector::resetCollector() {
        return  stub->resetCollector(this->mor);

    }

    int HistoryCollector::rewindCollector() {
        return  stub->rewindCollector(this->mor);

    }

    int HistoryCollector::setCollectorPageSize(int maxCount) {
        return  stub->setCollectorPageSize(this->mor,maxCount);

    }

} /* namespace DcmNs */
