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
 * WSClient.h
 *
 *  Created on: Mar 21, 2012
 *      Author: rmadapur
 */

#ifndef WSCLIENT_H_
#define WSCLIENT_H_
#include <string>
#include <curl/curl.h>
#include <iostream>
#include <sstream>
#include "discover/vsphere/vim25/ws/Argument.h"
#include "discover/vsphere/vim25/ws/XmlGen.h"
#include "discover/vsphere/vim25/ServiceContent.h"
#include "discover/vsphere/vim25/UserSession.h"
#include "discover/vsphere/vim25/ObjectContent.h"
#include "discover/vsphere/vim25/UpdateSet.h"
#include "discover/vsphere/vim25/Fault.h"
#include "discover/vsphere/VICError.h"
#include <vector>
using namespace std;

#define DEFAULT_CONNECTION_TIMEOUT      800
#define EVENT_CONNECTION_TIMEOUT        30
# define VIC__SOAP__REQUEST_HEADER                                         \
    "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"                            \
    "<soapenv:Envelope\n"                                                     \
    " xmlns:soapenv=\"http://schemas.xmlsoap.org/soap/envelope/\"\n"          \
    " xmlns:soapenc=\"http://schemas.xmlsoap.org/soap/encoding/\"\n"          \
    " xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\n"              \
    " xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\">\n"                      \
    "<soapenv:Body>\n"

# define VIC__SOAP__REQUEST_FOOTER                                         \
    "</soapenv:Body>\n"                                                       \
    "</soapenv:Envelope>"



namespace DcmNs {

    class WSClient {
        private:
            CURL *curl;
            struct curl_slist *header;
            CURLcode res;
            string buffer;


            int getObject(xmlNodePtr responseNode, string returnType,
                    void **r_object);
            int invoke(string request, string &response);
            int performOperation();

            static size_t responseRouter(char *data, size_t size, size_t nmemb,
                    void *f);
            size_t collectResponse(char *data, size_t size, size_t nmemb);

            string createSoapMessage(string methodName,
                    vector<Argument *> &paras);
        void xmlCleanup(xmlNodePtr & responseNode, XmlGen & xmlgen);

        public:
            /**
             * Reports the details of fault for the last operation as returned by vCenter
             */
            Fault *lastFault;
            /**
             * Constructor taking the vCenter URL
             * @param url - vCenter URL
             */
            WSClient(string url,int connectionTimeout);

            /**
             *  vSphere API
             *  vSpehere errors are reported as Fault
             *  @see Fault
             *
             *  @param methodName - vSphere Method Name for the Call.
             *  @param paras   - Parameters for the Call
             *  @param returType  - returType of the Object (Pass empty string if no return type
             *  is expected for e.g logout)
             *  @param r_object - Return object
             *
             *  @See vimStub for the usage of this method
             */
            int invoke(string methodName, vector<Argument *> &paras,
                    string returnType, void **r_object);

            virtual ~WSClient();

    };

} /* namespace DcmNs */
#endif /* WSCLIENT_H_ */
