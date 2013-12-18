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

/***************************************************************************
 *   Copyright (C) 2010-2015 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : rmadapur                                                     *
 **************************************************************************/
#include "AboutInfo.h"

namespace DcmNs {

    AboutInfo::AboutInfo() {
        // TODO Auto-generated constructor stub

    }

    AboutInfo::~AboutInfo() {
        // TODO Auto-generated destructor stub
    }

    string AboutInfo::getApiType() const {
        return apiType;
    }

    string AboutInfo::getApiVersion() const {
        return apiVersion;
    }

    string AboutInfo::getBuild() const {
        return build;
    }

    string AboutInfo::getFullName() const {
        return fullName;
    }

    string AboutInfo::getInstanceUuid() const {
        return instanceUuid;
    }

    string AboutInfo::getLicenseProductName() const {
        return licenseProductName;
    }

    string AboutInfo::getLicenseProductVersion() const {
        return licenseProductVersion;
    }

    string AboutInfo::getLocaleBuild() const {
        return localeBuild;
    }

    string AboutInfo::getLocaleVersion() const {
        return localeVersion;
    }

    string AboutInfo::getName() const {
        return name;
    }

    string AboutInfo::getOsType() const {
        return osType;
    }

    string AboutInfo::getProductLineId() const {
        return productLineId;
    }

    string AboutInfo::getVendor() const {
        return vendor;
    }

    string AboutInfo::getVersion() const {
        return version;
    }

    void AboutInfo::setApiType(string _apiType) {
        this->apiType = _apiType;
    }

    void AboutInfo::setApiVersion(string _apiVersion) {
        this->apiVersion = _apiVersion;
    }

    void AboutInfo::setBuild(string _build) {
        this->build = _build;
    }

    void AboutInfo::setFullName(string _fullName) {
        this->fullName = _fullName;
    }

    void AboutInfo::setInstanceUuid(string _instanceUuid) {
        this->instanceUuid = _instanceUuid;
    }

    void AboutInfo::setLicenseProductName(string _licenseProductName) {
        this->licenseProductName = _licenseProductName;
    }

    void AboutInfo::setLicenseProductVersion(string _licenseProductVersion) {
        this->licenseProductVersion = _licenseProductVersion;
    }

    void AboutInfo::setLocaleBuild(string _localeBuild) {
        this->localeBuild = _localeBuild;
    }

    void AboutInfo::setLocaleVersion(string _localeVersion) {
        this->localeVersion = _localeVersion;
    }

    void AboutInfo::setName(string _name) {
        this->name = _name;
    }

    void AboutInfo::setOsType(string _osType) {
        this->osType = _osType;
    }

    void AboutInfo::setProductLineId(string _productLineId) {
        this->productLineId = _productLineId;
    }

    void AboutInfo::setVendor(string _vendor) {
        this->vendor = _vendor;
    }

    void AboutInfo::setVersion(string _version) {
        this->version = _version;
    }

    void AboutInfo::deSerialize(xmlNodePtr node) {
           xmlNodePtr childNode = node->children;
           while (childNode != NULL) {
               XMLUtil::extractString(childNode, "version", this->version);
               XMLUtil::extractString(childNode, "apiVersion", this->apiVersion);
               //More can be done if required
               childNode = childNode->next;
           }
       }

} /* namespace DcmNs */
