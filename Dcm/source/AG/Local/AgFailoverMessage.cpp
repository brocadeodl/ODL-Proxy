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
 *   Author : ranantha                                                    *
 **************************************************************************/

#include "AG/Local/AgLocalObjectManager.h"
#include "AG/Local/AgFailoverMessage.h"
#include "AG/Local/AgLocalTypes.h"

namespace DcmNs
{
    AgFailoverMessage::AgFailoverMessage ()
        : ManagementInterfaceMessage (AgLocalObjectManager::getClassName (), AG_FAILOVER_MESSAGE)
    {
    }

    AgFailoverMessage::AgFailoverMessage (const string &Port, const bool &Enable)
        : ManagementInterfaceMessage (AgLocalObjectManager::getClassName(),
                                      AG_FAILOVER_MESSAGE),
        m_Port(Port),
        m_Enable(Enable)
    {
    }

    AgFailoverMessage::~AgFailoverMessage ()
    {
    }

    void  AgFailoverMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeBool(&m_Enable, "agFailover"));
    }

    void  AgFailoverMessage::setPort(const string &Port)
    {
        m_Port =  Port;
    }

    string  AgFailoverMessage::getPort() const
    {
        return (m_Port);
    }

    void  AgFailoverMessage::setEnable(const bool &Enable)
    {
        m_Enable =  Enable;
    }

    bool  AgFailoverMessage::getEnable() const
    {
        return (m_Enable);
    }

}
