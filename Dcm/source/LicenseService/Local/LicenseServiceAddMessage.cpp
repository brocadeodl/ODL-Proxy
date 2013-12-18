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
#include "LicenseService/Local/LicenseServiceAddMessage.h"
#include "LicenseService/Local/LicenseServiceTypes.h"

namespace DcmNs
{

    LicenseServiceAddMessage::LicenseServiceAddMessage ()
        : PrismMessage (LicenseServiceLocalObjectManager::getPrismServiceId (), LICENSESERVICEADD)
    {
    }

    LicenseServiceAddMessage::LicenseServiceAddMessage (const string &msgStatus)
        : PrismMessage (LicenseServiceLocalObjectManager::getPrismServiceId (), LICENSESERVICEADD),
        m_msgStatus    (msgStatus)
    {
    }

    LicenseServiceAddMessage::~LicenseServiceAddMessage ()
    {
    }

    void  LicenseServiceAddMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_msgStatus,"msgStatus"));
    }

    void  LicenseServiceAddMessage::setMsgStatus(const string &msgStatus)
    {
        m_msgStatus  =  msgStatus;
    }

    string  LicenseServiceAddMessage::getMsgStatus() const
    {
        return (m_msgStatus);
    }

}
