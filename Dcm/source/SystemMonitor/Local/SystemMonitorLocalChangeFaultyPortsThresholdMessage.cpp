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
#include "SystemMonitor/Local/SystemMonitorLocalChangeFaultyPortsThresholdMessage.h"
#include "SystemMonitor/Local/SystemMonitorLocalTypes.h"

namespace DcmNs
{

    SystemMonitorLocalChangeFaultyPortsThresholdMessage::SystemMonitorLocalChangeFaultyPortsThresholdMessage ()
        : PrismMessage (SystemMonitorLocalObjectManager::getPrismServiceId (), SYSTEMMONITORLOCALCHANGEFAULTYPORTSTHRESHOLD)
    {
    }

    SystemMonitorLocalChangeFaultyPortsThresholdMessage::SystemMonitorLocalChangeFaultyPortsThresholdMessage (const UI32 &downthreshold,const UI32 &marginalthreshold)
        : PrismMessage (SystemMonitorLocalObjectManager::getPrismServiceId (), SYSTEMMONITORLOCALCHANGEFAULTYPORTSTHRESHOLD),
        m_downthreshold    (downthreshold),
        m_marginalthreshold    (marginalthreshold)
    {
    }

    SystemMonitorLocalChangeFaultyPortsThresholdMessage::~SystemMonitorLocalChangeFaultyPortsThresholdMessage ()
    {
    }

    void  SystemMonitorLocalChangeFaultyPortsThresholdMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_downthreshold,"downthreshold"));
        addSerializableAttribute (new AttributeUI32(&m_marginalthreshold,"marginalthreshold"));
    }

    void  SystemMonitorLocalChangeFaultyPortsThresholdMessage::setDownthreshold(const UI32 &downthreshold)
    {
        m_downthreshold  =  downthreshold;
    }

    UI32  SystemMonitorLocalChangeFaultyPortsThresholdMessage::getDownthreshold() const
    {
        return (m_downthreshold);
    }

    void  SystemMonitorLocalChangeFaultyPortsThresholdMessage::setMarginalthreshold(const UI32 &marginalthreshold)
    {
        m_marginalthreshold  =  marginalthreshold;
    }

    UI32  SystemMonitorLocalChangeFaultyPortsThresholdMessage::getMarginalthreshold() const
    {
        return (m_marginalthreshold);
    }

}
