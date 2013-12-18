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
 *   Author : noku                                                     *
 **************************************************************************/

#include "SystemMonitor/Local/SystemMonitorLocalObjectManager.h"
#include "SystemMonitor/Local/SystemMonitorLocalFruMailSetEmailMessage.h"
#include "SystemMonitor/Local/SystemMonitorLocalTypes.h"

namespace DcmNs
{

    SystemMonitorLocalFruMailSetEmailMessage::SystemMonitorLocalFruMailSetEmailMessage ()
        : PrismMessage (SystemMonitorLocalObjectManager::getPrismServiceId (), SYSTEMMONITORLOCALFRUMAILSETEMAIL)
    {
    }

    SystemMonitorLocalFruMailSetEmailMessage::SystemMonitorLocalFruMailSetEmailMessage (const string &email)
        : PrismMessage (SystemMonitorLocalObjectManager::getPrismServiceId (), SYSTEMMONITORLOCALFRUMAILSETEMAIL),
        m_email    (email)
    {
    }

    SystemMonitorLocalFruMailSetEmailMessage::~SystemMonitorLocalFruMailSetEmailMessage ()
    {
    }

    void  SystemMonitorLocalFruMailSetEmailMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_email,"email"));
    }

    void  SystemMonitorLocalFruMailSetEmailMessage::setEmail(const string &email)
    {
        m_email  =  email;
    }

    string  SystemMonitorLocalFruMailSetEmailMessage::getEmail() const
    {
        return (m_email);
    }

}
