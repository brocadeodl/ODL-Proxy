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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : pahuja                                                       *
 **************************************************************************/

#include "SystemMonitor/Global/SystemMonitorGlobalSecMailAddEmailMessage.h"
#include "SystemMonitor/Global/SystemMonitorGlobalObjectManager.h"
#include "SystemMonitor/Global/SystemMonitorGlobalTypes.h"

namespace DcmNs
{

    SystemMonitorGlobalSecMailAddEmailMessage::SystemMonitorGlobalSecMailAddEmailMessage ()
        : PrismMessage (SystemMonitorGlobalObjectManager::getPrismServiceId (), SYSTEMMONITORGLOBALSECMAILADDEMAIL)
    {
    }

    SystemMonitorGlobalSecMailAddEmailMessage::SystemMonitorGlobalSecMailAddEmailMessage (const string &email)
        : PrismMessage (SystemMonitorGlobalObjectManager::getPrismServiceId (), SYSTEMMONITORGLOBALSECMAILADDEMAIL),
        m_email    (email)
    {
    }

    SystemMonitorGlobalSecMailAddEmailMessage::~SystemMonitorGlobalSecMailAddEmailMessage ()
    {
    }

    void  SystemMonitorGlobalSecMailAddEmailMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_email,"email"));
    }

    void  SystemMonitorGlobalSecMailAddEmailMessage::setEmail(const string &email)
    {
        m_email  =  email;
    }

    string  SystemMonitorGlobalSecMailAddEmailMessage::getEmail() const
    {
        return (m_email);
    }

}
