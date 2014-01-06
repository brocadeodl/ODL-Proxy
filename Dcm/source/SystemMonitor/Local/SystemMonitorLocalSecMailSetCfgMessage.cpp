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
#include "SystemMonitor/Local/SystemMonitorLocalSecMailSetCfgMessage.h"
#include "SystemMonitor/Local/SystemMonitorLocalTypes.h"

namespace DcmNs
{

    SystemMonitorLocalSecMailSetCfgMessage::SystemMonitorLocalSecMailSetCfgMessage ()
        : PrismMessage (SystemMonitorLocalObjectManager::getPrismServiceId (), SYSTEMMONITORLOCALSECMAILSETCFG)
    {
    }

    SystemMonitorLocalSecMailSetCfgMessage::SystemMonitorLocalSecMailSetCfgMessage (const bool &enableFlag)
        : PrismMessage (SystemMonitorLocalObjectManager::getPrismServiceId (), SYSTEMMONITORLOCALSECMAILSETCFG),
        m_enableFlag    (enableFlag)
    {
    }

    SystemMonitorLocalSecMailSetCfgMessage::~SystemMonitorLocalSecMailSetCfgMessage ()
    {
    }

    void  SystemMonitorLocalSecMailSetCfgMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeBool(&m_enableFlag,"enableFlag"));
    }

    void  SystemMonitorLocalSecMailSetCfgMessage::setEnableFlag(const bool &enableFlag)
    {
        m_enableFlag  =  enableFlag;
    }

    bool  SystemMonitorLocalSecMailSetCfgMessage::getEnableFlag() const
    {
        return (m_enableFlag);
    }

}