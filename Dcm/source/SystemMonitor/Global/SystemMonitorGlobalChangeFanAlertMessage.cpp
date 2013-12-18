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

#include "SystemMonitor/Global/SystemMonitorGlobalChangeFanAlertMessage.h"
#include "SystemMonitor/Global/SystemMonitorGlobalObjectManager.h"
#include "SystemMonitor/Global/SystemMonitorGlobalTypes.h"

namespace DcmNs
{

    SystemMonitorGlobalChangeFanAlertMessage::SystemMonitorGlobalChangeFanAlertMessage ()
        : PrismMessage (SystemMonitorGlobalObjectManager::getPrismServiceId (), SYSTEMMONITORGLOBALCHANGEFANALERT)
    {
    }

    SystemMonitorGlobalChangeFanAlertMessage::SystemMonitorGlobalChangeFanAlertMessage (const UI32 &action,const UI32 &state)
        : PrismMessage (SystemMonitorGlobalObjectManager::getPrismServiceId (), SYSTEMMONITORGLOBALCHANGEFANALERT),
        m_action    (action),
        m_state    (state)
    {
    }

    SystemMonitorGlobalChangeFanAlertMessage::~SystemMonitorGlobalChangeFanAlertMessage ()
    {
    }

    void  SystemMonitorGlobalChangeFanAlertMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_action,"action"));
        addSerializableAttribute (new AttributeUI32(&m_state,"state"));
    }

    void  SystemMonitorGlobalChangeFanAlertMessage::setAction(const UI32 &action)
    {
        m_action  =  action;
    }

    UI32  SystemMonitorGlobalChangeFanAlertMessage::getAction() const
    {
        return (m_action);
    }

    void  SystemMonitorGlobalChangeFanAlertMessage::setState(const UI32 &state)
    {
        m_state  =  state;
    }

    UI32  SystemMonitorGlobalChangeFanAlertMessage::getState() const
    {
        return (m_state);
    }

}
