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
 *   Author : vdharwad                                                     *
 **************************************************************************/

#include "AG/Local/AgLocalObjectManager.h"
#include "AG/Local/AgLocalConfigMessage.h"
#include "AG/Local/AgLocalTypes.h"

namespace DcmNs
{
	/* Empty constructor of AgConfigMessage */
    AgLocalConfigMessage::AgLocalConfigMessage ()
        : ManagementInterfaceMessage (AgLocalObjectManager::getClassName (), AG_LOCAL_CONFIG_MESSAGE)
    {
    }

	/* Constructor of AgConfigMessage  with one attribute agEnable */
    AgLocalConfigMessage::AgLocalConfigMessage (const bool &agEnable, const bool &reboot)
        : ManagementInterfaceMessage (AgLocalObjectManager::getClassName(),
                                      AG_LOCAL_CONFIG_MESSAGE),
        m_agEnable(agEnable),
        m_copyDefToStartUp(reboot)
    {
    }

	/* Destructor of AgConfigMessage */
    AgLocalConfigMessage::~AgLocalConfigMessage ()
    {
    }

    void  AgLocalConfigMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeBool(&m_agEnable,"agEnable"));
    }
	
	/* Set method for agEnable attribute */
    void  AgLocalConfigMessage::setEnable(const bool &agEnable)
    {
        m_agEnable =  agEnable;
    }

	/* Get method for agEnable attribute */
    bool  AgLocalConfigMessage::getEnable() const
    {
        return (m_agEnable);
    }

    bool  AgLocalConfigMessage::getCopyDefToStartUpFlag() const
    {
        return (m_copyDefToStartUp);
    }
}
