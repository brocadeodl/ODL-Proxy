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
#include "SystemMonitor/Local/SystemMonitorLocalChangeLineCardAlertMessage.h"
#include "SystemMonitor/Local/SystemMonitorLocalTypes.h"

namespace DcmNs
{

    SystemMonitorLocalChangeLineCardAlertMessage::SystemMonitorLocalChangeLineCardAlertMessage ()
        : PrismMessage (SystemMonitorLocalObjectManager::getPrismServiceId (), SYSTEMMONITORLOCALCHANGELINECARDALERT)
    {
    }

    SystemMonitorLocalChangeLineCardAlertMessage::SystemMonitorLocalChangeLineCardAlertMessage (const BitMap &state,const BitMap &action)
        : PrismMessage (SystemMonitorLocalObjectManager::getPrismServiceId (), SYSTEMMONITORLOCALCHANGELINECARDALERT),
        m_state    (state),
        m_action    (action)
    {
    }

    SystemMonitorLocalChangeLineCardAlertMessage::~SystemMonitorLocalChangeLineCardAlertMessage ()
    {
    }

    void  SystemMonitorLocalChangeLineCardAlertMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeBitMap(&m_action,"action"));
        addSerializableAttribute (new AttributeBitMap(&m_state,"state"));
    }

    void  SystemMonitorLocalChangeLineCardAlertMessage::setAction(const BitMap &action)
    {
        m_action  =  action;
    }

    BitMap  SystemMonitorLocalChangeLineCardAlertMessage::getAction() const
    {
        return (m_action);
    }

    void  SystemMonitorLocalChangeLineCardAlertMessage::setState(const BitMap &state)
    {
        m_state  =  state;
    }

    BitMap  SystemMonitorLocalChangeLineCardAlertMessage::getState() const
    {
        return (m_state);
    }

}
