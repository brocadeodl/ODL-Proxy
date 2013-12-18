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
 * TraversalSpec.cpp
 *
 *  Created on: Mar 23, 2012
 *      Author: rmadapur
 */

#include "TraversalSpec.h"

namespace DcmNs {

    TraversalSpec::TraversalSpec() {
        // TODO Auto-generated constructor stub

    }

    TraversalSpec::~TraversalSpec() {
        // TODO Auto-generated destructor stub
    }
    string TraversalSpec::serialize(string tagName, string _type) {
        string sb;
        sb.append(XMLUtil::beginTag(tagName, _type));
        sb.append(XMLUtil::serializeString("name", name));
        sb.append(XMLUtil::serializeString("type", type));
        sb.append(XMLUtil::serializeString("path", path));
        sb.append(XMLUtil::serializeBoolean("skip", skip));

        for(uint32_t i= 0;i<selectSet.size();i++){
            sb.append(selectSet[i].serialize("selectSet","SelectionSpec"));
        }
        sb.append(XMLUtil::endTag(tagName));
        return sb;
    }

} /* namespace DcmNs */
