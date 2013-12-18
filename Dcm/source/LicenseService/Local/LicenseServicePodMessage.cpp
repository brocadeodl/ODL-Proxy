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
#include "LicenseService/Local/LicenseServicePodMessage.h"
#include "LicenseService/Local/LicenseServiceTypes.h"

namespace DcmNs
{

    LicenseServicePodMessage::LicenseServicePodMessage ()
        : PrismMessage (LicenseServiceLocalObjectManager::getPrismServiceId (), LICENSESERVICEPOD)
    {
    }

    LicenseServicePodMessage::LicenseServicePodMessage (const string &option,const string &interface,const string &swSlotPort)
        : PrismMessage (LicenseServiceLocalObjectManager::getPrismServiceId (), LICENSESERVICEPOD),
        m_option    (option),
        m_interface    (interface),
        m_swSlotPort    (swSlotPort)
    {
    }

    LicenseServicePodMessage::~LicenseServicePodMessage ()
    {
    }

    void  LicenseServicePodMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_option,"option"));
        addSerializableAttribute (new AttributeString(&m_interface,"interface"));
        addSerializableAttribute (new AttributeString(&m_swSlotPort,"swSlotPort"));
    }

    void  LicenseServicePodMessage::setOption(const string &option)
    {
        m_option  =  option;
    }

    string  LicenseServicePodMessage::getOption() const
    {
        return (m_option);
    }

    void  LicenseServicePodMessage::setInterface(const string &interface)
    {
        m_interface  =  interface;
    }

    string  LicenseServicePodMessage::getInterface() const
    {
        return (m_interface);
    }

    void  LicenseServicePodMessage::setSwSlotPort(const string &swSlotPort)
    {
        m_swSlotPort  =  swSlotPort;
    }

    string  LicenseServicePodMessage::getSwSlotPort() const
    {
        return (m_swSlotPort);
    }

}
