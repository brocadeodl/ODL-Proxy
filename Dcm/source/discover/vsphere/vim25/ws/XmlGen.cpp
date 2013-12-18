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
 * XmlGen.cpp
 *
 *  Created on: Mar 22, 2012
 *      Author: rmadapur
 */

#include "discover/vsphere/vim25/ws/XmlGen.h"
#include "discover/vsphere/Debug.h"
#include <string>
#include "discover/vsphere/vim25/ManagedObjectReference.h"
#include "discover/vsphere/vim25/PropertyFilterSpec.h"
#include "discover/vsphere/vim25/EventFilterSpec.h"

using namespace std;
namespace DcmNs {

    XmlGen::XmlGen() {
        // TODO Auto-generated constructor stub

    }

    XmlGen::~XmlGen() {
        // TODO Auto-generated destructor stub
    }
    xmlNodePtr getXPathNode(const char *xpath, xmlXPathContextPtr ctxt) {
        xmlXPathObjectPtr obj;
        xmlNodePtr relnode;
        xmlNodePtr ret;

        if ((ctxt == NULL) || (xpath == NULL)) {
            printf("Invalid Context or XPath");
            return (NULL);
        }
        relnode = ctxt->node;
        obj = xmlXPathEval(BAD_CAST xpath, ctxt);
        ctxt->node = relnode;
        if ((obj == NULL) || (obj->type != XPATH_NODESET)
                || (obj->nodesetval == NULL) || (obj->nodesetval->nodeNr <= 0)
                || (obj->nodesetval->nodeTab == NULL)) {
            xmlXPathFreeObject(obj);
            return (NULL);
        }

        ret = obj->nodesetval->nodeTab[0];
        xmlXPathFreeObject(obj);
        return (ret);
    }
    string XmlGen::toXML(string tagName, string type, void *obj) {
        string sb;
        //printf("%s %s\n", tagName.c_str(), type.c_str());
        if (type.compare("ManagedObjectReference") == 0) {
            ManagedObjectReference *mor = (ManagedObjectReference*) obj;
            sb.append("<" + tagName + "  type=\"" + mor->getType() + "\">");
            sb.append(mor->getVal());
            sb.append("</" + tagName + ">");
        }

        if (type.compare("String[]") == 0) {
            vector<string> array = *(vector<string> *) obj;
            for (uint32_t i = 0; i < array.size(); i++) {
                sb.append(toXML(tagName, "String", &array[i]));
            }
        }
        if (type.compare("String") == 0) {
            string str = *(string *) obj;
            sb.append("<" + tagName + "  type=\"" + "xsd:string" + "\">");
            sb.append(str);
            sb.append("</" + tagName + ">");
        }
        if (type.compare("int") == 0) {
                    string str = *(string *) obj;
                    sb.append("<" + tagName + "  type=\"" + "xsd:int" + "\">");
                    sb.append(str);
                    sb.append("</" + tagName + ">");
                }

        if (type.compare("boolean") == 0) {
            bool boolean = *(bool *) obj;
            sb.append("<" + tagName + "  type=\"" + "xsd:boolean" + "\">");
            sb.append(boolean == true ? "true" : "false");
            sb.append("</" + tagName + ">");
        }

        if (type.compare("PropertyFilterSpec[]") == 0) {
            vector<PropertyFilterSpec> filterSpecArray = *(vector<
                    PropertyFilterSpec> *) obj;
            for (uint32_t i = 0; i < filterSpecArray.size(); i++) {
                sb.append(
                        filterSpecArray[i].serialize(tagName,
                                "PropertyFilterSpec"));
            }

        }
        if (type.compare("PropertyFilterSpec") == 0) {

            PropertyFilterSpec propertyFilterSpec = *(PropertyFilterSpec *) obj;

            sb.append(
                    propertyFilterSpec.serialize(tagName,
                            "PropertyFilterSpec"));

        }
        if (type.compare("EventFilterSpec") == 0) {
            EventFilterSpec eventFilterSpec = *(EventFilterSpec *) obj;

            sb.append(eventFilterSpec.serialize(tagName, "EventFilterSpec"));
        }

        return sb;
    }

    /**
     * Parse the Soap XML Document
     * @param response XML Doc(string representation to be parsed)
     * @param methodName Name of the method that was invoked
     * @param parseResponse - bool (false if we need to parse fault)
     * @param _responseNode libxml parsed XML Node pointer
     */
    int XmlGen::parseXML2(string response, string methodName,
            bool parseRepsonse, xmlNodePtr *_responseNode) {
        pctxt = NULL;
        xml = NULL;

        /* Set up a parser context so we can catch the details of XML errors. */
        pctxt = xmlNewParserCtxt();
        if (!pctxt || !pctxt->sax) {
            VIC_PLUG_LOG(VIC_TRACE_ERROR, "%s","XML Parse Error");
            return VIC_WSCLIENT_CURL_XML_PARSE_ERROR;

        }

        xml = xmlCtxtReadDoc(pctxt, BAD_CAST response.c_str(), "temp.xml", NULL,
                XML_PARSE_NOENT | XML_PARSE_NONET | XML_PARSE_NOWARNING);
        xpathContext = NULL;
        xpathContext = xmlXPathNewContext(xml);
        xpathContext->node = xmlDocGetRootElement(xml);

        xmlXPathRegisterNs(xpathContext, BAD_CAST "soapenv",
                BAD_CAST "http://schemas.xmlsoap.org/soap/envelope/");
        xmlXPathRegisterNs(xpathContext, BAD_CAST "vim", BAD_CAST "urn:vim25");

        xmlNodePtr responseNode = NULL;
        if (parseRepsonse) {
            string xPathExpression = "/soapenv:Envelope/soapenv:Body/vim:"
                    + methodName + "Response";
            responseNode = getXPathNode(xPathExpression.c_str(), xpathContext);

            xpathContext->node = responseNode;
            responseNode = getXPathNode("./vim:returnval", xpathContext);

            /* if(responseNode==NULL){
             return VIC_WSCLIENT_CURL_EMPTY_RESPONSE_ERROR;
             }*/
        }
        else { //fault
            responseNode = getXPathNode(
                    "/soapenv:Envelope/soapenv:Body/soapenv:Fault",
                    xpathContext);
        }

        *_responseNode = responseNode;



        return VIC_WSCLIENT_SUCCESS;

    }


    void XmlGen::cleanup() {
        //cleanup
        if (pctxt)
            xmlFreeParserCtxt(pctxt);
        if (xml)
            xmlFreeDoc(xml);
        if (xpathContext)
            xmlXPathFreeContext(xpathContext);
    }

} /* namespace DcmNs */
