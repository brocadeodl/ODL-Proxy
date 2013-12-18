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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : vbagavat                                                     *
 **************************************************************************/

#include "LicenseService/Local/LicenseServiceLocalObjectManager.h"
#include "LicenseService/Local/LicenseServiceGetLicenseIdShowMessage.h"
#include "LicenseService/Local/LicenseServiceTypes.h"

namespace DcmNs
{

    LicenseServiceGetLicenseIdShowMessage::LicenseServiceGetLicenseIdShowMessage ()
        : PrismMessage (LicenseServiceLocalObjectManager::getPrismServiceId (), LICENSESERVICEGETLICENSEIDSHOW)
    {
    }

    LicenseServiceGetLicenseIdShowMessage::LicenseServiceGetLicenseIdShowMessage (const vector<WorldWideName> &licenseid)
        : PrismMessage (LicenseServiceLocalObjectManager::getPrismServiceId (), LICENSESERVICEGETLICENSEIDSHOW),
        m_licenseid    (licenseid)
    {
    }

    LicenseServiceGetLicenseIdShowMessage::~LicenseServiceGetLicenseIdShowMessage ()
    {
    }

    void  LicenseServiceGetLicenseIdShowMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeWorldWideNameVector(&m_licenseid,"licenseid"));
    }

    void  LicenseServiceGetLicenseIdShowMessage::setLicenseid(const vector<WorldWideName> &licenseid)
    {
        m_licenseid  =  licenseid;
    }

    vector<WorldWideName>  LicenseServiceGetLicenseIdShowMessage::getLicenseid() const
    {
        return (m_licenseid);
    }

}
