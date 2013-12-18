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

#include "SystemMonitor/Global/SystemMonitorGlobalSfpMailSetCfgMessage.h"
#include "SystemMonitor/Global/SystemMonitorGlobalObjectManager.h"
#include "SystemMonitor/Global/SystemMonitorGlobalTypes.h"

namespace DcmNs
{

    SystemMonitorGlobalSfpMailSetCfgMessage::SystemMonitorGlobalSfpMailSetCfgMessage ()
        : PrismMessage (SystemMonitorGlobalObjectManager::getPrismServiceId (), SYSTEMMONITORGLOBALSFPMAILSETCFG)
    {
    }

    SystemMonitorGlobalSfpMailSetCfgMessage::SystemMonitorGlobalSfpMailSetCfgMessage (const bool &enableFlag)
        : PrismMessage (SystemMonitorGlobalObjectManager::getPrismServiceId (), SYSTEMMONITORGLOBALSFPMAILSETCFG),
        m_enableFlag    (enableFlag)
    {
    }

    SystemMonitorGlobalSfpMailSetCfgMessage::~SystemMonitorGlobalSfpMailSetCfgMessage ()
    {
    }

    void  SystemMonitorGlobalSfpMailSetCfgMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeBool(&m_enableFlag,"enableFlag"));
    }

    void  SystemMonitorGlobalSfpMailSetCfgMessage::setEnableFlag(const bool &enableFlag)
    {
        m_enableFlag  =  enableFlag;
    }

    bool  SystemMonitorGlobalSfpMailSetCfgMessage::getEnableFlag() const
    {
        return (m_enableFlag);
    }

}
