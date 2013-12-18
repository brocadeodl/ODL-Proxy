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
 * PropertySpec.cpp
 *
 *  Created on: Mar 23, 2012
 *      Author: rmadapur
 */

#include "PropertySpec.h"

namespace DcmNs {

    PropertySpec::PropertySpec() :
            all(false) {
        // TODO Auto-generated constructor stub

    }

    PropertySpec::~PropertySpec() {
        // TODO Auto-generated destructor stub
    }
    string PropertySpec::serialize(string tagName, string _type) {
        string sb;
        sb.append(XMLUtil::beginTag(tagName, _type));
        {
            //type Field
            sb.append(XMLUtil::serializeString("type", type));

        }
        {
            //all Field
            sb.append(XMLUtil::serializeBoolean("all", all));
        }
        {
            //pathSetVector
            for (uint32_t i = 0; i < pathSet.size(); i++) {
                sb.append(XMLUtil::serializeString("pathSet", pathSet[i]));
            }
        }

        sb.append(XMLUtil::endTag(tagName));
        return sb;
    }

} /* namespace DcmNs */
