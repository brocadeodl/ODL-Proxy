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
 *   Author : jrodrigu                                                     *
 **************************************************************************/

#include "SwitchConfig/Local/SwitchConfigLocalObjectManager.h"
#include "SwitchConfig/Local/SwitchConfigCfgChassisStateMessage.h"
#include "SwitchConfig/Local/SwitchConfigTypes.h"

namespace DcmNs
{

    SwitchConfigCfgChassisStateMessage::SwitchConfigCfgChassisStateMessage ()
        : PrismMessage (SwitchConfigLocalObjectManager::getPrismServiceId (), SWITCHCONFIGCFGCHASSISSTATE)
    {
    }

    SwitchConfigCfgChassisStateMessage::SwitchConfigCfgChassisStateMessage (const bool &enableChassis, const bool &beacon)
        : PrismMessage (SwitchConfigLocalObjectManager::getPrismServiceId (), SWITCHCONFIGCFGCHASSISSTATE),
        m_enableChassis    (enableChassis),
	    m_beacon    (beacon)
    {
		m_enableChassis  =  enableChassis;
		m_beacon  =  beacon;
    }

    SwitchConfigCfgChassisStateMessage::~SwitchConfigCfgChassisStateMessage ()
    {
    }

    void  SwitchConfigCfgChassisStateMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeBool(&m_enableChassis,"enableChassis"));
        addSerializableAttribute (new AttributeBool(&m_beacon,"beacon"));
    }

    void  SwitchConfigCfgChassisStateMessage::setChassisEnableFlag(const bool &enableChassis)
    {
        m_enableChassis  =  enableChassis;
    }

    bool  SwitchConfigCfgChassisStateMessage::getChassisEnableFlag() const
    {
        return (m_enableChassis);
    }

    void  SwitchConfigCfgChassisStateMessage::setBeacon(const bool &beacon)
    {
        m_beacon  =  beacon;
    }

    bool  SwitchConfigCfgChassisStateMessage::getBeacon() const
    {
        return (m_beacon);
    }

    void  SwitchConfigCfgChassisStateMessage::setResult(const SI32 &result)
    {
        m_result  =  result;
    }

    SI32  SwitchConfigCfgChassisStateMessage::getResult() const
    {
        return (m_result);
    }


}
