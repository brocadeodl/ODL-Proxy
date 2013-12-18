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
#include "LicenseService/Local/LicenseServiceUpdateLicenseFabsysInfoMessage.h"
#include "LicenseService/Local/LicenseServiceTypes.h"

namespace DcmNs
{

    LicenseServiceUpdateLicenseFabsysInfoMessage::LicenseServiceUpdateLicenseFabsysInfoMessage ()
        : ManagementInterfaceMessage (LicenseServiceLocalObjectManager::getClassName (),LICENSESERVICEUPDATELICENSEFABSYSINFO)
    {
    }

    LicenseServiceUpdateLicenseFabsysInfoMessage::LicenseServiceUpdateLicenseFabsysInfoMessage (const string &portId,const string &assignType)
        : ManagementInterfaceMessage (LicenseServiceLocalObjectManager::getClassName (),LICENSESERVICEUPDATELICENSEFABSYSINFO),
        m_portId    (portId),
        m_assignType    (assignType)
    {
    }

    LicenseServiceUpdateLicenseFabsysInfoMessage::~LicenseServiceUpdateLicenseFabsysInfoMessage ()
    {
    }

    void  LicenseServiceUpdateLicenseFabsysInfoMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_portId,"portId"));
        addSerializableAttribute (new AttributeString(&m_assignType,"assignType"));
    }

    void  LicenseServiceUpdateLicenseFabsysInfoMessage::setPortId(const string &portId)
    {
        m_portId  =  portId;
    }

    string  LicenseServiceUpdateLicenseFabsysInfoMessage::getPortId() const
    {
        return (m_portId);
    }

    void  LicenseServiceUpdateLicenseFabsysInfoMessage::setAssignType(const string &assignType)
    {
        m_assignType  =  assignType;
    }

    string  LicenseServiceUpdateLicenseFabsysInfoMessage::getAssignType() const
    {
        return (m_assignType);
    }

}
