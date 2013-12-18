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
 * EventFilterSpec.cpp
 *
 *  Created on: Apr 9, 2012
 *      Author: rmadapur
 */

#include "EventFilterSpec.h"

namespace DcmNs {

    EventFilterSpec::EventFilterSpec() {
        // TODO Auto-generated constructor stub

    }

    EventFilterSpec::~EventFilterSpec() {
        // TODO Auto-generated destructor stub
    }
    string EventFilterSpec::serialize(string tagName, string _type) {
        string sb;
        sb.append(XMLUtil::beginTag(tagName, _type));
        sb.append(time.serialize("time","EventFilterSpecByTime"));
        for (uint i = 0; i < type.size(); i++) {
            sb.append(XMLUtil::serializeString("type", type[i]));

        }
        sb.append(XMLUtil::endTag(tagName));
        return sb;

    }

    EventFilterSpecByTime::EventFilterSpecByTime() {
        // TODO Auto-generated constructor stub

    }

    EventFilterSpecByTime::~EventFilterSpecByTime() {
        // TODO Auto-generated destructor stub
    }
    string EventFilterSpecByTime::serialize(string tagName, string _type) {
        string sb;
        if(beginTime.empty()&&endTime.empty()){
            return sb;
        }
        sb.append(XMLUtil::beginTag(tagName, _type));
            sb.append(XMLUtil::serializeType("beginTime", beginTime,"dateTime"));
            sb.append(XMLUtil::serializeType("endTime", endTime,"dateTime"));
        sb.append(XMLUtil::endTag(tagName));
        return sb;

    }

} /* namespace DcmNs */
