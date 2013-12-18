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
 * vimStub.cpp
 *
 *  Created on: Mar 22, 2012
 *      Author: rmadapur
 */

#include "vimStub.h"
#include <vector>
#include <string>
#include <sstream>

using namespace std;

namespace DcmNs {

    vimStub::vimStub(string url,int connectionTimeout) {
        wsc = new WSClient(url,connectionTimeout);

    }

    vimStub::~vimStub() {
        if (wsc) {
            delete wsc;
            wsc = NULL;
        }
    }
    void cleanParameters(vector<Argument *> paras) {
        //Cleaning up the vector
        for (uint32_t i = 0; i < paras.size(); i++) {
            delete (paras[i]);
        }
    }
    int vimStub::retrieveServiceContent(ManagedObjectReference *_this,
            ServiceContent **r_serviceConten) {

        std::vector<Argument *> paras;
        paras.push_back(new Argument("_this", "ManagedObjectReference", _this));

        int returnStatus = wsc->invoke("RetrieveServiceContent", paras,
                "ServiceContent", (void**) r_serviceConten);

        cleanParameters(paras);
        return returnStatus;
    }

    int vimStub::login(ManagedObjectReference *_this, string userName,
            string password, UserSession **r_serviceConten) {

        std::vector<Argument *> paras;

        paras.push_back(new Argument("_this", "ManagedObjectReference", _this));
        paras.push_back(new Argument("userName", "String", &userName));
        paras.push_back(new Argument("password", "String", &password));

        int returnStatus = wsc->invoke("Login", paras, "UserSession",
                (void**) r_serviceConten);

        cleanParameters(paras);
        return returnStatus;
    }
    int vimStub::logout(ManagedObjectReference *_this) {

        std::vector<Argument *> paras;

        paras.push_back(new Argument("_this", "ManagedObjectReference", _this));

        int returnStatus = wsc->invoke("Logout", paras, "", NULL);

        cleanParameters(paras);
        return returnStatus;
    }

    int vimStub::createContainerView(ManagedObjectReference *_this,
            ManagedObjectReference *container, vector<string> *type,
            bool recursive, ManagedObjectReference **r_mor) {

        std::vector<Argument *> paras;
        paras.push_back(new Argument("_this", "ManagedObjectReference", _this));
        paras.push_back(
                new Argument("container", "ManagedObjectReference", container));
        paras.push_back(new Argument("type", "String[]", type));
        paras.push_back(new Argument("recursive", "boolean", &recursive));

        int returnStatus = wsc->invoke("CreateContainerView", paras,
                "ManagedObjectReference", (void**) r_mor);

        cleanParameters(paras);
        return returnStatus;
    }

    int vimStub::createCollectorForEvents(ManagedObjectReference *_this,
            EventFilterSpec *filter, ManagedObjectReference **r_mor) {

        std::vector<Argument *> paras;

        paras.push_back(new Argument("_this", "ManagedObjectReference", _this));
        paras.push_back(new Argument("filter", "EventFilterSpec", filter));

        int returnStatus = wsc->invoke("CreateCollectorForEvents", paras,
                "ManagedObjectReference", (void**) r_mor);

        cleanParameters(paras);
        return returnStatus;
    }

    int vimStub::createFilter(ManagedObjectReference *_this,
            PropertyFilterSpec *spec, bool partialUpdates,
            ManagedObjectReference **r_mor) {
        std::vector<Argument *> paras;
        paras.push_back(new Argument("_this", "ManagedObjectReference", _this));
        paras.push_back(new Argument("spec", "PropertyFilterSpec", spec));
        paras.push_back(
                new Argument("partialUpdates", "boolean", &partialUpdates));

        int returnStatus = wsc->invoke("CreateFilter", paras,
                "ManagedObjectReference", (void**) r_mor);

        cleanParameters(paras);
        return returnStatus;
    }
    int vimStub::destroyPropertyFilter(ManagedObjectReference *_this) {
        std::vector<Argument *> paras;

        paras.push_back(new Argument("_this", "ManagedObjectReference", _this));

        int returnStatus = wsc->invoke("DestroyPropertyFilter", paras, "",
                NULL);

        cleanParameters(paras);
        return returnStatus;
    }
    int vimStub::cancelWaitForUpdates(ManagedObjectReference *_this) {
        std::vector<Argument *> paras;

        paras.push_back(new Argument("_this", "ManagedObjectReference", _this));

        int returnStatus = wsc->invoke("CancelWaitForUpdates", paras, "", NULL);

        cleanParameters(paras);
        return returnStatus;
    }

    int vimStub::waitForUpdates(ManagedObjectReference *_this, string version,
            UpdateSet **r_mor) {

        std::vector<Argument *> paras;
        paras.push_back(new Argument("_this", "ManagedObjectReference", _this));
        paras.push_back(new Argument("version", "String", &version));

        int returnStatus = wsc->invoke("WaitForUpdates", paras, "UpdateSet",
                (void**) r_mor);

        cleanParameters(paras);
        return returnStatus;
    }

    int vimStub::retrieveProperties(ManagedObjectReference *_this,
            vector<PropertyFilterSpec> *specSet,
            vector<ObjectContent *> **r_mor) {
        std::vector<Argument *> paras;
        paras.push_back(new Argument("_this", "ManagedObjectReference", _this));
        paras.push_back(
                new Argument("specSet", "PropertyFilterSpec[]", specSet));

        int returnStatus = wsc->invoke("RetrieveProperties", paras,
                "ObjectContent[]", (void**) r_mor);

        cleanParameters(paras);
        return returnStatus;
    }

    int vimStub::destroyCollector(ManagedObjectReference *_this) {
        std::vector<Argument *> paras;

        paras.push_back(new Argument("_this", "ManagedObjectReference", _this));

        int returnStatus = wsc->invoke("DestroyCollector", paras, "", NULL);

        cleanParameters(paras);
        return returnStatus;
    }
    int vimStub::resetCollector(ManagedObjectReference *_this) {
        std::vector<Argument *> paras;

        paras.push_back(new Argument("_this", "ManagedObjectReference", _this));

        int returnStatus = wsc->invoke("ResetCollector", paras, "", NULL);

        cleanParameters(paras);
        return returnStatus;
    }
    int vimStub::rewindCollector(ManagedObjectReference *_this) {
        std::vector<Argument *> paras;

        paras.push_back(new Argument("_this", "ManagedObjectReference", _this));

        int returnStatus = wsc->invoke("RewindCollector", paras, "", NULL);

        cleanParameters(paras);
        return returnStatus;
    }
    int vimStub::setCollectorPageSize(ManagedObjectReference *_this,
            int maxCount) {
        std::vector<Argument *> paras;
        ostringstream convert;
        convert << maxCount;
        string maxCountStr = convert.str();
        paras.push_back(new Argument("_this", "ManagedObjectReference", _this));
        paras.push_back(new Argument("maxCount", "int", &maxCountStr));

        int returnStatus = wsc->invoke("SetCollectorPageSize", paras, "", NULL);

        cleanParameters(paras);
        return returnStatus;
    }

    int vimStub::readNextEvents(ManagedObjectReference *_this, int maxCount,
            vector<Event *> **r_mor) {
        std::vector<Argument *> paras;
        ostringstream convert;
        convert << maxCount;
        string maxCountStr = convert.str();
        paras.push_back(new Argument("_this", "ManagedObjectReference", _this));
        paras.push_back(new Argument("maxCount", "int", &maxCountStr));
        int returnStatus = wsc->invoke("ReadNextEvents", paras, "Event[]",
                (void**) r_mor);
        cleanParameters(paras);
        return returnStatus;
    }

    int vimStub::readPreviousEvents(ManagedObjectReference *_this, int maxCount,
            vector<Event *> **r_mor) {
        std::vector<Argument *> paras;
        ostringstream convert;
        convert << maxCount;
        string maxCountStr = convert.str();
        paras.push_back(new Argument("_this", "ManagedObjectReference", _this));
        paras.push_back(new Argument("maxCount", "int", &maxCountStr));
        int returnStatus = wsc->invoke("ReadPreviousEvents", paras, "Event[]",
                (void**) r_mor);

        cleanParameters(paras);
        return returnStatus;
    }

} /* namespace DcmNs */
