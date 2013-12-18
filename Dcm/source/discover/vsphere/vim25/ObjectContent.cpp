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
 * ObjectContent.cpp
 *
 *  Created on: Mar 25, 2012
 *      Author: rmadapur
 */

#include "ObjectContent.h"

namespace DcmNs {

    ObjectContent::ObjectContent() {
        obj = new ManagedObjectReference();
        propset = new vector<DynamicProperty *>();
    }

    ObjectContent::~ObjectContent() {
        VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s","Object Content Destructor \n");
        delete (obj);
        ListUtil<DynamicProperty>::clearListofPointers(propset);
        //delete (propset);
    }
    ObjectContent::ObjectContent(const ObjectContent &ref) {
        obj = ref.obj;
        propset = ref.propset;

    }
    void  ObjectContent::operator=(const ObjectContent &ref) {
        obj = ref.obj;
        propset = ref.propset;
    }
    void ObjectContent::deSerialize(xmlNodePtr node) {
        xmlNodePtr childNode = node->children;
        while (childNode != NULL) {
            WSObject::extractObject(childNode, "obj", this->obj);

            ListUtil<DynamicProperty>::extractObjectIntoList(childNode,
                                "propSet", this->propset);
            childNode = childNode->next;
        }
    }




} /* namespace DcmNs */
