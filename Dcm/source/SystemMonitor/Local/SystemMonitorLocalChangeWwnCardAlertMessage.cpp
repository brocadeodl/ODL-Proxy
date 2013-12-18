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

#include "SystemMonitor/Local/SystemMonitorLocalObjectManager.h"
#include "SystemMonitor/Local/SystemMonitorLocalChangeWwnCardAlertMessage.h"
#include "SystemMonitor/Local/SystemMonitorLocalTypes.h"

namespace DcmNs
{

    SystemMonitorLocalChangeWwnCardAlertMessage::SystemMonitorLocalChangeWwnCardAlertMessage ()
        : PrismMessage (SystemMonitorLocalObjectManager::getPrismServiceId (), SYSTEMMONITORLOCALCHANGEWWNCARDALERT)
    {
    }

    SystemMonitorLocalChangeWwnCardAlertMessage::SystemMonitorLocalChangeWwnCardAlertMessage (const BitMap &state,const BitMap &action)
        : PrismMessage (SystemMonitorLocalObjectManager::getPrismServiceId (), SYSTEMMONITORLOCALCHANGEWWNCARDALERT),
        m_state    (state),
        m_action    (action)
    {
    }

    SystemMonitorLocalChangeWwnCardAlertMessage::~SystemMonitorLocalChangeWwnCardAlertMessage ()
    {
    }

    void  SystemMonitorLocalChangeWwnCardAlertMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeBitMap(&m_action,"action"));
        addSerializableAttribute (new AttributeBitMap(&m_state,"state"));
    }

    void  SystemMonitorLocalChangeWwnCardAlertMessage::setAction(const BitMap &action)
    {
        m_action  =  action;
    }

    BitMap  SystemMonitorLocalChangeWwnCardAlertMessage::getAction() const
    {
        return (m_action);
    }

    void  SystemMonitorLocalChangeWwnCardAlertMessage::setState(const BitMap &state)
    {
        m_state  =  state;
    }

    BitMap  SystemMonitorLocalChangeWwnCardAlertMessage::getState() const
    {
        return (m_state);
    }

}
