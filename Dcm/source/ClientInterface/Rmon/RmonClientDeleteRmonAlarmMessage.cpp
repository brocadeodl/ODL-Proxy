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

#include "ClientInterface/Rmon/RmonClientDeleteRmonAlarmMessage.h"
#include "ClientInterface/DceWaveClientMessage.h"

namespace DcmNs
{

    RmonClientDeleteRmonAlarmMessage::RmonClientDeleteRmonAlarmMessage ()
        : DcmManagementInterfaceMessage (RMONCLIENTDELETERMONALARM)
    {
    }

    RmonClientDeleteRmonAlarmMessage::RmonClientDeleteRmonAlarmMessage (const SI32 &alarmIndex)
        : DcmManagementInterfaceMessage (RMONCLIENTDELETERMONALARM),
        m_alarmIndex    (alarmIndex)
    {
    }

    RmonClientDeleteRmonAlarmMessage::~RmonClientDeleteRmonAlarmMessage ()
    {
    }

    void  RmonClientDeleteRmonAlarmMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeSI32(&m_alarmIndex,"alarmIndex"));
    }

    void  RmonClientDeleteRmonAlarmMessage::setAlarmIndex(const SI32 &alarmIndex)
    {
        m_alarmIndex  =  alarmIndex;
    }

    SI32  RmonClientDeleteRmonAlarmMessage::getAlarmIndex() const
    {
        return (m_alarmIndex);
    }

	const void *RmonClientDeleteRmonAlarmMessage::getCStructureForInputs ()
	{
		int *pInput = (int*)malloc(sizeof(int));
		*pInput = m_alarmIndex;
		return (pInput);
	 }

	 void RmonClientDeleteRmonAlarmMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	 {
	 }

}
