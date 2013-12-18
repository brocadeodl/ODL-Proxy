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
 * WSObject.h
 *
 *  Created on: Mar 22, 2012
 *      Author: rmadapur
 */

#ifndef WSOBJECT_H_
#define WSOBJECT_H_
#include "discover/vsphere/vim25/ws/XMLUtil.h"
#include <string>
#include "discover/vsphere/Debug.h"
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#define XML_EXTRACTOBJECT(X,Y,Z) if(XMLUtil::extractString(X, Y, Z)) {X=X->next;continue;}

using namespace std;
namespace DcmNs {

    class WSObject {
        public:
           // string type;
           // void * obj;
            WSObject();
           /* WSObject(string type,void *obj){
                this->type =type;
                this->obj = obj;
            }*/
            virtual ~WSObject();

            virtual string serialize(string tagName,string type);
            virtual void deSerialize(xmlNodePtr dyPropNode);
            static bool extractObject(xmlNodePtr & node, string key, WSObject *objectptr);
    };

} /* namespace DcmNs */
#endif /* WSOBJECT_H_ */
