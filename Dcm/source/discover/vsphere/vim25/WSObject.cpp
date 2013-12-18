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
 * WSObject.cpp
 *
 *  Created on: Mar 22, 2012
 *      Author: rmadapur
 */

#include "WSObject.h"

namespace DcmNs {

    WSObject::WSObject() {
        // TODO Auto-generated constructor stub

    }

    WSObject::~WSObject() {
        // TODO Auto-generated destructor stub
    }
    string WSObject::serialize(string tagName, string type) {
        string sb;
        return sb;
    }
    void WSObject::deSerialize(xmlNodePtr dyPropNode) {

    }

    bool WSObject::extractObject(xmlNodePtr & node, string key,
            WSObject *objectptr) {

        if (xmlStrEqual(node->name, BAD_CAST key.c_str())) {

            objectptr->deSerialize(node);
            return true;
        }
        return false;
    }



} /* namespace DcmNs */
