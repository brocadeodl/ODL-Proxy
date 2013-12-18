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
 * WSClient.cpp
 *
 *  Created on: Mar 21, 2012
 *      Author: rmadapur
 */

#include "discover/vsphere/WSClient.h"

namespace DcmNs {

    WSClient::WSClient(string url,int connectionTimout) {
        lastFault = new Fault();
        header = NULL;
        header = curl_slist_append(header, "Content-Type:text/xml");
        header = curl_slist_append(header, "SOAPAction:urn:vim25/5.0");
        header = curl_slist_append(header, "Expect: nothing");

        curl = curl_easy_init();
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_USERAGENT, "5.0.0");
            curl_easy_setopt(curl, CURLOPT_HEADER, 0);
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 0);

            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
            curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "");
            //Set NOSIGNAL attribute for multi-threaded access.
            curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, connectionTimout);
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header);


            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

            curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);

        }
    }
    WSClient::~WSClient() {
        curl_slist_free_all(header);
        curl_easy_cleanup(curl);
        if(lastFault){
            delete(lastFault);
            lastFault = NULL;
        }
    }

    size_t WSClient::collectResponse(char *data, size_t size, size_t nmemb) {
        // What we will return
        int result = 0;

        // Append the data to the buffer
        buffer.append(data, size * nmemb);

        // How much did we write?
        result = size * nmemb;

        return result;

    }

    size_t WSClient::responseRouter(char *data, size_t size, size_t nmemb,
            void *f) {

        // Call non-static member function.
        //We collect the response in a member function rather than static function
        //so that the API can be used by multiple threads.
        return static_cast<WSClient*>(f)->collectResponse(data, size, nmemb);

    }

    int WSClient::invoke(string request, string &response) {

        buffer.clear();
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 0);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, request.length());

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WSClient::responseRouter);

        //curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);

        int responseCode = performOperation();
        if (responseCode < VIC_WSCLIENT_SUCCESS) {
            return responseCode;
        }

        response.clear();
        response.append(buffer);

        return responseCode;
    }

    int mapErrorCode(CURLcode curlCode){
        int   returCode = VIC_WSCLIENT_CURL_ERROR;
        switch(curlCode){
            case CURLE_COULDNT_CONNECT : returCode = VIC_WSCLIENT_CURL_CURLE_COULDNT_CONNECT;
            break;
            case CURLE_OPERATION_TIMEOUTED : returCode = VIC_WSCLIENT_CURL_CURLE_TIMEDOUT;
            break;
            default : break;

        }
        return returCode;
    }

    int WSClient::performOperation() {
        long responseCode = 0;
        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            VIC_PLUG_LOG(
                    VIC_TRACE_DEBUG,
                    "curl_easy_perform() returned an error: %s (%d) \n", curl_easy_strerror(res), res);
            return mapErrorCode(res);
        }

        res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &responseCode);

        if (res != CURLE_OK) {
            VIC_PLUG_LOG(VIC_TRACE_ERROR,
                    "curl_easy_getinfo(CURLINFO_RESPONSE_CODE) returned an "
                    "error: %s (%d) \n", curl_easy_strerror(res), res);
            return mapErrorCode(res);
        }
        if (responseCode < 0) {
            VIC_PLUG_LOG(
                    VIC_TRACE_ERROR,
                    "%s\n", ("curl_easy_getinfo(CURLINFO_RESPONSE_CODE) returned a "
                    "negative response code"));
            return VIC_WSCLIENT_CURL_NEGATIVE_RESPONSE_ERROR;
        }
        return responseCode;

    }
    string WSClient::createSoapMessage(string methodName,
            vector<Argument*> &paras) {
        string msg;
        XmlGen xmlgen;
        msg.append(VIC__SOAP__REQUEST_HEADER);
        msg.append("<" + methodName + " xmlns=\"urn:vim25\">");
        for (uint32_t i = 0; i < paras.size(); i++) {
            string key = paras[i]->getName();
            string type = paras[i]->getType();
            void *obj = paras[i]->getVal();
            msg.append(xmlgen.toXML(key, type, obj));
        }

        msg.append("</" + methodName + ">");
        msg.append(VIC__SOAP__REQUEST_FOOTER);
        return msg;
    }

    void WSClient::xmlCleanup(xmlNodePtr & responseNode, XmlGen & xmlgen) {

        xmlgen.cleanup();

    }
    int WSClient::invoke(string methodName, vector<Argument*> &paras,
            string returnType, void **r_object) {
        XmlGen xmlgen;
        int responseCode;
        string soapMsg = createSoapMessage(methodName, paras);
        string response;

        trace(TRACE_LEVEL_DEVEL, string("Request Message \n-----------\n" ) + soapMsg + string("\n-----------\n"));

        //VIC_PLUG_LOG(VIC_TRACE_DEBUG,
                //"Request Message = \n%s\n", soapMsg.c_str());
        responseCode = invoke(soapMsg, response);
        if (responseCode < VIC_WSCLIENT_SUCCESS) {
            VIC_PLUG_LOG(VIC_TRACE_DEBUG,
                           "Response Code = \n%d\n", responseCode);
            return responseCode;
        }

        trace(TRACE_LEVEL_DEVEL, string("Response Message \n-----------\n" ) + response + string("\n-----------\n"));
        //VIC_PLUG_LOG(VIC_TRACE_DEBUG,
                //"Response Message = \n%s\n", response.c_str());

        if (responseCode == 200) { //HTTP Response Code for Success Message
            xmlNodePtr responseNode = NULL;
            int xmlError = xmlgen.parseXML2(response, methodName, true,
                    &responseNode);

            if (xmlError < VIC_WSCLIENT_SUCCESS) {
                xmlCleanup(responseNode, xmlgen);
                return xmlError;
            }
            getObject(responseNode, returnType, r_object);
            xmlCleanup(responseNode, xmlgen);
            return VIC_WSCLIENT_SUCCESS;
        }
        else if (responseCode == 500) { //HTTP Response Code for Fault Message
            VIC_PLUG_LOG(VIC_TRACE_ERROR,"%s", "Fault Response Message");
            xmlNodePtr faultNode = NULL;
            int xmlError = xmlgen.parseXML2(response, methodName, false,
                    &faultNode);
            if (xmlError < VIC_WSCLIENT_SUCCESS) {
                xmlCleanup(faultNode, xmlgen);
                return xmlError;
            }
            lastFault->deSerialize(faultNode);
            xmlCleanup(faultNode, xmlgen);
            return VIC_WSCLIENT_CURL_FAULT_RESPONSE_ERROR;
        }

        return -1;
    }

    int WSClient::getObject(xmlNodePtr responseNode, string returnType,
            void **r_object) {
        if(returnType.compare("")==0){
            return 0;
        }else if (returnType.compare("ServiceContent") == 0) {

            XMLUtil::extractObject<ServiceContent>(responseNode, r_object);
            return 0;
        }
        else if (returnType.compare("UserSession") == 0) {
            XMLUtil::extractObject<UserSession>(responseNode, r_object);
            return 0;
        }
        else if (returnType.compare("ManagedObjectReference") == 0) {
            XMLUtil::extractObject<ManagedObjectReference>(responseNode,
                    r_object);

            return 0;
        }
        else if (returnType.compare("UpdateSet") == 0) {
            XMLUtil::extractObject<UpdateSet>(responseNode, r_object);
            return 0;
        }
        else if (returnType.compare("ObjectContent[]") == 0) {
            //check for responseNode needs to moved above.
            //responseNode can be null
            if (responseNode != NULL) {
                XMLUtil::extractList<ObjectContent>(responseNode->parent,
                        "returnval", r_object);
            }
            else {
                vector<ObjectContent *> *objContentList = new vector<
                        ObjectContent *>();
                *r_object = (void *) objContentList;
            }
            return 0;
        }
        else if (returnType.compare("Event[]") == 0) {
                    //check for responseNode needs to moved above.
                    //responseNode can be null
                    if (responseNode != NULL) {
                        XMLUtil::extractList<Event>(responseNode->parent, "returnval",
                                r_object);
                    }
                    else {
                        vector<Event *> *objContentList = new vector<Event *>();
                        *r_object = (void *) objContentList;
                    }
                    return 0;
                }
        return -1;
    }

} /* namespace DcmNs */
