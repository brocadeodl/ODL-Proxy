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
 * XmlGen.h
 *
 *  Created on: Mar 22, 2012
 *      Author: rmadapur
 */

#ifndef XMLGEN_H_
#define XMLGEN_H_
#include <string>
#include "discover/vsphere/vim25/WSObject.h"
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>


#include <vector>
#include "discover/vsphere/VICError.h"

using namespace std;
namespace DcmNs {

    class XmlGen {
            xmlParserCtxtPtr pctxt;
            xmlDocPtr xml;
            xmlXPathContextPtr xpathContext;
            //xmlXPathContextPtr xpathContext;
        public:
            XmlGen();
            virtual ~XmlGen();
            string toXML(string tag, string type, void *obj);
            int  parseXML2(string response,string methodName,bool parseResponse,xmlNodePtr *_responseNode);

            void cleanup();


    };

} /* namespace DcmNs */
#endif /* XMLGEN_H_ */
