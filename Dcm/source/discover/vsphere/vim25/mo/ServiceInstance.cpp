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
 * ServiceInstance.cpp
 *
 *  Created on: Apr 12, 2012
 *      Author: rmadapur
 */

#include "ServiceInstance.h"

namespace DcmNs {

    ServiceInstance::ServiceInstance(string _url,int connectionTimeout) : ManagedObject(
            NULL, NULL), userSession(NULL), url(_url+"/sdk"), serviceContent(NULL){
        SERVICE_INSTANCE_MOR.type = "ServiceInstance";
        SERVICE_INSTANCE_MOR.val = "ServiceInstance";
        stub = new vimStub(url,connectionTimeout);

    }
    void checkAndPrintError(int vcRes, vimStub *stub) {
        if (vcRes == VIC_WSCLIENT_CURL_FAULT_RESPONSE_ERROR) {
            printf("Fault Error String %s\n",
                    stub->getLastFault()->faultstring.c_str());
        }
    }

    int ServiceInstance::login(string userName, string password) {

        serviceContent = NULL;
        int code = retrieveServiceContent();
        if (code != VIC_WSCLIENT_SUCCESS) {
            return code;
        }
        userSession = NULL;
        code = stub->login(&serviceContent->sessionManager, userName, password,
                &userSession);
        if (code != VIC_WSCLIENT_SUCCESS) {
            checkAndPrintError(code, stub);
            printf("Failed to execute Login\n");
            return code;
        }

        return VIC_WSCLIENT_SUCCESS;
    }

    Fault* ServiceInstance::getLastFault() {
       return stub->getLastFault();
    }
    int ServiceInstance::logout() {

        int code = stub->logout(&serviceContent->sessionManager);
        if (code != VIC_WSCLIENT_SUCCESS) {
            checkAndPrintError(code, stub);
            printf("Failed to execute Logout\n");
            return code;
        }

        return VIC_WSCLIENT_SUCCESS;
    }

    int ServiceInstance::getServiceContent(ServiceContent **r_serviceContent) {
        if (serviceContent == NULL) {
            int code = retrieveServiceContent();
            if (code != VIC_WSCLIENT_SUCCESS) {
                return code;
            }
        }
        *r_serviceContent = serviceContent;
        return VIC_WSCLIENT_SUCCESS;
    }
    int ServiceInstance::retrieveServiceContent() {
        int code = stub->retrieveServiceContent(&SERVICE_INSTANCE_MOR,
                &serviceContent);
        if (code != VIC_WSCLIENT_SUCCESS) {
            checkAndPrintError(code, stub);
            printf("Failed to execute RetrieveServiceContent\n");
            return code;
        }
        return VIC_WSCLIENT_SUCCESS;
    }

    int ServiceInstance::getPropertyCollector(
            PropertyCollector **propCollector) {
        if (serviceContent != NULL) {
            *propCollector = new PropertyCollector(stub,
                    &serviceContent->propertyCollector);
            return VIC_WSCLIENT_SUCCESS;
        }
        return -1;
    }

    int ServiceInstance::getViewManager(ViewManager **viewmanager) {
        if (serviceContent != NULL) {
            *viewmanager = new ViewManager(stub,
                    &serviceContent->viewManager);
            return VIC_WSCLIENT_SUCCESS;
        }
        return -1;
    }
    int ServiceInstance::getEventManager(EventManager **eventManager) {
            if (serviceContent != NULL) {
                *eventManager = new EventManager(stub,
                        &serviceContent->eventManager);
                return VIC_WSCLIENT_SUCCESS;
            }
            return -1;
        }

    ServiceInstance::~ServiceInstance() {
        if (serviceContent) {
            delete (serviceContent);
            serviceContent = NULL;
        }
        if (userSession) {
            delete (userSession);
            userSession = NULL;
        }
        if (stub) {
            delete (stub);
            stub = NULL;
        }

    }

} /* namespace DcmNs */
