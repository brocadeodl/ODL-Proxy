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
 *   Author : pahuja                                                     *
 **************************************************************************/

#include "SystemMonitor/Global/SystemMonitorGlobalChangeWwnCardAlertMessage.h"
#include "SystemMonitor/Global/SystemMonitorGlobalObjectManager.h"
#include "SystemMonitor/Global/SystemMonitorGlobalTypes.h"

namespace DcmNs
{

    SystemMonitorGlobalChangeWwnCardAlertMessage::SystemMonitorGlobalChangeWwnCardAlertMessage ()
        : PrismMessage (SystemMonitorGlobalObjectManager::getPrismServiceId (), SYSTEMMONITORGLOBALCHANGEWWNCARDALERT)
    {
    }

    SystemMonitorGlobalChangeWwnCardAlertMessage::SystemMonitorGlobalChangeWwnCardAlertMessage (const UI32 &action,const UI32 &state)
        : PrismMessage (SystemMonitorGlobalObjectManager::getPrismServiceId (), SYSTEMMONITORGLOBALCHANGEWWNCARDALERT),
        m_action    (action),
        m_state    (state)
    {
    }

    SystemMonitorGlobalChangeWwnCardAlertMessage::~SystemMonitorGlobalChangeWwnCardAlertMessage ()
    {
    }

    void  SystemMonitorGlobalChangeWwnCardAlertMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_action,"action"));
        addSerializableAttribute (new AttributeUI32(&m_state,"state"));
    }

    void  SystemMonitorGlobalChangeWwnCardAlertMessage::setAction(const UI32 &action)
    {
        m_action  =  action;
    }

    UI32  SystemMonitorGlobalChangeWwnCardAlertMessage::getAction() const
    {
        return (m_action);
    }

    void  SystemMonitorGlobalChangeWwnCardAlertMessage::setState(const UI32 &state)
    {
        m_state  =  state;
    }

    UI32  SystemMonitorGlobalChangeWwnCardAlertMessage::getState() const
    {
        return (m_state);
    }

}
