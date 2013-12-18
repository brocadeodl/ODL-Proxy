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
 * ObjectSpec.cpp
 *
 *  Created on: Mar 23, 2012
 *      Author: rmadapur
 */

#include "ObjectSpec.h"

namespace DcmNs {

    ObjectSpec::ObjectSpec() {
        // TODO Auto-generated constructor stub

    }

    ObjectSpec::~ObjectSpec() {
        // TODO Auto-generated destructor stub
    }
    string ObjectSpec::serialize(string tagName, string _type) {
        string sb;
        sb.append(XMLUtil::beginTag(tagName, _type));
        sb.append(obj.serialize("obj", "ManagedObjectReference"));

        sb.append(XMLUtil::serializeBoolean("skip", skip));
        for (uint32_t i = 0; i < selectSet.size(); i++) {
            sb.append(selectSet[i].serialize("selectSet", "SelectionSpec"));
        }
        for (uint32_t i = 0; i < traversalSet.size(); i++) {
            sb.append(traversalSet[i].serialize("selectSet", "TraversalSpec"));
        }
        sb.append(XMLUtil::endTag(tagName));
        return sb;
    }

} /* namespace DcmNs */
