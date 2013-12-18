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
 * ManagedObjectReference.cpp
 *
 *  Created on: Mar 21, 2012
 *      Author: rmadapur
 */

#include "ManagedObjectReference.h"

namespace DcmNs {

    ManagedObjectReference::ManagedObjectReference() {
        // TODO Auto-generated constructor stub

    }
    ManagedObjectReference::ManagedObjectReference(
            const ManagedObjectReference &ref) {
        type = ref.type.c_str();
        val = ref.val.c_str();
    }
    void ManagedObjectReference::operator=(
            const ManagedObjectReference &ref) {
        type = ref.type.c_str();
        val = ref.val.c_str();
    }

    ManagedObjectReference::~ManagedObjectReference() {

    }

    string ManagedObjectReference::getType() const {
        return type;
    }

    string ManagedObjectReference::getVal() const {
        return val;
    }

    void ManagedObjectReference::setType(string _type) {
        this->type = _type;
    }

    void ManagedObjectReference::setVal(string _val) {
        this->val = _val;
    }

    void ManagedObjectReference::deSerialize(xmlNodePtr node) {
        //Get Val
        XMLUtil::extractString(node,val);
        XMLUtil::extractAttributeString(node->properties,"type",type);

        VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s  %s %s\n", (const char *)node->name, this->getType().c_str(),
                this->getVal().c_str());
    }

    string ManagedObjectReference::serialize(string tagName, string _type) {
        string sb;
        sb.append(
                "<" + tagName
                        + " xmlns=\"urn:vim25\"  xsi:type=\""+_type +"\" type=\""
                        + type + "\">");
        sb.append(val);

        sb.append(XMLUtil::endTag(tagName));
        return sb;
    }

} /* namespace DcmNs */
