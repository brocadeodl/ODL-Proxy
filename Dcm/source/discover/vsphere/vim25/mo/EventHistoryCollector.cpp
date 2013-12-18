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
 * EventHistoryCollector.cpp
 *
 *  Created on: Jul 4, 2012
 *      Author: rmadapur
 */

#include "EventHistoryCollector.h"

namespace DcmNs {



    EventHistoryCollector::EventHistoryCollector(vimStub *_stub,
               ManagedObjectReference *_mor) :
               HistoryCollector(_stub, _mor) {


       }

    EventHistoryCollector::~EventHistoryCollector() {
        if(mor!=NULL){
            delete mor;
        }

    }

    int EventHistoryCollector::getFilter(EventFilterSpec *filter){
        //TODO
        //return  stub->destroyCollector(this->mor);
        return -1;
    }
    int EventHistoryCollector::getLatestPage(vector<Event *> ** evenList){
        //return  stub->destroyCollector(this->mor);
        return -1;
    }
    int EventHistoryCollector::readNextEvents(int maxCount,vector<Event *> **eventList){
        return  stub->readNextEvents(this->mor,maxCount,eventList);
    }
    int EventHistoryCollector::readPreviousEvents(int maxCount,vector<Event *> **eventList){
        return  stub->readPreviousEvents(this->mor,maxCount,eventList);
    }




} /* namespace DcmNs */
