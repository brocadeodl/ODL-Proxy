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
 *   Author : pbalacha                                                     *
 **************************************************************************/

#include "Rmon/Local/RmonLocalObjectManager.h"
#include "Rmon/Local/RmonDeleteRmonAlarmSPMessage.h"
#include "Rmon/Local/RmonTypes.h"

namespace DcmNs
{

    RmonDeleteRmonAlarmSPMessage::RmonDeleteRmonAlarmSPMessage ()
        : PrismMessage (RmonLocalObjectManager::getPrismServiceId(),RMONDELETERMONALARMSP)
    {
    }

    RmonDeleteRmonAlarmSPMessage::RmonDeleteRmonAlarmSPMessage (const SI32 &alarmIndex)
        : PrismMessage (RmonLocalObjectManager::getPrismServiceId(),RMONDELETERMONALARMSP),
        m_alarmIndex    (alarmIndex)
    {
    }

    RmonDeleteRmonAlarmSPMessage::~RmonDeleteRmonAlarmSPMessage ()
    {
    }

    void  RmonDeleteRmonAlarmSPMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeSI32(&m_alarmIndex,"alarmIndex"));
    }

    void  RmonDeleteRmonAlarmSPMessage::setAlarmIndex(const SI32 &alarmIndex)
    {
        m_alarmIndex  =  alarmIndex;
    }

    SI32  RmonDeleteRmonAlarmSPMessage::getAlarmIndex() const
    {
        return (m_alarmIndex);
    }

}
