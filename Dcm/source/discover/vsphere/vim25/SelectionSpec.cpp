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
 * SelectionSpec.cpp
 *
 *  Created on: Mar 23, 2012
 *      Author: rmadapur
 */

#include "SelectionSpec.h"

namespace DcmNs {

    SelectionSpec::SelectionSpec() {
        // TODO Auto-generated constructor stub

    }

    SelectionSpec::~SelectionSpec() {
        // TODO Auto-generated destructor stub
    }
    string SelectionSpec::serialize(string tagName, string _type) {
                string sb;
                sb.append(XMLUtil::beginTag(tagName, _type));
                sb.append(XMLUtil::serializeString("name", name));

                sb.append(XMLUtil::endTag(tagName));
                return sb;
            }

} /* namespace DcmNs */
