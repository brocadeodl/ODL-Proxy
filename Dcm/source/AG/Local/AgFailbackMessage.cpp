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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : ranantha                                                     *
 **************************************************************************/

#include "AG/Local/AgLocalObjectManager.h"
#include "AG/Local/AgFailbackMessage.h"
#include "AG/Local/AgLocalTypes.h"

namespace DcmNs
{
    AgFailbackMessage::AgFailbackMessage ()
        : ManagementInterfaceMessage (AgLocalObjectManager::getClassName (), AG_FAILBACK_MESSAGE)
    {
    }

    AgFailbackMessage::AgFailbackMessage (const string &Port, const bool &Enable)
        : ManagementInterfaceMessage (AgLocalObjectManager::getClassName(),
                                      AG_FAILBACK_MESSAGE),
        m_Port(Port),
        m_Enable(Enable)
    {
    }

    AgFailbackMessage::~AgFailbackMessage ()
    {
    }

    void  AgFailbackMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeBool(&m_Enable, "agFailback"));
    }

    void  AgFailbackMessage::setPort(const string &Port)
    {
        m_Port =  Port;
    }

    string  AgFailbackMessage::getPort() const
    {
        return (m_Port);
    }

    void  AgFailbackMessage::setEnable(const bool &Enable)
    {
        m_Enable =  Enable;
    }

    bool  AgFailbackMessage::getEnable() const
    {
        return (m_Enable);
    }

}
