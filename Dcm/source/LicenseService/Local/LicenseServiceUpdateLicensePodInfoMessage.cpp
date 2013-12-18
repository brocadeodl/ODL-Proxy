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
#include "LicenseService/Local/LicenseServiceUpdateLicensePodInfoMessage.h"
#include "LicenseService/Local/LicenseServiceTypes.h"

namespace DcmNs
{

    LicenseServiceUpdateLicensePodInfoMessage::LicenseServiceUpdateLicensePodInfoMessage ()
        : PrismMessage (LicenseServiceLocalObjectManager::getPrismServiceId (), LICENSESERVICEUPDATELICENSEPODINFO)
    {
    }

    LicenseServiceUpdateLicensePodInfoMessage::LicenseServiceUpdateLicensePodInfoMessage (const string &portId,const string &assignType)
        : PrismMessage (LicenseServiceLocalObjectManager::getPrismServiceId (), LICENSESERVICEUPDATELICENSEPODINFO),
        m_portId    (portId),
        m_assignType    (assignType)
    {
    }

    LicenseServiceUpdateLicensePodInfoMessage::~LicenseServiceUpdateLicensePodInfoMessage ()
    {
    }

    void  LicenseServiceUpdateLicensePodInfoMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_portId,"portId"));
        addSerializableAttribute (new AttributeString(&m_assignType,"assignType"));
    }

    void  LicenseServiceUpdateLicensePodInfoMessage::setPortId(const string &portId)
    {
        m_portId  =  portId;
    }

    string  LicenseServiceUpdateLicensePodInfoMessage::getPortId() const
    {
        return (m_portId);
    }

    void  LicenseServiceUpdateLicensePodInfoMessage::setAssignType(const string &assignType)
    {
        m_assignType  =  assignType;
    }

    string  LicenseServiceUpdateLicensePodInfoMessage::getAssignType() const
    {
        return (m_assignType);
    }

}
