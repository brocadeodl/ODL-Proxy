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

#ifndef ABOUTINFO_H_
#define ABOUTINFO_H_
#include <string>
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include "discover/vsphere/vim25/WSObject.h"
using namespace std;
namespace DcmNs {

    class AboutInfo: public WSObject {
        public:
            AboutInfo();
            virtual ~AboutInfo();
            string getApiType() const;
        string getApiVersion() const;
        string getBuild() const;
        string getFullName() const;
        string getInstanceUuid() const;
        string getLicenseProductName() const;
        string getLicenseProductVersion() const;
        string getLocaleBuild() const;
        string getLocaleVersion() const;
        string getName() const;
        string getOsType() const;
        string getProductLineId() const;
        string getVendor() const;
        string getVersion() const;
        void setApiType(string apiType);
        void setApiVersion(string apiVersion);
        void setBuild(string build);
        void setFullName(string fullName);
        void setInstanceUuid(string instanceUuid);
        void setLicenseProductName(string licenseProductName);
        void setLicenseProductVersion(string licenseProductVersion);
        void setLocaleBuild(string localeBuild);
        void setLocaleVersion(string localeVersion);
        void setName(string name);
        void setOsType(string osType);
        void setProductLineId(string productLineId);
        void setVendor(string vendor);
        void setVersion(string version);
        string name;
            string fullName;
            string vendor;
            string version;
            string build;
            string localeVersion;
            string localeBuild;
            string osType;
            string productLineId;
            string apiType;
            string apiVersion;
            string instanceUuid;
            string licenseProductName;
            string licenseProductVersion;
            void deSerialize(xmlNodePtr node);
    };

} /* namespace DcmNs */
#endif /* ABOUTINFO_H_ */
