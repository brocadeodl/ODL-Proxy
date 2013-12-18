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

#include "Rmon/Global/RmonGlobalDeleteRmonAlarmMessage.h"
#include "Rmon/Global/RmonGlobalObjectManager.h"
#include "Rmon/Global/RmonGlobalTypes.h"

namespace DcmNs
{

    RmonGlobalDeleteRmonAlarmMessage::RmonGlobalDeleteRmonAlarmMessage ()
        : ManagementInterfaceMessage (RmonGlobalObjectManager::getClassName (),RMONGLOBALDELETERMONALARM)
    {
    }

    RmonGlobalDeleteRmonAlarmMessage::RmonGlobalDeleteRmonAlarmMessage (const SI32 &alarmIndex)
        : ManagementInterfaceMessage (RmonGlobalObjectManager::getClassName (),RMONGLOBALDELETERMONALARM),
        m_alarmIndex    (alarmIndex)
    {
    }

    RmonGlobalDeleteRmonAlarmMessage::~RmonGlobalDeleteRmonAlarmMessage ()
    {
    }

    void  RmonGlobalDeleteRmonAlarmMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeSI32(&m_alarmIndex,"alarmIndex"));
    }

    void  RmonGlobalDeleteRmonAlarmMessage::setAlarmIndex(const SI32 &alarmIndex)
    {
        m_alarmIndex  =  alarmIndex;
    }

    SI32  RmonGlobalDeleteRmonAlarmMessage::getAlarmIndex() const
    {
        return (m_alarmIndex);
    }

}
